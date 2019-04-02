//
// Created by Adam Smith on 2/14/2018.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lcd.h"

int set_tod_from_secs(int time_of_day_sec, tod_t *tod){

    if ( time_of_day_sec > 0 && time_of_day_sec < 86400 ) { // checks if the seconds are acceptable

        tod->hours = time_of_day_sec / 3600;

        tod->minutes = ( time_of_day_sec - tod->hours * 3600 ) / 60;

        tod->seconds = ( time_of_day_sec - ( tod->hours * 3600 + tod->minutes * 60 ) );

        if ( tod->hours > 12 ) { // so it wont be in military time
            tod->hours -= 12;
            tod->ispm = 1;
        } else if ( tod->hours == 12 ) {
            tod->ispm = 1;
        } else if ( tod->hours == 0 ) {
            tod->hours = 12;
            tod->ispm = 0;
        } else {
            tod->ispm = 0;
        }

        return 0;
    }

    else {
        if ( time_of_day_sec == 0 ){ // 0 seconds is technically midnight
            tod->hours = 12;
            tod->ispm = 0;
            tod->seconds = 0;
            tod->minutes = 0;
            return 0;
        }
        return 1;
    }
}

int set_display_bits_from_tod(tod_t tod, int *display) {

    if ( tod.hours >= 0 && tod.hours <= 12 &&
         tod.minutes >= 0 && tod.minutes < 60 &&
         tod.seconds >= 0 && tod.seconds < 60 ){ // checks if the time is valid

        int bitbask[11] = { 0b0111111, 0b0000110, 0b1011011, 0b1001111, 0b1100110, 0b1101101,
                            0b1111101, 0b0000111, 0b1111111, 0b1101111, 0b0000000};

        *display = 0;

        int min_ones = tod.minutes % 10; // gets the ints for each spot on the clock
        int min_tens = tod.minutes / 10;
        int hr_ones = tod.hours % 10;
        int hr_tens = tod.hours / 10;

        if (tod.ispm == 0 ) { // sets if it ia am or pm
            *display |= 0b01;
            *display <<= 7;
        } else {
            *display |= 0b10;
            *display <<= 7;
        }

        if ( hr_tens !=0 ){
            *display |= bitbask[hr_tens];
        }
        *display <<= 7;
        *display |= bitbask[hr_ones];
        *display <<= 7;
        *display |= bitbask[min_tens];
        *display <<= 7;
        *display |= bitbask[min_ones];

        return 0;

    } else {
        *display = 0;
        return 1;
    }
}

int lcd_update(){

    tod_t* thetime = malloc( sizeof( tod_t ) );

    if ( !set_tod_from_secs( TIME_OF_DAY_SEC, thetime) ) { // enters if success == 0 indicating correct tod set

        if ( !set_display_bits_from_tod( *thetime, &LCD_DISPLAY_PORT) ){ // enters if success == 0 indicating correct display set

            free( thetime );
            return 0;
        } else {
            free( thetime );
            return 1; }
    } else {
        free( thetime );
        return 1; }
}
