cmake_minimum_required(VERSION 3.10)
project(SudokuApp)

set(CMAKE_CXX_STANDARD 17)

add_executable(sudoku
    main.cpp
    sudoku.cpp
    sudoku.h
    button.cpp
    button.h
    # Add all other source/header files
)

# Link necessary Windows libraries
if(WIN32)
    target_link_libraries(sudoku user32 gdi32)
endif()
