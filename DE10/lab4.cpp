#include <unistd.h>	
#include <iostream>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <opencv2/opencv.hpp>
#include <cstdint> // For fixed-width integer types like uint32_t
#include <cstdlib> // For std::memcpy
#include <unordered_map>


cv::Mat imageBase, imageOverlay;
int32_t brightness=0,contrast=99;
uint32_t command=0;
std::vector<double> histogram(256, 0.0);
double pixel_scale;

struct Packet {
    uint32_t sequenceNumber;
    uint32_t totalChunks;
    uint32_t dataSize;
    std::vector<char> data; // Data payload
};
std::unordered_map<uint32_t, std::map<uint32_t, Packet>> messages;

/**
*	------------------------------------
 *	sets the program mode
 *	0x1001 is baseImage display
 *	0x1002 is modeOverlay
 *	0x2001 is modeBrightnessContrast
 *	------------------------------------
 */
void setMode(uint32_t command)
{
	mode = command;
	updateOutput();
}

void updateOutput()
{
	switch(command){
		case 0x1001:
		cv::imshow("Overlay Disabled", baseImage);
		break;
		case 0x1002:
		generateOverlayImage();
		break;
		case 0x2001:
		generateBrightnessContrastImage();'
		break;
}
/**
 *	------------------------------------
 *	uses baseImage, overlayImage
 *	overlays 
 */	
void generateOverlayImage()
{
    if (baseImage.empty() || overlayImage.empty()) {
        std::cerr << "Error: Could not load one of the images!" << std::endl;
        return;
    }
	// do not modify output and overlay
	cv::Mat output = baseImage.clone();
	cv::Mat overlay = overlayImage.clone();
	
	// crop overlay if larger than base
	int rows = std::min(output.rows, overlay.rows);
	int cols = std::min(output.cols, overlay.cols);
	cv::Mat overlay = overlay(cv::Rect(0, 0, cols, rows));
	
	// select overlay region of base image
	cv::Mat output = output(cv::Rect(0, 0, cols, rows));
	int alpha = 1; // just slap the overlay on top
	cv::addWeighted(overlay, alpha, roi, 1.0 - alpha, 0.0, output);
	cv::imshow("Overlay Result", output);
    waitKey();
}

/**
 *	------------------------------------
 *	Uses baseImage, brightness, contrast
 *	performs histogram equalization
 *	contrast is the equalization range
 *	brightness is added to pixel values
 *	image is displayed on DE10
 *	------------------------------------
 */
void modeBrightnessContrast()
{
    if (baseImage.empty() {
        std::cerr << "Error: Could not load image!" << std::endl;
        return;
    }
	cv::Mat output = baseImage.clone();
	
	int rows = base.rows;
	int cols = base.cols;

    //innitialize histogram for new image
    histogram.fill(0,256);
    pixel_scale = 1.0 /(rows * width);

    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < rows; y++)
        {
            // vector stores count of pixel with index value
            int i = base_image.pixelIndex(x,y);
            histogram[i]++;
        }
    }

    int sum = 0;
    for(int i = 0; i < 256; i++)
    {
        // resolve histogram equalization
        sum += histogram.at(i);
        histogram.replace(i,(brightness + sum * contrast * 1.0 / width / rows));
    }

    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < rows; y++)
        {
            // pixel value at a coordinate is an index
            // value at index is the replacement value for that pixel
            int temp = base_image.pixelIndex(x,y);
            temp = histogram.at(temp);
            temp = qBound(0,temp,255);
            output_image.setPixel(x,y,temp);
        }
    }

    // display file
	
	cv::imshow("Histogram Result", output);
}

void receivePackets(uint32_t messageId, const Packet& packet) {
    messages[messageId][packet.sequenceNumber] = packet;
}

bool recievedAllChunks(uint32_t messageId) {
    auto& chunks = messages[messageId];
    if (chunks.size() == chunks.begin()->second.totalChunks) {
        return true;
    }
    return false;
}



// Function to handle an image message
void processImageMessage(const std::vector<unsigned char> &data) {
    // Decode image from binary data
    cv::Mat image = cv::imdecode(data, cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cerr << "Failed to decode the image!" << std::endl;
        return;
    }

    // Display the image
    cv::imshow("Base Image", image);
    cv::imwrite("Base_Image.BMP", image); // Save image to file
    cv::waitKey(0); // Wait for the user to close the window
    std::cout << "Image processed and saved!" << std::endl;
}

void displayBaseImage(String title)
{
    cv::imshow(title, image);
    //cv::imwrite("Base_Image.BMP", image); // Save image to file
    //cv::waitKey(0); // Wait for the user to close the window
    //std::cout << "Image processed and saved!" << std::endl;
}

// Function to handle an integer message
void processIntegerMessage(uint32_t ID,uint32_t value) {
	uint32_t integerValue;
	std::memcpy(&integerValue, &buffer[16], sizeof(uint32_t));
    std::cout << "Received Integer at " << std::hex << ID << " with value : " << std::dec << integerValue << std::endl;
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
		close(sock);
        return -1;
    }

    std::cout << "Listening for messages..." << std::endl;

    char buffer[1024]; // Max UDP packet size
    sockaddr_in client_address;
    socklen_t client_length = sizeof(client_address);

    while (true) {
        int bytes_received = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_address, &client_length);

        if (bytes_received > 0) {
            buffer[bytes_received] = '\0'; // Null-terminate the received data if needed
			std::memcpy(&messageId, &buffer[0], sizeof(uint32_t));			
			std::memcpy(&messageId, &buffer[0], sizeof(uint32_t));			
			
			receivePackets()
            // Extract `messageID` (first 4 bytes)
			uint32_t messageId;
			std::memcpy(&messageId, &buffer[0], sizeof(uint32_t));			
			switch (messageId){
				case 0x11:
				
				//break;
				case 0x12:
				
				//break;
				case 0x20:
				
				//break;
				case 0x2B:
				
				//break;
				case 0x2C:
				{

								
				}
				// Extract and process integer (bytes 4–8, offset 12 for data)
					//uint32_t integerValue;
					//std::memcpy(&integerValue, &buffer[12], sizeof(uint32_t)); // Safely copy 4 bytes for the integer
					//processIntegerMessage(integerValue);
				break;
				default:
                std::cerr << "Unknown Message ID: " << std::hex << messageId << std::dec << std::endl;				
				break;
			}
        }
    }

    close(sock);
    return 0;
}
