
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#include "imageprocessing.h"
#include "util.h"

#include <FreeImage.h>

pthread_mutex_t trava, trava1;
int contador = 0;
int fim = 0;

imagem abrir_imagem(char *nome_do_arquivo, valores_base *valores) {
  FIBITMAP *bitmapIn;
  int x, y;
  int i, j, idx;
  RGBQUAD color;
  imagem I;

  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;

  bitmapIn = FreeImage_Load(FIF_JPEG, nome_do_arquivo, 0);

  if (bitmapIn == 0) {
    exit(1);
  } else {
   }

  x = FreeImage_GetWidth(bitmapIn);
  y = FreeImage_GetHeight(bitmapIn);

  I.width = x;
  I.height = y;
  valores->x = x;
  valores->y = y;

  I.r = (float*) mmap(NULL, sizeof(float) * valores->x * valores->y, protection, visibility, 0, 0);
  I.g = (float*) mmap(NULL, sizeof(float) * valores->x * valores->y, protection, visibility, 0, 0);
  I.b = (float*) mmap(NULL, sizeof(float) * valores->x * valores->y, protection, visibility, 0, 0);

   for (i=0; i<x; i++) {
     for (j=0; j <y; j++) {

      FreeImage_GetPixelColor(bitmapIn, i, j, &color);

      idx = i + (j*x);

      I.r[idx] = color.rgbRed;
      I.g[idx] = color.rgbGreen;
      I.b[idx] = color.rgbBlue;

    }
   }
  return I;
}

void brilho_varredura_serial(valores_base *valores){	 

	int i, j, idx, idy;

	if(valores->modo == 0){		        	
	  	for (i=0; i<valores->x; i++) {
	      	for (j=0; j<valores->y; j++) {

	        	idx = i + (j*valores->x);

				valores->I->r[idx] *= valores->brilho;
				if(valores->I->r[idx] > 255){
					valores->I->r[idx] = 255;
				}
				valores->I->g[idx] *= valores->brilho;
				if(valores->I->g[idx] > 255){
					valores->I->g[idx] = 255;
				}
				valores->I->b[idx] *= valores->brilho;
				if(valores->I->b[idx] > 255){
					valores->I->b[idx] = 255;
				}
			}
	  	}
	}else{
		for (i=0; i<valores->y; i++) {
	      	for (j=0; j<valores->x; j++) {
	
	        	idy = i + (j*valores->y);

	        	valores->I->r[idy] *= valores->brilho;
				if(valores->I->r[idy] > 255){
					valores->I->r[idy] = 255;
				}
				valores->I->g[idy] *= valores->brilho;
				if(valores->I->g[idy] > 255){
					valores->I->g[idy] = 255;
				}
				valores->I->b[idy] *= valores->brilho;
				if(valores->I->b[idy] > 255){
					valores->I->b[idy] = 255;
				}	

			}
		}
	}
}

void salvar_imagem(char *nome_do_arquivo, valores_base *valores) {
  FIBITMAP *bitmapOut;
  RGBQUAD color;
  int i, j, idx;

  bitmapOut = FreeImage_Allocate(valores->x, valores->y, 24, 0, 0, 0);

   for (i=0; i<valores->x; i++) {
     for (j=0; j<valores->y; j++) {

      idx = i + (j*valores->x);

      color.rgbRed = valores->I->r[idx];
      color.rgbGreen = valores->I->g[idx];
      color.rgbBlue = valores->I->b[idx];

      FreeImage_SetPixelColor(bitmapOut, i, j, &color);
    }
  }
  FreeImage_Save(FIF_JPEG, bitmapOut, nome_do_arquivo, JPEG_DEFAULT);

   	munmap(valores->I->r, sizeof(float) * valores->x * valores->y);
	munmap(valores->I->g, sizeof(float) * valores->x * valores->y);
	munmap(valores->I->b, sizeof(float) * valores->x * valores->y);
}

int * pixel_max (char *nome_do_arquivo, float *posicao){
FIBITMAP *bitmapIn;
  int x, y, i, j, k, idx;
  int * indice_max = (int *) calloc (3, sizeof (int));
  float aux1[3];
  RGBQUAD color;
  imagem I;

	for (i = 0; i<3; i++){
		posicao[i] = 0.0;	
		aux1[i] = 0.0;
	}

  bitmapIn = FreeImage_Load(FIF_JPEG, nome_do_arquivo, 0);

  if (bitmapIn == 0) {
  } else {
   }

  x = FreeImage_GetWidth(bitmapIn);
  y = FreeImage_GetHeight(bitmapIn);

  I.width = x;
  I.height = y;
  I.r = malloc(sizeof(float) * x * y);
  I.g = malloc(sizeof(float) * x * y);
  I.b = malloc(sizeof(float) * x * y);

   for (i=0; i<x; i++) {
     for (j=0; j <y; j++) {

      float soma_aux = 0.0, soma = 0.0;
      FreeImage_GetPixelColor(bitmapIn, i, j, &color);

      idx = i + (j*x);

      I.r[idx] = color.rgbRed;
      I.g[idx] = color.rgbGreen;
      I.b[idx] = color.rgbBlue;

	aux1[0] = I.r[idx];
	aux1[1] = I.g[idx];
	aux1[2] = I.b[idx];
	
	for (k = 0; k<3 ; k++){
		soma_aux += aux1[k]; 	
		soma +=	posicao[k];
	}
	if (soma_aux > soma){
		indice_max[0] = idx;
		indice_max[1] = i;
		indice_max[2] = j;
		for (k = 0; k<3 ; k++)
			posicao[k] = aux1[k];
	} 
     }	

 
    }

return indice_max;
}

void * brilho_threads_varredura(valores_base *valores){

	int ID, x, y;
	float brilho;
	pthread_mutex_lock(&trava1);
	ID = contador;
	contador++;
	brilho = valores->brilho;
	x = valores->x;
	y = valores->y;
	pthread_mutex_unlock(&trava1);    	
	int n1, n2;
	int i, j, idx, idy;

  	n1 = valores->range[2*ID];
  	n2 = valores->range[(2*ID)+1];

  	if(valores->modo == 0){
	  	for (n1; n1<=n2; n1++) {
	    	for (i=0; i<x; i++) {

		      	idx = i + (n1*x);

		      	valores->I->r[idx] *= brilho;
				if(valores->I->r[idx] > 255.0){
					valores->I->r[idx] = 255.0;
				}
				valores->I->g[idx] *= brilho;
				if(valores->I->g[idx] > 255.0){
					valores->I->g[idx] = 255.0;
				}
				valores->I->b[idx] *= brilho;
				if(valores->I->b[idx] > 255.0){
					valores->I->b[idx] = 255.0;
				}
		    }
	  	}

	  	if(valores->resto != 0){
	  		pthread_mutex_lock(&trava);
	  			if(fim == 1){
	  				pthread_mutex_unlock(&trava);
	  				return NULL;
	  			}else{
	  				fim = 1;
	  			}
	  		pthread_mutex_unlock(&trava);  

	    	n1 = y - valores->resto;

	    	for (n1; n1<y; n1++) {
	      		for (i=0; i<x; i++) {

			        idx = i + (n1*x);

			        valores->I->r[idx] *= brilho;
					if(valores->I->r[idx] > 255){
						valores->I->r[idx] = 255;
					}
					valores->I->g[idx] *= brilho;
					if(valores->I->g[idx] > 255){
						valores->I->g[idx] = 255;
					}
					valores->I->b[idx] *= brilho;
					if(valores->I->b[idx] > 255){
						valores->I->b[idx] = 255;
					}

		     	}
		    }
	  	}else{
	  		return NULL;
	  	}
	}else{
		for (n1; n1<=n2; n1++) {
	    	for ( i=0; i<y; i++) {
	      		idy;

		      	idy = i + (n1*y);

		      	valores->I->r[idy] *= brilho;
				if(valores->I->r[idy] > 255){
					valores->I->r[idy] = 255;
				}
				valores->I->g[idy] *= brilho;
				if(valores->I->g[idy] > 255){
					valores->I->g[idy] = 255;
				}
				valores->I->b[idy] *= brilho;
				if(valores->I->b[idy] > 255){
					valores->I->b[idy] = 255;
				}

		    }
	  	}

	  	if(valores->resto != 0){
	  		pthread_mutex_lock(&trava);
  			if(fim == 1){
  				pthread_mutex_unlock(&trava);
  				return NULL;
  			}else{
  				fim = 1;
  			}
	  		pthread_mutex_unlock(&trava); 	

	    	n1 = x - valores->resto;

	    	for (n1; n1<x; n1++) {
	      		for (i = 0; i<y; i++) {

			        idy = i + (n1*y);

			        valores->I->r[idy] *= brilho;
					if(valores->I->r[idy] > 255){
						valores->I->r[idy] = 255;
					}
					valores->I->g[idy] *= brilho;
					if(valores->I->g[idy] > 255){
						valores->I->g[idy] = 255;
					}
					valores->I->b[idy] *= brilho;
					if(valores->I->b[idy] > 255){
						valores->I->b[idy] = 255;
					}

		     	}
		    }
	    	pthread_mutex_unlock(&trava);  
	  	}else{
	  		return NULL;
	  	}
	}

  	return NULL;
}  

/*APLICANDO BRILHO POR MEIO DE THREADS VARRENDO AS LINHAS DA MATRIZ*/
void brilho_threads(valores_base *valores){

  	pthread_t vetor[valores->num_joaquim];	
	valores_base aux;
  	int id[valores->num_joaquim];
  	int jobs_por_thread;
  	int i = 0, j = 0;

  	valores->range = (int*)calloc(2*valores->num_joaquim, sizeof(int));

  	
  	if(valores->modo == 0){ /*Modo de varredura por linhas da matriz*/
	 	jobs_por_thread = valores->y/valores->num_joaquim;
	  	valores->resto = valores->y%valores->num_joaquim;
  	}else{ /*Modo de varredura por colunas da matriz*/
  		jobs_por_thread = valores->x/valores->num_joaquim;
	  	valores->resto = valores->x%valores->num_joaquim;
  	}


	aux = *valores;

  	/* Disparanto threads */
  	for (i=0; i < valores->num_joaquim; i++) {
  		j++;
    	valores->range[j] = jobs_por_thread*(i+1)-1;
    	j++;
    	pthread_create(&(vetor[i]), NULL, (void *) &brilho_threads_varredura, (void *) (&aux));
    	valores->range[j] = valores->range[j-1]+1;	
    }

  	/* Esperando threads */
  	for (i=0; i < valores->num_joaquim; i++) {
    	pthread_join(vetor[i], NULL);
  	}

  	free(valores->range);
}

void * brilho_processos_varredura(valores_base *valores, int ID){

  	int n1, n2, x, y;
  	int i, j, idx, idy;
  	float brilho;

  	brilho = valores->brilho;
  	x = valores->x;
  	y = valores->y;

  	n1 = valores->range[2*ID];

  	if(valores->modo == 0){
  		if(ID == valores->num_joaquim-1 && valores->resto == 0){
  			n2 = x;
		}else{
	  		n2 = valores->range[(2*ID)+1];
		}

	  	for (n1; n1<=n2; n1++) {
	    	for (i=0; i<x; i++) {

		      	idx = i + (n1*x);

		      	valores->I->r[idx] *= brilho;
				if(valores->I->r[idx] > 255.0){
					valores->I->r[idx] = 255.0;
				}
				valores->I->g[idx] *= brilho;
				if(valores->I->g[idx] > 255.0){
					valores->I->g[idx] = 255.0;
				}
				valores->I->b[idx] *= brilho;
				if(valores->I->b[idx] > 255.0){
					valores->I->b[idx] = 255.0;
				}
		    }
	  	}

	}else{
		if(ID == valores->num_joaquim-1 && valores->resto == 0){
	  		n2 = y;
		}else{
	  		n2 = valores->range[(2*ID)+1];
		}
		for (n1; n1<=n2; n1++) {
	    	for (i=0; i<y; i++) {

		      	idy = i + (n1*y);

		      	valores->I->r[idy] *= brilho;
				if(valores->I->r[idy] > 255){
					valores->I->r[idy] = 255;
				}
				valores->I->g[idy] *= brilho;
				if(valores->I->g[idy] > 255){
					valores->I->g[idy] = 255;
				}
				valores->I->b[idy] *= brilho;
				if(valores->I->b[idy] > 255){
					valores->I->b[idy] = 255;
				}

		    }
	  	}
	}
  	return NULL;
}  

void brilho_processos(valores_base *valores){
	
	pid_t ID;
 	valores_base aux;
	int id[valores->num_joaquim];
  	int jobs_por_processos, status = 0;
  	int i = 0, j = 0, idx;

  	valores->range = (int*)calloc(2*valores->num_joaquim, sizeof(int));

  	if(valores->modo == 0){ /*Modo de varredura por linhas da matriz*/
	 	jobs_por_processos = valores->y/valores->num_joaquim;
	  	valores->resto = valores->y%valores->num_joaquim;
  	}else{ /*Modo de varredura por colunas da matriz*/
  		jobs_por_processos = valores->x/valores->num_joaquim;
	  	valores->resto = valores->x%valores->num_joaquim;
  	}

  	aux = *valores;
  	j = 1;

  	/* Disparando processos */
  	for (i=0; i < valores->num_joaquim; i++) {
    	valores->range[j] = jobs_por_processos*(i+1)-1;
    	j++;
	    ID = fork();
	    id[i] = ID;
    	if(ID == 0){
    		ID = i;
    		brilho_processos_varredura((void *) &aux, ID);
    		exit(EXIT_SUCCESS);
    	}
    	valores->range[j] = valores->range[j-1]+1;
    	j++;
	}
	
	for (i = 0; i < valores->num_joaquim; i++) {
	    while (waitpid(id[i], NULL, 0) > 0);
	}

	free(valores->range);
}