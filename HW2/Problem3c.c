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
#include <string.h>

int main()
{
    //Start our program by reading from the testfile.txt and converting everything in it to upper case
    FILE *fp = fopen("testfile.txt", "r");
    if (fp == NULL)
    exit(EXIT_FAILURE);
    //Mostly temp vars for reading
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    //Upper case strings will be stored here
    //Store up to a max of 1024 strings
    char upperCaseStrings[512][BUFSIZ];
    int i=0;//Store where we are in the array
    while ((read = getline(&line, &len, fp)) != -1) {
        //Go through the string, line, and convert each char to uppercase
        char temp='A';
        if(i<512){
            int j=0;
            while(temp!='\0'){
                temp=toupper(line[j]);
                upperCaseStrings[i][j]=temp;
                j++;
            }
            i++;
        }
        else{
            break;
        }
    }
    fclose(fp);
    // int j=0;
    // for(j=0;j<i;j++){
    //     printf("%s",upperCaseStrings[j]);
    // }


    //Now that the file has been read into to upperCaseStrings and been made uppercase,
    //Open up the pipe
    int fd;
    char * myfifo = "/tmp/myfifo";

    //create the pipe
    mkfifo(myfifo, 0666);

    fd = open(myfifo, O_WRONLY);
    //Now loop through and send each item to the pipe
    int j=0;
    //i still stores the amount of lines read
    for(j=0;j<i;j++){
        write(fd,upperCaseStrings[j], sizeof(upperCaseStrings[j]));
        sleep(1);
    }
    write(fd, "Stop", sizeof("Stop"));
    close(fd);



    return 0;
}
