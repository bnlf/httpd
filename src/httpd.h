/* 
* MAC0448/5910 - Programação para Redes de Computadores - EP1 @ 2013.2
* Alunos: Bruno Nunes Leal Faria - nUSP: 8765551
*         Elaine Naomi Watanabe - nUSP: 8766722
*
* FILE: httpd.h
*/

/**
* Estrutura da requisição. 
* @type req_type
* @filePath Caminho para o arquivo no servidor
* @protocol req_proto
*/
typedef struct {
	char *type;
	char filePath[MAXLINE + 1];
	char *protocol;
} request;