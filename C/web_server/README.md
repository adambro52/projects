CSci4061 F2018 Assignment 2
section: 1
date: 11/06/18
name: Peter Van Dyke, Gavin Celichowski, Adam Smith
id: 5298742, 5177967, 5135927



1. The purpose of your program
This program is meant to be a multi-process chat application. Essentially, its a chat room that runs on a single machine that creates processes for each user and allows them to communicate with each other. There are 2 types of users in the chat:
up to 10 "users" with the ability to join/leave the chat, list the users in the chat, chat to all users in a broadcast fashion,
and chat privately to a single other user. The "admin" is the main server's user, and it has all of the abilities of a user plus
the ability to segfault a user, kick a user from the chat room, and exit the whole chat room, exiting all users and closing the chat room.


2. A brief description of who did what on the lab
We did almost all of the work together. We would use the "teletype" feature in the Atom editor to work together and see live updates on the code. Teletype functions like a Google Document, but for code.

3. How to compile the program
To compile the program, run "make" from the terminal in the project folder with the provided makefile. It is also
recommended to run 'make clean' before running 'make'

4. How to use the program from the shell (syntax)

To start the program, type ./server in the shell. This terminal will act as the chatroom admin. To add users type
./client <client name>
in a new terminal. Assuming that the chatroom isn't full and that the name isn't taken, this will add a new users in that terminal. The server must be running before ./client <user> can be run!

The client commands are as follows:
To send a message to from user to user, type \p2p <target user> <message text> (cannot send p2p to admin)
To list all the users in the chatroom, type \list
To disconnect to the chatroom, type \exit
To segfault the client, type \seg
To broadcast a message to all users and the admin, simply type your message

The server commands are as follows:
To list all the users in the chatroom, type \list
To send a message to a user, type \p2p <target user> <message text>
To terminate all user sessions and close the chatroom, type \exit
To kick a user, type \kick <user> (cannot kick admin)
To segfault a specific user, type \seg <user> (cannot seg admin)
To broadcast a message to all users, simply type your message (will have special admin-notice name)


5. What exactly your program does
It allows each users to send a direct message, broadcast a message, exit the chatroom, and see
who else is connected. It also provides functionality for the admin to kick a user, list all the
users, segfault a user, broadcast a message to all users, send a message to a specific user and exit.

When a user is added, it creates a middleman child process between the user and central server. It also makes 2 non blocking pipes between the child process and server and makes 2 non blocking pipes between the user process and child process (allowing for bidirectional communication). Our program uses a switch statement for handling specific commands. The initial connection
is originally directly between the main server process and the user process, but this connection is closed and all
server-client and client-server communication is directed through the child process to protect the server. The user, main server, and child server processes all poll using nonblocking read calls.

The user and main server poll commands from stdin and pipes between their respective process and the associated child process, and the child poll using nonblocking read calls on the pipes between it and the server, and it and its associated user process. All communication is organized using 2 pipes so that only one process reads and the other writes, and the other pipe has the opposite read/write ends open to make sure two way communication is correct. When a user is sent a \seg (from the server), \kick (from the server), \exit (in the user window), or uses a ^C interrupt (user window), the user process is killed, and so is the associated child process in the server (the message is received through the SIGPIPE signal when the pipes close). When the main server process exits via a \exit in the main server or a ^C interrupt in the main server, the main server process, as well as all child processes and user processes terminate and are cleaned up to prevent zombies/orphans.

6. Any explicit assumptions you have made
We assume the maximum number of users is 10, maximum message is 256 characters, and the maximum username length is 32 characters.


7. Your strategies for error handling
For errors, we have if statements throughout our program that check the return value of system calls. We print out statements describing the errors as they appear. If the user (client or admin) fails to execute a command correctly, the program will
print out the correct usage for the selected command. If the admin program experiences a SIGINT (^C), it will exit, and it will
also exit all of the children. If the child receives a \seg from the admin or a SIGINT (^C), it exits, and the admin cleans up
its associated child process.
