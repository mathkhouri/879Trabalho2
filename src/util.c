#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "imageprocessing.h"

clock_t ct0, ct1, dct; 	// medida de tempo baseada no clock da CPU
struct timeval rt0,rt1,drt; 	// tempo baseada em tempo real
char tipo_imagem[3][6]= {"Tipo1","Tipo2","Tipo3"};

void manipula_imagem(void){
	imagem I = abrir_imagem(parametros.nome_imagem_base, parametros.brilho);
	
	salvar_imagem(parametros.nome_nova_imagem, &I);
}

void manipula_imagem_threads(void){
	imagem I = abrir_imagem_threads(parametros.nome_imagem_base, parametros.brilho,params.num_threads);
	
	salvar_imagem_threads(parametros.nome_nova_imagem, &I,params.num_threads);
}

void medir_tempo(Funcao func_analisada)
{

	gettimeofday(&rt0, NULL);
	ct0 = clock();
	func_analisada();
	ct1 = clock();
	gettimeofday(&rt1, NULL);
}


double run(void){

	double mean_clock = 0.0, mean_real_sec = 0.0, mean_real_usec = 0.0;

	for (int i = 0; i < params.num_iteration; i++){
		switch (parametros.operacao){
			case ALTERA_BRILHO:
			case COPIA_IMAGEM:

				if(params.type == THREADS)
					medir_tempo(manipula_imagem_threads);
				else if (params.type == SEQUENCIAL)
					medir_tempo(manipula_imagem);
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

	if (params.type == SEQUENCIAL){
		printf("Tempo real: %.6lf segundos. %s: Sequencial \n",mean_real_sec+mean_real_usec/1000000, tipo_imagem[params.type_image]);
		printf("Tempo user: %f segundos. %s: Sequencial \n",mean_clock,tipo_imagem[params.type_image]);
	}
	 else 
	{
		printf("Tempo real: %.6lf segundos. %s: Num Threads: %d\n", mean_real_sec+mean_real_usec/1000000,tipo_imagem[params.type_image],params.num_threads);
		printf("Tempo user: %f segundos. %s: Num Threads: %d\n",mean_clock,tipo_imagem[params.type_image],params.num_threads);	
	}

	//free(all_times);

}