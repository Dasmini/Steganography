#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"

int main(int argc, char **argv)
{
   if(argc >= 4){
        if(check_operation_type(argv) == e_encode){
            printf("The operation selected is Encode!\n");
            EncodeInfo steganography;
            if(read_and_validate_encode_args(argv, &steganography) == e_success)
            {
                printf("Read and validated encode arguments successfully\n");
                printf("<-----------------Encoding------------->");
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
            printf("The operation selected is decode!\n");
        }
        else {
            printf("Invalid selection !\n");
            printf("For encode : ./a.out -e beautiful.bmp secret.txt [stegno.bmp]\n");
            printf("For decode : ./a.out -d stegno.bmp [output.txt]\n");
        }
    }
    else{
        printf("Atleast 3 inputs should be given!\n");
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

