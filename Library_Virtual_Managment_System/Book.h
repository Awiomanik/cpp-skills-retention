// Book class declaration
// Author WKK

#ifndef BOOK_H // Check wether the class was not already defined
#define BOOK_H 

#include <iostream>
#include <string>
#include <chrono>
#include <optional>

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
    Book(std::string title, std::string author, std::string ISBN);

    /// Accessors and Mutators
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

#endif // BOOK_H
