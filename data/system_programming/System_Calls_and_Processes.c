#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>
#include <signal.h>
#include <errno.h>

// =============================================================================
// PROCESS MANAGEMENT
// =============================================================================

// Fork and exec example
void demonstrateForkExec() {
    printf("=== FORK AND EXEC DEMO ===\n");
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // Child process
        printf("Child process: PID = %d, Parent PID = %d\n", getpid(), getppid());
        
        // Execute a command
        char* args[] = {"ls", "-la", NULL};
        execvp("ls", args);
        
        // If execvp returns, an error occurred
        perror("execvp failed");
        exit(EXIT_FAILURE);
        
    } else if (pid > 0) {
        // Parent process
        printf("Parent process: PID = %d, Child PID = %d\n", getpid(), pid);
        
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            printf("Child exited with status: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Child killed by signal: %d\n", WTERMSIG(status));
        }
        
    } else {
        // Fork failed
        perror("fork failed");
    }
    
    printf("\n");
}

// Process creation with pipes
void demonstratePipe() {
    printf("=== PIPE DEMO ===\n");
    
    int pipefd[2];
    pid_t pid;
    
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return;
    }
    
    pid = fork();
    
    if (pid == 0) {
        // Child process - write to pipe
        close(pipefd[0]); // Close read end
        
        const char* message = "Hello from child process!";
        write(pipefd[1], message, strlen(message));
        
        close(pipefd[1]);
        exit(EXIT_SUCCESS);
        
    } else if (pid > 0) {
        // Parent process - read from pipe
        close(pipefd[1]); // Close write end
        
        char buffer[256];
        ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);
        
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Parent received: '%s'\n", buffer);
        }
        
        close(pipefd[0]);
        wait(NULL); // Wait for child
    }
    
    printf("\n");
}

// =============================================================================
// FILE SYSTEM OPERATIONS
// =============================================================================

void demonstrateFileOperations() {
    printf("=== FILE SYSTEM OPERATIONS ===\n");
    
    const char* filename = "test_file.txt";
    
    // Create and write to file
    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open failed");
        return;
    }
    
    const char* content = "This is a test file for system programming demo.\n";
    write(fd, content, strlen(content));
    close(fd);
    
    printf("Created file: %s\n", filename);
    
    // Read file information
    struct stat file_stat;
    if (stat(filename, &file_stat) == -1) {
        perror("stat failed");
        return;
    }
    
    printf("File size: %ld bytes\n", file_stat.st_size);
    printf("File permissions: %o\n", file_stat.st_mode & 0777);
    printf("Last modified: %s", ctime(&file_stat.st_mtime));
    
    // Read file content
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open failed");
        return;
    }
    
    char read_buffer[256];
    ssize_t bytes_read = read(fd, read_buffer, sizeof(read_buffer) - 1);
    if (bytes_read > 0) {
        read_buffer[bytes_read] = '\0';
        printf("File content: %s", read_buffer);
    }
    
    close(fd);
    
    // Clean up
    unlink(filename);
    printf("Deleted file: %s\n", filename);
    
    printf("\n");
}

// Directory operations
void demonstrateDirectoryOperations() {
    printf("=== DIRECTORY OPERATIONS ===\n");
    
    const char* dirname = "test_dir";
    
    // Create directory
    if (mkdir(dirname, 0755) == -1) {
        perror("mkdir failed");
        return;
    }
    
    printf("Created directory: %s\n", dirname);
    
    // Change directory
    if (chdir(dirname) == -1) {
        perror("chdir failed");
        return;
    }
    
    char current_dir[256];
    if (getcwd(current_dir, sizeof(current_dir)) != NULL) {
        printf("Current directory: %s\n", current_dir);
    }
    
    // Create some files in the directory
    for (int i = 0; i < 3; i++) {
        char filename[50];
        snprintf(filename, sizeof(filename), "file_%d.txt", i);
        
        int fd = open(filename, O_CREAT | O_WRONLY, 0644);
        if (fd != -1) {
            const char* content = "Test content\n";
            write(fd, content, strlen(content));
            close(fd);
        }
    }
    
    // List directory contents
    DIR *dir;
    struct dirent *entry;
    
    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir failed");
        return;
    }
    
    printf("Directory contents:\n");
    while ((entry = readdir(dir)) != NULL) {
        printf("  %s", entry->d_name);
        
        if (entry->d_type == DT_DIR) {
            printf(" (directory)");
        } else if (entry->d_type == DT_REG) {
            printf(" (regular file)");
        }
        printf("\n");
    }
    
    closedir(dir);
    
    // Clean up - go back and remove directory
    chdir("..");
    
    // Remove files
    for (int i = 0; i < 3; i++) {
        char filename[50];
        snprintf(filename, sizeof(filename), "%s/file_%d.txt", dirname, i);
        unlink(filename);
    }
    
    rmdir(dirname);
    printf("Removed directory: %s\n", dirname);
    
    printf("\n");
}

// =============================================================================
// SIGNAL HANDLING
// =============================================================================

volatile sig_atomic_t signal_received = 0;

void signalHandler(int signum) {
    signal_received = signum;
    printf("Received signal: %d\n", signum);
}

void demonstrateSignals() {
    printf("=== SIGNAL HANDLING ===\n");
    
    // Set up signal handler
    struct sigaction sa;
    sa.sa_handler = signalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction failed");
        return;
    }
    
    printf("Signal handler installed for SIGINT (Ctrl+C)\n");
    printf("Try pressing Ctrl+C within 5 seconds...\n");
    
    // Wait for signal or timeout
    for (int i = 0; i < 5 && signal_received == 0; i++) {
        printf("Waiting... %d\n", i + 1);
        sleep(1);
    }
    
    if (signal_received) {
        printf("Signal %d was received!\n", signal_received);
    } else {
        printf("No signal received within timeout\n");
    }
    
    // Restore default signal handler
    signal(SIGINT, SIG_DFL);
    
    printf("\n");
}

// =============================================================================
// MEMORY MAPPING
// =============================================================================

void demonstrateMemoryMapping() {
    printf("=== MEMORY MAPPING ===\n");
    
    const char* filename = "mmap_test.txt";
    const char* content = "This is a test file for memory mapping demonstration.";
    
    // Create and write to file
    int fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open failed");
        return;
    }
    
    write(fd, content, strlen(content));
    
    // Get file size
    struct stat file_stat;
    fstat(fd, &file_stat);
    off_t file_size = file_stat.st_size;
    
    // Map file into memory
    char* mapped = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("mmap failed");
        close(fd);
        return;
    }
    
    printf("File mapped to memory at address %p\n", (void*)mapped);
    printf("Original content: %s\n", mapped);
    
    // Modify mapped memory
    strcpy(mapped, "Modified content in memory mapped file.");
    
    // Sync changes to file
    msync(mapped, file_size, MS_SYNC);
    
    // Unmap
    munmap(mapped, file_size);
    close(fd);
    
    // Read back to verify changes
    fd = open(filename, O_RDONLY);
    if (fd != -1) {
        char buffer[256];
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Modified file content: %s\n", buffer);
        }
        close(fd);
    }
    
    // Clean up
    unlink(filename);
    
    printf("\n");
}

// =============================================================================
// PROCESS INFORMATION
// =============================================================================

void demonstrateProcessInfo() {
    printf("=== PROCESS INFORMATION ===\n");
    
    printf("Current Process Information:\n");
    printf("PID: %d\n", getpid());
    printf("Parent PID: %d\n", getppid());
    printf("Process Group ID: %d\n", getpgrp());
    printf("Session ID: %d\n", getsid(getpid()));
    
    // User information
    printf("Real User ID: %d\n", getuid());
    printf("Effective User ID: %d\n", geteuid());
    printf("Real Group ID: %d\n", getgid());
    printf("Effective Group ID: %d\n", getegid());
    
    // Working directory
    char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Working Directory: %s\n", cwd);
    }
    
    // System information
    printf("\nSystem Information:\n");
    printf("Number of processors: %ld\n", sysconf(_SC_NPROCESSORS_ONLN));
    printf("Page size: %ld bytes\n", sysconf(_SC_PAGESIZE));
    printf("Clock ticks per second: %ld\n", sysconf(_SC_CLK_TCK));
    
    printf("\n");
}

// =============================================================================
// ENVIRONMENT VARIABLES
// =============================================================================

void demonstrateEnvironment() {
    printf("=== ENVIRONMENT VARIABLES ===\n");
    
    // Print all environment variables
    extern char** environ;
    printf("Environment Variables:\n");
    for (int i = 0; environ[i] != NULL; i++) {
        printf("  %s\n", environ[i]);
    }
    
    // Get specific environment variable
    const char* path = getenv("PATH");
    if (path) {
        printf("\nPATH = %s\n", path);
    }
    
    // Set environment variable
    if (setenv("TEST_VAR", "test_value", 1) == 0) {
        printf("Set TEST_VAR = %s\n", getenv("TEST_VAR"));
    }
    
    // Unset environment variable
    if (unsetenv("TEST_VAR") == 0) {
        printf("Unset TEST_VAR\n");
        printf("TEST_VAR = %s\n", getenv("TEST_VAR"));
    }
    
    printf("\n");
}

// =============================================================================
// TIME AND TIMING
// =============================================================================

void demonstrateTiming() {
    printf("=== TIME AND TIMING ===\n");
    
    // Current time
    time_t current_time = time(NULL);
    printf("Current time: %s", ctime(&current_time));
    
    // High-resolution timing
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    // Simulate some work
    usleep(100000); // 100ms
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double elapsed = (end.tv_sec - start.tv_sec) + 
                    (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("Elapsed time: %.6f seconds\n", elapsed);
    
    // CPU time
    clock_t cpu_start = clock();
    
    // Simulate CPU work
    volatile int sum = 0;
    for (int i = 0; i < 1000000; i++) {
        sum += i;
    }
    
    clock_t cpu_end = clock();
    double cpu_time = ((double)(cpu_end - cpu_start)) / CLOCKS_PER_SEC;
    printf("CPU time: %.6f seconds\n", cpu_time);
    printf("Sum result: %d\n", sum);
    
    printf("\n");
}

// =============================================================================
// ADVANCED PROCESS CONTROL
// =============================================================================

void demonstrateProcessControl() {
    printf("=== ADVANCED PROCESS CONTROL ===\n");
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // Child process
        printf("Child: PID = %d\n", getpid());
        
        // Set process priority
        if (setpriority(PRIO_PROCESS, 0, 5) == 0) {
            printf("Child: Set priority to 5\n");
        }
        
        // Sleep for 2 seconds
        printf("Child: Sleeping for 2 seconds...\n");
        sleep(2);
        
        printf("Child: Exiting\n");
        exit(42); // Custom exit code
        
    } else if (pid > 0) {
        // Parent process
        printf("Parent: Child PID = %d\n", pid);
        
        // Get child process status
        int status;
        pid_t terminated_pid = waitpid(pid, &status, WNOHANG);
        
        if (terminated_pid == 0) {
            printf("Parent: Child is still running\n");
        }
        
        // Wait for child to complete
        terminated_pid = waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            printf("Parent: Child exited with status %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Parent: Child killed by signal %d\n", WTERMSIG(status));
        }
        
    } else {
        perror("fork failed");
    }
    
    printf("\n");
}

// =============================================================================
// MAIN DEMONSTRATION
// =============================================================================

int main() {
    printf("System Programming Examples\n");
    printf("===========================\n\n");
    
    demonstrateProcessInfo();
    demonstrateForkExec();
    demonstratePipe();
    demonstrateFileOperations();
    demonstrateDirectoryOperations();
    demonstrateSignals();
    demonstrateMemoryMapping();
    demonstrateEnvironment();
    demonstrateTiming();
    demonstrateProcessControl();
    
    printf("All system programming examples demonstrated!\n");
    return 0;
}
