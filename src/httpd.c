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
	request req;

	// Le o que está vindo no socket
	// Termina quando recebe '\n'
	readSocket(buffer, MAXLINE, connfd);

	// Faz o parse da requisicao analisando buffer
	// Tratar GET ou POST
	// 
	req = parseRequest(buffer);

	printf(req);
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

request parseRequest(char buffer[]) {
	char *pBuffer;
	char *pBuffer2;
	char *ptr = buffer;
	request req;

	pBuffer = strtok_r(ptr, " ", &pBuffer2);
	if(!pBuffer) {
		req.type = "INVALID";
		req.protocol = "INVALID";
		req.filePath[0] = '\0';
		return req;
	}

	// Se HTTP Request = "GET" 
	if(strcmp(pBuffer, GET) == 0)
		req.type = "GET";
	// Se HTTP Request = "POST"
	else if (strcmp(pBuffer, "POST") == 0) 
		req.type = "POST";

	// PATH
	strncpy(req.filePath, pBuffer, MAXLINE);
	req.filePath[MAXLINE] = '\0';

	// Verifica se ainda tem algo no buffer para ler
	// pBuffer = strtok_r(buffer, " ", &pBuffer2);
	// if(!pBuffer)
	// 	return req;

	// Se tiver, está sendo passado a versão do protocolo HTTP
	if(strcmp(pBuffer, "HTTP/1.0") == 0)
		req.protocolo = "HTTP/1.0";
	else if	(strcmp(pBuffer, "HTTP/1.1") == 0)
		req.protocolo = "HTTP/1.1";
	
	return req;
}

