// Book class and Shelf class implementations
// Author: WKK

#include "Book.h"

// BOOK CLASS //

// Default constructor with initializer list
Book::Book() : title("-"), author("-"), ISBN("-"), isAvailable(true), dueDate(std::nullopt)
{ std::cout << "Uninitilized book was created.\n"; }
// Constructor of a new book
Book::Book(const std::string title, const std::string author, const std::string ISBN)
: title(title), author(author), ISBN(ISBN), isAvailable(true), dueDate(std::nullopt)
{ std::cout << "\"" << title << "\" book was created.\n"; }
// Complete constructor
Book::Book(const std::string title, const std::string author, const std::string ISBN, 
const bool isAvailable, const std::optional<std::chrono::system_clock::time_point> dueDate)
: title(title), author(author), ISBN(ISBN), isAvailable(isAvailable), dueDate(dueDate)
{ std::cout << "\"" << title << "\" book was constructed from data.\n"; }

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
std::string Book::toEncryptedString() const{
    std::stringstream ss;

    ss << Shelf::asciiEncode(title) << " : "
       << Shelf::asciiEncode(author) << " : "
       << Shelf::asciiEncode(ISBN) << " : "
       << Shelf::asciiEncode(isAvailable ? "true" : "false") << " : "
       << Shelf::asciiEncode(this->getDueDate());

    return ss.str();

}
Book Book::fromEncryptedString(const std::string& encryptedBook){
    std::string bookTitle, bookAuthor, bookISBN, bookAvailability, bookDueDate;
    std::istringstream ss(encryptedBook);
    bool availability;
    std::optional<std::chrono::system_clock::time_point> date;

    std::getline(ss, bookTitle, ':');
    std::getline(ss, bookAuthor, ':');
    std::getline(ss, bookISBN, ':');
    std::getline(ss, bookAvailability, ':');
    std::getline(ss, bookDueDate, ':');

    // Availability
    bookAvailability = Shelf::asciiDecode(bookAvailability);
    if (bookAvailability == "true") availability = true;
    else availability = false;
    // Due date
    bookDueDate = Shelf::asciiDecode(bookDueDate);
    if (bookDueDate == "No due date set.") date = std::nullopt;
    else date = std::chrono::system_clock::from_time_t(std::stol(bookDueDate));

    // Construct book
    return Book(Shelf::asciiDecode(bookTitle), Shelf::asciiDecode(bookAuthor), 
                Shelf::asciiDecode(bookISBN), availability, date);
}
// Operators
bool Book::operator==(const Book& other) const
{ return title == other.title && author == other.author && ISBN == other.ISBN; }

// SHELF CLASS //

// Initialize static book counter
unsigned int Shelf::bookCount = 0;

// Initialize instance to null-pointer
Shelf* Shelf::instance = nullptr;

// Public static methods to get Shelf instnce and to destroy it
Shelf* Shelf::getInstance(){
    if (instance == nullptr) instance = new Shelf();
    return instance;
}

// Destroctor to release memory
Shelf::~Shelf() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

// Other methods
std::string Shelf::asciiEncode(const std::string& text2Encode){
    std::ostringstream encoded;
    for (char ch : text2Encode) encoded << static_cast<int>(ch) << " ";
    return encoded.str();
}
std::string Shelf::asciiDecode(const std::string& text2Decode){
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
        std::string bookTitle, bookAuthor, bookISBN, bookAvailability, bookDueDate, line;
        bool availability;
        std::optional<std::chrono::system_clock::time_point> date;

        while (getline(file, line)){
            std::istringstream lineStream(line);
            std::getline(lineStream, bookTitle, ':');
            std::getline(lineStream, bookAuthor, ':');
            std::getline(lineStream, bookISBN, ':');
            std::getline(lineStream, bookAvailability, ':');
            std::getline(lineStream, bookDueDate, ':');

            // Availability
            bookAvailability = asciiDecode(bookAvailability);
            if (bookAvailability == "true") availability = true;
            else availability = false;
            // Due date
            bookDueDate = asciiDecode(bookDueDate);
            if (bookDueDate == "No due date set.") date = std::nullopt;
            else date = std::chrono::system_clock::from_time_t(std::stol(bookDueDate));

            // Append book 
            books.push_back(Book(asciiDecode(bookTitle), asciiDecode(bookAuthor), 
            asciiDecode(bookISBN), availability, date));
        }
        file.close();

        bookCount = books.size();
        
    } else std::cerr << "Error: Could not open file " << filename << std::endl;
}
void Shelf::saveBooksToFile(const std::string filename = "storage/book_data.dat") const{
    std::ofstream file(filename); // Opens file in truncate mode by default

    if (file.is_open()){
        for (const Book& book : books){
            file << book.toEncryptedString() << std::endl;
        }
        file.close();

    } else std::cerr << "Error: Could not open file " << filename << std::endl;
}
unsigned int Shelf::getIndex(const std::vector<Book>& books, const Book& targetBook){
    for(size_t i = 0; i < books.size(); i++) if(books[i] == targetBook) return i;
    return -1;
}

//Accessor
void Shelf::addBooks(const std::initializer_list<Book>& newBooks) { 
    books.insert(books.end(), newBooks); 
    bookCount = bookCount + newBooks.size(); 
}
void Shelf::clearBooks() { books.clear(); bookCount = 0; }
std::vector<Book> Shelf::getBooks() const { return books; } 
unsigned int Shelf::getBookCount() const { return bookCount; }









