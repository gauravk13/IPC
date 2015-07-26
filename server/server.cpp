#include <iostream>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fstream>
#include <cstdlib>

using std::cout;
using std::cerr;
using std::string;
using std::ofstream;

void dostuff(int); 
void create_files(int no_of_files);
void create_single_file(int file_id);
void error(string msg)
{
    cerr<<msg<<"\n";
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, pid;
     socklen_t clilen; 
     struct sockaddr_in serv_addr, cli_addr;
     if (argc != 2) {
         error("ERROR, no port provided\n");
     }
     create_files(10);
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
     while (1) {
         newsockfd = accept(sockfd, 
               (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0) 
             error("ERROR on accept");
         pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {
             close(sockfd);
             dostuff(newsockfd);
             exit(0);
         }
         else close(newsockfd);
     } /* end of while */
     return 0; /* we never get here */
}

/******** DOSTUFF() *********************
 There is a separate instance of this function 
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
void dostuff (int sock)
{
   int n;
   char buffer[256];
      
   bzero(buffer,256);
   n = read(sock,buffer,255);
   if (n < 0) error("ERROR reading from socket");
   cout<<buffer<<"\n";
   bzero(buffer,256);
   n = read(sock,buffer,255);
   if (n < 0) error("ERROR reading from socket");
   cout<<"Here is the message : " <<buffer<<"\n";
   n = write(sock,"I got your message",18);
   if (n < 0) error("ERROR writing to socket");
}



void create_files(int no_of_files) {
  
  for(int i=1; i<= no_of_files; i++) {
     create_single_file(i);
 }
}

void create_single_file (int file_id) {
  cout<<file_id;
  ofstream myfile;
  std::string file_name = std::to_string(file_id);
  file_name.append(".txt") ;
  myfile.open (file_name);
  myfile<<"hello";
  myfile.close();
}
