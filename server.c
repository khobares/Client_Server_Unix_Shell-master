//Advanced System Programming Project
//Section 1 
//Dhawal Rank 104340181
//Siddharth Khobare 104303348

#define _POSIX_SOURCE   //to use functionality from the POSIX.1 standard as ANCI C does not support kill()
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <netinet/in.h>
#include <sys/socket.h>

void child(int);
int main(int argc, char *argv[])
{
	int sd, client, portNumber, status;
	socklen_t len;
	struct sockaddr_in servAdd;
	if(argc != 2)
	{
		printf("Call Model: %s <Port#>\n", argv[0]);
		exit(0);
	}
	if((sd=socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		fprintf(stderr, "Cannot create socket\n");
		exit(1);
	}
	servAdd.sin_family=AF_INET;
	servAdd.sin_addr.s_addr=htonl(INADDR_ANY);
	sscanf(argv[1], "%d", &portNumber);
	servAdd.sin_port=htons((uint16_t)portNumber);
	bind(sd,(struct sockaddr*)&servAdd, sizeof(servAdd));
	listen(sd,5);
	while(1)
	{
		client=accept(sd,(struct sockaddr*)NULL,NULL);
		printf("client connected...\n");
		if(!fork())
			child(client);
		close(client);
		waitpid(0, &status, WNOHANG);
	}
}
void child(int sd)
{
	char out;
	int pid;
  	char *argumentList[20];
  	int handlingamsand, status, i, j;
  	int backup=dup(STDOUT_FILENO);

	while(1)
	{
  		char buf[1000000] = {};
    	int i=0,n;

    	if(n=read(sd, buf, 1000000)==0)
      {                                     //if client presses CTRL+D

        fprintf(stderr, "Client exited\n");
        close(sd);
        exit(0);
      }
      else if(n==-1)
        write(sd, "read() on server failed\n",25);
      else
      {
    	handlingamsand=fetchcommand(buf, argumentList);
  
    	if((pid = fork()) == -1)
    	{
    		  dup2(sd,STDOUT_FILENO);
      		perror("Error in Fork");
          write(sd,"fatal error\n",12);
      		exit(1);
      }
          
    	if(pid > 0) 
      {
        	waitpid(-1, &status, 0);
          dup2(STDOUT_FILENO,backup);
          if(status)                    //if exec fails
          {
            write(sd,"exec() failed. Please check the input\n",38);
          } 
        	
      }
      else  
      {   
  
          dup2(sd,STDOUT_FILENO);
          if(strcmp(argumentList[0],"cd")==0)       //ignore cd
          {
            write(sd,"Oops.. cd is not supported\n",27);
          }
             if(strcmp(argumentList[0],"exit")==0)        //ignore exit
          {
            write(sd,"Oops.. exit is not supported\n",29);
          }                        
          // For Child
        	execvp(argumentList[0], argumentList);
      }
  }
  }
}
//tokennizing function
int fetchcommand(char *line, char *argumentList[]){
  static int correct=0;
  int i=0, offset=0;
  char name[50];
  int found=0;

// flushing the buffer
  while(correct & argumentList[i] != NULL)  
    free(argumentList[i++]);

  i=0;
  while(sscanf(line+offset, "%s", name)==1){      // copy next string from
    argumentList[i] = (char *) malloc(strlen(name)+1); // line to argumentList  
    strcpy(argumentList[i++], name);
    while(line[offset] == ' ')  // discarding extra space
      offset++; 
    offset += strlen(name);
  }
// To execute background process 
  if(!strcmp(argumentList[i-1], "&")){ 
    argumentList[i-1] = NULL;
    found = 1;
  }
  else{
    if(argumentList[i-1][strlen(argumentList[i-1])-1] == '&'){
      argumentList[i-1][strlen(argumentList[i-1])-1]='\0';
      found = 1;
    } 
    argumentList[i] = NULL;
  }
  
  correct=1;
  return(found);
}
