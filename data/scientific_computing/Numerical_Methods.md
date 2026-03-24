# Numerical Methods and Scientific Computing

This file contains comprehensive numerical methods and scientific computing examples in C, including root finding, numerical integration, differentiation, linear algebra, statistics, and optimization algorithms.

## 📚 Numerical Methods Overview

### 🔢 Root Finding
- **Bisection Method**: Bracketing method for root finding
- **Newton-Raphson**: Iterative method using derivatives
- **Secant Method**: Derivative-free Newton method

### 📊 Numerical Integration
- **Trapezoidal Rule**: Linear approximation
- **Simpson's Rule**: Quadratic approximation
- **Midpoint Rule**: Rectangle approximation

### 📈 Numerical Differentiation
- **Forward Difference**: Right-point approximation
- **Backward Difference**: Left-point approximation
- **Central Difference**: Centered approximation

## 🔢 Root Finding Methods

### Bisection Method
```c
double bisection(double (*f)(double), double a, double b) {
    if (f(a) * f(b) >= 0) {
        printf("No root in the interval [%.2f, %.2f]\n", a, b);
        return NAN;
    }
    
    double c;
    int iterations = 0;
    
    while ((b - a) >= EPSILON && iterations < MAX_ITERATIONS) {
        c = (a + b) / 2;
        
        if (f(c) == 0.0) {
            return c;
        } else if (f(c) * f(a) < 0) {
            b = c;
        } else {
            a = c;
        }
        
        iterations++;
    }
    
    return c;
}
```

**Characteristics**:
- **Convergence**: Always converges if root exists
- **Speed**: Linear convergence
- **Requirements**: Function must be continuous, sign change in interval
- **Advantages**: Reliable, guaranteed convergence
- **Disadvantages**: Slow, requires bracketing interval

### Newton-Raphson Method
```c
double newtonRaphson(double (*f)(double), double (*f_prime)(double), double x0) {
    double x = x0;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = f(x);
        double fpx = f_prime(x);
        
        if (fabs(fpx) < EPSILON) {
            printf("Derivative too small, method failed\n");
            return NAN;
        }
        
        double x_new = x - fx / fpx;
        
        if (fabs(x_new - x) < EPSILON) {
            return x_new;
        }
        
        x = x_new;
        iterations++;
    }
    
    return x;
}
```

**Characteristics**:
- **Convergence**: Quadratic convergence near root
- **Speed**: Fast convergence
- **Requirements**: Differentiable function, good initial guess
- **Advantages**: Fast, efficient
- **Disadvantages**: May diverge, requires derivative

### Secant Method
```c
double secantMethod(double (*f)(double), double x0, double x1) {
    double x_prev = x0;
    double x_curr = x1;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double f_prev = f(x_prev);
        double f_curr = f(x_curr);
        
        if (fabs(f_curr - f_prev) < EPSILON) {
            return NAN;
        }
        
        double x_next = x_curr - f_curr * (x_curr - x_prev) / (f_curr - f_prev);
        
        if (fabs(x_next - x_curr) < EPSILON) {
            return x_next;
        }
        
        x_prev = x_curr;
        x_curr = x_next;
        iterations++;
    }
    
    return x_curr;
}
```

**Characteristics**:
- **Convergence**: Superlinear (≈1.618)
- **Speed**: Faster than bisection, slower than Newton
- **Requirements**: Two initial points, no derivative
- **Advantages**: No derivative needed
- **Disadvantages**: May diverge, requires good initial points

## 📊 Numerical Integration

### Trapezoidal Rule
```c
double trapezoidalRule(double (*f)(double), double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.5 * (f(a) + f(b));
    
    for (int i = 1; i < n; i++) {
        sum += f(a + i * h);
    }
    
    return sum * h;
}
```

**Error Analysis**:
- **Error**: O(h²)
- **Accuracy**: Second-order accurate
- **Use Case**: General purpose integration

### Simpson's Rule
```c
double simpsonsRule(double (*f)(double), double a, double b, int n) {
    if (n <= 0 || n % 2 != 0) return 0;
    
    double h = (b - a) / n;
    double sum = f(a) + f(b);
    
    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        if (i % 2 == 0) {
            sum += 2 * f(x);
        } else {
            sum += 4 * f(x);
        }
    }
    
    return sum * h / 3;
}
```

**Error Analysis**:
- **Error**: O(h⁴)
- **Accuracy**: Fourth-order accurate
- **Use Case**: Smooth functions, high accuracy needed

### Midpoint Rule
```c
double midpointRule(double (*f)(double), double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0;
    
    for (int i = 0; i < n; i++) {
        double x = a + (i + 0.5) * h;
        sum += f(x);
    }
    
    return sum * h;
}
```

**Error Analysis**:
- **Error**: O(h²)
- **Accuracy**: Second-order accurate
- **Use Case**: Simple implementation

## 📈 Numerical Differentiation

### Forward Difference
```c
double forwardDifference(double (*f)(double), double x, double h) {
    return (f(x + h) - f(x)) / h;
}
```

**Error**: O(h) - First-order accurate

### Backward Difference
```c
double backwardDifference(double (*f)(double), double x, double h) {
    return (f(x) - f(x - h)) / h;
}
```

**Error**: O(h) - First-order accurate

### Central Difference
```c
double centralDifference(double (*f)(double), double x, double h) {
    return (f(x + h) - f(x - h)) / (2 * h);
}
```

**Error**: O(h²) - Second-order accurate

### Second Derivative
```c
double secondDerivative(double (*f)(double), double x, double h) {
    return (f(x + h) - 2 * f(x) + f(x - h)) / (h * h);
}
```

**Error**: O(h²) - Second-order accurate

## 🔢 Linear Algebra

### Vector Operations
```c
typedef struct {
    double* data;
    int size;
} Vector;

// Dot product
double dotProduct(Vector* a, Vector* b) {
    double sum = 0;
    for (int i = 0; i < a->size; i++) {
        sum += a->data[i] * b->data[i];
    }
    return sum;
}

// Vector magnitude
double vectorMagnitude(Vector* v) {
    return sqrt(dotProduct(v, v));
}
```

### Matrix Operations
```c
typedef struct {
    double** data;
    int rows, cols;
} Matrix;

// Matrix multiplication
Matrix* matrixMultiply(Matrix* a, Matrix* b) {
    Matrix* result = createMatrix(a->rows, b->cols);
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            result->data[i][j] = 0;
            for (int k = 0; k < a->cols; k++) {
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
    
    return result;
}
```

### Gaussian Elimination
```c
int gaussianElimination(Matrix* A, Vector* b, Vector* x) {
    int n = A->rows;
    
    // Forward elimination
    for (int i = 0; i < n; i++) {
        // Find pivot
        int pivot = i;
        for (int j = i + 1; j < n; j++) {
            if (fabs(A->data[j][i]) > fabs(A->data[pivot][i])) {
                pivot = j;
            }
        }
        
        // Swap rows if necessary
        if (pivot != i) {
            // Swap matrix rows and RHS values
        }
        
        // Eliminate column
        for (int j = i + 1; j < n; j++) {
            double factor = A->data[j][i] / A->data[i][i];
            for (int k = i; k < n; k++) {
                A->data[j][k] -= factor * A->data[i][k];
            }
            b->data[j] -= factor * b->data[i];
        }
    }
    
    // Back substitution
    for (int i = n - 1; i >= 0; i--) {
        x->data[i] = b->data[i];
        for (int j = i + 1; j < n; j++) {
            x->data[i] -= A->data[i][j] * x->data[j];
        }
        x->data[i] /= A->data[i][i];
    }
    
    return 1;
}
```

## 📊 Statistics

### Basic Statistics
```c
// Mean
double calculateMean(double* data, int size) {
    double sum = 0;
    for (int i = 0; i < size; i++) {
        sum += data[i];
    }
    return sum / size;
}

// Standard deviation
double calculateStdDev(double* data, int size) {
    double mean = calculateMean(data, size);
    double sum = 0;
    
    for (int i = 0; i < size; i++) {
        sum += (data[i] - mean) * (data[i] - mean);
    }
    
    return sqrt(sum / (size - 1));
}

// Correlation coefficient
double calculateCorrelation(double* x, double* y, int size) {
    double mean_x = calculateMean(x, size);
    double mean_y = calculateMean(y, size);
    
    double sum_xy = 0, sum_xx = 0, sum_yy = 0;
    
    for (int i = 0; i < size; i++) {
        sum_xy += (x[i] - mean_x) * (y[i] - mean_y);
        sum_xx += (x[i] - mean_x) * (x[i] - mean_x);
        sum_yy += (y[i] - mean_y) * (y[i] - mean_y);
    }
    
    return sum_xy / sqrt(sum_xx * sum_yy);
}
```

## 🔧 Special Functions

### Exponential Function (Series)
```c
double expSeries(double x, int terms) {
    double result = 1.0;
    double term = 1.0;
    
    for (int i = 1; i <= terms; i++) {
        term *= x / i;
        result += term;
    }
    
    return result;
}
```

**Series**: $e^x = \sum_{n=0}^{\infty} \frac{x^n}{n!}$

### Natural Logarithm (Newton's Method)
```c
double logNewton(double x) {
    if (x <= 0) return NAN;
    if (x == 1) return 0;
    
    double y = x - 1; // Initial guess
    
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double f = exp(y) - x;
        double f_prime = exp(y);
        
        double y_new = y - f / f_prime;
        
        if (fabs(y_new - y) < EPSILON) {
            return y_new;
        }
        
        y = y_new;
    }
    
    return y;
}
```

### Trigonometric Functions (Taylor Series)
```c
// Sine function
double sinTaylor(double x, int terms) {
    // Reduce x to [-π, π]
    while (x > PI) x -= 2 * PI;
    while (x < -PI) x += 2 * PI;
    
    double result = 0;
    double term = x;
    int sign = 1;
    
    for (int i = 1; i <= terms; i++) {
        if (sign > 0) {
            result += term;
        } else {
            result -= term;
        }
        
        term *= x * x / ((2 * i) * (2 * i + 1));
        sign = -sign;
    }
    
    return result;
}
```

**Sine Series**: $\sin(x) = \sum_{n=0}^{\infty} (-1)^n \frac{x^{2n+1}}{(2n+1)!}$

## 🎯 Optimization

### Golden Section Search
```c
double goldenSectionSearch(double (*f)(double), double a, double b, double tolerance) {
    const double golden_ratio = (sqrt(5) - 1) / 2;
    
    double x1 = b - golden_ratio * (b - a);
    double x2 = a + golden_ratio * (b - a);
    
    double f1 = f(x1);
    double f2 = f(x2);
    
    while ((b - a) > tolerance) {
        if (f1 < f2) {
            b = x2;
            x2 = x1;
            f2 = f1;
            x1 = b - golden_ratio * (b - a);
            f1 = f(x1);
        } else {
            a = x1;
            x1 = x2;
            f1 = f2;
            x2 = a + golden_ratio * (b - a);
            f2 = f(x2);
        }
    }
    
    return (a + b) / 2;
}
```

**Golden Ratio**: $\phi = \frac{\sqrt{5} - 1}{2} \approx 0.618$

## 💡 Implementation Details

### Error Handling
```c
#define EPSILON 1e-10
#define MAX_ITERATIONS 1000

// Check for convergence
if (fabs(x_new - x) < EPSILON) {
    return x_new;
}

// Check for divergence
if (iterations >= MAX_ITERATIONS) {
    printf("Maximum iterations reached\n");
    return NAN;
}
```

### Numerical Stability
```c
// Avoid division by zero
if (fabs(denominator) < EPSILON) {
    printf("Denominator too small\n");
    return NAN;
}

// Check for valid input
if (x <= 0 && function == log) {
    printf("Invalid input for logarithm\n");
    return NAN;
}
```

## 📊 Performance Analysis

| Method | Convergence | Speed | Stability | Requirements |
|--------|-------------|-------|----------|-------------|
| Bisection | Guaranteed | Slow | Very stable | Bracketing interval |
| Newton | Quadratic | Fast | Unstable | Derivative, good guess |
| Secant | Superlinear | Medium | Medium | Two initial points |
| Trapezoidal | O(h²) | Fast | Stable | Continuous function |
| Simpson | O(h⁴) | Fast | Stable | Smooth function |

## 🚀 Advanced Topics

### 1. Adaptive Methods
```c
// Adaptive integration
double adaptiveIntegration(double (*f)(double), double a, double b, double tolerance) {
    double result = 0;
    // Recursive subdivision with error estimation
    // Use Simpson's rule where function is smooth
    // Use trapezoidal rule where function changes rapidly
}
```

### 2. Multi-dimensional Methods
```c
// Multi-dimensional root finding
void newtonND(double (*f[])(double*), double* x, int n) {
    // Jacobian matrix calculation
    // Multi-dimensional Newton iteration
}
```

### 3. Sparse Matrix Methods
```c
// Sparse matrix storage
typedef struct {
    double* values;
    int* row_indices;
    int* col_indices;
    int nnz; // Number of non-zero elements
} SparseMatrix;
```

### 4. Parallel Computing
```c
// Parallel integration
double parallelIntegration(double (*f)(double), double a, double b, int n, int num_threads) {
    // Divide integration range among threads
    // Combine partial results
}
```

## ⚠️ Common Pitfalls

### 1. Division by Zero
```c
// Wrong - No check for zero denominator
double derivative = (f(x + h) - f(x)) / h;

// Right - Check for small denominator
if (fabs(h) < EPSILON) {
    return NAN;
}
double derivative = (f(x + h) - f(x)) / h;
```

### 2. Poor Initial Guess
```c
// Wrong - Bad initial guess for Newton's method
double root = newtonRaphson(f, f_prime, 1000); // Far from actual root

// Right - Good initial guess or use bracketing method
double root = newtonRaphson(f, f_prime, 2.0); // Close to root
```

### 3. Insufficient Precision
```c
// Wrong - Too large step size
double derivative = centralDifference(f, x, 1.0); // Large h, poor accuracy

// Right - Appropriate step size
double derivative = centralDifference(f, x, 1e-6); // Small h, good accuracy
```

### 4. Not Checking Convergence
```c
// Wrong - No convergence check
for (int i = 0; i < MAX_ITERATIONS; i++) {
    x = x - f(x) / f_prime(x);
}

// Right - Check convergence
for (int i = 0; i < MAX_ITERATIONS; i++) {
    double x_new = x - f(x) / f_prime(x);
    if (fabs(x_new - x) < EPSILON) {
        return x_new;
    }
    x = x_new;
}
```

## 🔧 Real-World Applications

### 1. Physics Simulations
```c
// Numerical integration for trajectory calculation
double calculateTrajectory(double (*acceleration)(double), double t0, double t1) {
    return simpsonsRule(acceleration, t0, t1, 1000);
}
```

### 2. Financial Calculations
```c
// Option pricing using numerical methods
double optionPrice(double (*payoff)(double), double r, double sigma, double T) {
    // Monte Carlo or finite difference methods
}
```

### 3. Engineering Analysis
```c
// Root finding for design optimization
double findOptimalParameters(double (*objective)(double*), double* params) {
    // Multi-dimensional optimization
}
```

### 4. Data Analysis
```c
// Statistical analysis of experimental data
void analyzeData(double* data, int n) {
    double mean = calculateMean(data, n);
    double std_dev = calculateStdDev(data, n);
    // Generate statistics report
}
```

## 🎓 Best Practices

### 1. Error Estimation
```c
// Always estimate and report numerical errors
double error = fabs(approximate - exact);
printf("Result: %.6f ± %.6e\n", approximate, error);
```

### 2. Method Selection
```c
// Choose appropriate method based on requirements
if (need_high_accuracy && function_smooth) {
    result = simpsonsRule(f, a, b, n);
} else if (need_robustness) {
    result = bisection(f, a, b);
} else {
    result = trapezoidalRule(f, a, b, n);
}
```

### 3. Validation
```c
// Validate results with known test cases
double test_root = newtonRaphson(test_function, test_derivative, 1.0);
assert(fabs(test_root - known_root) < tolerance);
```

### 4. Documentation
```c
// Document numerical methods used
/*
 * Method: Newton-Raphson
 * Function: f(x) = x³ - 2x - 5
 * Initial guess: x₀ = 2
 * Tolerance: 1e-10
 * Iterations: 5
 */
```

### 5. Performance Optimization
```c
// Optimize critical sections
double fast_exp(double x) {
    // Use lookup table for common values
    if (x < 1.0) return exp_table[(int)(x * 100)];
    return exp(x); // Fall back to library function
}
```

Numerical methods provide powerful tools for solving mathematical problems that cannot be solved analytically. Master these techniques to tackle complex scientific and engineering challenges!
