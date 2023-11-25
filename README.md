# LSB-Image-Steganography
LSB Image Steganography: Conceal secret information within image files using Least Significant Bit manipulation. A simple yet powerful tool for covert data embedding in digital images.

Overview: This project implements a simple yet effective Least Significant Bit (LSB) Image Steganography technique. Steganography is the art and science of hiding information within other data, and LSB-based methods involve concealing data within the least significant bits of pixel values in an image.

#Features:

Encoding: Embed secret messages or files into image files using the LSB technique. Decoding: Retrieve hidden messages or files from steganographic images. Support for BMP Images: The implementation works with BMP (Bitmap) image files, a widely used image format. User-friendly Interface: Simple command-line interface for ease of use
![image](https://github.com/Ribizz007/LSB-Image-Steganography/assets/95741045/fc60db3c-4efe-4828-8100-917d378bb4e7)

How to Use:

1. Encoding:

* Provide the source image file, secret data file, and the desired stego image file name.
* The secret data is embedded in the least significant bits of the source image.
bash

./result.exe -e source_image.bmp secret.txt stego_image.bmp

2. Decoding:

*Provide the stego image file name to retrieve the hidden data.

./result.exe -d stego_image.bmp


Getting Started:

1. Clone the repository:
2. Compile the code:

 -> cd LSB-Image-Steganography
 
 ->make








