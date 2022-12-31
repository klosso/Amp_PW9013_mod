
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "bd37534f.hpp"
unsigned int __ut_cnt =0;
unsigned int __fail_ut_cnt =0;

#define EXPECT_EQ(__a, __b ) { if((__a) == (__b)) printf("%d - [\e[32mOK\e[m]\n",++__ut_cnt); else {printf("\nleft=0x%X\nright=0x%X\n%d - [\e[31mFAIL\e[m]\n",__a,__b,++__ut_cnt);fflush(stdout);__fail_ut_cnt++;};}
#define UT_SUMMARY { if(__fail_ut_cnt)printf("Overall: %d/%d FAILED !\n",__fail_ut_cnt,__ut_cnt); else printf("Overall: All %d tests passed",__ut_cnt);}

uint8_t results[10];
uint8_t counter=0;

class STM_Bd : public Bd37534f
{
public:
    STM_Bd(void* n) :Bd37534f(n){};
    // mock
    virtual int write_drv( const uint8_t in)
    {
        EXPECT_EQ(in, results[counter]);
        counter++;
        return 0;
    }
};

int main( int argc, char* argv[] )
{

    // ------ Bd37534f class ut --------
    STM_Bd impl(nullptr);

    printf("TEST: loudness on test \n");
    // loudness on test
    results[0]=0x75;
    results[1]=0;
    counter=0;
    impl.loudness(true);
    EXPECT_EQ(2,counter);


    printf("TEST: volume set to 10dB\n");
    results[0]=0x20;
    results[1]=0b01110110;
    counter=0;
    impl.set_vol(10);
    EXPECT_EQ(2,counter);

    printf("TEST: volume set to -10dB\n");
    // set_vol to -10
    results[0]=0x20;
    results[1]=0b10001010;
    counter=0;
    impl.set_vol(-10);
    EXPECT_EQ(2,counter);

    printf("TEST: exeed 19dB vol \n");
    results[0]=0x20;
    results[1]=0xFF;
    counter=0;
    impl.set_vol(20);

    printf("TEST: set_vol to -80dB\n");
    results[0]=0x20;
    results[1]=0xFF;
    counter=0;
    impl.set_vol(-80);

    printf("TEST: loudness mode exeed 20dB\n");
    results[0]=0x75;
    results[1]=20;
    counter=0;

    impl.loudness_mode(0,22);
    printf("TEST: loudness mode exeed hints 5\n");
    results[0]=0x75;
    results[1]=0x22;
    counter=0;
    impl.loudness_mode(5,2);

    printf("TEST: exeede tone to +20dB\n");
    results[0]=0x51;
    results[1]=20;
    counter=0;
    impl.set_bass(22);

    printf("TEST: exeede tone to -20dB\n");
    results[0]=0x51;
    results[1]=0x94;
    counter=0;
    impl.set_bass(-22);

    printf("TEST: normal tone test to +6dB\n");
    results[0]=0x54;
    results[1]=6;
    counter=0;
    impl.set_midd(6);

    printf("TEST: exeed tone setup test\n");
    results[0]=0x44;
    results[1]=0x22;
    counter=0;
    impl.midd_setup( Bd37534f::_1500Hz,2);
    EXPECT_EQ(2,counter);

    printf("TEST: input set to B\n");
    results[0]=0x05;
    results[1]=0x01;
    counter=0;
    impl.set_input(1);
    EXPECT_EQ(2,counter);

    printf("TEST: input set to E\n");
    results[0]=0x05;
    results[1]=0b1000;
    counter=0;
    impl.set_input(5);
    EXPECT_EQ(2,counter);


    UT_SUMMARY;
    return 0;
}
