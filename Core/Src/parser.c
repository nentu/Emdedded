#include <stdbool.h>

#include "parser.h"


bool is_color(char input_symbol){
	return (input_symbol == 'g' || input_symbol == 'r' || input_symbol == 'y' || input_symbol == 'n');
}


int digit_count = 0;
enum ParserState cur_state = START;
char parser_buffer[50];

enum ParserRes failed(){
	memset(parser_buffer, 0, sizeof(parser_buffer));
	digit_count = 0;
	cur_state = START;
	return UNKNOWN;
}

enum ParserRes parser_step(char input_symbol){
	if (cur_state == START) {
	    if (input_symbol == 'n') {
	        cur_state = CRT_N;
	    } else if (input_symbol == 's') {
	        cur_state = CHNG_S;
	    } else {
	        return failed();
	    }
	} else if (cur_state == CRT_N) {
	    if (input_symbol == 'e') {
	        cur_state = CRT_E;
	    } else {
	        return failed();
	    }
	} else if (cur_state == CRT_E) {
	    if (input_symbol == 'w') {
	        cur_state = CRT_W;
	    } else {
	        return failed();
	    }
	} else if (cur_state == CRT_W) {
	    if (input_symbol == ' ') {
	        cur_state = CRT__;
	    } else {
	        return failed();
	    }
	} else if (cur_state == CRT__) {
	    if (is_color(input_symbol)) {
	        cur_state = CRT_DIGIT;
	    } else {
	        return failed();
	    }
	} else if (cur_state == CRT_DIGIT) {
	    if (
	    		((input_symbol == '\n') || is_color(input_symbol))
				&& (digit_count == 8)
			) {
	        cur_state = CRT_EXEC;
	        return PRSR_CREATE_SCHEME;
	    } else if (is_color(input_symbol)) {
	        digit_count++;
	    } else {
	        return failed();
	    }
	} else if (cur_state == CRT_EXEC) {
	    if (input_symbol >= '0' && input_symbol <= '2') {
	        cur_state = START;
	        return PRSR_PERIOD;
	    } else {
	        return failed();
	    }
	} else if (cur_state == CHNG_S) {
	    if (input_symbol == 'e') {
	        cur_state = CHNG_E;
	    } else {
	        return failed();
	    }
	} else if (cur_state == CHNG_E) {
	    if (input_symbol == 't') {
	        cur_state = CHNG_T;
	    } else {
	        return failed();
	    }
	} else if (cur_state == CHNG_T) {
	    if (input_symbol == ' ') {
	        cur_state = CHNG__;
	    } else {
	        return failed();
	    }
	} else if (cur_state == CHNG__) {
	    if (input_symbol >= '0' && input_symbol <= '7') {
	        cur_state = START;
	        return PRSR_CHANGE_SCHEME;
	    } else if (input_symbol >= 'a' && input_symbol <= 'z') {
	        cur_state = LINE;
	    } else {
	        return failed();
	    }
	} else if (cur_state == LINE) {
	    if (input_symbol == '\n') {
	        cur_state = START;
	    	return PRSR_INTER;
	    } else if ((input_symbol >= 'a' && input_symbol <= 'z') || (input_symbol == ' ')) {
	        cur_state = LINE;  // stay in same state
	    } else {
	        return failed();
	    }
	} else {
	    return failed();
	}

	return PRSR_OK;
}
