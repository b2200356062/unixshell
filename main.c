#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <readline/readline.h>
#include <signal.h>

char** get_input(char* input){
	char** command = malloc(8 * sizeof(char *));
	if(command == NULL){
		perror("malloc failed\n");
		exit(1);
	}
	char* seperator = " ";
	char* parsed;
	int index = 0;

	parsed = strtok(input, seperator);
	while(parsed != NULL){
		command[index] = parsed;
		index++;
		parsed = strtok(NULL, seperator);
	}

	command[index] = NULL;
	return command;
}

int cd(char* path){
	return chdir(path);
}

void quit(){
	exit(1);
}

int main(){
	char **command;
	char* input;
	pid_t child_pid;
	int stat_loc;
	
	signal(SIGINT, SIG_IGN);

	while(1){
		input = readline("unix> ");
		command = get_input(input);

		if(strcmp(command[0], "quit") == 0|| strcmp(command[0], "exit") == 0){
			printf("exited shell\n");
			return 0;
		}

		if(strcmp(command[0], "cd") == 0){
			if(cd(command[1]) < 0){
				perror(command[1]);
			}
			continue;
		}

		child_pid = fork();
		if(child_pid < 0){
			perror("fork failed\n");
			exit(1);
		}
		if(child_pid == 0){

			signal(SIGINT, SIG_DFL);
			execvp(command[0], command);

			if(execvp(command[0], command) < 0){
				perror(command[0]);
				exit(1);
			}

			printf("this wont be printed if execvp is successful");
		}
		else{
			waitpid(child_pid, &stat_loc, WUNTRACED);
		}

		free(input);
		free(command);
	}

	// pid_t wait_result;


	// child_pid = fork();

	// if(child_pid == 0){
	// 	printf("## child ##\ncurrent pid: %d and child pid: %d\n", getpid(), child_pid);
	// 	sleep(1);
	// }
	// else{
	// 	wait_result = waitpid(child_pid, &stat_loc, WUNTRACED);
	// 	printf("## parent ##\ncurrent pid: %d, child pid: %d\n", getpid(), child_pid);
	// }

	return 0;
}
