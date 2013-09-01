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
	request req;

	// Le o que está vindo no socket
	// Termina quando recebe '\n'
	readSocket(buffer, MAXLINE);

	// Faz o parse da requisicao analisando buffer
	// Tratar GET ou POST
	// 
	req = parseRequest(buffer);

	// Define diretorio base
	// Verifica se existe


	// Envia resposta ao cliente
	sendRes();

}

request parseRequest(char req[]) {
	char *pBuffer;
	request req;

	pBuffer = strtok_r(req, " ", NULL);
	if(!pBuffer) {
		req.type = REQ_INVALID;
		req.protocol = PROT_INVALID;
		req.filePath[0] = '\0';
		return req;
	}

	if(strcmp(pBuffer, GET) == 0)

}

