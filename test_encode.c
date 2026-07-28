#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char **argv)
{
    if(check_operation_type(argv) == e_encode){
        printf("The operation selected is Encode!\n");
        if(argc < 4){
            printf("Encoding requires atleast 2 files! The source image and the secret file!");
            return -1;
        }
        EncodeInfo steganography;
        if(read_and_validate_encode_args(argv, &steganography) == e_success)
        {
            printf("Read and validated encode arguments successfully\n");
            printf("<-----------------Encoding------------->\n");
            if(do_encoding(&steganography) == e_success)
            {
                printf("Encoding done successfully!\n");
            }
            else
            {
                printf("Encoding Failed!\n");
            }
        }
        else
        {
            printf("Invalid encode arguments provided!\n");
            return -1;
        }
    }
    else if(check_operation_type(argv) == e_decode){
        if(argc < 3){
            printf("Atleast 1 file required to decode : The image to decode\n");
            return -1;
        }
        DecodeInfo Decode;
        printf("The operation selected is decode!\n");
        if(read_and_validate_decode_args(argv , &Decode) == e_success)
        {
            printf("Arguments read and validated sucessfully!\n");
            printf("<-----------------Decoding------------->\n");
            if(do_decoding(&Decode) == e_success)
            {
                printf("Decoding Done successfully\n");
            }
            else
            {
                printf("Failed to decode!\n");
            }
        }
    }
    else {
        printf("Invalid selection !\n");
        printf("For encode : ./a.out -e beautiful.bmp secret.txt [stegno.bmp]\n");
        printf("For decode : ./a.out -d stegno.bmp [output.txt]\n");
    }

    return 0;
}
    
    

OperationType check_operation_type(char **argv){
    if(strcmp(argv[1], "-e") == 0){
        return e_encode;
    }
    else if (strcmp(argv[1], "-d") == 0){
        return e_decode;
    }
    else {
        return e_unsupported;
    }
}

