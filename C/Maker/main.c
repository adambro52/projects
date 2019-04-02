/* CSci4061 F2018 Assignment 1
* login: celic005
* date: 10/5/2018
* name: Gavin Celichowski (celic005), Peter Van Dyke (vandy123), Adam Smith (smit8796)
* id: 5177967 , 5298742 , 5135927 */

// This is the main file for the code
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "util.h"

/*-------------------------------------------------------HELPER FUNCTIONS PROTOTYPES---------------------------------*/
void show_error_message(char * ExecName);
//Write your functions prototypes here
void show_targets(target_t targets[], int nTargetCount);
/*-------------------------------------------------------END OF HELPER FUNCTIONS PROTOTYPES--------------------------*/


/*-------------------------------------------------------HELPER FUNCTIONS--------------------------------------------*/

//This is the function for writing an error to the stream
//It prints the same message in all the cases
void show_error_message(char * ExecName)
{
	fprintf(stderr, "Usage: %s [options] [target] : only single target is allowed.\n", ExecName);
	fprintf(stderr, "-f FILE\t\tRead FILE as a makefile.\n");
	fprintf(stderr, "-h\t\tPrint this message and exit.\n");
	exit(0);
}

//Write your functions here

//Phase1: Warmup phase for parsing the structure here. Do it as per the PDF (Writeup)

/* show_targets iterates through the array of targets, and prints out each part
* of the target struct to the terminal
*/
void show_targets(target_t targets[], int nTargetCount)
{
	for(int i=0;i<nTargetCount;i++){
		printf("TargetName: %s\n", targets[i].TargetName);
		printf("DependencyCount: %d\n", targets[i].DependencyCount);
		printf("DependencyNames: ");
		for(int j=0; j<targets[i].DependencyCount;j++){
			printf("%s ",targets[i].DependencyNames[j]);
		}
		printf("\n");
		printf("Command: %s \n\n", targets[i].Command);
	}

}

/*
 * Now, the file has been parsed and the targets have been named.
 * You'll now want to check all dependencies (whether they are
 * available targets or files) and then execute the target that
 * was specified on the command line, along with their dependencies,
 * etc. Else if no target is mentioned then build the first target
 * found in Makefile.
 */

 //get_num_tokens is very similar to the provided function parse_into_tokens
 //but it only counts the number of tokens separated by the delimeter,
 //and does not write to any pointer/array
 int get_num_tokens(const char const* input_string, char* delim){
	int i=0;
 	char temp[ARG_MAX];
 	strcpy(temp,input_string);
 	char *tok = strtok(temp, delim);
 	while(tok!=NULL && i<ARG_MAX)
 	{
 		i++;
 		tok = strtok(NULL, delim);
 	}
	return i;
 }

 //execute handles the fork/wait/exec behavior
 //build gives execute a target, and if the target needs to
 //be built, execute parses the command's string into tokens,
 //prints the command to the terminal (stdout), forks a child
 //process. The parent process waits for the child process to
 //terminate, and either sets the target's status as FINISHED
 //if the child terminates normally, or prints out an error
 //message with the correct error code. The child process
 //calls execvp to execute the target's command, and
 //exits upon an execution error
 int execute(target_t target){
	char temp_cmd[sizeof(target.Command)];
	strcpy(temp_cmd,target.Command);
 	int status = 0;
 	int pid;
 	if(target.Status == NEEDS_BUILDING){
 		char* delim = " ";
 		int num_tokens = get_num_tokens(target.Command, delim);
 		char* tokens[num_tokens+1];
 		num_tokens = parse_into_tokens(target.Command, tokens, delim);
 		int exec_success = -5;
 		char *unix_cmd = tokens[0];
 		//prints the command and executes it
 		//prints an error message and exits if an error occurs
 		printf("%s\n",temp_cmd);
 		pid = fork();
		//child process
 		if(pid == 0){
 			exec_success = execvp(unix_cmd, tokens); //exec call
 			if(exec_success != -5){
 				perror("Failed to execute command");
 				exit(-1);
 			}
 		}
		//parent process
 		else{
 			if(pid > 0){
 				pid = waitpid(pid, &status, 0); //waits for child to finish
 				if(WEXITSTATUS(status) != 0){
 					printf("Child exited with error code = %d\n", WEXITSTATUS(status));
 					exit(-1);
 				}
 				target.Status = FINISHED; //indicates successful execution
 			}else{
 				perror("Failed to fork");
 			}
 		}
 	}
 }

/* Build mimics the GNU make utility. It recursively calls itself
 * on any dependencies in the makefile, and then calls execute
 * in a LIFO stack-like fashion to mimic GNU make's behavior
 */
int build(int index, target_t* targets, int nTargetCount){
 target_t target = targets[index];
 int i;
 //always want to run clean
 if(target.DependencyCount == 0){
	 target.Status = NEEDS_BUILDING;
 }
 //iterates through the target's dependencies, and if they have been
 //modified more recently than the target, build indicates that the target
 //needs to be built, and then it recursively calls itself using the current
 //dependency as the new target if a target exists for the dependency
 for(i = 0; i < target.DependencyCount; i++){
	int child_index = find_target(target.DependencyNames[i], targets, nTargetCount);
		int comparison = compare_modification_time(target.DependencyNames[i], target.TargetName);
		if(comparison == -1 || comparison == 1){
			target.Status = NEEDS_BUILDING; //indicates target needs to be built in execute()
			if(child_index != -1){
					build(child_index, targets, nTargetCount); //recursive call on dependency
			}
		}
	}
	execute(target); //call to execute current target
}








/*-------------------------------------------------------END OF HELPER FUNCTIONS-------------------------------------*/


/*-------------------------------------------------------MAIN PROGRAM------------------------------------------------*/
//Main commencement
int main(int argc, char *argv[])
{
  target_t targets[MAX_NODES];
  int nTargetCount = 0;

  /* Variables you'll want to use */
  char Makefile[64] = "Makefile";
  char TargetName[64];

  /* Declarations for getopt */
  extern int optind;
  extern char * optarg;
  int ch;
  char *format = "f:h";
  char *temp;

  //Getopt function is used to access the command line arguments. However there can be arguments which may or may not need the parameters after the command
  //Example -f <filename> needs a finename, and therefore we need to give a colon after that sort of argument
  //Ex. f: for h there won't be any argument hence we are not going to do the same for h, hence "f:h"
  while((ch = getopt(argc, argv, format)) != -1)
  {
	  switch(ch)
	  {
	  	  case 'f':
	  		  temp = strdup(optarg);
	  		  strcpy(Makefile, temp);  // here the strdup returns a string and that is later copied using the strcpy
	  		  free(temp);	//need to manually free the pointer
	  		  break;

	  	  case 'h':
	  	  default:
	  		  show_error_message(argv[0]);
	  		  exit(1);
	  }

  }

  argc -= optind;
  if(argc > 1)   //Means that we are giving more than 1 target which is not accepted
  {
	  show_error_message(argv[0]);
	  return -1;   //This line is not needed
  }

  /* Init Targets */
  memset(targets, 0, sizeof(targets));   //initialize all the nodes first, just to avoid the valgrind checks

  /* Parse graph file or die, This is the main function to perform the toplogical sort and hence populate the structure */
  if((nTargetCount = parse(Makefile, targets)) == -1)  //here the parser returns the starting address of the array of the structure. Here we gave the makefile and then it just does the parsing of the makefile and then it has created array of the nodes
	  return -1;


  //Phase1: Warmup-----------------------------------------------------------------------------------------------------
  //Parse the structure elements and print them as mentioned in the Project Writeup
  /* Comment out the following line before Phase2 */
  //show_targets(targets, nTargetCount);

  //End of Warmup------------------------------------------------------------------------------------------------------

  /*
   * Set Targetname
   * If target is not set, set it to default (first target from makefile)
   */

  if(argc == 1)
	strcpy(TargetName, argv[optind]);    // here we have the given target, this acts as a method to begin the building
  else
  	strcpy(TargetName, targets[0].TargetName);  // default part is the first target



  //Phase2: Begins ----------------------------------------------------------------------------------------------------
  /*Your code begins here*/

	//locates first target and calls build on it
	int first_target_idx = find_target(TargetName, targets, nTargetCount);
	build(first_target_idx, targets, nTargetCount);



  /*End of your code*/
  //End of Phase2------------------------------------------------------------------------------------------------------

  return 0;
}
/*-------------------------------------------------------END OF MAIN PROGRAM------------------------------------------*/
