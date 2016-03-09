#include "main_window.h"

#include "../fonts.h"
#include "../settings.h"

static Window *s_window = NULL;
static Layer *s_canvas = NULL;
static TextLayer *s_date_text = NULL;
static char s_date_buffer[3] = "";
static GPoint s_center;

static int s_hours, s_minutes, s_seconds;

static int32_t get_angle_for_second(int second) {
  return (second * 360) / 60;
}

static int32_t get_precise_angle_for_minute(int minute, int second) {
  return (minute * 360) / 60 + (second * 360) / 60 / 60;
}

static int32_t get_precise_angle_for_hour(int hour, int minute) {
  return (hour * 360) / 12 + (minute * 360) / 60 / 12;
}

static void get_circle_radius_endpoint(const GPoint* center, int radius, int32_t
    angle, /*OUT*/ GPoint* endpoint)
{
  endpoint->x = (int16_t)(sin_lookup(angle) * (int32_t)radius / TRIG_MAX_RATIO)
    + center->x;
  endpoint->y = (int16_t)(-cos_lookup(angle) * (int32_t)radius / TRIG_MAX_RATIO)
    + center->y;
}

static void draw_hand(GContext *ctx, int width, int radius, int center_radius,
    int32_t angle, int outer_circle_radius) {
  // Center highlight
  GPoint highlight_center = s_center;
  highlight_center.x += HIGHLIGHT_OFFSET_X;
  highlight_center.y += HIGHLIGHT_OFFSET_Y;
  graphics_context_set_antialiased(ctx, false);
  graphics_context_set_fill_color(ctx, HIGHLIGHT_COLOR);
  graphics_fill_circle(ctx, highlight_center, center_radius);

  // Rest of hand
  graphics_context_set_antialiased(ctx, true);
  graphics_context_set_stroke_color(ctx, HAND_COLOR);
  graphics_context_set_fill_color(ctx, HAND_COLOR);
  graphics_context_set_stroke_width(ctx, width);

  graphics_fill_circle(ctx, s_center, center_radius);

  GPoint hand_endpoint;
  get_circle_radius_endpoint(&s_center, radius - outer_circle_radius, angle,
      /*OUT*/ &hand_endpoint);
  graphics_draw_line(ctx, s_center, hand_endpoint);

  if (outer_circle_radius > 0) {
    GPoint outer_circle_center;
    get_circle_radius_endpoint(&s_center, radius, angle,
        /*OUT*/ &outer_circle_center);
    graphics_draw_circle(ctx, outer_circle_center, outer_circle_radius);
  }
}

static void layer_update_proc(Layer *layer, GContext *ctx) {
  // Minutes/seconds dots
  graphics_context_set_fill_color(ctx, SECONDS_DOTS_COLOR);
  for (int i = 0; i < 60; i++) {
    int angle = get_angle_for_second(i);
    GPoint dot_center;
    get_circle_radius_endpoint(&s_center, SECONDS_RADIUS,
        DEG_TO_TRIGANGLE(angle), /*OUT*/ &dot_center);
    graphics_fill_circle(ctx, dot_center, (i == 0) ? SECONDS_DOTS_BIG_RADIUS :
        SECONDS_DOTS_RADIUS);
  }

  if (settings_show_date()) {
    GPoint date_center = s_center;
    date_center.x += DATE_CIRCLE_X_OFFSET;
    date_center.y += DATE_CIRCLE_Y_OFFSET;
    graphics_context_set_fill_color(ctx, DATE_CIRCLE_FILL_COLOR);
    graphics_fill_circle(ctx, date_center, DATE_CIRCLE_RADIUS);
    graphics_draw_circle(ctx, date_center, DATE_CIRCLE_RADIUS);
  }

  int minute_angle = get_precise_angle_for_minute(s_minutes, s_seconds);
  draw_hand(ctx, MINUTES_WIDTH, MINUTES_RADIUS, MINUTES_CENTER_RADIUS,
      DEG_TO_TRIGANGLE(minute_angle), 0 /*outer_circle_radius*/);

  int hour_angle = get_precise_angle_for_hour(s_hours, s_minutes);
  draw_hand(ctx, HOURS_WIDTH, HOURS_RADIUS, HOURS_CENTER_RADIUS,
      DEG_TO_TRIGANGLE(hour_angle), 0 /*outer_circle_radius*/);

  int second_angle = get_angle_for_second(s_seconds);
  draw_hand(ctx, SECONDS_WIDTH, SECONDS_RADIUS, SECONDS_CENTER_RADIUS,
      DEG_TO_TRIGANGLE(second_angle), SECONDS_CIRCLE_RADIUS);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_canvas = layer_create(bounds);
  layer_set_update_proc(s_canvas, layer_update_proc);
  layer_add_child(window_layer, s_canvas);

  s_center = grect_center_point(&bounds);

  s_date_text = text_layer_create((GRect) {
    .origin = (GPoint) {
      .x = s_center.x + DATE_X_OFFSET,
      .y = s_center.y + DATE_Y_OFFSET
    },
    .size = (GSize) {
      .w = DATE_WIDTH,
      .h = DATE_FONT_HEIGHT
    }
  });
  text_layer_set_font(s_date_text, DATE_FONT);
  text_layer_set_background_color(s_date_text, GColorClear);
  text_layer_set_text_alignment(s_date_text, GTextAlignmentCenter);
  text_layer_set_text(s_date_text, s_date_buffer);

  layer_add_child(s_canvas, (Layer *) s_date_text);
}

static void window_unload(Window *window) {
  text_layer_destroy(s_date_text);
  layer_destroy(s_canvas);
  window_destroy(s_window);
}

void main_window_push() {
  s_window = window_create();
  window_set_background_color(s_window, BG_COLOR);
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(s_window, true);
}

void main_window_update(int day, int hours, int minutes, int seconds) {
  s_hours = hours;
  s_minutes = minutes;
  s_seconds = seconds;
  layer_mark_dirty(s_canvas);

  if (settings_show_date())
    snprintf(s_date_buffer, sizeof(s_date_buffer), "%d", day);
  else
    s_date_buffer[0] = '\0';
  layer_mark_dirty((Layer *) s_date_text);
}
