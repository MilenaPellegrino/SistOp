#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "parsing.h"
#include "parser.h"
#include "command.h"

//ls, exit, wc, help y cd
//const char * comands[5]={"ls", "exit", "wc", "help", "cd"};

static scommand parse_scommand(Parser p) {
    /* Devuelve NULL cuando hay un error de parseo */
    scommand result = scommand_new();
    char *pointer = NULL;
    arg_kind_t type;
    bool res1 = false;
    bool res2 = false;
    while (!res1 && !res2) {
        parser_skip_blanks(p);
        pointer = parser_next_argument(p, &type);
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
            printf("orden no encontrada");
            result = scommand_destroy(result);
        }
        parser_op_pipe(p, &res1);
        parser_op_background(p, &res2);

        free(pointer);
        pointer = NULL;
    }
    
    return result;
}

pipeline parse_pipeline(Parser p) {
    assert(p!=NULL && !parser_at_eof(p));

    pipeline result = pipeline_new();
    scommand cmd = NULL;
    bool error = false, another_pipe=true;

    cmd = parse_scommand(p);
    error = (cmd==NULL); /* Comando inválido al empezar */
    while (another_pipe && !error) {
        /*
         * COMPLETAR
         *
         */
    }
    /* Opcionalmente un OP_BACKGROUND al final */
    /*
     *
     * COMPLETAR
     *
     */

    /* Tolerancia a espacios posteriores */
    /* Consumir todo lo que hay inclusive el \n */
    /* Si hubo error, hacemos cleanup */

    return NULL; // MODIFICAR
}

