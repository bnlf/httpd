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
	char fileBuffer[MAXLINE];
	request req; // Pedido do cliente
	response res; // Resposta do servidor
	struct stat st;
	int n;
	int sizeContent = -1;

	// Le o que está vindo no socket
	n=read(connfd, buffer, MAXLINE);
	
	int i = strlen(buffer);
	char options[MAXLINE];
	int statusRead = 0;
	strcpy(options, buffer);

	while(statusRead == 0)
	{
		if((options[i-3] == '\n' && options[i-1] == '\n') || options[i-1] != '\n')
		{
			statusRead = 1;
		}
		else
		{
			n=read(connfd, options, MAXLINE);
			//strcat(buffer, options);
			//printf("%s\n", buffer);
			i = strlen(options);

			if(options[0] == '\r' && options[1] == '\n' && n == 2)
				statusRead = 1;
		}
	}

	// Faz o parse da requisicao 
	req = parseRequest(buffer);

	char *linePost;

	//Encontra no buffer o tamanho do conteudo	
	if(strcmp(req.method, "POST") ==0)
	{
		linePost = getLastLineRead(buffer);
	}	

	// Verifica metodo
	if(strcasecmp(req.method, "INVALID") == 0) {
		res.status = 501; // Not Implemented
	}

	// Dir base dos arquivos do servidor
	strcpy(fileBuffer, WWW_ROOT);	
	
	// Arquivo do request
	if(req.uri) {
		strcat(fileBuffer, req.uri);	
	}

	// Se terminado em /, abre o arquivo padrao
	if(strcmp(&fileBuffer[strlen(fileBuffer)-1], "/") == 0) {
		strcat(fileBuffer,"index.html");
	}

	// Verifica se arquivo existe no servidor
	if(stat(fileBuffer, &st) == -1) {
		res.status = 404; // File not Found
		res.fileName = "404.html";
	} else {
		res.status = 200; // ok
		res.fileName = fileBuffer;
	}

	// Verifica protocolo
	if(strcasecmp(req.vProtocol, "INVALID") == 0) {
		res.status = 501;
	} else {
		res.vProtocol = req.vProtocol;
	}

	// DEBUG
	printf("PEDIDO: %s %s %s\n", req.method, req.uri, req.vProtocol);
	printf("RESPOSTA: %d %s %s\n", res.status, res.fileName, res.vProtocol);

	// Envia resposta ao cliente
	sendResponse(req, res, connfd, linePost);

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
    	req.vProtocol = "HTTP/1.1"; // se nao especificado

	return req;
}

int sendResponse(request req,response res, int connfd, char *linePost){

	if(strcmp(req.method,"POST") == 0)
	{
		return sendPostMessage(req, res,connfd, linePost);
	}
	else if (res.status == 501) // Não suportado
	{
		//"Not Implemented"
		return sendErrorMessage(res.status, req, res, "Not Implemented", connfd);
	}	
	else if (res.status == 404) // Arquivo não encontrado
	{
		//"Page not found";
		return sendErrorMessage(res.status, req, res, "Page not found", connfd);
	} 
	else if (res.status == 200 ) // Ok
	{ 
		return sendFile(req, res,connfd);
	}
	else
	{
		//"500 Internal Server Error" - default
		return sendErrorMessage(500, req, res, "Internal Server Error", connfd);
	}


	return 0;
}

int sendErrorMessage(int status, request req, response res, char *message, int connfd){

	char buffer[MAXLINE];

	//envia o header com o numero do erro
	sendHeader(connfd, req, res, message, "text/html");

	//envia o html para que o usuario visualize a mensagem no navegador
	sprintf(buffer, "<html><head><title>Error</title></head>");
	write(connfd, buffer, strlen(buffer));

	sprintf(buffer, "<body><h1>%s says: error %d</h1><p>%s</p></body>", SERVERNAME,res.status, message);
	write(connfd, buffer, strlen(buffer));

	return 1;
}

int sendPostMessage(request req, response res, int connfd, char *linePost){

	char buffer[MAXLINE];
	
	//Prepara cabecalho HTML
	sprintf(buffer, "<html><head><title>Submitted Form</title></head>");
	
	//Cria body
	strcat(buffer, "<body><h1>Received variables</h1><br><table>");

	strcat(buffer, "<tr><th>Variables</th><th>Values</th></tr>");
	
  	char * pch;
  	char temp[250];

	pch = strtok (linePost,"&=");

	while (pch != NULL)
	{
		sprintf(temp, "<tr><td>%s</td>", pch);
		strcat(buffer, temp);

		pch = strtok (NULL, "&=");

		sprintf(temp, "<td>%s</td></tr>", pch);
		strcat(buffer, temp);

		pch = strtok (NULL, "&=");

	}

	//Fecha body e html
	strcat(buffer, "</table></body></html>");

	sendHeader(connfd, req, res, "OK", "text/html");

	write(connfd, buffer, strlen(buffer));

	return 0;
}

int sendFile(request req, response res, int connfd){

	//Abre o arquivo
	FILE *clientFile = fopen(res.fileName, "r");

	//Verifica a permissao do arquivo
	if(!clientFile)
	{
		res.status = 403;
		return sendErrorMessage(res.status,req,  res, "Forbidden", connfd);
	}
	else
	{
		int size;
		char buffer[MAXLINE];

		sendHeader(connfd, req, res, "OK", identifyMimeType(res.fileName));

		while(!feof(clientFile)){
			size = fread(buffer, 1, MAXLINE, clientFile);
			write(connfd, buffer, size);
		}

		fclose(clientFile);
	}

}

int sendHeader(int connfd, request req, response res, char *msgStatus, char *mimeType){

	char bufferHeader[100];

	//Envia a primeira linha, com o Protocolo, o Status e a Msg do Status
	sprintf(bufferHeader, "%s %d %s\r\n", res.vProtocol, res.status,msgStatus);
	write(connfd, bufferHeader, strlen(bufferHeader));

	//printf("%s\n",bufferHeader );

	//Envia o nome do servidor
	sprintf(bufferHeader, "Server: %s\r\n", SERVERNAME);
	write(connfd, bufferHeader, strlen(bufferHeader));

	//printf("%s\n",bufferHeader );

	//Configura data e hora
	time_t now;
	char bufHour[128];

	now = time(NULL);
	strftime(bufHour, sizeof(bufHour), "%a, %d %b %Y %H:%M:%S GMT"	, gmtime(&now));

	//Envia a data e hora
	sprintf(bufferHeader, "Date: %s\r\n", bufHour);
	write(connfd, bufferHeader, strlen(bufferHeader));

	//printf("%s\n",bufferHeader );

	//Envia o tipo de conteudo
	sprintf(bufferHeader, "Content-Type: %s;\r\n", mimeType);
	write(connfd, bufferHeader, strlen(bufferHeader));

	//printf("%s\n",bufferHeader );

	struct stat infoFileRequest;

	if(	strcasecmp(req.method, "GET") == 0 && 
		res.status == 200 && stat(res.fileName, &infoFileRequest) == 0 )
	{
		// Envia content-length
		sprintf(bufferHeader, "Content-Length: %d\r\n", (int) infoFileRequest.st_size);
		write(connfd, bufferHeader, strlen(bufferHeader));

		//printf("%s\n",bufferHeader );
	}

	// Avisa que a conexao foi fechada
	sprintf(bufferHeader, "Connection: close\r\n");
	write(connfd, bufferHeader, strlen(bufferHeader));

	//printf("%s\n",bufferHeader );

	//Finaliza o cabecalho enviando uma linha em branco
	write(connfd, "\r\n", 2);

	return 0;
}

char *getLastLineRead(char *buffer) {

    int numLines = 0;
    int start = 0;
    int end = 0;
    int bufSize = strlen(buffer);
    
    int i = 0;
    int j = 0;

    for (i=0;i<bufSize;i++) {
        if (buffer[i]=='\n') {
            numLines++;
        }
    }

    int *vetPositionLine = (int*) malloc(numLines);
   
    for (i=0;i<bufSize;i++) {
        if (buffer[i]=='\n') {
            vetPositionLine[j] = i;
            j++;            
        }
    }

    start = vetPositionLine[numLines-3];
    end = vetPositionLine[numLines-1];

    char *line = (char*) malloc(end-start);
    strncpy(line,buffer+end,bufSize-end);

    return line;
}

char *identifyMimeType(char *name){

	char *ext = strrchr(name, '.');
	if (!ext) return NULL;
	if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0) return "text/html";
	if (strcmp(ext, ".png") == 0) return "image/png";
	if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) return "image/jpeg";
	if (strcmp(ext, ".gif") == 0) return "image/gif";	
	if (strcmp(ext, ".css") == 0) return "text/css";

	return NULL;
}

