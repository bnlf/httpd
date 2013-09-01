/* 
* MAC0448/5910 - Programação para Redes de Computadores - EP1 @ 2013.2
* Alunos: Bruno Nunes Leal Faria - nUSP: 8765551
*         Elaine Naomi Watanabe - nUSP: 8766722
*
* FILE: httpd.h
*/

<<<<<<< HEAD
=======

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

/*
 * Tipos de requisição HTTP: GET e POST
 */
typedef enum {
	GET,
	POST,
	REQ_INVALID
} req_type;

/*
* Versões do protocolo HTTP
*/
typedef enum {
	HTTP10,
	HTTP11,
	PROT_INVALID
} req_proto;


>>>>>>> d9d8ff380eed4587664049e5e4efdd5e6dbc964a
/**
* Estrutura da requisição. 
* @type req_type
* @filePath Caminho para o arquivo no servidor
* @protocol req_proto
*/
typedef struct {
	char *type;
	char filePath[MAXLINE + 1];
<<<<<<< HEAD
	char *protocol;
} request;
=======
	req_proto protocol;
} request;

>>>>>>> d9d8ff380eed4587664049e5e4efdd5e6dbc964a
