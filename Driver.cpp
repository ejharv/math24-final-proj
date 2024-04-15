#include <fstream>
#include <iostream>
#include "SVDCompression.hpp"
#include "Image.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    // Check for the correct number of arguments
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <input_path> <output_path> <compression_factor>\n";
        return 1;
    }

    string inputFilePath = argv[1];
    string outputFilePath = argv[2];
    int k = stoi(argv[3]); // Converts the compression factor from string to integer

    // Open the PPM file to read the image
    ifstream inputFile(inputFilePath);
    if (!inputFile.is_open()){
        cerr << "Error opening file: " << inputFilePath << endl;
        return 1;
    }

    // Initialize the image from the input file
    Image img; // Using automatic storage to avoid manual memory management
    Image_init(&img, inputFile); // Pass address of img since Image_init expects a pointer
    inputFile.close();

    // Compress the image using the SVD compression module
    SVDCompression::compressImage(&img, k); // Directly pass the address of img

    // Now write the compressed image to an output file
    ofstream outputFile(outputFilePath);
    if (!outputFile.is_open()) {
        cerr << "Failed to open output file: " << outputFilePath << endl;
        return 1;
    }

    Image_print(&img, outputFile); // Pass the address of img since Image_print expects a pointer
    outputFile.close();

    cout << "Compression complete. Output saved to: " << outputFilePath << endl;

    return 0;
}


