#pragma once

#define PERS_VERSION 1
#define PERS_DATE 2
#define PERS_BT_VIB 3

typedef struct {
  int hours;
  int minutes;
  int second;
} Time;

typedef struct{
  bool date ;
  bool bt_vib;
  
} Pers;

Pers pers;