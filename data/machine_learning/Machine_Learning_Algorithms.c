#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <float.h>

// =============================================================================
// MACHINE LEARNING FUNDAMENTALS
// =============================================================================

#define MAX_SAMPLES 1000
#define MAX_FEATURES 10
#define LEARNING_RATE 0.01
#define EPOCHS 1000

// Data structures for machine learning
typedef struct {
    double features[MAX_FEATURES];
    int label;
    int feature_count;
} DataSample;

typedef struct {
    double weights[MAX_FEATURES];
    double bias;
    int feature_count;
} Model;

typedef struct {
    DataSample samples[MAX_SAMPLES];
    int sample_count;
    int feature_count;
    int class_count;
} Dataset;

// =============================================================================
// UTILITY FUNCTIONS
// =============================================================================

// Generate random double between 0 and 1
double randomDouble() {
    return (double)rand() / RAND_MAX;
}

// Generate random double in range
double randomDoubleRange(double min, double max) {
    return min + (max - min) * randomDouble();
}

// Sigmoid activation function
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

// Derivative of sigmoid
double sigmoidDerivative(double x) {
    double s = sigmoid(x);
    return s * (1.0 - s);
}

// ReLU activation function
double relu(double x) {
    return x > 0 ? x : 0;
}

// Derivative of ReLU
double reluDerivative(double x) {
    return x > 0 ? 1 : 0;
}

// Mean squared error
double meanSquaredError(double* predicted, double* actual, int size) {
    double error = 0.0;
    for (int i = 0; i < size; i++) {
        double diff = predicted[i] - actual[i];
        error += diff * diff;
    }
    return error / size;
}

// Cross entropy loss
double crossEntropyLoss(double predicted, int actual) {
    return -log(actual == 1 ? predicted : (1.0 - predicted));
}

// =============================================================================
// LINEAR REGRESSION
// =============================================================================

typedef struct {
    double weights[MAX_FEATURES];
    double bias;
    int feature_count;
} LinearRegression;

// Initialize linear regression model
void initLinearRegression(LinearRegression* model, int feature_count) {
    model->feature_count = feature_count;
    model->bias = 0.0;
    
    // Initialize weights with small random values
    for (int i = 0; i < feature_count; i++) {
        model->weights[i] = randomDoubleRange(-0.1, 0.1);
    }
}

// Predict using linear regression
double linearRegressionPredict(LinearRegression* model, double* features) {
    double prediction = model->bias;
    
    for (int i = 0; i < model->feature_count; i++) {
        prediction += model->weights[i] * features[i];
    }
    
    return prediction;
}

// Train linear regression using gradient descent
void trainLinearRegression(LinearRegression* model, Dataset* dataset) {
    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        double total_error = 0.0;
        
        // Calculate gradients
        double weight_gradients[MAX_FEATURES] = {0};
        double bias_gradient = 0.0;
        
        for (int i = 0; i < dataset->sample_count; i++) {
            double prediction = linearRegressionPredict(model, dataset->samples[i].features);
            double error = prediction - dataset->samples[i].label;
            
            // Accumulate gradients
            for (int j = 0; j < model->feature_count; j++) {
                weight_gradients[j] += error * dataset->samples[i].features[j];
            }
            bias_gradient += error;
            total_error += error * error;
        }
        
        // Update weights
        for (int j = 0; j < model->feature_count; j++) {
            model->weights[j] -= LEARNING_RATE * weight_gradients[j] / dataset->sample_count;
        }
        model->bias -= LEARNING_RATE * bias_gradient / dataset->sample_count;
        
        // Print progress
        if (epoch % 100 == 0) {
            printf("Epoch %d, MSE: %.6f\n", epoch, total_error / dataset->sample_count);
        }
    }
}

// =============================================================================
// LOGISTIC REGRESSION (BINARY CLASSIFICATION)
// =============================================================================

typedef struct {
    double weights[MAX_FEATURES];
    double bias;
    int feature_count;
} LogisticRegression;

// Initialize logistic regression model
void initLogisticRegression(LogisticRegression* model, int feature_count) {
    model->feature_count = feature_count;
    model->bias = 0.0;
    
    for (int i = 0; i < feature_count; i++) {
        model->weights[i] = randomDoubleRange(-0.1, 0.1);
    }
}

// Predict probability using logistic regression
double logisticRegressionPredict(LogisticRegression* model, double* features) {
    double z = model->bias;
    
    for (int i = 0; i < model->feature_count; i++) {
        z += model->weights[i] * features[i];
    }
    
    return sigmoid(z);
}

// Predict class (0 or 1)
int logisticRegressionClassify(LogisticRegression* model, double* features) {
    double probability = logisticRegressionPredict(model, features);
    return probability >= 0.5 ? 1 : 0;
}

// Train logistic regression
void trainLogisticRegression(LogisticRegression* model, Dataset* dataset) {
    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        double total_loss = 0.0;
        
        // Calculate gradients
        double weight_gradients[MAX_FEATURES] = {0};
        double bias_gradient = 0.0;
        
        for (int i = 0; i < dataset->sample_count; i++) {
            double probability = logisticRegressionPredict(model, dataset->samples[i].features);
            double error = probability - dataset->samples[i].label;
            
            // Accumulate gradients
            for (int j = 0; j < model->feature_count; j++) {
                weight_gradients[j] += error * dataset->samples[i].features[j];
            }
            bias_gradient += error;
            
            // Calculate loss
            total_loss += crossEntropyLoss(probability, dataset->samples[i].label);
        }
        
        // Update weights
        for (int j = 0; j < model->feature_count; j++) {
            model->weights[j] -= LEARNING_RATE * weight_gradients[j] / dataset->sample_count;
        }
        model->bias -= LEARNING_RATE * bias_gradient / dataset->sample_count;
        
        // Print progress
        if (epoch % 100 == 0) {
            printf("Epoch %d, Loss: %.6f\n", epoch, total_loss / dataset->sample_count);
        }
    }
}

// =============================================================================
// K-NEAREST NEIGHBORS (KNN)
// =============================================================================

typedef struct {
    Dataset* dataset;
    int k;
} KNNModel;

// Initialize KNN model
void initKNN(KNNModel* model, Dataset* dataset, int k) {
    model->dataset = dataset;
    model->k = k;
}

// Calculate Euclidean distance
double euclideanDistance(double* features1, double* features2, int feature_count) {
    double distance = 0.0;
    
    for (int i = 0; i < feature_count; i++) {
        double diff = features1[i] - features2[i];
        distance += diff * diff;
    }
    
    return sqrt(distance);
}

// Predict using KNN
int knnPredict(KNNModel* model, double* features) {
    // Calculate distances to all samples
    double distances[MAX_SAMPLES];
    
    for (int i = 0; i < model->dataset->sample_count; i++) {
        distances[i] = euclideanDistance(features, model->dataset->samples[i].features, 
                                       model->dataset->feature_count);
    }
    
    // Find k nearest neighbors
    int nearest_neighbors[MAX_SAMPLES];
    
    for (int i = 0; i < model->k; i++) {
        int min_index = 0;
        for (int j = 1; j < model->dataset->sample_count; j++) {
            if (distances[j] < distances[min_index]) {
                min_index = j;
            }
        }
        nearest_neighbors[i] = min_index;
        distances[min_index] = INFINITY;
    }
    
    // Vote for class
    int votes[10] = {0}; // Assuming max 10 classes
    
    for (int i = 0; i < model->k; i++) {
        int label = model->dataset->samples[nearest_neighbors[i]].label;
        if (label >= 0 && label < 10) {
            votes[label]++;
        }
    }
    
    // Find class with most votes
    int max_votes = 0;
    int predicted_class = 0;
    
    for (int i = 0; i < 10; i++) {
        if (votes[i] > max_votes) {
            max_votes = votes[i];
            predicted_class = i;
        }
    }
    
    return predicted_class;
}

// =============================================================================
// DECISION TREE
// =============================================================================

typedef struct TreeNode {
    int feature_index;
    double threshold;
    int class_label;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// Calculate Gini impurity
double giniImpurity(int* class_counts, int total_samples) {
    double gini = 1.0;
    
    for (int i = 0; i < 10; i++) {
        if (class_counts[i] > 0) {
            double probability = (double)class_counts[i] / total_samples;
            gini -= probability * probability;
        }
    }
    
    return gini;
}

// Find best split for decision tree
void findBestSplit(Dataset* dataset, int* best_feature, double* best_threshold, double* best_gini) {
    *best_gini = 1.0;
    *best_feature = -1;
    *best_threshold = 0.0;
    
    // Try each feature
    for (int feature = 0; feature < dataset->feature_count; feature++) {
        // Find min and max values for this feature
        double min_val = dataset->samples[0].features[feature];
        double max_val = dataset->samples[0].features[feature];
        
        for (int i = 1; i < dataset->sample_count; i++) {
            if (dataset->samples[i].features[feature] < min_val) {
                min_val = dataset->samples[i].features[feature];
            }
            if (dataset->samples[i].features[feature] > max_val) {
                max_val = dataset->samples[i].features[feature];
            }
        }
        
        // Try different thresholds
        for (int t = 0; t < 10; t++) {
            double threshold = min_val + (max_val - min_val) * t / 9.0;
            
            int left_class_counts[10] = {0};
            int right_class_counts[10] = {0};
            int left_count = 0, right_count = 0;
            
            // Split samples
            for (int i = 0; i < dataset->sample_count; i++) {
                if (dataset->samples[i].features[feature] <= threshold) {
                    left_class_counts[dataset->samples[i].label]++;
                    left_count++;
                } else {
                    right_class_counts[dataset->samples[i].label]++;
                    right_count++;
                }
            }
            
            // Calculate weighted Gini impurity
            double left_gini = giniImpurity(left_class_counts, left_count);
            double right_gini = giniImpurity(right_class_counts, right_count);
            double weighted_gini = (left_gini * left_count + right_gini * right_count) / dataset->sample_count;
            
            if (weighted_gini < *best_gini) {
                *best_gini = weighted_gini;
                *best_feature = feature;
                *best_threshold = threshold;
            }
        }
    }
}

// Create leaf node
TreeNode* createLeafNode(int class_label) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->feature_index = -1;
    node->threshold = 0.0;
    node->class_label = class_label;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Build decision tree recursively
TreeNode* buildDecisionTree(Dataset* dataset, int max_depth, int current_depth) {
    // Check stopping conditions
    if (current_depth >= max_depth || dataset->sample_count < 5) {
        // Find majority class
        int class_counts[10] = {0};
        for (int i = 0; i < dataset->sample_count; i++) {
            class_counts[dataset->samples[i].label]++;
        }
        
        int max_count = 0, majority_class = 0;
        for (int i = 0; i < 10; i++) {
            if (class_counts[i] > max_count) {
                max_count = class_counts[i];
                majority_class = i;
            }
        }
        
        return createLeafNode(majority_class);
    }
    
    // Find best split
    int best_feature;
    double best_threshold, best_gini;
    findBestSplit(dataset, &best_feature, &best_threshold, &best_gini);
    
    // Check if split improves purity
    int current_class_counts[10] = {0};
    for (int i = 0; i < dataset->sample_count; i++) {
        current_class_counts[dataset->samples[i].label]++;
    }
    double current_gini = giniImpurity(current_class_counts, dataset->sample_count);
    
    if (best_feature == -1 || best_gini >= current_gini) {
        // No improvement, create leaf
        int max_count = 0, majority_class = 0;
        for (int i = 0; i < 10; i++) {
            if (current_class_counts[i] > max_count) {
                max_count = current_class_counts[i];
                majority_class = i;
            }
        }
        
        return createLeafNode(majority_class);
    }
    
    // Create internal node
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->feature_index = best_feature;
    node->threshold = best_threshold;
    node->class_label = -1;
    
    // Split dataset
    Dataset left_dataset, right_dataset;
    int left_count = 0, right_count = 0;
    
    for (int i = 0; i < dataset->sample_count; i++) {
        if (dataset->samples[i].features[best_feature] <= best_threshold) {
            left_dataset.samples[left_count++] = dataset->samples[i];
        } else {
            right_dataset.samples[right_count++] = dataset->samples[i];
        }
    }
    
    left_dataset.sample_count = left_count;
    left_dataset.feature_count = dataset->feature_count;
    right_dataset.sample_count = right_count;
    right_dataset.feature_count = dataset->feature_count;
    
    // Recursively build subtrees
    node->left = buildDecisionTree(&left_dataset, max_depth, current_depth + 1);
    node->right = buildDecisionTree(&right_dataset, max_depth, current_depth + 1);
    
    return node;
}

// Predict using decision tree
int decisionTreePredict(TreeNode* tree, double* features) {
    if (tree->feature_index == -1) {
        return tree->class_label;
    }
    
    if (features[tree->feature_index] <= tree->threshold) {
        return decisionTreePredict(tree->left, features);
    } else {
        return decisionTreePredict(tree->right, features);
    }
}

// =============================================================================
// NEURAL NETWORK (SIMPLE MULTI-LAYER PERCEPTRON)
// =============================================================================

#define HIDDEN_NEURONS 4
#define OUTPUT_NEURONS 3

typedef struct {
    double input_weights[HIDDEN_NEURONS][MAX_FEATURES];
    double hidden_weights[OUTPUT_NEURONS][HIDDEN_NEURONS];
    double hidden_biases[HIDDEN_NEURONS];
    double output_biases[OUTPUT_NEURONS];
    double hidden_outputs[HIDDEN_NEURONS];
    double final_outputs[OUTPUT_NEURONS];
    int input_size;
} NeuralNetwork;

// Initialize neural network
void initNeuralNetwork(NeuralNetwork* nn, int input_size) {
    nn->input_size = input_size;
    
    // Initialize weights with random values
    for (int i = 0; i < HIDDEN_NEURONS; i++) {
        for (int j = 0; j < input_size; j++) {
            nn->input_weights[i][j] = randomDoubleRange(-0.5, 0.5);
        }
        nn->hidden_biases[i] = randomDoubleRange(-0.5, 0.5);
    }
    
    for (int i = 0; i < OUTPUT_NEURONS; i++) {
        for (int j = 0; j < HIDDEN_NEURONS; j++) {
            nn->hidden_weights[i][j] = randomDoubleRange(-0.5, 0.5);
        }
        nn->output_biases[i] = randomDoubleRange(-0.5, 0.5);
    }
}

// Forward pass through neural network
void neuralNetworkForward(NeuralNetwork* nn, double* inputs) {
    // Hidden layer
    for (int i = 0; i < HIDDEN_NEURONS; i++) {
        double sum = nn->hidden_biases[i];
        for (int j = 0; j < nn->input_size; j++) {
            sum += nn->input_weights[i][j] * inputs[j];
        }
        nn->hidden_outputs[i] = sigmoid(sum);
    }
    
    // Output layer
    for (int i = 0; i < OUTPUT_NEURONS; i++) {
        double sum = nn->output_biases[i];
        for (int j = 0; j < HIDDEN_NEURONS; j++) {
            sum += nn->hidden_weights[i][j] * nn->hidden_outputs[j];
        }
        nn->final_outputs[i] = sigmoid(sum);
    }
}

// Get prediction from neural network
int neuralNetworkPredict(NeuralNetwork* nn, double* inputs) {
    neuralNetworkForward(nn, inputs);
    
    // Find output with highest probability
    int max_index = 0;
    double max_value = nn->final_outputs[0];
    
    for (int i = 1; i < OUTPUT_NEURONS; i++) {
        if (nn->final_outputs[i] > max_value) {
            max_value = nn->final_outputs[i];
            max_index = i;
        }
    }
    
    return max_index;
}

// Train neural network (simplified backpropagation)
void trainNeuralNetwork(NeuralNetwork* nn, Dataset* dataset) {
    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        double total_error = 0.0;
        
        for (int sample = 0; sample < dataset->sample_count; sample++) {
            // Forward pass
            neuralNetworkForward(nn, dataset->samples[sample].features);
            
            // Calculate error (simplified)
            double target[OUTPUT_NEURONS] = {0};
            if (dataset->samples[sample].label >= 0 && dataset->samples[sample].label < OUTPUT_NEURONS) {
                target[dataset->samples[sample].label] = 1.0;
            }
            
            for (int i = 0; i < OUTPUT_NEURONS; i++) {
                double error = nn->final_outputs[i] - target[i];
                total_error += error * error;
                
                // Backpropagation (simplified)
                double output_delta = error * sigmoidDerivative(nn->final_outputs[i]);
                
                for (int j = 0; j < HIDDEN_NEURONS; j++) {
                    nn->hidden_weights[i][j] -= LEARNING_RATE * output_delta * nn->hidden_outputs[j];
                }
                nn->output_biases[i] -= LEARNING_RATE * output_delta;
            }
        }
        
        // Print progress
        if (epoch % 100 == 0) {
            printf("Epoch %d, Error: %.6f\n", epoch, total_error / dataset->sample_count);
        }
    }
}

// =============================================================================
// CLUSTERING (K-MEANS)
// =============================================================================

typedef struct {
    double centroids[MAX_FEATURES][10]; // Support up to 10 clusters
    int cluster_count;
    int feature_count;
} KMeans;

// Initialize K-Means with random centroids
void initKMeans(KMeans* kmeans, int cluster_count, int feature_count) {
    kmeans->cluster_count = cluster_count;
    kmeans->feature_count = feature_count;
    
    // Initialize random centroids
    for (int i = 0; i < cluster_count; i++) {
        for (int j = 0; j < feature_count; j++) {
            kmeans->centroids[j][i] = randomDoubleRange(0, 1);
        }
    }
}

// Assign samples to nearest centroid
void assignClusters(KMeans* kmeans, Dataset* dataset, int* assignments) {
    for (int i = 0; i < dataset->sample_count; i++) {
        double min_distance = INFINITY;
        int best_cluster = 0;
        
        for (int cluster = 0; cluster < kmeans->cluster_count; cluster++) {
            double distance = 0.0;
            for (int feature = 0; feature < kmeans->feature_count; feature++) {
                double diff = dataset->samples[i].features[feature] - kmeans->centroids[feature][cluster];
                distance += diff * diff;
            }
            distance = sqrt(distance);
            
            if (distance < min_distance) {
                min_distance = distance;
                best_cluster = cluster;
            }
        }
        
        assignments[i] = best_cluster;
    }
}

// Update centroids
void updateCentroids(KMeans* kmeans, Dataset* dataset, int* assignments) {
    // Reset centroids
    for (int cluster = 0; cluster < kmeans->cluster_count; cluster++) {
        for (int feature = 0; feature < kmeans->feature_count; feature++) {
            kmeans->centroids[feature][cluster] = 0.0;
        }
    }
    
    int cluster_sizes[10] = {0};
    
    // Sum up features for each cluster
    for (int i = 0; i < dataset->sample_count; i++) {
        int cluster = assignments[i];
        cluster_sizes[cluster]++;
        
        for (int feature = 0; feature < kmeans->feature_count; feature++) {
            kmeans->centroids[feature][cluster] += dataset->samples[i].features[feature];
        }
    }
    
    // Calculate averages
    for (int cluster = 0; cluster < kmeans->cluster_count; cluster++) {
        if (cluster_sizes[cluster] > 0) {
            for (int feature = 0; feature < kmeans->feature_count; feature++) {
                kmeans->centroids[feature][cluster] /= cluster_sizes[cluster];
            }
        }
    }
}

// Calculate clustering error
double calculateClusteringError(KMeans* kmeans, Dataset* dataset, int* assignments) {
    double total_error = 0.0;
    
    for (int i = 0; i < dataset->sample_count; i++) {
        int cluster = assignments[i];
        double distance = 0.0;
        
        for (int feature = 0; feature < kmeans->feature_count; feature++) {
            double diff = dataset->samples[i].features[feature] - kmeans->centroids[feature][cluster];
            distance += diff * diff;
        }
        
        total_error += sqrt(distance);
    }
    
    return total_error / dataset->sample_count;
}

// Train K-Means clustering
void trainKMeans(KMeans* kmeans, Dataset* dataset) {
    int assignments[MAX_SAMPLES];
    
    for (int iteration = 0; iteration < 100; iteration++) {
        // Assign samples to clusters
        assignClusters(kmeans, dataset, assignments);
        
        // Update centroids
        updateCentroids(kmeans, dataset, assignments);
        
        // Calculate and print error
        double error = calculateClusteringError(kmeans, dataset, assignments);
        printf("Iteration %d, Error: %.6f\n", iteration, error);
    }
}

// =============================================================================
// DATA GENERATION FOR DEMONSTRATION
// =============================================================================

// Generate linear regression dataset
void generateLinearDataset(Dataset* dataset, int sample_count) {
    dataset->sample_count = sample_count;
    dataset->feature_count = 1;
    dataset->class_count = 1;
    
    for (int i = 0; i < sample_count; i++) {
        double x = randomDoubleRange(0, 10);
        double y = 2.5 * x + 3.0 + randomDoubleRange(-2, 2); // y = 2.5x + 3 + noise
        
        dataset->samples[i].features[0] = x;
        dataset->samples[i].label = (int)y;
    }
}

// Generate classification dataset
void generateClassificationDataset(Dataset* dataset, int sample_count) {
    dataset->sample_count = sample_count;
    dataset->feature_count = 2;
    dataset->class_count = 2;
    
    for (int i = 0; i < sample_count; i++) {
        double x1 = randomDoubleRange(0, 10);
        double x2 = randomDoubleRange(0, 10);
        
        // Simple decision boundary: x1 + x2 > 10
        int label = (x1 + x2 > 10) ? 1 : 0;
        
        dataset->samples[i].features[0] = x1;
        dataset->samples[i].features[1] = x2;
        dataset->samples[i].label = label;
    }
}

// Generate multi-class dataset
void generateMultiClassDataset(Dataset* dataset, int sample_count) {
    dataset->sample_count = sample_count;
    dataset->feature_count = 2;
    dataset->class_count = 3;
    
    for (int i = 0; i < sample_count; i++) {
        double x1 = randomDoubleRange(0, 10);
        double x2 = randomDoubleRange(0, 10);
        
        int label;
        if (x1 < 3.33) {
            label = 0;
        } else if (x1 < 6.66) {
            label = 1;
        } else {
            label = 2;
        }
        
        dataset->samples[i].features[0] = x1;
        dataset->samples[i].features[1] = x2;
        dataset->samples[i].label = label;
    }
}

// =============================================================================
// DEMONSTRATION FUNCTIONS
// =============================================================================

void demonstrateLinearRegression() {
    printf("=== LINEAR REGRESSION DEMO ===\n");
    
    Dataset dataset;
    generateLinearDataset(&dataset, 100);
    
    LinearRegression model;
    initLinearRegression(&model, 1);
    
    printf("Training Linear Regression...\n");
    trainLinearRegression(&model, &dataset);
    
    // Test predictions
    printf("\nTest Predictions:\n");
    for (int i = 0; i < 5; i++) {
        double x = i * 2.0;
        double features[1] = {x};
        double prediction = linearRegressionPredict(&model, features);
        printf("Input: %.1f, Predicted: %.2f\n", x, prediction);
    }
    
    printf("\n");
}

void demonstrateLogisticRegression() {
    printf("=== LOGISTIC REGRESSION DEMO ===\n");
    
    Dataset dataset;
    generateClassificationDataset(&dataset, 200);
    
    LogisticRegression model;
    initLogisticRegression(&model, 2);
    
    printf("Training Logistic Regression...\n");
    trainLogisticRegression(&model, &dataset);
    
    // Test predictions
    printf("\nTest Predictions:\n");
    for (int i = 0; i < 5; i++) {
        double features[2] = {i * 2.0, i * 1.5};
        double probability = logisticRegressionPredict(&model, features);
        int predicted_class = logisticRegressionClassify(&model, features);
        printf("Input: (%.1f, %.1f), Probability: %.3f, Class: %d\n", 
               features[0], features[1], probability, predicted_class);
    }
    
    printf("\n");
}

void demonstrateKNN() {
    printf("=== K-NEAREST NEIGHBORS DEMO ===\n");
    
    Dataset dataset;
    generateClassificationDataset(&dataset, 100);
    
    KNNModel model;
    initKNN(&model, &dataset, 5);
    
    printf("Testing KNN (k=5)...\n");
    for (int i = 0; i < 5; i++) {
        double features[2] = {i * 2.0, i * 1.5};
        int predicted_class = knnPredict(&model, features);
        printf("Input: (%.1f, %.1f), Predicted Class: %d\n", 
               features[0], features[1], predicted_class);
    }
    
    printf("\n");
}

void demonstrateDecisionTree() {
    printf("=== DECISION TREE DEMO ===\n");
    
    Dataset dataset;
    generateMultiClassDataset(&dataset, 150);
    
    printf("Building Decision Tree...\n");
    TreeNode* tree = buildDecisionTree(&dataset, 5, 0);
    
    printf("Testing Decision Tree...\n");
    for (int i = 0; i < 5; i++) {
        double features[2] = {i * 2.0, i * 1.5};
        int predicted_class = decisionTreePredict(tree, features);
        printf("Input: (%.1f, %.1f), Predicted Class: %d\n", 
               features[0], features[1], predicted_class);
    }
    
    printf("\n");
}

void demonstrateNeuralNetwork() {
    printf("=== NEURAL NETWORK DEMO ===\n");
    
    Dataset dataset;
    generateMultiClassDataset(&dataset, 200);
    
    NeuralNetwork nn;
    initNeuralNetwork(&nn, 2);
    
    printf("Training Neural Network...\n");
    trainNeuralNetwork(&nn, &dataset);
    
    printf("Testing Neural Network...\n");
    for (int i = 0; i < 5; i++) {
        double features[2] = {i * 2.0, i * 1.5};
        int predicted_class = neuralNetworkPredict(&nn, features);
        printf("Input: (%.1f, %.1f), Predicted Class: %d\n", 
               features[0], features[1], predicted_class);
    }
    
    printf("\n");
}

void demonstrateKMeans() {
    printf("=== K-MEANS CLUSTERING DEMO ===\n");
    
    Dataset dataset;
    generateClassificationDataset(&dataset, 100);
    
    KMeans kmeans;
    initKMeans(&kmeans, 3, 2);
    
    printf("Training K-Means Clustering...\n");
    trainKMeans(&kmeans, &dataset);
    
    printf("\nFinal Centroids:\n");
    for (int cluster = 0; cluster < 3; cluster++) {
        printf("Cluster %d: (%.3f, %.3f)\n", 
               cluster, kmeans.centroids[0][cluster], kmeans.centroids[1][cluster]);
    }
    
    printf("\n");
}

void demonstrateModelComparison() {
    printf("=== MODEL COMPARISON DEMO ===\n");
    
    Dataset dataset;
    generateClassificationDataset(&dataset, 100);
    
    // Test different models
    LogisticRegression lr_model;
    initLogisticRegression(&lr_model, 2);
    trainLogisticRegression(&lr_model, &dataset);
    
    KNNModel knn_model;
    initKNN(&knn_model, &dataset, 5);
    
    printf("Comparing models on test data...\n");
    int correct_lr = 0, correct_knn = 0;
    
    for (int i = 0; i < 20; i++) {
        double features[2] = {randomDoubleRange(0, 10), randomDoubleRange(0, 10)};
        int true_label = (features[0] + features[1] > 10) ? 1 : 0;
        
        int lr_pred = logisticRegressionClassify(&lr_model, features);
        int knn_pred = knnPredict(&knn_model, features);
        
        if (lr_pred == true_label) correct_lr++;
        if (knn_pred == true_label) correct_knn++;
    }
    
    printf("Logistic Regression Accuracy: %.1f%% (%d/20)\n", 
           (double)correct_lr / 20 * 100, correct_lr);
    printf("KNN Accuracy: %.1f%% (%d/20)\n", 
           (double)correct_knn / 20 * 100, correct_knn);
    
    printf("\n");
}

// =============================================================================
// MAIN FUNCTION
// =============================================================================

int main() {
    printf("Machine Learning Algorithms\n");
    printf("==========================\n\n");
    
    // Seed random number generator
    srand(time(NULL));
    
    // Run demonstrations
    demonstrateLinearRegression();
    demonstrateLogisticRegression();
    demonstrateKNN();
    demonstrateDecisionTree();
    demonstrateNeuralNetwork();
    demonstrateKMeans();
    demonstrateModelComparison();
    
    printf("All machine learning algorithms demonstrated!\n");
    printf("Note: These are simplified implementations for educational purposes.\n");
    printf("For production use, consider established ML libraries like TensorFlow, PyTorch, or scikit-learn.\n");
    
    return 0;
}
