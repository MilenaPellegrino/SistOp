#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "parsing.h"
#include "parser.h"
#include "command.h"

//ls, exit, wc, help y cd
//const char * comands[5]={"ls", "exit", "wc", "help", "cd"};


//CREO QUE EL ERROR ESTÁ EN QUE TENEMOS QUE PARSEAR CON COMANDOS REALES
static scommand parse_scommand(Parser p) {
    scommand result = scommand_new();
    char *arg = NULL;
    arg_kind_t type;
    parser_skip_blanks(p);
    arg = parser_next_argument(p, &type);
    while (arg != NULL) {
		if (type == ARG_NORMAL){
        scommand_push_back(result, arg);
    }
        else if (type == ARG_INPUT){
            scommand_set_redir_in(result, arg);
    }
        else if (type == ARG_OUTPUT){
            scommand_set_redir_out(result, arg);
    }
        else {
            printf("Orden no encontrada.(error de parseo)\n");
            result = scommand_destroy(result);
        }
        free(arg);
        arg = NULL;
    	parser_skip_blanks(p);
    	arg = parser_next_argument(p, &type);
    }
    if (scommand_is_empty(result)) {
		result = scommand_destroy(result);
	}
    return result;
}

pipeline parse_pipeline(Parser p) {
    assert(p!=NULL && !parser_at_eof(p));
    pipeline result = pipeline_new();
    scommand cmd = scommand_new();
    bool error = false, another_pipe=true;
	bool background = false;
    bool garbage = false;
	char * garb= NULL;
	while (another_pipe && !error) {
		cmd = parse_scommand(p);
    	error = (cmd==NULL);
    	if (!error) {
			pipeline_push_back(result,cmd);
		}
    	parser_op_pipe(p, &another_pipe);
    	parser_op_background(p, &background);
    }
	pipeline_set_wait(result, background);
	parser_garbage(p,&garbage);
	garb = parser_last_garbage(p);
	if (garbage) {
		result = pipeline_destroy(result);
        printf("%s", garb);
	}
	p = parser_destroy(p);
    return result;
}
