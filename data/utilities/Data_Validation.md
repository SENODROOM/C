# Data Validation Utilities

This file contains comprehensive data validation utilities for C programming, including numeric validation, string validation, date/time validation, business rule validation, and validation chains for complex scenarios.

## 📚 Validation Categories

### 🔢 Numeric Validation
Integer, float, range, and positivity validation

### 🔤 String Validation
Length, format, pattern, and character set validation

### 📅 Date/Time Validation
Date, time, and format validation

### 💼 Business Validation
Credit cards, emails, phone numbers, passwords

### ⛓️ Validation Chains
Multiple rule validation with error aggregation

## 🔢 Numeric Validation

### Integer Validation
```c
ValidationResult validateInteger(const char *str);
```

**Validates**:
- Optional leading sign (+/-)
- All remaining characters are digits
- Non-empty string

**Examples**:
- `"123"` ✓ Valid
- `"-456"` ✓ Valid
- `"+789"` ✓ Valid
- `"12a34"` ✗ Invalid character
- `"+"` ✗ Sign without digits

### Float Validation
```c
ValidationResult validateFloat(const char *str);
```

**Validates**:
- Optional leading sign
- Single decimal point
- At least one digit
- Valid numeric characters only

**Examples**:
- `"123.45"` ✓ Valid
- `"-0.5"` ✓ Valid
- `"123"` ✓ Valid
- `"123..45"` ✗ Multiple decimal points
- `"abc"` ✗ Invalid characters

### Range Validation
```c
ValidationResult validateRange(int value, int min, int max);
ValidationResult validatePositive(int value);
```

**Use Cases**:
- Age validation (18-65)
- Score validation (0-100)
- Quantity validation (positive only)

## 🔤 String Validation

### Basic String Validation
```c
ValidationResult validateNotEmpty(const char *str);
ValidationResult validateLength(const char *str, int min, int max);
```

### Character Set Validation
```c
ValidationResult validateAlpha(const char *str);
ValidationResult validateAlphanumeric(const char *str);
```

**Validates**:
- **Alpha**: Letters only (A-Z, a-z)
- **Alphanumeric**: Letters and numbers only

### Pattern Validation
```c
ValidationResult validatePattern(const char *str, const char *pattern);
```

**Pattern Support**:
- `*` : Matches any sequence of characters
- `?` : Matches any single character

**Examples**:
- `"test@example.com"` with `"*@*.*"` ✓ Valid
- `"123-45-6789"` with `"???-??-????"` ✓ Valid

## 📅 Date/Time Validation

### Date Validation
```c
ValidationResult validateDate(int day, int month, int year);
ValidationResult validateDateString(const char *dateStr);
```

**Validates**:
- Year range: 1900-2100
- Month range: 1-12
- Day range: Varies by month
- Leap year handling for February
- Format: DD/MM/YYYY for string validation

**Leap Year Logic**:
```c
if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
    daysInMonth = 29; // February
} else {
    daysInMonth = 28;
}
```

### Time Validation
```c
ValidationResult validateTime(int hour, int minute, int second);
```

**Validates**:
- Hour: 0-23
- Minute: 0-59
- Second: 0-59

## 💼 Business Validation

### Credit Card Validation
```c
ValidationResult validateCreditCard(const char *cardNumber);
```

**Features**:
- Luhn algorithm implementation
- Support for spaces and dashes
- Length validation (13-19 digits)
- Character validation

**Luhn Algorithm Steps**:
1. Clean input (remove spaces/dashes)
2. Double every second digit from right
3. Subtract 9 if result > 9
4. Sum all digits
5. Valid if sum % 10 == 0

### Email Validation
```c
ValidationResult validateEmailAdvanced(const char *email);
```

**Validates**:
- Single @ symbol
- Local part: 1-64 characters
- Domain part: 4-253 characters
- Valid character sets
- At least one dot in domain
- No leading/trailing dots

**Valid Characters**:
- **Local**: alphanumeric, ., _, -, +, %
- **Domain**: alphanumeric, -, .

### Phone Number Validation
```c
ValidationResult validatePhoneNumberAdvanced(const char *phone);
```

**Validates**:
- At least 10 digits
- Optional leading + (international)
- Allows spaces, dashes, parentheses
- Valid character set

**Supported Formats**:
- `(555) 123-4567`
- `+1 555 123 4567`
- `555-123-4567`
- `5551234567`

### Password Validation
```c
ValidationResult validatePassword(const char *password);
```

**Requirements**:
- Minimum 8 characters
- At least one uppercase letter
- At least one lowercase letter
- At least one digit
- At least one special character

**Special Characters**:
`!@#$%^&*()_+-=[]{}|;:,.<>?`

## ⛓️ Validation Chains

### Chain Validation
```c
typedef struct {
    ValidationResult (*validator)(const char *value);
    const char *fieldName;
} ValidationRule;

ValidationResult validateChain(const char *value, ValidationRule *rules, int ruleCount);
```

**Features**:
- Multiple validators per field
- Field name in error messages
- Early termination on first failure
- Composable validation logic

**Example Usage**:
```c
ValidationRule usernameRules[] = {
    {validateNotEmpty, "Username"},
    {validateLength, "Username"},
    {validateAlphanumeric, "Username"}
};

ValidationResult result = validateChain(username, usernameRules, 3);
```

## 💡 Implementation Details

### ValidationResult Structure
```c
typedef struct {
    int isValid;
    char errorMessage[256];
} ValidationResult;
```

**Benefits**:
- Consistent return type
- Detailed error messages
- Easy error handling
- Chainable validation

### Error Message Format
```c
snprintf(result.errorMessage, sizeof(result.errorMessage), 
         "Field %s: %s", fieldName, specificError);
```

**Features**:
- Field name prefix
- Specific error description
- Buffer overflow protection
- Consistent formatting

## 🚀 Advanced Techniques

### 1. Custom Validators
```c
ValidationResult validateCustomRule(const char *value) {
    ValidationResult result = {0, ""};
    
    // Custom validation logic
    if (/* condition */) {
        strcpy(result.errorMessage, "Custom validation failed");
        return result;
    }
    
    result.isValid = 1;
    return result;
}
```

### 2. Conditional Validation
```c
ValidationResult validateConditional(const char *value, int condition) {
    if (!condition) {
        ValidationResult result = {1, ""};
        return result; // Skip validation
    }
    return validateNotEmpty(value);
}
```

### 3. Cross-Field Validation
```c
ValidationResult validatePasswordMatch(const char *password, const char *confirm) {
    ValidationResult result = {0, ""};
    
    if (strcmp(password, confirm) != 0) {
        strcpy(result.errorMessage, "Passwords do not match");
        return result;
    }
    
    result.isValid = 1;
    return result;
}
```

### 4. Validation Caching
```c
typedef struct {
    char *value;
    ValidationResult result;
    time_t timestamp;
} ValidationCache;

// Cache validation results for performance
```

## 📊 Performance Analysis

| Validator | Time Complexity | Space Complexity | Typical Use |
|------------|-----------------|------------------|-------------|
| Integer | O(n) | O(1) | Form input |
| Float | O(n) | O(1) | Numeric input |
| Email | O(n) | O(1) | User registration |
| Credit Card | O(n) | O(1) | Payment processing |
| Password | O(n) | O(1) | Authentication |

## 🧪 Testing Strategies

### 1. Unit Testing
```c
void testIntegerValidation() {
    ValidationResult vr = validateInteger("123");
    assert(vr.isValid == 1);
    
    vr = validateInteger("abc");
    assert(vr.isValid == 0);
    assert(strstr(vr.errorMessage, "Invalid character") != NULL);
}
```

### 2. Edge Case Testing
```c
void testEdgeCases() {
    // Empty inputs
    ValidationResult vr = validateInteger("");
    assert(vr.isValid == 0);
    
    // Boundary values
    vr = validateRange(100, 1, 100);
    assert(vr.isValid == 1);
    
    vr = validateRange(101, 1, 100);
    assert(vr.isValid == 0);
}
```

### 3. Performance Testing
```c
void benchmarkValidation() {
    clock_t start = clock();
    
    for (int i = 0; i < 10000; i++) {
        ValidationResult vr = validateEmail("test@example.com");
        // Use result to prevent optimization
    }
    
    clock_t end = clock();
    double time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Email validation: %f seconds for 10000 iterations\n", time);
}
```

### 4. Integration Testing
```c
void testValidationChain() {
    ValidationRule rules[] = {
        {validateNotEmpty, "Field"},
        {validateLength, "Field"}
    };
    
    ValidationResult vr = validateChain("test", rules, 2);
    assert(vr.isValid == 1);
}
```

## ⚠️ Common Pitfalls

### 1. Memory Safety
```c
// Wrong - buffer overflow
char buffer[10];
strcpy(buffer, veryLongString);

// Right - use safe functions
ValidationResult vr = validateLength(input, 0, 9);
```

### 2. NULL Pointer Handling
```c
// Wrong - potential crash
if (strlen(input) > 0) { // input might be NULL

// Right - check for NULL first
if (input && strlen(input) > 0) {
```

### 3. Locale Issues
```c
// Wrong - assumes ASCII
if (isalpha(ch)) { // May not work with non-ASCII

// Right - cast to unsigned char
if (isalpha((unsigned char)ch)) {
```

### 4. Integer Overflow
```c
// Wrong - potential overflow
int value = atoi(input); // No error checking

// Right - validate string first
ValidationResult vr = validateInteger(input);
if (vr.isValid) {
    int value = atoi(input);
}
```

### 5. Inconsistent Error Messages
```c
// Wrong - inconsistent format
"Invalid input"
"Input is not valid"
"Validation failed"

// Right - consistent format
"Field: Invalid input"
"Field: Input is not valid"
"Field: Validation failed"
```

## 🔧 Real-World Applications

### 1. Web Form Validation
```c
void validateRegistrationForm(const char *username, const char *email, 
                            const char *password, const char *confirmPassword) {
    ValidationRule usernameRules[] = {
        {validateNotEmpty, "Username"},
        {validateLength, "Username"},
        {validateAlphanumeric, "Username"}
    };
    
    ValidationResult vr = validateChain(username, usernameRules, 3);
    if (!vr.isValid) {
        printf("Username error: %s\n", vr.errorMessage);
        return;
    }
    
    vr = validateEmailAdvanced(email);
    if (!vr.isValid) {
        printf("Email error: %s\n", vr.errorMessage);
        return;
    }
    
    vr = validatePassword(password);
    if (!vr.isValid) {
        printf("Password error: %s\n", vr.errorMessage);
        return;
    }
    
    vr = validatePasswordMatch(password, confirmPassword);
    if (!vr.isValid) {
        printf("Password match error: %s\n", vr.errorMessage);
        return;
    }
    
    printf("Form validation passed!\n");
}
```

### 2. Data Import Validation
```c
void validateCSVRow(char **fields, int fieldCount) {
    // Validate each field according to its type
    ValidationResult vr = validateInteger(fields[0]); // ID
    if (!vr.isValid) {
        printf("ID error: %s\n", vr.errorMessage);
        return;
    }
    
    vr = validateAlpha(fields[1]); // Name
    if (!vr.isValid) {
        printf("Name error: %s\n", vr.errorMessage);
        return;
    }
    
    vr = validateEmailAdvanced(fields[2]); // Email
    if (!vr.isValid) {
        printf("Email error: %s\n", vr.errorMessage);
        return;
    }
    
    printf("Row validation passed!\n");
}
```

### 3. API Input Validation
```c
void validateAPIInput(const char *paramName, const char *paramValue, 
                     const char *expectedType) {
    ValidationResult vr = {0, ""};
    
    if (strcmp(expectedType, "int") == 0) {
        vr = validateInteger(paramValue);
    } else if (strcmp(expectedType, "email") == 0) {
        vr = validateEmailAdvanced(paramValue);
    } else if (strcmp(expectedType, "date") == 0) {
        vr = validateDateString(paramValue);
    }
    
    if (!vr.isValid) {
        printf("Parameter '%s' error: %s\n", paramName, vr.errorMessage);
    }
}
```

## 🎓 Best Practices

### 1. Validate Early, Validate Often
```c
// Validate at input boundary
void processUserInput(const char *input) {
    ValidationResult vr = validateNotEmpty(input);
    if (!vr.isValid) {
        // Handle error immediately
        return;
    }
    // Continue processing
}
```

### 2. Provide Clear Error Messages
```c
// Good - specific and actionable
"Password must contain at least one uppercase letter"

// Bad - vague
"Invalid password"
```

### 3. Use Consistent Return Types
```c
// All validators return ValidationResult
ValidationResult validateSomething(const char *input) {
    // Implementation
}
```

### 4. Chain Validations Logically
```c
// Validate format before content
ValidationResult vr = validateInteger(input);
if (vr.isValid) {
    vr = validateRange(atoi(input), min, max);
}
```

### 5. Consider Performance
```c
// Fast checks first
if (strlen(input) < minLength) return error;
if (strlen(input) > maxLength) return error;
// Then do more expensive checks
```

## 🔄 Integration Examples

### 1. Form Processing Pipeline
```c
int processForm(FormData *form) {
    ValidationResult vr;
    
    // Validate all fields
    vr = validateChain(form->name, nameRules, 3);
    if (!vr.isValid) return 0;
    
    vr = validateEmailAdvanced(form->email);
    if (!vr.isValid) return 0;
    
    vr = validateDate(form->day, form->month, form->year);
    if (!vr.isValid) return 0;
    
    // All validations passed
    return 1;
}
```

### 2. Configuration Validation
```c
int validateConfig(Config *config) {
    ValidationRule portRules[] = {
        {validateInteger, "Port"},
        {validateRange, "Port"}
    };
    
    char portStr[10];
    snprintf(portStr, sizeof(portStr), "%d", config->port);
    
    ValidationResult vr = validateChain(portStr, portRules, 2);
    return vr.isValid;
}
```

These data validation utilities provide a comprehensive, extensible framework for ensuring data integrity and security in C applications, with clear error reporting and flexible validation chains.
