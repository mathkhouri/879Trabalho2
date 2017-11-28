#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct user_parameters {
   char name_of_file[120];
   int number_threads;
   int num_iteracoes;
   int modo_execucao;
};


void parse (int argc, char* argv[], struct user_parameters* params){
	int i;
    for(i=1; i<argc; i++) {
    	 if(!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h")) {
            printf("----------------------------------------------\n");
            printf("-h, --help : Show help information\n");
            printf("-t : Number of tasks\n");
            printf("-n : name of file\n");
            printf("-i : numero de iteracoes\n");
            printf("-a : modo de execucao: 0 - sequencial, 1 - threads, 2 - processos\n");
            printf("----------------------------------------------\n");
         }	else if(!strcmp(argv[i], "-t")) {
	            if (++i < argc)
	                params->number_threads = atoi(argv[i]);
	            else {
	                fprintf(stderr, "-t requires a number\n");
	                exit(EXIT_FAILURE);
	            }

    	 } else if(!strcmp(argv[i], "-a")) {
	            if (++i < argc)
	                params->modo_execucao = atoi(argv[i]);
	            else {
	                fprintf(stderr, "-t requires a number\n");
	                exit(EXIT_FAILURE);
	            }

    	 } else if(!strcmp(argv[i], "-i")) {
	            if (++i < argc)
	                params->num_iteracoes = atoi(argv[i]);
	            else {
	                fprintf(stderr, "-i requires a number\n");
	                exit(EXIT_FAILURE);
	            }

    	 }	else if(!strcmp(argv[i], "-n")) {
	            if (++i < argc)
		            strcpy( params->name_of_file , (argv[i]));

	            else {
	                fprintf(stderr, "-n a file\n");
	                exit(EXIT_FAILURE);
	       	 	}

      	  }
        
    	}
	}


int main (int argc, char * argv[]){
	struct user_parameters params;
	/*linha 0 -> tempo real; linha 1 tempo user*/
	//double tempo[2][params.num_iteracoes];
	double media[2], aux = 0;
	char lixo[5][20], tipo[7]; 
	FILE *arq;
	

	params.num_iteracoes = 1;

	parse(argc, argv, &params);

	media[0] = media[1] = 0.0;

	int iteracao = 0, flag_tempo_user = 0;
	arq = fopen(params.name_of_file, "r");
	if(arq == NULL)
			printf("Erro, nao foi possivel abrir o arquivo\n");
	else if (params.modo_execucao == 1 || params.modo_execucao == 2)
		while( (fscanf(arq,"%s %s %lf %s %s %s %s %d\n",&lixo[0],&lixo[1], &aux, &lixo[2], &tipo, &lixo[3], &lixo[4], &params.number_threads))!=EOF ){
			//printf("%s %s %lf %s %s %s %s %d\n",lixo[0],lixo[1],  aux, lixo[2], tipo, lixo[3], lixo[4], params.number_threads);
			media[flag_tempo_user] += aux;
			//printf("aux[%d]: %lf\n",flag_tempo_user, aux);
			if (flag_tempo_user == 1){
				// so vai para a proxima iteracao, quando le os dois tempos.. user e real
				iteracao++;
			} 

			flag_tempo_user = 1 - flag_tempo_user;
		}
	else if (params.modo_execucao == 0)
		while( (fscanf(arq,"%s %s %lf %s %s %s\n",&lixo[0],&lixo[1], &aux, &lixo[2], &tipo, &lixo[3]))!=EOF ){
			//printf("%s %s %lf %s %s %s %s %d\n",lixo[0],lixo[1],  aux, lixo[2], tipo, lixo[3], lixo[4], params.number_threads);
			media[flag_tempo_user] += aux;
			//printf("aux[%d]: %lf\n",flag_tempo_user, aux);
			if (flag_tempo_user == 1){
				// so vai para a proxima iteracao, quando le os dois tempos.. user e real
				iteracao++;
			} 

			flag_tempo_user = 1 - flag_tempo_user;
		}

		/*i = 0 -> tempo real; i = 1 -> tempo user*/

		media[0] /= params.num_iteracoes;				// faz a média
		media[1] /= params.num_iteracoes;				// faz a média
		//printf("numero de iteracoes: %d\n",params.num_iteracoes );

	if (params.modo_execucao == 0)
	{
		printf("%s real: %lf %s %s %s\n",lixo[0], media[0], lixo[2], tipo, lixo[3]);
		printf("%s user: %lf %s %s %s\n",lixo[0], media[1], lixo[2], tipo, lixo[3]);
	}
	else if (params.modo_execucao == 1 ){
		printf("%s real: %lf %s %s %s %s %d\n",lixo[0], media[0], lixo[2], tipo, lixo[3], lixo[4], params.number_threads);
		printf("%s user: %lf %s %s %s %s %d\n",lixo[0], media[1], lixo[2], tipo, lixo[3], lixo[4], params.number_threads);
	}
	else{
		printf("%s real: %lf %s %s %s %s %d\n",lixo[0], media[0], lixo[2], tipo, lixo[3], "processos", params.number_threads);
		printf("%s user: %lf %s %s %s %s %d\n",lixo[0], media[1], lixo[2], tipo, lixo[3], "processos", params.number_threads);	
	}

	
	
	fclose(arq);


	return 0;
}