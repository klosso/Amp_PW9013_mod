/*
 * =====================================================================================
 *
 *       Filename:  tea6321.cpp
 *
 *    Description:  class definition for TEA6321 audio processor
 *
 *        Version:  1.0
 *        Created:  11.04.2022 00:10:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author: Sebastian Mikulski 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "tea6321.hpp"
#include <stdlib.h>

TEA6321::TEA6321(void* _opq):drv(_opq), addr(TEA6321_I2C_ADDR)
{}

void TEA6321::init()
{
    set_vol(0);
    set_input(0);
    loudness(true);
    mute(false);
    write_drv(FFR_SAD,0x2F);
    write_drv(FFL_SAD,0x2F);
    write_drv(FRR_SAD,0x2F);
    write_drv(FRL_SAD,0x2F);
}


void TEA6321::set_vol(int8_t vol)
{
    if( ( vol <= 20 ) && ( vol >=-31 ) )
    {
        vol_wrd = (vol_wrd & 0x2F)  | ( vol+43 );
        write_drv( VOL_SAD, vol_wrd );
    }
}

void TEA6321::set_input(uint8_t inp)
{
    if(inp < 4)
        write_drv(SWCH_SAD, (4 | (3-inp))); 
}

void TEA6321::loudness(bool on)
{
    vol_wrd = on ? vol_wrd & ~LOFF_BIT : vol_wrd | LOFF_BIT;
    write_drv( VOL_SAD, vol_wrd);
}
void TEA6321::mute(const bool mute)
{
    vol_wrd = mute ? vol_wrd | ZCM_BIT : vol_wrd & ~ZCM_BIT;
    write_drv( VOL_SAD, vol_wrd);
}

void TEA6321::set_trbl(int8_t val)
{
    uint8_t data =0;  //external Eq
    if (val >=12)
        data = 25;  //+12.6dB
    else if (val >= 10)
        data = 24;  //+10.5dB
    else if (val >= 9)
        data = 23;  //+9dB
    else if (val >= 7)
        data = 22;  //+7.5dB
    else if (val >= 6)
        data = 21;  //+6dB
    else if (val >= 4)
        data = 20;  //4.5dB
    else if (val >= 3)
        data = 19;  //+3dB
    else if (val >= 1)
        data = 18;  //1.5dB
    else if (val >= 0)
        data = 17;  //0dB
    else if (val >= -1)
        data = 15;  //-1.5dB
    else if (val >= -3)
        data = 14;  //-3dB
    else if (val >= -4)
        data = 13;  //-4.5dB
    else if (val >= -6)
        data = 12;  //-6dB
    else if (val >= -7)
        data = 11;  //-7.5dB
    else if (val >= -9)
        data = 10;  //-9dB
    else if (val >= -10)
        data = 9;  //-10.5dB
    else if (val >= -12)
        data = 8;  //-12dB

    write_drv( BASS_SAD, data );

}
void TEA6321::set_bass(int8_t val)
{
    uint8_t data =0;  //external Eq
    if(val >17)
        data = 0b11011;  //+18dB
    else if (val >=16)
        data = 0b11010;  //+16.2dB
    else if (val >=14)
        data = 0b11001;  //+14.4dB
    else if (val >=12)
        data =24;  //+12.6dB
    else if (val >= 10)
        data = 23;  //+10.8dB
    else if (val >= 9)
        data = 22;  //+9dB
    else if (val >= 7)
        data = 21;  //+7.2dB
    else if (val >= 5)
        data = 20;  //+5.4dB
    else if (val >= 3)
        data = 19;  //3.6dB
    else if (val >= 1)
        data = 18;  //+1.8dB
    else if (val >= 0)
        data = 17;  //0dB
    else if (val >= -1)
        data = 15;  //-1.8dB
    else if (val >= -3)
        data = 14;  //-3.6dB
    else if (val >= -5)
        data = 13;  //-5.4dB
    else if (val >= -7)
        data = 12;  //-7.2dB
    else if (val >= -9)
        data = 11;  //-9dB
    else if (val >= -10)
        data = 10;  //-10.8dB
    else if (val >= -12)
        data = 9;  //-12.6dB
    else if (val >= -14)
        data = 8;  //-14.4dB
    else if (val >= -16)
        data = 7;  //-16.2dB
    else if (val >= -18)
        data = 6;  //-18dB

    write_drv( BASS_SAD, data );
}

