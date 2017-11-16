echo "\033[0;31m\n\t\t\t ***** START THE SCRIPT *****\033[0m \n"

## melhorar isso.. esta mostrando mensagem de erro.
if ! find tests >>ops ; then
	echo "\033[0;31m\n\t\t\t ***** CRIANDO PASTAS PARA GUARDAR OS TESTES *****\033[0m \n"
	
	mkdir tests 
	
	for i in `seq 3`;do
		mkdir tests/tipo$i;
		mkdir tests/tipo$i/resultados;
		mkdir tests/tipo$i/resultados/threads;
		mkdir tests/tipo$i/resultados/sequencial;
		mkdir tests/tipo$i/imagens;
		mkdir tests/tipo$i/imagens/threads;
		mkdir tests/tipo$i/imagens/sequencial;
	done
	echo "\033[0;31m\n\t\t\t ***** Pastas criadas *****\033[0m \n";
fi

rm ops


##########################################

#rodando COM THREADS
#k representa o numero de threads
#j representa o tipo da imagem

# Executa Threads
for k in `seq 10`;do 
    for j in `seq 3`;do
        echo "teste$k.jpg = tipo$j.jpg * 2.0" | ./main -a 1 -n $k -t $j -i 2 >> tests/tipo$j/resultados/threads/result;
        mv teste$k.jpg tests/tipo$j/imagens/threads;
       # sleep 1;
    done; 
done
#rodando Sequencial

#j representa o tipo da imagem

# Executa sequencial
unset j
for j in `seq 3`;do
    echo "teste.jpg = tipo$j.jpg * 2.0" | ./main -a 0 -t $j -i 2 >> tests/tipo$j/resultados/sequencial/result;
    mv teste$k.jpg tests/tipo$j/imagens/sequencial;
   # sleep 1;
done 

## facilita analise para a gente
for i in `seq 3`; do
	cat tests/tipo$i/resultados/threads/result | grep user | sort -n > result_user_tipo$i.txt;
	cat tests/tipo$i/resultados/threads/result | grep real | sort -n > result_real_tipo$i.txt;
done

