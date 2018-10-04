#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <stdlib.h>


int main()
{
    //Much of this was grabbed from the class notes
    //Now that the file has been read into to upperCaseStrings and been made uppercase,
    //Open up the shared memory
    int shmid;
    if ((shmid = shmget (3457, BUFSIZ, IPC_CREAT|0666)) <0) {
        printf("SYSV DATA CREATION ERROR");
        exit(1);
    }

    //Making a second shared memory just as a true/false thing to know when something new was written
    int flag;
    if ((flag = shmget (3458, BUFSIZ, IPC_CREAT|0666)) <0) {
        printf("SYSV FLAG CREATION ERROR");
        exit(1);
    }

    //if those both created fine, now attatch to them and get pointers for them
    char* data=shmat(shmid, (void *)0, 0);
    char* flagger=shmat(flag, (void *)0, 0);

    if (data == (char *)(-1)) {
        printf("DATA ATTATCHING ERROR");
        exit(1);
    }

    if (flagger == (char *)(-1)) {
        printf("FLAGGER ATTATCHING ERROR");
        exit(1);
    }
    char lastFlag[BUFSIZ]="Oof";
    while(strcmp(data,"Stop")){
        //If the last flag is not the same as the current, read out the data and print it
        if(strcmp(lastFlag,flagger)){
            strncpy(lastFlag, flagger, BUFSIZ);
            printf("%s",data);
        }
    }

    /* detach from the segment: */
    if (shmdt(data) == -1) {
        perror("DATA DETATCH");
        exit(1);
    }
    /* detach from the segment: */
    if (shmdt(flagger) == -1) {
        perror("FLAGGER DETATCH");
        exit(1);
    }
    shmctl(shmid, IPC_RMID, NULL);
    shmctl(flag, IPC_RMID, NULL);





    return 0;
}
