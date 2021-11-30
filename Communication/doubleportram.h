#ifndef DOUBLEPORTRAM_H
#define DOUBLEPORTRAM_H
#include <QFile>
#include <QProgressBar>

class DoublePortRam
{
public:
    DoublePortRam();
    ~ DoublePortRam();
    int DeviceSwitch;
    unsigned char SendData[10];
    unsigned char ReceiveData[10];
    int TransmitMark;
    void Send(char, long unsigned int);
    quint8 Receive( long  unsigned int);
    quint16 GetU16Data(unsigned int addr);
    bool UI_to_RTC();
    void RTC_to_UI();
    void to_Menu();
    void to_PatternWoven_slot();
    void to_Fileoperations_slot();
    void to_SystemMaintance();
    void to_SettingMain_slot();
    void to_TestingMain_slot();

private:
   // Noanswer *pNoanswer;
};

#endif // DOUBLEPORTRAM_H
