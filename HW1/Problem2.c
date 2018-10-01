#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    //First of all, get PID to know which part of code to execute later
    pid_t parent=getpid();
    //Make sure there is only 1 argument
    if(argc!=2){
        printf("\nArgument Error\nPlease enter a single Argument N where N is a filename in the current directory \n");
        return 1;
    }
    //make sure a valid filename was entered
    if(open(argv[1],O_RDONLY)<0){
        printf("\nError Could not Find File\n");
        return 1;
    }

    //Fork and run ls
    if(getpid()==parent){
        fork();
        if(getpid()!=parent){
            //Not sure why you have to have ls twice but it doesnt work without
            execl("/bin/ls","/bin/ls","-l",NULL);

        }
    }
    //Fork and run ps
    if(getpid()==parent){
        fork();
        if(getpid()!=parent){
            execl("/bin/ps","/bin/ps","-ef",NULL);

        }
    }
    //fork and run more
    if(getpid()==parent){
        fork();
        if(getpid()!=parent){
            execl("usr/bin/more","usr/bin/more",argv[1],NULL);

        }
    }
    //Make the parent wait for each of the children to die
    if(getpid()==parent){
        wait(NULL);
        wait(NULL);
        wait(NULL);
        printf("Main process Terminates\n");
    }
    return 0;
}
