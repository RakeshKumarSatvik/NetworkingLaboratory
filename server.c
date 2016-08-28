/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}
//////////**********Enhanced Code Starts**********////////////////
// Function to read the message and reply to the client
// Fetch information from the client_addr using inet_ntop
void dostuff(int newsockfd, struct sockaddr_in cli_addr) {
     char buffer[256], ip_addr[256];
     int n;

     bzero(buffer,256);
     inet_ntop(AF_INET, &cli_addr.sin_addr, ip_addr, INET_ADDRSTRLEN);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message from Rakesh Kumar Satvik at %s: %s\n", ip_addr, buffer);
     n = write(newsockfd,"Reply from Server: I got your message Rakesh Kumar Satvik.",58);
     if (n < 0) error("ERROR writing to socket");
}
//////////**********Enhanced Code Ends**********////////////////

int main(int argc, char *argv[])
{
     signal (SIGCHLD, SIG_IGN);

     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n, pid;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);

//////////**********Enhanced Code Starts**********////////////////
     // Adding while loop to make sure that the server can accept more than one client
     // Create a new process for each of the client accepted and dostuff appropriately.
     while (1)
     {
	 newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
	 if (newsockfd < 0)
	     error("ERROR on accept");
	 pid = fork();
	 if (pid < 0)
 	     error("ERROR on fork");
	 if (pid == 0)
	 {
	     close(sockfd);
	     dostuff(newsockfd, cli_addr);
	     exit(0);
	 }
	 else
	     close(newsockfd);
     } /* end of while */
//////////**********Enhanced Code Ends**********////////////////
     close(sockfd);
     return 0;
}
