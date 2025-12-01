#include "keyboard_logic.h"
#include "light_controller.h"
#include "utils.h"

enum ProgramState cur_program_state = WORK;
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
void keyboard(int btn_id, int mode) {
    if(mode == 0){
        if(cur_program_state == WORK){
            if ( btn_id == 0){
                clear_lights();
                sprintf(write_buffer, "\nNo lights\n Enter 1-9 for watch color,\n 0 to shut down light,\n Press 10 to go to setup mode\n");
                write_string(write_buffer);
            }
            else if (btn_id == 10){
                cur_program_state = SETUP;
                sprintf(write_buffer, "\nSwitch to setup mode\n Enter 1-9 to choose with state you want to setup\n");
                write_string(write_buffer);
            }
            else if (btn_id > 0 && btn_id <= 9){
                set_light_power(color_model[(btn_id)].color, color_model[(btn_id)].power);
                sprintf(write_buffer, "\n %s Color %d power\nEnter 1-9 for watch color, 0 to shut down light, Press 10 to go to setup mode\n", ColorStrings[color_model[(btn_id)].color], color_model[(btn_id)].power);
                write_string(write_buffer);
            }
        }
        else if (cur_program_state == SETUP){
            if (btn_id > 0 && btn_id <= 9){
                cur_color_state = btn_id;
                cur_program_state = COLOR;
                sprintf(write_buffer, "\n %d state choosed\n Choose color: a - Green, b -Yellow, c - Red\n", cur_color_state);
                write_string(write_buffer);
            }
            else {
                sprintf(write_buffer, "\n Wrong number. Try again!\n", cur_color_state);
                write_string(write_buffer);            
            }

        }
        else if (cur_program_state == COLOR) {
            if (btn_id == 1){
                cur_color = GREEN;
            }
            else if (btn_id == 2){
                cur_color = YELLOW;
            } 
            else if (btn_id == 3){
                cur_color = RED;
            } else{
                sprintf(write_buffer, "\n Wrong symbol\n Choose color: a - Green, b -Yellow, c - Red\n", cur_color_state);
                write_string(write_buffer);
                return;
            }
            color_model[cur_color_state].color = cur_color;
            cur_program_state = POWER;

            sprintf(write_buffer, "\n %s color choosed\n Enter + for more light, And - for less light\nCur power: %d\n", ColorStrings[cur_color], color_model[cur_color_state].power);
            write_string(write_buffer);
        }
        else if (cur_program_state == POWER) {
            if (btn_id == 10){
                cur_program_state = WORK;
                sprintf(write_buffer, "\nSwitch to work mode\nEnter 1-9 for watch color, 0 to shut down light, Press 10 to go to setup mode\n");
                write_string(write_buffer);
            }
            else if(btn_id == 1){
                color_model[cur_color_state].power = min(color_model[cur_color_state].power + 10, 100);
                sprintf(write_buffer, "\n %d power\n Enter + for more light, And - for less light\n", color_model[cur_color_state].power);
                write_string(write_buffer);
            }
            else if(btn_id == 2){
                color_model[cur_color_state].power = max(color_model[cur_color_state].power - 10, 0);
                sprintf(write_buffer, "\n %d power\n Enter + for more light, And - for less light\n", color_model[cur_color_state].power);
                write_string(write_buffer);
            }
        }
    }
    else{
        sprintf(write_buffer, "\n %d key pressed\n", btn_id);
        write_string(write_buffer);
    }

}
