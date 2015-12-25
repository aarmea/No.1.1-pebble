#include "fonts.h"

GFont FONT_OSP_DIN_16;

void fonts_init() {
  FONT_OSP_DIN_16 =
    fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_OSP_DIN_16));
}

void fonts_deinit() {
  fonts_unload_custom_font(FONT_OSP_DIN_16);
}
