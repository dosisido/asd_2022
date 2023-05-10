#ifndef __DATE_H
#define __DATE_H

#include <stdio.h>

typedef struct date_time{
    int year;
    unsigned int month;
    unsigned int day;
    unsigned int hour;
    unsigned int minute;
} Datetime;

Datetime DT_newFile(FILE* fp);
Datetime DT_newDate(int year, int month, int day);
Datetime DT_newDateFile(FILE* fp);
int DT_cmp(Datetime d1, Datetime d2);
int DT_cmpDate(Datetime d1, Datetime d2);
Datetime DT_setNull();
void DT_showDate(Datetime dt);


#endif