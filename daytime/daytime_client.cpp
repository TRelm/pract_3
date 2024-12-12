#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <port>" << std::endl;
        return 1;
    }

    const char* server_ip = argv[1];
    int port = std::stoi(argv[2]);

    
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    
    sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        close(sockfd);
        return 1;
    }

   
    const char* request = "REQUEST";
    if (sendto(sockfd, request, std::strlen(request), 0, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Sendto failed");
        close(sockfd);
        return 1;
    }

    
    char buffer[BUFFER_SIZE];
    socklen_t server_len = sizeof(server_addr);
    int n = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0, (sockaddr*)&server_addr, &server_len);
    if (n < 0) {
        perror("Recvfrom failed");
        close(sockfd);
        return 1;
    }

    buffer[n] = '\0'; 
    std::cout << "Daytime from server: " << buffer << std::endl;

    
    close(sockfd);
    return 0;
}
