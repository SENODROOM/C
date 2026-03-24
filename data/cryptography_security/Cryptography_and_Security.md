# Cryptography and Security

This file contains comprehensive cryptography and security examples in C, including basic ciphers, hash functions, symmetric and asymmetric encryption, digital signatures, password security, and secure communication protocols.

## 📚 Cryptography Overview

### 🔐 Cryptographic Concepts
- **Encryption**: Transforming data to keep it secret
- **Decryption**: Reversing encryption to recover original data
- **Hashing**: One-way transformation for data integrity
- **Digital Signatures**: Authentication and non-repudiation
- **Key Management**: Secure generation and storage of keys

### 🛡️ Security Principles
- **Confidentiality**: Keeping information secret
- **Integrity**: Ensuring data hasn't been modified
- **Authentication**: Verifying identity
- **Non-repudiation**: Preventing denial of actions

## 🔤 Basic Ciphers

### Caesar Cipher
```c
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

void caesarDecipher(const char* input, char* output, int shift) {
    caesarCipher(input, output, 26 - shift);
}
```

**Characteristics**:
- **Type**: Substitution cipher
- **Security**: Very weak (easily broken)
- **Use Case**: Educational purposes only

### XOR Cipher
```c
void xorCipher(const unsigned char* input, const unsigned char* key, 
               unsigned char* output, int length) {
    for (int i = 0; i < length; i++) {
        output[i] = input[i] ^ key[i % KEY_SIZE];
    }
}
```

**Characteristics**:
- **Type**: Symmetric cipher
- **Security**: Weak without proper key management
- **Use Case**: Simple obfuscation

## 🔍 Hash Functions

### Simple Hash Function
```c
unsigned int simpleHash(const char* str) {
    unsigned int hash = 5381;
    int c;
    
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    
    return hash;
}
```

### MD5-like Hash Function
```c
void simpleMD5(const char* input, unsigned char* hash) {
    // Simplified MD5 implementation
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
```

**Hash Function Properties**:
- **Deterministic**: Same input always produces same output
- **Fixed Output**: Consistent output size regardless of input
- **One-Way**: Difficult to reverse (for cryptographic hashes)
- **Collision Resistant**: Hard to find two inputs with same hash

## 🔐 Symmetric Encryption

### AES-like Encryption
```c
typedef struct {
    unsigned char key[KEY_SIZE];
    unsigned char round_keys[11][16];
} AESContext;

void initAES(AESContext* ctx, const unsigned char* key) {
    memcpy(ctx->key, key, KEY_SIZE);
    
    // Simplified key expansion
    for (int round = 0; round < 11; round++) {
        for (int i = 0; i < 16; i++) {
            ctx->round_keys[round][i] = key[i] ^ round;
        }
    }
}

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
```

### AES Transformations

#### SubBytes
```c
void subBytes(unsigned char* state) {
    for (int i = 0; i < 16; i++) {
        state[i] = s_box[state[i]];
    }
}
```

#### ShiftRows
```c
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
```

#### MixColumns
```c
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
```

**Symmetric Encryption Characteristics**:
- **Key Requirements**: Single key for encryption and decryption
- **Speed**: Fast for large amounts of data
- **Security**: Depends on key secrecy and algorithm strength
- **Use Cases**: Bulk data encryption, file encryption

## 🔑 Asymmetric Encryption

### RSA Encryption
```c
typedef struct {
    unsigned long long n;  // Modulus
    unsigned long long e;  // Public exponent
    unsigned long long d;  // Private exponent
} RSAKeyPair;

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

void rsaEncrypt(const unsigned char* plaintext, unsigned char* ciphertext, RSAKeyPair* keypair) {
    for (int i = 0; i < 8; i++) {
        unsigned long long block = plaintext[i];
        ciphertext[i] = (unsigned char)modExp(block, keypair->e, keypair->n);
    }
}

void rsaDecrypt(const unsigned char* ciphertext, unsigned char* plaintext, RSAKeyPair* keypair) {
    for (int i = 0; i < 8; i++) {
        unsigned long long block = ciphertext[i];
        plaintext[i] = (unsigned char)modExp(block, keypair->d, keypair->n);
    }
}
```

### RSA Key Generation
```c
void generateRSAKeyPair(RSAKeyPair* keypair) {
    // For demonstration, use small primes
    unsigned long long p = 61;  // Prime 1
    unsigned long long q = 53;  // Prime 2
    
    keypair->n = p * q;
    unsigned long long phi = (p - 1) * (q - 1);
    
    // Choose public exponent
    keypair->e = 17;
    
    // Calculate private exponent
    keypair->d = 2753; // Pre-calculated for this example
}
```

**Asymmetric Encryption Characteristics**:
- **Key Requirements**: Public and private key pair
- **Speed**: Slower than symmetric encryption
- **Security**: Based on mathematical hardness problems
- **Use Cases**: Key exchange, digital signatures, small data encryption

## ✍️ Digital Signatures

### Signature Creation
```c
void createSignature(const char* message, unsigned char* signature, RSAKeyPair* keypair) {
    unsigned char hash[16];
    simpleMD5(message, hash);
    
    // Sign the hash with private key
    for (int i = 0; i < 16; i++) {
        signature[i] = (unsigned char)modExp(hash[i], keypair->d, keypair->n);
    }
}
```

### Signature Verification
```c
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
```

**Digital Signature Properties**:
- **Authentication**: Proves sender identity
- **Integrity**: Ensures message hasn't been modified
- **Non-repudiation**: Sender cannot deny sending message
- **Use Cases**: Software distribution, financial transactions

## 🔐 Password Security

### Password Hashing
```c
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
```

### Salt Generation
```c
void generateSalt(char* salt, int length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    
    srand(time(NULL));
    for (int i = 0; i < length; i++) {
        salt[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    salt[length] = '\0';
}
```

### Password Verification
```c
int verifyPassword(const char* password, const char* salt, const char* stored_hash) {
    char computed_hash[33];
    hashPassword(password, salt, computed_hash);
    
    return strcmp(computed_hash, stored_hash) == 0;
}
```

**Password Security Best Practices**:
- **Never store plaintext passwords**
- **Use cryptographic salt**
- **Use strong hashing algorithms (bcrypt, scrypt, Argon2)**
- **Implement rate limiting**
- **Use multi-factor authentication when possible**

## 🎲 Secure Random Number Generation

### Linear Congruential Generator
```c
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
```

### Random Byte Generation
```c
void generateRandomBytes(unsigned char* buffer, int length) {
    LCG lcg;
    initLCG(&lcg, time(NULL));
    
    for (int i = 0; i < length; i++) {
        buffer[i] = (unsigned char)randomLCG(&lcg);
    }
}
```

**Random Number Security**:
- **Avoid predictable seeds**
- **Use cryptographically secure generators for security applications**
- **Test randomness quality**
- **Consider hardware random number generators**

## 🛡️ Secure Communication

### Secure Message Format
```c
typedef struct {
    unsigned char iv[16];        // Initialization vector
    unsigned char encrypted[64];  // Encrypted data
    unsigned char signature[16];  // Digital signature
} SecureMessage;
```

### Secure Message Creation
```c
void createSecureMessage(const char* message, SecureMessage* secure_msg, 
                         AESContext* aes_ctx, RSAKeyPair* rsa_ctx) {
    // Generate random IV
    generateRandomBytes(secure_msg->iv, 16);
    
    // Encrypt message with AES
    aesEncrypt(aes_ctx, (const unsigned char*)message, secure_msg->encrypted);
    
    // Create signature
    createSignature(message, secure_msg->signature, rsa_ctx);
}
```

### Secure Message Processing
```c
int processSecureMessage(SecureMessage* secure_msg, char* message, 
                         AESContext* aes_ctx, RSAKeyPair* rsa_ctx) {
    // Verify signature
    if (!verifySignature((const char*)secure_msg->encrypted, 
                        secure_msg->signature, rsa_ctx)) {
        return 0; // Signature verification failed
    }
    
    // Decrypt message
    unsigned char decrypted[64];
    aesEncrypt(aes_ctx, secure_msg->encrypted, decrypted);
    
    strcpy(message, (char*)decrypted);
    return 1; // Success
}
```

**Secure Communication Protocol**:
- **Encryption**: Protect message confidentiality
- **Authentication**: Verify sender identity
- **Integrity**: Ensure message hasn't been modified
- **Non-repudiation**: Prevent sender denial

## 🔍 Security Analysis Tools

### Frequency Analysis
```c
void frequencyAnalysis(const unsigned char* data, int length) {
    int freq[256] = {0};
    
    // Count frequencies
    for (int i = 0; i < length; i++) {
        freq[data[i]]++;
    }
    
    // Print frequencies
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            printf("0x%02X: %d (%.2f%%)\n", i, freq[i], (freq[i] * 100.0) / length);
        }
    }
}
```

### Entropy Calculation
```c
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
```

**Cryptanalysis Techniques**:
- **Frequency Analysis**: Analyze character/pattern frequencies
- **Known Plaintext Attacks**: Use known plaintext-ciphertext pairs
- **Differential Cryptanalysis**: Analyze output differences
- **Linear Cryptanalysis**: Find linear approximations

## 💡 Advanced Topics

### 1. Key Derivation Functions
```c
void deriveKey(const char* password, const char* salt, unsigned char* key, int key_length) {
    // Simplified PBKDF2
    unsigned char hash[16];
    char combined[256];
    
    for (int i = 0; i < key_length; i++) {
        snprintf(combined, sizeof(combined), "%s%s%d", password, salt, i);
        simpleMD5(combined, hash);
        key[i] = hash[0];
    }
}
```

### 2. Message Authentication Codes
```c
void calculateHMAC(const char* message, const unsigned char* key, unsigned char* mac) {
    // Simplified HMAC
    unsigned char inner_hash[16];
    unsigned char outer_hash[16];
    
    // Inner hash
    char inner_data[MAX_DATA_SIZE];
    for (int i = 0; i < 16; i++) {
        inner_data[i] = key[i] ^ 0x36;
    }
    strcat(inner_data, message);
    simpleMD5(inner_data, inner_hash);
    
    // Outer hash
    char outer_data[MAX_DATA_SIZE];
    for (int i = 0; i < 16; i++) {
        outer_data[i] = key[i] ^ 0x5C;
    }
    strcat(outer_data, (char*)inner_hash);
    simpleMD5(outer_data, outer_hash);
    
    memcpy(mac, outer_hash, 16);
}
```

### 3. Perfect Forward Secrecy
```c
typedef struct {
    unsigned char private_key[32];
    unsigned char public_key[32];
} DHKeyPair;

void generateDHKeyPair(DHKeyPair* keypair) {
    // Simplified Diffie-Hellman
    generateRandomBytes(keypair->private_key, 32);
    
    // g^private mod p (simplified)
    for (int i = 0; i < 32; i++) {
        keypair->public_key[i] = keypair->private_key[i] ^ 0xFF;
    }
}
```

### 4. Zero-Knowledge Proofs
```c
int zeroKnowledgeProof(const unsigned char* secret, const unsigned char* challenge) {
    // Simplified zero-knowledge proof
    unsigned char response[16];
    
    // Create response based on secret and challenge
    for (int i = 0; i < 16; i++) {
        response[i] = secret[i] ^ challenge[i];
    }
    
    // Verify response (simplified)
    for (int i = 0; i < 16; i++) {
        if (response[i] != (secret[i] ^ challenge[i])) {
            return 0;
        }
    }
    
    return 1;
}
```

## 📊 Performance Considerations

### 1. Algorithm Selection
```c
// Choose algorithm based on requirements
void selectAlgorithm(int data_size, int security_level) {
    if (data_size > 1000000) {
        // Use symmetric encryption for large data
        printf("Use AES for large data\n");
    } else if (security_level > 8) {
        // Use asymmetric for high security
        printf("Use RSA for high security\n");
    } else {
        // Use hybrid approach
        printf("Use hybrid encryption\n");
    }
}
```

### 2. Key Management
```c
typedef struct {
    unsigned char key[32];
    time_t created;
    time_t expires;
    int usage_count;
} SecureKey;

void rotateKey(SecureKey* key) {
    // Generate new key
    generateRandomBytes(key->key, 32);
    key->created = time(NULL);
    key->expires = key->created + 86400; // 24 hours
    key->usage_count = 0;
}
```

### 3. Memory Security
```c
void secureMemory(unsigned char* buffer, int length) {
    // Clear sensitive data from memory
    for (int i = 0; i < length; i++) {
        buffer[i] = 0;
    }
    
    // Prevent compiler optimizations
    volatile unsigned char* ptr = buffer;
    for (int i = 0; i < length; i++) {
        ptr[i] = 0;
    }
}
```

## ⚠️ Common Pitfalls

### 1. Using Weak Algorithms
```c
// Wrong - Using weak encryption
void weakEncryption(const char* data) {
    // Simple XOR with fixed key
    for (int i = 0; data[i]; i++) {
        encrypted[i] = data[i] ^ 0x55;
    }
}

// Right - Using strong encryption
void strongEncryption(const char* data) {
    // Use AES with proper key
    aesEncrypt(&aes_ctx, (const unsigned char*)data, encrypted);
}
```

### 2. Key Management Issues
```c
// Wrong - Hardcoded keys
unsigned char hardcoded_key[] = "secret123456789";

// Right - Secure key generation
unsigned char secure_key[16];
generateRandomBytes(secure_key, 16);
```

### 3. Insufficient Randomness
```c
// Wrong - Predictable random numbers
srand(12345);
int random_number = rand();

// Right - Cryptographically secure random
unsigned char random_bytes[4];
generateRandomBytes(random_bytes, 4);
```

### 4. Side Channel Attacks
```c
// Wrong - Timing leaks information
int comparePasswords(const char* a, const char* b) {
    for (int i = 0; i < 32; i++) {
        if (a[i] != b[i]) return 0; // Early return
    }
    return 1;
}

// Right - Constant-time comparison
int constantTimeCompare(const char* a, const char* b) {
    int result = 0;
    for (int i = 0; i < 32; i++) {
        result |= a[i] ^ b[i];
    }
    return result == 0;
}
```

## 🔧 Real-World Applications

### 1. Secure File Storage
```c
void encryptFile(const char* filename, const unsigned char* key) {
    FILE* file = fopen(filename, "rb");
    if (!file) return;
    
    // Read file
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    unsigned char* data = malloc(file_size);
    fread(data, 1, file_size, file);
    fclose(file);
    
    // Encrypt data
    AESContext aes_ctx;
    initAES(&aes_ctx, key);
    
    for (long i = 0; i < file_size; i += 16) {
        aesEncrypt(&aes_ctx, data + i, data + i);
    }
    
    // Write encrypted file
    FILE* encrypted_file = fopen("encrypted_file.bin", "wb");
    fwrite(data, 1, file_size, encrypted_file);
    fclose(encrypted_file);
    
    free(data);
    secureMemory(key, 16);
}
```

### 2. Secure Network Communication
```c
void secureSend(int socket, const char* message, const unsigned char* key) {
    // Create secure message
    SecureMessage secure_msg;
    AESContext aes_ctx;
    RSAKeyPair rsa_ctx;
    
    initAES(&aes_ctx, key);
    generateRSAKeyPair(&rsa_ctx);
    
    createSecureMessage(message, &secure_msg, &aes_ctx, &rsa_ctx);
    
    // Send secure message
    send(socket, (char*)&secure_msg, sizeof(SecureMessage), 0);
}
```

### 3. User Authentication
```c
int authenticateUser(const char* username, const char* password) {
    // Get stored hash from database
    char stored_hash[33];
    char salt[17];
    getUserCredentials(username, stored_hash, salt);
    
    // Verify password
    return verifyPassword(password, salt, stored_hash);
}
```

### 4. Digital Document Signing
```c
void signDocument(const char* document, const char* private_key_file) {
    RSAKeyPair keypair;
    loadPrivateKey(private_key_file, &keypair);
    
    unsigned char signature[16];
    createSignature(document, signature, &keypair);
    
    // Save signature
    FILE* sig_file = fopen("document.sig", "wb");
    fwrite(signature, 1, 16, sig_file);
    fclose(sig_file);
}
```

## 🎓 Best Practices

### 1. Use Established Libraries
```c
// Instead of implementing your own crypto, use libraries like:
// - OpenSSL
// - Libsodium
// - Crypto++
// - Windows CryptoAPI
// - Apple Common Crypto
```

### 2. Proper Key Management
```c
// Use secure key storage
void secureKeyStorage(const unsigned char* key) {
    // Store in hardware security module
    // Use key vault services
    // Implement proper access controls
}
```

### 3. Regular Security Audits
```c
void securityAudit() {
    // Check for known vulnerabilities
    // Review cryptographic implementations
    // Test for side channel attacks
    // Verify proper key usage
}
```

### 4. Stay Updated
```c
// Keep cryptographic libraries updated
// Monitor security advisories
// Implement new algorithms when needed
// Phase out deprecated algorithms
```

### 5. Defense in Depth
```c
void layeredSecurity() {
    // Multiple layers of security
    // Different algorithms for different purposes
    // Redundant security measures
    // Regular security testing
}
```

## ⚠️ Security Warnings

### Educational Use Only
The cryptographic implementations in this file are **simplified for educational purposes** and **should not be used in production systems**. For real-world applications, always use:

- **Well-vetted cryptographic libraries**
- **Standardized algorithms** (AES, RSA, SHA-256, etc.)
- **Proper key management systems**
- **Security audits and reviews**

### Production Recommendations
- Use OpenSSL, Libsodium, or similar libraries
- Implement proper random number generation
- Use hardware security modules when possible
- Follow industry standards and best practices
- Regularly update cryptographic components

Cryptography is a complex field where small mistakes can have severe security consequences. Always prefer established, peer-reviewed implementations for production use!
