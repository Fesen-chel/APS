
cmake_minimum_required(VERSION 3.10)

project(aps)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
    request.cpp
    expert.cpp
    buffer.cpp
    client.cpp
    placement_dispatcher.cpp
    selection_dispatcher.cpp
    simulation.cpp
    globals.cpp
)

set(HEADERS
    request.h
    expert.h
    buffer.h
    client.h
    placement_dispatcher.h
    selection_dispatcher.h
    simulation.h
    globals.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS}

)