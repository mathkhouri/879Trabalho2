
SRCDIR=./src
BASH = sh
TEST_SCRIPT = test.sh

all: lex.yy.c y.tab.c lib_imageprocessing.o util.o
	gcc -omain lex.yy.c y.tab.c lib_imageprocessing.o util.o -ll -lm -lpthread -lfreeimage -I$(SRCDIR)
	gcc -o average src/average.c 2>> warning && rm warning

lex.yy.c:$(SRCDIR)/imageprocessing.l
	lex $(SRCDIR)/imageprocessing.l 

y.tab.c:$(SRCDIR)/imageprocessing.y $(SRCDIR)/imageprocessing.l
	bison -dy $(SRCDIR)/imageprocessing.y -Wconflicts-rr 2>> warnings && rm warnings

lib_imageprocessing.o:$(SRCDIR)/lib_imageprocessing.c
	gcc -c $(SRCDIR)/lib_imageprocessing.c -lm

util.o:$(SRCDIR)/util.c
	gcc -c $(SRCDIR)/util.c -lm

clean:
	rm *.h lex.yy.c y.tab.c *.o main

clean_all:
	rm -r tests
	rm *.h lex.yy.c y.tab.c *.o main average
	rm -r resultados
		

test:
	$(BASH) $(TEST_SCRIPT)
