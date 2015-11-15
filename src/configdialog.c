#include <pebble.h>
#include "configdialog.h"
#include "main.h"

void inbox_received_handler(DictionaryIterator *iter, void *context) {
   Tuple *date_t = dict_find(iter, PERS_DATE);
  
   if (date_t && date_t->value->int32 > 0){
      persist_write_bool(PERS_DATE,true);
      pers.date = true;
   }
   else{
      persist_write_bool(PERS_DATE,false);
      pers.date = false;
     //APP_LOG(APP_LOG_LEVEL_ERROR, "Could not parse config form phone");
   }
  Tuple *btvib_t = dict_find(iter, PERS_BT_VIB);  
    if (btvib_t && btvib_t->value->int32 > 0){
      persist_write_bool(PERS_BT_VIB,true);
      pers.bt_vib = true;
   }
   else{
      persist_write_bool(PERS_BT_VIB,false);
      pers.bt_vib = false;
     //APP_LOG(APP_LOG_LEVEL_ERROR, "Could not parse config form phone");
   }
   Tuple *backcolor_t = dict_find(iter, PERS_BACK_COLOR);  
    if (backcolor_t){
      persist_write_int(PERS_BACK_COLOR,backcolor_t->value->int32);
      pers.backColor = backcolor_t->value->int32;
   }
   else{
     APP_LOG(APP_LOG_LEVEL_ERROR, "Could not parse config form phone: BACK_COLOR");
     
   }
  
  Tuple *earthcolor_t = dict_find(iter, PERS_EARTH_COLOR);  
    if (earthcolor_t){
      persist_write_int(PERS_EARTH_COLOR,earthcolor_t->value->int32);
      pers.earthColor = earthcolor_t->value->int32;
   }
   else{
     APP_LOG(APP_LOG_LEVEL_ERROR, "Could not parse config form phone: earth_COLOR");
   }
  
  Tuple *mooncolor_t = dict_find(iter, PERS_MOON_COLOR);  
    if (mooncolor_t){
      persist_write_int(PERS_MOON_COLOR,mooncolor_t->value->int32);
      pers.moonColor = mooncolor_t->value->int32;
   }
   else{
     APP_LOG(APP_LOG_LEVEL_ERROR, "Could not parse config form phone: moon_COLOR");
   }
  
  Tuple *earthbold_t = dict_find(iter, PERS_EARTH_FONT_BOLD);  
  if (earthbold_t && earthbold_t->value->int32 > 0){
    persist_write_bool(PERS_EARTH_FONT_BOLD,true);
    pers.earthFontBold = true;
  }
  else{
    persist_write_bool(PERS_EARTH_FONT_BOLD,false);
    pers.earthFontBold = false;
   //APP_LOG(APP_LOG_LEVEL_ERROR, "Could not parse config form phone: moon_COLOR");
  }
  
  Tuple *earthinvert_t = dict_find(iter, PERS_EARTH_FONT_INVERT);  
  if (earthinvert_t && earthinvert_t->value->int32 > 0){
    persist_write_bool(PERS_EARTH_FONT_INVERT,true);
    pers.earthFontInvert = true;
  }
  else{
    persist_write_bool(PERS_EARTH_FONT_INVERT,false);
    pers.earthFontInvert = false;
   //APP_LOG(APP_LOG_LEVEL_ERROR, "Could not parse config form phone: moon_COLOR");
  }
  Tuple *mooninvert_t = dict_find(iter, PERS_MOON_FONT_INVERT);  
  if (mooninvert_t && mooninvert_t->value->int32 > 0){
    persist_write_bool(PERS_MOON_FONT_INVERT,true);
    pers.moonFontInvert = true;
  }
  else{
    persist_write_bool(PERS_MOON_FONT_INVERT,false);
    pers.moonFontInvert = false;
   //APP_LOG(APP_LOG_LEVEL_ERROR, "Could not parse config form phone: moon_COLOR");
  }
}

