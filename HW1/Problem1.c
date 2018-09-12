
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
//This function checks if the command line arguments are intergers
int isInteger(char myString[]){
    int i=0;
    //Go through the string looking for the terminating character
    //if a non number is found, return 0 else return false
    while(myString[i]!='\0'){
        //Check if the current character is a number
        if(!isdigit(myString[i])){
            return 0;
        }
        i++;
    }
    //If the program reaches this point, it means that we have a number.
    return 1;
}
int main(int argc, char *argv[])
{
    //First of all, get PID to know which part of code to execute later
    pid_t parent=getpid();
    //Make sure there is only 1 argument
    if(argc!=2){
        printf("\nArgument Error\nPlease enter a single Argument N where N is a positive integer \n");
        return 1;
    }
    //make sure an integer was entered
    if(!isInteger(argv[1])){
        printf("\nArgument Error\nPlease enter a positive integer as your argument \n");
        return 1;
    }

    //Convert the string to an integer
    int myNumber=0;
    int tens=1;
    //The conversion is done by converting the ascii value to an int and multiplying
    //by the appropriate amount of tens
    for(int i=strlen(argv[1])-1;i>-1;i--){
        myNumber+=(((argv[1][i])-48)*tens);
        tens*=10;
    }
    fork();
    if(getpid()==parent){
        fork();
    }
    if(getpid()==parent){
        fork();
    }
    if(getpid()==parent){
        for(int i=0;i<myNumber;i++){
            printf("This is the main process, my PID is %d \n",getpid());
            sleep(2);
        }
    }
    else{
        for(int i=0;i<myNumber;i++){
            printf("This is a child process, my PID is %d, my Parent PID is %d\n",getpid(),getppid());
            sleep(2);
        }
    }
    return 0;
}
