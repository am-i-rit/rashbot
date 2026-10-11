cmake_minimum_required(VERSION 3.20)
project(rashbot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)
find_package(Threads REQUIRED)

add_executable(rashbot
    src/uci_main.cpp 
    src/uci.cpp 
    src/chessboard.cpp 
    src/search.cpp
)


add_executable(rashbot-gui 
    src/main.cpp 
    src/chessboard.cpp 
    src/perft.cpp 
    src/search.cpp
)

add_custom_command(TARGET rashbot-gui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:rashbot-gui>/assets)

target_link_libraries(rashbot PRIVATE Threads::Threads)
target_link_libraries(rashbot-gui PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)
