%{
#include <stdio.h>
#include <stdlib.h>
#include "imageprocessing.h"
#include <FreeImage.h>
#include <string.h>
#include "util.h"

void yyerror(char *c);
int yylex(void);
struct user_parameters params;
struct user_parameters_operacao parametros;

%}
%union {
  char    strval[50];
  int     ival;
  float   fval;
}
%token <strval> STRING MAX_PIXEL
%token <ival> VAR IGUAL EOL ASPA 
%token <fval> MULT Operacao
%left SOMA

%%

PROGRAMA:
	PROGRAMA OPERA_PIXEL EOL
	| PROGRAMA EXPRESSAO EOL
	| PROGRAMA PIXEL_MAXIMO EOL
	|
        ;


EXPRESSAO:
    | STRING IGUAL STRING {

        parametros.operacao = COPIA_IMAGEM; // notifica a operacao.
        parametros.nome_nova_imagem = $1;
        parametros.nome_imagem_base = $3;
        parametros.brilho = 1.0;

        run();
                          }

    ;

OPERA_PIXEL:

    | STRING IGUAL STRING Operacao {

        parametros.operacao = ALTERA_BRILHO; // notifica a operacao.
        parametros.nome_nova_imagem = $1;
        parametros.nome_imagem_base = $3;
        parametros.brilho = $4;

        run();

				     }
    ;


PIXEL_MAXIMO:
    | MAX_PIXEL{
	
	printf("procurando pixel maximo...\n");
	printf("nome: %s\n", $1);
	float posicao[3];
	for (int i = 0; i<3 ; i++)	
		posicao[i] = 0.0;


	int *indice = pixel_max ($1, posicao);
	
	printf("A posicao onde o conjunto dos pixels sao maximos eh: %d\nLinha: %d\nColuna: %d\npixel RED: %.2f\npixel GREEN: %.2f\npixel BLUE: %.2f\n", indice[0],indice[1],indice[2], posicao[0],posicao[1],posicao[2]);

	
	
}
;


%%

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}

void parse(int argc, char* argv[], struct user_parameters* params)
{
    int i;
    for(i=1; i<argc; i++) {
        if(!strcmp(argv[i], "-t")){
            if (++i < argc)
            params->type_image = atoi(argv[i]) - 1;
            else {
                fprintf(stderr, "-t requires a number\n");
                exit(EXIT_FAILURE);
            }
        }
        else if(!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h")) {
            printf("----------------------------------------------\n");
            printf("-                Trabalho 2                  -\n");
            printf("-      Guia para parametros de execucao      -\n");
            printf("----------------------------------------------\n");
            printf("-h, --help : Show help information\n");
            printf("-t : type of the image: 1 - small image, 2 - medium image, 3 - large image\n");
            printf("-i : Number of iterations\n");
            printf("-n : Number of threads\n");
            printf("-a : Choose algorithm (leaving blank will run type sequential)\n(Options for type) 0 - sequential, 1 - with threads\n");
            exit(EXIT_SUCCESS);
        } else if(!strcmp(argv[i], "-i")) {
            if (++i < argc)
                params->num_iteration = atoi(argv[i]);
            else {
                fprintf(stderr, "-i requires a number\n");
                exit(EXIT_FAILURE);
            }
        } 
         else if(!strcmp(argv[i], "-n")) {
            if (++i < argc)
                params->num_threads = atoi(argv[i]);
            else {
                fprintf(stderr, "-n requires a number\n");
                exit(EXIT_FAILURE);
            }
        } else if(!strcmp(argv[i], "-a")) {
            if (++i < argc)
                if(atoi(argv[i]))
                    params->type = THREADS;
                else params->type = SEQUENCIAL;
            else {
                fprintf(stderr, "-a requires a number\n");
                exit(EXIT_FAILURE);
            }
        } else
            fprintf(stderr, "Unknown parameter : %s\n", argv[i]);
    }
}

int main(int argc, char* argv[]) {

	memset(&params, 0, sizeof(params));
	params.type = SEQUENCIAL; 		// default seq
	params.num_iteration = 1;
    parse(argc, argv, &params);

  FreeImage_Initialise(0);
  yyparse();

  return 0;

}
