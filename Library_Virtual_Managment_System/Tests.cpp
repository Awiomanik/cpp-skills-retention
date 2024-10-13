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

void userClassesTest() {

    std::cout << "\n========================================================================"
              << "\n=========================== User Classes Test =========================="
              << "\n========================================================================\n\n";

    // Prepare books for testing
    Book book1("1984", "George Orwell", "123456789");
    Book book2("Brave New World", "Aldous Huxley", "123456789");
    Book book3("Fahrenheit 451", "Ray Bradbury", "123456789");
    Book book4("The Great Gatsby", "F. Scott Fitzgerald", "123456789");
    Book book5("Moby-Dick", "Herman Melville", "123456789");
    Book book6("To Kill a Mockingbird", "Harper Lee", "123456789");
    Book book7("Title7", "Author3", "123456789");
    Book book8("Title8", "Author4", "123456789");
    Book book9("Title9", "Author1", "123456789");
    Book book10("Title10", "Author2", "123456789");
    Book book11("Title11", "Author3", "123456789");
    std::vector<Book> shelf = {book1, book2, book3};

    // Create a user
    std::cout << "\nCreating New Users:" << std::endl;
    Student student1("John", "johnspassword", true);
    std::cout << "Student created." << std::endl;
    Professor prof1("Anna", "AnnasPassword@!&$", true);
    std::cout << "Profesor created." << std::endl << std::endl;
    
    std::cout << "Creating Users from data:" << std::endl;
    Student student2("Default data", "example hash 1");
    std::cout << "Student created: " << std::endl
              << student2.getUserName() << ", " << student2.getHash() << ", "
              << student2.getNumberOfIssuedBooks() << std::endl;
    Professor prof2("Explicit default data", "example hash 2", 0, {});
    std::cout << "Profesor created: " << std::endl
              << prof2.getUserName() << ", " << prof2.getHash() << ", "
              << prof2.getNumberOfIssuedBooks() << std::endl;
    Student student3("Some data", "example hash 3", 2, shelf);
    std::cout << "Student created: " << std::endl
              << student3.getUserName() << ", " << student3.getHash() << ", "
              << student3.getNumberOfIssuedBooks() << std::endl << std::endl;

    // Test login with correct password
    std::cout << "Testing logging with correct password:" << std::endl;
    if (student1.login("johnspassword")) std::cout << "Student login successful!" << std::endl;
    else std::cout << "Student login failed!" << std::endl;
    if (prof1.login("AnnasPassword@!&$")) std::cout << "Professor login successful!" << std::endl << std::endl;
    else std::cout << "Professor login failed!" << std::endl << std::endl;

    // Test login with incorrect password
    std::cout << "Testing logging with incorrect password:" << std::endl;
    if (student1.login("wrongpassword")) std::cout << "Student login successful!" << std::endl;
    else std::cout << "Student login failed sucessfully!" << std::endl;
    if (prof1.login("wrongpassword")) std::cout << "Professor login successful!" << std::endl << std::endl;
    else std::cout << "Professor login failed sucessfully!" << std::endl << std::endl;

    // Test resetting password
    std::cout << "Testing password reset:" << std::endl;
    if (student1.resetPassword("31415926", "newpassword1")) std::cout << "Password reset successful!" << std::endl;
    else std::cout << "Password reset failed!" << std::endl;
    if (prof1.resetPassword("31415926", "newpassword2")) std::cout << "Password reset successful!" << std::endl << std::endl;
    else std::cout << "Password reset failed!" << std::endl << std::endl;

    std::cout << "Testing password reset with wrong reset code:" << std::endl;
    if (student1.resetPassword("123", "anotherpassword")) std::cout << "Password reset successful!" << std::endl;
    else std::cout << "Password reset failed successfully!" << std::endl << std::endl;

    // Test login with the new password
    std::cout << "Testing logging with the new password:" << std::endl;
    if (student1.login("newpassword1")) std::cout << "Student login with new password successful!" << std::endl;
    else std::cout << "Student login with new password failed!" << std::endl;
    if (prof1.login("newpassword2")) std::cout << "Professor login with new password successful!" << std::endl << std::endl;
    else std::cout << "Professor login with new password failed!" << std::endl << std::endl;

    // Test book issue
    std::cout << "Testing issuing books:" << std::endl;
    student1.issueBook(book1);
    student1.issueBook(book2);
    student1.issueBook(book3);
    student1.issueBook(book4);
    if(student1.issueBook(book5)) std::cout << "Student issued fifth book successfully!" << std::endl;
    else std::cout << "Student failed to issue fifth book!" << std::endl;
    if(student1.issueBook(book6)) std::cout << "Student issued sixth book successfully!" << std::endl;
    else std::cout << "Student failed to issue sixth book!" << std::endl;
    prof1.issueBook(book1);
    prof1.issueBook(book2);
    prof1.issueBook(book3);
    prof1.issueBook(book4);
    prof1.issueBook(book5);
    prof1.issueBook(book6);
    prof1.issueBook(book7);
    prof1.issueBook(book8);
    prof1.issueBook(book9);
    if(prof1.issueBook(book10)) std::cout << "Professor issued tenth book successfully!" << std::endl;
    else std::cout << "Professor failed to issue tenth book!" << std::endl;
    if(prof1.issueBook(book11)) std::cout << "Professor issued eleventh book successfully!" << std::endl;
    else std::cout << "Professor failed to issue eleventh book!" << std::endl;
    if(student1.getNumberOfIssuedBooks() == 5 && 
       prof1.getNumberOfIssuedBooks() == 10) 
        { std::cout << "All books issued successfully!" << std::endl << std::endl; }

    // Test book return
    std::cout << "Testing returning books:" << std::endl;
    std::cout << "Returning issued books:" << std::endl;
    student1.returnBook(book1);
    student1.returnBook(book2);
    prof1.returnBook(book3);
    prof1.returnBook(book4);
    if(prof1.getNumberOfIssuedBooks() == 8 && 
       student1.getNumberOfIssuedBooks() == 3)
        { std::cout << "Books successfully returned!" << std::endl; }

    std::cout << "Returning non-issued books:" << std::endl;
    if(!student1.returnBook(book10) && !prof1.returnBook(book3)) {
        unsigned int numStu = student1.getNumberOfIssuedBooks();
        unsigned int numPro = prof1.getNumberOfIssuedBooks();

        if(numStu == 3 && numPro == 8) {
            std::cout << "Non-issued books failed to return successfully!" << std::endl << std::endl;
        } else { 
            std::cout << "Number of issued books does not match expected one!" << std::endl <<
                         "Number of books issued (professor, student): (" << numPro <<
                         ", " << numStu << ")  -  Expected: (8, 3)" << std::endl << std::endl;
        }
    } else { std::cout << "Fail! Non-issued books got returned!" << std::endl << std::endl; }

    std::cout << "Final Test: Issue and Return Limits:" << std::endl;
    // Issue books until the student's limit is reached
    student1.issueBook(book1);
    student1.issueBook(book2);
    std::cout << "Number of currently issued books (expected: 5): " 
              << student1.getNumberOfIssuedBooks() << std::endl;
    // Attempt to issue another book beyond the limit
    student1.issueBook(book6);
    std::cout << "Number of currently issued books (expected: 5): " 
              << student1.getNumberOfIssuedBooks() << std::endl;
    // Return a book and attempt to issue again
    student1.returnBook(book1);
    student1.issueBook(book6);
    std::cout << "Number of currently issued books (expected: 5): " 
              << student1.getNumberOfIssuedBooks() << std::endl;
    // Attempt to return the same book multiple times
    if (student1.returnBook(book1)) std::cout << "Error: Returned book1 again!" << std::endl;
    else std::cout << "Correctly prevented returning book1 again." << std::endl;
    student1.returnBook(book2);
    student1.returnBook(book3);
    student1.returnBook(book4);
    student1.returnBook(book5);
    student1.returnBook(book6);
    std::cout << "Number of currently issued books (expected: 0): " 
              << student1.getNumberOfIssuedBooks() << std::endl;
    student1.returnBook(book1);    
    std::cout << "Number of currently issued books (expected: 0): " 
              << student1.getNumberOfIssuedBooks() << std::endl;

    // File manipulation test
    // Prepare a map of users (ssmap)
    ssmap users;
    users["john"] = student1.getHash() + student1.;
    users["anna"] = prof1.getHash();
    users["testuser"] = "testhash";

    // Save users to a file
    std::string filename = "users_test.json";
    User::saveUsers(users, filename);
    std::cout << "Users saved to file: " << filename << std::endl;

    // Load users from the file
    ssmap loadedUsers = User::loadUsers(filename);
    std::cout << "Users loaded from file." << std::endl;

    // Check if the loaded data matches the original data
    if (loadedUsers == users) {
        std::cout << "File manipulation test passed! Loaded users match saved users." << std::endl;
    } else {
        std::cout << "File manipulation test failed! Loaded users do not match saved users." << std::endl;
    }

    // Clean up: delete the test file after use
    if (std::remove(filename.c_str()) == 0) {
        std::cout << "Test file " << filename << " deleted successfully." << std::endl;
    } else {
        std::cout << "Failed to delete test file " << filename << std::endl;
    }

    std::cout << "\n========================================================================"
              << "\n============================ Test Completed ============================"
              << "\n========================================================================\n\n\n";
}









