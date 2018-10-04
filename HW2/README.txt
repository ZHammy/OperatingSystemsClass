All programs compiled like this except for the 5's:
    gcc Problem1.c -o Problem1

Problem5 is compiled with
    gcc Problem5c.c -o Problem5c -lrt

The -lrt is required or it will not work


With Problems 3-5, the program ending in c is the client reading the file and sending it while the server ends with s and displays it to terminal

With Problem 3, run Problem3c before Problem3s
With Problem 4, run Problem4s first, though if you dont, you just must be fast lest you lose part of the message
With Problem 5, run Problem5c first and then extremely quickly run Problem5s to display the messages
