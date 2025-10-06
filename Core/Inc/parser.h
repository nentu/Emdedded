#ifndef __Parser_H
#define __Parser_H

// State list

#define CHNG_E 0
#define CHNG_S 1
#define CHNG_T 2
#define CHNG__ 3
#define CRT_DIGIT 4
#define CRT_E 5
#define CRT_N 6
#define CRT_W 7
#define CRT__ 8
#define LINE 9
#define START 10


enum ParserRes{
	PRSR_CREATE_SCHEME,
	PRSR_CHANGE_SCHEME,
	PRSR_INTER,
	PRSR_OK,
	UNKNOWN
};


extern char parser_buffer[50];

enum ParserRes parser_step(char input_symbol);

#endif
