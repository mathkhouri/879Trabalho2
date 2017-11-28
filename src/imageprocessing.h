
#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

typedef struct {
  unsigned int width, height;
  float *r, *g, *b;
} imagem;

typedef struct {
  imagem *I;
  float brilho;
  int x;
  int y;
  int resto;
  int num_joaquim;
  int ID;	
  int *range;
  int modo; /*Modo= 0 -> varredura por linhas, Modo = 1 -> varredura por colunas*/
} valores_base;

typedef enum {
	COPIA_IMAGEM,
	ALTERA_BRILHO,
	PIXEL_MAXIMO
} _casos;

typedef enum{
	THREADS,
	SEQUENCIAL,
	PROCESSO
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
	int type_image;	
	int num_iteration;	// number of iteration
	int modo_varredura;
};

imagem abrir_imagem(char *nome_do_arquivo, valores_base *valores);
void brilho_varredura_serial(valores_base *valores);
void salvar_imagem(char *nome_do_arquivo, valores_base *valores);
int * pixel_max (char *nome_do_arquivo, float *posicao);

void * brilho_threads_varredura(valores_base *valores);
void brilho_threads(valores_base *valores);

void * brilho_processos_varredura(valores_base *valores, int ID);
void brilho_processos(valores_base *valores);

extern struct user_parameters_operacao parametros;
extern struct user_parameters params;

#endif
