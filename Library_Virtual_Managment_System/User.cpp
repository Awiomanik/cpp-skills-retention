// User classes implementations
// Author: WKK

#include "User.h"

/////////////////////
// USER (Abstract) //
/////////////////////

// Initialize static masterKey
std::string User::masterKey = "1234"; // temporary

// Hashing method
std::string User::hashPassword(const std::string& password){
    unsigned char hash[SHA256_DIGEST_LENGTH]; // 256 bits (32 bytes) buffer for sha hash
    // Computing hash, SHA256 requires a const unsigned char*, therefor casting
    SHA256(reinterpret_cast<const unsigned char*>(password.c_str()), password.size(), hash);
    // Converting the raw binary hash into human readable hexadecimal format
    std::ostringstream oss; // ostreamstring to build string easier and with stream-like interface (<<)
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    { oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]); }
    // build string and return hash
    return oss.str();
    }

// Constructor
User::User(const std::string& username, const std::string& password,
           unsigned int limit, unsigned int current,
           const std::vector<Book>& issuedBooks)
: name(username), limit(limit), current(current), 
{ hash = hashPassword(password); }

// Other methods
bool User::login(const std::string& password){
    if (hash == hashPassword(password)) return true;
    else return false;
}
void User::issueBook(const Book& book){
    IssuedBooks.push_back(book);
    numOfCurrentlyIssued++;
}
void User::returnBook(const Book& book){
    IssuedBooks.erase(std::find(IssuedBooks.begin(), IssuedBooks.end(), book));
    numOfCurrentlyIssued--;
}

// Reseting password with master key
bool User::resetPassword(const std::string& masterKeyIn, const std::string& newPassword){
    if (masterKey == masterKeyIn){
        hash = hashPassword(newPassword);
        return true;
    }
    else return false;
}

// Accessors and Mutators
std::string User::getUserName() const { return name; }
std::string User::getHash() const { return hash; }

// Destructor (pure virtual method)
User::~User() {}

///////////////////////
// STUDENT (Derived) //
///////////////////////

// Constructors with explicit call of the base constructor to ensure it's execution
Student::Student(const std::string& username, const std::string& password, 
        unsigned int bookLimit, unsigned int current)
    : User(username, password), limit(bookLimit), numOfCurrentlyIssued(current) {}

// Other methods
bool Student::issueBook(const Book& book) {
    if (numOfCurrentlyIssued >= limit) return false;
    User::issueBook(book);
    return true;
}

// Destructor
Student::~Student() {}

