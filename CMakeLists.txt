cmake_minimum_required(VERSION 3.10)
project(Sudoku_igra)

set(CMAKE_CXX_STANDARD 17)

add_executable(sudoku
    main.cpp
    Sudoku_igra.cpp
    Number_generator.cpp
    Sudoku_igra.h
    Number_generator.h
)

if(WIN32)
    target_link_libraries(sudoku user32 gdi32)
endif()
