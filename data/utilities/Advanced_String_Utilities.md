# Advanced String Utilities

This file contains advanced string manipulation utilities for C programming, including efficient string building, pattern matching, transformation, analysis, validation, and encoding functions.

## 📚 Utility Categories

### 🔧 String Building
Efficient string concatenation and formatting

### 🔍 Pattern Matching
Advanced searching and pattern recognition

### 🔄 String Transformation
Complex string modification operations

### 📊 String Analysis
Deep string analysis and extraction

### ✅ String Validation
Format validation for common data types

### 🔐 String Encoding
URL and Base64 encoding/decoding

## 🔧 String Building

### StringBuilder
Efficient dynamic string construction that minimizes memory allocations.

**Key Features**:
- Automatic memory management
- Efficient reallocation strategy
- Formatted string support
- O(n) performance for concatenations

```c
StringBuilder *sb = sbCreate(100);
sbAppend(sb, "Hello");
sbAppendFormat(sb, ", %s!", "World");
char *result = sbToString(sb);
sbFree(sb);
```

**Performance Benefits**:
- Avoids repeated malloc/free calls
- Reduces memory fragmentation
- Optimized for multiple concatenations

## 🔍 Pattern Matching

### Wildcard Matching
Supports `*` (any sequence) and `?` (any single character) patterns.

```c
int wildcardMatch(const char *pattern, const char *str);
```

**Examples**:
- `"H*o"` matches `"Hello"`
- `"Hell?"` matches `"Hello"`
- `"*test*"` matches `"this is a test"`

### Substring Operations
- **Count Substring**: Count occurrences of a pattern
- **Replace All**: Replace all occurrences with new string

```c
int countSubstring(const char *str, const char *substr);
char* replaceAll(const char *str, const char *oldSub, const char *newSub);
```

## 🔄 String Transformation

### Case Conversion
- **Title Case**: Capitalize first letter of each word
- **Case Insensitive**: Handle case-insensitive operations

```c
char* toTitleCase(const char *str);
// "hello world" → "Hello World"
```

### Word Operations
- **Reverse Words**: Reverse word order while preserving words
- **Remove Duplicates**: Remove duplicate characters

```c
char* reverseWords(const char *str);
char* removeDuplicates(const char *str);
```

## 📊 String Analysis

### Advanced Palindrome Detection
Ignores case, spaces, and non-alphanumeric characters.

```c
int isPalindromeAdvanced(const char *str);
// "A man, a plan, a canal: Panama" → true
```

### Longest Palindrome Substring
Finds the longest palindromic substring using expand-around-center algorithm.

```c
char* longestPalindromeSubstring(const char *str);
// "babad" → "bab" or "aba"
```

**Algorithm Complexity**:
- Time: O(n²)
- Space: O(1) (excluding result string)

## ✅ String Validation

### Email Validation
Comprehensive email format checking with RFC-compliant rules.

**Validation Criteria**:
- Single `@` symbol
- Valid local part characters
- Domain with at least one dot
- No leading/trailing dots

```c
int isValidEmail(const char *email);
```

### Phone Number Validation
Flexible phone number format validation.

**Supported Formats**:
- `(555) 123-4567`
- `555-123-4567`
- `+1 555 123 4567`
- `5551234567`

### URL Validation
HTTP/HTTPS URL format validation.

**Validation Criteria**:
- Valid protocol (http/https)
- Domain name with at least one dot
- Valid characters only

## 🔐 String Encoding

### URL Encoding/Decoding
Percent-encoding for safe URL transmission.

**Special Characters**:
- Space → `+` or `%20`
- `!@#$%^&*()` → `%XX` format

```c
char* urlEncode(const char *str);
char* urlDecode(const char *str);
```

### Base64 Encoding
Binary-safe text encoding for data transmission.

**Features**:
- Standard Base64 alphabet
- Proper padding with `=`
- Memory-safe implementation

```c
char* base64Encode(const char *data, size_t length);
```

## 💡 Implementation Details

### Memory Management
All functions follow consistent memory management patterns:
- **Input**: Const pointers (don't modify input)
- **Output**: Newly allocated memory (caller must free)
- **Error**: Return NULL on failure

### Error Handling
```c
char *result = someFunction(input);
if (result == NULL) {
    // Handle error
}
// Use result
free(result);
```

### Performance Optimizations
1. **StringBuilder**: Exponential capacity growth
2. **Pattern Matching**: Early termination on mismatches
3. **Validation**: Single-pass algorithms
4. **Encoding**: Minimal allocations

## 🚀 Advanced Techniques

### 1. StringBuilder Internals
```c
typedef struct {
    char *data;        // Dynamic buffer
    size_t length;     // Current string length
    size_t capacity;   // Buffer capacity
} StringBuilder;
```

**Growth Strategy**:
- Double capacity when needed
- Minimum allocation for requested size
- O(1) amortized append cost

### 2. Wildcard Matching Algorithm
```c
int wildcardMatch(const char *pattern, const char *str) {
    const char *star = NULL;
    const char *ss = str;
    
    while (*s) {
        if (*p == '*') {
            star = p;
            ss = s;
            p++;
        } else if (*p == '?' || *p == *s) {
            p++;
            s++;
        } else if (star) {
            p = star + 1;
            ss++;
            s = ss;
        } else {
            return 0;
        }
    }
    
    while (*p == '*') p++;
    return !*p;
}
```

### 3. Palindrome Detection
```c
int isPalindromeAdvanced(const char *str) {
    int left = 0, right = strlen(str) - 1;
    
    while (left < right) {
        // Skip non-alphanumeric
        while (left < right && !isalnum(str[left])) left++;
        while (left < right && !isalnum(str[right])) right--;
        
        if (tolower(str[left]) != tolower(str[right])) {
            return 0;
        }
        left++;
        right--;
    }
    return 1;
}
```

## 📊 Performance Analysis

| Operation | Time Complexity | Space Complexity | Notes |
|-----------|-----------------|------------------|-------|
| StringBuilder Append | O(1) amortized | O(n) | Efficient concatenation |
| Wildcard Match | O(n×m) | O(1) | n=str len, m=pattern len |
| Replace All | O(n×m) | O(n) | Creates new string |
| Title Case | O(n) | O(n) | Case conversion |
| Palindrome Check | O(n) | O(1) | Advanced version |
| Email Validation | O(n) | O(1) | Single pass |
| URL Encoding | O(n) | O(n) | Percent encoding |

## 🧪 Testing Strategies

### 1. Unit Testing
```c
void testStringBuilder() {
    StringBuilder *sb = sbCreate(10);
    assert(sb != NULL);
    assert(sbAppend(sb, "Hello"));
    assert(sbAppendFormat(sb, " %d", 42));
    assert(strcmp(sbToString(sb), "Hello 42") == 0);
    sbFree(sb);
}
```

### 2. Edge Case Testing
```c
void testEdgeCases() {
    // Empty strings
    assert(isValidEmail("") == 0);
    assert(wildcardMatch("", "") == 1);
    
    // NULL pointers
    assert(isValidEmail(NULL) == 0);
    assert(wildcardMatch(NULL, "test") == 0);
    
    // Large inputs
    char large[10000];
    memset(large, 'a', 9999);
    large[9999] = '\0';
    // Test with large string
}
```

### 3. Performance Testing
```c
void benchmarkStringBuilder() {
    clock_t start = clock();
    
    StringBuilder *sb = sbCreate(100);
    for (int i = 0; i < 10000; i++) {
        sbAppendFormat(sb, "Item %d ", i);
    }
    
    clock_t end = clock();
    double time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("StringBuilder: %f seconds\n", time);
    
    sbFree(sb);
}
```

## ⚠️ Common Pitfalls

### 1. Memory Leaks
```c
// Wrong
char *result = someFunction(input);
// Forget to free(result)

// Right
char *result = someFunction(input);
if (result) {
    // Use result
    free(result);
}
```

### 2. NULL Pointer Dereference
```c
// Wrong
if (strlen(input) > 0) { // input might be NULL
    // Process
}

// Right
if (input && strlen(input) > 0) {
    // Process
}
```

### 3. Buffer Overflows
```c
// Wrong
char buffer[10];
strcpy(buffer, largeString); // Potential overflow

// Right
StringBuilder *sb = sbCreate(100);
sbAppend(sb, largeString); // Safe
```

### 4. Encoding Issues
```c
// Wrong
char *encoded = urlEncode(utf8String);
// Might not handle multi-byte correctly

// Right
// Ensure proper UTF-8 handling before encoding
```

## 🔧 Real-World Applications

### 1. Web Development
- URL parameter encoding/decoding
- Form validation
- HTML sanitization
- JSON string manipulation

### 2. Data Processing
- CSV parsing and generation
- Log file analysis
- Text file processing
- Data format conversion

### 3. Security
- Input sanitization
- SQL injection prevention
- XSS protection
- Data validation

### 4. User Interfaces
- Search functionality
- Auto-completion
- Text formatting
- Input validation

## 🎓 Best Practices

### 1. Memory Management
```c
// Always check return values
char *result = function(input);
if (!result) {
    // Handle error
    return;
}

// Always free allocated memory
free(result);
```

### 2. Input Validation
```c
// Validate inputs before processing
if (!input || !*input) {
    return NULL;
}
```

### 3. Error Handling
```c
// Consistent error handling
if (error_condition) {
    // Clean up
    if (allocated_memory) free(allocated_memory);
    return NULL;
}
```

### 4. Performance Considerations
```c
// Use StringBuilder for multiple concatenations
StringBuilder *sb = sbCreate(initial_size);
// Multiple appends
char *result = sbToString(sb);
sbFree(sb);
```

## 🔄 Integration Examples

### 1. Web Form Processing
```c
void processFormData(const char *name, const char *email, const char *phone) {
    if (!isValidEmail(email)) {
        printf("Invalid email format\n");
        return;
    }
    
    if (!isValidPhoneNumber(phone)) {
        printf("Invalid phone format\n");
        return;
    }
    
    char *safeName = urlEncode(name);
    // Process safe data...
    free(safeName);
}
```

### 2. Text Analysis
```c
void analyzeText(const char *text) {
    printf("Text analysis:\n");
    printf("Is palindrome: %s\n", 
           isPalindromeAdvanced(text) ? "Yes" : "No");
    
    char *longestPal = longestPalindromeSubstring(text);
    printf("Longest palindrome: %s\n", longestPal);
    free(longestPal);
    
    char *titleCase = toTitleCase(text);
    printf("Title case: %s\n", titleCase);
    free(titleCase);
}
```

### 3. Data Validation Pipeline
```c
int validateUserData(const char *email, const char *url, const char *phone) {
    return isValidEmail(email) && 
           isValidURL(url) && 
           isValidPhoneNumber(phone);
}
```

These advanced string utilities provide a comprehensive toolkit for sophisticated string processing in C, enabling robust text manipulation, validation, and transformation capabilities for real-world applications.
