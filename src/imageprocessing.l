%{
#include <stdlib.h>
#include <string.h>
#include <math.h>
void yyerror(char *);

#include "y.tab.h"

%}



%%

[a-z]  {
        yylval.ival = yytext[0] - 'a';
        return VAR;
       }


\" { return ASPA; }

[a-zA-Z0-9\.]+  {
        for (int i = 0; i < yyleng; i++ )
          yylval.strval[i] = yytext[i];

        yylval.strval[yyleng] = '\0';
        //printf ("Achei nome: %s\n", yylval.strval);
        return STRING;
       }

[=]     {
          return IGUAL;
        }



([/*]\ )([0-9]+.[0-9]+)  {

	int tamanho =  strlen(yytext);
	float brilho = 0.0;
	int i = 0, j = 0;

	for (i = 2; yytext[i] != '.'; i++);
	for (j = 2; j<i ; j++){
		brilho += (float) (yytext[j] - '0')*((float) pow(10, i -j -1)); 
	}
	for (j = i+1 ; j < tamanho ; j++){
		brilho += ((float) (yytext[j] - '0'))/((float) pow(10, j -i));
	}
	
	if (yytext[0] == '/'){

		yylval.fval = 1.0/brilho;
		//yylval.fval[0] = 1.0 ;
	} else if (yytext[0] == '*') {
		yylval.fval = brilho;
		//yylval.fval[0] = 0.0 ;	
	}
	//printf("Passando: %f\n", yylval.fval);
          return Operacao;
        }

(\[[a-zA-Z0-9\.]+\])	{
	
	//printf("nome1: %s\n",yytext);
	for (int i = 1; i < yyleng -1; i++ ){
        	yylval.strval[i-1] = yytext[i];
	}
	yylval.strval[yyleng-1] = '\0';
	//printf("nome2: %s\n",yylval.strval);
	return MAX_PIXEL;
}


\n   {
         return EOL;
       }

[ \t]+ ;

[[:space:]] ;

.     {printf("**%s**\n", yytext);
yyerror("Token nao reconhecido!\n");}

%%

int  yywrap() {
  return 1;
}