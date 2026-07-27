#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defines types

#define MAX_SECRET_BUFF_SIZE 1
#define MAX_IMAGE_BUFF_SIZE (MAX_SECRET_BUFF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /*Source/stego image info*/
    char *stego_image_fname;
    FILE *fptr_stego_image;
    char image_data[MAX_IMAGE_BUFF_SIZE];

    /*Output file info*/
    char *output_fname;
    char extn_output[MAX_FILE_SUFFIX];
    char output_data[MAX_SECRET_BUFF_SIZE];
    FILE *fptr_output;
    uint size_exten_output;
    long size_output_file;

} DecodeInfo;

/* Decoding function prototype */

/*Read and validate decode arguments*/
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Open files*/
Status open_decode_files(DecodeInfo *decInfo);

/*Do Decoding*/
Status do_decoding(DecodeInfo *decInfo);

/*Check Magic string*/
Status check_magic_string(char *magic_string, DecodeInfo *decInfo);

/* Decode data from image*/
Status decode_info_from_image(char *magic_string, int size, DecodeInfo *decInfo);

/* Decode char byte from lsb*/
Status decode_data_from_lsb(char *data, DecodeInfo *decInfo);

/* Decode output file extension size*/
Status decode_output_extn_size(DecodeInfo *decInfo);

/* Decode lsb from byte for integer*/
uint decode_lsb_for_integer(int size, DecodeInfo *decInfo);

/* Decode output file extention*/
Status decode_output_file_extn(DecodeInfo *decInfo);

/* Decode size of output file*/
Status decode_output_file_size(DecodeInfo *decInfo);

/* Decode secret data to output file*/
Status decode_output_data(DecodeInfo *decInfo);

/* Close the files*/
Status close_decode_files(FILE *fptr_stego, FILE *fptr_output_file);
#endif