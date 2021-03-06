#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>
#include <regex>

using namespace std;

int main(int argc, char *argv[])
{
    int sockfd, portno, n, client_uid;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
      cerr<<"usage "<<argv[0]<<" hostname port \n";
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        cerr<<"ERROR opening socket\n";
        exit(0);
   }
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        cerr<<"ERROR, no such host\n";
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) { 
        cerr<<"ERROR connecting\n";
        exit(0);
     }	
    
   cout<<"Please enter the client id: ";

    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) {
         cerr<<"ERROR writing to socket\n";
         exit(0);
   }
   
   // getting mapping info from server 
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) {
         cerr<<"ERROR reading from socket\n";
         exit(0);
    }
    cout<<buffer<<"\n";
    
      if (regex_match ("Join any group from 1-10 to proceed", regex("(Join any group)(.*)") ))
{
     cout<<"Please enter the group ID: ";
     bzero(buffer,256);
     fgets(buffer,255,stdin);
     n = write(sockfd,buffer,strlen(buffer));
     if (n < 0) {
         cerr<<"ERROR writing to socket\n";
         exit(0);
     }
   } else {
      	bzero(buffer,256);
    	n = read(sockfd,buffer,255);
    	if (n < 0) {
        	 cerr<<"ERROR reading from socket\n";
       		  exit(0);
   	 }	
        cout<<buffer<<"\n";
     }
 

    cout<<"Please enter the message: ";

    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) {
         cerr<<"ERROR writing to socket\n";
         exit(0);
   }

    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) {
         cerr<<"ERROR reading from socket\n";
         exit(0);
    }
	cout<<buffer<<"\n";
    return 0;
}
