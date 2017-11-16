#ifndef ITIL_H
#define UTIL_H

#include "imageprocessing.h"
#include <time.h>
#include <sys/time.h>
typedef void (*Funcao)(void);

void manipula_imagem_threads(void);
void manipula_imagem(void);
void medir_tempo(Funcao func_analisada);
double run(void);

extern struct user_parameters_operacao parametros;
extern struct user_parameters params;

#endif