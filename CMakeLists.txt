cmake_minimum_required(VERSION 3.10)
project(Sudoku_igra)

add_executable(sudoku
    main.cpp
    Sudoku_igra.cpp
    Number_generator.cpp
    Sudoku_igra.h
    Number_generator.h
)

set_target_properties(sudoku PROPERTIES WIN32_EXECUTABLE TRUE)
