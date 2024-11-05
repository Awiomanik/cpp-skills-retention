// User classes implementations
// Author: WKK

#include "User.h"

/////////////////////
// USER (Abstract) //
/////////////////////

// Initialize static masterKey and encryptionKey
std::string User::masterKey = "31415926";
unsigned char User::encryptionKey[16] = {2, 7, 1, 8, 2, 8, 1, 8,
                                         2, 8, 4, 5, 9, 0, 4, 5};

// Hashing method
// Hash a given password using SHA-256
// Converts the input password into a fixed-size 256-bit hash.
// Returns the hash as a hexadecimal string.
// - password: The plaintext password input by the user.
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

// Constructor to create a user object when all user data is available.
// This constructor is used for loading user information from files.
User::User(const std::string& username, const std::string& hash, const unsigned int limit,
           const unsigned int current, const std::vector<Book>& issuedBooks)
: name(username), hash(hash),
limit(limit), numOfCurrentlyIssued(current), issuedBooks(issuedBooks) {}
// Constructor for creating a new user.
// Initializes the user object and hashes the password.
// - username: New user's name.
// - password: New user's password.
// - limit: Maximum number of books the user can issue.
User::User(const std::string& username, const std::string& password, const unsigned int limit) 
: name(username), limit(limit), numOfCurrentlyIssued(0), issuedBooks({}) 
{ hash = hashPassword(password); }
// Constructor for loading a user from JSON data.
// - data: The JSON object that contains user data.
// - username: The name of the user to load.
User::User(const json& data, const std::string& username) 
: name(username), limit(data["limit"]), hash(data["hash"]), 
    numOfCurrentlyIssued(data["numOfCurrentlyIssued"]){
    for(const std::string& strbk : data["issuedBooks"]) 
    { issuedBooks.push_back(Book::fromEncryptedString(strbk)); }
}

// Other methods
// Method to verify if the input password matches the user's stored password.
// Compares the SHA-256 hash of the input password with the stored hash.
bool User::login(const std::string& password){
    // Debug print
    //std::cout << "\n\n\nhash: " << hash << "\npassword: " 
    //<< password << "\nhashed password: " << hashPassword(password) << "\n\n\n";
    if (hash == hashPassword(password)) return true;
    else return false;
}
// Method to issue a new book to the user.
// - book: The Book object to be issued.
// Increments the number of issued books.
bool User::issueBook(const Book& book){
    issuedBooks.push_back(book);
    numOfCurrentlyIssued++;
    return false;
}
// Method to return a book to the library.
// - book: The Book object to be returned.
// Removes the book from the issued books list and decrements the count.
bool User::returnBook(const Book& book){
    unsigned int index = Shelf::getIndex(issuedBooks, book);
    if(index == -1) return false;
    issuedBooks.erase(issuedBooks.begin() + index);
    numOfCurrentlyIssued--;
    return true;
}

// Method to reset the user's password using the master key.
// - masterKeyIn: Input master key for authentication.
// - newPassword: The new password to set.
// If the master key matches, the user's password is updated.
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

// Destructor (pure virtual method to ensure the class remains abstract)
User::~User() {}

// Database manipulation
//
// User data will be stored in JSON file.
// Each entry will contain two fields:
// - hashed (SHA256) username 
// - encrypted (AES) user data
// User data will be stored as json string.

// Padding function to implement PKCS#7 padding for AES encryption.
// Pads the input data to a size that is a multiple of the AES block size (16 bytes).
void User::pad(std::vector<unsigned char>& data, int block_size) {
    int padding_len = block_size - (data.size() % block_size);
    for (int i = 0; i < padding_len; i++) {
        data.push_back(padding_len);
    }
}
// Function to remove PKCS#7 padding after decryption.
// Returns the size of the unpadded data.
int User::unpad(const std::vector<unsigned char>& data) { return data.size() - data.back(); }
// AES encryption in CBC mode with padding.
// Encrypts the given plaintext using AES CBC mode and appends an initialization vector.
// - plaintext: The data to encrypt.
// - ciphertext: The resulting encrypted data.
// - key: The AES key for encryption.
void User::encrypt(const std::vector<unsigned char>& plaintext, 
                   std::vector<unsigned char>& ciphertext, 
                   const unsigned char* key) {
    // Create and initialize the context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        std::cerr << "Error creating EVP_CIPHER_CTX!" << std::endl;
        exit(EXIT_FAILURE);
    }
    //std::cout << "EVP_CIPHER_CTX created successfully." << std::endl;

    // Pad plain text to be a multiple of block size (16 bytes)
    std::vector<unsigned char> paddedPlainText = plaintext;
    pad(paddedPlainText);
    //std::cout << "Plaintext after padding: " 
    //          << std::string(paddedPlainText.begin(), paddedPlainText.end()) 
    //          << std::endl;

    // Generate random initial vector (IV)
    unsigned char ini_vector[EVP_MAX_IV_LENGTH];
    if (!RAND_bytes(ini_vector, sizeof(ini_vector))) {
        std::cerr << "Error generating random IV!" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }
    //std::cout << "Generated IV: ";
    //for (int i = 0; i < 16; i++) std::cout << std::hex << (int)ini_vector[i] << " ";
    //std::cout << std::endl;

    // Initialize encryption operation
    if (EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, ini_vector) != 1) {
        std::cerr << "Error initializing encryption!" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }
    //std::cout << "Encryption initialized successfully." << std::endl;

    // Allocate memory for ciphertext: IV + padded plaintext + potential padding
    size_t padding_len = EVP_CIPHER_block_size(EVP_aes_128_cbc());
    ciphertext.resize(16 + paddedPlainText.size() + padding_len); // IV + plaintext + padding space
    memcpy(ciphertext.data(), ini_vector, 16); // Store the IV at the beginning
    //std::cout << "Ciphertext size after resizing: " << ciphertext.size() << std::endl;

    // Provide the plaintext to be encrypted, and get the encrypted output
    int len = 0;
    if (EVP_EncryptUpdate(ctx, ciphertext.data() + 16, &len, 
                          paddedPlainText.data(), paddedPlainText.size()) != 1) {
        std::cerr << "Error during encryption update!" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }
    //std::cout << "Encryption update successful. Data length: " << len << std::endl;

    // Finalize the encryption
    int ciphertext_len = 0;
    if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + 16 + len, &ciphertext_len) != 1) {
        std::cerr << "Error during encryption finalization!" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }
    //std::cout << "Encryption finalized. Final length: " << ciphertext_len << std::endl;

    // Adjust the size of the ciphertext to reflect the actual length
    //std::cout << "Resizing ciphertext. len = " << len << ", ciphertext_len = " << ciphertext_len << std::endl;
    if (len < 0 || ciphertext_len < 0) {
        std::cerr << "Invalid lengths detected: len = " << len << ", ciphertext_len = " << ciphertext_len << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }

    // Resize to the total size: IV + encrypted data
    size_t total_size = 16 + len + ciphertext_len;
    if (total_size > SIZE_MAX) {
        std::cerr << "New size is too large!" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }
    ciphertext.resize(total_size);
    //std::cout << "Ciphertext size after finalization: " << ciphertext.size() << std::endl;

    // Free the context
    EVP_CIPHER_CTX_free(ctx);
    //std::cout << "Encryption context freed." << std::endl;
}
// AES CBC decryption (with padding)
// Decrypts the ciphertext using AES in CBC mode with PKCS#7 padding.
// - ciphertext: The encrypted input data.
// - decryptedtext: The output buffer for the decrypted data.
// - key: The AES key for decryption.
void User::decrypt(const unsigned char* ciphertext, 
                   unsigned char* decryptedtext, 
                   const unsigned char* key,
                   size_t ciphertextSize) {
    // Create and initialize the context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        std::cerr << "Error creating EVP_CIPHER_CTX!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Extract the IV from the ciphertext
    unsigned char ini_vector[EVP_MAX_IV_LENGTH];
    memcpy(ini_vector, ciphertext, 16); // First 16 bytes are the IV

    // Initialize decryption operation
    if (EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, ini_vector) != 1) {
        std::cerr << "Error initializing decryption!" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }

    // Create a buffer for the decrypted data
    std::vector<unsigned char> decryptedData(ciphertextSize - 16); // Exclude IV
    int len;

    // Decrypt the ciphertext (excluding the IV)
    if (EVP_DecryptUpdate(ctx, decryptedData.data(), &len, ciphertext + 16, ciphertextSize - 16) != 1) {
        std::cerr << "Error during decryption update!" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }

    // Finalize the decryption
    int decrypted_len;
    if (EVP_DecryptFinal_ex(ctx, decryptedData.data() + len, &decrypted_len) != 1) {
        std::cerr << "Error during decryption finalization!" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }

    // Adjust the size of the decrypted data
    decryptedData.resize(len + decrypted_len);

    // Copy the decrypted data to the output buffer
    memcpy(decryptedtext, decryptedData.data(), decryptedData.size());

    // Free the context
    EVP_CIPHER_CTX_free(ctx);
}
// Load hashed usernames from file
svec User::loadHashedUsers(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Could not open the file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    svec users;
    while (true) {
        std::string hash;
        std::getline(file, hash, '\0'); // Read until null terminator

        if (hash.empty()) break; // Break if no more hashes

        // Debug output for each username read
        std::cout << "Loaded hash: " << hash << std::endl;

        users.push_back(hash);
    }

    file.close();
    return users;
}
// Get user data by username
json User::loadUserData(const std::string& username, const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Could not open the file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    while (true) {
        // Read the username
        std::string hash;
        std::getline(file, hash, '\0'); // Read until null terminator

        // Debug output for each username read
        std::cout << "Checking hash: " << hash << std::endl;

        if (hash == username) {
            // Read the length of the encrypted data
            size_t cipherSize;
            file.read(reinterpret_cast<char*>(&cipherSize), sizeof(cipherSize));

            std::vector<unsigned char> ciphertext(cipherSize);
            file.read(reinterpret_cast<char*>(ciphertext.data()), cipherSize);

            // Decrypt the data
            std::vector<unsigned char> decrypted_data(cipherSize);
            decrypt(ciphertext.data(), decrypted_data.data(), encryptionKey, cipherSize);
            std::string decrypted_str(decrypted_data.begin(), decrypted_data.end());

            file.close();
            return json::parse(decrypted_str);
        }

        // Skip the encrypted data length and data
        size_t cipherSize;
        file.read(reinterpret_cast<char*>(&cipherSize), sizeof(cipherSize));
        if (!file) break; // Break if end of file or error
        file.seekg(cipherSize, std::ios::cur); // Skip the cipher data
    }

    std::cerr << "User: " << username << " not found in file: " << filename << std::endl;
    file.close();
    return json{};
}
// Adds the current user's data (name, hashed password, limits, issued books, etc.) to the user database.
void User::addUserToDatabase(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary | std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // Prepare user data for encryption
    json k;
    k["hash"] = hash;
    k["limit"] = limit;
    k["numOfCurrentlyIssued"] = numOfCurrentlyIssued;

    // Serialize issued books
    svec issuedBooksString;
    for (const Book& bk : issuedBooks) {
        issuedBooksString.push_back(bk.toEncryptedString());
    }
    k["issuedBooks"] = issuedBooksString;

    // Serialize and encrypt data
    std::string serializedData = k.dump();
    std::vector<unsigned char> plaintext(serializedData.begin(), serializedData.end());
    std::vector<unsigned char> ciphertext;

    // Encrypt the data
    encrypt(plaintext, ciphertext, encryptionKey);

    // Write the username hash
    size_t hashSize = hash.size();
    file.write(hash.c_str(), hashSize);
    file.put('\0'); // Null-terminate for separation

    // Write the length of the ciphertext
    size_t cipherSize = ciphertext.size();
    file.write(reinterpret_cast<const char*>(&cipherSize), sizeof(cipherSize));
    
    // Write the ciphertext
    file.write(reinterpret_cast<const char*>(ciphertext.data()), cipherSize);

    std::cout << "User data saved to " << filename << std::endl;

    file.close();
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