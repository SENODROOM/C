#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

// =============================================================================
// DATA VALIDATION UTILITIES
// =============================================================================

// Validation result structure
typedef struct {
    int isValid;
    char errorMessage[256];
} ValidationResult;

// =============================================================================
// NUMERIC VALIDATION
// =============================================================================

// Check if string represents a valid integer
ValidationResult validateInteger(const char *str) {
    ValidationResult result = {0, ""};
    
    if (!str || !*str) {
        strcpy(result.errorMessage, "Empty string");
        return result;
    }
    
    int i = 0;
    
    // Handle optional sign
    if (str[i] == '+' || str[i] == '-') {
        i++;
        if (!str[i]) {
            strcpy(result.errorMessage, "Sign without digits");
            return result;
        }
    }
    
    // Check remaining characters
    while (str[i]) {
        if (!isdigit((unsigned char)str[i])) {
            snprintf(result.errorMessage, sizeof(result.errorMessage), 
                     "Invalid character '%c' at position %d", str[i], i);
            return result;
        }
        i++;
    }
    
    result.isValid = 1;
    return result;
}

// Check if string represents a valid float
ValidationResult validateFloat(const char *str) {
    ValidationResult result = {0, ""};
    
    if (!str || !*str) {
        strcpy(result.errorMessage, "Empty string");
        return result;
    }
    
    int i = 0;
    int digitCount = 0;
    int dotCount = 0;
    
    // Handle optional sign
    if (str[i] == '+' || str[i] == '-') {
        i++;
        if (!str[i]) {
            strcpy(result.errorMessage, "Sign without digits");
            return result;
        }
    }
    
    while (str[i]) {
        if (str[i] == '.') {
            dotCount++;
            if (dotCount > 1) {
                strcpy(result.errorMessage, "Multiple decimal points");
                return result;
            }
        } else if (isdigit((unsigned char)str[i])) {
            digitCount++;
        } else {
            snprintf(result.errorMessage, sizeof(result.errorMessage), 
                     "Invalid character '%c' at position %d", str[i], i);
            return result;
        }
        i++;
    }
    
    if (digitCount == 0) {
        strcpy(result.errorMessage, "No digits found");
        return result;
    }
    
    result.isValid = 1;
    return result;
}

// Check if number is in range
ValidationResult validateRange(int value, int min, int max) {
    ValidationResult result = {0, ""};
    
    if (value < min) {
        snprintf(result.errorMessage, sizeof(result.errorMessage), 
                 "Value %d is below minimum %d", value, min);
        return result;
    }
    
    if (value > max) {
        snprintf(result.errorMessage, sizeof(result.errorMessage), 
                 "Value %d is above maximum %d", value, max);
        return result;
    }
    
    result.isValid = 1;
    return result;
}

// Check if number is positive
ValidationResult validatePositive(int value) {
    ValidationResult result = {0, ""};
    
    if (value <= 0) {
        snprintf(result.errorMessage, sizeof(result.errorMessage), 
                 "Value %d is not positive", value);
        return result;
    }
    
    result.isValid = 1;
    return result;
}

// =============================================================================
// STRING VALIDATION
// =============================================================================

// Check if string is not empty
ValidationResult validateNotEmpty(const char *str) {
    ValidationResult result = {0, ""};
    
    if (!str || !*str) {
        strcpy(result.errorMessage, "String is empty");
        return result;
    }
    
    result.isValid = 1;
    return result;
}

// Check string length constraints
ValidationResult validateLength(const char *str, int minLength, int maxLength) {
    ValidationResult result = {0, ""};
    
    if (!str) {
        strcpy(result.errorMessage, "String is NULL");
        return result;
    }
    
    int length = strlen(str);
    
    if (length < minLength) {
        snprintf(result.errorMessage, sizeof(result.errorMessage), 
                 "String length %d is below minimum %d", length, minLength);
        return result;
    }
    
    if (length > maxLength) {
        snprintf(result.errorMessage, sizeof(result.errorMessage), 
                 "String length %d exceeds maximum %d", length, maxLength);
        return result;
    }
    
    result.isValid = 1;
    return result;
}

// Check if string contains only alphabetic characters
ValidationResult validateAlpha(const char *str) {
    ValidationResult result = {0, ""};
    
    if (!str || !*str) {
        strcpy(result.errorMessage, "Empty string");
        return result;
    }
    
    for (int i = 0; str[i]; i++) {
        if (!isalpha((unsigned char)str[i])) {
            snprintf(result.errorMessage, sizeof(result.errorMessage), 
                     "Non-alphabetic character '%c' at position %d", str[i], i);
            return result;
        }
    }
    
    result.isValid = 1;
    return result;
}

// Check if string contains only alphanumeric characters
ValidationResult validateAlphanumeric(const char *str) {
    ValidationResult result = {0, ""};
    
    if (!str || !*str) {
        strcpy(result.errorMessage, "Empty string");
        return result;
    }
    
    for (int i = 0; str[i]; i++) {
        if (!isalnum((unsigned char)str[i])) {
            snprintf(result.errorMessage, sizeof(result.errorMessage), 
                     "Non-alphanumeric character '%c' at position %d", str[i], i);
            return result;
        }
    }
    
    result.isValid = 1;
    return result;
}

// Check if string matches pattern (simple regex-like)
ValidationResult validatePattern(const char *str, const char *pattern) {
    ValidationResult result = {0, ""};
    
    if (!str || !pattern) {
        strcpy(result.errorMessage, "NULL string or pattern");
        return result;
    }
    
    // Simple pattern matching (supports * and ?)
    const char *s = str;
    const char *p = pattern;
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
            strcpy(result.errorMessage, "Pattern does not match");
            return result;
        }
    }
    
    while (*p == '*') p++;
    
    if (*p) {
        strcpy(result.errorMessage, "Pattern does not match");
        return result;
    }
    
    result.isValid = 1;
    return result;
}

// =============================================================================
// DATE AND TIME VALIDATION
// =============================================================================

// Check if date is valid
ValidationResult validateDate(int day, int month, int year) {
    ValidationResult result = {0, ""};
    
    // Basic range checks
    if (year < 1900 || year > 2100) {
        strcpy(result.errorMessage, "Year must be between 1900 and 2100");
        return result;
    }
    
    if (month < 1 || month > 12) {
        strcpy(result.errorMessage, "Month must be between 1 and 12");
        return result;
    }
    
    // Days in month
    int daysInMonth;
    switch (month) {
        case 2:
            // Check for leap year
            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
                daysInMonth = 29;
            } else {
                daysInMonth = 28;
            }
            break;
        case 4: case 6: case 9: case 11:
            daysInMonth = 30;
            break;
        default:
            daysInMonth = 31;
            break;
    }
    
    if (day < 1 || day > daysInMonth) {
        snprintf(result.errorMessage, sizeof(result.errorMessage), 
                 "Day must be between 1 and %d for month %d", daysInMonth, month);
        return result;
    }
    
    result.isValid = 1;
    return result;
}

// Parse and validate date string (DD/MM/YYYY format)
ValidationResult validateDateString(const char *dateStr) {
    ValidationResult result = {0, ""};
    
    if (!dateStr || !*dateStr) {
        strcpy(result.errorMessage, "Empty date string");
        return result;
    }
    
    int day, month, year;
    if (sscanf(dateStr, "%d/%d/%d", &day, &month, &year) != 3) {
        strcpy(result.errorMessage, "Invalid date format (expected DD/MM/YYYY)");
        return result;
    }
    
    return validateDate(day, month, year);
}

// Check if time is valid
ValidationResult validateTime(int hour, int minute, int second) {
    ValidationResult result = {0, ""};
    
    if (hour < 0 || hour > 23) {
        strcpy(result.errorMessage, "Hour must be between 0 and 23");
        return result;
    }
    
    if (minute < 0 || minute > 59) {
        strcpy(result.errorMessage, "Minute must be between 0 and 59");
        return result;
    }
    
    if (second < 0 || second > 59) {
        strcpy(result.errorMessage, "Second must be between 0 and 59");
        return result;
    }
    
    result.isValid = 1;
    return result;
}

// =============================================================================
// BUSINESS VALIDATION
// =============================================================================

// Validate credit card number (Luhn algorithm)
ValidationResult validateCreditCard(const char *cardNumber) {
    ValidationResult result = {0, ""};
    
    if (!cardNumber || !*cardNumber) {
        strcpy(result.errorMessage, "Empty card number");
        return result;
    }
    
    // Remove spaces and dashes
    char clean[20];
    int cleanIndex = 0;
    for (int i = 0; cardNumber[i] && cleanIndex < 19; i++) {
        if (isdigit((unsigned char)cardNumber[i])) {
            clean[cleanIndex++] = cardNumber[i];
        } else if (cardNumber[i] != ' ' && cardNumber[i] != '-') {
            strcpy(result.errorMessage, "Invalid character in card number");
            return result;
        }
    }
    clean[cleanIndex] = '\0';
    
    // Check length (13-19 digits for most cards)
    if (cleanIndex < 13 || cleanIndex > 19) {
        strcpy(result.errorMessage, "Invalid card number length");
        return result;
    }
    
    // Luhn algorithm
    int sum = 0;
    int doubleDigit = 0;
    
    for (int i = cleanIndex - 1; i >= 0; i--) {
        int digit = clean[i] - '0';
        
        if (doubleDigit) {
            digit *= 2;
            if (digit > 9) {
                digit -= 9;
            }
        }
        
        sum += digit;
        doubleDigit = !doubleDigit;
    }
    
    if (sum % 10 != 0) {
        strcpy(result.errorMessage, "Invalid credit card number (failed Luhn check)");
        return result;
    }
    
    result.isValid = 1;
    return result;
}

// Validate email address
ValidationResult validateEmailAdvanced(const char *email) {
    ValidationResult result = {0, ""};
    
    if (!email || !*email) {
        strcpy(result.errorMessage, "Empty email address");
        return result;
    }
    
    // Find @ symbol
    const char *at = strchr(email, '@');
    if (!at || at == email || strchr(at + 1, '@')) {
        strcpy(result.errorMessage, "Invalid email format (missing or multiple @)");
        return result;
    }
    
    // Check local part
    const char *p = email;
    int localLength = at - email;
    
    if (localLength < 1 || localLength > 64) {
        strcpy(result.errorMessage, "Local part must be 1-64 characters");
        return result;
    }
    
    // Check domain part
    const char *domain = at + 1;
    int domainLength = strlen(domain);
    
    if (domainLength < 4 || domainLength > 253) {
        strcpy(result.errorMessage, "Domain must be 4-253 characters");
        return result;
    }
    
    // Check for valid characters in local part
    for (int i = 0; i < localLength; i++) {
        char c = email[i];
        if (!isalnum((unsigned char)c) && c != '.' && c != '_' && 
            c != '-' && c != '+' && c != '%') {
            strcpy(result.errorMessage, "Invalid character in local part");
            return result;
        }
    }
    
    // Check domain format
    int dotCount = 0;
    for (int i = 0; domain[i]; i++) {
        char c = domain[i];
        if (c == '.') {
            dotCount++;
            if (i == 0 || !domain[i + 1]) {
                strcpy(result.errorMessage, "Invalid domain format (dot at start/end)");
                return result;
            }
        } else if (!isalnum((unsigned char)c) && c != '-' && c != '.') {
            strcpy(result.errorMessage, "Invalid character in domain");
            return result;
        }
    }
    
    if (dotCount < 1) {
        strcpy(result.errorMessage, "Domain must contain at least one dot");
        return result;
    }
    
    result.isValid = 1;
    return result;
}

// Validate phone number (international format)
ValidationResult validatePhoneNumberAdvanced(const char *phone) {
    ValidationResult result = {0, ""};
    
    if (!phone || !*phone) {
        strcpy(result.errorMessage, "Empty phone number");
        return result;
    }
    
    int digitCount = 0;
    int plusCount = 0;
    
    for (int i = 0; phone[i]; i++) {
        char c = phone[i];
        
        if (isdigit((unsigned char)c)) {
            digitCount++;
        } else if (c == '+') {
            plusCount++;
            if (plusCount > 1 || i > 0) {
                strcpy(result.errorMessage, "Invalid plus sign placement");
                return result;
            }
        } else if (c != ' ' && c != '-' && c != '(' && c != ')') {
            strcpy(result.errorMessage, "Invalid character in phone number");
            return result;
        }
    }
    
    if (digitCount < 10) {
        strcpy(result.errorMessage, "Phone number must contain at least 10 digits");
        return result;
    }
    
    result.isValid = 1;
    return result;
}

// Validate password strength
ValidationResult validatePassword(const char *password) {
    ValidationResult result = {0, ""};
    
    if (!password || !*password) {
        strcpy(result.errorMessage, "Empty password");
        return result;
    }
    
    int length = strlen(password);
    if (length < 8) {
        strcpy(result.errorMessage, "Password must be at least 8 characters");
        return result;
    }
    
    int hasUpper = 0, hasLower = 0, hasDigit = 0, hasSpecial = 0;
    
    for (int i = 0; password[i]; i++) {
        if (isupper((unsigned char)password[i])) hasUpper = 1;
        else if (islower((unsigned char)password[i])) hasLower = 1;
        else if (isdigit((unsigned char)password[i])) hasDigit = 1;
        else if (strchr("!@#$%^&*()_+-=[]{}|;:,.<>?", password[i])) hasSpecial = 1;
    }
    
    if (!hasUpper) {
        strcpy(result.errorMessage, "Password must contain at least one uppercase letter");
        return result;
    }
    
    if (!hasLower) {
        strcpy(result.errorMessage, "Password must contain at least one lowercase letter");
        return result;
    }
    
    if (!hasDigit) {
        strcpy(result.errorMessage, "Password must contain at least one digit");
        return result;
    }
    
    if (!hasSpecial) {
        strcpy(result.errorMessage, "Password must contain at least one special character");
        return result;
    }
    
    result.isValid = 1;
    return result;
}

// =============================================================================
// VALIDATION CHAINS
// =============================================================================

// Validation chain for multiple rules
typedef struct {
    ValidationResult (*validator)(const char *value);
    const char *fieldName;
} ValidationRule;

// Validate multiple rules
ValidationResult validateChain(const char *value, ValidationRule *rules, int ruleCount) {
    ValidationResult result = {0, ""};
    
    if (!value) {
        strcpy(result.errorMessage, "Value is NULL");
        return result;
    }
    
    for (int i = 0; i < ruleCount; i++) {
        result = rules[i].validator(value);
        if (!result.isValid) {
            // Prepend field name to error message
            char newError[300];
            snprintf(newError, sizeof(newError), "%s: %s", 
                     rules[i].fieldName, result.errorMessage);
            strcpy(result.errorMessage, newError);
            return result;
        }
    }
    
    result.isValid = 1;
    strcpy(result.errorMessage, "");
    return result;
}

// =============================================================================
// DEMONSTRATION FUNCTIONS
// =============================================================================

void demonstrateNumericValidation() {
    printf("=== NUMERIC VALIDATION ===\n");
    
    // Integer validation
    ValidationResult vr = validateInteger("12345");
    printf("Integer '12345': %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    vr = validateInteger("12a45");
    printf("Integer '12a45': %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    // Float validation
    vr = validateFloat("123.45");
    printf("Float '123.45': %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    vr = validateFloat("123..45");
    printf("Float '123..45': %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    // Range validation
    vr = validateRange(25, 18, 65);
    printf("Age 25 (18-65): %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    vr = validateRange(15, 18, 65);
    printf("Age 15 (18-65): %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    printf("\n");
}

void demonstrateStringValidation() {
    printf("=== STRING VALIDATION ===\n");
    
    // Not empty validation
    ValidationResult vr = validateNotEmpty("Hello");
    printf("String 'Hello': %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    vr = validateNotEmpty("");
    printf("Empty string: %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    // Length validation
    vr = validateLength("Hello", 3, 10);
    printf("Length 'Hello' (3-10): %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    vr = validateLength("Hi", 3, 10);
    printf("Length 'Hi' (3-10): %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    // Alpha validation
    vr = validateAlpha("HelloWorld");
    printf("Alpha 'HelloWorld': %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    vr = validateAlpha("Hello123");
    printf("Alpha 'Hello123': %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    // Pattern validation
    vr = validatePattern("test@example.com", "*@*.*");
    printf("Pattern 'test@example.com' (*@*.*): %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    printf("\n");
}

void demonstrateDateTimeValidation() {
    printf("=== DATE/TIME VALIDATION ===\n");
    
    // Date validation
    ValidationResult vr = validateDate(15, 6, 2023);
    printf("Date 15/6/2023: %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    vr = validateDate(31, 2, 2023);
    printf("Date 31/2/2023: %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    // Date string validation
    vr = validateDateString("25/12/2023");
    printf("Date string '25/12/2023': %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    vr = validateDateString("32/13/2023");
    printf("Date string '32/13/2023': %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    // Time validation
    vr = validateTime(14, 30, 45);
    printf("Time 14:30:45: %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    vr = validateTime(25, 30, 45);
    printf("Time 25:30:45: %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    printf("\n");
}

void demonstrateBusinessValidation() {
    printf("=== BUSINESS VALIDATION ===\n");
    
    // Credit card validation
    ValidationResult vr = validateCreditCard("4539 1488 0343 6467");
    printf("Credit card '4539 1488 0343 6467': %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    vr = validateCreditCard("1234 5678 9012 3456");
    printf("Credit card '1234 5678 9012 3456': %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    // Email validation
    vr = validateEmailAdvanced("user@example.com");
    printf("Email 'user@example.com': %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    vr = validateEmailAdvanced("invalid@");
    printf("Email 'invalid@': %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    // Phone validation
    vr = validatePhoneNumberAdvanced("+1 (555) 123-4567");
    printf("Phone '+1 (555) 123-4567': %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    vr = validatePhoneNumberAdvanced("123");
    printf("Phone '123': %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    // Password validation
    vr = validatePassword("Secure123!");
    printf("Password 'Secure123!': %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    vr = validatePassword("weak");
    printf("Password 'weak': %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    printf("\n");
}

void demonstrateValidationChain() {
    printf("=== VALIDATION CHAIN ===\n");
    
    // Define validation rules for username
    ValidationRule usernameRules[] = {
        {validateNotEmpty, "Username"},
        {validateLength, "Username"},
        {validateAlphanumeric, "Username"}
    };
    
    // Test username
    const char *username = "User123";
    ValidationResult vr = validateChain(username, usernameRules, 3);
    printf("Username 'User123': %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    // Test invalid username
    username = "U"; // Too short
    vr = validateChain(username, usernameRules, 3);
    printf("Username 'U': %s\n", vr.isValid ? "Valid" : vr.errorMessage);
    
    printf("\n");
}

int main() {
    printf("Data Validation Utilities\n");
    printf("========================\n\n");
    
    demonstrateNumericValidation();
    demonstrateStringValidation();
    demonstrateDateTimeValidation();
    demonstrateBusinessValidation();
    demonstrateValidationChain();
    
    printf("All data validation utilities demonstrated!\n");
    return 0;
}
