#include <iostream>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <opencv2/opencv.hpp>

// Function to handle an image message
void processImageMessage(const std::vector<unsigned char> &data) {
    // Decode image from binary data
    cv::Mat image = cv::imdecode(data, cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cerr << "Failed to decode the image!" << std::endl;
        return;
    }

    // Display the image
    cv::imshow("Received Image", image);
    cv::imwrite("received_image.jpg", image); // Save image to file
    cv::waitKey(0); // Wait for the user to close the window
    std::cout << "Image processed and saved!" << std::endl;
}

// Function to handle an integer message
void processIntegerMessage(uint32_t value) {
    std::cout << "Received Integer: " << value << std::endl;
    // Perform any additional logic you want for integers
}

// Main function to receive and process messages
int main() {
    // Set up the UDP socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation failed!" << std::endl;
        return -1;
    }

    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(5005); // Listening port
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Binding failed!" << std::endl;
        return -1;
    }

    std::cout << "Listening for messages..." << std::endl;

    char buffer[65507]; // Max UDP packet size
    sockaddr_in client_address;
    socklen_t client_length = sizeof(client_address);

    while (true) {
        int bytes_received = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_address, &client_length);

        if (bytes_received > 0) {
            buffer[bytes_received] = '\0'; // Null-terminate the received data if needed

            // Extract `messageID` (first 4 bytes)
            uint32_t messageId = *(uint32_t*)&buffer[0];

            // Act based on `messageID`
            if (messageId == 1) { // Image message
                std::cout << "Processing image message..." << std::endl;

                // Extract and process image data (bytes after header)
                std::vector<unsigned char> imageData(buffer + 4, buffer + bytes_received);
                processImageMessage(imageData);

            } else if (messageId == 2) { // Integer message
                std::cout << "Processing integer message..." << std::endl;

                // Extract and process integer (bytes 4–8)
                uint32_t integerValue = *(uint32_t*)&buffer[4];
                processIntegerMessage(integerValue);

            } else {
                std::cerr << "Unknown Message ID: " << messageId << std::endl;
            }
        }
    }

    close(sock);
    return 0;
}
