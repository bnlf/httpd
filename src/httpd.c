/* 
* MAC0448/5910 - Programação para Redes de Computadores - EP1 @ 2013.2
* Alunos: Bruno Nunes Leal Faria - nUSP: 8765551
*         Elaine Naomi Watanabe - nUSP: 8766722
*
* FILE: httpd.c
*/

#include "httpd.h"

//chama o servidor passando o numero do socket
void httpd(int connfd) {
	char buffer[];

	// Le o que está vindo no socket
	// Termina quando recebe '\n'
	readSocket(buffer, MAXLINE, connfd);

	// Faz o parse da requisicao analisando buffer
	// Tratar GET ou POST
	// 
	parseBuffer(buffer);

	// Define diretorio base
	// Verifica se existe


	// Envia resposta ao cliente
	sendRes();

}

//le o socket e armazena no buffer
void readSocket(char buffer[], int tam, int connfd)
{

	char line;
	int i = 0;
		
	while(i < tam && read(connfd, &line, 1))
	{
		if(line == '\r')
		{
			continue;
		}	
		else if(line == '\n')
		{
			break;
		}	
		else
		{
			buffer[i++] = line;
		}
	}
	
	buffer[i]= '\0';	

}

char parseBuffer(char** buffer) {

}

