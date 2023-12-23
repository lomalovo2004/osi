#include <stdio.h>
#include <stdlib.h>

void write_to_file(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error opening file.\n");
        fclose(file);
        return;
    }

    unsigned char data[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    fwrite(data, sizeof(unsigned char), sizeof(data), file);

    fclose(file);
}

void read_from_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file.\n");
        fclose(file);
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

void read_from_offset(const char *filename, long offset, size_t count) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file.\n");
        fclose(file);
        return;
    }

    fseek(file, offset, SEEK_SET);

    unsigned char *buffer = malloc(count * sizeof(unsigned char));
    if (buffer == NULL){
        printf("Memmory does not allocated\n");
        fclose(file);
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

    write_to_file(filename);
    read_from_file(filename);
    read_from_offset(filename, 3, 4);

    return 0;
}
