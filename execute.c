#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <unistd.h> //syscall
#include "execute.h"
#include "command.h"
#include "builtin.h"

char *comm_inter[] = {
    "ls",
    "wc"
}; 

unsigned int long_comm  = sizeof(comm_inter) / sizeof(comm_inter[0]);

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

void execute_pipeline(pipeline apipe){
    assert(apipe != NULL);
    scommand cmd;
    bool wait = pipeline_get_wait(apipe);
    bool command=true, builtin=true;
    while (pipeline_length(apipe)!=0 && (command || builtin)) {
        cmd = pipeline_front(apipe);
        pipeline_pop_front(apipe);
        command = is_command(cmd);
        builtin = builtin_is_internal(cmd);
        if ((!command && !builtin) || (command && builtin)) {
            printf("error\n");
        }
        else if (command && !builtin) { //syscall
            command_run(cmd);
        }
        else if (builtin && !command){
            builtin_run(cmd);
        }
    }
    
}