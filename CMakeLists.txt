cmake_minimum_required (VERSION 3.28)

project (ray_tracing CXX)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

add_executable (${PROJECT_NAME} "sources/main.cpp" "sources/ray_tracing.hpp")

# SDL2
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY  https://github.com/libsdl-org/SDL
    GIT_TAG         release-2.30.11
    GIT_PROGRESS    TRUE
)

FetchContent_MakeAvailable(SDL2)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main)
