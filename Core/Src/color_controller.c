#include "color_controller.h"
#include "light_controller.h"

enum ColorState cur_state = WORK;
enum BtnColor cur_color;
int cur_power;
int cur_color_state;

void change_color(input_symbol) {
    if(cur_state == WORK){
        if ( input_symbol == '0'){
            clear_lights();
        }
        else if (input_symbol == '\n'){
            cur_state = SETUP;
        }
        else if (input_symbol > '0' && input_symbol <= '9'){
            set_light_power(color_model[(input_symbol - '0')].color, color_model[(input_symbol-'0')].power);
        }
    }
    else if (cur_state == SETUP){
        cur_color_state = input_symbol-'0';
        cur_state = COLOR;
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
    }
    else if (cur_state == POWER) {
        if (input_symbol == '\n'){
            cur_state = WORK;
        }
        else if( input_symbol == '+'){
            color_model[cur_color_state].power += 10;
        }
        else if( input_symbol == '-'){
            color_model[cur_color_state].power -= 10;
        }
    }

}