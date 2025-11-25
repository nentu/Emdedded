#ifndef _COLOR_CONTROLLER_H
#define _COLOR_CONTROLLER_H
enum ColorState {
  SETUP,
  WORK,
  STATE,
  COLOR,
  POWER
};

enum ColorState change_color(char input_symbol);

#endif