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
    result->scomman = g_queue_new();
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

//JUANCHO
void scommand_set_redir_in(scommand self, char * filename){
    assert(self != NULL);
    self->in = filename;
    free(filename);
}

void scommand_set_redir_out(scommand self, char * filename){
    assert(self != NULL);
    self->out = filename;
    free(filename);
}
//
//
bool scommand_is_empty(const scommand self){
    return (self == NULL);
}
//Hasta acá

unsigned int scommand_length(const scommand self){ //si falla es por no  poner guint
    assert(self!=NULL);
    unsigned int len = g_queue_get_length(self);
    return len;
unsigned int scommand_length(const scommand self){ //si falla es por no  poner guint
    assert(self!=NULL);
    unsigned int len = g_queue_get_length(self);
    return len;
}

char * scommand_front(const scommand self){
    assert(self!=NULL && !scommand_is_empty(self));
    char * fst_elem = g_queue_peek_head(self);
    assert(fst_elem!=NULL);
    return fst_elem;
    assert(self!=NULL && !scommand_is_empty(self));
    char * fst_elem = g_queue_peek_head(self);
    assert(fst_elem!=NULL);
    return fst_elem;
}
//
//
char * scommand_get_redir_in(const scommand self){
    assert(self!=NULL);
    return self.in;
}

char * scommand_get_redir_out(const scommand self){
    assert(self!=NULL);
    return self.out;
}

char * scommand_to_string(const scommand self){
	GQueue tmp = g_queue_copy(self->comman);
	char * result = NULL;
	gpointer aux = NULL;
	char * killme = NULL;
	char * killme2 = NULL;
	char space = ' ';
	char *sp = &space;
	aux = g_queue_pop_head(tmp);
	result = (* char)g_string_new_take((gchar*) aux);
	while (!g_queue_is_empty(tmp)) {
		aux = g_queue_pop_head(tmp);
		killme2 = result;
		killme = str_merge(sp, (char *)aux);
		result = str_merge(result,killme);
		killme = (* char)g_string_free((* gchar)killme,true);
		killme2 = (* char)g_string_free((* gchar)killme2,true);
	}
	return result;
}

struct pipeline_s{
    GQueue *commands;
    bool wait;
}

pipeline pipeline_new(void){
    pipeline result = malloc(sizeof(struct pipeline_s));
    result->commands = g_queue_new();
    result->true;
    assert(result != NULL && pipeline_is_empty(result) && pipeline_get_wait(result));
    return result;
}

pipeline pipeline_destroy(pipeline self){
    assert(self != NULL);
    //GQueue *commands
    assert(result !=NULL);
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
