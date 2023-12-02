#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void matching (FILE* input, unsigned long int *res, unsigned long int hex)
{
    unsigned long int num = 0;
    fread(&num, sizeof(int), 1, input);
    do 
    {
        if (hex ^ (num == 0))
        {
            (*res)++;
        } 
    } while (fread(&num, sizeof(int), 1, input));
}

int num_check (char* str, unsigned long int *res)
{

    char* end_ptr;

    if ((*res = strtol(str, &end_ptr, 16)) < 0)
    {
        return 1;
    }
    if (end_ptr == str + strlen(str)) {
        return 0;
    
    }else{return 1;}
}

void sum_bytes (FILE* input, unsigned long long int *res)
{
    unsigned char c;
    while (fread(&c, sizeof(char), 1, input))
    {
        *res = (*res ^ c);
    }
}

void sum_bytes4 (FILE* input, unsigned long long int *res)
{

    unsigned long int bytes;
    unsigned long int distance = 0;

    while (fread(&bytes, sizeof(int), 1, input))
    {
        distance += 4;
        *res = (*res ^ bytes);
    }
    fseek(input, distance, SEEK_SET);
    bytes = 0;
    unsigned char c;
    while (fread(&c, sizeof(char), 1, input))
    {
        bytes = bytes ^ c;
    }
    *res = (*res ^ bytes);
}

int xor8 (char* file)
{
    FILE* input = fopen(file, "rb");
    if (input == NULL)
    {
        printf("Error opening input file\n");
        return 1;
    }
    unsigned long long int res = 0;
    sum_bytes(input, &res);
    printf("XOR8 Result: %llu\n", res);
    fclose(input);
    return 1;
}

int xor32 (char* file)
{
    FILE* input = fopen(file, "rb");
    if (input == NULL)
    {
        printf("Error opening input file\n");
        return 1;
    }
    unsigned long long int res = 0;
    sum_bytes4 (input, &res);
    printf("XOR32 Result: %llu\n", res);
    fclose(input);
    return 1;
}

int mask_hex (char* file, char* hex)
{
    FILE* input = fopen(file, "rb");
    if (input == NULL)
    {
        printf("Error opening input file\n");
        return 1;
    }
    unsigned long int hex_num = 0;
    if (num_check(hex, &hex_num) != 0)
    {
        printf("invalid number\n");
        return 0;
    }
    unsigned long int meeted_amount = 0;
    matching(input, &meeted_amount, hex_num);
    printf("Mask result: %lu\n", meeted_amount);
    fclose(input);
    return 1;
}

int main(int argc, char** argv){
    if (argc == 3 || argc == 4){
        if (!strcmp(argv[2], "xor8")){
            return xor8(argv[1]);
        }
        if (!strcmp(argv[2], "xor32")){
            return xor32(argv[1]);
        }
        if (argv[3] == NULL){
            printf("Use: ./lab4 inputfile.txt mask <number>\n");
            
        } else if(!strcmp(argv[2], "mask")){
            return mask_hex(argv[1], argv[3]);
        }
    }
    else{
        printf("Use: ./lab4 inputfile.txt <flag>\n");
    }
}