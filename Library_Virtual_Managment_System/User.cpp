// User classes implementations
// Author: WKK

#include "User.h"

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
User::User(const std::string& username, const std::string& password): name(username)
{ hash = password; } //temporary

bool User::login(const std::string& password){
    if (hash == password) return true; // temporary
    else return false;
}

// Reseting password with master key
bool User::resetPassword(const std::string& masterKeyIn, const std::string& newPassword){
    if (masterKey == masterKeyIn){
        hash = newPassword; // temporary
        return true;
    }
    else return false;
}

// Accessors and Mutators
std::string User::getUserName() const
{ return name; }