// User classes implementations
// Author: WKK

#include "User.h"

/////////////////////
// USER (Abstract) //
/////////////////////

// Initialize static masterKey
std::string User::masterKey = "31415926"; // temporary

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

// Constructor from all data
User::User(const std::string& username, const std::string& hash, const unsigned int limit,
           const unsigned int current, const std::vector<Book>& issuedBooks)
: name(username), hash(hash),
limit(limit), numOfCurrentlyIssued(current), issuedBooks(issuedBooks) {}

// New user onstructor
User::User(const std::string& username, const std::string& password, const unsigned int limit) 
: name(username), limit(limit), numOfCurrentlyIssued(0), issuedBooks(std::vector<Book>()) 
{ hash = password; }

// Other methods
bool User::login(const std::string& password){
    if (hash == hashPassword(password)) return true;
    else return false;
}
bool User::issueBook(const Book& book){
    issuedBooks.push_back(book);
    numOfCurrentlyIssued++;
    return false;
}
void User::returnBook(const Book& book){
    issuedBooks.erase(std::find(issuedBooks.begin(), issuedBooks.end(), book));
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

// Constructors with explicit call of the base constructor
Student::Student(const std::string& username, const std::string& hash, 
                 unsigned int current, std::vector<Book> books)
: User(username, hash, 5, current, books) {}

// New user constructor
Student::Student(const std::string& username, const std::string& password, bool isNewFlag)
: User(username, password, 5) {}

// Other methods
bool Student::issueBook(const Book& book) {
    if (numOfCurrentlyIssued >= limit) return false;
    User::issueBook(book);
    return true;
}

// Destructor
Student::~Student() { delete issuedBooks; }

