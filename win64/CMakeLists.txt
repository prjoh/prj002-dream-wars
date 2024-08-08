cmake_minimum_required(VERSION 3.23)

project(dream_wars)

set(CMAKE_CXX_STANDARD 20)

if (APPLE)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${project_SOURCE_DIR}/cmake")
elseif (WIN32)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
    set(CMAKE_CXX_FLAGS "-static")
endif()

# Find the SDL2 framework
if (APPLE)
    find_package(SDL2 REQUIRED COMPONENTS SDL2)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_mixer REQUIRED)
elseif (WIN32)
    set(SDL2_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/x64/SDL2.dll")
    set(SDL2_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include/SDL2")
    set(SDL2_LIBRARY "${CMAKE_CURRENT_LIST_DIR}/lib/x64/SDL2.dll")
    find_package(SDL2 REQUIRED)
    set(SDL2_IMAGE_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/x64/SDL2_image.dll")
    set(SDL2_IMAGE_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include/SDL2")
    set(SDL2_IMAGE_LIBRARY "${CMAKE_CURRENT_LIST_DIR}/lib/x64/SDL2_image.dll")
    find_package(SDL2_image REQUIRED)
    set(SDL2_TTF_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/x64/SDL2_ttf.dll")
    set(SDL2_TTF_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include/SDL2")
    set(SDL2_TTF_LIBRARY "${CMAKE_CURRENT_LIST_DIR}/lib/x64/SDL2_ttf.dll")
    find_package(SDL2_ttf REQUIRED)
    set(SDL2_MIXER_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/x64/SDL2_mixer.dll")
    set(SDL2_MIXER_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include/SDL2")
    set(SDL2_MIXER_LIBRARY "${CMAKE_CURRENT_LIST_DIR}/lib/x64/SDL2_mixer.dll")
    find_package(SDL2_mixer REQUIRED)
endif()

# Setup Freetype library
if (APPLE)
    find_package(Freetype CONFIG)
    find_package(Freetype REQUIRED)
elseif (WIN32)
    set(FREETYPE_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include/freetype/)
    set(FREETYPE_LIBRARY ${PROJECT_SOURCE_DIR}/lib/x64/freetype.dll)
    find_package(Freetype REQUIRED)
endif()

# Setup assimp
if (APPLE)
    add_library(assimp SHARED IMPORTED)
    set_target_properties(assimp PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/lib/macos/libassimp.dylib)
elseif (WIN32)
    add_library(assimp SHARED IMPORTED)
    set(ASSIMP_PATH ${CMAKE_CURRENT_LIST_DIR}/lib/x64/libassimp-5.dll)
    set_target_properties(assimp PROPERTIES
            IMPORTED_IMPLIB "${CMAKE_CURRENT_LIST_DIR}/lib/x64/libassimp.dll.a"
            IMPORTED_LOCATION "${ASSIMP_PATH}")
endif()

# Setup OpenGL
find_package(OpenGL REQUIRED)

# Setup glad
add_library(glad STATIC "${CMAKE_CURRENT_LIST_DIR}/include/glad.c")
target_include_directories(glad PRIVATE "${CMAKE_CURRENT_LIST_DIR}/include")

# Setup imgui
add_library(imgui
        # Main Imgui files
        ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui.cpp
        ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_draw.cpp
        ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_tables.cpp
        ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_widgets.cpp
        # SDL2+OpenGL-specific files
        ${CMAKE_CURRENT_LIST_DIR}/imgui/backends/imgui_impl_sdl.cpp
        ${CMAKE_CURRENT_LIST_DIR}/imgui/backends/imgui_impl_opengl3.cpp)
# Make SDL2 available to Imgui
if (APPLE)
    target_include_directories(imgui PUBLIC ${SDL2_INCLUDE_DIRS})
elseif(WIN32)
    target_include_directories(imgui PUBLIC ${SDL2_INCLUDE_DIR})
endif()
# imgui/backends/ contains the SDL implementation
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include/imgui/ ${CMAKE_CURRENT_LIST_DIR}/include/imgui/backends/)
# Configure Imgui to use OpengGL through SDL2
#target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_CUSTOM=<SDL_opengl.h>  GL_GLEXT_PROTOTYPES=1)

add_executable(${PROJECT_NAME} main.cpp mesh.h texture.h material.h line.h mesh.cpp util.h util.cpp texture.cpp line.cpp
        material.cpp camera.h lights.h common.h common.cpp transform.h transform.cpp input.h camera.cpp perlin.h
        perlin.cpp entity.h component.h common.cpp system.cpp system.h component.cpp entity.cpp player.h easing.h particle.h
        particle.cpp collision.h collision.cpp audio.h audio.cpp ai.h gui.h gui.cpp health.h game.h game.cpp load.h load.cpp
        terrain.h terrain.cpp game_state.h game_state.cpp text.h text.cpp coroutine.h coroutine.cpp screen.h screen.cpp random.h random.cpp)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE assimp)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework OpenGL")
#    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework CoreFoundation")
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer imgui Freetype::Freetype)
elseif (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} ${SDL2_MIXER_LIBRARY} glad imgui ${FREETYPE_LIBRARIES})
endif()

if (APPLE)
    set_target_properties(
            ${PROJECT_NAME} PROPERTIES
            OUTPUT_NAME "game"
            MACOSX_BUNDLE TRUE
    )
    add_custom_command(
            TARGET ${PROJECT_NAME}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res/ game.app/Contents/Resources/res
    )
elseif (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            OUTPUT_NAME ${PROJECT_NAME}
            WIN32 TRUE
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PROJECT_NAME}"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PROJECT_NAME}"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PROJECT_NAME}"
            )
    # copy the .dll files to the same folder as the executable
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_PATH}"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_IMAGE_PATH}"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_TTF_PATH}"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_MIXER_PATH}"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${FREETYPE_LIBRARY}"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ASSIMP_PATH}"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
    # copy resources to binary folder
    add_custom_command(
            TARGET ${PROJECT_NAME}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/res/
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/res/
    )
endif()

# Include library headers
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/freetype")
