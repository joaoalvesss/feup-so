#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>


#define BUFFER_SIZE 1024
int main(int argc, char *argv[]) {
    char txtfile[BUFFER_SIZE];
    char epubfile[BUFFER_SIZE];
    char epub[BUFFER_SIZE] = {'.','e','p','u','b','\0'};
    int n;
    int pid = 0;
    //create n+1 processes
    int i = 0;
    for(i; i < argc-1; i++)
        if(pid == 0){ pid = fork(); n=i;}
    if (pid==0) n=i;
    //each process converts a file, execpt for the first process created, which zips the files
    if (n!=0) { //n processes get in
    snprintf(txtfile, BUFFER_SIZE, "%s", argv[n]);
    int h=0;
    while(txtfile[h] != '.'){
        epubfile[h]=txtfile[h];
        h++;
    }
    for (int j = 0; j<6; j++) {
        epubfile[h]=epub[j];
        h++;
    }
    printf("[pid%d] converting %s ...\n", pid, txtfile);
    execl("/usr/bin/pandoc","/usr/bin/pandoc", txtfile, "-o", epubfile, NULL);
    }

    else { //one process gets in
        wait(NULL); //wait for the children and its children to finish converting the files
        char *final[] = {"/usr/bin/zip", "ebooks.zip", '\0'};
        int fsize = 2;  //size of final
        i=1;
        for (i; i<argc; i++) { //goes for all the filenames in argv
            snprintf(epubfile, BUFFER_SIZE, "%s", argv[i]);
            int h=0;
            while(epubfile[h] != '.') {
                h++;
            }
            for (int j = 0; j<5; j++) {
                epubfile[h]=epub[j];
                h++;
            }
            final[fsize]=epubfile;
            fsize++;
            final[fsize]='\0';
        }
        execv("/usr/bin/zip", final);
    }
    return 0;
}