#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include "comm.h"
#include "util.h"

/* -------------------------Main function for the client ----------------------*/
void main(int argc, char * argv[]) {

	int server_write_user_read[2], server_read_user_write[2];

	// You will need to get user name as a parameter, argv[1].
	//connects to the server using given connect_to_server function
	if(connect_to_server("GAVINPETERADAM", argv[1], server_write_user_read, server_read_user_write) == -1) {
		exit(-1);
	}
	close(server_write_user_read[1]); //closes pipe ends that client should not access
	close(server_read_user_write[0]);
	char buf[MAX_MSG]; //allocates buffers for reading from stdin and child process pipes
	char read_buf[MAX_MSG];
	fcntl(0, F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK); //sets stdin and pipes to be nonblocking
	fcntl(server_write_user_read[0], F_SETFL, fcntl(server_write_user_read[0], F_GETFL) | O_NONBLOCK);
	printf("pipe to read: %d\n", server_write_user_read[0]); //prints pipe user is reading and writing from
	printf("pipe to write: %d\n", server_read_user_write[1]);
	print_prompt(argv[1]); //prints user prompt
	memset(buf,'\0', MAX_MSG); //clears buffers before reading
	memset(read_buf,'\0', MAX_MSG);
	int nbytes; //allocates variable for number of bytes read in nonblocking I/O
	/* -------------- YOUR CODE STARTS HERE -----------------------------------*/
	while(1){
		usleep(500); //sleeps to reduce CPU usage
		// Poll stdin (input from the terminal) and send it to server (child process) via pipe
		if((nbytes = read(0,buf,MAX_MSG)) > 0){
		  buf[strlen(buf)-1] = '\0'; //strips newline character from stdin input
			//sends the stdin input to the child process for the server to eventually process
			if((write(server_read_user_write[1], buf, strlen(buf))) < 0){
				perror("Failed to write into pipe");
			}
			//clears buffer and prints prompt
			fflush(stdout);
		  memset(buf,'\0', MAX_MSG);
		  print_prompt(argv[1]);
		}


		// poll pipe retrieved and print it to stdout
		if((nbytes = read(server_write_user_read[0], read_buf, MAX_MSG)) > -1) {
			if(nbytes == 0){
				//broken pipe from server's child process, this window should exit
				printf("It seems the server has disconnected\n");
				fflush(stdout);
				exit(1);
			}
			//checks for extra credit segfault command
			if(strcmp(read_buf, "\\seg") == 0){
				char *n = NULL;
				*n = 1;
			}
			//prints input received from server to stdout
			printf("%s\n", read_buf);
			//clears stdout, resets buf, and prints prompt
			fflush(stdout);
			memset(read_buf,'\0', MAX_MSG);
			print_prompt(argv[1]);
		}
	}



	/* -------------- YOUR CODE ENDS HERE -----------------------------------*/
}

/*--------------------------End of main for the client --------------------------*/
