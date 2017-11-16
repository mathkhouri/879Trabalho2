
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "imageprocessing.h"
#include "util.h"

#include <FreeImage.h>

pthread_mutex_t trava;

struct_thread valores_thread;

/*
imagem abrir_imagem(char *nome_do_arquivo);
void salvar_imagem(char *nome_do_arquivo);
void liberar_imagem(imagem *i);
 */

imagem abrir_imagem(char *nome_do_arquivo) {
  FIBITMAP *bitmapIn;
  int x, y;
  RGBQUAD color;
  imagem I;

  bitmapIn = FreeImage_Load(FIF_JPEG, nome_do_arquivo, 0);

  if (bitmapIn == 0) {
    //// printf("Erro! Nao achei arquivo - %s\n", nome_do_arquivo);
    exit(1);
  } else {
    //  // printf("Arquivo lido corretamente!\n");
   }

  x = FreeImage_GetWidth(bitmapIn);
  y = FreeImage_GetHeight(bitmapIn);

  I.width = x;
  I.height = y;
  I.r = malloc(sizeof(float) * x * y);
  I.g = malloc(sizeof(float) * x * y);
  I.b = malloc(sizeof(float) * x * y);


  /* ACHO INTERESSANTE TRANSFORMAR ISSO EM UMA FUNCAO.. PARA PODER CHAMAR AO CRIAS THREADS */
   for (int i=0; i<x; i++) {
     for (int j=0; j <y; j++) {
      int idx;
      FreeImage_GetPixelColor(bitmapIn, i, j, &color);

      idx = i + (j*x);

      I.r[idx] = color.rgbRed;
      I.g[idx] = color.rgbGreen;
      I.b[idx] = color.rgbBlue;
    }
   }
  return I;
}

void brilho_varredura_serial(imagem *I, float brilho, int x, int y){

  for (int i=0; i<x; i++) {
      for (int j=0; j<y; j++) {
        int idx;

        idx = i + (j*x);

        I->r[idx] = I->r[idx]*brilho < 255 ? I->r[idx]*brilho : 255;
        I->g[idx] = I->g[idx]*brilho < 255 ? I->g[idx]*brilho : 255;
        I->b[idx] = I->b[idx]*brilho < 255 ? I->b[idx]*brilho : 255;
      }
  }
}

void salvar_imagem(char *nome_do_arquivo, imagem *I) {
  FIBITMAP *bitmapOut;
  RGBQUAD color;

  //// printf("Salvando imagem %d por %d...\n", I->width, I->height);
  bitmapOut = FreeImage_Allocate(I->width, I->height, 24, 0, 0, 0);

   for (int i=0; i<I->width; i++) {
     for (int j=0; j<I->height; j++) {
      int idx;

      idx = i + (j*I->width);
      color.rgbRed = I->r[idx];
      color.rgbGreen = I->g[idx];
      color.rgbBlue = I->b[idx];

      FreeImage_SetPixelColor(bitmapOut, i, j, &color);
    }
  }

  FreeImage_Save(FIF_JPEG, bitmapOut, nome_do_arquivo, JPEG_DEFAULT);
}

int * pixel_max (char *nome_do_arquivo, float *posicao){
FIBITMAP *bitmapIn;
  int x, y;
  int * indice_max = (int *) calloc (3, sizeof (int));
  float aux1[3];
  RGBQUAD color;
  imagem I;
	
 //

	for (int i = 0; i<3; i++){
		posicao[i] = 0.0;	
		aux1[i] = 0.0;
	}

  bitmapIn = FreeImage_Load(FIF_JPEG, nome_do_arquivo, 0);

  if (bitmapIn == 0) {
    // printf("Erro! Nao achei arquivo - %s\n", nome_do_arquivo);
  } else {
   // // printf("Arquivo lido corretamente!\n");
   }

  x = FreeImage_GetWidth(bitmapIn);
  y = FreeImage_GetHeight(bitmapIn);

  I.width = x;
  I.height = y;
  I.r = malloc(sizeof(float) * x * y);
  I.g = malloc(sizeof(float) * x * y);
  I.b = malloc(sizeof(float) * x * y);

   for (int i=0; i<x; i++) {
     for (int j=0; j <y; j++) {
      int idx;
      float soma_aux = 0.0, soma = 0.0;
      FreeImage_GetPixelColor(bitmapIn, i, j, &color);

      idx = i + (j*x);

      I.r[idx] = color.rgbRed;
      I.g[idx] = color.rgbGreen;
      I.b[idx] = color.rgbBlue;

	aux1[0] = I.r[idx];
	aux1[1] = I.g[idx];
	aux1[2] = I.b[idx];
	
	//// printf("Imagem: %.2f\n", I.r[idx]);
	for (int k = 0; k<3 ; k++){
		soma_aux += aux1[k]; 	
		soma +=	posicao[k];
		//// printf("Aux1[k] =  %.2f\nsoma: %.2f\n",aux1[k], soma_aux);
	}
	if (soma_aux > soma){
		indice_max[0] = idx;
		indice_max[1] = i;
		indice_max[2] = j;
		for (int k = 0; k<3 ; k++)
			posicao[k] = aux1[k];
	} 
     }	

 
    }

return indice_max;
}

	
/*
imagem abrir_imagem_threads(char *nome_do_arquivo);
void salvar_imagem_threads(char *nome_do_arquivo);
void liberar_imagem_threads(imagem *i);
 */


void * brilho_varredura_por_threads(void *arg, imagem *I, float brilho, int n1, int n2, int x, int y, int resto) {
  for (int i=0; i<x; i++) {
    for (n1; n1<=n2; n1++) {
      int idx;

      idx = i + (n1*x);

      I->r[idx] = I->r[idx]*brilho < 255 ? I->r[idx]*brilho : 255;
      I->g[idx] = I->g[idx]*brilho < 255 ? I->g[idx]*brilho : 255;
      I->b[idx] = I->b[idx]*brilho < 255 ? I->b[idx]*brilho : 255;
    }
  }

  if(resto != 0 && !pthread_mutex_trylock(&trava)){
    n1 = y-resto;

    for (int i=0; i<x; i++) {
      for (n1; n1<y; n1++) {
        int idx;

        idx = i + (n1*x);

        I->r[idx] = I->r[idx]*brilho < 255 ? I->r[idx]*brilho : 255;
        I->g[idx] = I->g[idx]*brilho < 255 ? I->g[idx]*brilho : 255;
        I->b[idx] = I->b[idx]*brilho < 255 ? I->b[idx]*brilho : 255;
      }
    }
    pthread_mutex_unlock(&trava);  
  }

  return NULL;
}  

/*APLICANDO BRILHO POR MEIO DE THREADS VARRENDO AS LINHAS DA MATRIZ*/
void brilho_threads(imagem *I, float brilho, int num_threads, int x, int y){

  pthread_t brilho_varredura_por_threads[num_threads];
  int thread_id[num_threads];
  int linhas_por_thread, resto = 0;
  int i = 0, n1 = 0, n2 = 0;

  linhas_por_thread = y/num_threads;
  resto = y%num_threads;


  /* Identificadores de thread */
  for (i=0; i<num_threads; i++) {
    thread_id[i] = i;
  }

  /* Disparanto threads */
  for (i=0; i<num_threads; i++) {
    n2 = linhas_por_thread*(i+1)-1;
    pthread_create(&(brilho_varredura_por_threads[i]), NULL, (void * (*)(void *)) brilho_varredura_por_threads, (void *) (&thread_id[i]), &I, brilho, n1, n2, x, y, resto);
    n1 = n2+1;
  }

  /* Esperando threads */
  for (i=0; i<num_threads; i++) {
    pthread_join(brilho_varredura_por_threads[i], NULL);
  }
}