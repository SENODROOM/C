# Machine Learning Algorithms

This file contains comprehensive machine learning algorithm implementations in C, including linear regression, logistic regression, K-nearest neighbors, decision trees, neural networks, and K-means clustering.

## 📚 Machine Learning Overview

### 🤖 ML Categories
- **Supervised Learning**: Learning from labeled data
- **Unsupervised Learning**: Finding patterns in unlabeled data
- **Reinforcement Learning**: Learning through rewards and penalties
- **Deep Learning**: Neural networks with multiple layers

### 🎯 Core Concepts
- **Features**: Input variables for prediction
- **Labels**: Target values for supervised learning
- **Training**: Process of learning from data
- **Prediction**: Using trained model on new data
- **Evaluation**: Measuring model performance

## 📊 Data Structures

### Data Sample Structure
```c
typedef struct {
    double features[MAX_FEATURES];
    int label;
    int feature_count;
} DataSample;
```

### Dataset Structure
```c
typedef struct {
    DataSample samples[MAX_SAMPLES];
    int sample_count;
    int feature_count;
    int class_count;
} Dataset;
```

### Model Structure
```c
typedef struct {
    double weights[MAX_FEATURES];
    double bias;
    int feature_count;
} Model;
```

## 📈 Linear Regression

### Linear Regression Model
```c
typedef struct {
    double weights[MAX_FEATURES];
    double bias;
    int feature_count;
} LinearRegression;
```

### Model Initialization
```c
void initLinearRegression(LinearRegression* model, int feature_count) {
    model->feature_count = feature_count;
    model->bias = 0.0;
    
    // Initialize weights with small random values
    for (int i = 0; i < feature_count; i++) {
        model->weights[i] = randomDoubleRange(-0.1, 0.1);
    }
}
```

### Prediction Function
```c
double linearRegressionPredict(LinearRegression* model, double* features) {
    double prediction = model->bias;
    
    for (int i = 0; i < model->feature_count; i++) {
        prediction += model->weights[i] * features[i];
    }
    
    return prediction;
}
```

### Training with Gradient Descent
```c
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
```

**Linear Regression Characteristics**:
- **Type**: Supervised learning
- **Output**: Continuous values
- **Loss Function**: Mean Squared Error
- **Use Case**: Predicting numeric values

## 🎯 Logistic Regression

### Logistic Regression Model
```c
typedef struct {
    double weights[MAX_FEATURES];
    double bias;
    int feature_count;
} LogisticRegression;
```

### Sigmoid Activation Function
```c
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}
```

### Probability Prediction
```c
double logisticRegressionPredict(LogisticRegression* model, double* features) {
    double z = model->bias;
    
    for (int i = 0; i < model->feature_count; i++) {
        z += model->weights[i] * features[i];
    }
    
    return sigmoid(z);
}
```

### Classification
```c
int logisticRegressionClassify(LogisticRegression* model, double* features) {
    double probability = logisticRegressionPredict(model, features);
    return probability >= 0.5 ? 1 : 0;
}
```

### Training with Cross-Entropy Loss
```c
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
```

**Logistic Regression Characteristics**:
- **Type**: Supervised learning
- **Output**: Binary classification
- **Loss Function**: Cross-entropy
- **Use Case**: Binary classification problems

## 🎯 K-Nearest Neighbors (KNN)

### KNN Model Structure
```c
typedef struct {
    Dataset* dataset;
    int k;
} KNNModel;
```

### Euclidean Distance
```c
double euclideanDistance(double* features1, double* features2, int feature_count) {
    double distance = 0.0;
    
    for (int i = 0; i < feature_count; i++) {
        double diff = features1[i] - features2[i];
        distance += diff * diff;
    }
    
    return sqrt(distance);
}
```

### KNN Prediction
```c
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
```

**KNN Characteristics**:
- **Type**: Supervised learning
- **Algorithm**: Instance-based learning
- **Distance Metric**: Euclidean distance
- **Use Case**: Classification and regression

## 🌳 Decision Trees

### Decision Tree Node Structure
```c
typedef struct TreeNode {
    int feature_index;
    double threshold;
    int class_label;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;
```

### Gini Impurity Calculation
```c
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
```

### Best Split Finding
```c
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
```

### Tree Building
```c
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
    
    // Split dataset and build subtrees recursively
    // ... (implementation continues)
    
    return node;
}
```

**Decision Tree Characteristics**:
- **Type**: Supervised learning
- **Algorithm**: Tree-based learning
- **Splitting Criterion**: Gini impurity
- **Use Case**: Classification and regression

## 🧠 Neural Networks

### Neural Network Structure
```c
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
```

### Network Initialization
```c
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
```

### Forward Pass
```c
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
```

### Training with Backpropagation
```c
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
```

**Neural Network Characteristics**:
- **Type**: Supervised learning
- **Architecture**: Multi-layer perceptron
- **Activation**: Sigmoid function
- **Use Case**: Complex classification and regression

## 🎯 K-Means Clustering

### K-Means Structure
```c
typedef struct {
    double centroids[MAX_FEATURES][10]; // Support up to 10 clusters
    int cluster_count;
    int feature_count;
} KMeans;
```

### Cluster Assignment
```c
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
```

### Centroid Update
```c
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
```

### Training Algorithm
```c
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
```

**K-Means Characteristics**:
- **Type**: Unsupervised learning
- **Algorithm**: Iterative clustering
- **Distance Metric**: Euclidean distance
- **Use Case**: Data clustering and segmentation

## 💡 Activation Functions

### Sigmoid Function
```c
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

double sigmoidDerivative(double x) {
    double s = sigmoid(x);
    return s * (1.0 - s);
}
```

### ReLU Function
```c
double relu(double x) {
    return x > 0 ? x : 0;
}

double reluDerivative(double x) {
    return x > 0 ? 1 : 0;
}
```

### Tanh Function
```c
double tanhActivation(double x) {
    return tanh(x);
}

double tanhDerivative(double x) {
    double t = tanh(x);
    return 1.0 - t * t;
}
```

## 📊 Loss Functions

### Mean Squared Error
```c
double meanSquaredError(double* predicted, double* actual, int size) {
    double error = 0.0;
    for (int i = 0; i < size; i++) {
        double diff = predicted[i] - actual[i];
        error += diff * diff;
    }
    return error / size;
}
```

### Cross-Entropy Loss
```c
double crossEntropyLoss(double predicted, int actual) {
    return -log(actual == 1 ? predicted : (1.0 - predicted));
}
```

### Hinge Loss
```c
double hingeLoss(double predicted, int actual) {
    return fmax(0, 1.0 - actual * predicted);
}
```

## 🎯 Optimization Algorithms

### Gradient Descent
```c
void gradientDescent(double* weights, double* gradients, int size, double learning_rate) {
    for (int i = 0; i < size; i++) {
        weights[i] -= learning_rate * gradients[i];
    }
}
```

### Stochastic Gradient Descent
```c
void stochasticGradientDescent(Model* model, Dataset* dataset, double learning_rate) {
    // Process one random sample at a time
    int sample_index = rand() % dataset->sample_count;
    
    // Calculate gradient for single sample
    // Update weights immediately
    // ...
}
```

### Mini-Batch Gradient Descent
```c
void miniBatchGradientDescent(Model* model, Dataset* dataset, int batch_size, double learning_rate) {
    for (int batch_start = 0; batch_start < dataset->sample_count; batch_start += batch_size) {
        int batch_end = fmin(batch_start + batch_size, dataset->sample_count);
        
        // Calculate gradients for mini-batch
        // Update weights
        // ...
    }
}
```

## 🔍 Evaluation Metrics

### Accuracy
```c
double calculateAccuracy(int* predictions, int* actual, int size) {
    int correct = 0;
    for (int i = 0; i < size; i++) {
        if (predictions[i] == actual[i]) {
            correct++;
        }
    }
    return (double)correct / size;
}
```

### Precision and Recall
```c
void calculatePrecisionRecall(int* predictions, int* actual, int size, double* precision, double* recall) {
    int true_positive = 0, false_positive = 0, false_negative = 0;
    
    for (int i = 0; i < size; i++) {
        if (predictions[i] == 1 && actual[i] == 1) true_positive++;
        if (predictions[i] == 1 && actual[i] == 0) false_positive++;
        if (predictions[i] == 0 && actual[i] == 1) false_negative++;
    }
    
    *precision = (double)true_positive / (true_positive + false_positive);
    *recall = (double)true_positive / (true_positive + false_negative);
}
```

### F1 Score
```c
double calculateF1Score(double precision, double recall) {
    return 2.0 * precision * recall / (precision + recall);
}
```

## 🧪 Data Preprocessing

### Feature Scaling
```c
void featureScaling(Dataset* dataset) {
    // Find min and max for each feature
    double min_values[MAX_FEATURES];
    double max_values[MAX_FEATURES];
    
    for (int feature = 0; feature < dataset->feature_count; feature++) {
        min_values[feature] = dataset->samples[0].features[feature];
        max_values[feature] = dataset->samples[0].features[feature];
        
        for (int i = 1; i < dataset->sample_count; i++) {
            if (dataset->samples[i].features[feature] < min_values[feature]) {
                min_values[feature] = dataset->samples[i].features[feature];
            }
            if (dataset->samples[i].features[feature] > max_values[feature]) {
                max_values[feature] = dataset->samples[i].features[feature];
            }
        }
    }
    
    // Scale features to [0, 1]
    for (int i = 0; i < dataset->sample_count; i++) {
        for (int feature = 0; feature < dataset->feature_count; feature++) {
            double range = max_values[feature] - min_values[feature];
            if (range > 0) {
                dataset->samples[i].features[feature] = 
                    (dataset->samples[i].features[feature] - min_values[feature]) / range;
            }
        }
    }
}
```

### Data Shuffling
```c
void shuffleDataset(Dataset* dataset) {
    for (int i = 0; i < dataset->sample_count - 1; i++) {
        int j = i + rand() % (dataset->sample_count - i);
        
        DataSample temp = dataset->samples[i];
        dataset->samples[i] = dataset->samples[j];
        dataset->samples[j] = temp;
    }
}
```

### Train-Test Split
```c
void trainTestSplit(Dataset* dataset, Dataset* train, Dataset* test, double train_ratio) {
    int train_size = (int)(dataset->sample_count * train_ratio);
    
    // Copy training data
    train->sample_count = train_size;
    train->feature_count = dataset->feature_count;
    train->class_count = dataset->class_count;
    for (int i = 0; i < train_size; i++) {
        train->samples[i] = dataset->samples[i];
    }
    
    // Copy test data
    test->sample_count = dataset->sample_count - train_size;
    test->feature_count = dataset->feature_count;
    test->class_count = dataset->class_count;
    for (int i = 0; i < test->sample_count; i++) {
        test->samples[i] = dataset->samples[train_size + i];
    }
}
```

## 📊 Model Comparison

### Cross-Validation
```c
double crossValidation(Model* model, Dataset* dataset, int folds) {
    double total_accuracy = 0.0;
    int fold_size = dataset->sample_count / folds;
    
    for (int fold = 0; fold < folds; fold++) {
        // Create train and test sets for this fold
        Dataset train, test;
        // ... (implementation)
        
        // Train model
        trainModel(model, &train);
        
        // Test model
        double accuracy = testModel(model, &test);
        total_accuracy += accuracy;
    }
    
    return total_accuracy / folds;
}
```

### Model Selection
```c
void compareModels() {
    LinearRegression lr_model;
    LogisticRegression log_reg_model;
    KNNModel knn_model;
    
    // Train all models
    // ... (implementation)
    
    // Evaluate on test set
    double lr_accuracy = evaluateLinearRegression(&lr_model, &test_set);
    double log_reg_accuracy = evaluateLogisticRegression(&log_reg_model, &test_set);
    double knn_accuracy = evaluateKNN(&knn_model, &test_set);
    
    printf("Linear Regression Accuracy: %.2f%%\n", lr_accuracy * 100);
    printf("Logistic Regression Accuracy: %.2f%%\n", log_reg_accuracy * 100);
    printf("KNN Accuracy: %.2f%%\n", knn_accuracy * 100);
}
```

## ⚠️ Common Pitfalls

### 1. Overfitting
```c
// Wrong - Training too long without validation
for (int epoch = 0; epoch < 10000; epoch++) {
    trainModel(model, dataset); // No validation
}

// Right - Use validation set and early stopping
for (int epoch = 0; epoch < 10000; epoch++) {
    trainModel(model, train_set);
    double val_accuracy = evaluateModel(model, val_set);
    if (val_accuracy < best_accuracy) break;
}
```

### 2. Learning Rate Issues
```c
// Wrong - Learning rate too high
double learning_rate = 1.0; // May cause divergence

// Wrong - Learning rate too low
double learning_rate = 0.000001; // Very slow convergence

// Right - Appropriate learning rate
double learning_rate = 0.01; // Good balance
```

### 3. Data Leakage
```c
// Wrong - Preprocessing on entire dataset
featureScaling(full_dataset);
trainTestSplit(full_dataset, &train, &test, 0.8);

// Right - Preprocess only training data
trainTestSplit(full_dataset, &train, &test, 0.8);
featureScaling(&train);
featureScaling(&test); // Use same scaling parameters
```

### 4. Not Shuffling Data
```c
// Wrong - Not shuffling before train-test split
trainTestSplit(dataset, &train, &test, 0.8);
// Data may be ordered, causing bias

// Right - Shuffle before splitting
shuffleDataset(dataset);
trainTestSplit(dataset, &train, &test, 0.8);
```

## 🔧 Real-World Applications

### 1. Predictive Maintenance
```c
void predictMaintenance() {
    // Features: temperature, vibration, hours_since_last_maintenance
    // Target: probability of failure
    
    LinearRegression model;
    trainLinearRegression(&model, &maintenance_data);
    
    double features[3] = {current_temp, current_vibration, hours_since_last};
    double failure_probability = linearRegressionPredict(&model, features);
    
    if (failure_probability > 0.8) {
        printf("High risk of failure detected!\n");
    }
}
```

### 2. Customer Churn Prediction
```c
void predictCustomerChurn() {
    // Features: usage_frequency, support_tickets, subscription_months
    // Target: churn (0/1)
    
    LogisticRegression model;
    trainLogisticRegression(&model, &customer_data);
    
    double features[3] = {customer.usage, customer.tickets, customer.months};
    double churn_probability = logisticRegressionPredict(&model, features);
    
    if (churn_probability > 0.5) {
        printf("Customer at risk of churn\n");
    }
}
```

### 3. Image Classification
```c
void classifyImage(double* image_features) {
    // Features: pixel values, edges, textures
    // Target: class label (0-9)
    
    NeuralNetwork nn;
    trainNeuralNetwork(&nn, &image_dataset);
    
    int predicted_class = neuralNetworkPredict(&nn, image_features);
    printf("Predicted class: %d\n", predicted_class);
}
```

### 4. Market Segmentation
```c
void segmentMarket() {
    // Features: age, income, spending habits
    // Clusters: customer segments
    
    KMeans kmeans;
    initKMeans(&kmeans, 3, 4); // 3 clusters, 4 features
    trainKMeans(&kmeans, &customer_data);
    
    // Assign customers to segments
    int assignments[MAX_CUSTOMERS];
    assignClusters(&kmeans, &customer_data, assignments);
}
```

## 🎓 Best Practices

### 1. Data Quality
```c
// Always validate and clean data
void validateData(Dataset* dataset) {
    for (int i = 0; i < dataset->sample_count; i++) {
        for (int j = 0; j < dataset->feature_count; j++) {
            if (isnan(dataset->samples[i].features[j]) {
                printf("NaN found in sample %d, feature %d\n", i, j);
                dataset->samples[i].features[j] = 0.0;
            }
        }
    }
}
```

### 2. Hyperparameter Tuning
```c
void tuneHyperparameters() {
    // Try different learning rates
    double learning_rates[] = {0.001, 0.01, 0.1};
    
    for (int i = 0; i < 3; i++) {
        printf("Testing learning rate: %.3f\n", learning_rates[i]);
        trainModelWithLearningRate(learning_rates[i]);
        double accuracy = evaluateModel(&test_set);
        printf("Accuracy: %.2f%%\n", accuracy * 100);
    }
}
```

### 3. Model Persistence
```c
void saveModel(Model* model, const char* filename) {
    FILE* file = fopen(filename, "wb");
    fwrite(model, sizeof(Model), 1, file);
    fclose(file);
}

void loadModel(Model* model, const char* filename) {
    FILE* file = fopen(filename, "rb");
    fread(model, sizeof(Model), 1, file);
    fclose(file);
}
```

### 4. Ensemble Methods
```c
int ensemblePredict(Model* models, int model_count, double* features) {
    int votes[10] = {0};
    
    for (int i = 0; i < model_count; i++) {
        int prediction = predictModel(&models[i], features);
        votes[prediction]++;
    }
    
    // Return majority vote
    int max_votes = 0, best_class = 0;
    for (int i = 0; i < 10; i++) {
        if (votes[i] > max_votes) {
            max_votes = votes[i];
            best_class = i;
        }
    }
    
    return best_class;
}
```

### 5. Regularization
```c
void addL2Regularization(Model* model, double lambda) {
    for (int i = 0; i < model->feature_count; i++) {
        model->weights[i] -= lambda * model->weights[i];
    }
}
```

## ⚠️ Security Considerations

### 1. Input Validation
```c
int validateFeatures(double* features, int feature_count) {
    for (int i = 0; i < feature_count; i++) {
        if (isnan(features[i]) || isinf(features[i])) {
            return 0; // Invalid input
        }
    }
    return 1; // Valid input
}
```

### 2. Model Security
```c
void secureModel(Model* model) {
    // Clear sensitive data
    memset(model->weights, 0, sizeof(model->weights));
    model->bias = 0;
    
    // Use secure memory allocation if available
    // ...
}
```

### 3. Output Validation
```c
int validatePrediction(double prediction, double min_val, double max_val) {
    return prediction >= min_val && prediction <= max_val;
}
```

Machine learning in C provides fundamental understanding of algorithms and data structures. While C may not be the first choice for production ML systems, implementing these algorithms helps understand the underlying mathematics and computer science principles. For production use, consider established libraries like TensorFlow, PyTorch, or scikit-learn!
