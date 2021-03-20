project(spacebattle)

add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp
                               bullets.cpp ship.cpp starlayers.cpp 
                               enemies.cpp forcefield.cpp)

enable_abcg(${PROJECT_NAME})
