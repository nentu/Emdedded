#include "color_controller.h"
#include "light_controller.h"


 #define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

 #define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })


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
            sprintf(write_buffer, "\nNo lights\n Enter 1-9 for watch color, 0 to shut down light, Press Enter to go to setup mode\n");
            write_string(write_buffer);
        }
        else if (input_symbol == '\r'){
            cur_state = SETUP;
            sprintf(write_buffer, "\nSwitch to setup mode\n Enter 1-9 to choose with state you want to setup\n");
            write_string(write_buffer);
        }
        else if (input_symbol > '0' && input_symbol <= '9'){
            set_light_power(color_model[(input_symbol - '0')].color, color_model[(input_symbol-'0')].power);
            sprintf(write_buffer, "\n %s Color %d power\nEnter 1-9 for watch color, 0 to shut down light, Press Enter to go to setup mode\n", ColorStrings[color_model[(input_symbol - '0')].color], color_model[(input_symbol-'0')].power);
            write_string(write_buffer);
        }
    }
    else if (cur_state == SETUP){
        if (input_symbol > '0' && input_symbol <= '9'){
            cur_color_state = input_symbol-'0';
            cur_state = COLOR;
            sprintf(write_buffer, "\n %d state choosed\n Choose color: a - Green, b -Yellow, c - Red\n", cur_color_state);
            write_string(write_buffer);
        }
        else {
            sprintf(write_buffer, "\n Wrong number. Try again!\n", cur_color_state);
            write_string(write_buffer);            
        }

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
        } else{
            sprintf(write_buffer, "\n Wrong symbol\n Choose color: a - Green, b -Yellow, c - Red\n", cur_color_state);
            write_string(write_buffer);
            return;
        }
        color_model[cur_color_state].color = cur_color;
        cur_state = POWER;

        sprintf(write_buffer, "\n %s color choosed\n Enter + for more light, And - for less light\nCur power: %d\n", ColorStrings[cur_color], color_model[cur_color_state].power);
        write_string(write_buffer);
    }
    else if (cur_state == POWER) {
        if (input_symbol == '\r'){
            cur_state = WORK;
            sprintf(write_buffer, "\nSwitch to work mode\nEnter 1-9 for watch color, 0 to shut down light, Press Enter to go to setup mode\n");
            write_string(write_buffer);
        }
        else if( input_symbol == '+'){
            color_model[cur_color_state].power = min(color_model[cur_color_state].power + 10, 100);
            sprintf(write_buffer, "\n %d power\n Enter + for more light, And - for less light\n", color_model[cur_color_state].power);
            write_string(write_buffer);
        }
        else if( input_symbol == '-'){
            color_model[cur_color_state].power = max(color_model[cur_color_state].power - 10, 0);
            sprintf(write_buffer, "\n %d power\n Enter + for more light, And - for less light\n", color_model[cur_color_state].power);
            write_string(write_buffer);
        }
    }

}
