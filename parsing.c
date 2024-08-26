#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "parsing.h"
#include "parser.h"
#include "command.h"

//ls, exit, wc, help y cd
//const char * comands[5]={"ls", "exit", "wc", "help", "cd"};

static scommand parse_scommand(Parser p) {
    scommand result = scommand_new();
    char *pointer = NULL;
    arg_kind_t type;
    parser_skip_blanks(p);
    pointer = parser_next_argument(p, &type);
    while (pointer != NULL) {
		if (type == ARG_NORMAL){
        scommand_push_back(result, pointer);
    }
        else if (type == ARG_INPUT){
            scommand_set_redir_in(result, pointer);
    }
        else if (type == ARG_OUTPUT){
            scommand_set_redir_out(result, pointer);
    }
        else {
            printf("Orden no encontrada.\n");
            result = scommand_destroy(result);
        }
        free(pointer);
        pointer = NULL;
    	parser_skip_blanks(p);
    	pointer = parser_next_argument(p, &type);
    }
    if (scommand_is_empty(result)) {
		result = scommand_destroy(result);
	}
    return result;
}

pipeline parse_pipeline(Parser p) {
    assert(p!=NULL && !parser_at_eof(p));
    pipeline result = pipeline_new();
    scommand cmd = NULL;
    bool error = false, another_pipe=true;
	bool res1 = false;
	bool res2 = false;
    bool garbage = false;
	char * garb= NULL;
	while (another_pipe && !error) {
		cmd = parse_scommand(p);
    	error = (cmd==NULL);
    	if (!error) {
			pipeline_push_back(result,cmd);
		} 
    	parser_op_pipe(p, &another_pipe);
    	parser_op_background(p, &res1);
    	parser_op_background(p, &res2);
		another_pipe = another_pipe || (res1 && res2);
    }
	pipeline_set_wait(result,res1 && !res2);
	parser_garbage(p,&garbage);
	garb = parser_last_garbage(p);
	if (garbage) {
		if (sizeof(garb)!=sizeof(char)) {
			printf("Error, los caracteres <%s> estan de mas.\n", garb);
		} else {
			printf("Error, el caracter <%s> esta de mas.\n", garb);
		}
		result = pipeline_destroy(result);
	}
	p = parser_destroy(p);
    return result;
}
