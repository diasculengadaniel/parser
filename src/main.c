#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

char *exp;
void eval_exp(double *answer);/*Esse negócio ta baita estranho.*/

void main(void){
	double answer;
	char *p;
	
	p = malloc(100);
	if(!p){
		printf("Falha na alocação.\n");
		exit(1);
	}
	
	/*Processa expressões até que uma linha em branco seja digitada.*/
	do{
		exp = p;
		printf("Digite a expressão: ");
		gets(exp);
		if(!*exp)break;
		eval_exp(&answer);
		printf("A resposta é %.2f\n",answer);
	}while(*p);
}