#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <glib.h>
#include <string.h>
#include <unistd.h> //syscall
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>


#include "execute.h"
#include "command.h"
#include "builtin.h"

#define SIZE 128

char *intern_comm[] = {
    "ls",
    "wc"
}; 

unsigned int comm_long  = sizeof(intern_comm) / sizeof(intern_comm[0]);

char **command_to_array (scommand command) {
	assert (command != NULL);
	char **result = malloc((scommand_length(command)+2)*sizeof(char *));
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
    for(unsigned int i=0; i<comm_long && matches; i++){
        char *comm_act = intern_comm[i];
        if (strcmp(comm_act, scommand_front(cmd)) == 0){
            is_command = true;
            matches = false;
        }
    }
    return is_command;
}

int command_run (scommand cmd, int fd, pipeline apipe) {
    char **cmd_arr;
    int aux_pointer =0; 
    bool waiting = pipeline_get_wait(apipe);
	operator opp = scommand_get_operator(cmd);
	pid_t pid;
    int error_or_succes = 1;
    char *out = scommand_get_redir_out(cmd);
    int status;

    if (opp == PIPELINE) {
        int pipefd[2];				// la idea con esto es hacer una
        if (pipe(pipefd) == -1) {	// llamada a command_run con el
			perror("pipe");
        }
        pid = fork();
        if (fd > 1) {
            dup2(0, fd);
        }
        if (pid == -1) {
            perror("fork");
	    } else if (pid == 0) {                            //Hijo
            cmd_arr = command_to_array(cmd);
            if (execvp(cmd_arr[0], cmd_arr) == -1) {
                error_or_succes = 0;
            } else {
                cmd = scommand_destroy(cmd);
                cmd = pipeline_front(apipe);
                pipeline_pop_front(apipe);
                aux_pointer = pipefd[0];
                command_run(cmd, aux_pointer, apipe);
                close(pipefd[0]);
                close(pipefd[1]);
            }
	    } else if (pid > 0) {                                //Papi
            close(pipefd[0]);
            dup2(1, pipefd[1]);    // este no iria al reves? : dup2(pipefd[1], 1)               
            close(1);
            close(pipefd[1]);
            if (out != NULL) {
                int file_descriptor_out = open(out, O_WRONLY);
                dup2(1, file_descriptor_out);           //............HACER ERRORES
                close(1);                               //Qué cerrar????
                close(file_descriptor_out);             //???
            }
            if (waiting) {
                wait(&status);
            }
        }
    }

    else if (opp == DOBLE_AMPERSAND) {
        pid = fork();
        if (fd > 1) {
            dup2(0, fd);
        }
        if (pid == -1) {
            perror("fork");
	    } else if (pid > 0) {                                //Papi
            if (out != NULL) {
                int file_descriptor_out = open(out, O_WRONLY);
                dup2(1, file_descriptor_out);           //............HACER ERRORES
                close(1);                               //Qué cerrar????
                close(file_descriptor_out);             //???
            }
            if (waiting) {
                wait(&status);
            }
	    } else if (pid == 0) {                            //Hijo
            cmd_arr = command_to_array(cmd);
            if (execvp(cmd_arr[0], cmd_arr) == -1) {
                error_or_succes = 0;
            } else {
                cmd = scommand_destroy(cmd);
                cmd = pipeline_front(apipe);
                pipeline_pop_front(apipe);
                command_run(cmd, 0, apipe);
            }
        }
    }

    scommand_destroy(cmd);
    return error_or_succes;
}

void execute_pipeline(pipeline apipe){
    assert(apipe != NULL);
    scommand cmd;
    bool command=true, builtin=true;
    if (pipeline_is_empty(apipe)) { // caso base
        return;
    }
    if (builtin_alone(apipe)) {     // caso base
        return;
    }

    while (pipeline_length(apipe)!=0 && (command || builtin)) {
        cmd = pipeline_front(apipe);
        pipeline_pop_front(apipe);
        command = is_command(cmd);
        builtin = builtin_is_internal(cmd);
        if (command && !builtin) { //syscall
			while (!pipeline_is_empty(apipe)) {
				command_run(cmd, 0, apipe);
			}
        }
        else if (builtin && !command){
            builtin_run(cmd);
        }
        else if ((!command && !builtin) || (command && builtin)) {
            printf("error\n");
        }
    }
}
