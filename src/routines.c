/**
*Este módulo contém um analisador recursivo descendente de expessões simples, que não
*reconheçe variáveis.
*Created by: Dias Culenga Daniel.
*Version 1.0
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define DELIMITADOR 1
#define VARIAVEL    2
#define NUMERO      3

#define MAX_TOKEN 80

extern char *exp; /*Contém a expressão.*/
char token[MAX_TOKEN];
char tok_type;

void eval_exp(double *answer), eval_exp2(double *answer);
void eval_exp3(double *answer), eval_exp4(double *answer);
void eval_exp5(double *answer);
void eval_exp6(double *answer), atom(double *answer);
void get_token(void), putback(void);
void serror(int error);
int isdelim(char c);

/*Ponto de entrada do analisador.*/
void eval_exp(double *answer){
	get_token();
	if(!*token){
		serror(2);
		return;
	}
	eval_exp2(answer);
	if(*token) serror(0);/*último token deve ser null*/
}

/*Soma ou subtrai dois termos*/
void eval_exp2(double *answer){
	register char op;
	double tmp;
	
	eval_exp3(answer);
	while((op = *token)=='+'|| op =='-'){
		get_token();
		eval_exp3(&tmp);
		switch(op){
			case '-': *answer = *answer - tmp;break;
			case '+': *answer = *answer +tmp;break;
		}
	}
}

/*Multiplica ou divide dois factores*/
void eval_exp3(double *answer){
	register char op;
	double tmp;
	
	eval_exp4(answer);
	while((op = *token)== '*'|| op == '/'|| op == '%'){
		get_token();
		eval_exp4(&tmp);
		switch(op){
			case '*': *answer = *answer * tmp;break;
			case '/': *answer = *answer / tmp;break;
			case '%': *answer = (int) *answer % (int)tmp;break;
		}
	}
}

/*Processa um expoente.*/
void eval_exp4(double *answer){
	double tmp, ex;
	register int t;
	
	eval_exp5(answer);
	if(*token=='^'){
		get_token();
		eval_exp4(&tmp);
		ex = *answer;
		if(tmp == 0.0){
			*answer = 1.0;
			return;
		}
		for(t=tmp-1; t>0; --t) *answer =(*answer) * (double)ex;
	}
}

/*Avalia um + ou - unário.*/
void eval_exp5(double *answer){
	register char op;
	
	op = 0;
	if((tok_type==DELIMITADOR)&& *token=='+'|| *token== '-'){
		op = *token;
		get_token();
	}
	eval_exp6(answer);
	if(op == '-') *answer = -(*answer);
}

/*Processa uma expressão entre parênteses.*/
void eval_exp6(double *answer){
	if((*token == '(')){
		get_token();
		eval_exp2(answer);
		if(*token!=')')
			serror(1);
		get_token();
	}
	else atom(answer);
}

/*Ontém o valor real de um número.*/
void atom(double *answer){
	if(tok_type==NUMERO){
		*answer = atof(token);
		get_token();
		return;
	}
	serror(0);/*Caso contrário erro de sintaxe na expressão.*/
}

/*Devolve um token à stream de entrada.*/
void putback(void){
	char *t;
	t = token;
	for(;*t;t++) exp--;
}

/*Apresenta um erro de sintaxe.*/
void serror(int error){
	static char *e[] = {
		"Erro de sinraxe",
		"Falta parênteses",
		"Nenhuma expressão presente"
	};
	printf("%s\n", e[error]);
}

/*Devolve o próximo token.*/
void get_token(void){
	register char *tmp;
	
	tok_type = 0;
	tmp = token;
	*tmp = '\0';
	
	if(!*exp)return ;/*Final na expressão.*/
	while(isspace(*exp)) ++exp; /*Ignora os espaços.*/
	
	if(strchr("+-*/^=()",*exp)){
		tok_type = DELIMITADOR;
		/*avança para o próximo char*/
		*tmp++ = *exp++; 
	}
	
	else if(isalpha(*exp)){
		while(!isdelim(*exp))*tmp++ = *exp++;
		tok_type = VARIAVEL;
	}
	else if(isdigit(*exp)){
		while(!isdelim(*exp))*tmp++ = *exp++;
		tok_type = NUMERO;
	}
	
	*tmp = '\0';
}

/*Devolve verdadeiro se c é um delimitador*/
int isdelim(char c){
		if(strchr("+-*/^%=()",c)||c=='\r'||c==9|| c==0)
			return 1;
		return 0;
	}
	