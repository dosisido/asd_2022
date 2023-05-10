#include<stdio.h>
#include<stdlib.h>
#include "datetime.h"

// struct{
//     int year;
//     unsigned int month;
//     unsigned int day;
//     unsigned int hour;
//     unsigned int minute;
// };

Datetime DT_newFile(FILE* fp){
    Datetime dt;
    fscanf(fp, "%d/%u/%u %u:%u", &dt.year, &dt.month, &dt.day, &dt.hour, &dt.minute);
    return dt;
}

Datetime DT_newDateFile(FILE* fp){
    Datetime dt;
    fscanf(fp, "%d/%u/%u", &dt.year, &dt.month, &dt.day);
    dt.hour = 0;
    dt.minute = 0;
    return dt;
}

Datetime DT_newDate(int year, int month, int day){
    Datetime dt;
    dt.year = year;
    dt.month = month;
    dt.day = day;
    dt.hour = 0;
    dt.minute = 0;
    return dt;
}

int DT_cmp(Datetime d1, Datetime d2){
    if(d1.year > d2.year) return 1;
    if(d1.year < d2.year) return -1;
    if(d1.month > d2.month) return 1;
    if(d1.month < d2.month) return -1;
    if(d1.day > d2.day) return 1;
    if(d1.day < d2.day) return -1;
    if(d1.hour > d2.hour) return 1;
    if(d1.hour < d2.hour) return -1;
    if(d1.minute > d2.minute) return 1;
    if(d1.minute < d2.minute) return -1;
    return 0;
}

int DT_cmpDate(Datetime d1, Datetime d2){
    if(d1.year > d2.year) return 1;
    if(d1.year < d2.year) return -1;
    if(d1.month > d2.month) return 1;
    if(d1.month < d2.month) return -1;
    if(d1.day > d2.day) return 1;
    if(d1.day < d2.day) return -1;
    return 0;
}

Datetime DT_setNull(){
    Datetime dt;
    dt.year = 0;
    dt.month = 0;
    dt.day = 0;
    dt.hour = 0;
    dt.minute = 0;
    return dt;
}

void DT_showDate(Datetime dt){
    printf("%d/%u/%u", dt.year, dt.month, dt.day);
}