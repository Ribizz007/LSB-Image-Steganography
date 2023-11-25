#include <stdio.h>
#include <string.h>
#include "encode.h"
#include"common.h"
#include "types.h"

/* Function Definitions */
Status read_and_validate_encode_args(char *argv[],EncodeInfo *encInfo)
{
    //STEP 1 : Check argv[2] is bmp or not
    //         if true -> store into src_image_fname,goto STEP 2
    //         else -> print error and return 
    if(strcmp(strstr(argv[2],"."),".bmp")==0)
    {
        encInfo->src_image_fname=argv[2];
        //STEP 2 : Check argv[3] is .txt or not
        //         if true -> store into secret_fname,goto STEP 3
        //         else -> print error and return e_failure
        if(strcmp(strstr(argv[3],"."),".txt")==0)
        {
            strcpy(encInfo->extn_secret_file,strstr(argv[3],"."));
            encInfo->secret_fname=argv[3];
            //STEP 3 : Check argv[4] is passed or not
            //         if true -> check .bmp or not
            //                 if true -> store into stego_image_fname
            //                 else -> print error and return e_failure
            //         else -> Store default name into stego_image_fname.//output.bmp 
            
            if(argv[4]!=NULL)
            {
                if(strcmp(strstr(argv[4],"."),".bmp")==0)
                {
                    encInfo->stego_image_fname=argv[4];                    
                }
                else
                {
                    printf("Output file is not .bmp file\n");
                    return e_failure;
                }
            }
            else
            {
                encInfo->stego_image_fname="stgo.bmp";
                printf("INFO : Output file not mentioned, default file name added\n");
            }
        }
        else
        {
            printf("secret file is not .txt file\n");
            return e_failure;
        }
    }
    else
    {
        printf("source file is not bmp file\n");
        return e_failure;
    }
    //return e_success
    return e_success;

}

//ENCODE
Status do_encoding(EncodeInfo *encInfo)
{
    //STEP1 : call the open_files function
    if(open_files(encInfo)==e_success)
    {
        //if true print msg and goto STEP2
        printf("INFO : Files are opened successfully\n");
        //STEP2 : Call the check_capacity function
        if(check_capacity(encInfo)==e_success)
        {
            //if true-> print msg and goto STEP3
            printf("INFO : Checking file capacity successfully completed\n");
            //STEP3 : Call the copy header function
            if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image)==e_success)
            {
                //if true -> print msg and go to STEP4
                printf("INFO : Copy bmp header is successfull\n");
                // STEP4 : Call the encode_magic_string function
                if(encode_magic_string(MAGIC_STRING,encInfo)==e_success)
                {
                   //if true -> print msg and go to STEP5
                    printf("INFO : Magic string encode executed successfully\n");
                    // STEP5 : Call the file extention size function
                        if(encode_extn_size(strlen(encInfo->extn_secret_file),encInfo)==e_success)
                        {
                        //if true -> print msg and go to STEP6
                        printf("INFO : Secret file extention size encodeed successfully.\n");
                        // STEP6 : Call the encode_secret_file_extn function
                        if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo)==e_success)
                        {
                            //if true -> print msg and go to STEP7
                            printf("INFO : Secret file extention encodeed successfully.\n");
                            //STEP7 : Call the encode secret file size 
                            if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_success)
                            {
                                //if true -> print msg and go to STEP8
                                printf("INFO : Secret file size encodeed successfully\n");
                                // STEP8 : Call the encode_secret_file_size
                                if(encode_secret_file_data(encInfo)==e_success)
                                {
                                    //if true -> print msg and go to STEP9
                                    printf("INFO : Secret file data encodeed successfully\n");
                                    //STEP 9: Copy remaining image bytes from src to stego image after encoding
                                    if(copy_remaining_img_data(encInfo)==e_success)
                                    {
                                        //if true -> print msg 
                                        printf("INFO : Copying the renaining data is successfully\n");
                                    }
                                    else
                                    {
                                        //else -> print error msg and return failure.
                                        printf("copy_remaining_img_data not execuited successfully\n");
                                        return e_failure;
                                    }
                                }
                                else
                                {
                                    //else -> print error msg and return failure.
                                    printf("encode_secret_file_data not execuited successfully\n");
                                    return e_failure;
                                }
                            }
                            else
                            {
                                //else -> print error msg and return failure.
                                printf("encode_secret_file_size not execuited successfully\n");
                                return e_failure;
                            }

                        }
                        else
                        {
                            //else -> print error msg and return failure.
                            printf("encode_secret_file_extn not execuited successfully\n");
                            return e_failure;
                        }

                        }
                        else
                        {
                            //else -> print error msg and return failure.
                            printf("Encode file extention size not execuited successfully\n");
                            return e_failure;
                        }

                }
                else
                {
                    //else -> print error msg and return failure.
                    printf("Encode magic string not execuited successfully\n");
                    return e_failure;
                }

            }
            else
            {
                //else -> print error msg and return failure.
                printf("INFO : Hedder NOT copyed successfully\n");
                return e_failure;
            }
        }
        else
        {
            //else -> print error msg and return failure.
            printf("INFO : Capacity check , No capacity\n");     
            return e_failure;   
        }
    }
    else
    {
        //else -> print error msg and return failure.
        printf("INFO : Files are not open sucessfully\n");
        return e_failure;
    }
      
    printf("--------------------------------------\n");
    printf("INFO : Encoding Successfully Completed\n");
    printf("--------------------------------------\n");

    return e_success ;

    
}

/*Copy the remaing data to */
Status copy_remaining_img_data(EncodeInfo *encInfo)
{
    // Variable to store each byte of remaining data
    char remaining;
    // Continue reading one byte at a time from the source file and writing to the stego file
    while(fread(&remaining, 1, 1, encInfo -> fptr_src_image))
    {
        fwrite(&remaining, 1, 1, encInfo -> fptr_stego_image);
    }
    // Return success status
    return e_success;
}

/*Encode data from the secret file into the stego image.*/
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    // Variable to store each character from the secret file
    char ch;
    // Set the file pointer of the secret file to the beginning
    rewind(encInfo -> fptr_secret);
    // Continue encoding data from the secret file to the stego image until the end of the secret file is reached
    while(ftell(encInfo->fptr_secret) < encInfo->size_secret_file)
    {
        // Read one character from the secret file
        fread(&ch,1,1,encInfo->fptr_secret);
        encode_data_to_image( &ch,1,encInfo->fptr_src_image, encInfo->fptr_stego_image);
    }
    // Return success status
    return e_success;
}

/*Encode the size of the secret file into the stego image.*/
Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
    // Call the encode_size_to_image function to encode the size into the stego image
    if(encode_size_to_image( file_size,encInfo->fptr_src_image, encInfo->fptr_stego_image)==e_success);
    {
    // If the encoding is successful, return e_success
    return e_success; 
    }
}

/*Encode the file extension into the stego image.*/
Status encode_secret_file_extn( char *file_extn, EncodeInfo *encInfo)
{
    // Call the encode_data_to_image function to encode the file extension into the stego image
    if(encode_data_to_image( file_extn,strlen(file_extn),encInfo->fptr_src_image, encInfo->fptr_stego_image)==e_success);
    // If the encoding is successful, return e_success
    return e_success;    
}

/*Encode the size of the file extension into the stego image.*/
Status encode_extn_size(int extn_size, EncodeInfo *encInfo)
{   
    // Call the encode_size_to_image function to encode the extension size into the stego image
    if(encode_size_to_image( extn_size,encInfo->fptr_src_image, encInfo->fptr_stego_image)==e_success);
    // If the encoding is successful, return e_success
    return e_success;    
}

/*   Check if the stego image has enough capacity to encode the secret file.*/
Status check_capacity(EncodeInfo *encInfo)
{
    // Get the size of the secret file
    encInfo -> size_secret_file = get_file_size(encInfo->fptr_secret);
    /*Calculate the expected size of the stego image after encoding:
    size_of_beautyful.bmp=54+16+32+(strlen(extn_secret_file)+32+((secret_file_size)*8))*/
    // Check if the actual size of the stego image is greater than the expected size
    if(get_image_size_for_bmp(encInfo->fptr_src_image)>(54+16+32+(strlen(encInfo->extn_secret_file)+32+(encInfo -> size_secret_file*8))))
    {
        printf("INFO : Have Capacity \n");
        return e_success;
    }
    else
    {
        printf("INFO : NO Capacity \n");
        return e_failure;
    }

}

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/*
   Get the size of a file.
   Input:
   - fptr: Pointer to the FILE structure representing the file.
   Returns:
   - uint: Size of the file.
*/
uint get_file_size(FILE *fptr)
{
    // Move the file pointer to the end of the file
    fseek(fptr,0,SEEK_END);
    // Print an informational message indicating that the file size was obtained successfully
    printf("INFO : get file sixe successfully\n");
    // Return the size of the file
    return ftell(fptr);
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    // Set the file pointer of the source image to the beginning
    rewind(fptr_src_image);
    // Set the file pointer of the destination image to the beginning
    fseek(fptr_dest_image,0,SEEK_SET);
    // Create a buffer to store the BMP header (54 bytes)
    char header[54];
    // Read the BMP header from the source image
    fread(&header,1,54,fptr_src_image);
    // Write the BMP header to the destination image
    fwrite(header,1,54,fptr_dest_image);

    // Check for errors in reading or writing
    if(ferror(fptr_src_image)||ferror(fptr_dest_image))
    {
        // Return failure status if there is an error
        return e_failure;
    }
    // Return success status
    return e_success;
}

/*
 * Encode a magic string into the stego image.
 * Inputs:
 * - magic_string: Pointer to the string containing the magic string to be encoded.
 * - encInfo: Pointer to the EncodeInfo structure containing file pointers and other information.
 * Returns:
 * - Status: e_success if the operation is successful, otherwise e_failure.
 */
Status encode_magic_string( char *magic_string, EncodeInfo *encInfo)
{
    // Call the encode_data_to_image function to encode the magic string into the stego image.
    if(encode_data_to_image( magic_string,strlen(magic_string),encInfo->fptr_src_image, encInfo->fptr_stego_image)==e_success)
    // If the encoding is successful, return e_success
    return e_success;
}

/*
 * Encode data into the least significant bits (LSBs) of the stego image.
 * Input:
 * - data: Pointer to the data to be encoded.
 * - size: Size of the data to be encoded.
 * - fptr_src_image: FILE pointer for the source image.
 * - fptr_stego_image: FILE pointer for the stego image.
 * Returns:
 * - Status: e_success if the operation is successful, otherwise e_failure.
 */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    // Iterate through each byte of data to be encoded
    for(int i=0;i<size;i++)
    {
        // Create a buffer to store 8 bytes from the source image
        char image_buffer[8];
        // Read 8 bytes from the source image
        fread(&image_buffer,8,1,fptr_src_image);
        // Encode the current byte of data into the LSBs of the image buffer
        encode_byte_to_lsb(data[i], image_buffer);
        // Write the modified image buffer to the stego image
        fwrite(image_buffer,8,1,fptr_stego_image);
    }
    // Return success status
    return e_success;
}

/*
 * Encode data (size) into the least significant bits (LSBs) of the stego image.
 * Input:
 * - data: Size to be encoded.
 * - fptr_src_image: FILE pointer for the source image.
 * - fptr_stego_image: FILE pointer for the stego image.
 * Returns:
 * - Status: e_success if the operation is successful, otherwise e_failure.
 */
Status encode_size_to_image(int data, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    // Create a buffer to store 32 bytes from the source image
    char image_buffer[32];
    // Read 32 bytes from the source image
    fread(image_buffer, 32, 1, fptr_src_image);
    // Iterate through each bit of the data to be encoded
    for (int i = 0; i < 32; i++)
    {
        // Modify the corresponding bit in the image buffer with the current bit of the data
        image_buffer[i] = (image_buffer[i] & ~1) | ((data >> i) & 1);
    }
    // Write the modified image buffer to the stego image
    fwrite(image_buffer, 32, 1, fptr_stego_image);
    // Return success status
    return e_success;
}

/*
 * Encode a byte of data into the least significant bits (LSBs) of an image buffer.
 * Parameters:
 * - data: Byte of data to be encoded.
 * - image_buffer: Pointer to the image buffer where the data will be encoded.
 * Returns:
 * - Status: e_success if the operation is successful, otherwise e_failure.
 */
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    // Iterate through each bit of the byte to be encoded
    for (int i = 0; i < 8; i++)
    {
        // Modify the corresponding bit in the image buffer with the current bit of the data
        image_buffer[i] = (image_buffer[i] & ~1) | ((data >> i) & 1);
    }
    // Return success status
    return e_success;
}

/*
 * Open files for encoding.
 * Input:
 * - encInfo: Pointer to the EncodeInfo structure containing file names and FILE pointers.
 * Returns:
 * - Status: e_success if the files are successfully opened, otherwise e_failure.
 */
Status open_files(EncodeInfo *encInfo)
{
    // Open the source image file in read mode
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Check for errors in opening the source image file
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }

    // Open the secret file in read mode
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");

    // Check for errors in opening the secret file
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        // Close the source image file before returning failure
        fclose(encInfo->fptr_src_image);

        return e_failure;
    }

    // Open the stego image file in write mode
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");

    // Check for errors in opening the stego image file
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        // Close the source and secret files before returning failure
        fclose(encInfo->fptr_src_image);
        fclose(encInfo->fptr_secret);

        return e_failure;
    }

    // All files opened successfully, return e_success
    return e_success;
}
