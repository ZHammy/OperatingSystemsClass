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
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/mman.h>



int main()
{
    //This file IO stuff is the same as Q3
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

    //Much of this was grabbed from the class notes
    //file descriptor
    int shm_fd;
    //ptr to mmap
    void *ptr;
    //create the object
    shm_fd = shm_open("temperino", O_CREAT | O_RDWR, 0666);
    /* configure the size of the shared memory object */
    ftruncate(shm_fd, BUFSIZ);
    /* memory map the shared memory object */
    ptr = mmap(NULL, BUFSIZ, PROT_WRITE|PROT_READ, MAP_SHARED, shm_fd, 0);




    //Making a second shared memory just as a true/false thing to know when something new was written
    int flag;
    if ((flag = shmget (3458, BUFSIZ, IPC_CREAT|0666)) <0) {
        printf("SYSV FLAG CREATION ERROR");
        exit(1);
    }

    //if those both created fine, now attatch to them and get pointers for them
    char* flagger=shmat(flag, (void *)0, 0);


    if (flagger == (char *)(-1)) {
        printf("FLAGGER ATTATCHING ERROR");
        exit(1);
    }
    //Initially set Flagger to 0 which we will use to tell if there is new data to be read
    strncpy(flagger, "asdf", BUFSIZ);

    //Now loop through and send each item to the shared memory
    int j=0;
    //i still stores the amount of lines read
    for(j=0;j<i;j++){
        /* write to the shared memory object */
        sprintf(ptr,"%s",upperCaseStrings[j]);
        strncpy(flagger, upperCaseStrings[j], BUFSIZ);
        sleep(1);
    }
    i++;
    sprintf(ptr,"%s","Stop");
    strncpy(flagger, "Stop", BUFSIZ);


    /* detach from the segment: */
    if (shmdt(flagger) == -1) {
        perror("FLAGGER DETATCH");
        exit(1);
    }

    return 0;
}
