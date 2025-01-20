cmake_minimum_required (VERSION 3.29)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (ray_tracing CXX)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# SDL2
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY  https://github.com/libsdl-org/SDL
    GIT_TAG         release-2.30.11
    GIT_PROGRESS    TRUE
)

FetchContent_MakeAvailable(SDL2)

add_executable (ray_tracing "sources/main.ixx")

target_include_directories(ray_tracing PRIVATE ${SDL2_SOURCE_DIR}/include)

target_link_libraries(ray_tracing PRIVATE SDL2::SDL2 SDL2::SDL2main)

add_custom_command(TARGET ray_tracing POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<$<CONFIG:Debug>:${SDL2_BINARY_DIR}/Debug/SDL2d.dll>
        $<$<CONFIG:Release>:${SDL2_BINARY_DIR}/Release/SDL2.dll>
        $<TARGET_FILE_DIR:ray_tracing>
    COMMENT "Copying SDL2 DLL to output directory"
)