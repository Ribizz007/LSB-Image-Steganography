#include <stdio.h>
#include <string.h>
#include "encode.h"
#include"decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    // Check if there are command line arguments
    if (argc > 1)
    {
        // Check the operation type based on the command line arguments
        OperationType operationType = check_operation_type(argv);
        // If the operation type is encoding
        if (operationType == e_encode)
        {
            // Start encoding part
            if (argc >= 4)
            {
                printf("INFO : Selected encoding, Encoding Started.\n");

                // Initialize EncodeInfo structure
                EncodeInfo encInfo;

                // Read and validate encoding arguments
                if (read_and_validate_encode_args(argv, &encInfo) == e_success)
                {
                    printf("INFO : Read and Validation is successfully executed.\n");

                    // Perform encoding
                    do_encoding(&encInfo);
                }
                else
                {
                    printf("INFO : Read and Validation is not successfully executed.\n");
                    return 0;
                }
            }
            else
            {
                printf("INFO : For Encoding, please pass a minimum of 4 arguments like ./a.out -e source_image secret_data_file\n");
                return 0;
            }
        }
        // If the operation type is decoding
        else if (operationType == e_decode)
        {
            // Start decoding part
            if (argc >= 3)
            {
                printf("INFO : Selected decoding, Decoding started.\n");

                // Initialize DecodeInfo structure
                DecodeInfo decInfo;

                // Read and validate decoding arguments
                if (read_and_validate_decode_args(argv, &decInfo) == e_success)
                {
                    // Perform decoding
                    do_decoding(&decInfo);
                }
            }
            else
            {
                printf("INFO : For Decoding, please pass a minimum of 3 arguments like ./a.out -e source_image\n");
            }
        }
        // If the operation type is unsupported
        else
        {
            // Print an error message
            printf("ERROR : Unsupported operation type.\n");
        }
    }
    else
    {
        // Print information about the required number of arguments
        printf("INFO : Please pass valid arguments.\nINFO : Encoding - minimum 4 arguments.\nINFO : Decoding - minimum 3 arguments.\n");
    }

    // Return status
    return 0;
}

/*Check the operation type based on command line arguments.*/
OperationType check_operation_type(char *argv[])
{

    if (strcmp(argv[1],"-e")==0)
    {
        //STEP 1 : check arvg[1] is -e or not
        //         if yes -> return e_encode
        //         else -> goto STEP 2:
        return e_encode;
    }
    else if (strcmp(argv[1],"-d")==0)
    {
        //STEP 2 : check argv[1] is -d or not
        //         if yes -> return e_decode
        //         else -> goto STEP 3:
        return e_decode;
    }
    else 
    {
        //step 3 : return e_unsupported
        return e_unsupported;
    }    
}

