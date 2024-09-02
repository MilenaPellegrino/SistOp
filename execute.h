/* Ejecuta comandos simples y pipelines.
 * No toca ningún comando interno.
 */

#ifndef EXECUTE_H
#define EXECUTE_H

#include "command.h"


char **command_to_array (scommand command);
/*
* guarda los argumentos de command en un arreglo de carecteres donde el ultimo elemento es un pointer a null
*/

bool is_command (const scommand cmd);
/*
*determina si es
*/

int command_run (const scommand cmd, int input, pipeline apipe);

void execute_pipeline(pipeline apipe);
/*
 * Ejecuta un pipeline, identificando comandos internos, forkeando, y
 *   redirigiendo la entrada y salida. puede modificar `apipe' en el proceso
 *   de ejecución.
 *   apipe: pipeline a ejecutar
 * Requires: apipe!=NULL
 */

#endif /* EXECUTE_H */
