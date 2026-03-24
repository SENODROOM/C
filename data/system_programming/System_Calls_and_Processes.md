# System Calls and Processes

This file contains comprehensive system programming examples in C, including process management, file system operations, signal handling, memory mapping, and advanced process control.

## 📚 System Programming Overview

### 🔧 System Calls
- **Process Control**: fork(), exec(), wait(), kill()
- **File Operations**: open(), read(), write(), close()
- **Directory Operations**: opendir(), readdir(), mkdir()
- **Signal Handling**: signal(), sigaction(), kill()
- **Memory Management**: mmap(), munmap(), brk()

### 🖥️ Process Concepts
- **Process ID (PID)**: Unique process identifier
- **Parent Process**: Process that created this process
- **Process Group**: Group of related processes
- **Session**: Set of process groups

## 🔄 Process Management

### Fork and Exec Pattern
```c
#include <unistd.h>
#include <sys/wait.h>

pid_t pid = fork();

if (pid == 0) {
    // Child process
    char* args[] = {"ls", "-la", NULL};
    execvp("ls", args);
    perror("execvp failed");
    exit(EXIT_FAILURE);
    
} else if (pid > 0) {
    // Parent process
    int status;
    waitpid(pid, &status, 0);
    
    if (WIFEXITED(status)) {
        printf("Child exited with status: %d\n", WEXITSTATUS(status));
    }
}
```

### Process Creation Functions
```c
// Fork - Create new process
pid_t fork(void);

// Exec family - Replace process image
int execl(const char *path, const char *arg, ...);
int execv(const char *path, char *const argv[]);
int execle(const char *path, const char *arg, ...);
int execve(const char *path, char *const argv[], char *const envp[]);
int execlp(const char *file, const char *arg, ...);
int execvp(const char *file, char *const argv[]);
int execvpe(const char *file, char *const argv[], char *const envp[]);

// Wait for process termination
pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options);
```

### Process Status Macros
```c
if (WIFEXITED(status)) {
    int exit_code = WEXITSTATUS(status);
}

if (WIFSIGNALED(status)) {
    int signal = WTERMSIG(status);
}

if (WIFSTOPPED(status)) {
    int signal = WSTOPSIG(status);
}
```

## 📁 File System Operations

### File Operations
```c
#include <fcntl.h>
#include <sys/stat.h>

// Open file
int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);

// Write to file
ssize_t bytes_written = write(fd, buffer, size);

// Read from file
ssize_t bytes_read = read(fd, buffer, size);

// Close file
close(fd);

// Get file information
struct stat file_stat;
stat(filename, &file_stat);

// Remove file
unlink(filename);
```

### File Open Flags
```c
O_RDONLY    // Read only
O_WRONLY    // Write only
O_RDWR      // Read and write
O_CREAT     // Create if doesn't exist
O_EXCL      // Exclusive create
O_TRUNC     // Truncate to zero length
O_APPEND    // Append to file
O_NONBLOCK  // Non-blocking mode
```

### File Permissions
```c
// Permission bits
S_IRUSR  // Owner read
S_IWUSR  // Owner write
S_IXUSR  // Owner execute
S_IRGRP  // Group read
S_IWGRP  // Group write
S_IXGRP  // Group execute
S_IROTH  // Others read
S_IWOTH  // Others write
S_IXOTH  // Others execute

// Common combinations
0644  // rw-r--r--
0755  // rwxr-xr-x
0600  // rw-------
0777  // rwxrwxrwx
```

### Directory Operations
```c
#include <dirent.h>

// Create directory
mkdir(dirname, 0755);

// Remove directory
rmdir(dirname);

// Open directory
DIR *dir = opendir(dirname);

// Read directory entry
struct dirent *entry = readdir(dir);

// Close directory
closedir(dir);

// Change directory
chdir(dirname);

// Get current directory
char cwd[PATH_MAX];
getcwd(cwd, sizeof(cwd));
```

## 📡 Inter-Process Communication

### Pipes
```c
#include <unistd.h>

int pipefd[2];

// Create pipe
if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
}

pid_t pid = fork();

if (pid == 0) {
    // Child - write to pipe
    close(pipefd[0]); // Close read end
    write(pipefd[1], message, strlen(message));
    close(pipefd[1]);
    
} else {
    // Parent - read from pipe
    close(pipefd[1]); // Close write end
    read(pipefd[0], buffer, sizeof(buffer));
    close(pipefd[0]);
}
```

### Named Pipes (FIFO)
```c
#include <sys/stat.h>
#include <fcntl.h>

// Create named pipe
mkfifo(fifoname, 0666);

// Open named pipe
int fd = open(fifoname, O_RDONLY);

// Read/write operations
read(fd, buffer, size);
write(fd, buffer, size);

// Remove named pipe
unlink(fifoname);
```

## ⚡ Signal Handling

### Basic Signal Handling
```c
#include <signal.h>

void signalHandler(int signum) {
    printf("Received signal: %d\n", signum);
}

// Install signal handler
signal(SIGINT, signalHandler);
signal(SIGTERM, signalHandler);
```

### Advanced Signal Handling
```c
#include <signal.h>

struct sigaction sa;

sa.sa_handler = signalHandler;
sigemptyset(&sa.sa_mask);
sa.sa_flags = SA_RESTART;

sigaction(SIGINT, &sa, NULL);
```

### Signal Types
```c
SIGINT   // Interrupt (Ctrl+C)
SIGTERM  // Termination
SIGKILL  // Kill (cannot be caught)
SIGSTOP  // Stop (cannot be caught)
SIGCONT  // Continue
SIGSEGV  // Segmentation fault
SIGFPE   // Floating point exception
SIGILL   // Illegal instruction
SIGABRT  // Abort
SIGALRM  // Alarm clock
SIGCHLD  // Child status changed
```

### Sending Signals
```c
// Send signal to process
kill(pid, SIGTERM);

// Send signal to process group
killpg(pgid, SIGTERM);

// Send signal to all processes
kill(-1, SIGTERM);

// Send signal to self
raise(SIGINT);
```

## 🗺️ Memory Mapping

### Memory Mapping Operations
```c
#include <sys/mman.h>

// Map file into memory
void* mapped = mmap(NULL, size, PROT_READ | PROT_WRITE, 
                    MAP_SHARED, fd, 0);

if (mapped == MAP_FAILED) {
    perror("mmap failed");
}

// Sync changes to file
msync(mapped, size, MS_SYNC);

// Unmap memory
munmap(mapped, size);
```

### mmap Flags
```c
MAP_SHARED     // Changes are shared
MAP_PRIVATE    // Changes are private
MAP_FIXED     // Fixed address
MAP_ANONYMOUS // Anonymous mapping
```

### Memory Protection
```c
// Change memory protection
mprotect(addr, size, PROT_READ | PROT_WRITE);

// Protection flags
PROT_NONE     // No access
PROT_READ     // Read access
PROT_WRITE    // Write access
PROT_EXEC     // Execute access
```

## 🔍 Process Information

### Getting Process Information
```c
#include <unistd.h>
#include <sys/types.h>

// Process IDs
pid_t pid = getpid();           // Current process ID
pid_t ppid = getppid();         // Parent process ID
pid_t pgid = getpgrp();         // Process group ID
pid_t sid = getsid(pid);       // Session ID

// User information
uid_t uid = getuid();           // Real user ID
uid_t euid = geteuid();         // Effective user ID
gid_t gid = getgid();           // Real group ID
gid_t egid = getegid();         // Effective group ID

// Working directory
char cwd[PATH_MAX];
getcwd(cwd, sizeof(cwd));
```

### System Information
```c
#include <unistd.h>

// System limits
long processors = sysconf(_SC_NPROCESSORS_ONLN);
long page_size = sysconf(_SC_PAGESIZE);
long clock_ticks = sysconf(_SC_CLK_TCK);

// Host information
char hostname[256];
gethostname(hostname, sizeof(hostname));
```

## 🌍 Environment Variables

### Environment Operations
```c
#include <stdlib.h>

// Get environment variable
char* value = getenv("PATH");

// Set environment variable
setenv("VAR", "value", 1); // 1 = overwrite

// Unset environment variable
unsetenv("VAR");

// Clear all environment variables
clearenv();

// Access environment directly
extern char** environ;
for (int i = 0; environ[i] != NULL; i++) {
    printf("%s\n", environ[i]);
}
```

## ⏱️ Time and Timing

### Time Functions
```c
#include <time.h>

// Current time
time_t current_time = time(NULL);
printf("Time: %s", ctime(&current_time));

// High-resolution time
struct timespec ts;
clock_gettime(CLOCK_MONOTONIC, &ts);

// CPU time
clock_t start = clock();
// ... do work ...
clock_t end = clock();
double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;

// Sleep functions
sleep(1);           // Seconds
usleep(100000);     // Microseconds
nanosleep(&ts, NULL); // Nanoseconds
```

### Time Formats
```c
// Convert time_t to string
char* time_str = ctime(&time);

// Convert to local time
struct tm* local = localtime(&time);

// Convert to UTC time
struct tm* utc = gmtime(&time);

// Format time
char buffer[80];
strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local);
```

## 🔧 Advanced Process Control

### Process Priority
```c
#include <sys/resource.h>

// Set process priority
int nice(int increment);  // Range: -20 to 19
setpriority(PRIO_PROCESS, 0, 5);

// Get process priority
int priority = getpriority(PRIO_PROCESS, 0);

// Resource limits
struct rlimit rl;
getrlimit(RLIMIT_NOFILE, &rl);
rl.rlim_cur = 1024;
setrlimit(RLIMIT_NOFILE, &rl);
```

### Process Groups and Sessions
```c
// Create new session
pid_t sid = setsid();

// Set process group
setpgid(0, 0);  // Make this process its own group leader

// Get process group
pid_t pgid = getpgrp();
```

### Daemon Process
```c
void createDaemon() {
    // Fork and exit parent
    pid_t pid = fork();
    if (pid > 0) exit(EXIT_SUCCESS);
    
    // Create new session
    if (setsid() < 0) exit(EXIT_FAILURE);
    
    // Change working directory
    chdir("/");
    
    // Close standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    // Redirect to /dev/null
    open("/dev/null", O_RDONLY);
    open("/dev/null", O_WRONLY);
    open("/dev/null", O_WRONLY);
}
```

## 💡 Advanced Topics

### 1. File Descriptors
```c
// Duplicate file descriptor
int dup2(int oldfd, int newfd);

// Pipe with file descriptor
int pipe2(int pipefd[2], int flags);

// File descriptor flags
int flags = fcntl(fd, F_GETFL);
fcntl(fd, F_SETFL, flags | O_NONBLOCK);
```

### 2. Memory Allocation
```c
// Break point manipulation
void* sbrk(intptr_t increment);
int brk(void* addr);

// Anonymous memory mapping
void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, 
                 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
```

### 3. Process Tracing
```c
#include <sys/ptrace.h>

// Trace process
ptrace(PTRACE_TRACEME, 0, NULL, NULL);

// Wait for traced process
wait(&status);

// Continue traced process
ptrace(PTRACE_CONT, pid, NULL, NULL);
```

### 4. System V IPC
```c
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>

// Shared memory
int shmid = shmget(key, size, IPC_CREAT | 0666);
void* shmaddr = shmat(shmid, NULL, 0);

// Semaphores
int semid = semget(key, 1, IPC_CREAT | 0666);
semop(semid, &operation, 1);

// Message queues
int msgid = msgget(key, IPC_CREAT | 0666);
msgsnd(msgid, &message, size, 0);
```

## 📊 Error Handling

### System Call Error Handling
```c
#include <errno.h>
#include <string.h>

// Check return values
if (result == -1) {
    perror("system_call failed");
    // or
    fprintf(stderr, "system_call failed: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
}

// Save errno before other calls
int saved_errno = errno;
// ... other calls that might change errno ...
printf("Error: %s\n", strerror(saved_errno));
```

### Common Error Codes
```c
EACCES  // Permission denied
ENOENT  // No such file or directory
EEXIST  // File exists
ENOMEM  // Not enough memory
EAGAIN  // Resource temporarily unavailable
EBUSY   // Device or resource busy
EINVAL  // Invalid argument
EIO     // I/O error
```

## ⚠️ Common Pitfalls

### 1. Race Conditions
```c
// Wrong - Race condition between fork and exec
pid_t pid = fork();
if (pid == 0) {
    // Child
    execvp(command, args); // Might fail if parent modifies args
}

// Right - Use exec immediately after fork
pid_t pid = fork();
if (pid == 0) {
    char* child_args[] = {"ls", "-la", NULL};
    execvp("ls", child_args);
}
```

### 2. Zombie Processes
```c
// Wrong - Not waiting for child
fork(); // Child becomes zombie if parent doesn't wait

// Right - Always wait for children
pid_t pid = fork();
if (pid > 0) {
    wait(NULL); // Wait for child
}
```

### 3. Signal Safety
```c
// Wrong - Using non-signal-safe functions in signal handler
void handler(int sig) {
    printf("Signal received\n"); // printf is not signal-safe
}

// Right - Use only signal-safe functions
volatile sig_atomic_t flag = 0;
void handler(int sig) {
    flag = 1; // Only set flag
}
```

### 4. Memory Leaks
```c
// Wrong - Forgetting to close file descriptors
int fd = open(filename, O_RDONLY);
// Use fd but forget to close

// Right - Always close resources
int fd = open(filename, O_RDONLY);
// Use fd...
close(fd);
```

## 🔧 Real-World Applications

### 1. Shell Implementation
```c
void executeCommand(char** args) {
    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        perror("exec failed");
        exit(EXIT_FAILURE);
    } else {
        wait(NULL);
    }
}
```

### 2. File System Monitor
```c
void monitorDirectory(const char* dirname) {
    while (1) {
        DIR* dir = opendir(dirname);
        struct dirent* entry;
        
        while ((entry = readdir(dir)) != NULL) {
            // Process file changes
        }
        
        closedir(dir);
        sleep(1);
    }
}
```

### 3. Process Manager
```c
void listProcesses() {
    DIR* proc = opendir("/proc");
    struct dirent* entry;
    
    while ((entry = readdir(proc)) != NULL) {
        if (isdigit(entry->d_name[0])) {
            pid_t pid = atoi(entry->d_name);
            // Read process information from /proc/[pid]/...
        }
    }
    
    closedir(proc);
}
```

### 4. System Service
```c
void createService() {
    // Fork to background
    if (fork() > 0) {
        exit(EXIT_SUCCESS); // Parent exits
    }
    
    // Create new session
    setsid();
    
    // Daemonize
    chdir("/");
    umask(0);
    
    // Main service loop
    while (1) {
        // Handle service requests
        sleep(1);
    }
}
```

## 🎓 Best Practices

### 1. Error Handling
```c
// Always check system call return values
if (system_call() == -1) {
    perror("system_call failed");
    exit(EXIT_FAILURE);
}
```

### 2. Resource Management
```c
// Always close file descriptors
// Always wait for child processes
// Always free allocated memory
// Always clean up temporary files
```

### 3. Signal Safety
```c
// Use only async-signal-safe functions in signal handlers
// Use volatile sig_atomic_t for shared variables
// Avoid complex operations in signal handlers
```

### 4. Security
```c
// Validate all input
// Use least privilege principle
// Check return values of security-sensitive functions
// Avoid buffer overflows
```

### 5. Portability
```c
// Use standard POSIX functions
// Avoid platform-specific code when possible
// Use feature test macros
// Document platform dependencies
```

System programming in C provides direct access to operating system services and enables creation of powerful system utilities and applications. Master these concepts to build robust system-level software!
