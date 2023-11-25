#ifndef DECODE_H
#define DECODE_H
#include "types.h"  // Contains user defined types


#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
  /* Source Image info */
  char *src_image_fname;    //store argv[2],file name
  FILE *fptr_src_image;     //file pointer for source image

  /* Secret File Info */
  char secret_fname[50];       //store avg[3],secret file name    
  FILE *fptr_secret;        //File pointer for secret_data_file
  char extn_secret_file[MAX_FILE_SUFFIX];     //exten. of sectrt file
  int extn_secret_file_size;
  int size_secret_file;    //size of the secret filoe

} DecodeInfo;

/* Decoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the Decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p  files */
Status open_src_files(DecodeInfo *decInfo);

/* Check  Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* decode secret  extenstion size*/
Status decode_secret_file_extn_size( DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn(  DecodeInfo *decInfo);

Status open_out_file(DecodeInfo *decInfo);

/* decode secret file size*/
Status decode_secret_file_size(DecodeInfo *decInfo);

/* decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode function, which does the real decoding */
Status decode_image_to_data(DecodeInfo *decInfo,char *data, int size);//, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Decode a byte into LSB of image data array */
Status decode_byte_from_lsb(char *img, char *data);


#endif