#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "strextra.h"

#include "command.h"

typedef struct scommand_s {
    GQueue scomman;
    char* out;
    char* in;
}


scommand scommand_new(void){
    scommand result = malloc(sizeof(struct scommand_s));
    assert(result!= NULL);
    result->scomman = NULL;
    result->out = NULL;
    result->in = NULL;
    assert(scommand_is_empty(result) &&  scommand_get_redir_in (result) &&  scommand_get_redir_out (result));
    return result;
}

scommand scommand_destroy(scommand self){
    assert(self!= NULL);
    g_queue_free_full(self->scomman, g_free);
    free(out);
    free(in);
    free(self);
    self = NULL;
    return self;
}

void scommand_push_back(scommand self, char * argument){
    assert(self!=NULL && argument!=NULL); 
    g_queue_push_tail(self->scomman, argument);
    assert(!scommand_is_empty());
}

void scommand_pop_front(scommand self){
    assert(self!=NULL && !scommand_is_empty(self));
    gpointer kill_data =  g_queue_pop_head(self->scomman);
    g_free(kill_data);
}

void scommand_set_redir_in(scommand self, char * filename){

}

void scommand_set_redir_out(scommand self, char * filename){

}

bool scommand_is_empty(const scommand self){

}

unsigned int scommand_length(const scommand self){


}

char * scommand_front(const scommand self){

}

char * scommand_get_redir_in(const scommand self){
    assert(self!=NULL);
    return self.in;
}

char * scommand_get_redir_out(const scommand self){
    assert(self!=NULL);
    return self.out;
}

char * scommand_to_string(const scommand self){

}


pipeline pipeline_new(void){


}

pipeline pipeline_destroy(pipeline self){

}

void pipeline_push_back(pipeline self, scommand sc){

}

void pipeline_pop_front(pipeline self){

}

void pipeline_set_wait(pipeline self, const bool w){

}

bool pipeline_is_empty(const pipeline self){

}

unsigned int pipeline_length(const pipeline self){

}

scommand pipeline_front(const pipeline self){

}

bool pipeline_get_wait(const pipeline self){

}

char * pipeline_to_string(const pipeline self){

}


