#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <glib.h>
#include <string.h>
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
	return *result;
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

int command_run (const scommand cmd, char *input, pipeline apipe) {
    char *cmd_arr[SIZE];
    bool wait = pipeline_get_wait(apipe);
	operator opp = scommand_get_operator(cmd);
	pid_t pid;
    int error_or_succes = 1;
	if (input != NULL) {
		scommand_set_redir_in(cmd,input);
	}
	if (opp == PIPELINE) { 			// Ve el caso de hacer un PIPE
		int pipefd[2];				// la idea con esto es hacer una
		if (pipe(pipefd) == -1) {	// llamada a command_run con el
			perror("pipe"); 		// siguiente comando del pipeline
		}
	}
	pid = fork();
	if (pid == -1) {
		perror("fork");

	} else if (pid == 0) {                            //Hijo
		cmd_arr[] = command_to_array(cmd);
		if (execvp(cmd_arr[0], cmd_arr) == -1)
        {
            error_or_succes = 0;
        }
        else {
         if (opp == PIPELINE) {
			cmd = scommand_destroy(cmd);
        	cmd = pipeline_front(apipe);
        	pipeline_pop_front(apipe);
            command_run(cmd, pipefd[0], apipe);
            close(pipefd[0]);
            close(pipefd[1]);
		    }   
        }
	}
    else if (pid > 0) {                                //Papi
        if (opp == PIPELINE) {
            close(pipefd[0]);
            dup2(1, pipefd[1]);                    //.............HACER ERRORES
            close(1);
            close(pipefd[1]);
        }
        char *out = scommand_get_redir_out(cmd);
        if (out != NULL)
        {
            int file_descriptor_out = open(out, O_WRONLY);
            dup2(1, file_descriptor_out);           //............HACER ERRORES
            close(1);                               //Qué cerrar????
            close(file_descriptor_out);             //???
        }
	    if (wait) {
	        wait(NULL);
    	}
    }

    scommand_destroy(cmd);
    return error_or_succes;
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
