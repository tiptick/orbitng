#include <pebble.h>
#include "main.h"

#include <configdialog.h>
#include <health.h>

static Layer *s_canvas_layer;
static int s_radius = 52;
#define HAND_MARGIN  0
static Time s_last_time;
struct tm *s_tick_time;
static bool isbtoff = false;
static bool isbattlow = false;
static TextLayer *s_time_layer;
static Window *s_main_window;


static void canvas_update_proc(Layer *this_layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(this_layer);
  GPoint center = GPoint(bounds.size.w / 2, (bounds.size.h / 2));
#ifdef PBL_SDK_2
  if (pers.backColor>10){
   graphics_context_set_fill_color(ctx, GColorWhite);
  }else{
    graphics_context_set_fill_color(ctx, GColorBlack);
  }
#elif PBL_SDK_3
  graphics_context_set_fill_color(ctx, GColorFromHEX(pers.backColor));
#endif
  graphics_fill_rect(ctx, GRect(0, 0, bounds.size.w, bounds.size.h), 0, GCornerNone);

  graphics_context_set_fill_color(ctx, GColorBlack);
#ifdef PBL_SDK_3
  graphics_context_set_stroke_color(ctx, pers.earthOrbitInvert ? GColorBlack : GColorWhite);
  graphics_context_set_stroke_width(ctx, 2);
  graphics_context_set_fill_color(ctx,GColorFromHEX(pers.earthColor));
  graphics_draw_circle(ctx, center, s_radius);
  graphics_fill_circle(ctx, center, s_radius-1);

#elif PBL_SDK_2
  graphics_context_set_stroke_color(ctx, pers.earthOrbitInvert ? GColorBlack : GColorWhite);
//graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_draw_circle(ctx, center, s_radius);
 // graphics_context_set_fill_color(ctx, GColorBlack);
 // graphics_draw_circle(ctx, center, s_radius-2);
#endif
 
  #if defined(PBL_HEALTH)
   GPoint health_hand = (GPoint) {
    .x = (int16_t)(sin_lookup(TRIG_MAX_ANGLE * health.steps / health.goal ) * (int32_t)(s_radius - HAND_MARGIN) / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(-cos_lookup(TRIG_MAX_ANGLE * health.steps / health.goal) * (int32_t)(s_radius - HAND_MARGIN) / TRIG_MAX_RATIO) + center.y,
  };
   graphics_context_set_fill_color(ctx, GColorBlack);
   graphics_fill_circle(ctx,health_hand, 4);
  
  
  
  #endif
  
  //float minute_angle = TRIG_MAX_ANGLE * s_last_time.minutes / 60;
  GPoint minute_hand = (GPoint) {
    .x = (int16_t)(sin_lookup(TRIG_MAX_ANGLE * s_last_time.minutes / 60) * (int32_t)(s_radius - HAND_MARGIN) / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(-cos_lookup(TRIG_MAX_ANGLE * s_last_time.minutes / 60) * (int32_t)(s_radius - HAND_MARGIN) / TRIG_MAX_RATIO) + center.y,
  };
 //graphics_draw_line(ctx, center, minute_hand);
#ifdef PBL_SDK_3
  graphics_context_set_fill_color(ctx, pers.moonOrbitInvert ? GColorBlack : GColorWhite);
   graphics_fill_circle(ctx,minute_hand, 17);
  graphics_context_set_fill_color(ctx , GColorFromHEX(pers.moonColor) );
  graphics_fill_circle(ctx,minute_hand, 15);
#elif PBL_SDK_2  
   graphics_context_set_fill_color(ctx, pers.moonOrbitInvert ? GColorBlack : GColorWhite);
   graphics_fill_circle(ctx,minute_hand, 17);
   if (pers.moonColor>10){
   graphics_context_set_fill_color(ctx, GColorWhite);
  }else{
    graphics_context_set_fill_color(ctx, GColorBlack);
  }
  
  graphics_fill_circle(ctx,minute_hand, 15);
#endif

  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  static char s_buffer[8];
  
   if (pers.moonFontInvert){
     graphics_context_set_text_color(ctx,GColorBlack);   
  }else{
     graphics_context_set_text_color(ctx,GColorWhite);   
  }
  
  strftime(s_buffer, sizeof(s_buffer), "%M" , tick_time);
  GRect sgr =  GRect(minute_hand.x-7, minute_hand.y-12,15,15);
  graphics_draw_text(ctx, 
                     s_buffer,  
                     fonts_get_system_font(FONT_KEY_GOTHIC_18), 
                     sgr,
                     GTextOverflowModeWordWrap,
                    GTextAlignmentCenter,
                   NULL
                    );
  
  GPoint second_hand = (GPoint) {
    .x = (int16_t)(sin_lookup(TRIG_MAX_ANGLE * s_last_time.second / 60) * (int32_t)(17) / TRIG_MAX_RATIO) + minute_hand.x,
    .y = (int16_t)(-cos_lookup(TRIG_MAX_ANGLE * s_last_time.second / 60) * (int32_t)(17) / TRIG_MAX_RATIO) + minute_hand.y,
  };
  graphics_context_set_fill_color(ctx, pers.moonOrbitInvert ? GColorBlack : GColorWhite);
  graphics_fill_circle(ctx,second_hand, 3);

  if (pers.earthFontInvert){
     graphics_context_set_text_color(ctx,GColorBlack);   
  }else{
     graphics_context_set_text_color(ctx,GColorWhite);   
  }
    
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H" : "%I" , tick_time);
  GRect sgr2 = GRect(0,bounds.size.h/2-30,bounds.size.w, bounds.size.h/2);
  graphics_draw_text(ctx, 
                     s_buffer,  
                     fonts_get_system_font(pers.earthFontBold ? FONT_KEY_BITHAM_42_BOLD :FONT_KEY_BITHAM_42_LIGHT), 
                     sgr2,
                     GTextOverflowModeWordWrap,
                    GTextAlignmentCenter,
                   NULL
                    );
 //+++DATE 
if(pers.date == true){
    strftime(s_buffer, sizeof(s_buffer),clock_is_24h_style() ?
                                           "%d %m" : "%m %d" , tick_time);
  GRect sgr3 =  GRect(center.x-28, PBL_IF_ROUND_ELSE(98,93),55,15);
  graphics_draw_text(ctx, 
                     s_buffer,  
                     fonts_get_system_font(FONT_KEY_GOTHIC_14), 
                     sgr3,
                     GTextOverflowModeWordWrap,
                    GTextAlignmentCenter,
                   NULL
                    );
  //---DATE
}
  
  
  
  
 #ifdef PBL_SDK_3 
  if (isbtoff) {
      GPoint bt_point = (GPoint){ .x=0,.y=0};
      graphics_context_set_fill_color(ctx, GColorBlue);
      graphics_fill_circle(ctx,bt_point, 20);
  }
  #endif
 #ifdef PBL_SDK_3 
  if (isbattlow) {
      GPoint batt_point = (GPoint){ .x=bounds.size.w,.y=0};
      graphics_context_set_fill_color(ctx, GColorRed);
      graphics_fill_circle(ctx,batt_point, 20);
  }
  #endif
}

void bt_handler(bool connected) {
  if (connected) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Phone is connected!");
   if (pers.bt_vib){
    vibes_short_pulse();
   }
    isbtoff = false;
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "Phone is not connected!");
    if (pers.bt_vib){
      vibes_short_pulse();
    }
    isbtoff = true;
  }
}

static void battery_handler(BatteryChargeState new_state) {
  if (new_state.charge_percent <= 10){
    isbattlow = true;
   }
  else  {
    isbattlow = false;   
  }
}



static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  
  s_last_time.hours = tick_time->tm_hour;
  s_last_time.hours -= (s_last_time.hours > 12) ? 12 : 0;
  s_last_time.minutes = tick_time->tm_min;
  s_last_time.second = tick_time->tm_sec;

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H" : "%I", tick_time);

  // Display this time on the TextLayer
 // text_layer_set_text(s_time_layer, s_buffer);
}
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
 update_time();
 layer_mark_dirty(s_canvas_layer);
}

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
      GRect((bounds.size.w/2) - 28, PBL_IF_ROUND_ELSE(60, 54), 60, 60));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  // Add it as a child layer to the Window's root layer
    GRect window_bounds = layer_get_bounds(window_layer);
    s_canvas_layer = layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
    
    layer_add_child(window_layer, s_canvas_layer);
    layer_set_update_proc(s_canvas_layer, canvas_update_proc);
   // layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
    battery_handler(battery_state_service_peek());

//BT  
  #ifdef PBL_SDK_2
  if (bluetooth_connection_service_peek()) {
  #elif PBL_SDK_3
  if (connection_service_peek_pebble_app_connection()) {
  #endif
     isbtoff = false;
  } else {
     isbtoff = true;
  }
//-BT  
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  #ifdef PBL_SDK_2
    bluetooth_connection_service_unsubscribe();
  #elif PBL_SDK_3
    connection_service_unsubscribe();
  #endif

}


static void init(){
  // Register with TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);

  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  #ifdef PBL_SDK_2
  bluetooth_connection_service_subscribe(bt_handler);
  #elif PBL_SDK_3
  connection_service_subscribe((ConnectionHandlers) {
  .pebble_app_connection_handler = bt_handler
});
#endif
   
    battery_state_service_subscribe(battery_handler);
  
  
//config handling
  
  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
   
//persistency handling
  
  if ( persist_exists(PERS_VERSION) == false ){
    //no data stored so fallback to coded defaults
    persist_write_int(PERS_VERSION,3); //we are version 2
    persist_write_bool(PERS_DATE,true);
    persist_write_bool(PERS_BT_VIB,true);
    persist_write_int(PERS_BACK_COLOR,0);
    persist_write_int(PERS_EARTH_COLOR,0);
    persist_write_int(PERS_MOON_COLOR,0);
    persist_write_bool(PERS_EARTH_FONT_BOLD,false);
    persist_write_bool(PERS_EARTH_FONT_INVERT,false);
    persist_write_bool(PERS_MOON_FONT_INVERT,false);
    persist_write_bool(PERS_EARTH_ORBIT_INVERT,false);
    persist_write_bool(PERS_MOON_ORBIT_INVERT,false);
    
    pers.date       = persist_read_bool(PERS_DATE);   
    pers.bt_vib     = persist_read_bool(PERS_BT_VIB); 
    pers.earthColor = persist_read_int(PERS_EARTH_COLOR);
    pers.backColor  = persist_read_int(PERS_BACK_COLOR);
    pers.moonColor  = persist_read_int(PERS_MOON_COLOR);
    pers.earthFontBold = persist_read_bool(PERS_EARTH_FONT_BOLD);
    pers.earthFontInvert = persist_read_bool(PERS_EARTH_FONT_INVERT);
    pers.moonFontInvert = persist_read_bool(PERS_MOON_FONT_INVERT);
    pers.moonOrbitInvert = persist_read_bool(PERS_MOON_ORBIT_INVERT);
    pers.earthOrbitInvert = persist_read_bool(PERS_EARTH_ORBIT_INVERT);
    
  }
  else{
   // data exists 
   // check if we have the correct version
   if (persist_read_int(PERS_VERSION)==1){
     pers.date = persist_read_bool(PERS_DATE);
     persist_write_bool(PERS_BT_VIB,false);    //false is default
     pers.bt_vib = persist_read_bool(PERS_BT_VIB); 
      persist_write_int(PERS_BACK_COLOR,0);
      persist_write_int(PERS_EARTH_COLOR,0);
      persist_write_int(PERS_MOON_COLOR,0);
      persist_write_bool(PERS_EARTH_FONT_BOLD,false);
      persist_write_bool(PERS_EARTH_FONT_INVERT,false);
      persist_write_bool(PERS_MOON_FONT_INVERT,false);
      persist_write_int(PERS_VERSION,3);
   }
   if (persist_read_int(PERS_VERSION)==2){
     pers.date = persist_read_bool(PERS_DATE);
     pers.bt_vib = persist_read_bool(PERS_BT_VIB);  
     persist_write_int(PERS_BACK_COLOR,0);
      persist_write_int(PERS_EARTH_COLOR,0);
      persist_write_int(PERS_MOON_COLOR,0);
      persist_write_bool(PERS_EARTH_FONT_BOLD,false);
      persist_write_bool(PERS_EARTH_FONT_INVERT,false);
      persist_write_bool(PERS_MOON_FONT_INVERT,false);
      persist_write_int(PERS_VERSION,3);
   }  
   if (persist_read_int(PERS_VERSION)==3){
      pers.date       = persist_read_bool(PERS_DATE);   
      pers.bt_vib     = persist_read_bool(PERS_BT_VIB); 
      pers.earthColor = persist_read_int(PERS_EARTH_COLOR);
      pers.backColor  = persist_read_int(PERS_BACK_COLOR);
      pers.moonColor  = persist_read_int(PERS_MOON_COLOR);
      pers.earthFontBold = persist_read_bool(PERS_EARTH_FONT_BOLD);
      pers.earthFontInvert = persist_read_bool(PERS_EARTH_FONT_INVERT);
      pers.moonFontInvert = persist_read_bool(PERS_MOON_FONT_INVERT);
      pers.moonOrbitInvert = persist_read_bool(PERS_MOON_ORBIT_INVERT);
      pers.earthOrbitInvert = persist_read_bool(PERS_EARTH_ORBIT_INVERT);
   } 
    
   else{
     //wrong version
     ;
   }   
    
  }  
  
#if defined(PBL_HEALTH)
  // Attempt to subscribe 
if(!health_service_events_subscribe(health_handler, NULL)) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Health not available!");
}
#else
APP_LOG(APP_LOG_LEVEL_ERROR, "Health not available!");
#endif
  
}

static void deinit(){
    // Destroy Window
  window_destroy(s_main_window);
}
  
  
  
int main(){
 health.goal = 5000;
  init();
  
  app_event_loop();
  deinit(); 
  return 0;
}

