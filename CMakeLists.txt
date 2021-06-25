cmake_minimum_required(VERSION 3.17)
project(Projekt2)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS -Wall)

find_package(Threads REQUIRED)
find_package(Curses REQUIRED)


add_executable(Projekt2 main.cpp Map.cpp Station.cpp Track.cpp Passenger.cpp Train.cpp Renderer.cpp)


if(TARGET Threads::Threads)
    target_link_libraries(Projekt2 PRIVATE Threads::Threads ${CURSES_LIBRARIES})
endif()