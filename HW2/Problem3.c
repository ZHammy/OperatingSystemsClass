#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
//Used this as a base for basic message communication
//https://stackoverflow.com/questions/2784500/how-to-send-a-simple-string-between-two-programs-using-pipes
int main(int argc, char *argv[])
{
    //First of all, get PID to know which part of code to execute later
    pid_t parent=getpid();
    //Make sure there is incorrect args
    if(argc!=2){
        printf("\nArgument Error\nPlease enter a single argument A where the name of a text file in the current directory \n");
        return 1;
    }




    //Create a child that will serve as the server
    fork();
    //Check if the code being run is the child
    if(parent!=getpid()){
        //First make a file descripter and the path for the pipe
        int fd;
        char * myPipe = "myPipe";

        //actually create the pipe
        mkfifo(myPipe, 0666);


        //This will store our raw message that we will be sending
        char rawMessageFromText[2048][BUFSIZ];
        //next open the file
        FILE *fp = fopen(argv[1], "r");

        //Thanks https://stackoverflow.com/questions/4237084/reading-from-a-file-and-storing-in-array
        //For this snippet on reading files and appending the endline to the strings
        //Need to keep track of where we are in the array
        //read from the file into the string array
        int i=0;
        while (fgets(rawMessageFromText[i], sizeof(rawMessageFromText[0]), fp)){
                rawMessageFromText[i][strlen(rawMessageFromText[i])-1] = '\0';
                int j=0;
                for(j=0;j<strlen(rawMessageFromText[i])-1;j++){
                    rawMessageFromText[i][j]=toupper(rawMessageFromText[i][j]);
                }
                //Now Open the pipe
                fd = open(myPipe, O_WRONLY);
                //Write our message
                write(fd, rawMessageFromText[i], sizeof(rawMessageFromText[i]));
                //close pipe
                close(fd);
                //sleep
                sleep(1);
        }
        //Now Open the pipe
        fd = open(myPipe, O_WRONLY);
        //Write our stop message
        write(fd, "Stop", sizeof("Stop"));
        //close pipe
        close(fd);

        exit(0);
    }

    //The parent from here will serve as the client

    //First make a file descripter and the path for the pipe
    int fd;
    char * myPipe = "myPipe";


    //create our string buffer
    char message[2048];

    //check if the message is stop, if not, keep outputting buffer
    while(!strcmp(message,"Stop")){
        fd=open(myPipe,O_RDONLY);
        read(fd,message,2048);
        printf("Message Recieved: \n %s",message);
        close(fd);
        sleep(1);
    }
    //make sure child (server) is done
    wait(NULL);
    //delete the pipe
    unlink(myPipe);

    return 0;


}
