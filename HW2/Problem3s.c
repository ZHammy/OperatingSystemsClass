#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>


int main()
{
    int fd;
    //mark where the pipe is
    char * myfifo = "/tmp/myfifo";
    char buf[BUFSIZ];
    //open the pipe
    fd = open(myfifo, O_RDONLY);
    //Loop through and wait till buf says stop
    while(strcmp(buf,"Stop")){
        read(fd, buf, BUFSIZ);
        //Only print if not null
        if(strcmp(buf,"")){
            printf("%s\n", buf);
        }
    }


    close(fd);
    //delete the pipe
    unlink(myfifo);

    return 0;
}
