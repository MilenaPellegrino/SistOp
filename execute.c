#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <unistd.h> //syscall
#include "execute.h"
#include "command.h"
#include "builtin.h"

#define SIZE 128

char *comm_inter[] = {
    "ls",
    "wc"
}; 

unsigned int long_comm  = sizeof(comm_inter) / sizeof(comm_inter[0]);

char *command_to_array (scommand command) {
	assert (command != NULL);
	char *result[SIZE];
	char *aux = NULL;
	guint length = scommand_length(command);
	for (guint i=0; i < length; i++) {
		aux = scommand_front(command);
		result[i] = malloc(strlen(aux)+1);
		strcpy(result[i], aux);
		scommand_pop_front(command);
	}
	aux = scommand_get_redir_in(command);
	result[length] = malloc(strlen(aux)+1);
	strcpy(result[length], aux);
	result[length+1] = NULL;
	return result;
}

bool is_command (const scommand cmd){
    assert(cmd!=NULL);
    bool is_command = false;
    bool matches = true;
    for(unsigned int i=0; i<long_comm && matches; i++){
        char *comm_act = comm_inter[i];
        if (strcmp(comm_act, scommand_front(cmd))){
            is_command = true;
            matches = false;
        }
    }
    return is_command;
}

void command_run (const scommand cmd, char *input, pipeline apipe) {
    bool wait = pipeline_get_wait(apipe);
	operator opp = scommand_get_operator(cmd);
	pid_t pid;
	if (input != NULL) {
		command_set_redir_in(cmd,input);
	}
	if (opp == PIPELINE) { 			// Ve el caso de hacer un PIPE
		int pipefd[2];				// la idea con esto es hacer una
		if (pipe(pipefd) == -1) {	// llamada a command_run con el
			perror("pipe");			// siguiente comando del pipeline
			exit(EXIT_FAILURE);
		}
	}
	pid=fork();
	if (pid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	} else if (pid == 0) {
		char *cmd_arr[SIZE] = command_to_array(cmd);
		execvp(cmd_arr[0], cmd_arr);
		if (opp == PIPELINE) {
			cmd = scommand_destroy(cmd);
        	cmd = pipeline_front(apipe);
        	pipeline_pop_front(apipe);
			// de alguna forma pasar el output del comando actual 
			// en el apartado char *input en la llamada del sig
			// command_run
		}
	}
	//
	//
	//COMPLETAR
	//Funcion open, close, y dup.
	//
	if (wait) {
		wait(NULL);
	}
}

void execute_pipeline(pipeline apipe){
    assert(apipe != NULL);
    scommand cmd;
    bool command=true, builtin=true;
    while (pipeline_length(apipe)!=0 && (command || builtin)) {
        cmd = pipeline_front(apipe);
        pipeline_pop_front(apipe);
        command = is_command(cmd);
        builtin = builtin_is_internal(cmd);
        if ((!command && !builtin) || (command && builtin)) {
            printf("error\n");
        }
        else if (builtin && !command){
            builtin_run(cmd);
        }
        else if (command && !builtin) { //syscall
			while (!pipeline_is_empty(apipe)) {
				command_run(cmd, NULL, apipe);
			}
        }
    }
}
