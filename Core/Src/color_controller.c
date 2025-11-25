#include "color_controller.h"
#include "light_controller.h"

enum ColorState cur_state = WORK;


void change_color(input_symbol) {
    if(cur_state == WORK){
        switch (input_symbol)
        {
        case '1':
            set_light_power(color_mode[1].color, color_mode[1].power)
            break;
        case '2':
            set_light_power(color_mode[2].color, color_mode[2].power)
            break;
        case '3':
            set_light_power(color_mode[3].color, color_mode[3].power)
            break;
        case '4':
            set_light_power(color_mode[4].color, color_mode[4].power)
            break;
        case '5':
            set_light_power(color_mode[5].color, color_mode[5].power)
            break;
        case '6':
            set_light_power(color_mode[6].color, color_mode[6].power)
            break;
        case '7':
            set_light_power(color_mode[7].color, color_mode[7].power)
            break;
        case '8':
            set_light_power(color_mode[8].color, color_mode[8].power)
            break;
        case '9':
            set_light_power(color_mode[9].color, color_mode[9].power)
            break;
        case '0':
            clear_lights()
            break;
        case '\n':
            cur_state = SETUP;
            break;
        }
    }
    if (cur_state == SETUP){

    }

}