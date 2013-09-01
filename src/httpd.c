/* 
* MAC0448/5910 - Programação para Redes de Computadores - EP1 @ 2013.2
* Alunos: Bruno Nunes Leal Faria - nUSP: 8765551
*         Elaine Naomi Watanabe - nUSP: 8766722
*
* FILE: httpd.c
*/

#include "httpd.h"

void httpd(int connfd) {
	char buffer[];

	// Le o que está vindo no socket
	// Termina quando recebe '\n'
	readSocket(buffer, MAXLINE);

	// Faz o parse da requisicao analisando buffer
	// Tratar GET ou POST
	// 
	parseBuffer(buffer);

	// Define diretorio base
	// Verifica se existe


	// Envia resposta ao cliente
	sendRes();

}

char parseBuffer(char** buffer) {

}

