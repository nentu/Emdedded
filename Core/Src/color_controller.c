#include "color_controller.h"
#include "light_controller.h"

enum ColorState cur_state = WORK;
enum BtnColor cur_color;
int cur_power;
int cur_color_state;


ColorMode color_model[10] = {
        {RED, 69},
		{GREEN, 10},
        {GREEN, 40},
        {GREEN, 100},
        {YELLOW, 10},
        {YELLOW, 40},
        {YELLOW, 100},
        {RED, 10},
        {RED, 40},
        {RED, 100},
};
char write_buffer[200];
const char *ColorStrings[] = {
    "GREEN",
    "YELLOW",
    "RED",
};
void change_color(char input_symbol) {
    if(cur_state == WORK){
        if ( input_symbol == '0'){
            clear_lights();
            sprintf(write_buffer, "\nNo lights\n");
            write_string(write_buffer);
        }
        else if (input_symbol == '\r'){
            cur_state = SETUP;
            sprintf(write_buffer, "\nSwitch to setup mode\n");
            write_string(write_buffer);
        }
        else if (input_symbol > '0' && input_symbol <= '9'){
            set_light_power(color_model[(input_symbol - '0')].color, color_model[(input_symbol-'0')].power);
            sprintf(write_buffer, "\n %d Color %d power\n", color_model[(input_symbol - '0')].color, color_model[(input_symbol-'0')].power);
            write_string(write_buffer);
        }
    }
    else if (cur_state == SETUP){
        cur_color_state = input_symbol-'0';
        cur_state = COLOR;
        sprintf(write_buffer, "\n %d state choosed\n", cur_color_state);
        write_string(write_buffer);
    }
    else if (cur_state == COLOR) {
        if (input_symbol  == 'a'){
            cur_color = GREEN;
        }
        else if (input_symbol == 'b'){
            cur_color = YELLOW;
        } 
        else if (input_symbol == 'c'){
            cur_color = RED;
        }
        color_model[cur_color_state].color = cur_color;
        cur_state = POWER;

        sprintf(write_buffer, "\n %s color choosed\n", ColorStrings[cur_color]);
        write_string(write_buffer);
    }
    else if (cur_state == POWER) {
        if (input_symbol == '\r'){
            cur_state = WORK;
        }
        else if( input_symbol == '+'){
            color_model[cur_color_state].power += 10;
            sprintf(write_buffer, "\n Power + 10\n");
            write_string(write_buffer);
        }
        else if( input_symbol == '-'){
            color_model[cur_color_state].power -= 10;
            sprintf(write_buffer, "\n Power - 10\n");
            write_string(write_buffer);
        }
    }

}
