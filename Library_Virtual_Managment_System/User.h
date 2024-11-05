// User classes declarations
// Author: WKK

#ifndef USER_H
#define USER_H

#include <string>
#include <sstream>
#include <iomanip> // For std::hex, std::setw, std::setfill
#include <iomanip>
#include <vector>
#include <nlohmann/json.hpp>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/evp.h>

#include "Book.h"

using json = nlohmann::json;
using svec = std::vector<std::string>;

class User {
// Abstract class for users
private:
    static std::string masterKey;
    static unsigned char encryptionKey[16];

    // Hashing method
    static std::string hashPassword(const std::string& password);

protected:
    std::string name;
    std::string hash;
    const unsigned int limit;
    unsigned int numOfCurrentlyIssued;
    std::vector<Book> issuedBooks;

public:
    // Constructor from all data
    User(const std::string& username, const std::string& hash, const unsigned int limit,
         const unsigned int current, const std::vector<Book>& issuedBooks);
    // New user constructor 
    User(const std::string& username, const std::string& password, const unsigned int limit);
    // Constrictor from json
    User(const json& encryptedData, const std::string& hashedUsername);

    // Other methods
    bool login(const std::string& password);
    virtual bool issueBook(const Book& book);
    bool returnBook(const Book& book);

    // Destructor (pure virtual method to ensure it's override and make USer abstract)
    virtual ~User() = 0;

    // Reseting password with master key
    bool resetPassword(const std::string& masterKeyIn, const std::string& newPassword);

    // Accessors and Mutators
    std::string getUserName() const;
    std::string getHash() const;
    unsigned int getNumberOfIssuedBooks() const;

    // Database manipulation
    //
    // User data will be stored in JSON file.
    // Each entry will contain two fields:
    // - hashed (SHA256) username 
    // - encrypted (AES) user data
    // User data will be stored as json string.
private:
    // Padding function (PKCS#7)
    static void pad(std::vector<unsigned char>& data, int block_size = 16);
    // Unpadding function (PKCS#7)
    static int unpad(const std::vector<unsigned char>& data);
    static void encrypt(const std::vector<unsigned char>& plaintext, 
                        std::vector<unsigned char>& ciphertext, const unsigned char* key);
    // AES CBC decryption (with padding)
    static void decrypt(const unsigned char* ciphertext, 
                        unsigned char* decryptedtext, 
                        const unsigned char* key,
                        size_t ciphertextSize);
public:
    // Get hashed usernames from file
    static svec loadHashedUsers(const std::string& filename = "user_data.dat");
    // Get users data by Username
    static json loadUserData(const std::string& username, 
                             const std::string& filename = "user_data.dat");
    // Add User to database
    void addUserToDatabase(const std::string& filename = "user_data.dat");
};

class Student : public User {
public:
    // Constructor
    Student(const std::string& username, const std::string& hash, 
            const unsigned int current = 0, const std::vector<Book>& books = {});

    // New user constructor (Must be used with isNewFlag)
    Student(const std::string& username, const std::string& password, const bool isNewFlag);

    // Other methods
    bool issueBook(const Book& book) override;

    // Destructor
    ~Student();
};

class Professor : public User {
public:
    // Constructor
    Professor(const std::string& username, const std::string& hash, 
              const unsigned int current = 0, const std::vector<Book>& books = {});

    // New user constructor (Must be used with isNewFlag)
    Professor(const std::string& username, const std::string& password, const bool isNewFlag);

    // Other methods
    bool issueBook(const Book& book) override;

    // Destructor
    ~Professor();
};


#endif //USER_H