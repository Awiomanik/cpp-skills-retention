// Tests implementation
// Author: WKK

#include "Book.h"
#include "User.h"
#include "tests.h"

void bookClassTest() {
    std::cout << "\n========================================================================="
              << "\n============================ Book Class Test ============================"
              << "\n=========================================================================\n\n";

    // Creating an array of Book objects
    Book shelf[2];

    // Initializing books
    shelf[0] = Book(
        "Gwiezdny Posłaniec: Nasza cywilizacja z perspektywy kosmicznej",
        "Neil deGrasse Tyson",
        "9788367710985");
        
    shelf[1] = Book(
        "Fundacja",
        "Isaac Asimov",
        "9788381885171");

    // Display initial availability
    std::cout << "\nInitial Availability:\n";
    shelf[0].displayDetails();
    shelf[1].displayDetails();

    // Issue books with due dates
    std::cout << "\nIssuing Books:\n";
    std::cout << shelf[0].issueBook(10);
    std::cout << shelf[1].issueBook(5);

    // Check availability after issuing
    std::cout << "\nAvailability After Issuing:\n";
    shelf[0].displayDetails();
    shelf[1].displayDetails();

    // Return the first book
    std::cout << "\nReturning the first book:\n";
    shelf[0].returnBook();
    
    // Check availability after returning
    std::cout << "\nAvailability After Returning:\n";
    shelf[0].displayDetails();
    shelf[1].displayDetails();

    std::cout << "\n========================================================================"
              << "\n============================ Test Completed ============================"
              << "\n========================================================================\n\n\n";
}

void shelfClassTest(){
    std::string testFileName = "storage/book_data_test.dat";

    std::cout << "\n========================================================================"
              << "\n=========================== Shelf Class Test ==========================="
              << "\n========================================================================\n\n";

    Shelf* shelf = Shelf::getInstance();
    shelf->getBooks().clear();

    // Test saving books to a file
    std::cout << "Saving (no) books to test file Test:\n";
    shelf->saveBooksToFile(testFileName);
    std::cout << "Books saved succesfully in " << testFileName << "!\n" << std::endl;

    // Test loading books from a file
    std::cout << "Loading (no) books from file Test:\n";
    shelf->loadBooksFromFile(testFileName);
    // Verify that the books were loaded correctly
    const auto& books = shelf->getBooks();
    std::cout << 
    (books.empty() ? "Books successfully loaded from file!" : "Error: books file not empty!")
    << std::endl << std::endl;

    // Test encoding and decoding a string
    std::cout << "Encoding and Decoding Test:\n";
    std::string originalTitle = "Test name...  !@#:>}. a b c x y z  !";
    std::string encodedTitle = shelf->asciiEncode(originalTitle);
    std::string decodedTitle = shelf->asciiDecode(encodedTitle);

    std::cout << "Original Title: " << originalTitle << "\n";
    std::cout << "Encoded  Title: " << encodedTitle << "\n";
    std::cout << "Decoded  Title: " << decodedTitle << "\n";
    std::cout << (originalTitle == decodedTitle ? 
    "Encoding/Decoding successfull!\n" : "Encoding/Decoding failed!\n") << std::endl;

    // Verify file save/load for many books
    std::cout << "Final test:\n";
    Book book1(
        "Gwiezdny Posłaniec: Nasza cywilizacja z perspektywy kosmicznej",
        "Neil deGrasse Tyson",
        "9788367710985");
    Book book2("Fundacja", "Isaac Asimov", "9788381885171");
    Book bookSpecialChar("L'étranger", "Albert Camus", "9782070360024");
    Book bookIssued("Issued book", "anonymus", "123456789012", 
    false, std::chrono::system_clock::now());

    shelf->addBooks({book1, book2, bookSpecialChar, bookIssued});
    shelf->saveBooksToFile(testFileName);
    shelf->clearBooks();
    shelf->loadBooksFromFile(testFileName); // 
    std::cout << "\nBooks loaded onto a shelf:\n";
    for (Book bk : shelf->getBooks()) {
        bk.displayDetails();
    }
    std::cout << "Number of books: " << shelf->getBookCount();

    std::cout << "\n========================================================================"
              << "\n============================ Test Completed ============================"
              << "\n========================================================================\n\n\n";
}

void userClassTest() {

    std::cout << "\n========================================================================"
              << "\n============================ User Class Test ==========================="
              << "\n========================================================================\n\n";

    // Create a user
    std::cout << "Creating User." << std::endl << std::endl;;
    User user("John", "johnspassword");

    // Test login with correct password
    std::cout << "Testing logging with correct password:" << std::endl;
    if (user.login("johnspassword")) std::cout << "Login successful!" << std::endl << std::endl;
    else std::cout << "Login failed!" << std::endl << std::endl;

    // Test login with incorrect password
    std::cout << "Testing logging with incorrect password:" << std::endl;
    if (user.login("wrongpassword")) std::cout << "Login successful!" << std::endl << std::endl;
    else std::cout << "Login failed sucessfully!" << std::endl << std::endl;

    // Test resetting password
    std::cout << "Testing password reset:" << std::endl;
    if (user.resetPassword("1234", "newpassword")) std::cout << "Password reset successful!" << std::endl << std::endl;
    else std::cout << "Password reset failed!" << std::endl << std::endl;

    // Test login with the new password
    std::cout << "Testing logging with the new password:" << std::endl;
    if (user.login("newpassword")) std::cout << "Login with new password successful!" << std::endl << std::endl;
    else std::cout << "Login with new password failed!" << std::endl;

    std::cout << "\n========================================================================"
              << "\n============================ Test Completed ============================"
              << "\n========================================================================\n\n\n";
}









