#include <stdio.h>
#include <stdbool.h>
#include "command.h"
#include <assert.h>
#include <string.h>
#include "builtin.h"
// En caso de que haya mas comandos internos se podrian agregar sin problemas
char *comm_inter[] = {
    "cd",
    "help",
    "exit"
}; 
// Obtenemos el tamano por si se agregan mas
unsigned int long_comm  = sizeof(comm_inter) / sizeof(comm_inter[0]);  

bool builtin_is_internal(scommand cmd){
    assert(cmd!=NULL);
    bool is_internal = false;
    for(unsigned int i =0; i<long_comm; i++){
        char *comm_act = comm_inter[i];
        if(strcmp(comm_act, scommand_front(cmd))){
            is_internal = true;
            break;
        }
    }
    return is_internal;
}

bool builtin_alone(pipeline p){
    assert(p!=NULL);
    bool pipe_alone = false; 
    if(pipeline_length(p) == 1 && builtin_is_internal(pipeline_front(p))){
        pipe_alone = true;
    }
    assert(pipeline_length(p) == 1 &&
           builtin_is_internal(pipeline_front(p)));
    return pipe_alone;
}

void builtin_run(scommand cmd){
    assert(builtin_is_internal(cmd));

}