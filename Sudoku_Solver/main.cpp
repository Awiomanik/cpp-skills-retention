#include <iostream>
#include <vector>
#include <conio.h>
#include <chrono>
#include <thread>
#include <string>

using Grid = std::vector<std::vector<short>>;
using ms = std::chrono::milliseconds;

// Size of the Sudoku grid (9x9)
const short SIZE = 9; 

// Predefined sudokus
const int numOfPredefinedSudokus = 9;
const std::string predefinedSudokusNames[numOfPredefinedSudokus] = {
    "Casual Conundrum (50 clues)",
    "Gentle Challenge (45 clues)",
    "Balanced Brain Teaser (40 clues)",
    "Moderate Mayhem (35 clues)",
    "Intermediate Intrigue (30 clues)",
    "Tough Nut to Crack (25 clues)",
    "Complex Conundrum (21 clues)",
    "Ultimate Puzzle Master (19 clues)",
    "Insidious Enigma (17 clues)"
};
const Grid predefinedSudokus[numOfPredefinedSudokus] = {
    // Very Easy
    Grid{
        {0, 9, 0, 7, 1, 4, 3, 6, 8},
        {0, 1, 4, 8, 9, 6, 7, 0, 5},
        {7, 6, 0, 2, 0, 5, 1, 9, 4},
        {2, 0, 6, 0, 0, 9, 5, 8, 0},
        {0, 0, 0, 0, 0, 7, 2, 1, 6},
        {1, 0, 5, 0, 0, 2, 0, 0, 7},
        {9, 5, 0, 0, 0, 0, 6, 3, 2},
        {4, 0, 3, 0, 6, 1, 8, 7, 9},
        {0, 8, 0, 9, 0, 0, 0, 5, 1}
    },
    // Easy
    Grid{
        {7, 2, 4, 0, 0, 6, 0, 1, 8},
        {0, 3, 5, 0, 0, 9, 0, 7, 4},
        {8, 0, 0, 7, 1, 4, 2, 3, 5},
        {4, 0, 9, 0, 7, 0, 0, 0, 6},
        {3, 0, 0, 2, 6, 5, 1, 4, 0},
        {0, 6, 1, 9, 0, 8, 3, 5, 0},
        {9, 4, 2, 0, 3, 7, 0, 0, 1},
        {0, 1, 0, 0, 8, 0, 0, 0, 0},
        {0, 0, 3, 0, 0, 1, 0, 0, 2}
    },
    // Moderate
    Grid{
        {1, 2, 6, 0, 0, 8, 0, 7, 0},
        {5, 0, 0, 6, 2, 3, 0, 8, 0},
        {0, 3, 0, 7, 0, 1, 9, 0, 0},
        {2, 0, 0, 0, 0, 0, 0, 0, 1},
        {3, 1, 5, 8, 0, 0, 0, 0, 9},
        {9, 6, 0, 5, 1, 0, 0, 0, 3},
        {0, 5, 0, 2, 3, 7, 4, 0, 0},
        {7, 0, 3, 0, 0, 0, 6, 1, 2},
        {8, 4, 0, 0, 9, 0, 0, 0, 7}
    },
    // Medium
    Grid{
        {0, 0, 0, 0, 7, 0, 0, 0, 4},
        {0, 0, 0, 0, 0, 0, 0, 8, 9},
        {9, 0, 5, 0, 1, 0, 0, 0, 0},
        {2, 6, 9, 5, 8, 1, 0, 0, 0},
        {0, 8, 7, 0, 3, 4, 9, 1, 0},
        {4, 0, 3, 0, 0, 7, 2, 0, 8},
        {6, 0, 0, 0, 0, 0, 4, 0, 0},
        {3, 5, 8, 0, 0, 0, 6, 7, 0},
        {0, 0, 4, 1, 0, 0, 8, 2, 0}
    },
    // Intermediate
    Grid{
        {6, 0, 0, 4, 9, 0, 8, 0, 1},
        {0, 0, 0, 0, 0, 1, 0, 0, 0},
        {8, 0, 0, 0, 0, 0, 7, 0, 0},
        {0, 4, 2, 0, 0, 0, 0, 8, 0},
        {7, 0, 0, 0, 0, 0, 2, 3, 4},
        {0, 0, 8, 6, 0, 0, 9, 0, 7},
        {0, 0, 6, 0, 0, 8, 1, 0, 3},
        {0, 7, 0, 1, 5, 9, 0, 0, 0},
        {9, 0, 0, 2, 0, 0, 0, 7, 0}
    },
    // Challenging
    Grid{
        {5, 0, 6, 0, 0, 0, 0, 7, 1},
        {0, 0, 0, 0, 1, 0, 0, 3, 0},
        {1, 7, 0, 0, 0, 0, 8, 4, 0},
        {0, 5, 0, 0, 8, 0, 0, 6, 2},
        {0, 0, 0, 0, 4, 6, 0, 9, 0},
        {9, 0, 0, 0, 0, 0, 0, 0, 7},
        {0, 0, 9, 0, 0, 0, 0, 8, 0},
        {0, 0, 0, 0, 3, 0, 0, 0, 0},
        {7, 4, 0, 9, 0, 0, 0, 0, 0}
    },
    // Hard
    Grid{
        {9, 0, 0, 0, 5, 1, 7, 0, 0},
        {0, 4, 8, 0, 0, 0, 0, 0, 0},
        {0, 7, 0, 0, 0, 2, 3, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 2},
        {0, 0, 6, 0, 2, 0, 5, 4, 0},
        {4, 0, 0, 0, 0, 5, 0, 0, 3},
        {0, 0, 0, 6, 0, 0, 0, 0, 0},
        {0, 0, 9, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 7, 0, 0, 0, 0}
    },
    // Fiendish
    Grid{
        {0, 0, 0, 4, 0, 0, 0, 8, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 6, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 5},
        {0, 7, 0, 0, 0, 0, 0, 0, 0},
        {3, 9, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 4, 9, 0, 7, 0},
        {9, 0, 0, 0, 0, 3, 5, 0, 2},
        {0, 0, 0, 0, 6, 7, 0, 0, 4}
    },
    // Expert
    Grid{
        {0, 0, 0, 0, 2, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 5, 0, 0},
        {0, 0, 0, 5, 4, 0, 0, 9, 0},
        {0, 9, 0, 8, 0, 0, 0, 0, 2},
        {0, 0, 0, 0, 0, 5, 0, 0, 0},
        {0, 2, 0, 0, 9, 0, 1, 0, 0},
        {6, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 7, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 7, 0, 3, 0, 0}
    }
};

// Function to display the Sudoku grid
void displayGrid(const Grid& grid) {
    system("cls");    

    std::cout << "+-------+-------+-------+\n| ";
    for (short row = 0; row < SIZE; ++row) {
        if (row % 3 == 0 && row != 0) std::cout << "\b\b+-------+-------+-------+\n| ";
        
        for (short col = 0; col < SIZE; ++col) {
            if (col % 3 == 0 && col != 0) std::cout << "| ";
            if (grid[row][col] == 0) std::cout << ". ";
            else std::cout << grid[row][col] << " ";
            
        }
        std::cout << "|\n| ";
    }
    std::cout << "\b\b+-------+-------+-------+\n";
}

// Function to update single character in sudoku (Sudoku must be at the top left corner of the screen)
void updateDisplayedGrid(short num, short row, short col){
    // Move the cursor to the correct position in the console
    std::cout << "\033[" << ((row + 2) + (row / 3)) << ";" << ((col * 2 + 3) + (col / 3) * 2) << "H"; // Adjust for 1-based index and space

    // Update the cell value
    if (num == 0) std::cout << ". ";
    else std::cout << num << " ";

    std::cout << "\033[" << 14 << ";" << 1 << "H";
    std::cout.flush();
}

// Function to input the Sudoku puzzle
void inputSudoku(Grid& grid) {
    displayGrid(grid);

    for (short row = 0; row < SIZE; row++) {
        for (short col = 0; col < SIZE; col++) {
            std::cout << "\nInput your Sudoku puzzle (use 'Enter' for empty cells):\n";
            bool validInput = false;

            while (!validInput) {
                std::cout << "\rEnter value for cell (" << row + 1 << ", " << col + 1 << "): ";
                
                char input = _getch();
                
                if (input == '\r') { // If Enter (empty input)
                    grid[row][col] = 0; // Set cell as empty (0)
                    validInput = true;

                } else if (input == '\b') { // backspace
                    if (col == 0 && row > 0) {
                        col = SIZE - 1;
                        row--;
                        grid[row][col] = 0;
                    } else if (col > 0) {
                        col--;
                        grid[row][col] = 0;
                    }
                    updateDisplayedGrid(0, row, col);
                    std::cout << "\n\n";

                } else if (std::isdigit(input)) {
                    short value = input - '0'; // Convert char to short
                    
                    if (value > 0 && value <= 9) {
                        grid[row][col] = value;
                        validInput = true;
                    } 
                    //else std::cout << "\nInvalid input! Please enter a number between 1 and 9, or press Enter for an empty cell.\n";
                } //else std::cout << "\nInvalid input! Please enter a number between 1 and 9, or press Enter for an empty cell.\n";
            }

            updateDisplayedGrid(grid[row][col], row, col);
        }
    }
    displayGrid(grid);
    std::cout << "Sudoku puzzle input complete!\n";
    std::cout << "Solving...";
    
}

// Function to check validity of a given number in a given cell
bool isValid(const Grid& grid, short row, short col, short number) {
    for (short i = 0; i < SIZE; ++i){
        if (grid[row][i] == number || grid[i][col] == number) return false;
    }

    short box_row = (row / 3) * 3;
    short box_col = (col / 3) * 3;
    for (short i = 0; i < 3; ++i) {
        for (short j = 0; j < 3; ++j) {
            if (grid[box_row + i][box_col + j] == number) return false;
        }
    }

    return true;
}

/** 
 * @brief Recursive function to solve Sudoku puzzle.
 * @param grid A 9x9 vector of shorts representing the puzzle grid.
 * @param displayProgress Whether to display every try or solve as quickly as possible. (Defaults to false)
 * @param tempo std::chrono::milliseconds value representing time span in ms of each step (how long to display every try). (Defaults to 10)
 * @return true if the grid is solved, otherwise false.
 */
bool solveSudoku(Grid& grid, bool displayProgress=false, ms tempo=ms(10)){

    for (short row = 0; row < SIZE; ++row) {
        for (short col = 0; col < SIZE; ++col) {

            if (grid[row][col] == 0) {

                for (short num = 1; num <= 9; ++num) {
                    if (displayProgress){
                        updateDisplayedGrid(num, row, col);
                        std::this_thread::sleep_for(tempo);
                    }

                    if (isValid(grid, row, col, num)) {
                        grid[row][col] = num;
                        if (solveSudoku(grid, displayProgress, tempo)) return true;
                        grid[row][col] = 0; // If not solved return grid to previous state (backtrack)

                }   }

                if (displayProgress) {
                    updateDisplayedGrid(0, row, col);
                    std::this_thread::sleep_for(tempo);
                }

                return false; // No valid number found, trigger backtracking
    }   }   }
    return true; // Solved
}

int main() {
    Grid sudokuGrid;
    char choice;

    bool running = true;
    while (running){

        system("cls");
        std::cout << "   ____          __       __            ____       __\n  / __/__ __ ___/ /___   / /__ __ __   / __/___   / /_  __ ___  ____\n _\\ \\ / // // _  // _ \\ /  '_// // /  _\\ \\ / _ \\ / /| |/ // -_)/ __/\n/___/ \\___/ \\___/ \\___//_/\\_\\ \\___/  /___/ \\___//_/ |___/ \\__//_/\n\n\n";

        // Step 1: Choose between custom input or predefined grid
        std::cout << "Do you want to (I)nput a Sudoku puzzle or use a (P)redefined one? ";
        std::cin >> choice;

        if (choice == 'I' || choice == 'i') {
            // Custom input
            sudokuGrid = Grid(9, std::vector<short>(9, 0));
            inputSudoku(sudokuGrid);
        } else {
            // Use predefined puzzle 
            short sudoku_index = 0;
            bool choosing = true;
            while (choosing){
                displayGrid(predefinedSudokus[sudoku_index]);
                std::cout << "\nConfirm choice of " << predefinedSudokusNames[sudoku_index] 
                          << " with (ENTER),\nor change it to (H)arder or (E)asier sudoku.";
                choice = _getch();
                if (choice == 'h' || choice == 'H') sudoku_index = ++sudoku_index % numOfPredefinedSudokus;
                else if (choice == 'e' || choice == 'E') sudoku_index = (--sudoku_index + numOfPredefinedSudokus) % numOfPredefinedSudokus;
                else if (choice == '\r') choosing = false;
            }

            sudokuGrid = predefinedSudokus[sudoku_index];
            displayGrid(sudokuGrid);

            std::cout << "Using predefined Sudoku puzzle: " << predefinedSudokusNames[sudoku_index] << " sudoku.";
        }

        // Step 2: Choose whether player wants to solve sudoku or to get it solved by the programm
        std::cout << "\n\nDo you want the (P)rogramm to solve the puzzle or do you want to try and solve it (Y)ourself? ";
        std::cin >> choice;

        // Programm solves sudoku
        if(choice == 'P' || choice == 'p'){

            // Step 2.2 Choose how to display solution
            std::cout << "Do you want a (Q)uick solution or to (D)isplay progress? ";
            std::cin >> choice;

            bool displayProgress = false;
            int delayMs = 0;

            if (choice == 'D' || choice == 'd') {
                displayProgress = true;

                // Ask for delay speed if displaying progress
                std::cout << "Enter delay in milliseconds for each step (e.g., 100): ";
                std::cin >> delayMs;
            }

            // Step 2.3: Wait for button press to start solving
            std::cout << "\nPress any key to start solving...";
            _getch();

            // Step 4: Solve the puzzle
            if (solveSudoku(sudokuGrid, displayProgress, ms(delayMs))) {
                displayGrid(sudokuGrid); 
                std::cout << "\nSudoku solved!\n\n";
            }
            else std::cout << "\rNo solution exists for the given Sudoku! \n";
        }

        // User solves sudoku ###########################
        else{
            std::cout << "there!";
        }

        std::cout << "\nWould you like to e(X)it the programm or (C)ontinue with new sudoku? ";
        std::cin >> choice;
        if (choice == 'x' || choice == 'X') running = false;
    }

    system("cls");
    return 0;
}