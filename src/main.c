#include <pebble.h>

#include "fonts.h"
#include "windows/main_window.h"

static void tick_handler(struct tm *time_now, TimeUnits changed) {
  main_window_update(time_now->tm_hour, time_now->tm_min, time_now->tm_sec);
}

static void init() {
  fonts_init();
  main_window_push();

  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void deinit() {
  fonts_deinit();

  tick_timer_service_unsubscribe();
}

int main() {
  init();
  app_event_loop();
  deinit();
}
