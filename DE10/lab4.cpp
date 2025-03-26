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
double pixel_scale;
std::string windowTitle = "Lab4 Output"

struct Packet {
    uint32_t sequenceNumber;
    uint32_t totalChunks;
//    uint32_t dataSize; // accessed before storage
    std::vector<char> data; // Data payload
};
std::unordered_map<uint32_t, std::map<uint32_t, Packet>> messages;

bool isMessageComplete(uint32_t messageID)
{
	// Get the inner map for the message
    auto& packets = messages[messageID];
	
	// Check that packets.size() == totalChunks;
	if (!packets.empty()) {
        return packets.size() == packets.begin()->second.totalChunks;
    }
	return false;
}

std::vector<char> getCompleteMessage(uint32_t messageID) {
    std::vector<char> completeData;

    // Get the inner map (sequenceNumber -> Packet) for the message
    auto& packets = messages[messageID];

    // Concatenate data in order of sequenceNumber
    for (const auto& [seqNum, packet] : packets) {
        completeData.insert(completeData.end(), packet.data.begin(), packet.data.end());
    }

    // Free memory by erasing the entry
    messages.erase(messageID);

    return completeData;
}

void sortMessages(uint32_t messageID, Packet packet)
{
	storePaket(messageID,packet);
	
	if(isMessageComplete(messageID))
	{
		switch{
			case 0x11:
			setImageBase(constructGrayscaleImage(getCompleteMessage(messageID)))'
			break;
			case 0x12;
			setImageOverlay(constructGrayscaleImage(getCompleteMessage(messageID)))'
			break;
			case 0x20;
			setMode(constructInt32_t(getCompleteMessage(messageID)));
			break;
			case 0x2B;
			setBrightness(constructInt32_t(getCompleteMessage(messageID)));
			break;
			case 0x2C;
			setContrast(constructInt32_t(getCompleteMessage(messageID))));
			break;
			default:
			std::cerr << "Unknown Message ID: " << std::hex << messageId << std::dec << std::endl;				
			break;
		}
	}
}
cv::Mat constructGrayscaleImage(std::vector<char> data)
{
	// Replace IMREAD_COLOR with IMREAD_GRAYSCALE if needed
    cv::Mat image = cv::imdecode(data, cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
         throw std::runtime_error("Error: Could not decode image from char array!")
    }
	return image;
}

int32_t constructInt32_t(std::vector<char> data)
{
	if (data.size() == sizeof(int32_t)) {
        throw std::runtime_error("Incorrect data size for int32_t.Data size: " + std::to_string(data.size()) + " Expected: 4");
    }
	int32_t value;
    std::memcpy(&value, data.data(), sizeof(int32_t));
	std::cout << "Extracted int32_t value: " << value << std::endl;
}

/**
*	------------------------------------
 *	sets the program mode
 *	0x1001 is baseImage display
 *	0x1002 is modeOverlay
 *	0x2001 is modeBrightnessContrast
 *	------------------------------------
 */
void setMode(uint32_t data)
{
	command = data;
	updateOutput();
}
void setBrightness(uint32_t data)
{
	brightness = data;
	updateOutput();
}
void setContrast(uint32_t data)
{
	contrast = data;
	updateOutput();
}
void setImageBase(cv::Mat image)
{
	imageBase = image;
	updateOutput();
}
void setImageOverlay(cv::Mat image)
{
	imageOverlay = image;
	updateOutput();
}

void updateOutput()
{
	switch(command){
		case 0x1001:
		cv::imshow(windowTitle, baseImage);
		break;
		case 0x1002:
		imageOverlay();
		break;
		case 0x2001:
		histogramEqualization();'
		break;
}
/**
 *	------------------------------------
 *	uses baseImage, overlayImage
 *	overlays 
 */	
void imageOverlay()
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
	cv::imshow(windowTitle, output);
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
void histogramEqualization()
{
    if (baseImage.empty() {
        std::cerr << "Error: Could not load image!" << std::endl;
        return;
    }
	cv::Mat output;
    cv::cvtColor(baseImage, output, cv::COLOR_BGR2GRAY);
	
	int rows = output.rows;
	int cols = output.cols;

    //innitialize histogram for new image
	std::vector<int> Histogram(256, 0);
    pixel_scale = 1.0 /(rows * cols);

    for(int x = 0; x < cols; x++)
    {
        for(int y = 0; y < rows; y++)
        {
            // vector stores count of pixel with index value
            int pixelValue = output.at<uchar>(y, x);
            histogram[pixelValue]++;
        }
    }

    int sum = 0;
    for(int i = 0; i < 256; i++)
    {
        // resolve histogram equalization
        sum += histogram[i];
		histogram[i]=std::min(255, std::max(0, brightness + sum * contrast * 1.0 / cols / rows));
    }

	for (int x = 0; x < cols; x++) {
		for (int y = 0; y < rows; y++) {
            // pixel value at a coordinate is an index
            // value at index is the replacement value for that pixel
			int pixelValue = output.at<uchar>(y, x);
			pixelValue = histogram[pixelValue];
			output.at<uchar>(y, x) = newValue;
		}
	}

    // display file
	cv::imshow(windowTitle, output);
    waitKey();
}
void storeBufferAsPacket(const char* charArray, size_t arraySize = 1024) {
	
    // Ensure the array has enough bytes for metadata (16 bytes: messageID, sequenceNumber, totalChunks, dataSize)
    if (arraySize < sizeof(uint32_t) * 4) {
        throw std::runtime_error("Character array is too small to extract packet metadata");
    }
	
    uint32_t messageID, sequenceNumber, totalChunks, dataSize;
    std::memcpy(&messageID, charArray, sizeof(uint32_t)); // Extract messageID
    std::memcpy(&sequenceNumber, charArray + sizeof(uint32_t), sizeof(uint32_t)); // Extract sequenceNumber
    std::memcpy(&totalChunks, charArray + 2 * sizeof(uint32_t), sizeof(uint32_t)); // Extract totalChunks
    std::memcpy(&dataSize, charArray + 3 * sizeof(uint32_t), sizeof(uint32_t)); // Extract dataSize

    if (dataSize > arraySize - sizeof(uint32_t) * 4) {
        throw std::runtime_error("Character array is too small for the indicated data payload size");
    }

    //Extract data payload
    const char* dataStart = charArray + sizeof(uint32_t) * 4; // Payload starts after metadata
    std::vector<char> data(dataStart, dataStart + dataSize); // Copy the data payload

    //Create the Packet
    Packet packet = {sequenceNumber, totalChunks, data};
    messages[messageID][sequenceNumber] = packet;
}

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
            //buffer[bytes_received] = '\0'; // Null-terminate the received data if needed
			std::memcpy(&messageId, &buffer[0], sizeof(uint32_t));		
			
			storeBufferAsPacket(buffer);

        }
    }

    close(sock);
    return 0;
}
