# Difference Histogram Shifting for Data Hiding

Welcome to the repository for the **Difference Histogram Shifting (DHS)** method for data hiding, implemented in C++! This project demonstrates how to embed hidden data into PNG images while preserving their original quality.

## Overview

Difference Histogram Shifting is a technique used to discreetly embed information within an image by manipulating the distribution of pixel intensity differences.

## Requirements

- **OpenCV Library for C++**: This project relies on OpenCV for image processing tasks.

## Compilation Instructions

### On Linux

To compile the project, use the following commands. Ensure that OpenCV is correctly installed on your system.

1. **Compile the Extractor**
   ```bash
   g++ extract.cpp -I /usr/local/share/opencv4/ -I /usr/local/include/opencv4/ -L /usr/local/lib/ -lopencv_core -lopencv_videoio -lopencv_imgcodecs -o extract

2. **Compile the Embedder**
   ```bash
   g++ embed.cpp -I /usr/local/share/opencv4/ -I /usr/local/include/opencv4/ -L /usr/local/lib/ -lopencv_core -lopencv_videoio -lopencv_imgcodecs -o embed


## Usage
   ```bash
./embed <image.png> <data>
```

```bash
./extract <embeddedImage.png>
```
