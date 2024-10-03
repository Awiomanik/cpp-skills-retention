// Book class and Shelf class implementations
// Author: WKK

#include "Book.h"

// BOOK CLASS //

// Initialize static book counter
unsigned int Book::bookCount = 0;

// Default constructor with initializer list
Book::Book() : title("-"), author("-"), ISBN("-"), isAvailable(true), dueDate(std::nullopt)
{ std::cout << "Uninitilized book was created.\n"; bookCount++; }
// Constructor of a new book
Book::Book(const std::string title, const std::string author, const std::string ISBN)
: title(title), author(author), ISBN(ISBN), isAvailable(true), dueDate(std::nullopt)
{ std::cout << "\"" << title << "\" book was created.\n"; bookCount++; }
// Complete constructor
Book::Book(const std::string title, const std::string author, const std::string ISBN, 
const bool isAvailable, const std::optional<std::chrono::system_clock::time_point> dueDate)
: title(title), author(author), ISBN(ISBN), isAvailable(isAvailable), dueDate(dueDate)
{ std::cout << "Uninitilized book was constructed from data.\n"; bookCount++; }

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

// SHELF CLASS //

// Initialize instance to null-pointer
Shelf* Shelf::instance = nullptr;

// Public static method to get Shelf instnce
Shelf* Shelf::getInstance(){
    if (instance == nullptr) instance = new Shelf();
    return instance;
}

// Other methods
std::string Shelf::encode(const std::string& text2Encode){
    std::ostringstream encoded;
    for (char ch : text2Encode) encoded << static_cast<int>(ch) << " ";
    return encoded.str();
}
std::string Shelf::decode(const std::string& text2Decode){
    std::istringstream encoded(text2Decode);
    std::string asciiCharacter;
    std::ostringstream decoded;

    // default dalimeter for string extraction (>>) is whitespace
    while (encoded >> asciiCharacter) decoded << static_cast<char>(std::stoi(asciiCharacter));

    return decoded.str();
}

void Shelf::loadBooksFromFile(const std::string filename = "storage/book_data.dat"){
    std::ifstream file(filename);
    if (file.is_open()){
        std::string bookTitle, bookAuthor, bookISBN, bookAvailability, bookDueDate;
        bool availability;
        std::optional<std::chrono::system_clock::time_point> date;

        while(file >> bookTitle >> bookAuthor >> bookISBN >> bookAvailability >> bookDueDate){
            // Availability
            if (bookAvailability == "true") availability = true;
            else availability = false;
            // Due date
            if (bookDueDate == "-1") date = std::nullopt;
            else date = std::chrono::system_clock::from_time_t(std::stol(bookDueDate));

            // Append book 
            books.push_back(Book(bookTitle, bookAuthor, bookISBN, availability, date));
        }
    } else std::cerr << "Error: Could not open file " << filename << std::endl;
}
void Shelf::saveBooksToFile(const std::string filename = "storage/book_data.dat") const{
    
}

//Accessor
std::vector<Book> Shelf::getBooks() const { return books; } 









