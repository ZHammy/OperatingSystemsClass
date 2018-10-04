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

int main(int argc, char *argv[])
{
    //First of all, get PID to know which part of code to execute later
    pid_t parent=getpid();
    //Make sure there is incorrect args
    if(argc!=2){
        printf("\nArgument Error\nPlease enter a single argument A where A is an email address \n");
        return 1;
    }




    //Create a child to go get the name of the files
    fork();
    //Check if the code being run is the child
    if(parent!=getpid()){
        //Need to use find to find all c files in the directory
        //First make a file where all the files will be stored
        int fileDescriptor=open("cFiles.txt",O_RDWR|O_CREAT,0666);
        //Now redirect standard outputs to the file name
        dup2(fileDescriptor,1);
        dup2(fileDescriptor,2);
        //Run find and it will output to a file
        execl("/usr/bin/find","/usr/bin/find","-name","\"*.c\"","-type","f",NULL);
        close(fileDescriptor);
        exit(0);
    }

    //Make parent wait for the child to finish and write to file
    wait(NULL);

    //Create an array of strings to store the file names in after reading them from the created file
    //Just go with max size of like 1000, should be enough
    char lines[1000][BUFSIZ];

    //next open the file
    FILE *fp = fopen("cFiles.txt", "r");

    //Thanks https://stackoverflow.com/questions/4237084/reading-from-a-file-and-storing-in-array
    //For this snippet on reading files and appending the endline to the strings
    //Need to keep track of where we are in the array
    int i=0;
    //read from the file into the string array
    while (fgets(lines[i], sizeof(lines[0]), fp)){
            lines[i][strlen(lines[i])-1] = '\0';
            i = i + 1;
    }
    fclose(fp);
    char subjectLine[1024];
    sprintf(subjectLine, "\"%d\"", i);




    //Now that we made concatonated the strings, just run mailx using the arg provided as the address
    char command[2048];
    sprintf(command,"echo \"$(cat \\cFiles.txt)\" | mailx -r LinuxTest@Linux.com, -s %s %s",subjectLine,argv[1]);
    system(command);
    //execl("bin/echo","bin/echo","\"$(cat \\$cFiles.txt)\"","|","mailx","-r","LinuxTest@Linux.com",subjectLine,argv[1],NULL);
    //execl("/usr/bin/mailx","/usr/bin/mailx","-s",subjectLine,"-a","cFiles.txt",argv[1],NULL);

    //"\"$(cat \\$cFiles.txt)\"","mailx","-r","LinuxTest@Linux.com",subjectLine,argv[1],NULL);


    return 0;
}
