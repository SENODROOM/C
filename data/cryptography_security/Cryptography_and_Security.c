#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// =============================================================================
// BASIC CRYPTOGRAPHIC PRIMITIVES
// =============================================================================

#define BLOCK_SIZE 16
#define KEY_SIZE 16
#define MAX_DATA_SIZE 1024

// Simple XOR cipher
void xorCipher(const unsigned char* input, const unsigned char* key, 
               unsigned char* output, int length) {
    for (int i = 0; i < length; i++) {
        output[i] = input[i] ^ key[i % KEY_SIZE];
    }
}

// Caesar cipher
void caesarCipher(const char* input, char* output, int shift) {
    for (int i = 0; input[i]; i++) {
        if (isalpha(input[i])) {
            char base = isupper(input[i]) ? 'A' : 'a';
            output[i] = (input[i] - base + shift) % 26 + base;
        } else {
            output[i] = input[i];
        }
    }
    output[strlen(input)] = '\0';
}

// Reverse Caesar cipher
void caesarDecipher(const char* input, char* output, int shift) {
    caesarCipher(input, output, 26 - shift);
}

// =============================================================================
// HASH FUNCTIONS
// =============================================================================

// Simple hash function (for demonstration only)
unsigned int simpleHash(const char* str) {
    unsigned int hash = 5381;
    int c;
    
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    
    return hash;
}

// MD5-like hash function (simplified)
void simpleMD5(const char* input, unsigned char* hash) {
    // This is a simplified version for demonstration
    // Real MD5 is much more complex
    unsigned int h0 = 0x67452301;
    unsigned int h1 = 0xEFCDAB89;
    unsigned int h2 = 0x98BADCFE;
    unsigned int h3 = 0x10325476;
    
    int len = strlen(input);
    for (int i = 0; i < len; i++) {
        h0 = ((h0 << 1) | (h0 >> 31)) ^ input[i];
        h1 = ((h1 << 3) | (h1 >> 29)) ^ input[i];
        h2 = ((h2 << 5) | (h2 >> 27)) ^ input[i];
        h3 = ((h3 << 7) | (h3 >> 25)) ^ input[i];
    }
    
    // Store hash (16 bytes)
    for (int i = 0; i < 4; i++) {
        hash[i] = (h0 >> (i * 8)) & 0xFF;
        hash[i + 4] = (h1 >> (i * 8)) & 0xFF;
        hash[i + 8] = (h2 >> (i * 8)) & 0xFF;
        hash[i + 12] = (h3 >> (i * 8)) & 0xFF;
    }
}

// =============================================================================
// SYMMETRIC ENCRYPTION (SIMPLIFIED AES-LIKE)
// =============================================================================

typedef struct {
    unsigned char key[KEY_SIZE];
    unsigned char round_keys[11][16]; // AES-128 uses 11 round keys
} AESContext;

// S-Box for AES
static const unsigned char s_box[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

// Initialize AES context
void initAES(AESContext* ctx, const unsigned char* key) {
    memcpy(ctx->key, key, KEY_SIZE);
    
    // Simplified key expansion (real AES is more complex)
    for (int round = 0; round < 11; round++) {
        for (int i = 0; i < 16; i++) {
            ctx->round_keys[round][i] = key[i] ^ round;
        }
    }
}

// AES SubBytes transformation
void subBytes(unsigned char* state) {
    for (int i = 0; i < 16; i++) {
        state[i] = s_box[state[i]];
    }
}

// AES ShiftRows transformation
void shiftRows(unsigned char* state) {
    unsigned char temp;
    
    // Row 1: shift 1
    temp = state[1];
    state[1] = state[5];
    state[5] = state[9];
    state[9] = state[13];
    state[13] = temp;
    
    // Row 2: shift 2
    temp = state[2];
    state[2] = state[10];
    state[10] = temp;
    temp = state[6];
    state[6] = state[14];
    state[14] = temp;
    
    // Row 3: shift 3
    temp = state[3];
    state[3] = state[15];
    state[15] = state[11];
    state[11] = state[7];
    state[7] = temp;
}

// AES MixColumns transformation (simplified)
void mixColumns(unsigned char* state) {
    unsigned char temp[16];
    memcpy(temp, state, 16);
    
    for (int i = 0; i < 4; i++) {
        state[i*4] = temp[i*4] ^ temp[i*4+1];
        state[i*4+1] = temp[i*4+1] ^ temp[i*4+2];
        state[i*4+2] = temp[i*4+2] ^ temp[i*4+3];
        state[i*4+3] = temp[i*4+3] ^ temp[i*4];
    }
}

// AES encryption (simplified)
void aesEncrypt(AESContext* ctx, const unsigned char* plaintext, unsigned char* ciphertext) {
    unsigned char state[16];
    memcpy(state, plaintext, 16);
    
    // Initial round key addition
    for (int i = 0; i < 16; i++) {
        state[i] ^= ctx->round_keys[0][i];
    }
    
    // Main rounds
    for (int round = 1; round < 10; round++) {
        subBytes(state);
        shiftRows(state);
        mixColumns(state);
        
        // Add round key
        for (int i = 0; i < 16; i++) {
            state[i] ^= ctx->round_keys[round][i];
        }
    }
    
    // Final round
    subBytes(state);
    shiftRows(state);
    
    // Add final round key
    for (int i = 0; i < 16; i++) {
        state[i] ^= ctx->round_keys[10][i];
    }
    
    memcpy(ciphertext, state, 16);
}

// =============================================================================
// ASYMMETRIC ENCRYPTION (SIMPLIFIED RSA)
// =============================================================================

typedef struct {
    unsigned long long n;  // Modulus
    unsigned long long e;  // Public exponent
    unsigned long long d;  // Private exponent
} RSAKeyPair;

// Simple modular exponentiation
unsigned long long modExp(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long result = 1;
    base = base % mod;
    
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    
    return result;
}

// Generate RSA key pair (simplified)
void generateRSAKeyPair(RSAKeyPair* keypair) {
    // For demonstration, use small primes (in real RSA, use large primes)
    unsigned long long p = 61;  // Prime 1
    unsigned long long q = 53;  // Prime 2
    
    keypair->n = p * q;
    unsigned long long phi = (p - 1) * (q - 1);
    
    // Choose public exponent (commonly 65537, but using 17 for simplicity)
    keypair->e = 17;
    
    // Calculate private exponent (simplified)
    keypair->d = 2753; // Pre-calculated for this example
}

// RSA encryption
void rsaEncrypt(const unsigned char* plaintext, unsigned char* ciphertext, RSAKeyPair* keypair) {
    for (int i = 0; i < 8; i++) { // Process 8 bytes at a time
        unsigned long long block = plaintext[i];
        ciphertext[i] = (unsigned char)modExp(block, keypair->e, keypair->n);
    }
}

// RSA decryption
void rsaDecrypt(const unsigned char* ciphertext, unsigned char* plaintext, RSAKeyPair* keypair) {
    for (int i = 0; i < 8; i++) { // Process 8 bytes at a time
        unsigned long long block = ciphertext[i];
        plaintext[i] = (unsigned char)modExp(block, keypair->d, keypair->n);
    }
}

// =============================================================================
// DIGITAL SIGNATURES
// =============================================================================

// Create digital signature (simplified)
void createSignature(const char* message, unsigned char* signature, RSAKeyPair* keypair) {
    unsigned char hash[16];
    simpleMD5(message, hash);
    
    // Sign the hash with private key
    for (int i = 0; i < 16; i++) {
        signature[i] = (unsigned char)modExp(hash[i], keypair->d, keypair->n);
    }
}

// Verify digital signature
int verifySignature(const char* message, const unsigned char* signature, RSAKeyPair* keypair) {
    unsigned char hash[16];
    simpleMD5(message, hash);
    
    // Decrypt signature with public key
    for (int i = 0; i < 16; i++) {
        unsigned char decrypted = (unsigned char)modExp(signature[i], keypair->e, keypair->n);
        if (decrypted != hash[i]) {
            return 0; // Signature invalid
        }
    }
    
    return 1; // Signature valid
}

// =============================================================================
// PASSWORD SECURITY
// =============================================================================

// Simple password hashing (use proper hashing in production)
void hashPassword(const char* password, const char* salt, char* hash) {
    char combined[256];
    snprintf(combined, sizeof(combined), "%s%s", password, salt);
    
    unsigned char md5_hash[16];
    simpleMD5(combined, md5_hash);
    
    // Convert to hex string
    for (int i = 0; i < 16; i++) {
        sprintf(hash + (i * 2), "%02x", md5_hash[i]);
    }
    hash[32] = '\0';
}

// Generate random salt
void generateSalt(char* salt, int length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    
    srand(time(NULL));
    for (int i = 0; i < length; i++) {
        salt[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    salt[length] = '\0';
}

// Verify password
int verifyPassword(const char* password, const char* salt, const char* stored_hash) {
    char computed_hash[33];
    hashPassword(password, salt, computed_hash);
    
    return strcmp(computed_hash, stored_hash) == 0;
}

// =============================================================================
// SECURE RANDOM NUMBER GENERATION
// =============================================================================

// Linear congruential generator (not cryptographically secure)
typedef struct {
    unsigned int seed;
} LCG;

void initLCG(LCG* lcg, unsigned int seed) {
    lcg->seed = seed;
}

unsigned int randomLCG(LCG* lcg) {
    lcg->seed = (lcg->seed * 1103515245 + 12345) & 0x7fffffff;
    return lcg->seed;
}

// Generate random bytes
void generateRandomBytes(unsigned char* buffer, int length) {
    LCG lcg;
    initLCG(&lcg, time(NULL));
    
    for (int i = 0; i < length; i++) {
        buffer[i] = (unsigned char)randomLCG(&lcg);
    }
}

// =============================================================================
// SECURE COMMUNICATION PROTOCOLS
// =============================================================================

// Simple secure message format
typedef struct {
    unsigned char iv[16];        // Initialization vector
    unsigned char encrypted[64];  // Encrypted data
    unsigned char signature[16];  // Digital signature
} SecureMessage;

// Create secure message
void createSecureMessage(const char* message, SecureMessage* secure_msg, AESContext* aes_ctx, RSAKeyPair* rsa_ctx) {
    // Generate random IV
    generateRandomBytes(secure_msg->iv, 16);
    
    // Encrypt message with AES
    aesEncrypt(aes_ctx, (const unsigned char*)message, secure_msg->encrypted);
    
    // Create signature
    createSignature(message, secure_msg->signature, rsa_ctx);
}

// Verify and decrypt secure message
int processSecureMessage(SecureMessage* secure_msg, char* message, AESContext* aes_ctx, RSAKeyPair* rsa_ctx) {
    // Verify signature
    if (!verifySignature((const char*)secure_msg->encrypted, secure_msg->signature, rsa_ctx)) {
        return 0; // Signature verification failed
    }
    
    // Decrypt message
    unsigned char decrypted[64];
    aesEncrypt(aes_ctx, secure_msg->encrypted, decrypted); // Using same function for demo
    
    strcpy(message, (char*)decrypted);
    return 1; // Success
}

// =============================================================================
// SECURITY ANALYSIS TOOLS
// =============================================================================

// Frequency analysis for cryptanalysis
void frequencyAnalysis(const unsigned char* data, int length) {
    int freq[256] = {0};
    
    // Count frequencies
    for (int i = 0; i < length; i++) {
        freq[data[i]]++;
    }
    
    // Print frequencies
    printf("Frequency Analysis:\n");
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            printf("0x%02X: %d (%.2f%%)\n", i, freq[i], (freq[i] * 100.0) / length);
        }
    }
}

// Entropy calculation
double calculateEntropy(const unsigned char* data, int length) {
    int freq[256] = {0};
    double entropy = 0.0;
    
    // Count frequencies
    for (int i = 0; i < length; i++) {
        freq[data[i]]++;
    }
    
    // Calculate entropy
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            double p = (double)freq[i] / length;
            entropy -= p * log2(p);
        }
    }
    
    return entropy;
}

// =============================================================================
// DEMONSTRATION FUNCTIONS
// =============================================================================

void demonstrateBasicCiphers() {
    printf("=== BASIC CIPHERS DEMO ===\n");
    
    const char* message = "Hello, World!";
    char encrypted[100], decrypted[100];
    
    // Caesar cipher
    printf("Original: %s\n", message);
    caesarCipher(message, encrypted, 3);
    printf("Caesar (+3): %s\n", encrypted);
    caesarDecipher(encrypted, decrypted, 3);
    printf("Caesar (-3): %s\n", decrypted);
    
    // XOR cipher
    unsigned char key[] = "secretkey1234567";
    unsigned char xor_encrypted[100], xor_decrypted[100];
    int len = strlen(message);
    
    xorCipher((const unsigned char*)message, key, xor_encrypted, len);
    xor_encrypted[len] = '\0';
    printf("XOR Encrypted: ");
    for (int i = 0; i < len; i++) {
        printf("%02X", xor_encrypted[i]);
    }
    printf("\n");
    
    xorCipher(xor_encrypted, key, xor_decrypted, len);
    xor_decrypted[len] = '\0';
    printf("XOR Decrypted: %s\n", xor_decrypted);
    
    printf("\n");
}

void demonstrateHashFunctions() {
    printf("=== HASH FUNCTIONS DEMO ===\n");
    
    const char* message = "This is a test message";
    unsigned char hash[16];
    
    // Simple hash
    unsigned int simple_hash_result = simpleHash(message);
    printf("Simple Hash: %u\n", simple_hash_result);
    
    // MD5-like hash
    simpleMD5(message, hash);
    printf("MD5-like Hash: ");
    for (int i = 0; i < 16; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
    
    // Hash different message
    const char* message2 = "This is a different message";
    simpleMD5(message2, hash);
    printf("MD5 of different message: ");
    for (int i = 0; i < 16; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
    
    printf("\n");
}

void demonstrateSymmetricEncryption() {
    printf("=== SYMMETRIC ENCRYPTION DEMO ===\n");
    
    const char* message = "AES Test Message";
    unsigned char ciphertext[16];
    unsigned char decrypted[16];
    
    AESContext aes_ctx;
    unsigned char key[16] = "1234567890123456";
    initAES(&aes_ctx, key);
    
    printf("Original: %s\n", message);
    
    // Encrypt
    aesEncrypt(&aes_ctx, (const unsigned char*)message, ciphertext);
    printf("Encrypted: ");
    for (int i = 0; i < 16; i++) {
        printf("%02X", ciphertext[i]);
    }
    printf("\n");
    
    // Decrypt (using same function for demo)
    aesEncrypt(&aes_ctx, ciphertext, decrypted);
    decrypted[15] = '\0'; // Null terminate
    printf("Decrypted: %s\n", decrypted);
    
    printf("\n");
}

void demonstrateAsymmetricEncryption() {
    printf("=== ASYMMETRIC ENCRYPTION DEMO ===\n");
    
    const char* message = "RSA Test";
    unsigned char encrypted[8], decrypted[8];
    
    RSAKeyPair keypair;
    generateRSAKeyPair(&keypair);
    
    printf("Original: %s\n", message);
    printf("Public Key (n=%llu, e=%llu)\n", keypair.n, keypair.e);
    printf("Private Key (d=%llu)\n", keypair.d);
    
    // Encrypt
    rsaEncrypt((const unsigned char*)message, encrypted, &keypair);
    printf("Encrypted: ");
    for (int i = 0; i < 8; i++) {
        printf("%02X", encrypted[i]);
    }
    printf("\n");
    
    // Decrypt
    rsaDecrypt(encrypted, decrypted, &keypair);
    decrypted[7] = '\0'; // Null terminate
    printf("Decrypted: %s\n", decrypted);
    
    printf("\n");
}

void demonstrateDigitalSignatures() {
    printf("=== DIGITAL SIGNATURES DEMO ===\n");
    
    const char* message = "Important message";
    unsigned char signature[16];
    
    RSAKeyPair keypair;
    generateRSAKeyPair(&keypair);
    
    printf("Message: %s\n", message);
    
    // Create signature
    createSignature(message, signature, &keypair);
    printf("Signature: ");
    for (int i = 0; i < 16; i++) {
        printf("%02X", signature[i]);
    }
    printf("\n");
    
    // Verify signature
    int is_valid = verifySignature(message, signature, &keypair);
    printf("Signature verification: %s\n", is_valid ? "VALID" : "INVALID");
    
    // Test with modified message
    const char* modified_message = "Modified message";
    int is_modified_valid = verifySignature(modified_message, signature, &keypair);
    printf("Modified message verification: %s\n", is_modified_valid ? "VALID" : "INVALID");
    
    printf("\n");
}

void demonstratePasswordSecurity() {
    printf("=== PASSWORD SECURITY DEMO ===\n");
    
    const char* password = "MySecurePassword123";
    char salt[17];
    char hash[33];
    
    // Generate salt
    generateSalt(salt, 16);
    printf("Salt: %s\n", salt);
    
    // Hash password
    hashPassword(password, salt, hash);
    printf("Password Hash: %s\n", hash);
    
    // Verify password
    int is_valid = verifyPassword(password, salt, hash);
    printf("Password verification: %s\n", is_valid ? "VALID" : "INVALID");
    
    // Test with wrong password
    const char* wrong_password = "WrongPassword";
    int is_wrong_valid = verifyPassword(wrong_password, salt, hash);
    printf("Wrong password verification: %s\n", is_wrong_valid ? "VALID" : "INVALID");
    
    printf("\n");
}

void demonstrateSecureCommunication() {
    printf("=== SECURE COMMUNICATION DEMO ===\n");
    
    const char* message = "Secure message content";
    SecureMessage secure_msg;
    
    AESContext aes_ctx;
    unsigned char aes_key[16] = "AESkey1234567890";
    initAES(&aes_ctx, aes_key);
    
    RSAKeyPair rsa_ctx;
    generateRSAKeyPair(&rsa_ctx);
    
    printf("Original Message: %s\n", message);
    
    // Create secure message
    createSecureMessage(message, &secure_msg, &aes_ctx, &rsa_ctx);
    printf("Secure message created\n");
    
    // Process secure message
    char decrypted_message[65];
    int success = processSecureMessage(&secure_msg, decrypted_message, &aes_ctx, &rsa_ctx);
    
    if (success) {
        printf("Decrypted Message: %s\n", decrypted_message);
        printf("Communication successful\n");
    } else {
        printf("Communication failed - signature verification error\n");
    }
    
    printf("\n");
}

void demonstrateSecurityAnalysis() {
    printf("=== SECURITY ANALYSIS DEMO ===\n");
    
    // Generate test data
    unsigned char test_data[256];
    generateRandomBytes(test_data, 256);
    
    // Frequency analysis
    printf("Performing frequency analysis on random data...\n");
    frequencyAnalysis(test_data, 256);
    
    // Entropy calculation
    double entropy = calculateEntropy(test_data, 256);
    printf("\nEntropy of random data: %.4f bits\n", entropy);
    
    // Test with regular text
    const char* text = "The quick brown fox jumps over the lazy dog";
    double text_entropy = calculateEntropy((const unsigned char*)text, strlen(text));
    printf("Entropy of text: %.4f bits\n", text_entropy);
    
    printf("\n");
}

// =============================================================================
// MAIN FUNCTION
// =============================================================================

int main() {
    printf("Cryptography and Security Examples\n");
    printf("=================================\n\n");
    
    // Seed random number generator
    srand(time(NULL));
    
    demonstrateBasicCiphers();
    demonstrateHashFunctions();
    demonstrateSymmetricEncryption();
    demonstrateAsymmetricEncryption();
    demonstrateDigitalSignatures();
    demonstratePasswordSecurity();
    demonstrateSecureCommunication();
    demonstrateSecurityAnalysis();
    
    printf("All cryptography and security examples demonstrated!\n");
    printf("NOTE: These are simplified examples for educational purposes.\n");
    printf("Use established cryptographic libraries for production use.\n");
    
    return 0;
}
