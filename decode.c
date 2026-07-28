#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if(strchr(argv[2], '.') != NULL && strcmp(strchr(argv[2], '.'), ".bmp") == 0)
    {
        printf("Proper bmp file recieved!\n");
        decInfo -> stego_image_fname = argv[2];
    }
    else
    {
        printf("Proper bmp file not recieved!\n");
        return e_failure;
    }
    if(argv[3] != NULL && strchr(argv[3], '.') != NULL)
    {
        printf("Output file recieved\n");
        decInfo -> output_fname = argv[3];
    }
    else
    {
        decInfo -> output_fname = NULL;
        printf("Output file not provided, will be created during decoding!'\n");
        
    }
    return e_success;
}

Status open_decode_file(DecodeInfo *decInfo)
{
    decInfo -> fptr_stego_image = fopen(decInfo -> stego_image_fname, "r");
    if(decInfo -> fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR : Unable to open file %s\n", decInfo -> stego_image_fname);
        return e_failure;
    }
    
    return e_success;
}

Status open_output_file(DecodeInfo *decInfo)
{
    if(decInfo -> output_fname == NULL)
    {
        char output_name[30] = "output";
        strcat(output_name, decInfo -> extn_output);
        decInfo -> output_fname = output_name;
    }

    decInfo -> fptr_output = fopen(decInfo -> output_fname, "w");
    if(decInfo -> fptr_output == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR : Unable to open file %s\n", decInfo -> output_fname);
        return e_failure;
    }
    return e_success;
}

Status check_magic_string(char *magic_string, DecodeInfo *decInfo)
{
    if(fseek(decInfo -> fptr_stego_image, 54, SEEK_SET) == 0)
    {
        if(decode_info_from_image(magic_string, 2, decInfo) == e_success)
        {
             return e_success;
        }   
    }
    return e_failure;
}

Status decode_info_from_image(char *data, int size, DecodeInfo *decInfo)
{
    for (int i = 0; i < size; i++)
    {
        fread(decInfo -> image_data, 8, 1, decInfo -> fptr_stego_image);
        decode_data_from_lsb(decInfo -> image_data, decInfo);
        if(data[i] != decInfo -> output_data[0])
        {
            printf("Required information not found\n");
            return e_failure;
        }
    }
    return e_success;
}

Status decode_data_from_lsb(char *data, DecodeInfo *decInfo)
{
    // get lsb from the image data and frame the character in output data
    decInfo -> output_data[0] = 0;
    for (int i = 0; i < 8; i++)
    {
        decInfo -> output_data[0] = decInfo -> output_data[0] | ((data[i] & 1) << (7 - i));
    }
}

Status decode_output_extn_size(DecodeInfo *decInfo)
{
    //size = 32, because bits of 1 integer is stored across 32 bytes!
    decInfo -> size_exten_output = decode_lsb_for_integer(32, decInfo);
    return e_success;
}

uint decode_lsb_for_integer(int size, DecodeInfo *decInfo)
{
    char str[size];
    uint result = 0;
    fread(str, size, 1, decInfo -> fptr_stego_image);
    for (int i = 0; i < size; i++)
    {
        result = result | ((str[i] & 1) << (size - 1 - i));
    }
    return result;
}

Status decode_output_file_extn(DecodeInfo *decInfo)
{
    for(int i = 0; i < decInfo -> size_exten_output; i++)
    {
        fread(decInfo -> image_data, 8, 1, decInfo -> fptr_stego_image);
        decode_data_from_lsb(decInfo -> image_data, decInfo);
        decInfo -> extn_output[i] = decInfo -> output_data[0];
    }
    decInfo -> extn_output[decInfo -> size_exten_output] = '\0';
    return e_success;
    
}

Status decode_output_file_size(DecodeInfo *decInfo)
{
    decInfo -> size_output_file = decode_lsb_for_integer(32, decInfo);
    return e_success;
}

Status decode_output_data(DecodeInfo *decInfo)
{
    if(open_output_file(decInfo) == e_failure)
    {
        printf("Output file creating and opening failed!\n");
        return e_failure;
    }

    for (int i = 0; i < decInfo -> size_output_file; i++)
    {
        fread(decInfo -> image_data, 8, 1, decInfo -> fptr_stego_image);
        decode_data_from_lsb(decInfo -> image_data, decInfo);
        if(fputc(decInfo ->output_data[0], decInfo ->fptr_output) == -1)
        {
            return e_failure;
        }
    }
    return e_success;
}

Status close_decode_files(FILE *fptr_stego, FILE *fptr_output_file)
{
    if(fclose(fptr_stego) == 0 && fclose(fptr_output_file) == 0)
    {
        return e_success;
    }
    return e_failure;
}

Status do_decoding(DecodeInfo *decInfo)
{
    if(open_decode_file(decInfo) == e_success)
    {
        printf("Files opened successfully!\n");
    }
    else
    {
        printf("Failed to open files!\n");
        return e_failure;
    }

    // To check the existance of magic strig
    if(check_magic_string(MAGIC_STRING, decInfo) == e_success)
    {
        printf("Magic string found!\n");
    }
    else
    {
        printf("Magic string not found!\n");
        return e_failure;
    }

    // To find the extention size of the output file
    if(decode_output_extn_size(decInfo) == e_success)
    {
        printf("Output file extension size fetched successfully\n");
    }
    else
    {
        printf("Failed to fetch output file extension\n");
        return e_failure;
    }

    if(decode_output_file_extn(decInfo) == e_success)
    {
        printf("Output file extention fetched successfully\n");
    }
    else
    {
        printf("Failed to fetch file extension\n");
        return e_failure;
    }

    if(decode_output_file_size(decInfo) == e_success)
    {
        printf("Output file size fetched successfully\n");
    }
    else
    {
        printf("Failed to fetch output file size\n");
        return e_failure;
    }

    if(decode_output_data(decInfo) == e_success)
    {
        printf("Secret data fetched successfully!\n");
    }
    else
    {
        printf("Failed to fetch secret data\n");
        return e_failure;
    }

    if(close_decode_files(decInfo -> fptr_stego_image, decInfo -> fptr_output) == e_success)
    {
        printf("Files closed successfully\n");
    }
    else
    {
        printf("Failed to close files!\n");
        return e_failure;
    }
    return e_success;
}