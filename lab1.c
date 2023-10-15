#include <stdio.h>
#include <stdlib.h>

void writeToFile(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    unsigned char data[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    fwrite(data, sizeof(unsigned char), sizeof(data), file);

    fclose(file);
}

void readFromFile(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    unsigned char buffer;
    while (fread(&buffer, sizeof(unsigned char), 1, file) == 1) {
        printf("\n");
        printf("Bytes: %u ", buffer);
        printf("_p: %p ", file->_p);
        printf("_r: %d ", file->_r);
        printf("_w: %d ", file->_w);
        printf("_flags: %d ", file->_flags);
        printf("_file: %d ", file->_file);
        printf("_offset: %lld ", file->_offset);
        printf("_blksize: %d ", file->_blksize);
    }

    fclose(file);
}

void readFromOffset(const char *filename, long offset, size_t count) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fseek(file, offset, SEEK_SET);

    unsigned char *buffer = malloc(count * sizeof(unsigned char));
    if (buffer == NULL){
        printf("Memmory does not allocated\n");
        return;
    }
    fread(buffer, sizeof(unsigned char), count, file);

    printf("\nBuffer: ");
    for (int i = 0; i < count; i++) {
        printf("%u ", buffer[i]);
    }
    printf("\n");

    free(buffer);
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Enter file's path.\n");
        return 1;
    }

    const char *filename = argv[1];

    writeToFile(filename);
    readFromFile(filename);
    readFromOffset(filename, 3, 4);

    return 0;
}
