#include "bd37534f.hpp"
#include <stdlib.h>


/*
 *--------------------------------------------------------------------------------------
 *       Class:  Bd37534f
 *      Method:  Bd37534f
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
Bd37534f::Bd37534f (void* _drv, uint8_t _addr):drv(_drv), addr(_addr)
{
//    write_drv(0x00); //sub addr
}  /* -----  end of method Bd37534f::Bd37534f  (constructor)  ----- */

void Bd37534f::init()
{
    write_drv(0x01, 0x85); // Adv mute time =1ms, adv sw time = 4.7ms
    write_drv(0x02, 0x43); //Lvl metter rst, LPF=120Hz/0deg, out=LPF
    write_drv(0x03, 0x01); //Loudness 250Hz
    write_drv(0x06, 0x14); //Mute off, gain +10dB
}
/*
 *--------------------------------------------------------------------------------------
 *       Class:  Bd37534f
 *      Method:  ~Bd37534f
 * Description:  destructor
 *--------------------------------------------------------------------------------------
 */
Bd37534f::~Bd37534f ()
{
}  /* -----  end of method Bd37534f::~Bd37534f  (destructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Bd37534f
 *      Method:  operator =
 * Description:  assignment operator
 *--------------------------------------------------------------------------------------
 */
/*    Bd37534f&
Bd37534f::operator = ( const Bd37534f &other )
{
    if ( this != &other ) {
    }
    return *this;
}   -----  end of method Bd37534f::operator =  (assignment operator)  ----- */


uint8_t Bd37534f::convert_to_bin( const uint8_t val)
{
    return (val == 0)? 0x80 : (val & 0x80) | (~(val -1) & 0x7F);
}        /* -----  end of template function calc_binVal  ----- */

void Bd37534f::set_vol(const int8_t val )
{
    uint8_t bin;
    if ((val > -79) && (val < 15))
        bin=convert_to_bin(val);
    else
        bin=0xFF;
    write_drv(0x20,bin);
}

/* -------------------------------------------------
 *!/
 * @brief : set gain/att for tone
 *
 */
void Bd37534f::set_tone(const tone_t tone, const int8_t value )
{
    uint8_t sign = 0;
    int8_t val=value;
    if (val < 0)
    {
        sign = 0x80;
        val=-val;
    }
    if( val > 20 )
        val = 20;
    write_drv(0x51+(tone*3), sign | (val & 0x1F));
}

void Bd37534f::set_bass(const int8_t val )
{
     Bd37534f::set_tone(Bd37534f::BASS,val);
}
void Bd37534f::set_midd(const int8_t val )
{
     Bd37534f::set_tone(Bd37534f::MIDD,val);
}
void Bd37534f::set_trbl(const int8_t val )
{
     Bd37534f::set_tone(Bd37534f::TRBL,val);
}

void Bd37534f::tone_setup(const Bd37534f::tone_t tone, const uint8_t fo_val, const uint8_t q_val)
{
//    if (q_val >3 ) return;  // erroe iidication
    write_drv(0x41+(3*tone), (0x30 & (fo_val<<4)) | ( 0x03 & q_val) );

}
void Bd37534f::bass_setup(const Bd37534f::bass_fo_t fo_val, const uint8_t q_val)
{
    tone_setup(Bd37534f::BASS, fo_val, q_val);
}
void Bd37534f::midd_setup(const Bd37534f::midd_fo_t fo_val, const uint8_t q_val)
{
    tone_setup(Bd37534f::MIDD, fo_val, q_val);
}
void Bd37534f::trbl_setup(const Bd37534f::trbl_fo_t fo_val, const uint8_t q_val)
{
    tone_setup(Bd37534f::TRBL, fo_val, q_val);
}
void Bd37534f::loudness( const bool on )
{
    if (on)
        write_drv(0x75, 0x00);
    else
        write_drv(0x75, loud_mode);
}
void Bd37534f::loudness_mode( const uint8_t mode,const uint8_t vol)
{
    loud_mode = (0x1F & (vol>20)?20:vol) | ((mode & 0x3)<<5);
    write_drv(0x75, loud_mode);
}
void Bd37534f::set_input( uint8_t inp )
{
    inp %= 0x06;
    if(inp == 5)
        inp =0x8;
    write_drv(0x5, 0x1F & inp);
}


