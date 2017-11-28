#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "imageprocessing.h"

clock_t ct0, ct1, dct; 	// medida de tempo baseada no clock da CPU
struct timeval rt0,rt1,drt; 	// tempo baseada em tempo real
char tipo_imagem[4][6]= {"Tipo1","Tipo2","Tipo3","Tipo4"};
valores_base *valores;

void manipula_imagem(void){
	double mean_clock = 0.0, mean_real_sec = 0.0, mean_real_usec = 0.0;

	
	valores_base aux;
	aux = *valores;
	imagem I = abrir_imagem(parametros.nome_imagem_base, &aux);
	aux.I = &I;

	gettimeofday(&rt0, NULL);
	ct0 = clock();
	brilho_varredura_serial( &aux);
	ct1 = clock();
	gettimeofday(&rt1, NULL);


	timersub(&rt1, &rt0, &drt);
	mean_real_sec += drt.tv_sec;		// media em segundos
	mean_real_usec += drt.tv_usec;		// media em  micro segundos
	mean_clock += (double) (ct1 - ct0)/CLOCKS_PER_SEC;


	if (params.type == SEQUENCIAL){
		printf("Tempo real: %.6lf segundos. %s: Sequencial \n",mean_real_sec+mean_real_usec/1000000, tipo_imagem[params.type_image]);
		printf("Tempo user: %f segundos. %s: Sequencial \n",mean_clock,tipo_imagem[params.type_image]);
	}
	 else
	{
		printf("Tempo real: %.6lf segundos. %s: Num Threads: %d\n", mean_real_sec+mean_real_usec/1000000,tipo_imagem[params.type_image],params.num_threads);
		printf("Tempo user: %f segundos. %s: Num Threads: %d\n",mean_clock,tipo_imagem[params.type_image],params.num_threads);	
	}

	salvar_imagem(parametros.nome_nova_imagem, &aux);
}

void manipula_imagem_threads(void){
	double mean_clock = 0.0, mean_real_sec = 0.0, mean_real_usec = 0.0;


	valores_base aux;
	aux = *valores;
	imagem I = abrir_imagem(parametros.nome_imagem_base, &aux);
	aux.I = &I;

	gettimeofday(&rt0, NULL);
	ct0 = clock();
	brilho_threads(&aux);	  	
	ct1 = clock();
	gettimeofday(&rt1, NULL);

	timersub(&rt1, &rt0, &drt);
	mean_real_sec += drt.tv_sec;		// media em segundos
	mean_real_usec += drt.tv_usec;		// media em  micro segundos
	mean_clock += (double) (ct1 - ct0)/CLOCKS_PER_SEC;

	if (params.type == SEQUENCIAL){
		printf("Tempo real: %.6lf segundos. %s: Sequencial \n",mean_real_sec+mean_real_usec/1000000, tipo_imagem[params.type_image]);
		printf("Tempo user: %f segundos. %s: Sequencial \n",mean_clock,tipo_imagem[params.type_image]);
	}
	 else
	{
		printf("Tempo real: %.6lf segundos. %s: Num Threads: %d\n", mean_real_sec+mean_real_usec/1000000,tipo_imagem[params.type_image],params.num_threads);
		printf("Tempo user: %f segundos. %s: Num Threads: %d\n",mean_clock,tipo_imagem[params.type_image],params.num_threads);	
	}

	salvar_imagem(parametros.nome_nova_imagem, &aux);
}

void manipula_imagem_processo(void){
	double mean_clock = 0.0, mean_real_sec = 0.0, mean_real_usec = 0.0;

	

	valores_base aux;
	aux = *valores;
	imagem I = abrir_imagem(parametros.nome_imagem_base, &aux);
	aux.I = &I;

	gettimeofday(&rt0, NULL);
	ct0 = clock();
	brilho_processos(&aux);
	ct1 = clock();
	gettimeofday(&rt1, NULL);

	timersub(&rt1, &rt0, &drt);
	mean_real_sec += drt.tv_sec;		// media em segundos
	mean_real_usec += drt.tv_usec;		// media em  micro segundos
	mean_clock += (double) (ct1 - ct0)/CLOCKS_PER_SEC;

	if (params.type == SEQUENCIAL){
		printf("Tempo real: %.6lf segundos. %s: Sequencial \n",mean_real_sec+mean_real_usec/1000000, tipo_imagem[params.type_image]);
		printf("Tempo user: %f segundos. %s: Sequencial \n",mean_clock,tipo_imagem[params.type_image]);
	}
	 else
	{
		printf("Tempo real: %.6lf segundos. %s: Num Threads: %d\n", mean_real_sec+mean_real_usec/1000000,tipo_imagem[params.type_image],params.num_threads);
		printf("Tempo user: %f segundos. %s: Num Threads: %d\n",mean_clock,tipo_imagem[params.type_image],params.num_threads);	
	}

	salvar_imagem(parametros.nome_nova_imagem, &aux);  
}

void medir_tempo(Funcao func_analisada){

	gettimeofday(&rt0, NULL);
	ct0 = clock();
	func_analisada();
	ct1 = clock();
	gettimeofday(&rt1, NULL);
}


double run(void){
	valores = malloc(sizeof(valores_base));

	double mean_clock = 0.0, mean_real_sec = 0.0, mean_real_usec = 0.0;
	
		valores->modo = params.modo_varredura;
		valores->brilho = parametros.brilho;
		valores->num_joaquim = params.num_threads;

	for (int i = 0; i < params.num_iteration; i++){
		switch (parametros.operacao){
			case ALTERA_BRILHO:
			case COPIA_IMAGEM:

			if(params.type == THREADS)
					manipula_imagem_threads();
				else if (params.type == SEQUENCIAL)
					manipula_imagem();
				else if (params.type == PROCESSO)
					manipula_imagem_processo();
				else {
					printf("DEU MERDA - tipo indefinido\n");
					exit(1);
				}
				break;

			case PIXEL_MAXIMO:

				if(params.type == THREADS)
					printf("Pixel max via thread\n");
				else if (params.type == SEQUENCIAL)
					printf("PIXEL_MAXIMO\n");
				break;

			default:
				printf("Erro!\n");
				exit(EXIT_FAILURE);
				return 0;
		}

		timersub(&rt1, &rt0, &drt);
		mean_real_sec += drt.tv_sec;		// media em segundos
		mean_real_usec += drt.tv_usec;		// media em  micro segundos
		mean_clock += (double) (ct1 - ct0)/CLOCKS_PER_SEC;

	}

	mean_clock /= params.num_iteration;
	mean_real_usec /= params.num_iteration;
	mean_real_sec /= params.num_iteration;

  	free(valores);
}