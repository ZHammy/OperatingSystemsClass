#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h> 
#include <unistd.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    //First of all, get PID to know which part of code to execute later
    pid_t parent=getpid();
    //Make sure there is incorrect args
    if(argc!=3){
        printf("\nArgument Error\nPlease enter exactly 2 Arguments (program name, file name) \n");
        return 1;
    }


    //Fork the first child
    fork();
    //Check if the code being run is the child
    if(parent!=getpid()){
        //If it is the child, get the file descriptor for the desired file
        int fileDescriptor=open(argv[2],O_RDWR|O_CREAT);
        //Now redirect standard outputs to the file name
        dup2(fileDescriptor,1);
        dup2(fileDescriptor,2);

        //Now finally run the program sent as an arg
        //For some reason it needs the arg twice so do that
        execl(argv[1],argv[1],NULL);
        return 0;
    }

    return 0;
}
