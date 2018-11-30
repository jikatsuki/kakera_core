#ifndef KAKERA_CORE_STRUCTS
#define KAKERA_CORE_STRUCTS

#include <vector>
#include <list>
#include <functional>
#include <stack>
#include <queue>
#include <type_traits>
#include <chrono>
#include "kakera_timer.h"

using namespace std;

namespace kakera_private
{
    template<typename T>
    class Tree
    {
    public:
        struct Node
        {
            T data;
            Node* parent;
            vector<Node*> children;
        };

        void PreOrderDepthFirstSearch(function<void(Node*)> callback)
        {
            if (_root != nullptr)
            {
                stack<Node*> tempStack;
                tempStack.emplace(_root);
                while (!tempStack.empty())
                {
                    Node* node = tempStack.top();
                    tempStack.pop();
                    callback(node);
                    for (auto iter = node->children.rbegin(); iter != node->children.rend(); iter++)
                    {
                        tempStack.emplace(*iter);
                    }
                }
            }
        }

        void BreadthFirstSearch(function<void(Node*)> callback)
        {
            if (_root != nullptr)
            {
                queue<Node*> tempQueue;
                tempQueue.emplace(_root);
                while (!tempQueue.empty())
                {
                    Node* node = tempQueue.front();
                    tempQueue.pop();
                    callback(node);
                    for (auto iter = node->children.begin(); iter != node->children.end(); iter++)
                    {
                        tempQueue.emplace(*iter);
                    }
                }
            }
        }

        Node* GetNodeByData(T data)
        {
            if (_root != nullptr)
            {
                Node* result = nullptr;
                BreadthFirstSearch([&data, &result](Node* node) {
                    if (node->data == data)
                    {
                        result = node;
                    }
                });
                return result;
            }
            return nullptr;
        }

        Node* GetRoot()
        {
            return _root;
        }

        void Clear()
        {
            list<Node*> tempList;
            BreadthFirstSearch([&tempList](Node* node) {
                tempList.emplace_front(node);
            });
            for (Node* node : tempList)
            {
                delete node;
            }
            tempList.clear();
        }

        int InsertNode(Node* node, Node* parent = nullptr)
        {
            if (node != nullptr)
            {
                if (_root != nullptr)
                {
                    node->parent = parent;
                    parent->children.emplace_back(node);
                    return 0;
                }
                else
                {
                    if (parent != nullptr)
                    {
                        _root = parent;
                        node->parent = _root;
                        _root->children.emplace_back(node);
                        return 0;
                    }
                    else
                    {
                        return -1;
                    }
                }
            }
            else
            {
                return -1;
            }
        }

        int DeleteNode(Node* node)
        {
            if (node != nullptr && node != _root)
            {
                for (auto iter = node->parent->children.begin(); iter != node->parent->children.end(); iter++)
                {
                    if (*iter == node)
                    {
                        node->parent->children.erase(iter);
                        break;
                    }
                }
                node->parent->children.insert(node->parent->children.end(), node->children.begin(), node->children.end());
                node->children.clear();
                delete node;
                return 0;
            }
            else if (node != nullptr && node == _root)
            {
                Clear();
                return 0;
            }
            else if (node == nullptr)
            {
                return -1;
            }
            else
            {
                return -1;
            }
        }

        list<T>* ConvertToList()
        {
            list<T>* result = new list<T>;
            BreadthFirstSearch([&result](Node* node) {
                result->emplace_back(node->data);
            });
            return result;
        }

    private:
        Node* _root = nullptr;
    };

    struct Point
    {
        int x = 0;
        int y = 0;
    };

    struct Size_2D
    {
        int w = 0;
        int h = 0;
    };

    using Position_2D = Point;

    struct RenderInfo
    {
        SDL_Rect* positionAndSize = nullptr;
        SDL_Rect* cropArea = nullptr;
        bool isRender = false;
    };

    struct TimerInfo
    {
        kakera_TimerCallback callback = nullptr;
        void* userdata = nullptr;
        bool isLimited = false;
        bool shouldCollected = false;
        chrono::high_resolution_clock::time_point startTime;
        chrono::high_resolution_clock::time_point endTime;        
    };
}

#endif //!KAKERA_CORE_STRUCTS