#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_VALUES 1000000

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_filename>\n", argv[0]);
        return 1;
    }

    const char *input_filename = argv[1];
    printf("Trying to open '%s'\n", input_filename);

    FILE *infile = fopen(input_filename, "rb");
    if (infile == NULL) {
        perror("Error opening input file");
        return 1;
    }
    
    char output_filename[256];
    snprintf(output_filename, sizeof(output_filename), "%s_parsed.txt", strtok(argv[1], "."));


    FILE *outfile = fopen(output_filename, "w");
    if (outfile == NULL) {
        perror("Error opening output file");
        fclose(infile);
        return 1;
    }

    uint64_t value;
    uint64_t *seen_values = malloc(MAX_VALUES * sizeof(uint64_t)); 
    if (seen_values == NULL) {
        perror("Memory allocation error");
        fclose(infile);
        fclose(outfile);
        return 1;
    }

    size_t seen_count = 0;
    int is_duplicate;

    while (fread(&value, sizeof(value), 1, infile) == 1) {
        is_duplicate = 0;

        for (size_t i = 0; i < seen_count; i++) {
            if (seen_values[i] == value) {
                is_duplicate = 1;
                break;
            }
        }

        if (!is_duplicate) {
            seen_values[seen_count++] = value;
            fprintf(outfile, "%lx\n", value);
        }
    }

    if (ferror(infile)) {
        perror("Error reading input file");
    }

    printf("Parsed values written to file '%s'\n", output_filename);

    free(seen_values);
    fclose(infile);
    fclose(outfile);

    return 0;
}
