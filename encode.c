#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

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
* Goes to the end of the file
* Returns the index
*/
uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr);
} 
/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status read_and_validate_encode_args(char **argv, EncodeInfo *encInfo)
{
    if(strcmp(strchr(argv[2] , '.'), ".bmp") == 0)
    {
        printf("Recieved proper bmp file\n");
        encInfo -> src_image_fname = argv[2];
    }
    else
    {
        printf("Proper bmp file not recieved!\n");
        return e_failure;
    }
    if(strchr(argv[3], '.') != NULL && strcmp(strchr(argv[3], '.'), ".txt") == 0)
    {
        printf("Recieved proper txt file\n");
        encInfo -> secret_fname = argv[3];
    }
    else
    {
        printf("Proper txt file not recieved!\n");
        return e_failure;
    }
    if(argv[4] != NULL)
    {
        encInfo -> stego_image_fname = argv[4];
    }
    else
    {
        printf("Output filename not given, So creating one named stego.bmp\n");
        encInfo -> stego_image_fname = "stego.bmp";
    }
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    printf("Lets open the file in required mode\n");
    if(open_files(encInfo) == e_success)
    {
        printf("Required file opened in required mode successfully!\n");
    }
    else
    {
        printf("File opening failed!\n");
        return e_failure;
    }
    printf("Check if the file %s is suitable for encoding...\n", encInfo -> src_image_fname);
    if(check_capacity(encInfo) == e_success)
    {
        printf("The file %s is suitable for encoding!\n", encInfo -> src_image_fname);
    }
    else
    {
        printf("The file %s is not suitable for encoding!\n", encInfo -> src_image_fname);
        return e_failure;
    }
    if(copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
    {
        printf("Copying done!\n");
    }
    else
    {
        printf("Copying failed\n");
        return e_failure;
    }
    printf("Started encoding the magic string...\n");
    if(encode_magic_string(MAGIC_STRING, encInfo) == e_success)
    {
        printf("Magic string encoded successfully\n");
    }
    else
    {
        printf("Failed to encode magic string!\n");
        return e_failure;
    }

    // Encode extension size
    strcpy(encInfo -> extn_secret_file, strchr(encInfo -> secret_fname, '.'));
    if(encode_extension_size(4, encInfo) == e_success)
    {
        printf("File extension size encoded successfully\n");
    }
    else
    {
        printf("Failed to encode file extension size\n");
        return e_failure;
    }

    // Encode secret file extension
    if(encode_secret_file_extn(encInfo -> extn_secret_file, encInfo) == e_success)
    {
        printf("File extension encoded successfully\n");
    }
    else
    {
        printf("Failed to encode file extension\n");
        return e_failure;
    }

    // Encode secret file size
    if(encode_secret_file_size(encInfo -> size_secret_file, encInfo) == e_success)
    {
        printf("File size encoded successfully\n");
        //printf("Image capacity : %d\n", encInfo -> image_capacity);
        //printf("Secret file size : %ld\n", encInfo -> size_secret_file);
    }
    else
    {
        printf("Failed to encode file size\n");
        return e_failure;
    }

    //Encode secret data
    if(encode_secret_file_data(encInfo) == e_success)
    {
        printf("File data encoded successfully\n");
    }
    else
    {
        printf("Failed to encode file data\n");
        return e_failure;
    }

    //Copy remaining data
    if(copy_remaining_img_data(encInfo ->fptr_src_image, encInfo -> fptr_stego_image) == e_success)
    {
        printf("Remaining data copied to destination\n");
    }
    else
    {
        printf("Failed to copy remaining datat to destination\n");
        return e_failure;
    }
    return e_success;
}

//2 + 4 + 4 + 4 are the bytes required to store the encode contents
Status check_capacity(EncodeInfo *encInfo)
{
    encInfo -> image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);
    encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);    
    if(encInfo -> image_capacity > (2 + 4 + 4 + 4 + encInfo -> size_secret_file) * 8)      
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    fseek(fptr_src_image, 0, SEEK_SET);
    char str[54];
    fread(str, 54, 1, fptr_src_image);
    fwrite(str, 54, 1, fptr_dest_image);
    return e_success;
}

Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    encode_data_to_image(magic_string, 2, encInfo);
    return e_success;
}

Status encode_data_to_image(char *data, int size, EncodeInfo *encInfo)
{
    for (int i = 0; i < size; i++)
    {
        fread(encInfo -> image_data, 8, 1, encInfo -> fptr_src_image);
        encode_byte_to_lsb(data[i], encInfo -> image_data);
        fwrite(encInfo -> image_data, 8, 1, encInfo -> fptr_stego_image);
    }
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for (int i = 0; i < 8; i++)
    {
        image_buffer[i] = (image_buffer[i] & 0xFE) | ((data >> (7 - i)) & 1);
    }
}

Status encode_extension_size(int size, EncodeInfo *encInfo)
{
    encode_integer_to_lsb(size, encInfo);
    return e_success;
}

Status encode_integer_to_lsb(int size, EncodeInfo *encInfo)
{
    char str[32];
    fread(str, 32, 1, encInfo -> fptr_src_image);
    for (int i = 0 ; i < 32; i++)
    {
        str[i] = (str[i] & 0xFE) | ((size >> (31 - i)) & 1);
    }
    fwrite(str, 32, 1, encInfo -> fptr_stego_image);
}

Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    encode_data_to_image(file_extn, 4, encInfo);
    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    encode_integer_to_lsb(file_size, encInfo);
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char str[encInfo -> size_secret_file];
    fseek(encInfo -> fptr_secret, SEEK_SET, 0);
    fread(str, encInfo -> size_secret_file, 1, encInfo -> fptr_secret);
    encode_data_to_image(str, encInfo -> size_secret_file, encInfo);
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while(fread(&ch, 1, 1, fptr_src) == 1){
        fwrite(&ch, 1, 1, fptr_dest);
    }
    return e_success;
}