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

	// Se raiz, concatena com index.html 
	if(strcasecmp(req.uri[streln(req.uri)-1], "/") {
		strcat(req.uri,"index.html");
	}
		
	// Verifica se arquivo existe no servidor
	if(stat(req.uri, &st) == -1) {
		res.status = 404; // File not Found
		res.fileName = "/404.html"; // 404 html file
	} else {
		res.status = 200; // ok
		res.fileName = req.uri;
	}

	// Verifica protocolo
	if(strcasecmp(req.vProtocol, "INVALID") == 0) {
		res.status = 501;
		printf("501 - Protocolo não suportado\n");
	} else {
		res.vProtocol = req.vProtocol;
	}

	// DEBUG
	printf("%s %s %s\n", req.method, req.uri, req.vProtocol);
	printf("%d %s %s\n", res.status, res.fileName, res.vProtocol);

	// Envia resposta ao cliente
	// sendRes(res);

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

void httpHeader(int client_socket,char *mimeType, int tamanho, time_t ultAtu, int request_check, char *check_type) {

    /* Preparando a data e hora */
	time_t agora;
	char bufHora[128];
	char buffer[100];

    /* Preparando o protocolo */
	sprintf(buffer, "%s %d %s\r\n", PROTOCOLO, request_check,check_type);
	write(client_socket, buffer, strlen(buffer));

    /* Preparando o nome do servidor */
	sprintf(buffer, "Server: %s\r\n", SERVERNAME);
	write(client_socket, buffer, strlen(buffer));

    /* Preparando a data/hota */
	agora = time(NULL);
	strftime(bufHora, sizeof(bufHora), RFC1123, gmtime(&agora));
    sprintf(buffer, "Date: %s\r\n", bufHora);
	write(client_socket, buffer, strlen(buffer));

    /* Preparando o tipo de conteúdo */
	sprintf(buffer, "Content-Type: %s\r\n", mimeType);
	write(client_socket, buffer, strlen(buffer));

    /* Preparando o tamanho */
	if (tamanho >= 0) {
		sprintf(buffer, "Content-Length: %d\r\n", tamanho);
		write(client_socket, buffer, strlen(buffer));
	}

    /* Preparando a data/hora da última modificação para efeitos de cache */
	if (ultAtu != -1){
		strftime(bufHora, sizeof(bufHora), RFC1123, gmtime(&ultAtu));
		sprintf(buffer, "Last-Modified: %s\r\n", bufHora);
		write(client_socket, buffer, strlen(buffer));
   	}

    /* Enviando linha em branco para finalizar o cabeçalho */
	write(client_socket, "\r\n", 2);
}

// Verifica mime types
char *get_mime_type(char *name){
	char *ext = strrchr(name, '.');
	if (!ext) return NULL;
	if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0) return "text/html";
	if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) return "image/jpeg";
	if (strcmp(ext, ".png") == 0) return "image/png";
	if (strcmp(ext, ".css") == 0) return "text/css";
	if (strcmp(ext, ".avi") == 0) return "video/x-msvideo";
	if (strcmp(ext, ".mpeg") == 0 || strcmp(ext, ".mpg") == 0) return "video/mpeg";
	return NULL;
}