/* 
* MAC0448/5910 - Programação para Redes de Computadores - EP1 @ 2013.2
* Alunos: Bruno Nunes Leal Faria - nUSP: 8765551
*         Elaine Naomi Watanabe - nUSP: 8766722
*
* FILE: httpd.h
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>

// Tamanho máximo de caracteres de entrada
#define MAXLINE 4096

// Pasta raiz do servidor 
#define WWW_ROOT "www"

// Nome do servidor
#define SERVERNAME "Mano Server 1.0"

/**
* Estrutura da requisição. 
* @method: Tipo de requisicao (GET/POST)
* @uri: Endereco para arquivo no servidor
* @vProtocol: Versao do protocolo HTTP
*/
typedef struct {
	char *method;
	char *uri;
	char *vProtocol;
} request;

/**
* Estrutura da resposta. 
* @status: id do status de retorno
* @vProtocol: Versao do protocolo HTTP
* @fileName: Nome do arquivo em disco da requisicao
*/
typedef struct {
	int status;
	char *vProtocol;
	char *fileName;
} response;

/**
* Parse do request do cliente
* @buffer Recebe dados do socket
* Monta estrutura do request de acordo com a requisicao do usuario
* 
*/
request parseRequest(char buffer[]);

/**
* Envia resposta de acordo com o request e responses ja montados
* @req estrutura do request do usuario
* @res estrutura da resposta
* @connfd socket
* @linePost somente para POST. Parametros
* 
*/
int sendResponse(request req, response res, int connfd, char *linePost);

/*
* Envia resposta com headers e html pedido
* @req estrutura do requesst
* @res estrutura do response
* @connfd socket
*/
int sendFile(request req, response res, int connfd);

/*
* Envia header
* @req estrutura do requesst
* @res estrutura do response
* @msgStatus status do retornoß
* @mimeType tipo de arquivo
* @connfd socket
*/
int sendHeader(int connfd, request req, response res, char *msgStatus, char *mimeType);

/*
* Identifica tipo de arquivo pela extensao
* @name nome do arquivo
*/
char *identifyMimeType(char *name);

/*
* Envia html de error
* @status status da requisicao
* @res estrutura do request
* @req estrutura do response
* @message mensagem de erro
* @connfd socket
*/
int sendErrorMessage(int status, request req, response res, char *message, int connfd);

/*
* Envia POST
* @req estrutura do requesst
* @res estrutura do response
* @linePost Parametros do Post
* @connfd socket
*/
int sendPostMessage(request req, response res, int connfd, char *linePost);

/*
* Pega ultima linha do request
*/
char *getLastLineRead(char *buffer);
