
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "pslibrary.h"

#define READY  0
#define RUNNING  1
#define WAITING  2
#define DONE  3
#define EXPIRED 4

#define P1 100
#define P2 200

#define MAX_INDEX 12
#define MAX_SIZE (6 * MAX_INDEX)

static char stateChars[] = {'r','R','w','\0', 'r'};

typedef struct processData {
    int firstRun;
    int IO;
    int lastRun;
    int state;
    int processVal;
    int quantum;
    char * pArr;
} ProcessData;

//helper functions
//int max(int, int);
//int displayMetrics(char* p1, char* p2);
//int shortestJob(ProcessData, ProcessData);

int max(int a, int b){
    if (a >= b)
        return a;
    return b;
}

//is p1 shorter or equal
int shortestJob(ProcessData p1, ProcessData p2){

    if (p1.firstRun > 0 && p2.firstRun > 0){
        if (p1.firstRun == p2.firstRun)
            return -1;
        else if (p1.firstRun < p2.firstRun)
            return 1;
        return 0;
    }

    //should only happen on psjf
    else if (p1.firstRun > 0 && p2.firstRun == 0){
        if (p1.firstRun == p2.lastRun)
            return -1;
        else if (p1.firstRun < p2.lastRun)
            return 1;
        return 0; 
    }
    
    else{
        //both finish IO at same time
        if (p1.lastRun == p2.lastRun)
            return -1;
        else if (p1.lastRun < p2.lastRun)
            return 1;
        return 0;
    }
}

int displayMetrics(char* p1, char* p2){

    int i, wait1 = 0, wait2 = 0, run = 0, total;
    for (i=0; i < strlen(p1); i++){
        if (p1[i] == 'r'){
            wait1++;
        }else if(p1[i] == 'R'){
            run++;
        }
    }
    for (i=0; i < strlen(p2); i++){
        if (p2[i] == 'r'){
            wait2++;
        }else if(p2[i] == 'R'){
            run++;
        }
    }
    total = max(strlen(p1), strlen(p2));
    printf("%d %d %.1f %.5f\n", wait1, wait2, (((double)wait1 + wait2)/2), ((double)run/total));
    return 0;
}

void fcfs(char *s1, char *s2, int x1, int y1, int z1, int x2, int y2, int z2){

    int i;

    ProcessData p1, p2, currentProcess, idleProcess, temp;

    p1.firstRun = x1;
    p1.IO = y1;
    p1.lastRun = z1;
    p1.state = RUNNING;
    p1.pArr = s1;

    p2.firstRun = x2;
    p2.IO = y2;
    p2.lastRun = z2;
    p2.state = READY;
    p2.pArr = s2;

    currentProcess = p1;
    idleProcess = p2;
    for (i = 0; i < MAX_SIZE; i++){

        if (currentProcess.state == DONE && idleProcess.state == DONE)
            break;

        currentProcess.pArr[i] = stateChars[currentProcess.state];
        if (idleProcess.state != DONE)
            idleProcess.pArr[i] = stateChars[idleProcess.state];

        if (currentProcess.state == RUNNING){
            if (currentProcess.firstRun > 0){
                currentProcess.firstRun--;
                if (currentProcess.firstRun == 0)
                    currentProcess.state = WAITING;
            }
            if (currentProcess.IO <= 0 ){
                currentProcess.lastRun--;
                if (currentProcess.lastRun == 0){
                    currentProcess.state = DONE;
                    if (i < MAX_SIZE)
                        currentProcess.pArr[i + 1] = stateChars[DONE];
                }
            }
        }else{
            currentProcess.IO--;
            if (currentProcess.IO <= 0)
                currentProcess.state = READY;
        }

        if (idleProcess.state == WAITING){
            idleProcess.IO--;
            if (idleProcess.IO <= 0)
                idleProcess.state = READY;
        }
        if ((currentProcess.state == WAITING && idleProcess.state == READY) || 
            (currentProcess.state == DONE && idleProcess.state == READY)){
            temp = currentProcess;
            currentProcess = idleProcess;
            idleProcess = temp;
            currentProcess.state = RUNNING;
        }
        else if (currentProcess.state == READY && idleProcess.state == READY){
            temp = currentProcess;
            currentProcess = idleProcess;
            idleProcess = temp;
            currentProcess.state = RUNNING;
        }
        //printf("first:%d io:%d last:%d\n", currentProcess.firstRun, currentProcess.IO, currentProcess.lastRun);
    }
}

void sjf(char *s1, char *s2, int x1, int y1, int z1, int x2, int y2, int z2){

    int i, jobVal;

    ProcessData p1, p2, currentProcess, idleProcess, temp;

    p1.firstRun = x1;
    p1.IO = y1;
    p1.lastRun = z1;
    p1.pArr = s1;

    p2.firstRun = x2;
    p2.IO = y2;
    p2.lastRun = z2;
    p2.pArr = s2;

    jobVal = shortestJob(p1, p2);

    //p1 is shorter or is equal in length and tie must be broken
    if (jobVal == 1 || jobVal == -1){
        p1.state = RUNNING;
        p2.state = READY;
        currentProcess = p1;
        idleProcess = p2;
    }else{
        p1.state = READY;
        p2.state = RUNNING;
        currentProcess = p2;
        idleProcess = p1;
    }

    for (i=0; i < MAX_SIZE; i++){

        if (currentProcess.state == DONE && idleProcess.state == DONE)
            break;

        currentProcess.pArr[i] = stateChars[currentProcess.state];
        if (idleProcess.state != DONE)
            idleProcess.pArr[i] = stateChars[idleProcess.state];

        if (currentProcess.state == RUNNING){
            if (currentProcess.firstRun > 0){
                currentProcess.firstRun--;
                if (currentProcess.firstRun == 0)
                    currentProcess.state = WAITING;
            }
            if (currentProcess.IO <= 0 ){
                currentProcess.lastRun--;
                if (currentProcess.lastRun == 0){
                    currentProcess.state = DONE;
                    if (i < MAX_SIZE)
                        currentProcess.pArr[i + 1] = stateChars[DONE];
                }
            }
        }else{
            currentProcess.IO--;
            if (currentProcess.IO <= 0)
                currentProcess.state = READY;
        }

        if (idleProcess.state == WAITING){
            idleProcess.IO--;
            if (idleProcess.IO <= 0)
                idleProcess.state = READY;
        }

        //both are in ready queue at same time
        if (currentProcess.state == READY && idleProcess.state == READY){

            jobVal = shortestJob(currentProcess, idleProcess);
            if (jobVal == 0 || (jobVal == -1 && idleProcess.processVal == P1)){
                temp = currentProcess;
                currentProcess = idleProcess;
                idleProcess = temp;
            }
            currentProcess.state = RUNNING;
        }
        else if (currentProcess.state == DONE && idleProcess.state == WAITING){
            temp = currentProcess;
            currentProcess = idleProcess;
            idleProcess = temp;
        }
        else if (currentProcess.state == READY && idleProcess.state == DONE){
            currentProcess.state = RUNNING;
        }
        else if ((currentProcess.state == WAITING && idleProcess.state == READY) ||
                 (currentProcess.state == DONE && idleProcess.state == READY)){

            temp = currentProcess;
            currentProcess = idleProcess;
            idleProcess = temp;
            currentProcess.state = RUNNING;
        }
    }
}

void psjf(char *s1, char *s2, int x1, int y1, int z1, int x2, int y2, int z2){

    int i, jobVal;

    ProcessData p1, p2, currentProcess, idleProcess, temp;

    p1.firstRun = x1;
    p1.IO = y1;
    p1.lastRun = z1;
    p1.pArr = s1;
    p1.processVal = P1;

    p2.firstRun = x2;
    p2.IO = y2;
    p2.lastRun = z2;
    p2.pArr = s2;
    p2.processVal = P2;

    jobVal = shortestJob(p1, p2);

    //p1 is shorter or is equal in length and tie must be broken
    if (jobVal == 1 || jobVal == -1){
        p1.state = RUNNING;
        p2.state = READY;
        currentProcess = p1;
        idleProcess = p2;
    }else{
        p1.state = READY;
        p2.state = RUNNING;
        currentProcess = p2;
        idleProcess = p1;
    }

    for (i=0; i < MAX_SIZE; i++){

        if (currentProcess.state == DONE && idleProcess.state == DONE)
            break;

        currentProcess.pArr[i] = stateChars[currentProcess.state];
        if (idleProcess.state != DONE)
            idleProcess.pArr[i] = stateChars[idleProcess.state];

        if (currentProcess.state == RUNNING){
            if (currentProcess.firstRun > 0){
                currentProcess.firstRun--;
                if (currentProcess.firstRun == 0)
                    currentProcess.state = WAITING;
            }
            if (currentProcess.IO <= 0 ){
                currentProcess.lastRun--;
                if (currentProcess.lastRun == 0){
                    currentProcess.state = DONE;
                    if (i < MAX_SIZE)
                        currentProcess.pArr[i + 1] = stateChars[DONE];
                }
            }
        }else{
            currentProcess.IO--;
            if (currentProcess.IO <= 0)
                currentProcess.state = READY;
        }

        if (idleProcess.state == WAITING){
            idleProcess.IO--;
            if (idleProcess.IO <= 0)
                idleProcess.state = READY;
        }

        //both are in ready queue at same time
        if (currentProcess.state == READY && idleProcess.state == READY){

            jobVal = shortestJob(currentProcess, idleProcess);
            if (jobVal == 0 || (jobVal == -1 && idleProcess.processVal == P1)){
                temp = currentProcess;
                currentProcess = idleProcess;
                idleProcess = temp;
            }
            currentProcess.state = RUNNING;
        }

        else if ((currentProcess.state == WAITING && idleProcess.state == READY) ||
                 (currentProcess.state == DONE && idleProcess.state == READY)){
            temp = currentProcess;
            currentProcess = idleProcess;
            idleProcess = temp;
            currentProcess.state = RUNNING;
        }

        else if (currentProcess.state == RUNNING && idleProcess.state == READY){
            jobVal = shortestJob(currentProcess, idleProcess);
            if (jobVal == 0 || (jobVal == -1 && idleProcess.processVal == P1)){
                temp = currentProcess;
                currentProcess = idleProcess;
                idleProcess = temp;
                currentProcess.state = RUNNING;
                idleProcess.state = READY;
            }
        }else if (currentProcess.state == READY && idleProcess.state == DONE){
            currentProcess.state = RUNNING;
        }
        else if (currentProcess.state == DONE && idleProcess.state == WAITING){
            temp = currentProcess;
            currentProcess = idleProcess;
            idleProcess = temp;
        }
        else if (currentProcess.state == READY && idleProcess.state == DONE){
            currentProcess.state = RUNNING;
        }
        else if (currentProcess.state == WAITING && idleProcess.state == READY){
            
            temp = currentProcess;
            currentProcess = idleProcess;
            idleProcess = temp;
            currentProcess.state = RUNNING;
        }
        
    }
}

void rr(char *s1, char *s2, int q, int x1, int y1, int z1, int x2, int y2, int z2){

    int i;

    ProcessData p1, p2, currentProcess, idleProcess, temp;

    p1.firstRun = x1;
    p1.IO = y1;
    p1.lastRun = z1;
    p1.state = RUNNING;
    p1.pArr = s1;
    p1.processVal = P1;

    p2.firstRun = x2;
    p2.IO = y2;
    p2.lastRun = z2;
    p2.state = EXPIRED;
    p2.pArr = s2;
    p2.processVal = P2;

    currentProcess = p1;
    currentProcess.quantum = q;
    idleProcess = p2;
    idleProcess.quantum = 0;

    for (i=0; i < MAX_SIZE; i++){
        
        if (currentProcess.state == DONE && idleProcess.state == DONE)
            break;

        currentProcess.pArr[i] = stateChars[currentProcess.state];
        if (idleProcess.state != DONE)
            idleProcess.pArr[i] = stateChars[idleProcess.state];

        if (currentProcess.state == RUNNING){
            if (currentProcess.firstRun > 0){
                currentProcess.firstRun--;
                if (currentProcess.firstRun == 0){
                    currentProcess.state = WAITING;
                    currentProcess.quantum = 0;  
                }
            }
            if (currentProcess.IO <= 0 ){
                currentProcess.lastRun--;
                if (currentProcess.lastRun == 0){
                    currentProcess.state = DONE;
                    if (i < MAX_SIZE)
                        currentProcess.pArr[i + 1] = stateChars[DONE];
                    currentProcess.quantum = 0;
                }
            }
            currentProcess.quantum--;
            if (currentProcess.quantum <= 0 && currentProcess.state == RUNNING){
                currentProcess.state = EXPIRED;
            }
        }else{
            currentProcess.IO--;
            if (currentProcess.IO <= 0){
                currentProcess.state = READY;
            }
        }

        if (idleProcess.state == WAITING){
            idleProcess.IO--;
            if (idleProcess.IO <= 0){
                idleProcess.state = READY;
            }
        }
        //printf("1.) Current Q: %d\n", currentProcess.quantum);
       
        //Normal RR switch
        if (currentProcess.state == EXPIRED && idleProcess.state == EXPIRED){

            temp = currentProcess;
            currentProcess = idleProcess;
            idleProcess = temp;
            currentProcess.state = RUNNING;
            currentProcess.quantum = q;
        }

        //idle process finished IO and current quantum's expired, break tie (both in ready queue)
        //or both finished IO at the same time
        else if (currentProcess.state == EXPIRED && idleProcess.state == READY){

            if (idleProcess.processVal == P1){
                temp = currentProcess;
                currentProcess = idleProcess;
                idleProcess = temp;
                currentProcess.state = RUNNING;
                idleProcess.state = EXPIRED;
                currentProcess.quantum = q;
            }else{
                currentProcess.state = RUNNING;
                currentProcess.quantum = q;
                idleProcess.state = EXPIRED;
            }

        }
        else if (currentProcess.state == READY && idleProcess.state == READY){

            if (idleProcess.processVal == P1){
                temp = currentProcess;
                currentProcess = idleProcess;
                idleProcess = temp;
                currentProcess.state = RUNNING;
                idleProcess.state = EXPIRED;
                currentProcess.quantum = q;
            }else{
                currentProcess.state = RUNNING;
                currentProcess.quantum = q;
                idleProcess.state = EXPIRED;
            }
        }
        else if ((currentProcess.state == WAITING && idleProcess.state == READY) ||
                  (currentProcess.state == WAITING && idleProcess.state == EXPIRED)){

            temp = currentProcess;
            currentProcess = idleProcess;
            idleProcess = temp;
            currentProcess.state = RUNNING;
            currentProcess.quantum = q;

        }
        else if (currentProcess.state == READY && idleProcess.state == WAITING){
            currentProcess.state = RUNNING;
            currentProcess.quantum = q;
        }
        //finished conditions, always give running process CPU
        else if ((currentProcess.state == DONE && idleProcess.state == READY) ||
                 (currentProcess.state == DONE && idleProcess.state == EXPIRED)){
            temp = currentProcess;
            currentProcess = idleProcess;
            idleProcess = temp;
            currentProcess.state = RUNNING;
            currentProcess.quantum = q;
            
        }else if (currentProcess.state == DONE && idleProcess.state == WAITING){
            temp = currentProcess;
            currentProcess = idleProcess;
            idleProcess = temp;
            currentProcess.state = WAITING;
            currentProcess.quantum = q;   
        }else if (idleProcess.state == DONE){
            if (currentProcess.state == EXPIRED || currentProcess.state == READY){
                currentProcess.state = RUNNING;
                currentProcess.quantum = q;
            }
        }
        
    }
}
