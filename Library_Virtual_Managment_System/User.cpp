// User classes implementations
// Author: WKK

#include "User.h"

/////////////////////
// USER (Abstract) //
/////////////////////

// Initialize static masterKey
std::string User::masterKey = "31415926";

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
: name(username), limit(limit), numOfCurrentlyIssued(0), issuedBooks({}) 
{ hash = hashPassword(password); }
// Constructor from json
User::User(const json& data, const std::string& username) : name(username){
    hash = data["hash"];
    limit = data["limit"];
    numOfCurrentlyIssued = ["numOfCurrentlyIssued"];
    for(std::string strbk : data["issuedBooks"]) 
    { issuedBooks.push_back(Book::fromEncryptedString(strbk)); }
}

// Other methods
bool User::login(const std::string& password){
    // Debug print
    //std::cout << "\n\n\nhash: " << hash << "\npassword: " 
    //<< password << "\nhashed password: " << hashPassword(password) << "\n\n\n";
    if (hash == hashPassword(password)) return true;
    else return false;
}
bool User::issueBook(const Book& book){
    issuedBooks.push_back(book);
    numOfCurrentlyIssued++;
    return false;
}
bool User::returnBook(const Book& book){
    unsigned int index = Shelf::getIndex(issuedBooks, book);
    if(index == -1) return false;
    issuedBooks.erase(issuedBooks.begin() + index);
    numOfCurrentlyIssued--;
    return true;
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
unsigned int User::getNumberOfIssuedBooks() const { return issuedBooks.size(); }

// Destructor (pure virtual method)
User::~User() {}

// Database manipulation
//
// User data will be stored in JSON file.
// Each entry will contain two fields:
// - hashed (SHA256) username 
// - encrypted (AES) user data
// User data will be stored as json string.

// Padding function (PKCS#7)
void User::pad(unsigned char* data, int block_size){
    int data_len = sizeof(data) / sizeof(char);
    int padding_len = block_size - (data_len % block_size);
    for(int i = 0; i < padding_len; i++) data[data_len + i] = padding_len;  
}
// Unpadding function (PKCS#7)
int User::unpad(unsigned char* data){
    int data_len = sizeof(data) / sizeof(char);
    return data_len - data[data_len - 1]
}
// AES CBC encryption (padded data)
void User::encrypt(const unsigned char* plaintext, unsigned char* ciphertext, 
                   const unsigned char* key, unsigned char* ini_vector){
    AES_KEY aesKey;
    int plain_len = sizeof(plaintext) / sizeof(char);

    // Encrypt key
    if (AES_set_encrypt_key(key, 128, &aesKey) != 0) {
        std::cerr << "Error setting encryption key!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Encryption
    AES_cbc_encrypt(plaintext, ciphertext, plain_len, &aesKey, ini_vector, AES_ENCRYPT);
}
// AES CBC decryption (with padding)
void User::decrypt(const unsigned char* ciphertext,
                   unsigned char* decryptedtext, 
                   const unsigned char* key, unsigned char* ini_vector){
    AES_KEY aesKey;
    int cipher_len = sizeof(ciphertext) / sizeof(char);

    // Key
    if (AES_set_decrypt_key(key, 128, &aesKey) != 0) {
        std::cerr << "Error setting decryption key!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Decryption
    AES_cbc_encrypt(ciphertext, decryptedtext, cipher_len, &aesKey, ini_vector, AES_DECRYPT);
}
// Get hashed usernames from file
svec User::loadHashedUsers(const std::string& filename){
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open the file: " << filename << std::endl;
        exit();
    }
    json j; 
    file >> j;

    svec users;
    if (j.contains("users") && j["users"].is_array()) {
        for (const auto& user : j["users"]) if (user.contains("username")) 
        { usernames.push_back(user["username"]); }
    } else { std::cerr << "Invalid JSON structure or missing 'users' array." << std::endl; }

    return users;
}
// Get users data by Username
json User::loadUserData(const std::string& username, const std::string& filename){
    std::ifstream file(filename);
    json j, k;

    file >> j;
    for(auto& [json_username, data] : j.items()){
        if(json_username == username){
            decrypt(data) >> k;
            return k;
        }
    }
    std::cerr << "User: " << username << " not found." << filename << std::endl;
}
// ######################
void User::saveUsers(const ssmap& users, const std::string& filename){
    std::ofstream file(filename);
    json j;

    for(const auto& [username, data] : users) j[username] = data; // decrypt it later
    file << j.dump(8);
}



///////////////////////
// STUDENT (Derived) //
///////////////////////

// Constructors with explicit call of the base constructor
Student::Student(const std::string& username, const std::string& hash, 
                 const unsigned int current, const std::vector<Book>& books)
: User(username, hash, 5, current, books) {}

// New user constructor (isNewFlag is not used directly, it's function is to make overloading unamiguous)
Student::Student(const std::string& username, const std::string& password, const bool isNewFlag)
: User(username, password, 5) {
    // Debug print:
    //std::cout << "New user constructor was called" << std::endl;
}

// Other methods
bool Student::issueBook(const Book& book) {
    if (numOfCurrentlyIssued >= limit) return false;
    User::issueBook(book);
    return true;
}

// Destructor
Student::~Student() {}

/////////////////////////
// PROFESSOR (Derived) //
/////////////////////////

// Constructors
Professor::Professor(const std::string& username, const std::string& hash,
                     const unsigned int current, const std::vector<Book>& books)
: User(username, hash, 10, current, books) {}
Professor::Professor(const std::string& username, const std::string& password, const bool isNewFlag)
: User(username, password, 10) {}

// Other mathods
bool Professor::issueBook(const Book& book) {
    if (numOfCurrentlyIssued >= limit) return false;
    User::issueBook(book);
    return true;
}

// Destructor
Professor::~Professor() {}