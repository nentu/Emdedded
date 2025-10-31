#include <stdbool.h>

#include "parser.h"

bool is_color(char input_symbol){
	return (input_symbol == 'g' || input_symbol == 'r' || input_symbol == 'y' || input_symbol == 'n');
}

int append_char(char *str, char ch) {
    int len = strlen(str);
    str[len] = ch;
    str[len + 1] = '\0';
    return 0;
}


int digit_count = 0;
enum ParserState cur_state = START;
char parser_buffer[50] = {0};

void clear_buffer(){
	memset(parser_buffer, 0, sizeof(parser_buffer));
}

enum ParserRes failed(){
	clear_buffer();
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
	    	append_char(parser_buffer, input_symbol);
	    } else {
	        return failed();
	    }
	} else if (cur_state == CRT_DIGIT) {
	    if (input_symbol == '\r') {
	    	digit_count = 0;
	        cur_state = CRT_EXEC;
	        return PRSR_CREATE_SCHEME;
	    } else if (is_color(input_symbol)) {
	    	append_char(parser_buffer, input_symbol);
	        digit_count++;
	    } else {
	        return failed();
	    }
	} else if (cur_state == CRT_EXEC) {
		char string[] = "\nStart CTR_EXEC\n";
        HAL_UART_Transmit( &huart6, (uint8_t *) string, strlen( string ), 100 );
	    if (input_symbol >= '1' && input_symbol <= '3') {
	        cur_state = START;
	    	append_char(parser_buffer, input_symbol);
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
	    	append_char(parser_buffer, input_symbol);
	        return PRSR_CHANGE_SCHEME;
	    } else if (input_symbol >= 'a' && input_symbol <= 'z') {
	        cur_state = LINE;
	    	append_char(parser_buffer, input_symbol);
	    } else {
	        return failed();
	    }
	} else if (cur_state == LINE) {
	    if (input_symbol == '\r') {
	        cur_state = START;
	    	return PRSR_INTER;
	    } else if ((input_symbol >= 'a' && input_symbol <= 'z') || (input_symbol == ' ')) {
	        cur_state = LINE;  // stay in same state
	    	append_char(parser_buffer, input_symbol);
	    } else {
	        return failed();
	    }
	} else {
	    return failed();
	}

	if (digit_count == 7){
		digit_count = 0;
        cur_state = CRT_EXEC;
		return PRSR_CREATE_SCHEME;
	}

	return PRSR_OK;
}
