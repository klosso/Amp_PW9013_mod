#pragma once
#include <stdint.h>

#define TEA6321_I2C_ADDR 0x80

enum tone_t{ BASS=0, MIDD, TRBL };
enum bass_fo_t{ _60Hz=0, _80Hz, _100Hz, _120Hz };
enum midd_fo_t{ _500Hz=0, _1000Hz, _1500Hz, _2500Hz };
enum trbl_fo_t{ _7500Hz=0, _10000Hz, _12500Hz, _15000Hz };


#define VOL_SAD 0x00
#define FFR_SAD 0x01
#define FFL_SAD 0x02
#define FRR_SAD 0x03
#define FRL_SAD 0x04
#define BASS_SAD 0x05
#define TRBL_SAD 0x06
#define SWCH_SAD 0x07

#define LOFF_BIT 0x40
#define ZCM_BIT 0x80


class TEA6321
{
public:
    TEA6321(void*);
    void init();
    void set_vol(const int8_t val );
    void set_bass(const int8_t val );
    void set_trbl(const int8_t val );
    void loudness( const bool on );
    void mute(const bool);
    void set_input( uint8_t inp );


protected:
    virtual int write_drv(uint8_t subbadr, uint8_t data)
    {
        return -1;
    };
    void* drv;
    uint8_t addr;
private:
    uint8_t vol_wrd;



};
