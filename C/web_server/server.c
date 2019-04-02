#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "comm.h"
#include "util.h"

/* -----------Functions that implement server functionality -------------------------*/

/*
 * Returns the empty slot on success, or -1 on failure
 */
int find_empty_slot(USER * user_list) {
	// iterate through the user_list and check m_status to see if any slot is EMPTY
	// return the index of the empty slot
  int i = 0;
	for(i=0;i<MAX_USER;i++) {
    	if(user_list[i].m_status == SLOT_EMPTY) {
			return i;
		}
	}
	return -1;
}

/*
 * list the existing users on the server shell
 */
int list_users(int idx, USER * user_list)
{
	// iterate through the user list
	// if you find any slot which is not empty, print that m_user_id
	// if every slot is empty, print "<no users>""
	// If the function is called by the server (that is, idx is -1), then printf the list
	// If the function is called by the user, then send the list to the user using write() and passing m_fd_to_user
	// return 0 on success
	int i, flag = 0;
	char buf[MAX_MSG] = {}, *s = NULL;

	/* construct a list of user names */
	s = buf;
	strncpy(s, "---connected user list---\n", strlen("---connected user list---\n"));
	s += strlen("---connected user list---\n");
	for (i = 0; i < MAX_USER; i++) {
		if (user_list[i].m_status == SLOT_EMPTY)
			continue;
		flag = 1;
		strncpy(s, user_list[i].m_user_id, strlen(user_list[i].m_user_id));
		s = s + strlen(user_list[i].m_user_id);
		strncpy(s, "\n", 1);
		s++;
	}
	if (flag == 0) {
		strcpy(buf, "<no users>\n");
	} else {
		s--;
		strncpy(s, "\0", 1);
	}

	if(idx < 0) {
		printf("%s",buf);
		printf("\n");
	} else {
		/* write to the given pipe fd */
		if (write(user_list[idx].m_fd_to_user, buf, strlen(buf) + 1) < 0)
			perror("writing to server shell");
	}

	return 0;
}

/*
 * add a new user
 */
int add_user(int idx, USER * user_list, int pid, char * user_id, int pipe_to_child, int pipe_to_parent)
{
  // populate the user_list structure with the arguments passed to this function
  USER new_user;
  new_user.m_pid = pid;
  strcpy(new_user.m_user_id,user_id);
  new_user.m_fd_to_user = pipe_to_parent;
  new_user.m_fd_to_server = pipe_to_child;
  new_user.m_status = SLOT_FULL;
  user_list[idx] = new_user;
	// return the index of user added
	return idx;
}

/*
 * Kill a user
 */
void kill_user(int idx, USER * user_list) {
  int status = 0;
  int kill_signal;
  // kill a user (specified by idx) by using the systemcall kill()
  // then call waitpid on the user
  if((kill_signal = kill(user_list[idx].m_pid, SIGKILL)) != -1){
    waitpid(user_list[idx].m_pid, &status, 0);
  }else{
    perror("Failed to kill user");
  }
}

/*
 * Perform cleanup actions after the used has been killed
 */
 // typedef struct _userInfo {
 // 	int m_pid;
 // 	char m_user_id[MAX_USER_ID];
 // 	int m_fd_to_user;
 // 	int m_fd_to_server;
 // 	int m_status;
 // } USER;
void cleanup_user(int idx, USER * user_list){
  // m_pid should be set back to -1
  user_list[idx].m_pid = -1;

  // m_user_id should be set to zero, using memset()
  if((memset(user_list[idx].m_user_id, '\0', MAX_USER_ID)) == NULL){
    perror("Failed to clean up user");
  }

  // close all the fd
  // set the value of all fd back to -1
  if((close(user_list[idx].m_fd_to_user)) == -1){
    perror("Failed to close pipe to user");
  }else{
    user_list[idx].m_fd_to_user = -1;
  }
  if((close(user_list[idx].m_fd_to_server)) == -1){
    perror("Failed to close pipe to server");
  }else{
    user_list[idx].m_fd_to_server = -1;
  }

  // set the status back to empty
  user_list[idx].m_status = SLOT_EMPTY;
}

/*
 * Kills the user and performs cleanup
 */
void kick_user(int idx, USER * user_list) {
	// should kill_user()
  kill_user(idx, user_list);
	// then perform cleanup_user()
  printf("%s has left the chat\n",user_list[idx].m_user_id);
  cleanup_user(idx, user_list);
}

/*
 * broadcast message to all users
 */
int broadcast_msg(USER * user_list, char *buf, char *sender)
{
  //iterate over the user_list and if a slot is full, and the user is not the sender itself,
	//then send the message to that user
	//return zero on success
  for(int i = 0; i < MAX_USER; i++){
    if((user_list[i].m_status == SLOT_FULL) && (strcmp(user_list[i].m_user_id, sender) != 0)){
      if((write(user_list[i].m_fd_to_user, buf, strlen(buf))) == -1){
        perror("Failed to broadcast to users");
        return -1;
      }
    }
  }
	return 0;
}

/*
 * Cleanup user chat boxes
 */
void cleanup_users(USER * user_list)
{
  // go over the user list and check for any empty slots
  // call cleanup user for each of those users.
  for(int i = 0; i < MAX_USER; i++){
    if(user_list[i].m_status == SLOT_EMPTY){
      cleanup_user(i, user_list);
    }
  }
}

/*
 * find user index for given user name
 */
int find_user_index(USER * user_list, char * user_id)
{
	// go over the  user list to return the index of the user which matches the argument user_id
	// return -1 if not found
	int i, user_idx = -1;
	if (user_id == NULL) {
		return user_idx;
	}
	for (i=0;i<MAX_USER;i++) {
		if (user_list[i].m_status == SLOT_EMPTY)
			continue;
		if (strcmp(user_list[i].m_user_id, user_id) == 0) {
			return i;
		}
	}

	return -1;
}

/*
 * given a command's input buffer, extract name
 */
int extract_name(char * buf, char * user_name)
{
	char inbuf[MAX_MSG];
    char * tokens[16];
    strcpy(inbuf, buf);

    int token_cnt = parse_line(inbuf, tokens, " ");

    if(token_cnt >= 2) {
        strcpy(user_name, tokens[1]);
        return 0;
    }

    return -1;
}

//given a command's input buffer, extracts the text besides the
//command name and target name (ex. /<command> <target> <text> extracts text)
int extract_text(char *buf, char * text)
{
    char inbuf[MAX_MSG];
    char * tokens[16];
    char * s = NULL;
    strcpy(inbuf, buf);

    int token_cnt = parse_line(inbuf, tokens, " ");

    if(token_cnt >= 3) {
        //Find " "
        s = strchr(buf, ' ');
        s = strchr(s+1, ' ');

        strcpy(text, s+1);
        return 0;
    }

    return -1;
}

//sends a message (buf) to the user at index idx in init_user_list
//from sender
void send_p2p_msg(int sender, int idx, USER * user_list, char *buf)
{
	// get the target user by name using extract_name() function
  char sender_name[MAX_USER_ID];
  memset(sender_name,'\0', MAX_USER_ID);
  //in case admin p2p's a user, specifies admin name
  if(sender == -1){
    strcpy(sender_name,"admin");
  }else{
    strcpy(sender_name,user_list[sender].m_user_id);
  }
  //gets target's name using extract_name
  char target_user[MAX_USER_ID];
  memset(target_user,'\0', MAX_USER_ID);
  extract_name(buf,user_list[idx].m_user_id);
  if((extract_name(buf,target_user)) != -1){
    // find the user id using find_user_index()
    int target;
    if((target = find_user_index(user_list, target_user)) != -1){
      //builds up the message contents (inbuf) by concatenating
      //multiple strings and buffers to match the form
      //<sender>: <message>
      char contents[MAX_MSG];
      memset(contents,'\0',MAX_MSG);
      extract_text(buf,contents);
      char *s, inbuf[strlen(contents) + strlen(sender_name) + strlen(": ")];
      memset(inbuf,'\0', strlen(contents) + strlen(sender_name) + strlen(": "));
      s = inbuf;
      strncpy(s,sender_name,strlen(sender_name));
      s += strlen(sender_name);
      strncpy(s,": ",strlen(": "));
      s += strlen(": ");
      strncpy(s,contents,strlen(contents));
      	// if the user is found then write the message that the user wants to send to that user.
      if((write(user_list[target].m_fd_to_user, inbuf, strlen(contents) + strlen(sender_name) + strlen(": "))) != -1){
        //successful write
      }else{
        //unsuccessful write
        if((write(user_list[idx].m_fd_to_user, "Failed to write to user\n", strlen("Failed to write to user\n"))) == -1){
        }
      }
    }else{
      // if user not found, write back to the original user "User not found", using the write()function on pipes.
      if((write(user_list[idx].m_fd_to_user, "User index not found\n", strlen("User index not found\n"))) == -1){
        perror("Failed to report error to user");
      }
    }
  }else{
    // if user not found, write back to the original user "User not found", using the write()function on pipes.
    if((write(user_list[idx].m_fd_to_user, "User not found\n", strlen("User not found\n"))) == -1){
      perror("Failed to report error to user");
    }
  }
}

//takes in the filename of the file being executed, and prints an error message stating the commands and their usage
void show_error_message(char *filename)
{
  int fd;
  if((fd = open(filename,O_WRONLY)) != -1){
    if((write(fd, "\\list : lists users\n", strlen("\\list : lists users\n"))) == -1){
      perror("Failed to write list usage to user");
    }
    if((write(fd, "\\exit : disconnect from the chat\n", strlen("\\exit : disconnect from the chat\n"))) == -1){
      perror("Failed to write exit usage to user");
    }
    if((write(fd, "\\p2p <username> <message> : writes <message> to <username>\n",
     strlen("\\p2p <username> <message> : writes <message> to <username>\n"))) == -1){
      perror("Failed to write p2p usage to user");
    }
    if((write(fd, "\\<any-other-text> : writes <any-other-text> to all users\n",
     strlen("\\<any-other-text> : writes <any-other-text> to all users\n"))) == -1){
      perror("Failed to write <any-other-text> usage to user");
    }
  }else{
    perror("Failed to write to file");
  }
}


/*
 * Populates the user list initially
 */
void init_user_list(USER * user_list) {

	//iterate over the MAX_USER
	//memset() all m_user_id to zero
	//set all fd to -1
	//set the status to be EMPTY
	int i=0;
	for(i=0;i<MAX_USER;i++) {
		user_list[i].m_pid = -1;
		memset(user_list[i].m_user_id, '\0', MAX_USER_ID);
		user_list[i].m_fd_to_user = -1;
		user_list[i].m_fd_to_server = -1;
		user_list[i].m_status = SLOT_EMPTY;
	}
}

//checks to make sure buf doesn't only contain spaces,
//as this will segfault in the parsing program
//returns -1 if there is a non space character and
// 1 if the buffer will segfault while parsing
int check_segfault_entry(char* buf){
  for(int i = 0; i < strlen(buf); i++){
    if(((int)((char)buf[i]) != 10 && (int)((char)buf[i]) != 32)){
      return -1;
    }
  }
  return 1;
}

/* ---------------------End of the functions that implementServer functionality -----------------*/


/* ---------------------Start of the Main function ----------------------------------------------*/
int main(int argc, char * argv[])
{
	int nbytes; //number of bytes read in polling reads
	setup_connection("GAVINPETERADAM"); // Specifies the connection point as argument.
	USER user_list[MAX_USER]; //user list
	init_user_list(user_list);   // Initialize user list
	char buf[MAX_MSG], pipe_buf[MAX_MSG]; //allocating buffers for stdin and pipe reads
	fcntl(0, F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK); //sets stdin to nonblocking
	print_prompt("admin"); //prints initial prompt
  memset(buf,'\0', MAX_MSG); //clears the buffers to be read into
  memset(pipe_buf,'\0', MAX_MSG);
  int child_read_user_write[2]; //declares arrays for pipes between the server and user
  int child_write_user_read[2]; //used in get_connection
  char user_id[MAX_USER_ID]; //array allocated to receive the output parameter of get_connection
  int user; //checks return value of get connection
	while(1) {
		/* ------------------------YOUR CODE FOR MAIN--------------------------------*/
    usleep(500); //sleeps to reduce CPU usages



    // Check max user and same user id


    // Handling a new connection using get_connection
    if((user = get_connection(user_id, child_write_user_read, child_read_user_write)) > -1){
      //allocating variables to be used in parent/child processes
      pid_t pid;
      int status = 0;
      int server_write_child_read[2];
      int server_read_child_write[2];
      int slot = find_empty_slot(user_list); //checks to make suer
      // Server process: Add a new user information into an empty slot
      if(slot > -1 && find_user_index(user_list,user_id) == -1){
        //sets up pipes and adds user to the user list
        close(child_write_user_read[0]);
        close(child_read_user_write[1]);
        add_user(slot, user_list, getpid(), user_id, child_read_user_write[1], child_write_user_read[0]);
        printf("%s joined the chat, slot: %d\n",user_id,slot);
        print_prompt("admin");
        if((pipe(server_write_child_read)) == -1 || (pipe(server_read_child_write)) == -1){
          perror("Failed to create pipe");
        }
        //sets pipes to be nonblocking
        int user_flags = fcntl(user_list[slot].m_fd_to_server, F_GETFL, 0);
        int server_flags = fcntl(server_write_child_read[0], F_GETFL, 0);
        int child_flags = fcntl(child_read_user_write[0], F_GETFL, 0);
        fcntl(user_list[slot].m_fd_to_server, F_SETFL, user_flags | O_NONBLOCK);
        fcntl(server_write_child_read[0], F_SETFL, server_flags | O_NONBLOCK);
        fcntl(child_read_user_write[0], F_SETFL, child_flags | O_NONBLOCK);
        // creating the child process
        if((pid = fork()) == 0){
          //close appropriate ends of the pipe, allocates
          //buffers for nonblocking reads, and empties them
          //initially
          close(server_write_child_read[1]);
          close(server_read_child_write[0]);
          int bytes_read_from_server, bytes_read_from_user, written;
          char input_from_server[MAX_MSG], input_from_user[MAX_MSG];
          memset(input_from_user,'\0', MAX_MSG);
          memset(input_from_server,'\0', MAX_MSG);
          while(1){
            // Child process: poll users and SERVER
            usleep(500);
            if(((bytes_read_from_server = read(server_write_child_read[0], input_from_server, MAX_MSG)) > -1)
              || ((bytes_read_from_user = read(child_read_user_write[0], input_from_user, MAX_MSG)) > -1)){
              if(bytes_read_from_server == 0){
                //broken pipe from central server
                close(server_write_child_read[0]);
                close(server_read_child_write[1]);
                close(child_write_user_read[1]); // close the pipes to the user so it knows to exit
                close(child_read_user_write[0]);
                close(child_write_user_read[0]);
                close(child_read_user_write[1]);
                exit(1);
              }
              if(bytes_read_from_user == 0){
                //broken pipe because user process has terminated, need to terminate Child
                //sends the child the exit command so it exits
                char * k_command = "\\exit";
                if((write(server_read_child_write[1], k_command, strlen(k_command))) == -1){
                  perror("Failed to exit");
                }
                close(child_read_user_write[0]);
                close(child_write_user_read[1]);
                close(server_write_child_read[0]);
                close(server_read_child_write[1]);
                close(child_write_user_read[0]); // close the pipes to the user so it knows to exit
                close(child_read_user_write[1]);
                exit(1);
              }

              if(bytes_read_from_server > 1){
                //writes input from server to the user
                if((written = write(child_write_user_read[1],input_from_server, strlen(input_from_server))) == -1){
                    perror("Failed to write to user");
                }
              }
              if(bytes_read_from_user > 1){
                //writes input from user to the server
                if((write(written = server_read_child_write[1],input_from_user, strlen(input_from_user))) == -1){
                    perror("Failed to write to server");
                }
              }
            //clears buffers
            memset(input_from_server,'\0', MAX_MSG);
            memset(input_from_user,'\0', MAX_MSG);
            }
          }
        }else if(pid > 0){
          //updates the child process pid and closes appropriate pipes
          user_list[slot].m_pid = pid;
          close(child_read_user_write[0]);
          close(child_write_user_read[1]);
        }else{
          perror("Failed to fork");
        }
      }else{
        close(child_read_user_write[0]);
        close(child_write_user_read[1]);
        printf("User attempted to join using an existing user's ID or the chat is full\n");
        fflush(stdout);
        print_prompt("admin");
      }
    }

    // Poll stdin (input from the terminal) and handle admnistrative command
    if((nbytes = read(0,buf,MAX_MSG)) > 0){
      //checks if buffer is only spaces (causes segfault when parsing)
      if(check_segfault_entry(buf) == 1){
        //will segfault if parsed, ignores this input
        print_prompt("admin");
        memset(buf,'\0', MAX_MSG);
      }else{
        //will not segault while parsing, processes the input
        char buf_copy[strlen(buf)]; //copy of the buf that is preserved through parsing
        strcpy(buf_copy, buf);
        //parses and processes the input buf into tokens
        char * tokens[MAX_MSG];
        int num_tokens;
        buf[strlen(buf)-1] = '\0';
        buf_copy[strlen(buf_copy) - 1] = '\0';
        num_tokens = parse_line(buf,tokens," ");
        int command_num;
        command_num = get_command_type(buf); //gets the command from the parsed input
        //chooses the appropriate command and executes accordingly
        switch(command_num){
          case LIST:
          //lists users
            list_users(-1,user_list);
            print_prompt("admin");
            break;
          case KICK:
            ;
            //kicks a user
            int to_be_kicked = find_user_index(user_list, tokens[1]);
            if(to_be_kicked > -1){
              print_prompt("admin");
              kick_user(to_be_kicked,user_list);
              print_prompt("admin");
            }else{
              printf("Usage: \\kick <user>\n");
              fflush(stdout);
              print_prompt("admin");
            }
            break;
          case P2P:
            ;
            int recipient = find_user_index(user_list, tokens[1]);
            if(recipient > -1){
              send_p2p_msg(-1, recipient, user_list, buf_copy);
              print_prompt("admin");
            }else{
              printf("Usage: \\p2p <user> <message>\n");
              fflush(stdout);
              print_prompt("admin");
            }
            // have to get the recipient id from the user list
            break;
          case SEG:
            ;
            int to_be_segfaulted = find_user_index(user_list, tokens[1]);
            if(to_be_segfaulted > -1){
              write(user_list[to_be_segfaulted].m_fd_to_user,"\\seg",strlen("\\seg"));
            }else{
              printf("Usage: \\seg <user>\n");
              fflush(stdout);
            }
            print_prompt("admin");
            //extra credit
            break;
          case EXIT:
            for(int j = 0; j < MAX_USER; j++){
              if(user_list[j].m_status == SLOT_FULL){
                kick_user(j,user_list);
              }
            }
            kill(getpid(), SIGKILL);
            break;
          default: //no command given, will broadcast to all users
            //write to all open pipes using special admin-notice
            //to indicate a public announcement
            ;
            //string building
            char *s, broadcast_msg_text[MAX_MSG + strlen("admin-notice: ")];
            memset(broadcast_msg_text,'\0', MAX_MSG + strlen("admin-notice: "));
            s = broadcast_msg_text;
            strncpy(s,"admin-notice: ",strlen("admin-notice: "));
            s += strlen("admin-notice: ");
            strncpy(s,buf_copy,strlen(buf_copy));
            //sends message to all users
            broadcast_msg(user_list, broadcast_msg_text, "admin-notice");
            print_prompt("admin");
        }
        //resets buf
        memset(buf,'\0', MAX_MSG);
        fflush(stdout);
      }
    }

    //poll child processes and handle user commands
    for(int i = 0; i < MAX_USER; i++){
      //checks to make sure a user is in the slot
      if(user_list[i].m_status == SLOT_FULL){
        //checks the associated child process' pipe for input
        if((nbytes = read(user_list[i].m_fd_to_server, pipe_buf, MAX_MSG)) > 0){
          //checks if buffer is only spaces (causes segfault when parsing)
          if(check_segfault_entry(pipe_buf) == 1){
            //input will cause segfault, ignores input
            print_prompt("admin");
            memset(pipe_buf,'\0', MAX_MSG);
          }else{
            //input will not cause segfault, processes pipe_buf input
            char buf_copy[strlen(pipe_buf)]; //copies pipe_buf so it is preserved through parsing
            strcpy(buf_copy, pipe_buf);
            //tokenizes the input
            char * tokens[MAX_MSG];
            int num_tokens;
            num_tokens = parse_line(pipe_buf,tokens," ");
            int command_num;
            //gets the command
            command_num = get_command_type(pipe_buf);
            switch(command_num){
              case LIST:
              //lists users in the client window
                list_users(i,user_list);
                break;
              case P2P:
              //sends a message to another user person to person
                ;
                int recipient = find_user_index(user_list, tokens[1]);
                if(recipient == i){
                  //user tried to send a message to itself
                  write(user_list[i].m_fd_to_user, "Usage: \\p2p <user> <message> (you cannot send a message to yourself)",
                  strlen("Usage: \\p2p <user> <message> (you cannot send a message to yourself)"));
                }
                else if(recipient > -1){
                  //successful sending of message
                  send_p2p_msg(i, recipient, user_list, buf_copy);
                }else{
                  //user tried to send a message to an invalid username
                  write(user_list[i].m_fd_to_user, "Usage: \\p2p <user> <message> (invalid recipient)",
                   strlen("Usage: \\p2p <user> <message> (invalid recipient)"));
                }
                break;

              case EXIT:
                //Exits user from server using kick_user
                ;
                kick_user(i,user_list);
                print_prompt("admin");
                break;
              case SEG:
                //segfaults user if they send the command
                if((write(user_list[i].m_fd_to_user,"\\seg",strlen("\\seg")) != -1)){
                  printf("Failed to segfault %s\n",user_list[i].m_user_id);
                }
                break;
              default:
                //writes to all users in chat using broadcast_msg
                //will output in form <user>: <message>
                printf("%s: %s\n",user_list[i].m_user_id,buf_copy); //prints to admin window
                fflush(stdout);
                //string building for writing to users
                char *s, broadcast_msg_text[MAX_MSG + strlen(user_list[i].m_user_id) + strlen(": ")];
                memset(broadcast_msg_text,'\0', MAX_MSG + strlen(user_list[i].m_user_id) + strlen(": "));
                s = broadcast_msg_text;
                strncpy(s,user_list[i].m_user_id,strlen(user_list[i].m_user_id));
                s += strlen(user_list[i].m_user_id);
                strncpy(s,": ",strlen(": "));
                s += strlen(": ");
                strncpy(s,buf_copy, strlen(buf_copy));
                //writes to all users
                broadcast_msg(user_list,broadcast_msg_text,user_list[i].m_user_id);
                print_prompt("admin");
                break;
          }
          //resets pipe_buf and flushes stdout
          memset(pipe_buf,'\0', MAX_MSG);
          fflush(stdout);
        }
      }
    }
  }


		/* ------------------------YOUR CODE FOR MAIN--------------------------------*/
	}
}

/* --------------------End of the main function ----------------------------------------*/
