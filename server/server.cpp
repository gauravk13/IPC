#include <iostream>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fstream>
#include <cstdlib>
#include <map>
#include <list>
#include <stdlib.h>

using namespace std;

void map_client_to_group(int, int);
void dostuff(int); 
void create_files(int no_of_files);
void create_single_file(int file_id);
void error(string msg)
{
    cerr<<msg<<"\n";
    exit(1);
}

map <int , list < int > > GtC;
map <int, int> CtG;

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
   int n,client_id,group_id;
   char buffer[256];
      
  //client -id  
   bzero(buffer,256);
   n = read(sock,buffer,255);
   if (n < 0) error("ERROR reading from socket");
   cout<<buffer<<"\n";
   
   client_id = atoi(buffer);
   
  // check for client-group mapping
   map_client_to_group(sock,client_id);
  
   // message of client 
   bzero(buffer,256);
   n = read(sock,buffer,255);
   if (n < 0) error("ERROR reading from socket");

   group_id = CtG[client_id];
   string file_name = to_string(group_id);
   file_name.append(".txt");
  // write to file 
   ofstream myfile;
  myfile.open (file_name);
  myfile<<"Client Id: "<< client_id<<" :: ";
  myfile<<buffer;
  myfile.close();

   cout<<"Here is the message : " <<buffer<<"\n";
   n = write(sock,"I got your message",18);
   if (n < 0) error("ERROR writing to socket");
}

void map_client_to_group( int sock, int client_id) {
      int n,group_id;
      char buffer[255];
      if( !CtG[client_id] ) {
           n = write(sock,"Join any group from 1-10 to proceed",50);
   	   if (n < 0) error("ERROR writing to socket");	
           // got the group_id
           bzero(buffer,256);
           n = read(sock,buffer,255);
           if (n < 0) error("ERROR reading from socket");
 	   group_id = atoi(buffer);
         // updating map CtG
           CtG[client_id] = group_id;
         // updating map GtC  
	 list <int> l = GtC[group_id];
           l.push_back(client_id);
           GtC[group_id] = l;
  
       }  else {
		string msg = "you are part of group Id : ";
        	string s = to_string( CtG[client_id]);
		msg.append(s);
                const void * a = msg.c_str();
		n = write(sock,a,50);
        	if (n < 0) error("ERROR writing to socket");
	}
}

void create_files(int no_of_files) {
  
  for(int i=1; i<= no_of_files; i++) {
     create_single_file(i);
 }
}

void create_single_file (int file_id) {
  ofstream myfile;
  std::string file_name = std::to_string(file_id);
  file_name.append(".txt") ;
  myfile.open (file_name);
  myfile<<"hello";
  myfile.close();
}
