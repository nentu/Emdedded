#ifndef __Parser_H
#define __Parser_H

// State list

enum ParserState{
	CHNG_E,
	CHNG_S,
	CHNG_T,
	CHNG__,
	CRT_DIGIT,
	CRT_E,
	CRT_EXEC,
	CRT_N,
	CRT_W,
	CRT__,
	LINE,
	START
};


enum ParserRes{
	PRSR_CREATE_SCHEME,
	PRSR_CHANGE_SCHEME,
	PRSR_INTER,
	PRSR_PERIOD,
	PRSR_OK,
	UNKNOWN
};


extern char parser_buffer[50];

enum ParserRes parser_step(char input_symbol);

#endif
