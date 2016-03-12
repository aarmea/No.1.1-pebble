#include <pebble.h>

#include "fonts.h"
#include "settings.h"
#include "windows/main_window.h"

static void tick_handler(struct tm *time_now, TimeUnits changed) {
  main_window_update(time_now->tm_mday, time_now->tm_hour, time_now->tm_min,
      time_now->tm_sec);
}

static void init() {
  fonts_init();
  settings_init();
  main_window_push();

  time_t time_on_load = time(NULL);
  struct tm *tm_on_load = localtime(&time_on_load);
  tick_handler(tm_on_load, SECOND_UNIT);

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
