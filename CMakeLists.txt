cmake_minimum_required(VERSION 3.5)
project(OpenGL_Test)

# Set C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW 2.0 REQUIRED)

# GLM
find_package(glm REQUIRED)

# Add executable
add_executable(OpenGL_Test 
    main.cpp

    imgui/imgui.cpp
    imgui/imgui.h
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imconfig.h
    imgui/imgui_impl_glfw.cpp
    imgui/imgui_impl_glfw.h
    imgui/imgui_impl_opengl3.cpp
    imgui/imgui_impl_opengl3.h
    imgui/imgui_internal.h
    imgui/imgui_impl_opengl3_loader.h
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h
    
    common/lvlloader.cpp
    common/lvlloader.hpp

    common/objloader.cpp
    common/objloader.hpp

    common/object.cpp
    common/object.hpp

    common/face.hpp
    common/normal.hpp
    common/vertex.hpp

    common/light.cpp
    common/light.hpp

    common/camera.cpp
    common/camera.hpp
    
    common/shader.cpp
	common/shader.hpp

	SimpleFragmentShader.glsl
	SimpleVertexShader.glsl
)

# Add include directories
target_include_directories(OpenGL_Test
    PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(OpenGL_Test
    PRIVATE ${OPENGL_LIBRARIES} glfw ${GLEW_LIBRARIES} ${GLM_LIBRARIES}
)

# Set the macOS RPATH to use the @executable_path
if(APPLE)
    set_target_properties(OpenGL_Test PROPERTIES
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "@executable_path"
    )
endif()
