#pragma once
#include <stdio.h>
#include <stdint.h>
class Bd37534f
{
public:

	enum tone_t{ BASS=0, MIDD, TRBL };
	enum bass_fo_t{ _60Hz=0, _80Hz, _100Hz, _120Hz };
	enum midd_fo_t{ _500Hz=0, _1000Hz, _1500Hz, _2500Hz };
	enum trbl_fo_t{ _7500Hz=0, _10000Hz, _12500Hz, _15000Hz };

	Bd37534f (void* _drv, uint8_t _addr = 0x80);
	~Bd37534f ();
	void init();
	void set_vol(const int8_t val );
	void set_bass(const int8_t val );
	void set_midd(const int8_t val );
	void set_trbl(const int8_t val );
	void set_tone( const tone_t tone, int8_t val);
	void bass_setup(const bass_fo_t fo_val, const uint8_t q_val);
	void midd_setup(const midd_fo_t fo_val, const uint8_t q_val);
	void trbl_setup(const trbl_fo_t fo_val, const uint8_t q_val);
	void loudness( const bool on );
	void loudness_mode( const uint8_t mode,const uint8_t vol);
	void set_input( uint8_t inp );
protected:
    virtual int write_drv(const uint8_t addr,const uint8_t val){return -1;};
    const void* drv;
    uint8_t addr;
private:
	uint8_t convert_to_bin(const uint8_t);
	void tone_setup(const Bd37534f::tone_t tone, const uint8_t fo_val, const uint8_t q_val);
	uint8_t volume;
	uint8_t loud_mode;


};  /* -----  end of method Bd37534f::Bd37534f  (constructor)  ----- */

