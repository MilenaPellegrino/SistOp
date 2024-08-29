#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "parsing.h"
#include "parser.h"
#include "command.h"

static scommand parse_scommand(Parser p) {
    scommand result = scommand_new();
    char *arg = NULL;
    arg_kind_t type;
    parser_skip_blanks(p);
    arg = parser_next_argument(p, &type);
    while (arg != NULL && result != NULL) {
		if (type == ARG_NORMAL){
        scommand_push_back(result, arg);
    }
        else if (type == ARG_INPUT){
            scommand_set_redir_in(result, arg);
    }
        else if (type == ARG_OUTPUT){
            scommand_set_redir_out(result, arg);
    }
    	parser_skip_blanks(p);
        arg = NULL;
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
    bool error = false, another_pipe=true, at_eof = false;
	bool wait = false, garbage = false; 
	char * garb= NULL;
		cmd = parse_scommand(p);
    	error = (cmd==NULL);
	while (another_pipe && !error && !at_eof) {
		pipeline_push_back(result, cmd);
    	parser_op_pipe(p, &another_pipe);
        at_eof = parser_at_eof(p);
    	cmd = parse_scommand(p);
    	error = (cmd==NULL);
    }
    parser_op_background(p, &wait);
	pipeline_set_wait(result, !wait);
	parser_garbage(p,&garbage);
	garb = parser_last_garbage(p);
	if (garbage || pipeline_is_empty(result)) {
		result = pipeline_destroy(result);
        result = NULL;
        printf("Hay basura: %s\n", garb);
	}
    return result;
}
