echo "\033[0;31m\n\t\t\t ***** INICIANDO SCRIPT *****\033[0m \n"
 date >> tempo_executando
# ------------------------------------------- Criando pastas para armazenar testes --------------------------------- #
if ! find tests >> ops ; then
	echo "\033[0;31m\n\t\t\t ***** CRIANDO PASTAS PARA GUARDAR OS TESTES *****\033[0m \n"
	
	mkdir tests 
	
	for i in `seq 2`;do

		mkdir tests/tipo$i;

		mkdir tests/tipo$i/resultados;

		mkdir tests/tipo$i/resultados/threads;
		mkdir tests/tipo$i/resultados/sequencial;
		mkdir tests/tipo$i/resultados/processo;

		mkdir tests/tipo$i/imagens;

		mkdir tests/tipo$i/imagens/threads;
		mkdir tests/tipo$i/imagens/sequencial;
		mkdir tests/tipo$i/imagens/processo;

		mkdir tests/tipo$i/resultados/threads/varredura_linha;
		mkdir tests/tipo$i/resultados/sequencial/varredura_linha;
		mkdir tests/tipo$i/resultados/processo/varredura_linha;
		mkdir tests/tipo$i/resultados/threads/varredura_coluna;
		mkdir tests/tipo$i/resultados/sequencial/varredura_coluna;
		mkdir tests/tipo$i/resultados/processo/varredura_coluna;

		mkdir tests/tipo$i/imagens/threads/varredura_linha;
		mkdir tests/tipo$i/imagens/sequencial/varredura_linha;
		mkdir tests/tipo$i/imagens/processo/varredura_linha;
		mkdir tests/tipo$i/imagens/threads/varredura_coluna;
		mkdir tests/tipo$i/imagens/sequencial/varredura_coluna;
		mkdir tests/tipo$i/imagens/processo/varredura_coluna;

	done
	echo "\033[0;31m\n\t\t\t ***** Pastas criadas *****\033[0m \n";
fi

rm ops		# se as pastas existem, evita que sejam printadas no terminal

# ----------------------------------------------- FIM CRIACAO DAS PASTAS ----------------------------------------- #


# ------------------------------------------------------------ Executa Threads ----------------------------------------------------------------# 

echo "\033[0;31m\n\t\t\t ***** EXECUTANDO MODO THREADS *****\033[0m \n"


for k in `seq 10`;do 		#k representa o numero de threads
    for j in `seq 2`;do   	#j representa o tipo da imagem
       for i in `seq 30`;do
	       	echo "teste$k.jpg = tipo$j.jpg * 2.0" | ./main -a 1 -n $k -t $j -i 1 -m 0 >> result_parciais_linha$j.txt;
	        mv teste$k.jpg tests/tipo$j/imagens/threads/varredura_linha;
	        echo "teste$k.jpg = tipo$j.jpg * 2.0" | ./main -a 1 -n $k -t $j -i 1 -m 1 >> result_parciais_coluna$j.txt;
	        mv teste$k.jpg tests/tipo$j/imagens/threads/varredura_coluna;
	        printf "\rTrabalhando com $k threads: iteracao $i/30 | $j/2 dos tipos concluido.";
	        sleep 0.5;
	    done;
	    echo;
	    ./average -n result_parciais_linha$j.txt -i 30 -a 1 >> tests/tipo$j/resultados/threads/varredura_linha/result;
	    mv result_parciais_linha$j.txt tests/tipo$j/resultados/threads/varredura_linha/;
	    ./average -n result_parciais_coluna$j.txt -i 30 -a 1 >> tests/tipo$j/resultados/threads/varredura_coluna/result;
	    mv result_parciais_coluna$j.txt tests/tipo$j/resultados/threads/varredura_coluna/;

	done
done

echo "\033[0;31m\n\t\t\t ***** MODO THREADS CONCLUIDO. *****\033[0m \n"

# -------------------------------------------------------------  FIM THREADS  -----------------------------------------------------------------#




# ------------------------------------------------------------ Executa Processos ----------------------------------------------------------------# 

echo "\033[0;31m\n\t\t\t ***** EXECUTANDO MODO PROCESSOS *****\033[0m \n"
# Garante a limpeza das variaveis #
# unset k
# unset j
# unset i

for k in `seq 10`;do 		#k representa o numero de threads
    for j in `seq 2`;do   	#j representa o tipo da imagem
       for i in `seq 30`;do
	       	echo "teste$k.jpg = tipo$j.jpg * 2.0" | ./main -a 2 -n $k -t $j -i 1 -m 0 >> result_parciais_linha$j.txt;
	        mv teste$k.jpg tests/tipo$j/imagens/processo/varredura_linha;
	        echo "teste$k.jpg = tipo$j.jpg * 2.0" | ./main -a 2 -n $k -t $j -i 1 -m 1 >> result_parciais_coluna$j.txt;
	        mv teste$k.jpg tests/tipo$j/imagens/processo/varredura_coluna;
	        printf "\rTrabalhando com $k processo: iteracao $i/30 | $j/2 dos tipos concluido.";
	        sleep 0.5;
	    done;
	    echo;
	    ./average -n result_parciais_linha$j.txt -i 30 -a 2 >> tests/tipo$j/resultados/processo/varredura_linha/result;
	    mv result_parciais_linha$j.txt tests/tipo$j/resultados/processo/varredura_linha/;
	    ./average -n result_parciais_coluna$j.txt -i 30 -a 2 >> tests/tipo$j/resultados/processo/varredura_coluna/result;
	    mv result_parciais_coluna$j.txt tests/tipo$j/resultados/processo/varredura_coluna/;

	done
done

echo "\033[0;31m\n\t\t\t ***** MODO PROCESSOS CONCLUIDO. *****\033[0m \n"

# -------------------------------------------------------------  FIM PROCESSOS  -----------------------------------------------------------------#


# ------------------------------------------------------------ Executa Sequencial ----------------------------------------------------------------# 

echo "\033[0;31m\n\t\t\t ***** EXECUTANDO MODO SEQUENCIAL *****\033[0m \n"

unset j 	# Garante a limpeza das variaveis

for j in `seq 2`;do  	#j representa o tipo da imagem
	 for i in `seq 30`;do
		    echo "teste.jpg = tipo$j.jpg * 2.0" | ./main -a 0 -t $j -i 1 -m 0 >> result_parciais_linha$j.txt;
		    mv teste.jpg tests/tipo$j/imagens/sequencial/varredura_linha;
		    echo "teste.jpg = tipo$j.jpg * 2.0" | ./main -a 0 -t $j -i 1 -m 1 >> result_parciais_coluna$j.txt;
		    mv teste.jpg tests/tipo$j/imagens/sequencial/varredura_coluna;
		    printf "\rSequencial: iteracao $i/30 | $j/2 dos tipos concluido.";
		       sleep 0.5;
	    done;
	    echo;
	    ./average -n result_parciais_linha$j.txt -i 30 -a 0 >> tests/tipo$j/resultados/sequencial/varredura_linha/result;
	    mv result_parciais_linha$j.txt tests/tipo$j/resultados/sequencial/varredura_linha/;
	    ./average -n result_parciais_coluna$j.txt -i 30 -a 0 >> tests/tipo$j/resultados/sequencial/varredura_coluna/result;
	    mv result_parciais_coluna$j.txt tests/tipo$j/resultados/sequencial/varredura_coluna/;
done 
# printa uma quebra de linha
echo 

echo "\033[0;31m\n\t\t\t ***** MODO SEQUENCIAL CONCLUIDO. *****\033[0m \n"

# -------------------------------------------------------------  FIM SEQUENCIAL  -----------------------------------------------------------------#


# ----------------------------------------------------------- Analise automatizada ---------------------------------------------------------------# 
 
echo "\033[0;31m\n\t\t\t ***** EXECUTANDO ANALISE DOS DADOS OBTIDOS... *****\033[0m \n"

if  ! find resultados >> ops; then
	echo "Criando pastas para resultados..."
	mkdir resultados
	mkdir resultados/threads
	mkdir resultados/processo
	mkdir resultados/sequencial
	mkdir resultados/geral
	echo "Pastas criadas com sucesso!!"
fi

rm ops

unset i

for i in `seq 2`; do

		printf "$i/2 da analise concluida.\r";
	
	cat tests/tipo$i/resultados/threads/varredura_linha/result | grep "Num Threads: [0-9]*" | grep user | sort -n > resultados/threads/result_user_threads_linha_tipo$i.txt;

	cat tests/tipo$i/resultados/threads/varredura_linha/result | grep "Num Threads: [0-9]*" | grep real | sort -n > resultados/threads/result_real_threads_linha_tipo$i.txt;

	cat tests/tipo$i/resultados/threads/varredura_coluna/result | grep "Num Threads: [0-9]*" | grep user | sort -n > resultados/threads/result_user_threads_coluna_tipo$i.txt;

	cat tests/tipo$i/resultados/threads/varredura_coluna/result | grep "Num Threads: [0-9]*" | grep real | sort -n > resultados/threads/result_real_threads_coluna_tipo$i.txt;


	cat tests/tipo$i/resultados/processo/varredura_linha/result | grep "Num processos [0-9]*" | grep user | sort -n > resultados/processo/result_user_processo_linha_tipo$i.txt;

	cat tests/tipo$i/resultados/processo/varredura_linha/result | grep "Num processos [0-9]*" | grep real | sort -n > resultados/processo/result_real_processo_linha_tipo$i.txt;

	cat tests/tipo$i/resultados/processo/varredura_coluna/result | grep "Num processos [0-9]*" | grep user | sort -n > resultados/processo/result_user_processo_coluna_tipo$i.txt;

	cat tests/tipo$i/resultados/processo/varredura_coluna/result | grep "Num processos [0-9]*" | grep real | sort -n > resultados/processo/result_real_processo_coluna_tipo$i.txt;


	cat tests/tipo$i/resultados/sequencial/varredura_linha/result | grep "Sequencial" | grep user | sort -n > resultados/sequencial/result_user_sequencial_linha_tipo$i.txt;

	cat tests/tipo$i/resultados/sequencial/varredura_linha/result | grep "Sequencial" | grep real | sort -n > resultados/sequencial/result_real_sequencial_linha_tipo$i.txt;

	cat tests/tipo$i/resultados/sequencial/varredura_coluna/result | grep "Sequencial" | grep user | sort -n > resultados/sequencial/result_user_sequencial_coluna_tipo$i.txt;

	cat tests/tipo$i/resultados/sequencial/varredura_coluna/result | grep "Sequencial" | grep real | sort -n > resultados/sequencial/result_real_sequencial_coluna_tipo$i.txt;
	sleep 0.7

done

for i in `seq 2`; do

	cat resultados/sequencial/result_real_sequencial_coluna_tipo$i.txt >> resultados/geral/real_coluna_tipo$i.txt;
	cat resultados/threads/result_real_threads_coluna_tipo$i.txt >> resultados/geral/real_coluna_tipo$i.txt;
	cat resultados/processo/result_real_processo_coluna_tipo$i.txt >> resultados/geral/real_coluna_tipo$i.txt;

	cat resultados/sequencial/result_real_sequencial_linha_tipo$i.txt >> resultados/geral/real_linha_tipo$i.txt;
	cat resultados/threads/result_real_threads_linha_tipo$i.txt >> resultados/geral/real_linha_tipo$i.txt;
	cat resultados/processo/result_real_processo_linha_tipo$i.txt >> resultados/geral/real_linha_tipo$i.txt;

	cat resultados/sequencial/result_user_sequencial_coluna_tipo$i.txt >> resultados/geral/user_coluna_tipo$i.txt;
	cat resultados/threads/result_user_threads_coluna_tipo$i.txt >> resultados/geral/user_coluna_tipo$i.txt;
	cat resultados/processo/result_user_processo_coluna_tipo$i.txt >> resultados/geral/user_coluna_tipo$i.txt;

	cat resultados/sequencial/result_user_sequencial_linha_tipo$i.txt >> resultados/geral/user_linha_tipo$i.txt;
	cat resultados/threads/result_user_threads_linha_tipo$i.txt >> resultados/geral/user_linha_tipo$i.txt;
	cat resultados/processo/result_user_processo_linha_tipo$i.txt >> resultados/geral/user_linha_tipo$i.txt;

	cat resultados/geral/real_linha_tipo$i.txt  | sort -n > resultados/geral/real_linha_tipos$i.txt 
	cat resultados/geral/real_coluna_tipo$i.txt | sort -n > resultados/geral/real_coluna_tipos$i.txt 
	cat resultados/geral/user_linha_tipo$i.txt | sort -n > resultados/geral/user_linha_tipos$i.txt 
	cat resultados/geral/user_coluna_tipo$i.txt  | sort -n > resultados/geral/user_coluna_tipos$i.txt 

	rm resultados/geral/real_linha_tipo$i.txt 
	rm resultados/geral/real_coluna_tipo$i.txt
	rm resultados/geral/user_linha_tipo$i.txt 
	rm resultados/geral/user_coluna_tipo$i.txt

done
echo
echo "\033[0;31m\n\t\t ***** DADOS SALVOS EM RESULTADOS *****\033[0m \n"
# ----------------------------------------------------------- FIM ANALISE DE DADOS ---------------------------------------------------------------#

echo "\033[0;31m\n\t\t\t ***** FIM SCRIPT *****\033[0m \n"

date >> tempo_executando
#shutdown