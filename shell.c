#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXLINE 80
#define MAXARG 20

void background(char * cmd);

int main(){
    char cmd[MAXLINE];
    pid_t pid;
    int status;

    while(1){
        /*Get user input*/
        printf("mysh$ ");
        fflush(stdout);
        fgets(cmd, MAXLINE, stdin);
        if( strcmp(cmd, "exit\n") == 0 ){
            exit(0);
        }
        /*Create child process*/
        pid = fork();

        /*If there was an error, exit*/
        if( pid < 0 ){
            perror("Fork error");
            exit(1);
        }

        /*If this is the child process, run the command in the background*/
        else if(pid == 0){
            char cmd_cpy[MAXLINE];
            memcpy(cmd_cpy, cmd, MAXLINE);
            background(cmd_cpy);
        }

        /*Otherwise this is the parent, continue to receive user input*/
        else{
            wait(&status);
            fprintf(stderr, "Child terminated with exit code %d\n", status);
            continue;
        }

    }

}

void background(char * cmd){
    /*Tokenize input*/
    int i = 0;
    char * argv[MAXARG];
    argv[i] = strtok(cmd, "\t \n");

    /*If there is nothing in the command string, terminate execution*/
    if(argv[i++] == NULL){
        exit(0);
    }
    while( i < MAXARG && ( argv[i++] = strtok(NULL, "\t \n") ) != NULL ){

    }
    argv[i] = "\0";

    /*Execute the command*/
    execvp(argv[0], argv);

    /*Shouldn't ever get here if execvp was successful*/
    perror("execvp() failed");
    exit(1);
}