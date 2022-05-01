/* 1st May 2022 (07:22 A.M) */


/* General Specifications:
 *
 * Brainfuck operates on an array of memory cells, each initially set to zero.
 * In the original implementation, the array was 30,000 cells long.
 * There is a pointer, initially pointing to the first memory cell.
 *
 * The commands are:
 * | Command | Description                                              |
 * |---------|----------------------------------------------------------|
 * |    >    | Move the pointer to the right                            |
 * |    <    | Move the pointer to the left                             |
 * |    +    | Increment the memory cell at the pointer                 |
 * |    -    | Decrement the memory cell at the pointer                 |
 * |    .    | Output the character signified by the cell at the pointer|
 * |    ,    | Input a character and store it in the cell at the pointer|
 * |    [    | Jump past the matching ] if the cell at the pointer is 0 |
 * |    ]    | Jump back to the matching [ if the cell at the pointer
 *             is nonzero.                                              |
 *
 * Source: https://esolangs.org/wiki/Brainfuck
 */


#include <stdio.h>
#include <stdlib.h>


#define TOTAL_CELLS 30000


unsigned long get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    unsigned long size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    return size;
}


int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage %s <file_name>\n", argv[0]);
        return 0;
    }

    char MEM_CELLS[TOTAL_CELLS] = {0};

    const char *file_name = argv[1];
    FILE *fptr = fopen(file_name, "r");
 
    if (fptr == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", argv[1]);
        return -1;
    }

    unsigned long file_size = get_file_size(fptr);

    char *buffer;
    buffer = malloc(file_size * sizeof(char));
    
    if (buffer == NULL) {
        fputs("Failed to allocate memory.", stderr);
        fclose(fptr);
        return -1;
    }

    fread(buffer, file_size, 1, fptr);
    fclose(fptr);

    unsigned short sp = 0;
    unsigned long pc = 0;
    unsigned long before_match, after_match;

    for (; pc < file_size; pc++) {
        switch (buffer[pc]) {
            case '>': sp++; break;
            case '<': sp--; break;
            case '+': MEM_CELLS[sp]++; break;
            case '-': MEM_CELLS[sp]--; break;
            case '.': printf("%c", MEM_CELLS[sp]); break;
            case ',': scanf("%c", &MEM_CELLS[sp]); break;
            case '[':
                before_match = pc;

                if (MEM_CELLS[sp] == 0) {
                    pc = after_match; 
                }
                break;
            case ']':
                after_match = pc;

                if (MEM_CELLS[sp] != 0) {
                    pc = before_match;
                }
                break;
            default:
                break;
        }

        if (sp < 0 || sp >= TOTAL_CELLS) {
            fprintf(stderr, "Invalid memory access: %d\n", sp);
            return -1;
        }
    }

    free(buffer);
    return 0;
}
