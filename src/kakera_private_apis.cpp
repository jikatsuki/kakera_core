#include "kakera_private_apis.h"

using namespace std;

void kakera_private::PushRefreshEvent(kakera_Scene* scene)
{
    if (!scene->isRefresh)
        scene->isRefresh = true;
}

void kakera_private::PushRefreshEvent(kakera_Scene* scene, RefreshType type, RefreshInfo& info)
{
    if (!scene->isRefresh)
        scene->isRefresh = true;
    SDL_Rect* refreshRect;
    switch (type)
    {
    case RefreshType::Refresh_Unmoved:
    {        
        refreshRect = new SDL_Rect;
        memmove(refreshRect, &info.unmoved.refreshArea, sizeof(SDL_Rect));
    }
    break;
    case RefreshType::Refresh_Moved:
    {
        refreshRect = kakera_private::Unite2Rect(&info.moved.oldPosition, &info.moved.newPosition);
    }
    break;
    case RefreshType::Refresh_Rotate:
    {
        int xc = info.rotate.positionAndSize.x + info.rotate.positionAndSize.w / 2;
        int yc = info.rotate.positionAndSize.y + info.rotate.positionAndSize.h / 2;
        int d = sqrt(info.rotate.positionAndSize.w * info.rotate.positionAndSize.w + info.rotate.positionAndSize.h * info.rotate.positionAndSize.h);
        refreshRect = new SDL_Rect({
            xc - d / 2,
            yc - d / 2,
            d,
            d
        });        
    }
    break;
    }
    if (scene->dirtyArea != nullptr)
    {
        SDL_Rect* newDirtyArea = kakera_private::Unite2Rect(scene->dirtyArea, refreshRect);
        delete scene->dirtyArea;
        scene->dirtyArea = newDirtyArea;
    }
    else
    {
        scene->dirtyArea = refreshRect;
    }
}

void kakera_private::DestroyElementWithoutRefreshingList(kakera_Element ** element)
{
    kakera_private::CheckNullPointer(*element);
    kakera_private::RunCallback((*element), KAKERA_ELEMENT_ON_DESTROY);
    (*element)->callbackList.clear();
    (*element)->dataList.clear();
    delete (*element)->renderInfo.positionAndSize;
    delete (*element)->renderInfo.cropArea;
    delete *element;
    *element = nullptr;
}

void kakera_private::RefreshAll(kakera_Window * window)
{
    SDL_RenderClear(window->renderer);
    if (window->activeScene != nullptr)
    {
        for (auto element : *window->activeScene->elementList)
        {
            switch (element->reference)
            {
            case KAKERA_POSREFER_PARENT:
            {
                if (element->node == window->activeScene->elementTree.GetRoot())
                {
                    int window_w, window_h;
                    kakera_GetWindowSize(window, &window_w, &window_h);
                    element->position.x = 0;
                    element->position.y = 0;
                    element->displaySize.w = window_w;
                    element->displaySize.h = window_h;
                    element->renderInfo.positionAndSize->x = 0;
                    element->renderInfo.positionAndSize->y = 0;
                    element->renderInfo.positionAndSize->w = window_w;
                    element->renderInfo.positionAndSize->h = window_h;
                }
                else
                {
                    SDL_Rect viewport = {
                        element->node->parent->data->viewport.x,
                        element->node->parent->data->viewport.y,
                        element->node->parent->data->displaySize.w,
                        element->node->parent->data->displaySize.h
                    };
                    SDL_Rect elementPositionAndSize = {
                        element->position.x,
                        element->position.y,
                        element->displaySize.w,
                        element->displaySize.h
                    };
                    if (kakera_private::Is2RectIntersected(&viewport, &elementPositionAndSize))
                    {
                        if (!element->renderInfo.isRender)
                        {
                            element->renderInfo.isRender = true;
                        }

                        if (!element->isRotating)
                        {
                            auto result = kakera_private::Get2RectIntersection(&viewport, &elementPositionAndSize);
                            memmove(element->renderInfo.positionAndSize, result, sizeof(SDL_Rect));
                            delete result;
                            if (element->renderInfo.positionAndSize->w < element->displaySize.w ||
                                element->renderInfo.positionAndSize->h < element->displaySize.h)
                            {
                                element->renderInfo.cropArea->x = (static_cast<float>(element->realSize.w) / static_cast<float>(element->displaySize.w)) * (element->displaySize.w - element->renderInfo.positionAndSize->w);
                                element->renderInfo.cropArea->y = (static_cast<float>(element->realSize.h) / static_cast<float>(element->displaySize.h)) * (element->displaySize.h - element->renderInfo.positionAndSize->h);
                                element->renderInfo.cropArea->w = (static_cast<float>(element->renderInfo.positionAndSize->w) / static_cast<float>(element->displaySize.w)) * element->realSize.w;
                                element->renderInfo.cropArea->h = (static_cast<float>(element->renderInfo.positionAndSize->h) / static_cast<float>(element->displaySize.h)) * element->realSize.h;
                            }
                            else
                            {
                                element->renderInfo.cropArea->x = 0;
                                element->renderInfo.cropArea->y = 0;
                                element->renderInfo.cropArea->w = element->realSize.w;
                                element->renderInfo.cropArea->h = element->realSize.h;
                            }
                        }
                        else
                        {
                            memmove(element->renderInfo.positionAndSize, &elementPositionAndSize, sizeof(SDL_Rect));
                            element->renderInfo.cropArea->x = 0;
                            element->renderInfo.cropArea->y = 0;
                            element->renderInfo.cropArea->w = element->realSize.w;
                            element->renderInfo.cropArea->h = element->realSize.h;
                        }
                    }
                    else
                    {
                        element->renderInfo.isRender = false;
                    }
                }
                break;
            }
            case KAKERA_POSREFER_WINDOW:
            {
                element->renderInfo.positionAndSize->x = element->position.x;
                element->renderInfo.positionAndSize->y = element->position.y;
                element->renderInfo.positionAndSize->w = element->displaySize.w;
                element->renderInfo.positionAndSize->h = element->displaySize.h;
                element->renderInfo.cropArea->x = 0;
                element->renderInfo.cropArea->y = 0;
                element->renderInfo.cropArea->w = element->realSize.w;
                element->renderInfo.cropArea->h = element->realSize.h;
                break;
            }
            }
            if (element->renderInfo.isRender && element->texture != nullptr)
            {
                SDL_RenderCopyEx(
                    window->renderer,
                    element->texture,
                    element->renderInfo.cropArea,
                    element->renderInfo.positionAndSize,
                    element->rotateAngle,
                    NULL,
                    SDL_FLIP_NONE
                );
            }
        }
    }
    SDL_RenderPresent(window->renderer);
}

void kakera_private::RefreshRect(kakera_Window * window, SDL_Rect* rect)
{
    SDL_RenderFillRect(window->renderer, rect);
    if (window->activeScene != nullptr)
    {
        for (auto element : *window->activeScene->elementList)
        {
            SDL_Rect eleRect = {
                element->position.x,
                element->position.y,
                element->displaySize.w,
                element->displaySize.h
            };
            if (Is2RectIntersected(&eleRect, rect))
            {
                switch (element->reference)
                {
                case KAKERA_POSREFER_PARENT:
                {
                    if (element->node == window->activeScene->elementTree.GetRoot())
                    {
                        int window_w, window_h;
                        kakera_GetWindowSize(window, &window_w, &window_h);
                        element->position.x = 0;
                        element->position.y = 0;
                        element->displaySize.w = window_w;
                        element->displaySize.h = window_h;
                        element->renderInfo.positionAndSize->x = 0;
                        element->renderInfo.positionAndSize->y = 0;
                        element->renderInfo.positionAndSize->w = window_w;
                        element->renderInfo.positionAndSize->h = window_h;
                    }
                    else
                    {
                        SDL_Rect viewport = {
                            element->node->parent->data->viewport.x,
                            element->node->parent->data->viewport.y,
                            element->node->parent->data->displaySize.w,
                            element->node->parent->data->displaySize.h
                        };
                        SDL_Rect elementPositionAndSize = {
                            element->position.x,
                            element->position.y,
                            element->displaySize.w,
                            element->displaySize.h
                        };
                        if (kakera_private::Is2RectIntersected(&viewport, &elementPositionAndSize))
                        {
                            if (!element->renderInfo.isRender)
                            {
                                element->renderInfo.isRender = true;
                            }

                            if (!element->isRotating)
                            {
                                auto result = kakera_private::Get2RectIntersection(&viewport, &elementPositionAndSize);
                                memmove(element->renderInfo.positionAndSize, result, sizeof(SDL_Rect));
                                delete result;
                                if (element->renderInfo.positionAndSize->w < element->displaySize.w ||
                                    element->renderInfo.positionAndSize->h < element->displaySize.h)
                                {
                                    element->renderInfo.cropArea->x = (static_cast<float>(element->realSize.w) / static_cast<float>(element->displaySize.w)) * (element->displaySize.w - element->renderInfo.positionAndSize->w);
                                    element->renderInfo.cropArea->y = (static_cast<float>(element->realSize.h) / static_cast<float>(element->displaySize.h)) * (element->displaySize.h - element->renderInfo.positionAndSize->h);
                                    element->renderInfo.cropArea->w = (static_cast<float>(element->renderInfo.positionAndSize->w) / static_cast<float>(element->displaySize.w)) * element->realSize.w;
                                    element->renderInfo.cropArea->h = (static_cast<float>(element->renderInfo.positionAndSize->h) / static_cast<float>(element->displaySize.h)) * element->realSize.h;
                                }
                                else
                                {
                                    element->renderInfo.cropArea->x = 0;
                                    element->renderInfo.cropArea->y = 0;
                                    element->renderInfo.cropArea->w = element->realSize.w;
                                    element->renderInfo.cropArea->h = element->realSize.h;
                                }
                            }
                            else
                            {
                                memmove(element->renderInfo.positionAndSize, &elementPositionAndSize, sizeof(SDL_Rect));
                                element->renderInfo.cropArea->x = 0;
                                element->renderInfo.cropArea->y = 0;
                                element->renderInfo.cropArea->w = element->realSize.w;
                                element->renderInfo.cropArea->h = element->realSize.h;
                            }
                        }
                        else
                        {
                            element->renderInfo.isRender = false;
                        }
                    }
                    break;
                }
                case KAKERA_POSREFER_WINDOW:
                {
                    element->renderInfo.positionAndSize->x = element->position.x;
                    element->renderInfo.positionAndSize->y = element->position.y;
                    element->renderInfo.positionAndSize->w = element->displaySize.w;
                    element->renderInfo.positionAndSize->h = element->displaySize.h;
                    element->renderInfo.cropArea->x = 0;
                    element->renderInfo.cropArea->y = 0;
                    element->renderInfo.cropArea->w = element->realSize.w;
                    element->renderInfo.cropArea->h = element->realSize.h;
                    break;
                }
                }
                if (element->renderInfo.isRender && element->texture != nullptr)
                {
                    SDL_RenderCopyEx(
                        window->renderer,
                        element->texture,
                        element->renderInfo.cropArea,
                        element->renderInfo.positionAndSize,
                        element->rotateAngle,
                        NULL,
                        SDL_FLIP_NONE
                    );
                }
            }
        }
    }
    SDL_RenderPresent(window->renderer);
}
