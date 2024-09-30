// Book class implementation
// Author: WKK

#include "Book.h"

// Initialize static book counter
unsigned int Book::bookCount = 0;

// Default constructor with initializer list
Book::Book() : title("-"), author("-"), ISBN("-"), isAvailable(true), dueDate(std::nullopt)
{ std::cout << "Uninitilized book was created.\n"; bookCount++; }

// Constructor with initializer list
Book::Book(std::string title, std::string author, std::string ISBN)
: title(title), author(author), ISBN(ISBN), isAvailable(true), dueDate(std::nullopt)
{ std::cout << "\"" << title << "\" book was created.\n"; bookCount++; }

// Accessors and Mutators
std::string Book::getTitle() const { return title; }
void Book::setTitle(const std::string& newTitle) { title = newTitle; }

std::string Book::getAuthor() const { return author; }
void Book::setAuthor(const std::string& newAuthor) { author = newAuthor; }

std::string Book::getISBN() const { return ISBN; }
void Book::setISBN(const std::string newISBN) { ISBN = newISBN; }

bool Book::getAvailability() const { return isAvailable; }
void Book::setAvailability(const bool availability) { isAvailable = availability; }
void Book::toggleAvailibility() { isAvailable = !isAvailable; }

std::string Book::getDueDate() const
{
    if (dueDate)
    {
        time_t dueDate_time_t = std::chrono::system_clock::to_time_t(*dueDate); // Conversion to time_t
        tm* dueDate_tm = localtime(&dueDate_time_t); // conversion to tm struct
        char buffer[15]; //"%Y-%m-%d" is 10 characters + Null terminator + 4 for extra safety
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", dueDate_tm); // Convert to charr array
        return std::string(buffer); // Convert to string
    }
    return "No due date set.";
}
void Book::setDueDate(unsigned int daysFromNow) 
{ dueDate = std::chrono::system_clock::now() + std::chrono::hours(24 * daysFromNow); }
void Book::clearDueDate() { dueDate = std::nullopt; }

// Other methods
void Book::displayDetails() const
{
    std::string temp_availability_string;
    if (isAvailable) {temp_availability_string = " is available.\n"; }
    else {temp_availability_string = " is not available due " + getDueDate() + ".\n"; }

    std::cout << "\"" << title << "\" by " << author << " (ISBN: "<< ISBN << ")" 
    << temp_availability_string;
}
std::string Book::issueBook(int howManyDays)
{
    if (getAvailability())
    {
        toggleAvailibility();
        setDueDate(howManyDays);
        return "Book succesfully issued due " + getDueDate() + ".\n";
    }
    return "Book is not available at the moment. Due date: " + getDueDate() + ".\n";
}
void Book::returnBook() { clearDueDate(); setAvailability(true); }