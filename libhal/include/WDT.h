#ifndef dsWDT
#define dsWDT


namespace WDT
{
    enum WatchDogs
    {
        TIMG0,
        TIMG1,
        RTC
    };

    void disableBootProtection(int);
    void disableWatchdog(int);
}

#endif