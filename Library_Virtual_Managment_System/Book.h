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

public:
    // Default constructor with initializer list
    Book();
    // Constructor with initializer list
    Book(const std::string title, const std::string author, const std::string ISBN);
    // Complete constructor
    Book(const std::string title, const std::string author, const std::string ISBN, 
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
    bool operator==(const Book& other) const;
};

class Shelf{
// Singleton class for holding all of the books
private:
    static Shelf* instance; // Static pointer to the single instance of itself
    std::vector<Book> books;
    static unsigned int bookCount;

    // Constructor (private to prevent instantiation from outside the class)
    Shelf() {}

public:
    // Public static methods to get Shelf instnce and to destroy it
    static Shelf* getInstance();

    // Destructor
    ~Shelf();

    // Prevent copying and assigning the shelf
    Shelf(const Shelf&) = delete; // Deleting copy constructor
    Shelf& operator=(const Shelf&) = delete; // Deleting assignment operator

    // Other methods
    std::string asciiEncode(const std::string& text2Encode) const;
    std::string asciiDecode(const std::string& text2Decode) const;
    void loadBooksFromFile(const std::string filename);
    void saveBooksToFile(const std::string filename) const;

    // Accessors and Mutators
    void addBooks(const std::initializer_list<Book>& newBooks);
    void clearBooks();
    std::vector<Book> getBooks() const; 
    unsigned int getBookCount() const;
};

#endif // BOOK_H
