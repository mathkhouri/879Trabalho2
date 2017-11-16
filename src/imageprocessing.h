
#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

typedef struct {
  unsigned int width, height;
  float *r, *g, *b;
} imagem;

typedef enum {
	COPIA_IMAGEM,
	ALTERA_BRILHO,
	PIXEL_MAXIMO
} _casos;

typedef enum{
	THREADS,
	SEQUENCIAL
} _type;

struct user_parameters_operacao {
	_casos operacao;
	char *nome_nova_imagem;
	char *nome_imagem_base;
	float brilho;
};


struct user_parameters {
	_type type;		// type of execution
	int num_threads;	// number of threads
	int type_image;	// check length.
	int num_iteration;	// number of iteration
};

imagem abrir_imagem(char *nome_do_arquivo);
void brilho(imagem *I, float brilho, int x, int y);
void salvar_imagem(char *nome_do_arquivo, imagem *I);
void liberar_imagem(imagem *i);
int * pixel_max (char *nome_do_arquivo, float *posicao);

void * brilho_varredura_por_threads(void *arg, imagem *I, float valor, int n1, int n2, int x, int y, int resto);
void brilho_threads(imagem *I, float valor, int num_threads, int x, int y);

extern struct user_parameters_operacao parametros;
extern struct user_parameters params;

#endif
