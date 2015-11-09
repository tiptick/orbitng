#include <pebble.h>
#include "main.h"

#include <configdialog.h>




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

  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, GRect(0, 0, bounds.size.w, bounds.size.h), 0, GCornerNone);

  graphics_context_set_fill_color(ctx, GColorBlack);
#ifdef PBL_SDK_3
  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_context_set_stroke_width(ctx, 2);
  graphics_draw_circle(ctx, center, s_radius);
#elif PBL_SDK_2
  graphics_context_set_stroke_color(ctx, GColorWhite);
//graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_draw_circle(ctx, center, s_radius);
 // graphics_context_set_fill_color(ctx, GColorBlack);
 // graphics_draw_circle(ctx, center, s_radius-2);
#endif
 
  //float minute_angle = TRIG_MAX_ANGLE * s_last_time.minutes / 60;
  GPoint minute_hand = (GPoint) {
    .x = (int16_t)(sin_lookup(TRIG_MAX_ANGLE * s_last_time.minutes / 60) * (int32_t)(s_radius - HAND_MARGIN) / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(-cos_lookup(TRIG_MAX_ANGLE * s_last_time.minutes / 60) * (int32_t)(s_radius - HAND_MARGIN) / TRIG_MAX_RATIO) + center.y,
  };
 //graphics_draw_line(ctx, center, minute_hand);
#ifdef PBL_SDK_3
  graphics_context_set_fill_color(ctx, GColorWhite);
#elif PBL_SDK_2  
  graphics_context_set_fill_color(ctx, GColorWhite);
#endif
  graphics_fill_circle(ctx,minute_hand, 17);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_circle(ctx,minute_hand, 15);
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  static char s_buffer[8];
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
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_circle(ctx,second_hand, 3);

  strftime(s_buffer, sizeof(s_buffer), "%H" , tick_time);
  GRect sgr2 = GRect(center.x - 30, PBL_IF_ROUND_ELSE(60, 54), 60, 60);
  graphics_draw_text(ctx, 
                     s_buffer,  
                     fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT), 
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
    vibes_short_pulse();
    isbtoff = false;
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "Phone is not connected!");
    vibes_short_pulse();
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
    persist_write_int(PERS_VERSION,1); //we are version 1
    persist_write_bool(PERS_DATE,true);
    
    pers.date = persist_read_bool(PERS_DATE);   
    
  }
  else{
   // data exists 
   // check if we have the correct version
   if (persist_read_int(PERS_VERSION)==1){
     pers.date = persist_read_bool(PERS_DATE);
      
   }
   else{
     //wrong version
     ;
   }   
    
  }  
  
  
  
}

static void deinit(){
    // Destroy Window
  window_destroy(s_main_window);
}


int main(){
  init();
  app_event_loop();
  deinit(); 
  return 0;
}

