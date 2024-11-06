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

/// Function to read keyboard buffer till it gets one of desired inputs
/// @param desiredInputs vector of characters as intigers (small letters that will be capitalized automatically and special symbols).
/// @param checkForDigits flag for reading digits (except 0) (defaults to false).
/// @param checkForX flag for also reading the x character (defaults to true).
/// @return inputed character as integers.
int read(std::vector<int> desiredInputs, bool checkForDigits=false, bool checkForX=true){
    int choice;
    if (checkForX) desiredInputs.push_back('x');
    for (auto character : desiredInputs) if (character > 96 && character < 123) desiredInputs.push_back(character - 32);
    if (checkForDigits) desiredInputs.insert(desiredInputs.end(), {49, 50, 51, 52, 53, 54, 55, 56, 57});

    bool invalidCharacter = true;
    while(invalidCharacter) {
        choice = _getch();
        for (auto character : desiredInputs) {if (choice == character) {invalidCharacter = false; break;}}}

    return choice;
}

// Function to display the Sudoku grid
void displayGrid(const Grid& grid, short coloredRow=-1, short coloredColumn=-1, int colorCode=33) {
    std::cout << "\033[1;1H"; // Move the coursor to the top of the console

    std::cout << "+-------+-------+-------+\n";
    for (short row = 0; row < SIZE; ++row) {
        if (row % 3 == 0 && row != 0) std::cout << "+-------+-------+-------+\n";
        std::cout << "| ";

        // Change color
        if (coloredRow == row) std::cout << "\033[" << colorCode << "m";

        // Draw
        for (short col = 0; col < SIZE; ++col) {
            if (col == coloredColumn) std::cout << "\033[" << colorCode << "m";

            if (col % 3 == 0 && col != 0) {
                if (row == coloredRow || col == coloredColumn) std::cout << "\033[0m" << "| " << "\033[" << colorCode << "m";
                else std::cout << "| ";
            }

            if (grid[row][col] == 0) std::cout << ". ";

            else std::cout << grid[row][col] << " ";

            if (col == coloredColumn && row != coloredRow) std::cout << "\b" << "\033[0m" << " ";
        }

        // Return to original color
        if (coloredRow == row) std::cout << "\033[0m";
        
        std::cout << "|\n";
    }
    std::cout << "\b\b+-------+-------+-------+\n\n";
}

// Function to update single character in sudoku (Sudoku must be at the top left corner of the screen)
void updateDisplayedGrid(short num, short row, short col, int colorCode=0){
    // Move the cursor to the correct position in the console
    std::cout << "\033[" << ((row + 2) + (row / 3)) << ";" << ((col * 2 + 3) + (col / 3) * 2) << "H"; // Adjust for 1-based index and space

    // Update the cell value
    if (num == 0) std::cout << "\033[" << colorCode << "m" << "." << "\033[m";
    else if (num <= 9) std::cout << "\033[" << colorCode << "m" << num << "\033[m";
    else std::cout << "\033[" << colorCode << "m" << static_cast<char>(num) << "\033[m";

    std::cout << "\033[" << 15 << ";" << 1 << "H";
    std::cout.flush();
}

// Function to check validity of a given number in a given cell
bool isValid(const Grid& grid, short row, short col, short number) {
    for (short i = 0; i < SIZE; ++i){
        if ((i != row && grid[i][col] == number) || (i != col && grid[row][i] == number)) return false;
    }

    short box_row = (row / 3) * 3;
    short box_col = (col / 3) * 3;
    for (short i = 0; i < 3; ++i) {
        for (short j = 0; j < 3; ++j) {
            if (box_row + i != row && box_col + j != col && grid[box_row + i][box_col + j] == number) return false;
        }
    }

    return true;
}

// Function to check wheter the puzzle is solvable
bool isSolvable(const Grid& grid){
    for (short row=0; row < SIZE; row++) for (short col=0; col < SIZE; col++) if (grid[row][col] != 0 && !isValid(grid, row, col, grid[row][col])) return false;
    return true;
}

// Function checks given grid for solvability and colors invalid numbers
void colorValidity(const Grid& grid, std::vector<std::vector<bool>> acc, int colorCode=31){
    for (short i=0; i<SIZE; i++) for (int j=0; j<SIZE; j++) if (!isValid(grid, i, j, grid[i][j]) && grid[i][j] != 0 && acc[i][j]) updateDisplayedGrid(grid[i][j], i, j, colorCode);
}

// Function to change sudoku tiles one by one
bool changeTiles(Grid& grid, const std::vector<std::vector<bool>>& accessibleGrid){
    short row = 4;
    short col = 4;
    int key;
    bool quit = false;
    std::vector<std::vector<bool>> tempAccGrid;

    bool running = true;
    while(running){
        // Disply colored grid
        displayGrid(grid, row, col); 
        tempAccGrid = accessibleGrid;
        for (int i=0; i < SIZE; i++) tempAccGrid[i][col] = tempAccGrid[row][i] = false;
        colorValidity(grid, tempAccGrid);
        if (accessibleGrid[row][col]) updateDisplayedGrid('_', row, col, 33);

        std::cout << "\nPlease use W (^), S (v), A(<), D(>) keys to navigate to the tile you want to replace" << std::endl
                  << "and input a new digit ('0' for an empty tile).\n";

        // Parse input
        while(true){
            key = read({'w', 's', 'a', 'd', '0'}, true);
            if (key == 119) {row = (--row + SIZE) % SIZE; break;}   // w
            if (key == 115) {row = ++row % SIZE; break;}            // s
            if (key == 97) {col = (--col + SIZE) % SIZE; break;}    // a
            if (key == 100) {col = ++col % SIZE; break;}            // d
            if (isdigit(key) && accessibleGrid[row][col]) {grid[row][col] = key - '0'; break;}  // digit
            if (key == 'x') running = quit = false; break;
        }

        // Check validity
        if (isSolvable(grid)) break;
    }
    return quit;
}

// Function to input the Sudoku puzzle
bool inputSudoku(Grid& grid) {
    displayGrid(grid);
    int colorCode = 0;
    bool quit = false;

    for (short row = 0; row < SIZE; row++) {
        for (short col = 0; col < SIZE; col++) {
            std::cout << "\n\nInput your sudoku puzzle.\n(Use digit keys to input clues, use 'Enter' key to leave empty tile)\n";
            
            bool invalidInput = true;
            while (invalidInput) {
                updateDisplayedGrid(95, row, col, 33);
                std::cout << "\rEnter value for cell (" << row + 1 << ", " << col + 1 << "): ";
                
                int input = read({'\r', '\b'}, true);
                
                if (input == 'x' || input == 'X'){
                    return true;
                } else if (input == '\r') { // If Enter (empty input)
                    grid[row][col] = 0; // Set cell as empty (0)
                    invalidInput = false;
                    colorCode = 32;

                } else if (input == '\b') { // backspace
                    updateDisplayedGrid(0, row, col);
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

                } else if (input > '0' && input <= '9') {
                    input -= '0';
                    grid[row][col] = input;
                    invalidInput = false;
                    if (isValid(grid, row, col, input)) colorCode = 32;
                    else colorCode = 31;
                }
                updateDisplayedGrid(grid[row][col], row, col, colorCode);
            }
        }
    }
        
    std::vector<std::vector<bool>> acc(9, std::vector<bool>(9, true));
    system("cls");
    displayGrid(grid);
    colorValidity(grid, acc);

    while(true){
        if (!isSolvable(grid)) std::cout << "\033[31mSudoku is unsolvable!\n\033[0m";

        if (changeTiles(grid, acc)) {quit = true; break;}

        system("cls");
        displayGrid(grid);
        std::cout << "\033[32mYour sudoku puzzle seems solvable!\033[0m" << std::endl
                  << "Are you (S)atisfieed with the grid or would you like to (C)hange some of the tiles?\n";
        int choice = read({'s', 'c'});

        if (choice == 'c' || choice == 'C'){
            if (changeTiles(grid, acc)) quit = true;
        } else if (choice == 's' || choice == 'S'){
            break;
        } else if (choice == 'x' || choice == 'X') {quit = true; break;}
    }

    return quit;
}

/** 
 * @brief Recursive function to solve Sudoku puzzle.
 * @param grid A 9x9 vector of shorts representing the puzzle grid.
 * @param displayProgress Whether to display every try or solve as quickly as possible. (Defaults to false)
 * @param tempo std::chrono::milliseconds value representing time span in ms of each step (how long to display every try). (Defaults to 10)
 * @return true if the grid is solved, otherwise false.
 */
bool solveSudoku(Grid& grid, bool displayProgress=false, ms tempo=ms(10)){

    // Check validity of the sudoku
    if (!isSolvable(grid)) return false;

    // Brute-force with backtracking
    for (short row = 0; row < SIZE; ++row) {
        for (short col = 0; col < SIZE; ++col) {

            if (grid[row][col] == 0) {

                for (short num = 1; num <= 9; ++num) {
                    if (displayProgress){
                        updateDisplayedGrid(num, row, col, 33);
                        std::this_thread::sleep_for(tempo);
                    }

                    if (isValid(grid, row, col, num)) {
                        if (displayProgress){
                            updateDisplayedGrid(num, row, col, 32);
                            std::this_thread::sleep_for(tempo);
                        }
                        grid[row][col] = num;
                        if (solveSudoku(grid, displayProgress, tempo)) return true;
                        grid[row][col] = 0; // If not solved return grid to previous state (backtrack)

                }   }

                if (displayProgress) {
                    updateDisplayedGrid(0, row, col, 31);
                    std::this_thread::sleep_for(tempo);
                }

                return false; // No valid number found, trigger backtracking
    }   }   }
    return true; // Solved
}

int main() {
    Grid sudokuGrid;
    int choice;

    while (true){

        system("cls");
        std::cout << "\033[33m   ____          __       __            ____       __\n  / __/__ __ ___/ /___   / /__ __ __   / __/___   / /_  __ ___  ____\n _\\ \\ / // // _  // _ \\ /  '_// // /  _\\ \\ / _ \\ / /| |/ // -_)/ __/\n/___/ \\___/ \\___/ \\___//_/\\_\\ \\___/  /___/ \\___//_/ |___/ \\__//_/\033[0m\n\n\n";


        // Step 1: Choose between custom input or predefined grid
        std::cout << "Do you want to (I)nput a Sudoku puzzle or use a (P)redefined one?\n\n(You can e(X)it the programm at any point)\n";
        choice = read({'i', 'p'});
        system("cls");

        if (choice == 'x' || choice == 'X') break;
        if (choice == 'i' || choice == 'I') {
            // Custom input
            sudokuGrid = Grid(9, std::vector<short>(9, 0));
            if (inputSudoku(sudokuGrid)) break;
        } else {
            // Use predefined puzzle 
            short sudoku_index = 0;
            bool exitFlag = false;
            while (true){
                displayGrid(predefinedSudokus[sudoku_index]);
                std::cout << "Confirm choice of " << predefinedSudokusNames[sudoku_index] 
                          << " with (ENTER),\nor change it to \033[31m(H)arder\033[0m or \033[32m(E)asier\033[0m sudoku.";
                choice = read({'h', 'e', '\r'});
                if (choice == 'h' || choice == 'H') sudoku_index = ++sudoku_index % numOfPredefinedSudokus;
                else if (choice == 'e' || choice == 'E') sudoku_index = (--sudoku_index + numOfPredefinedSudokus) % numOfPredefinedSudokus;
                else if (choice == '\r') break;
                else if (choice == 'x' || choice == 'X') {exitFlag = true; break;}
            } if (exitFlag) break;

            sudokuGrid = predefinedSudokus[sudoku_index];
            displayGrid(sudokuGrid);

            std::cout << "Using predefined Sudoku puzzle: " << predefinedSudokusNames[sudoku_index] << " sudoku.";
        }

        // Step 2: Choose whether player wants to solve sudoku or to get it solved by the programm
        std::cout << "\nDo you want the (P)rogramm to solve the puzzle or do you want to try and solve it (Y)ourself?";
        choice = read({'p', 'y'});

        if (choice == 'x' || choice == 'X') break;
        // Programm solves sudoku
        if(choice == 'P' || choice == 'p'){
            // Step 2.2 Choose how to display solution
            std::cout << "\nDo you want a (Q)uick solution or to (D)isplay progress? ";
            choice = read({'q', 'd'});

            bool displayProgress = false;
            int delayMs = 0;

            if (choice == 'x' || choice == 'X') break;
            if (choice == 'D' || choice == 'd') {
                displayProgress = true;
                // Ask for delay speed if displaying progress
                std::cout << "\nEnter delay in milliseconds for each step (e.g., 100): ";
                std::cin >> delayMs;
            }
            // Step 2.3: Wait for button press to start solving
            std::cout << "\nPress any key to start solving...";
            _getch();

            // Step 4: Solve the puzzle
            if (solveSudoku(sudokuGrid, displayProgress, ms(delayMs))) {
                system("cls");
                displayGrid(sudokuGrid); 
                std::cout << "\n\033[32mSmudoku solved!\033[0m\n\n";
            }
            else std::cout << "\r\033[31mNo solution exists for the given Sudoku!\033[0m\n";
        }
        // User solves sudoku ###########################
        else{
            system("cls");
            std::cerr << "[ERROR] Feature not implemented: User solves sudoku\n";
        }

        std::cout << "\nWould you like to e(X)it the programm or (R)eturn back to the main screen?";
        choice = read({'r'});
        if (choice == 'x' || choice == 'X') break;
    }

    system("cls");
    return 0;
}
