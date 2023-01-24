#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

#define BUFFER_SIZE 1024
int main(int argc, char *argv[]) { //argv[1]-file, argv[2,3]-int
    int m = atoi(argv[3]); // m characters in a fragment
    int n = atoi(argv[2]); //n fragments
    char buff[BUFFER_SIZE]; 
    FILE *file = fopen(argv[1], "r");
    if(file == NULL) {
        perror("Error opening file\n");
        return EXIT_FAILURE;
    }
    int h = fread(buff, 1, BUFFER_SIZE, file); //h - num of characters in file
    srandom(time(0));
    for (int i=0; i<n; i++) {
        printf(">");
        int pos = random() % (h-m-1);
        for (int j = 0; j<m; j++) {
            if (buff[pos] == '\n') pos++;
            else if (buff[pos] == '\r') pos+=2;
            printf("%c", buff[pos]);
            pos++;
        }
        printf("<\n");
    }
    return EXIT_SUCCESS;
}