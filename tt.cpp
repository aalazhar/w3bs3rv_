#include <iostream>
#include <sstream>

int main() {
    std::stringstream input;
    input << "3\r\nfoo\r\n6\r\nbarbaz\r\n0\r\n\r\n";

    std::string line;
    std::string requestData;
    std::getline(input, line);  // Read the first line

    // Parse and process the chunks
    while (std::getline(input, line)) {
        int chunkSize = std::hexToDec(line);  // Convert hex chunk size to decimal
        if (chunkSize == 0) {
            // End of chunks
            break;
        }

        // Read the chunk data
        char* buffer = new char[chunkSize + 1];
        input.read(buffer, chunkSize);
        buffer[chunkSize] = '\0';

        // Process the chunk data
        requestData += buffer;

        delete[] buffer;
        std::getline(input, line);  // Discard the CRLF after the chunk data
    }

    // Process the complete request data
    std::cout << "Request data: " << requestData << std::endl;

    return 0;
}