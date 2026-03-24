#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define PI 3.14159265358979323846
#define EPSILON 1e-10
#define MAX_ITERATIONS 1000

// =============================================================================
// BASIC MATHEMATICAL FUNCTIONS
// =============================================================================

// Factorial function
double factorial(int n) {
    if (n < 0) return -1; // Error for negative input
    if (n == 0 || n == 1) return 1;
    
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

// Combination (n choose k)
double combination(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;
    
    // Use symmetry to minimize calculations
    k = (k < n - k) ? k : n - k;
    
    double result = 1.0;
    for (int i = 1; i <= k; i++) {
        result = result * (n - k + i) / i;
    }
    return result;
}

// =============================================================================
// ROOT FINDING METHODS
// =============================================================================

// Bisection method for finding roots
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

// Newton-Raphson method
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
    
    printf("Maximum iterations reached\n");
    return x;
}

// Secant method
double secantMethod(double (*f)(double), double x0, double x1) {
    double x_prev = x0;
    double x_curr = x1;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double f_prev = f(x_prev);
        double f_curr = f(x_curr);
        
        if (fabs(f_curr - f_prev) < EPSILON) {
            printf("Function values too close, method failed\n");
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
    
    printf("Maximum iterations reached\n");
    return x_curr;
}

// =============================================================================
// NUMERICAL INTEGRATION
// =============================================================================

// Trapezoidal rule
double trapezoidalRule(double (*f)(double), double a, double b, int n) {
    if (n <= 0) return 0;
    
    double h = (b - a) / n;
    double sum = 0.5 * (f(a) + f(b));
    
    for (int i = 1; i < n; i++) {
        sum += f(a + i * h);
    }
    
    return sum * h;
}

// Simpson's rule
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

// Midpoint rule
double midpointRule(double (*f)(double), double a, double b, int n) {
    if (n <= 0) return 0;
    
    double h = (b - a) / n;
    double sum = 0;
    
    for (int i = 0; i < n; i++) {
        double x = a + (i + 0.5) * h;
        sum += f(x);
    }
    
    return sum * h;
}

// =============================================================================
// NUMERICAL DIFFERENTIATION
// =============================================================================

// Forward difference
double forwardDifference(double (*f)(double), double x, double h) {
    return (f(x + h) - f(x)) / h;
}

// Backward difference
double backwardDifference(double (*f)(double), double x, double h) {
    return (f(x) - f(x - h)) / h;
}

// Central difference
double centralDifference(double (*f)(double), double x, double h) {
    return (f(x + h) - f(x - h)) / (2 * h);
}

// Second derivative (central difference)
double secondDerivative(double (*f)(double), double x, double h) {
    return (f(x + h) - 2 * f(x) + f(x - h)) / (h * h);
}

// =============================================================================
// LINEAR ALGEBRA
// =============================================================================

// Vector operations
typedef struct {
    double* data;
    int size;
} Vector;

Vector* createVector(int size) {
    Vector* v = (Vector*)malloc(sizeof(Vector));
    v->data = (double*)malloc(size * sizeof(double));
    v->size = size;
    return v;
}

void freeVector(Vector* v) {
    if (v) {
        free(v->data);
        free(v);
    }
}

// Vector dot product
double dotProduct(Vector* a, Vector* b) {
    if (a->size != b->size) return NAN;
    
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

// Matrix operations
typedef struct {
    double** data;
    int rows, cols;
} Matrix;

Matrix* createMatrix(int rows, int cols) {
    Matrix* m = (Matrix*)malloc(sizeof(Matrix));
    m->data = (double**)malloc(rows * sizeof(double*));
    m->rows = rows;
    m->cols = cols;
    
    for (int i = 0; i < rows; i++) {
        m->data[i] = (double*)malloc(cols * sizeof(double));
    }
    
    return m;
}

void freeMatrix(Matrix* m) {
    if (m) {
        for (int i = 0; i < m->rows; i++) {
            free(m->data[i]);
        }
        free(m->data);
        free(m);
    }
}

// Matrix multiplication
Matrix* matrixMultiply(Matrix* a, Matrix* b) {
    if (a->cols != b->rows) return NULL;
    
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

// Gaussian elimination for solving linear systems
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
            // Swap matrix rows
            for (int j = i; j < n; j++) {
                double temp = A->data[i][j];
                A->data[i][j] = A->data[pivot][j];
                A->data[pivot][j] = temp;
            }
            
            // Swap RHS values
            double temp = b->data[i];
            b->data[i] = b->data[pivot];
            b->data[pivot] = temp;
        }
        
        // Check for zero pivot
        if (fabs(A->data[i][i]) < EPSILON) {
            printf("Matrix is singular\n");
            return 0;
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

// =============================================================================
// STATISTICS
// =============================================================================

// Calculate mean
double calculateMean(double* data, int size) {
    if (size == 0) return 0;
    
    double sum = 0;
    for (int i = 0; i < size; i++) {
        sum += data[i];
    }
    return sum / size;
}

// Calculate standard deviation
double calculateStdDev(double* data, int size) {
    if (size <= 1) return 0;
    
    double mean = calculateMean(data, size);
    double sum = 0;
    
    for (int i = 0; i < size; i++) {
        sum += (data[i] - mean) * (data[i] - mean);
    }
    
    return sqrt(sum / (size - 1));
}

// Calculate correlation coefficient
double calculateCorrelation(double* x, double* y, int size) {
    if (size <= 1) return 0;
    
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

// =============================================================================
// SPECIAL FUNCTIONS
// =============================================================================

// Exponential function (series expansion)
double expSeries(double x, int terms) {
    double result = 1.0;
    double term = 1.0;
    
    for (int i = 1; i <= terms; i++) {
        term *= x / i;
        result += term;
    }
    
    return result;
}

// Natural logarithm (Newton's method)
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

// Sine function (Taylor series)
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

// Cosine function (Taylor series)
double cosTaylor(double x, int terms) {
    // Reduce x to [-π, π]
    while (x > PI) x -= 2 * PI;
    while (x < -PI) x += 2 * PI;
    
    double result = 1;
    double term = 1;
    int sign = -1;
    
    for (int i = 1; i <= terms; i++) {
        term *= x * x / ((2 * i - 1) * (2 * i));
        
        if (sign > 0) {
            result += term;
        } else {
            result -= term;
        }
        
        sign = -sign;
    }
    
    return result;
}

// =============================================================================
// OPTIMIZATION
// =============================================================================

// Golden section search for 1D optimization
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

// =============================================================================
// DEMONSTRATION FUNCTIONS
// =============================================================================

// Sample functions for testing
double sampleFunction1(double x) {
    return x * x - 4; // Root at x = ±2
}

double sampleFunction1_prime(double x) {
    return 2 * x;
}

double sampleFunction2(double x) {
    return x * x * x - 2 * x - 5; // Root near x ≈ 2.09
}

double sampleFunction2_prime(double x) {
    return 3 * x * x - 2;
}

double testFunction(double x) {
    return sin(x) + 0.5 * x;
}

double testFunction_prime(double x) {
    return cos(x) + 0.5;
}

void demonstrateRootFinding() {
    printf("=== ROOT FINDING METHODS ===\n");
    
    // Bisection method
    printf("Bisection method for x² - 4 = 0 in [0, 3]:\n");
    double root1 = bisection(sampleFunction1, 0, 3);
    printf("Root: %.6f (expected: 2.000000)\n", root1);
    
    // Newton-Raphson method
    printf("\nNewton-Raphson for x³ - 2x - 5 = 0, initial guess x₀ = 2:\n");
    double root2 = newtonRaphson(sampleFunction2, sampleFunction2_prime, 2);
    printf("Root: %.6f\n", root2);
    
    // Secant method
    printf("\nSecant method for sin(x) + 0.5x = 0, x₀ = 1, x₁ = 2:\n");
    double root3 = secantMethod(testFunction, 1, 2);
    printf("Root: %.6f\n", root3);
    
    printf("\n");
}

void demonstrateIntegration() {
    printf("=== NUMERICAL INTEGRATION ===\n");
    
    // Define integration limits
    double a = 0, b = PI;
    int n = 1000;
    
    // Test with sin(x) from 0 to π
    printf("Integral of sin(x) from 0 to π:\n");
    printf("Exact value: 2.000000\n");
    
    double trap = trapezoidalRule(sin, a, b, n);
    printf("Trapezoidal rule (n=%d): %.6f\n", n, trap);
    
    double simp = simpsonsRule(sin, a, b, n);
    printf("Simpson's rule (n=%d): %.6f\n", n, simp);
    
    double mid = midpointRule(sin, a, b, n);
    printf("Midpoint rule (n=%d): %.6f\n", n, mid);
    
    printf("\n");
}

void demonstrateDifferentiation() {
    printf("=== NUMERICAL DIFFERENTIATION ===\n");
    
    double x = 1.0;
    double h = 0.0001;
    
    printf("Derivative of sin(x) at x = %.2f\n", x);
    printf("Exact value: cos(%.2f) = %.6f\n", x, cos(x));
    
    double fd = forwardDifference(sin, x, h);
    printf("Forward difference: %.6f\n", fd);
    
    double bd = backwardDifference(sin, x, h);
    printf("Backward difference: %.6f\n", bd);
    
    double cd = centralDifference(sin, x, h);
    printf("Central difference: %.6f\n", cd);
    
    double sd = secondDerivative(sin, x, h);
    printf("Second derivative: %.6f (exact: -sin(%.2f) = %.6f)\n", sd, x, -sin(x));
    
    printf("\n");
}

void demonstrateLinearAlgebra() {
    printf("=== LINEAR ALGEBRA ===\n");
    
    // Create vectors
    Vector* v1 = createVector(3);
    Vector* v2 = createVector(3);
    
    v1->data[0] = 1; v1->data[1] = 2; v1->data[2] = 3;
    v2->data[0] = 4; v2->data[1] = 5; v2->data[2] = 6;
    
    printf("Vector v1: [%.1f, %.1f, %.1f]\n", v1->data[0], v1->data[1], v1->data[2]);
    printf("Vector v2: [%.1f, %.1f, %.1f]\n", v2->data[0], v2->data[1], v2->data[2]);
    
    double dot = dotProduct(v1, v2);
    printf("Dot product: %.1f\n", dot);
    
    printf("Magnitude v1: %.3f\n", vectorMagnitude(v1));
    printf("Magnitude v2: %.3f\n", vectorMagnitude(v2));
    
    // Create matrix and solve linear system
    Matrix* A = createMatrix(3, 3);
    Vector* b = createVector(3);
    Vector* x = createVector(3);
    
    // Set up system: 2x + 3y - z = 1, x - y + 2z = 3, 3x + 2y + z = 2
    A->data[0][0] = 2; A->data[0][1] = 3; A->data[0][2] = -1;
    A->data[1][0] = 1; A->data[1][1] = -1; A->data[1][2] = 2;
    A->data[2][0] = 3; A->data[2][1] = 2; A->data[2][2] = 1;
    
    b->data[0] = 1; b->data[1] = 3; b->data[2] = 2;
    
    printf("\nSolving linear system Ax = b:\n");
    if (gaussianElimination(A, b, x)) {
        printf("Solution: x = %.3f, y = %.3f, z = %.3f\n", x->data[0], x->data[1], x->data[2]);
    }
    
    // Cleanup
    freeVector(v1);
    freeVector(v2);
    freeMatrix(A);
    freeVector(b);
    freeVector(x);
    
    printf("\n");
}

void demonstrateStatistics() {
    printf("=== STATISTICS ===\n");
    
    double data[] = {2.5, 3.1, 2.8, 3.5, 2.9, 3.2, 2.7, 3.0, 3.3, 2.6};
    int size = sizeof(data) / sizeof(data[0]);
    
    printf("Data: ");
    for (int i = 0; i < size; i++) {
        printf("%.1f ", data[i]);
    }
    printf("\n");
    
    double mean = calculateMean(data, size);
    double std_dev = calculateStdDev(data, size);
    
    printf("Mean: %.3f\n", mean);
    printf("Standard deviation: %.3f\n", std_dev);
    
    // Correlation between two datasets
    double x[] = {1, 2, 3, 4, 5};
    double y[] = {2, 4, 5, 4, 5};
    
    double corr = calculateCorrelation(x, y, 5);
    printf("Correlation coefficient: %.3f\n", corr);
    
    printf("\n");
}

void demonstrateSpecialFunctions() {
    printf("=== SPECIAL FUNCTIONS ===\n");
    
    double x = 1.0;
    
    // Exponential function
    printf("exp(%.1f) using series (10 terms): %.6f\n", x, expSeries(x, 10));
    printf("exp(%.1f) using math.h: %.6f\n", x, exp(x));
    
    // Natural logarithm
    double y = 2.71828;
    printf("ln(%.5f) using Newton's method: %.6f\n", y, logNewton(y));
    printf("ln(%.5f) using math.h: %.6f\n", y, log(y));
    
    // Trigonometric functions
    double angle = PI / 4; // 45 degrees
    
    printf("sin(π/4) using Taylor series (10 terms): %.6f\n", sinTaylor(angle, 10));
    printf("sin(π/4) using math.h: %.6f\n", sin(angle));
    
    printf("cos(π/4) using Taylor series (10 terms): %.6f\n", cosTaylor(angle, 10));
    printf("cos(π/4) using math.h: %.6f\n", cos(angle));
    
    printf("\n");
}

void demonstrateOptimization() {
    printf("=== OPTIMIZATION ===\n");
    
    // Golden section search for minimum of f(x) = x² - 4x + 3
    // This has minimum at x = 2
    double (*objective)(double) = [](double x) { return x * x - 4 * x + 3; };
    
    printf("Finding minimum of f(x) = x² - 4x + 3 in [0, 4]\n");
    printf("Expected minimum at x = 2, f(2) = -1\n");
    
    double minimum = goldenSectionSearch(objective, 0, 4, 0.0001);
    double min_value = objective(minimum);
    
    printf("Found minimum at x = %.6f, f(x) = %.6f\n", minimum, min_value);
    
    printf("\n");
}

int main() {
    printf("Numerical Methods and Scientific Computing\n");
    printf("======================================\n\n");
    
    demonstrateRootFinding();
    demonstrateIntegration();
    demonstrateDifferentiation();
    demonstrateLinearAlgebra();
    demonstrateStatistics();
    demonstrateSpecialFunctions();
    demonstrateOptimization();
    
    printf("All numerical methods demonstrated!\n");
    return 0;
}
