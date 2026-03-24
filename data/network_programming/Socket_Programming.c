#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

// Note: This is Unix/Linux socket programming
// For Windows, you would need to use winsock2.h and ws2_32.lib

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

// =============================================================================
// TCP SERVER IMPLEMENTATION
// =============================================================================

void* handleClient(void* socket_desc) {
    int client_socket = *(int*)socket_desc;
    char buffer[BUFFER_SIZE];
    int read_size;
    
    // Send welcome message to client
    char* welcome_msg = "Welcome to TCP Server! Type 'exit' to disconnect.\n";
    send(client_socket, welcome_msg, strlen(welcome_msg), 0);
    
    // Receive messages from client
    while ((read_size = recv(client_socket, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[read_size] = '\0';
        
        printf("Client message: %s", buffer);
        
        // Check for exit condition
        if (strncmp(buffer, "exit", 4) == 0) {
            char* exit_msg = "Goodbye!\n";
            send(client_socket, exit_msg, strlen(exit_msg), 0);
            break;
        }
        
        // Echo back to client
        char response[BUFFER_SIZE];
        snprintf(response, BUFFER_SIZE, "Echo: %s", buffer);
        send(client_socket, response, strlen(response), 0);
    }
    
    if (read_size == 0) {
        printf("Client disconnected\n");
    } else if (read_size == -1) {
        perror("recv failed");
    }
    
    close(client_socket);
    free(socket_desc);
    return NULL;
}

void startTCPServer() {
    int server_socket, client_socket, *new_sock;
    struct sockaddr_in server_addr, client_addr;
    int addrlen = sizeof(struct sockaddr_in);
    
    printf("=== TCP SERVER ===\n");
    
    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Could not create socket");
        return;
    }
    
    printf("Socket created\n");
    
    // Prepare sockaddr_in structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    // Bind
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return;
    }
    
    printf("Bind done\n");
    
    // Listen
    listen(server_socket, MAX_CLIENTS);
    printf("Server listening on port %d\n", PORT);
    
    // Accept incoming connections
    while (1) {
        printf("Waiting for incoming connections...\n");
        
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }
        
        printf("Connection accepted from %s:%d\n", 
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        
        // Create new thread for client
        pthread_t thread_id;
        new_sock = malloc(sizeof(int));
        *new_sock = client_socket;
        
        if (pthread_create(&thread_id, NULL, handleClient, (void*)new_sock) < 0) {
            perror("Could not create thread");
            free(new_sock);
        }
        
        // Detach thread to allow independent execution
        pthread_detach(thread_id);
    }
    
    close(server_socket);
}

// =============================================================================
// TCP CLIENT IMPLEMENTATION
// =============================================================================

void startTCPClient(const char* server_ip) {
    int socket_desc;
    struct sockaddr_in server_addr;
    char message[BUFFER_SIZE];
    char server_reply[BUFFER_SIZE];
    
    printf("=== TCP CLIENT ===\n");
    
    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Could not create socket\n");
        return;
    }
    
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    
    // Connect to remote server
    if (connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Connect error\n");
        return;
    }
    
    printf("Connected to server %s:%d\n", server_ip, PORT);
    
    // Receive welcome message
    if (recv(socket_desc, server_reply, BUFFER_SIZE - 1, 0) < 0) {
        printf("Recv failed\n");
        return;
    }
    printf("Server: %s", server_reply);
    
    // Communicate with server
    while (1) {
        printf("Enter message (or 'exit' to quit): ");
        fgets(message, BUFFER_SIZE, stdin);
        
        // Send message
        if (send(socket_desc, message, strlen(message), 0) < 0) {
            printf("Send failed\n");
            break;
        }
        
        // Check for exit condition
        if (strncmp(message, "exit", 4) == 0) {
            break;
        }
        
        // Receive reply
        if (recv(socket_desc, server_reply, BUFFER_SIZE - 1, 0) < 0) {
            printf("Recv failed\n");
            break;
        }
        
        server_reply[recv(socket_desc, server_reply, BUFFER_SIZE - 1, 0)] = '\0';
        printf("Server reply: %s", server_reply);
    }
    
    close(socket_desc);
    printf("Disconnected from server\n");
}

// =============================================================================
// UDP SERVER IMPLEMENTATION
// =============================================================================

void startUDPServer() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t client_len = sizeof(client_addr);
    
    printf("=== UDP SERVER ===\n");
    
    // Create UDP socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        perror("Could not create UDP socket");
        return;
    }
    
    printf("UDP socket created\n");
    
    // Prepare sockaddr_in structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT + 1); // Use different port for UDP
    
    // Bind
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("UDP bind failed");
        return;
    }
    
    printf("UDP server listening on port %d\n", PORT + 1);
    
    // Receive UDP messages
    while (1) {
        printf("Waiting for UDP messages...\n");
        
        int recv_len = recvfrom(server_socket, buffer, BUFFER_SIZE - 1, 0, 
                               (struct sockaddr*)&client_addr, &client_len);
        if (recv_len < 0) {
            perror("UDP recvfrom failed");
            continue;
        }
        
        buffer[recv_len] = '\0';
        printf("UDP message from %s:%d: %s", 
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);
        
        // Send response
        char response[BUFFER_SIZE];
        snprintf(response, BUFFER_SIZE, "UDP Echo: %s", buffer);
        sendto(server_socket, response, strlen(response), 0, 
               (struct sockaddr*)&client_addr, client_len);
        
        if (strncmp(buffer, "exit", 4) == 0) {
            break;
        }
    }
    
    close(server_socket);
}

// =============================================================================
// UDP CLIENT IMPLEMENTATION
// =============================================================================

void startUDPClient(const char* server_ip) {
    int socket_desc;
    struct sockaddr_in server_addr;
    char message[BUFFER_SIZE];
    char server_reply[BUFFER_SIZE];
    
    printf("=== UDP CLIENT ===\n");
    
    // Create UDP socket
    socket_desc = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_desc == -1) {
        printf("Could not create UDP socket\n");
        return;
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(PORT + 1);
    
    printf("UDP client ready, sending to %s:%d\n", server_ip, PORT + 1);
    
    // Communicate with server
    while (1) {
        printf("Enter UDP message (or 'exit' to quit): ");
        fgets(message, BUFFER_SIZE, stdin);
        
        // Send message
        if (sendto(socket_desc, message, strlen(message), 0, 
                  (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            printf("UDP send failed\n");
            break;
        }
        
        // Check for exit condition
        if (strncmp(message, "exit", 4) == 0) {
            break;
        }
        
        // Receive reply
        socklen_t server_len = sizeof(server_addr);
        int recv_len = recvfrom(socket_desc, server_reply, BUFFER_SIZE - 1, 0, 
                               (struct sockaddr*)&server_addr, &server_len);
        if (recv_len < 0) {
            printf("UDP recv failed\n");
            break;
        }
        
        server_reply[recv_len] = '\0';
        printf("Server reply: %s", server_reply);
    }
    
    close(socket_desc);
    printf("UDP client closed\n");
}

// =============================================================================
// ADVANCED NETWORKING EXAMPLES
// =============================================================================

// Simple HTTP client
void simpleHTTPClient(const char* host, const char* path) {
    int socket_desc;
    struct sockaddr_in server_addr;
    char request[BUFFER_SIZE];
    char response[4096];
    
    printf("=== SIMPLE HTTP CLIENT ===\n");
    
    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Could not create socket\n");
        return;
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80); // HTTP port
    
    // Convert hostname to IP address (simplified)
    server_addr.sin_addr.s_addr = inet_addr(host);
    
    // Connect
    if (connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("HTTP connection failed\n");
        return;
    }
    
    // Send HTTP GET request
    snprintf(request, BUFFER_SIZE, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", path, host);
    if (send(socket_desc, request, strlen(request), 0) < 0) {
        printf("HTTP request failed\n");
        close(socket_desc);
        return;
    }
    
    printf("HTTP Request sent:\n%s", request);
    
    // Receive HTTP response
    int total_bytes = 0;
    int bytes_received;
    while ((bytes_received = recv(socket_desc, response + total_bytes, 
                                  sizeof(response) - total_bytes - 1, 0)) > 0) {
        total_bytes += bytes_received;
        if (total_bytes >= sizeof(response) - 1) break;
    }
    
    response[total_bytes] = '\0';
    printf("\nHTTP Response (first 1000 chars):\n%.1000s\n", response);
    
    close(socket_desc);
}

// Simple port scanner
void simplePortScanner(const char* target_ip, int start_port, int end_port) {
    int socket_desc;
    struct sockaddr_in target_addr;
    
    printf("=== SIMPLE PORT SCANNER ===\n");
    printf("Scanning %s from port %d to %d\n", target_ip, start_port, end_port);
    
    target_addr.sin_family = AF_INET;
    target_addr.sin_addr.s_addr = inet_addr(target_ip);
    
    for (int port = start_port; port <= end_port; port++) {
        target_addr.sin_port = htons(port);
        
        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_desc == -1) {
            continue;
        }
        
        // Set timeout for connection attempt
        struct timeval timeout;
        timeout.tv_sec = 1; // 1 second timeout
        timeout.tv_usec = 0;
        setsockopt(socket_desc, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
        
        if (connect(socket_desc, (struct sockaddr*)&target_addr, sizeof(target_addr)) == 0) {
            printf("Port %d: OPEN\n", port);
        }
        
        close(socket_desc);
    }
    
    printf("Port scan completed\n");
}

// =============================================================================
// DEMONSTRATION FUNCTIONS
// =============================================================================

void demonstrateTCPServer() {
    printf("Starting TCP Server (press Ctrl+C to stop)...\n");
    startTCPServer();
}

void demonstrateTCPClient() {
    // Connect to localhost
    startTCPClient("127.0.0.1");
}

void demonstrateUDPServer() {
    printf("Starting UDP Server (press Ctrl+C to stop)...\n");
    startUDPServer();
}

void demonstrateUDPClient() {
    // Connect to localhost
    startUDPClient("127.0.0.1");
}

void demonstrateHTTPClient() {
    // Simple HTTP request to example.com (using IP for simplicity)
    printf("Note: This example uses a direct IP. In practice, you'd use DNS resolution.\n");
    simpleHTTPClient("93.184.216.34", "/"); // example.com IP
}

void demonstratePortScanner() {
    // Scan localhost common ports
    simplePortScanner("127.0.0.1", 20, 25);
}

int main(int argc, char* argv[]) {
    printf("Socket Programming Examples\n");
    printf("===========================\n\n");
    
    if (argc < 2) {
        printf("Usage: %s <mode>\n", argv[0]);
        printf("Modes:\n");
        printf("  tcp_server    - Start TCP server\n");
        printf("  tcp_client    - Start TCP client\n");
        printf("  udp_server    - Start UDP server\n");
        printf("  udp_client    - Start UDP client\n");
        printf("  http_client   - Simple HTTP client\n");
        printf("  port_scanner  - Port scanner\n");
        return 1;
    }
    
    if (strcmp(argv[1], "tcp_server") == 0) {
        demonstrateTCPServer();
    } else if (strcmp(argv[1], "tcp_client") == 0) {
        demonstrateTCPClient();
    } else if (strcmp(argv[1], "udp_server") == 0) {
        demonstrateUDPServer();
    } else if (strcmp(argv[1], "udp_client") == 0) {
        demonstrateUDPClient();
    } else if (strcmp(argv[1], "http_client") == 0) {
        demonstrateHTTPClient();
    } else if (strcmp(argv[1], "port_scanner") == 0) {
        demonstratePortScanner();
    } else {
        printf("Unknown mode: %s\n", argv[1]);
        return 1;
    }
    
    return 0;
}
