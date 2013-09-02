/* 
* MAC0448/5910 - Programação para Redes de Computadores - EP1 @ 2013.2
* Alunos: Bruno Nunes Leal Faria - nUSP: 8765551
*         Elaine Naomi Watanabe - nUSP: 8766722
*
* FILE: httpd.c
*/

#include "httpd.h"

void httpd(int connfd) {

	char buffer[MAXLINE]; // Buffer dos dados de input
	request req; // Pedido do cliente
	response res; // Resposta do servidor
	struct stat st;

	// Le o que está vindo no socket
	readSocket(buffer, MAXLINE, connfd);

	// Faz o parse da requisicao 
	req = parseRequest(buffer);

	// Verifica metodo
	if(strcasecmp(req.method, "INVALID") == 0) {
		res.status = 501; // Not Implemented
	}

	// Verifica Uri
	// if(stat(req.uri, &st) == 0 && S_ISREG(st.st_mode)) {
	// 	res.status = 200; // Ok
	// 	res.fileName = req.uri;
	// } else {
	// 	res.status = 404; // File not Found
	// 	strcat(res.fileName, "/404.html");
	// }

	// Verifica protocolo
	if(strcasecmp(req.vProtocol, "INVALID") == 0) {
		res.status = 501;
		printf("501 - Protocolo não suportado\n");
	} else {
		res.vProtocol = req.vProtocol;
	}

	// DEBUG
	// printf("%s %s %s\n", req.method, req.uri, req.vProtocol);
	// printf("%d %s %s\n", res.status, res.fileName, res.vProtocol);

	// Envia resposta ao cliente
	// sendRes();

}

/* le uma linha do socket e armazena no buffer
* Params: 
* 		@buffer buffer para salvar os dados de input
* 		@tam tamanho do buffer
*		@connfd descritor do socket
*/			
void readSocket(char buffer[], int tam, int connfd)
{
	// vars de suporte
	char line;
	int i = 0;

	while(i < tam && read(connfd, &line, 1))
	{
		if(line == '\r') {
			continue;
		}
		// Termina se detectar \n (line feed)
		else if(line == '\n') {
			break;
		}	
		else {
			buffer[i++] = line;
		}
	}
	
	buffer[i]= '\0';	
}

request parseRequest(char buffer[]) {
	char *ptr = buffer;
	char method[MAXLINE], uri[MAXLINE], vProtocol[MAXLINE];
	request req;
	
	sscanf(ptr, "%s %s %s", method, uri, vProtocol); 

	// Somente GET ou POST
	if(strcasecmp(method, "GET") == 0) 
        req.method = "GET";
 	else if (strcasecmp(method, "POST") == 0) 
        req.method = "POST";
    else {
    	req.method = "INVALID";
		req.vProtocol = "INVALID";
		req.uri[0] = '\0';
		return req;
    }

    // Sera testado futuramente. Por enquanto aceita que é um uri valido
    req.uri = uri;
    
    if(strcasecmp(vProtocol, "HTTP/1.0") == 0)
		req.vProtocol = "HTTP/1.0";
	else if	(strcasecmp(vProtocol, "HTTP/1.1") == 0)
		req.vProtocol = "HTTP/1.1";
    else
    	req.vProtocol = "INVALID";

	return req;
}

