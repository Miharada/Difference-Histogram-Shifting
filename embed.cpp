#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <bitset>
#include <iostream>

// Show ASCII and Binary of Text That Want to Be Embedded
void PrintStringBinary(std::vector<int> binarr, std::string plaintext){
    std::cout<< "Plaintext: " << plaintext << std::endl;
    std::cout<<"Binary : ";
    for (int bit : binarr) {
        std::cout << bit;
    }
    std::cout<<std::endl;
}

// Convert ASCII to Binary
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
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <image_file>" << " <text_to_embed>" << std::endl;
        return 1;
    }

    // Get the image filename from the command line arguments
    std::string imagePath = argv[1];
    std::string textarg = argv[2];
    std:: string text = textarg + "|_";

    std::vector<int> binarr = stringToBinaryArray(text);
    PrintStringBinary(binarr, text);
    

    cv::Mat img = cv::imread(imagePath, cv::IMREAD_COLOR);
	
	if (img.empty()){
		std::cout << "No File" << std::endl;
		return -1;
	}

    std::cout << "Width : " << img.size().width << std::endl;
    std::cout << "Height: " << img.size().height << std::endl;

    int capacity = 0;
    int blockSize = 3;
 
    // Process Image into sub-block 3x3
    for (int y=0, inc=3; y <= img.rows - blockSize; y+=inc){ //img.rows
        for (int x=0, inc2=3; x <= img.cols - blockSize; x+=inc2){ //img.cols
           
             for (int j=0; j < blockSize; ++j){
                for (int i=0; i < blockSize; i++){
                    // Get the pixel value at (x+1, y+j)

                    cv::Vec3b& pixel = img.at<cv::Vec3b>(y+j, x+i);
                    
                    // Iterate Pixels in Sub Block
                    for (int c=0; c < 3; ++c){
            

                        if (pixel[c] == 0 || pixel[c] == 255){
                            if (pixel[c] == 0){
                                pixel[c] = 1; 
                            }
                            else if (pixel[c] == 255){
                                pixel[c] = 254;
                            }
                        }
                    }
                    // Embed Data
                    cv::Vec3b& midpixel = img.at<cv::Vec3b>(y+j, x+1);
                    int D{0};

                    for (int c=0; c < 3; ++c){
                        // Calculate Difference Pixel
                        
                        if (i==1 && c==1){
                            if (midpixel[1] == 0){
                                midpixel[1] = 1;
                            }
                            else if (midpixel[1] == 255){
                                midpixel[1] = 254;
                            }
                            continue;
                        }
                        else{
                            if (midpixel[1] == 0){
                                midpixel[1] = 1;
                            }
                            else if (midpixel[1] == 255){
                                midpixel[1] = 254;
                            }
                            D = pixel[c] - midpixel[1];
                        }

                        // Count Capacity
                        if (D == -1 || D == 0){
                            capacity += 1;
                        }
    
                        // Embed process Here
                        if (D < -1){
                            pixel[c] -= 1;
                        }
                        else if (D > 0){
                            pixel[c] += 1;
                        }
                        else if (D == -1 && binarr.size() != 0){
                            pixel[c] -= binarr.front();
                            binarr.erase(binarr.begin());
                        }
                        else if (D == 0 && binarr.size() != 0){
                            pixel[c] += binarr.front();
                            binarr.erase(binarr.begin());
                        }
                    }
                }
             }
        }
    }
    std::cout<<std::endl;
    // Save the modified image to a file
    if (!cv::imwrite("modified_image.png", img)) {
        std::cerr << "Error: Unable to save the modified image." << std::endl;
        return -1;
    }

    std::cout << "Bit Capacity: " << capacity << " bit" << std::endl;
    std::cout << "Char Capacity: " << capacity/8 << " Char" << std::endl;
    std::cout << "Length Input: " << text.length() << " Char <-> " << text.length()*8 << " bits" << std::endl;

    std::cout << "Modified image saved as modified_image.png" << std::endl;
    std::cout << "Done!" << std::endl;

    return 0;
}
