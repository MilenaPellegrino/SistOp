#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <glib.h>
#include "strextra.h"
#include "command.h"

struct scommand_s {
    GQueue* scomman;
    char* out;
    char* in;
};


scommand scommand_new(void){
    scommand result = malloc(sizeof(struct scommand_s));
    assert(result!= NULL);
    result->scomman = g_queue_new();
    result->out = NULL;
    result->in = NULL;
    assert(scommand_is_empty(result) 
		   &&  scommand_get_redir_in(result)==NULL 
		   &&  scommand_get_redir_out (result)==NULL);
    return result;
}


scommand scommand_destroy(scommand self){
    assert(self!= NULL);
    g_queue_free_full(self->scomman, g_free);
    free(self->out);
    free(self->in);
    free(self);
    self = NULL;
    return self;
}

void scommand_push_back(scommand self, char * argument){
    assert(self!=NULL && argument!=NULL); 
    g_queue_push_tail(self->scomman, argument);
    assert(!scommand_is_empty(self));
}

void scommand_pop_front(scommand self){
    assert(self!=NULL && !scommand_is_empty(self));
    gpointer kill_data =  g_queue_pop_head(self->scomman);
    g_free(kill_data);
}

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

bool scommand_is_empty(const scommand self){
    assert(self != NULL);
    return (scommand_length(self) == 0);
}

unsigned int scommand_length(const scommand self){ //si falla es por no  poner guint
    assert(self!=NULL);
    unsigned int len = g_queue_get_length(self->scomman);
    return len;
}

char * scommand_front(const scommand self){
    assert(self!=NULL && !scommand_is_empty(self));
    char * fst_elem = g_queue_peek_head(self->scomman);
    assert(fst_elem!=NULL);
    return fst_elem;
}

char * scommand_get_redir_in(const scommand self){
    assert(self!=NULL);
    return self->in;
}

char * scommand_get_redir_out(const scommand self){
    assert(self!=NULL);
    return self->out;
}

char * scommand_to_string(const scommand self){
	char *result = NULL;
	char space = ' ';
	char *sp = &space;
	char *mayor = " > ";
	char *minor = " < ";
	char *killme = NULL;
	char *killme2 = NULL;

	if (!g_queue_is_empty(self->scomman)) {
		GQueue *tmp = g_queue_copy(self->scomman);
		gpointer aux = NULL;

		aux = g_queue_pop_head(tmp);
		GString *gstr = g_string_new_take((gchar *)aux);
        result = gstr->str;

		while (!g_queue_is_empty(tmp)) {
			aux = g_queue_pop_head(tmp);
			killme2 = result;
			killme = strmerge(sp, (char *)aux);
			result = strmerge(result,killme);
            g_free(killme);
            g_free(killme2);
		}
        g_string_free(gstr, TRUE);
        g_queue_free(tmp);

	} if (self->out!=NULL) {
		if (result!=NULL) {
			killme = strmerge(mayor, self->out);
			killme2 = result;
			result = strmerge(result, killme);
            g_free(killme);
            g_free(killme2);
		} else {
			result = strmerge(mayor, self->out);
		}

	} if (self->in!=NULL) {
		if (result!=NULL) {
			killme = strmerge(minor, self->in);
			killme2 = result;
			result = strmerge(result, killme);
            g_free(killme);
            g_free(killme2);
		} else {
			result = strmerge(minor, self->in);
		}
	}
	return result;
}

struct pipeline_s{
    GQueue *commands;
    bool wait;
};

pipeline pipeline_new(void){
    pipeline result = malloc(sizeof(struct pipeline_s));
    result->commands = g_queue_new();
    result->wait = true;
    assert(result != NULL && pipeline_is_empty(result) && pipeline_get_wait(result));
    return result;
}

pipeline pipeline_destroy(pipeline self){ 
    assert(self != NULL);
    GQueue *coms = self->commands;
    while (!g_queue_is_empty(coms)) {
        scommand_destroy(g_queue_pop_head(coms));
    }
    g_queue_free(coms);
    free(self);
    return NULL;
}

void pipeline_push_back(pipeline self, scommand sc){
    assert(self!=NULL && sc!=NULL);
    g_queue_push_tail(self->commands, sc);
    assert(!pipeline_is_empty(self));
}

void pipeline_pop_front(pipeline self){
    assert(self!=NULL && !pipeline_is_empty(self));
    gpointer kill_data =  g_queue_pop_head(self->commands);
    g_free(kill_data);
}


void pipeline_set_wait(pipeline self, const bool w){
    assert(self != NULL);
    self->wait = w;
}

bool pipeline_is_empty(const pipeline self){
    assert(self != NULL);
    return (pipeline_length(self) == 0);
}

unsigned int pipeline_length(const pipeline self){
    assert(self!=NULL);
    unsigned int leng = g_queue_get_length(self->commands);
    return leng;
}

scommand pipeline_front(const pipeline self){
    assert(self!=NULL && !pipeline_is_empty(self));
    scommand fst_el = g_queue_peek_head(self->commands);
    assert(fst_el!=NULL);
    return fst_el;
}

bool pipeline_get_wait(const pipeline self){
    assert(self!=NULL);
    bool res = self->wait;
    return res;
}

char * pipeline_to_string(const pipeline self){
	char *result = NULL;
	char *killme = NULL;
	char *killme2 = NULL;
	char *amper = " && ";
	char *pipe = " | ";

	if (!g_queue_is_empty(self->commands)) {
		GQueue *tmp = g_queue_copy(self->commands);
		gpointer aux = NULL;

		aux = g_queue_pop_head(tmp);
		result = scommand_to_string((scommand)aux);

		while (!g_queue_is_empty(aux)) {
			aux = g_queue_pop_head(tmp);
			killme = scommand_to_string((scommand)aux);

			if (self->wait==true) {
				killme2 = strmerge(amper, killme);
			} else {
				killme2 = strmerge(pipe, killme);	
			}

            g_free(result);
            result = strmerge(result, killme2);
            g_free(killme);
            g_free(killme2);
		}
        g_queue_free(tmp);
	}
	return result;
}

