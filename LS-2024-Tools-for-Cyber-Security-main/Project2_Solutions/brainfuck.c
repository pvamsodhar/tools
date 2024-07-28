#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 30000

void run(char* code) {
    char memory[MEMORY_SIZE] = {0};
    char* ptr = memory;

    char* ip = code;

    while (*ip) {
        // fprintf(stdout, "Executing %c\n", *ip);
        // printf("%s\n", memory);
        switch (*ip) {
            case '>':
                ptr++;
                break;
            case '<':
                ptr--;
                break;
            case '+':
                (*ptr)++;
                break;
            case '-':
                (*ptr)--;
                break;
            case '.':
                fprintf(stdout, "%c", *ptr);
                break;
            case ',':
                scanf("%c", ptr);
                break;
            case '[':
                if (*ptr == 0) {
                    int loops = 1;

                    while (loops > 0) {
                        ip++;
                        if (*ip == '[') loops++;
                        if (*ip == ']') loops--;
                    }
                }
                break;
            case ']':
                if (*ptr) { 
                    int loops = 1;
                    while (loops > 0) {
                        ip--;
                        if (*ip == '[') loops--;
                        if (*ip == ']') loops++;
                    }
                }
        }
        ip++;
    }

}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s \"<brainfuck-code>\"\n", argv[0]);
    }

    run(argv[1]);
}