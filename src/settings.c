#include <pebble.h>

#include "settings.h"

// Settings definitions
// These must be kept in sync with the onSubmit of settings.js and the appKeys
// section of appinfo.json
#define SETTINGS_SHOW_DATE 0
static bool s_show_date = false;
bool settings_show_date() { return s_show_date; }
// End settings definitions

#define SETTINGS_INBOX_SIZE 128
#define SETTINGS_OUTBOX_SIZE 128
#define SETTINGS_BUFFER_SIZE 32


static AppSync s_sync;
static uint8_t s_sync_buffer[SETTINGS_BUFFER_SIZE];

static void settings_load_from_storage() {
  s_show_date = persist_read_bool(SETTINGS_SHOW_DATE);
}

static void settings_sync_changed_handler(const uint32_t key, const Tuple
    *new_tuple, const Tuple *old_tuple, void *context) {
  switch(key) {
    case SETTINGS_SHOW_DATE:
      s_show_date = (bool) new_tuple->value->int32;
      persist_write_bool(SETTINGS_SHOW_DATE, s_show_date);
      break;
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Invalid settings key %lu", key);
      break;
  }
}

static void settings_sync_error_handler(DictionaryResult dict_error,
    AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR,
      "Settings sync error. DictionaryResult: %lu; AppMessageResult: %lu",
      (uint32_t) dict_error, (uint32_t) app_message_error);
}

void settings_init() {
  app_message_open(SETTINGS_INBOX_SIZE, SETTINGS_INBOX_SIZE);

  settings_load_from_storage();

  Tuplet initial_values[] = {
    TupletInteger(SETTINGS_SHOW_DATE, s_show_date)
  };

  app_sync_init(&s_sync, s_sync_buffer, SETTINGS_BUFFER_SIZE, initial_values,
      ARRAY_LENGTH(initial_values), settings_sync_changed_handler,
      settings_sync_error_handler, NULL);
}
