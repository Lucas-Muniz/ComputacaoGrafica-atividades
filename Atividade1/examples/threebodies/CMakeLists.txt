project(threebodies)
add_executable(${PROJECT_NAME} main.cpp model.cpp openglwindow.cpp 
                               camera.cpp body.cpp threebodies.cpp)
enable_abcg(${PROJECT_NAME})