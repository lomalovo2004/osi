#include <stdio.h>

int copy_file(const char *FileName_input, const char *FileName_output) {
    FILE *file_input = fopen(FileName_input, "rb");
    if (file_input == NULL) {
        printf("Error opening input file.\n");
        return 1;
    }

    FILE *file_output = fopen(FileName_output, "wb");
    if (file_output == NULL) {
        printf("Error opening output file.\n");
        fclose(file_input);
        return 1;
    }

    char buffer[BUFSIZ];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, sizeof(char), *buffer, file_input)) > 0) {
        fwrite(buffer, sizeof(char), sizeof(buffer), file_output);
    }

    fclose(file_input);
    fclose(file_output);

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <file_input.txt> <file_output.txt>\n", argv[0]);
        return 1;
    }

    const char *FileName_input = argv[1];
    const char *FileName_output = argv[2];

    int result = copy_file(FileName_input, FileName_output);
    if (result == 0) {
        printf("File copied successfully.\n");
    }

    return result;
}
