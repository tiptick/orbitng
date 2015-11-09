#include <pebble.h>
#include "configdialog.h"
#include "main.h"

void inbox_received_handler(DictionaryIterator *iter, void *context) {
   Tuple *date_t = dict_find(iter, PERS_DATE);
   
   if (date_t && date_t->value->int32 > 0){
      persist_write_bool(PERS_DATE,true);
    
   }
   else{
      persist_write_bool(PERS_DATE,false);
     //APP_LOG(APP_LOG_LEVEL_ERROR, "Could not parse config form phone");
   }
    
}

