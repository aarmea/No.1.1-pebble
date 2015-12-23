#pragma once

#include <pebble.h>

#define BG_COLOR GColorWhite
#define HAND_COLOR GColorBlack

#define HIGHLIGHT_OFFSET_X (-1)
#define HIGHLIGHT_OFFSET_Y (-1)
#define HIGHLIGHT_COLOR GColorDarkGray

#define MINUTES_WIDTH 6
#define MINUTES_RADIUS 73
#define MINUTES_CENTER_RADIUS 0

#define HOURS_WIDTH 8
#define HOURS_RADIUS 36
#define HOURS_CENTER_RADIUS 5

#define SECONDS_WIDTH 2
#define SECONDS_RADIUS 82
#define SECONDS_CENTER_RADIUS 1
#define SECONDS_CIRCLE_RADIUS 4

#define SECONDS_DOTS_RADIUS 1
#define SECONDS_DOTS_BIG_RADIUS 2
#define SECONDS_DOTS_COLOR GColorBlack

// TODO: Make this a user-accessible configuration option
#define SHOW_DATE true
#define DATE_FONT FONT_OSP_DIN_24
#define DATE_FONT_HEIGHT 24
#define DATE_WIDTH 24
#define DATE_X_OFFSET 47
#define DATE_Y_OFFSET (-DATE_FONT_HEIGHT / 2 - 3)
