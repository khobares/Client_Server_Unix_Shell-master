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

int main(int argc, char *argv[])
{
	char message[255];
	int server, portNumber,n;
	socklen_t len;
	struct sockaddr_in servAdd;
	char buf[1000];
	printf("Connecting to the Archer's server...\n");
	sleep(2);

	if(argc != 3)
	{
		printf("Call model:%s <IP> <Port#>\n",argv[0]);
		exit(0);
	}

	if((server=socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		fprintf(stderr, "Cannot create socket\n");
		exit(1);
	}

	servAdd.sin_family=AF_INET;
	sscanf(argv[2],"%d", &portNumber);
	servAdd.sin_port=htons((uint16_t)portNumber);
	
	if(inet_pton(AF_INET, argv[1],&servAdd.sin_addr)<0)
	{
		fprintf(stderr, "inet_pton() has failed\n");
		exit(2);
	}

	if(connect(server, (struct sockaddr *)&servAdd, sizeof(servAdd))<0)
	{
		fprintf(stderr, "connect() has filed\n");
		exit(3);
	}

	printf("Connection successful...\n");
	fprintf(stderr, "This is a simple Shell. Type commands and press enter to see the results. Although you may not have sufficient rights to run all the commands. You can exit any time by pressing Ctrl+D, Ctrl+C or Ctrl+D anytime. Happy Coding. \n");
	while(1)
	{
		fprintf(stderr, "archer:~>");
		memset(buf,0,sizeof(buf));
		if(n=read(0, buf, 1000000)==0)				//if CTRL+D is pressed
		{
			printf("Ctrl+D pressed. Exiting...\n");
			if(write(server,0,1)==-1)
				fprintf(stderr, "write() failed\n");
			else
			{
				usleep(20);
            	close(server);
				exit(0);
			}
		}
		if(n==-1)
			fprintf(stderr, "read() failed\n");
		if(buf[0] != '\n')
		{
			if(write(server, buf, 1000000)==-1)
				fprintf(stderr, "write() failed\n");
			else
			{
				memset(buf,0,sizeof(buf));
            	if(read(server, buf, 1000000))
					fprintf(stderr, "%s\n", buf);
				else
					fprintf(stderr, "read() Failed\n");
            }
		}
		memset(buf,0,sizeof(buf));
	}
	close(server);
	exit(0);
}