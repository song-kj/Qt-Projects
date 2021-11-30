#ifndef XRFILEDE_H
#define XRFILEDE_H
#include <QByteArray>
class XRFILE
{
    private:
      char* prePath;
      char* afPath;
      char* pkeyStr;
    public:
     XRFILE()
     {

     }
     XRFILE(char* preFilePath, char* afFilePath)
     {
         prePath=preFilePath;
         afPath=afFilePath;
     }
     quint8 MCU_type;
     int decryptFile();
     int decryptFile(char* filepath,char* afilepath);//创达花型解密
};

#endif // XRFILEDE_H
