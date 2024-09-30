// Tests implementation
// Author: WKK

#include "Book.h"
#include "tests.h"

void bookClassTest()
{
    std::cout << "\n=== Book Class Test ===\n\n";

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

    std::cout << "\n=== Test Completed ===\n";
}