// User classes declarations
// Author: WKK

#ifndef USER_H
#define USER_H

#include <string>
#include <sstream>
#include <iomanip> // For std::hex, std::setw, std::setfill
#include <iomanip>
#include <openssl/sha.h>

class User {
private:
    std::string name;
    std::string hash;

    static std::string masterKey;

    // Hashing method
    std::string hashPassword(const std::string& password);

public:
    // Constructor
    User(const std::string& username, const std::string& password);

    bool login(const std::string& password);

    // Reseting password with master key
    bool resetPassword(const std::string& masterKeyIn, const std::string& newPassword);

    // Accessors and Mutators
    std::string getUserName() const;
};

#endif //USER_H