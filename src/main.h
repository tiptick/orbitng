#pragma once

#define PERS_VERSION 1
#define PERS_DATE 2
#define PERS_BT_VIB 3
#define PERS_BACK_COLOR 4
#define PERS_EARTH_COLOR 5 
#define PERS_MOON_COLOR 6
#define PERS_EARTH_FONT_BOLD 7
#define PERS_EARTH_FONT_INVERT 8
#define PERS_MOON_FONT_INVERT 9

typedef struct {
  int hours;
  int minutes;
  int second;
} Time;

typedef struct{
  bool date ;
  bool bt_vib;
  long backColor;
  long earthColor;
  long moonColor;
  bool earthFontBold;
  bool earthFontInvert;
  bool moonFontInvert;
  
} Pers;

Pers pers;