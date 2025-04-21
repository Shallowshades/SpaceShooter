# 标题
cmake_minimum_required(VERSION 3.10.0)
project(Shooter VERSION 0.1.0 LANGUAGES C CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
if(MSVC)
    # add_compile_options(/W4)
else()
    # add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 设置输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

# 设置输出文件名称-系统名称
set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

# # Windows
# if(WIN32)
#     include_directories("C:/library/SDL3/include")
#     link_directories("C:/library/SDL3/lib/x64")
# # Mac
# elseif(APPLE)
#     include_directories("/usr/local/include/SDL2")
#     link_directories("/usr/local/lib")
# # Linux
# elseif(UNIX)
#     include_directories("/usr/include/SDL2")
#     link_directories("/usr/lib/x86_64-linux-gnu")
# endif()

# 库校验
message(STATUS "SDL2_libraries: ${SDL2_LIBRARIES}")
message(STATUS "SDL2_image: ${SDL2_IMAGE_LIBRARIES}")
message(STATUS "SDL2_ttf: ${SDL2_TTF_LIBRARIES}")
message(STATUS "SDL2_mixer libraries: ${SDL2_MIXER_LIBRARIES}")

# 查找并载入Cmake预设 (PATH)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)


# 添加可执行文件
add_executable(${TARGET} 
                        "src/main.cpp"
                        "src/Scene.cpp"
                        "src/SceneMain.cpp"
                        "src/SceneTitle.cpp"
                        "src/SceneEnd.cpp"
                        )

# 链接库
target_link_libraries(${TARGET} 
                        ${SDL2_LIBRARIES}
                        ${SDL2_IMAGE_LIBRARIES}
                        ${SDL2_TTF_LIBRARIES}
                        ${SDL2_MIXER_LIBRARIES}
                        SDL2_image::SDL2_image
                        SDL2_ttf::SDL2_ttf
                        SDL2_mixer::SDL2_mixer
                        )

if (MSVC)
    target_link_options(${TARGET} PRIVATE /SUBSYSTEM:WINDOWS)
endif()