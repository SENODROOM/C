#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <time.h>

// =============================================================================
// ADVANCED STRING UTILITIES
// =============================================================================

// String builder for efficient concatenation
typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} StringBuilder;

// Initialize string builder
StringBuilder* sbCreate(size_t initialCapacity) {
    StringBuilder *sb = (StringBuilder*)malloc(sizeof(StringBuilder));
    if (!sb) return NULL;
    
    sb->data = (char*)malloc(initialCapacity);
    if (!sb->data) {
        free(sb);
        return NULL;
    }
    
    sb->data[0] = '\0';
    sb->length = 0;
    sb->capacity = initialCapacity;
    return sb;
}

// Append to string builder
int sbAppend(StringBuilder *sb, const char *str) {
    if (!sb || !str) return 0;
    
    size_t strLen = strlen(str);
    size_t needed = sb->length + strLen + 1;
    
    if (needed > sb->capacity) {
        size_t newCapacity = sb->capacity * 2;
        if (newCapacity < needed) newCapacity = needed;
        
        char *newData = (char*)realloc(sb->data, newCapacity);
        if (!newData) return 0;
        
        sb->data = newData;
        sb->capacity = newCapacity;
    }
    
    strcpy(sb->data + sb->length, str);
    sb->length += strLen;
    return 1;
}

// Append formatted string
int sbAppendFormat(StringBuilder *sb, const char *format, ...) {
    if (!sb || !format) return 0;
    
    va_list args;
    va_start(args, format);
    
    // Calculate required size
    va_list argsCopy;
    va_copy(argsCopy, args);
    int size = vsnprintf(NULL, 0, format, argsCopy);
    va_end(argsCopy);
    
    if (size < 0) {
        va_end(args);
        return 0;
    }
    
    size_t needed = sb->length + size + 1;
    if (needed > sb->capacity) {
        size_t newCapacity = sb->capacity * 2;
        if (newCapacity < needed) newCapacity = needed;
        
        char *newData = (char*)realloc(sb->data, newCapacity);
        if (!newData) {
            va_end(args);
            return 0;
        }
        
        sb->data = newData;
        sb->capacity = newCapacity;
    }
    
    vsprintf(sb->data + sb->length, format, args);
    sb->length += size;
    va_end(args);
    return 1;
}

// Get string from builder
char* sbToString(StringBuilder *sb) {
    if (!sb) return NULL;
    return sb->data;
}

// Free string builder
void sbFree(StringBuilder *sb) {
    if (sb) {
        free(sb->data);
        free(sb);
    }
}

// =============================================================================
// PATTERN MATCHING AND SEARCHING
// =============================================================================

// Simple wildcard matching (* and ?)
int wildcardMatch(const char *pattern, const char *str) {
    if (!pattern || !str) return 0;
    
    const char *p = pattern;
    const char *s = str;
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

// Count occurrences of substring
int countSubstring(const char *str, const char *substr) {
    if (!str || !substr || !*substr) return 0;
    
    int count = 0;
    const char *p = str;
    size_t substrLen = strlen(substr);
    
    while ((p = strstr(p, substr)) != NULL) {
        count++;
        p += substrLen;
    }
    
    return count;
}

// Replace all occurrences of substring
char* replaceAll(const char *str, const char *oldSub, const char *newSub) {
    if (!str || !oldSub || !newSub) return NULL;
    
    size_t oldLen = strlen(oldSub);
    size_t newLen = strlen(newSub);
    
    // Count occurrences
    int occurrences = countSubstring(str, oldSub);
    if (occurrences == 0) {
        return strdup(str);
    }
    
    // Calculate new string size
    size_t strLen = strlen(str);
    size_t resultSize = strLen + occurrences * (newLen - oldLen) + 1;
    
    char *result = (char*)malloc(resultSize);
    if (!result) return NULL;
    
    const char *src = str;
    char *dst = result;
    
    while (*src) {
        if (strncmp(src, oldSub, oldLen) == 0) {
            strcpy(dst, newSub);
            dst += newLen;
            src += oldLen;
        } else {
            *dst++ = *src++;
        }
    }
    
    *dst = '\0';
    return result;
}

// =============================================================================
// STRING TRANSFORMATION
// =============================================================================

// Convert string to title case
char* toTitleCase(const char *str) {
    if (!str) return NULL;
    
    size_t len = strlen(str);
    char *result = (char*)malloc(len + 1);
    if (!result) return NULL;
    
    int capitalizeNext = 1;
    
    for (size_t i = 0; i < len; i++) {
        if (isspace((unsigned char)str[i])) {
            result[i] = str[i];
            capitalizeNext = 1;
        } else if (capitalizeNext) {
            result[i] = toupper((unsigned char)str[i]);
            capitalizeNext = 0;
        } else {
            result[i] = tolower((unsigned char)str[i]);
        }
    }
    
    result[len] = '\0';
    return result;
}

// Reverse words in string
char* reverseWords(const char *str) {
    if (!str) return NULL;
    
    StringBuilder *sb = sbCreate(strlen(str) + 1);
    if (!sb) return NULL;
    
    // Split into words and reverse order
    char *temp = strdup(str);
    char *token = strtok(temp, " \t\n");
    char *words[100];
    int wordCount = 0;
    
    while (token && wordCount < 100) {
        words[wordCount++] = token;
        token = strtok(NULL, " \t\n");
    }
    
    // Build reversed string
    for (int i = wordCount - 1; i >= 0; i--) {
        sbAppend(sb, words[i]);
        if (i > 0) sbAppend(sb, " ");
    }
    
    char *result = strdup(sbToString(sb));
    free(temp);
    sbFree(sb);
    return result;
}

// Remove duplicate characters
char* removeDuplicates(const char *str) {
    if (!str) return NULL;
    
    int seen[256] = {0};
    StringBuilder *sb = sbCreate(strlen(str) + 1);
    if (!sb) return NULL;
    
    for (size_t i = 0; str[i]; i++) {
        unsigned char c = (unsigned char)str[i];
        if (!seen[c]) {
            seen[c] = 1;
            char ch[2] = {c, '\0'};
            sbAppend(sb, ch);
        }
    }
    
    char *result = strdup(sbToString(sb));
    sbFree(sb);
    return result;
}

// =============================================================================
// STRING ANALYSIS
// =============================================================================

// Check if string is palindrome (ignoring case and non-alphanumeric)
int isPalindromeAdvanced(const char *str) {
    if (!str) return 0;
    
    int left = 0;
    int right = strlen(str) - 1;
    
    while (left < right) {
        // Skip non-alphanumeric
        while (left < right && !isalnum((unsigned char)str[left])) left++;
        while (left < right && !isalnum((unsigned char)str[right])) right--;
        
        if (left < right) {
            if (tolower((unsigned char)str[left]) != 
                tolower((unsigned char)str[right])) {
                return 0;
            }
            left++;
            right--;
        }
    }
    
    return 1;
}

// Find longest palindrome substring
char* longestPalindromeSubstring(const char *str) {
    if (!str || !*str) return strdup("");
    
    size_t len = strlen(str);
    int start = 0, maxLen = 1;
    
    // Expand around center
    for (int i = 0; i < len; i++) {
        // Odd length
        int l = i, r = i;
        while (l >= 0 && r < len && str[l] == str[r]) {
            if (r - l + 1 > maxLen) {
                start = l;
                maxLen = r - l + 1;
            }
            l--;
            r++;
        }
        
        // Even length
        l = i;
        r = i + 1;
        while (l >= 0 && r < len && str[l] == str[r]) {
            if (r - l + 1 > maxLen) {
                start = l;
                maxLen = r - l + 1;
            }
            l--;
            r++;
        }
    }
    
    char *result = (char*)malloc(maxLen + 1);
    strncpy(result, str + start, maxLen);
    result[maxLen] = '\0';
    return result;
}

// =============================================================================
// STRING VALIDATION
// =============================================================================

// Validate email format
int isValidEmail(const char *email) {
    if (!email || !*email) return 0;
    
    const char *at = strchr(email, '@');
    if (!at || at == email || strchr(at + 1, '@')) return 0;
    
    // Check local part
    const char *p = email;
    while (p < at) {
        if (!isalnum((unsigned char)*p) && *p != '.' && *p != '_' && *p != '-' && *p != '+') {
            return 0;
        }
        p++;
    }
    
    // Check domain part
    p = at + 1;
    int dotCount = 0;
    while (*p) {
        if (*p == '.') {
            dotCount++;
            if (p == at + 1 || !*(p + 1)) return 0; // Leading or trailing dot
        } else if (!isalnum((unsigned char)*p) && *p != '-' && *p != '.') {
            return 0;
        }
        p++;
    }
    
    return dotCount >= 1;
}

// Validate phone number format
int isValidPhoneNumber(const char *phone) {
    if (!phone || !*phone) return 0;
    
    int digitCount = 0;
    const char *p = phone;
    
    while (*p) {
        if (isdigit((unsigned char)*p)) {
            digitCount++;
        } else if (*p != '(' && *p != ')' && *p != '-' && *p != '+' && *p != ' ') {
            return 0;
        }
        p++;
    }
    
    return digitCount >= 10; // At least 10 digits
}

// Validate URL format
int isValidURL(const char *url) {
    if (!url || !*url) return 0;
    
    // Check protocol
    if (strncmp(url, "http://", 7) == 0) {
        url += 7;
    } else if (strncmp(url, "https://", 8) == 0) {
        url += 8;
    } else {
        return 0;
    }
    
    // Check domain
    if (!*url) return 0;
    
    int dotCount = 0;
    const char *p = url;
    
    while (*p && *p != '/' && *p != '?' && *p != '#') {
        if (*p == '.') {
            dotCount++;
        } else if (!isalnum((unsigned char)*p) && *p != '-' && *p != '.') {
            return 0;
        }
        p++;
    }
    
    return dotCount >= 1;
}

// =============================================================================
// STRING ENCODING/DECODING
// =============================================================================

// URL encode string
char* urlEncode(const char *str) {
    if (!str) return NULL;
    
    StringBuilder *sb = sbCreate(strlen(str) * 3 + 1);
    if (!sb) return NULL;
    
    for (size_t i = 0; str[i]; i++) {
        unsigned char c = (unsigned char)str[i];
        
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            char ch[2] = {c, '\0'};
            sbAppend(sb, ch);
        } else {
            sbAppendFormat(sb, "%%%02X", c);
        }
    }
    
    char *result = strdup(sbToString(sb));
    sbFree(sb);
    return result;
}

// URL decode string
char* urlDecode(const char *str) {
    if (!str) return NULL;
    
    StringBuilder *sb = sbCreate(strlen(str) + 1);
    if (!sb) return NULL;
    
    for (size_t i = 0; str[i]; i++) {
        if (str[i] == '%' && isxdigit((unsigned char)str[i+1]) && 
            isxdigit((unsigned char)str[i+2])) {
            char hex[3] = {str[i+1], str[i+2], '\0'};
            char decoded = (char)strtol(hex, NULL, 16);
            char ch[2] = {decoded, '\0'};
            sbAppend(sb, ch);
            i += 2;
        } else if (str[i] == '+') {
            sbAppend(sb, " ");
        } else {
            char ch[2] = {str[i], '\0'};
            sbAppend(sb, ch);
        }
    }
    
    char *result = strdup(sbToString(sb));
    sbFree(sb);
    return result;
}

// Base64 encoding
static const char base64Chars[] = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char* base64Encode(const char *data, size_t length) {
    if (!data || length == 0) return NULL;
    
    StringBuilder *sb = sbCreate((length / 3 + 1) * 4 + 1);
    if (!sb) return NULL;
    
    for (size_t i = 0; i < length; i += 3) {
        size_t bytesRemaining = length - i;
        size_t a = data[i];
        size_t b = (bytesRemaining > 1) ? data[i + 1] : 0;
        size_t c = (bytesRemaining > 2) ? data[i + 2] : 0;
        
        size_t bitmap = ((a & 0xFF) << 16) | ((b & 0xFF) << 8) | (c & 0xFF);
        
        sbAppendFormat(sb, "%c", base64Chars[(bitmap >> 18) & 0x3F]);
        sbAppendFormat(sb, "%c", base64Chars[(bitmap >> 12) & 0x3F]);
        sbAppendFormat(sb, "%c", bytesRemaining > 1 ? base64Chars[(bitmap >> 6) & 0x3F] : '=');
        sbAppendFormat(sb, "%c", bytesRemaining > 2 ? base64Chars[bitmap & 0x3F] : '=');
    }
    
    char *result = strdup(sbToString(sb));
    sbFree(sb);
    return result;
}

// =============================================================================
// DEMONSTRATION FUNCTIONS
// =============================================================================

void demonstrateStringBuilder() {
    printf("=== STRING BUILDER ===\n");
    
    StringBuilder *sb = sbCreate(100);
    sbAppend(sb, "Hello");
    sbAppend(sb, ", ");
    sbAppend(sb, "World!");
    sbAppendFormat(sb, " The answer is %d.", 42);
    
    printf("Built string: %s\n", sbToString(sb));
    printf("Length: %zu, Capacity: %zu\n", sb->length, sb->capacity);
    
    sbFree(sb);
    printf("\n");
}

void demonstratePatternMatching() {
    printf("=== PATTERN MATCHING ===\n");
    
    const char *str = "Hello World";
    printf("Wildcard match 'H*o*': %s\n", 
           wildcardMatch("H*o*", str) ? "Yes" : "No");
    printf("Wildcard match 'Hell?': %s\n", 
           wildcardMatch("Hell?", str) ? "Yes" : "No");
    
    printf("Substring 'l' count: %d\n", countSubstring(str, "l"));
    
    char *replaced = replaceAll(str, "l", "L");
    printf("Replace 'l' with 'L': %s\n", replaced);
    free(replaced);
    
    printf("\n");
}

void demonstrateStringTransformation() {
    printf("=== STRING TRANSFORMATION ===\n");
    
    const char *str = "hello world from c programming";
    
    char *titleCase = toTitleCase(str);
    printf("Title case: %s\n", titleCase);
    free(titleCase);
    
    char *reversedWords = reverseWords(str);
    printf("Reversed words: %s\n", reversedWords);
    free(reversedWords);
    
    char *noDuplicates = removeDuplicates("programming");
    printf("Remove duplicates: %s\n", noDuplicates);
    free(noDuplicates);
    
    printf("\n");
}

void demonstrateStringAnalysis() {
    printf("=== STRING ANALYSIS ===\n");
    
    printf("Is 'A man, a plan, a canal: Panama' palindrome: %s\n", 
           isPalindromeAdvanced("A man, a plan, a canal: Panama") ? "Yes" : "No");
    
    char *longestPal = longestPalindromeSubstring("babad");
    printf("Longest palindrome in 'babad': %s\n", longestPal);
    free(longestPal);
    
    printf("\n");
}

void demonstrateValidation() {
    printf("=== STRING VALIDATION ===\n");
    
    printf("Email 'user@example.com': %s\n", 
           isValidEmail("user@example.com") ? "Valid" : "Invalid");
    printf("Email 'invalid@': %s\n", 
           isValidEmail("invalid@") ? "Valid" : "Invalid");
    
    printf("Phone '(555) 123-4567': %s\n", 
           isValidPhoneNumber("(555) 123-4567") ? "Valid" : "Invalid");
    
    printf("URL 'https://www.example.com': %s\n", 
           isValidURL("https://www.example.com") ? "Valid" : "Invalid");
    
    printf("\n");
}

void demonstrateEncoding() {
    printf("=== STRING ENCODING ===\n");
    
    const char *original = "Hello World!";
    
    char *encoded = urlEncode(original);
    printf("URL encoded: %s\n", encoded);
    
    char *decoded = urlDecode(encoded);
    printf("URL decoded: %s\n", decoded);
    
    char *base64 = base64Encode(original, strlen(original));
    printf("Base64 encoded: %s\n", base64);
    
    free(encoded);
    free(decoded);
    free(base64);
    
    printf("\n");
}

int main() {
    printf("Advanced String Utilities\n");
    printf("========================\n\n");
    
    demonstrateStringBuilder();
    demonstratePatternMatching();
    demonstrateStringTransformation();
    demonstrateStringAnalysis();
    demonstrateValidation();
    demonstrateEncoding();
    
    printf("All advanced string utilities demonstrated!\n");
    return 0;
}
