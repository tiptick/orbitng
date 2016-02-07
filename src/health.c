#include <pebble.h>
#include "health.h"
#include "main.h"

void health_handler(HealthEventType event, void *context) {
  // Which type of event occured?
  switch(event) {
    case HealthEventSignificantUpdate:
      APP_LOG(APP_LOG_LEVEL_INFO, 
              "New HealthService HealthEventSignificantUpdate event");
     // break;
    //fallthrough
    case HealthEventMovementUpdate:
      APP_LOG(APP_LOG_LEVEL_INFO, 
              "New HealthService HealthEventMovementUpdate event");
      HealthMetric metric = HealthMetricStepCount;
      time_t start = time_start_of_today();
      time_t end = time(NULL);
      
      // Check the metric has data available for today
      HealthServiceAccessibilityMask mask = 
        health_service_metric_accessible(metric, start, end);
      
      if(mask & HealthServiceAccessibilityMaskAvailable) {
        // Data is available!
        APP_LOG(APP_LOG_LEVEL_INFO, "Steps today: %d", 
                (int)health_service_sum_today(metric));
        health.steps = (int)health_service_sum_today(metric);
      } else {
        // No data recorded yet today
        APP_LOG(APP_LOG_LEVEL_ERROR, "Data unavailable!");
      }
    
      break;
    case HealthEventSleepUpdate:
      APP_LOG(APP_LOG_LEVEL_INFO, 
              "New HealthService HealthEventSleepUpdate event");
      break;
  }
}  