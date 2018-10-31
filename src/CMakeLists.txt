aux_source_directory(. SOURCE_FILES)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include_private)

if(WIN32)
	set(KAKERA_SDL_INCLUDE_PATH "" CACHE PATH "SDL2 include path")
	set(KAKERA_SDL_LIBRARY_PATH "" CACHE PATH "SDL2 library path")
else()
	set(KAKERA_SDL_INCLUDE_PATH "/usr/include" CACHE PATH "SDL2 include path")
	set(KAKERA_SDL_LIBRARY_PATH "/usr/lib/x86_64-linux-gnu" CACHE PATH "SDL2 library path")
endif()

include_directories(${KAKERA_SDL_INCLUDE_PATH})
link_directories(${KAKERA_SDL_LIBRARY_PATH})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "CLANG")
	add_compile_options(-std=c++14 -lstdc++)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	add_compile_options(/std:c++14)
endif()
	
add_library(kakera_core SHARED ${SOURCE_FILES})

target_link_libraries(kakera_core SDL2 SDL2main SDL2_image SDL2_ttf)

if(WIN32)
	target_link_libraries(kakera_core shcore)
endif()