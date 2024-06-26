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
    
    common/objloader.cpp
    common/objloader.hpp
    
    common/face.hpp
    common/normal.hpp
    common/vertex.hpp
    
    common/shader.cpp
	common/shader.hpp

	SimpleFragmentShader.fragmentshader
	SimpleVertexShader.vertexshader
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
