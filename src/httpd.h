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

void readSocket(char buffer[], int tam, int connfd);

request parseRequest(char buffer[]);

