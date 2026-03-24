# Socket Programming

This file contains comprehensive socket programming examples in C, including TCP/UDP servers and clients, HTTP client implementation, and port scanning utilities.

## 📚 Socket Programming Overview

### 🔌 Socket Types
- **TCP (Stream)**: Reliable, connection-oriented communication
- **UDP (Datagram)**: Unreliable, connectionless communication
- **Raw Sockets**: Direct IP protocol access

### 🌐 Network Protocols
- **IPv4/IPv6**: Internet Protocol versions
- **HTTP/HTTPS**: Web protocols
- **FTP**: File Transfer Protocol
- **SMTP**: Email protocol

## 🔌 TCP Socket Programming

### TCP Server Implementation
```c
void startTCPServer() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    
    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    // Bind socket
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    // Listen for connections
    listen(server_socket, MAX_CLIENTS);
    
    // Accept connections
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addrlen);
    
    // Handle client communication
    handleClient(client_socket);
}
```

### TCP Client Implementation
```c
void startTCPClient(const char* server_ip) {
    int socket_desc;
    struct sockaddr_in server_addr;
    
    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    
    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(PORT);
    
    // Connect to server
    connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    // Send/receive data
    send(socket_desc, message, strlen(message), 0);
    recv(socket_desc, server_reply, BUFFER_SIZE, 0);
}
```

### Multi-threaded Server
```c
void* handleClient(void* socket_desc) {
    int client_socket = *(int*)socket_desc;
    
    // Handle client communication
    while (recv(client_socket, buffer, BUFFER_SIZE, 0) > 0) {
        // Process message
        send(client_socket, response, strlen(response), 0);
    }
    
    close(client_socket);
    return NULL;
}
```

## 📦 UDP Socket Programming

### UDP Server Implementation
```c
void startUDPServer() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    // Create UDP socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    
    // Bind socket
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    // Receive messages
    while (1) {
        recvfrom(server_socket, buffer, BUFFER_SIZE, 0, 
                 (struct sockaddr*)&client_addr, &client_len);
        
        // Send response
        sendto(server_socket, response, strlen(response), 0, 
               (struct sockaddr*)&client_addr, client_len);
    }
}
```

### UDP Client Implementation
```c
void startUDPClient(const char* server_ip) {
    int socket_desc;
    struct sockaddr_in server_addr;
    
    // Create UDP socket
    socket_desc = socket(AF_INET, SOCK_DGRAM, 0);
    
    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(PORT);
    
    // Send message
    sendto(socket_desc, message, strlen(message), 0, 
           (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    // Receive response
    recvfrom(socket_desc, server_reply, BUFFER_SIZE, 0, 
             (struct sockaddr*)&server_addr, &server_len);
}
```

## 🌐 HTTP Client Implementation

### Simple HTTP GET Request
```c
void simpleHTTPClient(const char* host, const char* path) {
    int socket_desc;
    struct sockaddr_in server_addr;
    char request[BUFFER_SIZE];
    
    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    
    // Connect to HTTP server (port 80)
    server_addr.sin_port = htons(80);
    connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    // Send HTTP GET request
    snprintf(request, BUFFER_SIZE, 
             "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", path, host);
    send(socket_desc, request, strlen(request), 0);
    
    // Receive HTTP response
    recv(socket_desc, response, sizeof(response), 0);
    
    close(socket_desc);
}
```

### HTTP Request Format
```c
// HTTP GET request
"GET /path HTTP/1.1\r\n"
"Host: example.com\r\n"
"User-Agent: MyClient/1.0\r\n"
"Accept: */*\r\n"
"\r\n"

// HTTP POST request
"POST /api/data HTTP/1.1\r\n"
"Host: example.com\r\n"
"Content-Type: application/json\r\n"
"Content-Length: 123\r\n"
"\r\n"
"{\"key\": \"value\"}"
```

## 🔍 Network Utilities

### Port Scanner
```c
void simplePortScanner(const char* target_ip, int start_port, int end_port) {
    int socket_desc;
    struct sockaddr_in target_addr;
    
    target_addr.sin_family = AF_INET;
    target_addr.sin_addr.s_addr = inet_addr(target_ip);
    
    for (int port = start_port; port <= end_port; port++) {
        target_addr.sin_port = htons(port);
        
        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        
        // Set connection timeout
        struct timeval timeout = {.tv_sec = 1, .tv_usec = 0};
        setsockopt(socket_desc, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
        
        if (connect(socket_desc, (struct sockaddr*)&target_addr, sizeof(target_addr)) == 0) {
            printf("Port %d: OPEN\n", port);
        }
        
        close(socket_desc);
    }
}
```

### Network Information
```c
void getNetworkInfo() {
    char hostname[256];
    struct hostent *host_info;
    
    // Get local hostname
    gethostname(hostname, sizeof(hostname));
    printf("Hostname: %s\n", hostname);
    
    // Get host information
    host_info = gethostbyname(hostname);
    if (host_info) {
        printf("IP Address: %s\n", inet_ntoa(*(struct in_addr*)host_info->h_addr_list[0]));
    }
}
```

## 💡 Key Socket Functions

### Socket Creation
```c
int socket(int domain, int type, int protocol);
// domain: AF_INET (IPv4), AF_INET6 (IPv6)
// type: SOCK_STREAM (TCP), SOCK_DGRAM (UDP)
// protocol: 0 (default for given type)
```

### Address Configuration
```c
struct sockaddr_in {
    sa_family_t sin_family;    // Address family
    unsigned short sin_port;  // Port number (network byte order)
    struct in_addr sin_addr;  // IP address
};
```

### Byte Order Conversion
```c
uint16_t htons(uint16_t hostshort);  // Host to network short
uint32_t htonl(uint32_t hostlong);   // Host to network long
uint16_t ntohs(uint16_t netshort);   // Network to host short
uint32_t ntohl(uint32_t netlong);    // Network to host long
```

### Socket Options
```c
int setsockopt(int socket, int level, int optname, 
               const void *optval, socklen_t optlen);

// Common options:
// SO_REUSEADDR: Allow address reuse
// SO_SNDBUF: Send buffer size
// SO_RCVBUF: Receive buffer size
// SO_SNDTIMEO: Send timeout
// SO_RCVTIMEO: Receive timeout
```

## 🚀 Advanced Topics

### 1. Non-blocking Sockets
```c
// Set non-blocking mode
int flags = fcntl(socket_desc, F_GETFL, 0);
fcntl(socket_desc, F_SETFL, flags | O_NONBLOCK);

// Use select() for non-blocking I/O
fd_set readfds;
FD_ZERO(&readfds);
FD_SET(socket_desc, &readfds);

int result = select(socket_desc + 1, &readfds, NULL, NULL, &timeout);
if (result > 0 && FD_ISSET(socket_desc, &readfds)) {
    // Data available to read
}
```

### 2. SSL/TLS Sockets
```c
// OpenSSL integration (conceptual)
SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
SSL *ssl = SSL_new(ctx);
SSL_set_fd(ssl, socket_desc);
SSL_connect(ssl);

SSL_write(ssl, data, length);
SSL_read(ssl, buffer, sizeof(buffer));
```

### 3. Multicast Sockets
```c
// Join multicast group
struct ip_mreq mreq;
mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_IP);
mreq.imr_interface.s_addr = htonl(INADDR_ANY);

setsockopt(socket_desc, IPPROTO_IP, IP_ADD_MEMBERSHIP, 
           &mreq, sizeof(mreq));
```

### 4. Socket Pairs
```c
int socketpair(int domain, int type, int protocol, int sv[2]);
// Creates two connected sockets
// sv[0] and sv[1] are connected to each other
```

## 📊 Error Handling

### Common Socket Errors
```c
if (socket_desc == -1) {
    switch (errno) {
        case EACCES: printf("Permission denied\n"); break;
        case EADDRINUSE: printf("Address already in use\n"); break;
        case ECONNREFUSED: printf("Connection refused\n"); break;
        case ETIMEDOUT: printf("Connection timed out\n"); break;
        default: printf("Socket error: %s\n", strerror(errno));
    }
}
```

### Robust Error Handling
```c
int safeSend(int socket, const void* data, size_t length) {
    size_t total_sent = 0;
    while (total_sent < length) {
        ssize_t sent = send(socket, data + total_sent, length - total_sent, 0);
        if (sent == -1) {
            if (errno == EINTR) continue; // Interrupted, retry
            return -1;
        }
        total_sent += sent;
    }
    return total_sent;
}

int safeRecv(int socket, void* buffer, size_t length) {
    size_t total_received = 0;
    while (total_received < length) {
        ssize_t received = recv(socket, buffer + total_received, 
                               length - total_received, 0);
        if (received == -1) {
            if (errno == EINTR) continue;
            return -1;
        }
        if (received == 0) break; // Connection closed
        total_received += received;
    }
    return total_received;
}
```

## ⚠️ Common Pitfalls

### 1. Byte Order Issues
```c
// Wrong - using host byte order
server_addr.sin_port = PORT;

// Right - using network byte order
server_addr.sin_port = htons(PORT);
```

### 2. Buffer Management
```c
// Wrong - not null-terminating received data
recv(socket_desc, buffer, BUFFER_SIZE, 0);
printf("Received: %s\n", buffer); // May print garbage

// Right - null-terminate received data
int bytes = recv(socket_desc, buffer, BUFFER_SIZE - 1, 0);
buffer[bytes] = '\0';
printf("Received: %s\n", buffer);
```

### 3. Socket Cleanup
```c
// Wrong - forgetting to close sockets
int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
// Use socket but forget to close

// Right - always close sockets
int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
// Use socket...
close(socket_desc);
```

### 4. Blocking Operations
```c
// Wrong - blocking accept without timeout
accept(server_socket, (struct sockaddr*)&client_addr, &addrlen);
// May block indefinitely

// Right - use select() or set timeout
fd_set readfds;
FD_ZERO(&readfds);
FD_SET(server_socket, &readfds);
select(server_socket + 1, &readfds, NULL, NULL, &timeout);
if (FD_ISSET(server_socket, &readfds)) {
    accept(server_socket, (struct sockaddr*)&client_addr, &addrlen);
}
```

## 🔧 Real-World Applications

### 1. Web Server
```c
void handleHTTPRequest(int client_socket) {
    char request[4096];
    recv(client_socket, request, sizeof(request), 0);
    
    // Parse HTTP request
    if (strncmp(request, "GET", 3) == 0) {
        char* response = "HTTP/1.1 200 OK\r\n"
                       "Content-Type: text/html\r\n"
                       "\r\n"
                       "<html><body><h1>Hello, World!</h1></body></html>";
        send(client_socket, response, strlen(response), 0);
    }
    
    close(client_socket);
}
```

### 2. Chat Server
```c
typedef struct {
    int socket;
    char name[50];
} Client;

Client clients[MAX_CLIENTS];
int client_count = 0;

void broadcastMessage(const char* message, int sender_socket) {
    for (int i = 0; i < client_count; i++) {
        if (clients[i].socket != sender_socket) {
            send(clients[i].socket, message, strlen(message), 0);
        }
    }
}
```

### 3. File Transfer
```c
void sendFile(int socket, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) return;
    
    char buffer[1024];
    size_t bytes_read;
    
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(socket, buffer, bytes_read, 0);
    }
    
    fclose(file);
}
```

### 4. Network Monitor
```c
void monitorNetwork() {
    int raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    
    while (1) {
        char buffer[65536];
        recv(raw_socket, buffer, sizeof(buffer), 0);
        
        // Parse IP header
        struct iphdr* ip_header = (struct iphdr*)buffer;
        printf("Packet from %s\n", inet_ntoa(*(struct in_addr*)&ip_header->saddr));
    }
}
```

## 🎓 Best Practices

### 1. Resource Management
```c
// Always close sockets
void cleanupSocket(int* socket_desc) {
    if (*socket_desc != -1) {
        close(*socket_desc);
        *socket_desc = -1;
    }
}
```

### 2. Input Validation
```c
int validatePort(int port) {
    return (port > 0 && port <= 65535);
}

int validateIP(const char* ip) {
    struct in_addr addr;
    return inet_aton(ip, &addr) != 0;
}
```

### 3. Protocol Compliance
```c
// Follow HTTP standards
void sendHTTPResponse(int socket, int status_code, const char* content) {
    char response[1024];
    snprintf(response, sizeof(response),
             "HTTP/1.1 %d %s\r\n"
             "Content-Type: text/html\r\n"
             "Content-Length: %zu\r\n"
             "\r\n"
             "%s",
             status_code, getHTTPStatusText(status_code),
             strlen(content), content);
    
    send(socket, response, strlen(response), 0);
}
```

### 4. Security Considerations
```c
// Validate input data
int validateInput(const char* input, size_t max_length) {
    if (strlen(input) > max_length) return 0;
    
    // Check for dangerous characters
    for (size_t i = 0; input[i]; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    
    return 1;
}
```

Socket programming in C provides powerful networking capabilities for building client-server applications, network utilities, and communication systems. Master these concepts to create robust networked applications!
