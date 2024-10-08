// User classes declarations
// Author: WKK

#ifndef USER_H
#define USER_H

#include <string>
#include <sstream>
#include <iomanip> // For std::hex, std::setw, std::setfill
#include <iomanip>
#include <vector>
#include <openssl/sha.h>

#include "Book.h"

class User {
// Abstract class for users
private:
    std::string name;
    std::string hash;
    std::vector<Book> issuedBooks;
    unsigned int numOfCurrentlyIssued;
    const unsigned int limit;

    static std::string masterKey;

    // Hashing method
    std::string hashPassword(const std::string& password);    

public:
    // Constructor
    User(const std::string& username, const std::string& password,
         unsigned int limit = 5, unsigned int current = 0,
         const std::vector<Book>& issuedBooks = {});

    // Other methods
    bool login(const std::string& password);
    virtual void issueBook(const Book& book);
    void returnBook(const Book& book);

    // Reseting password with master key
    bool resetPassword(const std::string& masterKeyIn, const std::string& newPassword);

    // Accessors and Mutators
    std::string getUserName() const;
    std::string getHash() const;

    // Destructor (pure virtual method to ensure it's override and make USer abstract)
    virtual ~User() = 0;
};

class Student : public User {
public:
    const unsigned int limit;

    // Constructor
    Student(const std::string& username, const std::string& password, 
            unsigned int bookLimit = 5, unsigned int current = 0);

    // Other methods
    bool issueBook(const Book& book) override;

    // Destructor
    ~Student();
};


#endif //USER_H