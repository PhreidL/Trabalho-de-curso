/*
O arquivo gerado é separado em uma coluna de CPNJ e uma coluna de CPF.
*/
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <ctime>

#define A 600000
#define B 1200000
using namespace std;

//função que faz o devido calculo para os digitos verificadores do CNPJ
void calcCNPJ(char *cnpj){
    int sum = 0, mult = 5;

    for(int i = 0; i < 12; i++){
        sum = sum + ((cnpj[i] - '0') * mult);
        mult--;
        if(mult < 2)
            mult = 9;
    }

    int num = 11 - (sum % 11);

    if(num >= 10)
        num = 0;

    cnpj[12] = num + '0';

    mult = 6, sum = 0;

    for(int i = 0; i < 13; i++){

        sum = sum + ((cnpj[i] - '0') * mult);
        mult--;
        if(mult < 2)
            mult = 9;
    }

    num = 11 - (sum % 11);

    if(num >= 10)
        num = 0;

    cnpj[13] = num + '0';
}

//função que faz o devido calculo para os digitos verificadores do CPF
void calcCPF(char* cpf){
    int sum = 0, mult = 11;

    for(int i = 0; i < 9; i++){

        sum = sum + ((cpf[i] - '0') * (mult - 1));
        mult--;
    }
    
    int num = 11 - (sum % 11);

    if(num >= 10)
        num = 0;

    mult = 11, sum = 0;
    
    cpf[9] = num + '0';

    for(int i = 0; i < 10; i++){
        sum = sum + ((cpf[i] - '0') * (mult));
        mult--;
    }

    num = 11 - (sum % 11);
    if(num >= 10)
        num = 0;

    cpf[10] = num + '0';
}

/*void gravaArquivoS(char cnpj[A][14], char cpf[A][13]){
	FILE *calc;
    calc = fopen ("NOVABASESequencial.txt", "w+");
	fprintf(calc,"     CNPJ      |     CPF    \n");
	fprintf(calc,"____________________________\n");
    for(int i = 0; i < A; i++){
    	for(int j = 0; j < 14; j++){

            fprintf(calc, "%c", cnpj[i][j]);
	    }
      	fprintf(calc," | ", cnpj[i][15]);
	    for(int j = 0; j < 11; j++){
	        fprintf(calc, "%c", cpf[i][j]);
            //printf("%c", cpf[i][j]);
	    }
	    fprintf(calc,"\n", cpf[i][13]);
	}
	fclose(calc);
}*/

//função que grava os CNPJs e os CPFs com seus digitos verificadores
void gravaArquivoP(char cnpj[A][14], char cpf[A][13]){
	FILE *calc;
    calc = fopen ("NOVABASE.txt", "w+");
	fprintf(calc,"     CNPJ      |     CPF    \n");
	fprintf(calc,"____________________________\n");
    for(int i = 0; i < A; i++){
    	for(int j = 0; j < 14; j++){

            fprintf(calc, "%c", cnpj[i][j]);
	    }
      	fprintf(calc," | ", cnpj[i][15]);
	    for(int j = 0; j < 11; j++){
	        fprintf(calc, "%c", cpf[i][j]);
            //printf("%c", cpf[i][j]);
	    }
	    fprintf(calc,"\n", cpf[i][13]);
	}
	fclose(calc);
}

void processamentoS(char cpf[A][13], char cnpj[A][14]){
	
	std::clock_t c_start = std::clock();
    auto t_start = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < 600000; i++){
		calcCPF(cpf[i]);
		calcCNPJ(cnpj[i]);
	}
	std::clock_t c_end = std::clock();
    auto t_end = std::chrono::high_resolution_clock::now();
    printf("Tempo Serial: %f", (std::chrono::duration<double, std::milli>(t_end-t_start).count())/1000);
    //gravaArquivoS(cnpj,cpf);
}

/*função que faz o processamento do cálculo dos digitos do CPNJ e do CPF,
separando em várias threads e por fim, gravando os CPNJ's e CPF's com os
digitos devidamente gerados 
*/
void processamentoP(char cpf[A][13], char cnpj[A][14]){
	
	std::clock_t c_start = std::clock();
    auto t_start = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < 300000; i++){
        thread t1(calcCPF, cpf[i]);
        thread t3(calcCPF, cpf[i + 300000]);

        thread t2(calcCNPJ, cnpj[i]);
        thread t4(calcCNPJ, cnpj[i + 300000]);

        t1.join();
        t2.join();
        t3.join();
        t4.join();
    }
    
    std::clock_t c_end = std::clock();
    auto t_end = std::chrono::high_resolution_clock::now();
    printf("Tempo Paralelo: %f\n", (std::chrono::duration<double, std::milli>(t_end-t_start).count())/1000);
    gravaArquivoP(cnpj,cpf);
}

//função faz a separação e identificação dos CPNJs e CPFs
void identifica(char temp[B][13]){
	static char cpf[A][13], cnpj[A][14];
	int lCnpj = 0, lCpf = 0;
	
	for(int l = 0; l < B; l++){
        if(temp[l][0] <= '9' && temp[l][0] >= '0'){
            for(int c = 0; c < 13; c++){
                cnpj[lCnpj][c] = temp[l][c];
            }

            lCnpj++;
        }
        else{
            for(int d = 3; d < 13; d++){
                cpf[lCpf][d - 3] = temp[l][d];
            }

            lCpf++;
        }
    }
    processamentoP(cpf,cnpj);
    processamentoS(cpf,cnpj);
}

int main(){
    FILE *base;
    base = fopen ("BASEPROJETO.txt", "r");

    static char temp[B][13];

	//for que faz a leitura dos dados do arquivo, para a a matriz temporária
    for(int l = 0; l < B; l++){
        for(int c = 0; c < 13; c++)
            fscanf(base, "%c", temp[l]+c);
    }

    fclose(base);
    identifica(temp);
    return 0;
}
