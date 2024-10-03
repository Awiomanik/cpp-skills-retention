// Book class and Shelf class declarations
// Author WKK

#ifndef BOOK_H // Check wether the class was not already defined
#define BOOK_H 

#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <optional>
#include <vector>
#include <fstream>

class Book {
private:
    std::string title;
    std::string author;
    std::string ISBN;
    bool isAvailable;
    std::optional<std::chrono::system_clock::time_point> dueDate;

    static unsigned int bookCount;

public:
    // Default constructor with initializer list
    Book();
    // Constructor with initializer list
    Book(const std::string title, const std::string author, const std::string ISBN);
    // Complete constructor
    Book::Book(const std::string title, const std::string author, const std::string ISBN, 
    const bool isAvailable, const std::optional<std::chrono::system_clock::time_point> dueDate);


    // Accessors and Mutators
    std::string getTitle() const;
    void setTitle(const std::string& newTitle);
    std::string getAuthor() const;
    void setAuthor(const std::string& newAuthor);
    std::string getISBN() const;
    void setISBN(const std::string newISBN);
    bool getAvailability() const;
    void setAvailability(const bool availability);
    void toggleAvailibility();
    std::string getDueDate() const;
    void setDueDate(unsigned int daysFromNow);
    void clearDueDate();

    // Other Methods
    void displayDetails() const;
    std::string issueBook(int howManyDays);
    void returnBook();
};

class Shelf{
// Singleton class for holding all of the books
private:
    static Shelf* instance; // Static pointer to the single instance of itself
    std::vector<Book> books;

    // Constructor (private to prevent instantiation from outside the class)
    Shelf() {}

public:
    // Public static method to get Shelf instnce
    static Shelf* getInstance();

    // Prevent copying and assigning the shelf
    Shelf(const Shelf&) = delete; // Deleting copy constructor
    Shelf& operator=(const Shelf&) = delete; // Deleting assignment operator

    // Other methods
    std::string encode(const std::string& text2Encode);
    std::string decode(const std::string& text2Decode);
    void loadBooksFromFile(const std::string filename);
    void saveBooksToFile(const std::string filename) const;

    //Accessor
    std::vector<Book> getBooks() const; 
};

#endif // BOOK_H
