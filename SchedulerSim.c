
#include "pslibrary.h"

#define MAX_INDEX 12
#define MAX_SIZE (6 * MAX_INDEX)


int main(int argc, char* argv[]){

    if (argc != 8){
       fprintf(stderr, "[ERROR]: Exactly 7 parameters required!");
       exit(EXIT_FAILURE);
    }

    int q = atoi(argv[1]);
    int x1 = atoi(argv[2]);
    int y1 = atoi(argv[3]);
    int z1 = atoi(argv[4]);
    int x2 = atoi(argv[5]);
    int y2 = atoi(argv[6]);
    int z2 = atoi(argv[7]);

    char s1[MAX_SIZE];
    char s2[MAX_SIZE];

    printf("\nCS 3733 Assignment 2, written by Ryan Gill\n");
    printf("%d %d %d %d %d %d %d \n", q, x1, y1, z1, x2, y2, z2);

    memset(s1,'x',MAX_SIZE);
    memset(s2,'y',MAX_SIZE);
    fcfs(s1, s2, x1, y1, z1, x2, y2, z2);
    printf("\nScheduler FCFS:\n");
    printf("%s\n", s1);
    printf("%s\n", s2);
    displayMetrics(s1, s2);

    memset(s1,'x',MAX_SIZE);
    memset(s2,'y',MAX_SIZE);
    sjf(s1, s2, x1, y1, z1, x2, y2, z2);
    printf("\nScheduler SJF:\n");
    printf("%s\n", s1);
    printf("%s\n", s2);
    displayMetrics(s1, s2);

    memset(s1,'x',MAX_SIZE);
    memset(s2,'y',MAX_SIZE);
    psjf(s1, s2, x1, y1, z1, x2, y2, z2);
    printf("\nScheduler PSJF:\n");
    printf("%s\n", s1);
    printf("%s\n", s2);
    displayMetrics(s1, s2);

    memset(s1,'x',MAX_SIZE);
    memset(s2,'y',MAX_SIZE);
    rr(s1, s2, q, x1, y1, z1, x2, y2, z2);
    printf("\nScheduler RR:\n");
    printf("%s\n", s1);
    printf("%s\n", s2);
    displayMetrics(s1, s2);

    exit(EXIT_SUCCESS);

}