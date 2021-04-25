project(threebodiesv2)
add_executable(${PROJECT_NAME} main.cpp model.cpp openglwindow.cpp 
                               camera.cpp body.cpp threebodies.cpp
                               trackball.cpp)
enable_abcg(${PROJECT_NAME})