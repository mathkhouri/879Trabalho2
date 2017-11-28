# Trabalho 2: Uma linguagem para processamento de imagens - Paralelizado

     ****** Autores ******

	Fernando Suzuki
	Guilherme Pupio
	Josiane Souza
	Matheus Khouri

     *********************

## Manual para execucao de testes automatizados

Estando neste diretório, ao rodar o `make` temos as seguintes possibilidades:

* `make`		: Compila os arquivos contidos na pasta `src`, gerando um executavel `main` neste mesmo diretorio.

* `make test`	: Executa o script `test.sh`, que testa todos os casos requeridos no projeto. Ao executar este script, todos os dados sao salvos no diretorio `tests`, que possui os resultados parciais de cada execucao. O diretorio `tests` contem diversos diretorios intuitivos sobre os resultados parciais. Além do diretório `tests` é gerado o diretorio `resultados` onde há outros 4 diretórios que sao explicados a seguir.

	* `resultados/sequencial` -> Apresenta todos os resultados gerados (linhas,colunas e tipos de imagem) devido as execucoes seriais.	

	* `resultados/threads`	-> Apresenta todos os resultados gerados (linhas,colunas, e tipos de imagem) devido as execucoes com variados numero de threads.

	* `resultados/processos`	-> Apresenta todos os resultados gerados (linhas,colunas, e tipos de imagem) devido as execucoes com variados numero de processos.

	* `resultados/geral` 	-> Apresenta arquivos contendo os tempos execucao comparando sequencial, threads e processos para cada modo de execucao (linhas, colunas e tipo de imagem).

* `make clean`	: Limpa o diretorio, deixando apenas os diretorios `tests`, `resultados`, `src` e as imagens e o script `test.sh`.

* `make clean_all` : Limpa todo o diretorio, deixando a penas as imagens, o script `test.sh` e as imagens.


##  Manual para execucao de testes manuais.

Para uma execucao manual recomendamos o uso da ferramenta `echo` como segue o exemplo

 `echo teste.jpg = tipo1.jpg * 2.5 | ./main`

O programa `main` foi desenvolvido de forma que o usuario possa optar pelo modo de execucao. Executando o comando `./main -h` mostramos ao usuario, um pequeno manual com os parametros que o programa aceita. A seguir temos um exemplo com todos os parametros aceitos pelo programa.

Exemplo:

`echo teste.jpg = tipo1.jpg / 1.0 | ./main -a MODO_DE_EXECUCAO -t TIPO_DA_IMAGEM -i NUMERO_DE_ITERACOES -m MODO_VARREDURA -n NUMERO_PARALELO`

onde 

* MODO_DE_EXECUCAO = 0 ou 1 ou 2. Onde os valores anteriores levam a execucoes seriais, com threads, com processos, respectivamente.

* TIPO_DA_IMAGEM = 1 ou 2 ou 3. Onde os valores anteriores avisam ao programa se as entradas serao imagens pequenas, medias ou grandes. (afeta apenas a saida do programa)

* NUMERO_DE_ITERACOES = n. Onde `n` pertence aos inteiros positivos. 

* MODO_VARREDURA = 0 ou 1. Onde os valores anteriores levam a varredura da matriz por linha ou coluna, respectivamente.

* NUMERO_PARALELO = n. Onde `n` pertence aos inteiros positivos. Avisa o programa quantas threads/processos serao geradas.

Caso algum destes parametros nao seja utilizado, o programa tratara como se o parametro tivesse recebido o respectivo primeiro caso, dos descritos acima.

Exemplo:
	A seguinte instrucao executara a divisao do brilho da imagem por 1.6 por meio de 4 threads, no modo de varredura linha com apenas uma iteracao. 

`echo teste.jpg = tipo2.jpg / 1.6 | ./main -a 1 -n 4`

 
## Nota importante.

É muito importante seguir a sintaxe proposta no exemplo. Como por exemplo, o brilho sempre deve ser um float, contendo 'numero ponto numeros'. Tambem é necessario seguir os espacos entre cada elemento como `nome_da_saida \espaco \igual \espaco nome_da_entrada \espaco \operacao \espaco \float`.


Caso surja alguma duvida de como executar, entre em contato conosco!

* guicpupio@gmail.com
* mathkhouri@gmail.com
* fersuzuki.fs@gmail.com
* josi.canobel@gmail.com


