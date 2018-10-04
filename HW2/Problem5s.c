#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>



int main()
{
    //Much of this was grabbed from the class notes
    //Now that the file has been read into to upperCaseStrings and been made uppercase,
    //Open up the shared memory
    int shm_fd;
    /* pointer to shared memory obect */
    void *ptr;
    /* open the shared memory object */
    shm_fd = shm_open("temperino", O_CREAT | O_RDWR, 0666);
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
    char lastFlag[BUFSIZ]="Oof";
    while(strcmp((char *)ptr,"Stop")){
        //If the last flag is not the same as the current, read out the data and print it
        if(strcmp(lastFlag,flagger)){
            strncpy(lastFlag, flagger, BUFSIZ);
            printf("%s",(char *)ptr);
        }
    }

    /* detach from the segment: */
    if (shmdt(flagger) == -1) {
        perror("FLAGGER DETATCH");
        exit(1);
    }
    shmctl(flag, IPC_RMID, NULL);





    return 0;
}
