#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <bitset>
#include <iostream>

void PrintPixelCoordinate(int x, int y, int i, int j, cv::Vec3b& pixel){
    std::cout << "Pixel found at (" << x + i << ", " << y + j 
                                    << ") with RGB values: (" 
                                    << static_cast<int>(pixel[0]) << ", "
                                    << static_cast<int>(pixel[1]) << ", "
                                    << static_cast<int>(pixel[2]) << ")" << std::endl;
}

void PrintStringBinary(std::vector<int> binarr){
    std::string binaryString;
    std::cout<<"Binary : ";
    for (int bit : binarr) {
        binaryString += std::to_string(bit);
    }
    std::string delimiter = "0111110001011111"; // Pipe symbeol as delimiter '|_'
    size_t pos = 0;
    std::string result;

    pos = binaryString.find(delimiter);

    if (pos != std::string::npos) {
        // Extract substring up to the delimiter
        result = binaryString.substr(0, pos);
        
        // Print the result
        std::cout << result << std::endl;
    } else {
        // If delimiter not found, print the whole string
        std::cout << binaryString << std::endl;
    }

    // Ensure the length of the binary string is a multiple of 8
    if (result.length() % 8 != 0) {
        std::cerr << "Error: Binary string length is not a multiple of 8." << std::endl;
    }
    // Convert Binary to ASCII
    std::string asciiString;
    for (size_t i = 0; i < result.length(); i += 8) {
        std::string byteString = result.substr(i, 8);
        std::bitset<8> byte(byteString);
        char asciiChar = static_cast<char>(byte.to_ulong());
        asciiString += asciiChar;
    }

    // Output the resulting ASCII string
    std::cout << "ASCII String: " << asciiString << std::endl;

    std::cout<<std::endl;
}

std::vector<int> stringToBinaryArray(const std::string& str){
    std::vector<int> binaryArray;
    binaryArray.reserve(str.size()*8); // Reserve space to avoid multiple allocations

    for (char c : str){
        std::bitset<8> bits(c); //Convert char to 8-bit binary representation
        
        // Append each bit to the binaryArray
        for (int i=7; i>=0; --i){
            binaryArray.push_back(bits[i]);
        }
    }
    return binaryArray;
}

int main(int argc, char** argv){
    
    
    std::cout<<std::endl;
    // Check if the user provided an image filename
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <image_file>" << std::endl;
        return 1;
    }

    // Get the image filename from the command line arguments
    std::string imagePath = argv[1];
    
    cv::Mat img = cv::imread(imagePath, cv::IMREAD_COLOR);
	
	if (img.empty()){
		std::cout << "No File" << std::endl;
		return -1;
	}

    std::vector<int> binarydata{};

    int blockSize = 3;

    // Process Image into sub-block 3x3
    for (int y=0, inc=3; y <= img.rows - blockSize; y+=inc){ //img.rows
        for (int x=0, inc2=3; x <= img.cols - blockSize; x+=inc2){ //img.cols
           
             for (int j=0; j < blockSize; ++j){
                for (int i=0; i < blockSize; ++i){

                    cv::Vec3b& pixel = img.at<cv::Vec3b>(y+j, x+i);
                    cv::Vec3b& midpixel = img.at<cv::Vec3b>(y+j, x+1);
                    int D{0};


                    for (int c=0; c < 3; ++c){
                        // Calculate D
                        if (i==1 && c==1){
                            continue;
                        }
                        else{
                            D = pixel[c] - midpixel[1];
                        }
                        
                        if (D == 0 || D == -1){
                            binarydata.push_back(0);
                        }
                        else if (D == 1 || D == -2){
                            binarydata.push_back(1);
                        }
                        
                    }
                    
                }
             }
        }
    }
    PrintStringBinary(binarydata);
    return 0;
}