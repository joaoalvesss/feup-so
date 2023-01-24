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

int n = 0;
int gid;

#define BUFFER_SIZE 1024
void kill_child() {
    kill(gid, kill_child);
    wait(NULL);
    exit(0);
}

void delete_pipes() {
    char getname[BUFFER_SIZE];
    for(int h = 1; h<=n; h++){
        if(h == n){snprintf(getname, BUFFER_SIZE, "pipe%dto%d", h, 1);}
        else snprintf(getname, BUFFER_SIZE, "pipe%dto%d", h, h+1);
        unlink(getname);
    }
    kill(gid, kill_child);
    wait(NULL);
    exit(0);
}

int main(int argc, char *argv[]) { //argv[1]-n agrv[2]-p argv[3]-t
    char getname[BUFFER_SIZE];
    int reading_file;
    int writing_file;
    int value;
    gid = 0;
    int i = 1;
    int pn;
    n = atoi(argv[1]);  //n processes
    double p = atof(argv[2])*100.0; //p probability
    int t = atoi(argv[3]); //time a process is blocked in seconds
    //making the fifos
    for(int h = 1; h<=n; h++){
        if(h == n){snprintf(getname, BUFFER_SIZE, "pipe%dto%d", h, 1);}
        else snprintf(getname, BUFFER_SIZE, "pipe%dto%d", h, h+1);
        mkfifo(getname, 0777);
    }
    //creating the processes
    for(i; i < n; i++){
        if(gid == 0){gid = fork();pn=i;}
    }
    if (gid==0) {pn=i;}
    //each process opens the corresponding fifos to read from and write to
    if(pn == 1){
        snprintf(getname, BUFFER_SIZE, "pipe%dto%d", n, pn);
        if ((reading_file = open(getname, O_RDONLY))==-1) {
            perror("Error opening file");
            exit(1);
        }
    }
    if(pn == n){
        snprintf(getname, BUFFER_SIZE, "pipe%dto%d", pn, 1);
        if ((writing_file = open(getname, O_WRONLY))==-1) {
            perror("Error opening file");
            exit(1);
        }
    }
    if (pn!=1) {
        snprintf(getname, BUFFER_SIZE,"pipe%dto%d", pn - 1, pn);
        if ((reading_file = open(getname, O_RDONLY))==-1) {
            perror("Error opening file");
            exit(1);
        }
        }
    if (pn!=n) {
        snprintf(getname, BUFFER_SIZE, "pipe%dto%d", pn, pn + 1);
        if ((writing_file = open(getname, O_WRONLY))==-1) {
            perror("Error opening file");
            exit(1);
        }
    }
    // process num 1 sends the token to fifo pipe1to2
    if(pn == 1){
        int num = 0;
        if((write(writing_file, &num, sizeof(int)))==-1) {
            perror("Writing file error");
            exit(1);
        }
    }
    //loop where processes recieve and send the token between each other
    srandom(time(0) * getpid());
    while(true) {
        signal(SIGINT, delete_pipes);
        if ((read(reading_file, &value, sizeof(int)))==-1) {
            perror("Reading file error");
            exit(1);
        }
        value++;
        double x = (double)random() / (double)(RAND_MAX/100);
        if (x <= p) {
            printf("[p%d] lock on token (val = %d)\n", pn, value);
            fflush(stdout);
            sleep(t);
            printf("[p%d] unlock token\n", pn);
            fflush(stdout);
        }
        if ((write(writing_file, &value, sizeof(int)))==-1) {
            perror("Writing file error");
            exit(1);
        }
    }
    return EXIT_SUCCESS;
}