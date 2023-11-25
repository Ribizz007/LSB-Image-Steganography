#include <stdio.h>
#include <string.h>
#include "decode.h"
#include"common.h"
#include "types.h"

Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo)
{
    //STEP 1 : Check argv[2] is bmp or not
    if(strcmp(strstr(argv[2],"."),".bmp")==0)
    {
        //if true -> store into src_image_fname,goto STEP 2
        decInfo->src_image_fname=argv[2];  
        //STEP 2 : Check argv[3] is passed or not
        if(argv[3]!=NULL)
        {
            //if true -> store into secret_fname
           strcpy(decInfo->secret_fname,argv[3]);                    
        }
        else
        {
            //else -> Store default name into secret_fname.//output 
            strcpy(decInfo->secret_fname,"output");
        }
    }
    else
    {
        //else -> print error and return e_failure.
        printf("source file is not bmp file\n");
        return e_failure;
    }
    //return e_success
    return e_success;    
}

//DECODE
Status do_decoding(DecodeInfo *decInfo)
{
    //STEP1 : call the open_files function
    if(open_src_files(decInfo)==e_success)
    {
        //if true print msg and goto STEP 2
        printf("INFO : Srcfiles  opened successfully\n");
        //STEP 2 : call decode_magic_string
        if(decode_magic_string(decInfo)==e_success)
        {
            //if true -> print msg and go to STEP 3
            printf("INFO : Decodeed magic string  successfully\n");
            //STEP 3 : call decode_secret_file_extn_size
            if(decode_secret_file_extn_size(decInfo)==e_success)
            {
                //if true -> print msg and go to STEP 4
                printf("INFO : Decoded secret file extension size successfully\n");
                //STEP 4 : Call decode_secret_file_extn
                if(decode_secret_file_extn(decInfo)==e_success)
                {                    
                    //if true -> print msg and go to STEP 5
                    printf("INFO : Decode secret file extntion successfully\n");
                    //STEP 5 : Call open_out_file to open the output file
                    if(open_out_file(decInfo)==e_success)
                    {
                        //if true -> print msg and go to STEP 6
                        printf("INFO : Opend output file successfull\n");
                        //STEP 6 : Call decode_secret_file_data_size to decode file size. 
                        if(decode_secret_file_size(decInfo)==e_success)
                        {
                            //if true -> print msg and go to STEP 7
                            printf("INFO : Decode secret file size successfully\n");
                            //STEP 6 : Call decode_secret_file_data to decode the data.
                            if(decode_secret_file_data(decInfo)==e_success)
                            {
                                //if true -> print msg.
                                printf("INFO : Decode secret file data successfully\n");
                            }
                                //else -> print error msg and return failure.
                            else
                            {
                                printf("INFO : Decode secret file data not successfully\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            //else -> print error msg and return failure.
                            printf("INFO : Decode secret file size not successfully\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        //else -> print error msg and return failure.
                        printf("INFO : Open output file not successfull\n");
                        return e_failure;
                    }
                }
                else
                {
                    //else -> print error msg and return failure.
                    printf("INFO : Decode secret file extn not execuited successfully\n");
                    return e_failure;
                }
            }
             else
            {
                //else -> print error msg and return failure.
                printf("INFO : Decode file extention size not successfully\n");
                return e_failure;
            }
        }
        else
        {
            //else -> print error msg and return failure.
            printf("INFO : Decode magic string not successfully\n");
            return e_failure;
        }
    }
    else
    {
        //else -> print error msg and return failure.
        printf("INFO : Srcfiles not open sucessfully\n");
        return e_failure;
    }

    printf("--------------------------------------\n");
    printf("INFO : Decoding Successfully Completed\n");
    printf("--------------------------------------\n");

}

/*Decode the secret data from the stego image*/
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    // Create a character array to store the decoded secret data
    char ch[decInfo->size_secret_file];
    // Initialize the array to zeros
    memset(ch, 0, sizeof(ch));
    // Decode the secret data from the stego image
    decode_image_to_data(decInfo,ch,decInfo->size_secret_file);
    //strcat(ch,"\0");
    printf("SECRET IS = %s\n",ch);
    // Write the decoded secret data to the secret file
    fwrite(ch,decInfo->size_secret_file,1,decInfo->fptr_secret);
// Add the End-of-File marker
    // if (fputc(EOF, decInfo->fptr_secret) == EOF) {
    //     perror("Error adding EOF");
    //     fclose(decInfo->fptr_secret);
    //     return e_failure;
    // }
    //fwrite(NULL, sizeof(int), 1, decInfo->fptr_secret);

    // Close the file
    fclose(decInfo->fptr_secret); 
    return e_success;
}

/*Decode the size of the secret file from the stego image.*/
Status  decode_secret_file_size(DecodeInfo *decInfo)
{
    // Create a character array to store the encoded size data
    char ch[4]={0};
    // Initialize the size of the secret file to 0
    decInfo->size_secret_file = 0;

    // Decode the size data from the stego image
    decode_image_to_data(decInfo,ch,4);

    // Assuming charArray is little-endian, combine the bytes to form an integer
    decInfo->size_secret_file |= ((int)ch[0] & 0xFF) << 0;
    decInfo->size_secret_file |= ((int)ch[1] & 0xFF) << 8;
    decInfo->size_secret_file |= ((int)ch[2] & 0xFF) << 16;
    decInfo->size_secret_file |= ((int)ch[3] & 0xFF) << 24;

    // Check if the size is valid (non-zero)
    if(decInfo->size_secret_file==0)
    {
        return e_failure;
    }
    // Return success status
    return e_success;
}

/*Open the output secret file for writing.*/
Status open_out_file(DecodeInfo *decInfo)
{
    // Concatenate the extension to the secret file name
    strcat(decInfo->secret_fname,decInfo->extn_secret_file);
    // Open the secret file for writing
    decInfo->fptr_secret=fopen(decInfo->secret_fname,"w");

   // Do ERROR handiling
    if (decInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open secretfile %s\n", decInfo->src_image_fname);

    	return e_failure;
    }
    // Print the name of the opened secret file
    printf("INFO : Opened secret file: %s\n", decInfo->secret_fname);
    // Return success status
    return e_success;
}
/*Decode the extension of the secret file from the stego image.*/
Status decode_secret_file_extn(  DecodeInfo *decInfo)
{
    // Create a character array to store the decoded extension data
    char ch[decInfo->extn_secret_file_size];

    // Decode the extension data from the stego image
    decode_image_to_data(decInfo,ch,4);  
    // Copy the decoded extension to the structure member
    strcpy(decInfo->extn_secret_file,ch);
    
    // Print the decoded extension
    printf("INFO : Decoded Extension: %s\n", decInfo->extn_secret_file);
    // Print the secret file name with the decoded extension
    printf("INFO : Secret File Name with Extension: %s\n", decInfo->secret_fname);

    // Return success status
    return e_success;
}

/* Decode the size of the extension of the secret file from the stego image.*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    // Create a character array to store the encoded size data
    char ch[4] = {0};

    // Initialize the size of the extension to 0
    decInfo->extn_secret_file_size = 0;

    // Decode the size data from the stego image
    decode_image_to_data(decInfo, ch, 4);

    // Assuming charArray is little-endian, combine the bytes to form an integer
    decInfo->extn_secret_file_size |= ((int)ch[0] & 0xFF) << 0;
    decInfo->extn_secret_file_size |= ((int)ch[1] & 0xFF) << 8;
    decInfo->extn_secret_file_size |= ((int)ch[2] & 0xFF) << 16;
    decInfo->extn_secret_file_size |= ((int)ch[3] & 0xFF) << 24;

    // Check if the size is valid (non-zero)
    if (decInfo->extn_secret_file_size == 0)
    {
        return e_failure;
    }

    // Print the decoded extension size
    printf("INFO : Extension Size: %d\n", decInfo->extn_secret_file_size);

    // Return success status
    return e_success;
}



//Decoding magic string
Status decode_magic_string( DecodeInfo *decInfo)
{
    // Declare  an array to store two characters
    char ch[3]={0};

    // Set the file pointer to the 54th byte in the file
    fseek(decInfo->fptr_src_image,54,SEEK_SET);
   
    //call decode image to data
    decode_image_to_data(decInfo,ch,2);
    // ch[2]='\0';
    // printf("%c\n",ch[2]);

    // Compare the formed characters with a predefined magic string
    if(strcmp(ch,MAGIC_STRING)==0)
    {
        // If the comparison is successful, return success status
        return e_success;
    }
    // If the comparison is not successful, return failure status
    return e_failure;

}

/*Decode image data from the stego image and store it in the provided data array.*/
Status decode_image_to_data(DecodeInfo *decInfo,char *data, int size)
{
    // Create an image buffer to store the encoded image data
    char image_buffer[size*8];
    // Read the image data from the stego image
    fread(&image_buffer,size*8,1,decInfo->fptr_src_image);
    
    // Decode each byte of image data using decode_byte_from_lsb and store in the data array
    for(int i=0;i<size;i++)
    {        
        decode_byte_from_lsb(&image_buffer[i*8],&data[i]);     
    }
    // Print the decoded data to the console
    // printf("Decoded Data: %s\n", data);
    // Return success status
    return e_success;
}

/*Decode a byte from the least significant bits (LSB) of the image byte array.*/
Status decode_byte_from_lsb(char *img, char *data)
{
    // Loop through each bit of the image byte array
    for (int i = 0; i < 8; i++)
    {
        // Extract the LSB of each bit from the image byte and store it in the data byte
        *data = *data | ((img[i] & 0x01) << i);
    }
    // Return success status
    return e_success;
}


// Open the given src file
Status open_src_files(DecodeInfo *decInfo)
{
    //open Src file
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r");
    // Do ERROR handiling
    if (decInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_image_fname);

    	return e_failure;
    }
    // No failure return e_success
    return e_success;

}
