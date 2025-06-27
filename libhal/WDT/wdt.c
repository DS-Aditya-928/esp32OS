#include "WDT.h" 

#define MAGICNUM 0x050D83AA1

#define TIMG0_BASE 0x3FF5F000
#define TIMG1_BASE 0x3FF60000

#define TIMG_WP_OFF 0x0064
#define TIMG_WDTCONFIG0_OFF 0x48

#define RTC_BASE 0x3FF48000

#define RTC_WP_OFF 0x00A4
#define RTC_CNTL_WDTCONFIG0_OFF 0x8C 

static void disableWP(int tnum)
{
    switch(tnum)
    {
        case 0:
        {
            *(int*)(TIMG0_BASE + TIMG_WP_OFF) = MAGICNUM;
            break;
        }
        
        case 1:
        {
            *(int*)(TIMG1_BASE + TIMG_WP_OFF) = MAGICNUM;
            break;
        }

        case 2:
        {
            *(int*)(RTC_BASE + RTC_WP_OFF) = MAGICNUM;
            break;
        }
    }    
}

static void enableWP(int tnum)
{
    switch(tnum)
    {
        case 0:
        {
            *(int*)(TIMG0_BASE + TIMG_WP_OFF) = 1;
            break;
        }
        
        case 1:
        {
            *(int*)(TIMG1_BASE + TIMG_WP_OFF) = 1;
            break;
        }

        case 2:
        {
            *(int*)(RTC_BASE + RTC_WP_OFF) = 1;
            break;
        }
    }
}

void WDT::disableBootProtection(int tnum)
{
    //first disable write protection
    disableWP(tnum);
    unsigned int* confReg = 0;
    switch(tnum)
    {
        case 0:
        {
            confReg = (unsigned int*)(TIMG0_BASE + TIMG_WDTCONFIG0_OFF);
            break;
        }

        case 1:
        {
            confReg = (unsigned int*)(TIMG1_BASE + TIMG_WDTCONFIG0_OFF);
            break;
        }

        case 2:
        {
            confReg = (unsigned int*)(RTC_BASE + RTC_CNTL_WDTCONFIG0_OFF);
            break;
        }

        default:
            return;
    }

    unsigned int status = *(confReg);

    switch(tnum)
    {
        case 0:
        case 1:
        {
            status &= ~(1U << 14);
            break;
        }

        case 2:
        {
            status &= ~(1U << 10);
            break;
        }
    }

    *confReg = status;
    enableWP(tnum);
}