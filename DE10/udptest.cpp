#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

int main() {
    // Create a UDP socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation failed!" << std::endl;
        return -1;
    }

    // Bind the socket to the device at 192.168.1.9 and port 5005
    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(5005); // Listening port
    inet_pton(AF_INET, "192.168.1.9", &server_address.sin_addr);

    if (bind(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Binding failed!" << std::endl;
        close(sock);
        return -1;
    }

    std::cout << "Listening for UDP transmissions on 192.168.1.9:5005..." << std::endl;

    // Buffer for incoming data
    char buffer[65507]; // Max UDP packet size
    sockaddr_in client_address;
    socklen_t client_length = sizeof(client_address);

    while (true) {
        // Receive UDP datagram
        int bytes_received = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_address, &client_length);

        if (bytes_received > 0) {
            // Extract metadata from the first 12 bytes
            uint32_t messageId = *(uint32_t*)&buffer[0];          // First 4 bytes: messageId
            uint32_t sequenceNumber = *(uint32_t*)&buffer[4];     // Next 4 bytes: sequenceNumber
            uint32_t totalChunks = *(uint32_t*)&buffer[8];        // Next 4 bytes: totalChunks

            // Extract first byte of chunk data
            char firstByteOfData = buffer[12]; // First byte after the metadata

            // Print the information
            std::cout << "Message ID: " << messageId 
                      << ", Chunk Number: " << sequenceNumber + 1 << " of " << totalChunks 
                      << ", First Byte of Data: 0x" << std::hex << (unsigned int)(unsigned char)firstByteOfData 
                      << std::dec << std::endl;
        }
    }

    // Close the socket
    close(sock);
    return 0;
}
