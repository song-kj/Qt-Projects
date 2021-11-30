#include "xrfile.h"
#include<stdio.h>
#include <stdlib.h>
#include <QFile>
#include <QDebug>

int XRFILE::decryptFile()
{
    pkeyStr = "HZXRFZJX";
    FILE *pPrefile=fopen(prePath,"rb");qDebug()<<prePath;
    fseek(pPrefile,0,SEEK_SET);
    //读取前4字节是否为“HZXR”，判断文件是否加密
    char mark[4];
    fread(mark,1,4,pPrefile);
    if(mark[0]!=72||mark[1]!=90||mark[2]!=88||mark[3]!=82)
    {
        fclose(pPrefile);
        return 0;
    }
    //在0x08处读取芯片类型
    fseek(pPrefile,0x08,SEEK_SET);
    fread(mark,1,1,pPrefile);
    MCU_type=mark[0];
    FILE *pAffile=fopen(afPath,"wb");
    fseek(pPrefile,0,SEEK_END);
    long fileL=ftell(pPrefile)-0x20;
    //在文件末尾读取加密key的随机长度
    fseek(pPrefile,-1,SEEK_END);
    char tmp[1];
    fread(tmp,1,1,pPrefile);
    int randL=(int)tmp[0];
    char* pRand=(char*)malloc(randL);
    //在0x20处读取加密密钥
    fseek(pPrefile,0x20,SEEK_SET);
    fread(pRand,1,randL,pPrefile);

    for(int i=0;i<randL;i++)
    {
        pRand[i]^=pkeyStr[i%8];
    }
    char buff[randL];
    int readCnt = fread(buff,1,randL,pPrefile);
    int  wCnt=0;
    while (readCnt > 0)
    {
        for (int i = 0; i < readCnt; i++)
        {
            buff[i] ^= pRand[i];
        }
        wCnt += readCnt;
        if (wCnt <= fileL - randL - 1)
        {
            fwrite(buff,1,randL,pAffile);
        }
        else
        {
            fwrite(buff,1,readCnt-1,pAffile);
            //wCnt += (readCnt - 1);
        }
        readCnt = fread(buff,1,randL,pPrefile);
    }
    fclose(pPrefile);
    fclose(pAffile);
    free(pRand);
    return 1;
}

int XRFILE::decryptFile(char *filepath, char *afilepath)
{
    try {
        QFile file(filepath);
        if(file.open(QIODevice::ReadWrite)){
            int effectLen=file.size()-5-0x400;
            QByteArray buffAdr;
            int keyAdr;
            file.seek(0x14);
            buffAdr=file.read(2);
            keyAdr=buffAdr[1]*256+buffAdr[0];
            int quotients =(int)( effectLen / 0x10);
            int remianders =(int)( effectLen % 0x10);
            QByteArray buffKey;
            if(keyAdr>0x1d0)
                keyAdr-=0x190;
            file.seek(keyAdr);
            buffKey=file.read(effectLen);
            file.seek(0x400);
            //QByteArray buff=file.read(effectLen);
            char buff[effectLen];
            file.read(buff,effectLen);
            for(int cnt=0;cnt<quotients;cnt++)
            {
                for(int i=0;i<16;i++)
                    buff[cnt*16+i]^=buffKey[i];
            }
            for(int j=0;j<remianders;j++)
                buff[quotients*16+j]^=buffKey[j];
            QFile filecmd(afilepath);
            if(filecmd.open(QIODevice::ReadWrite))
            {
                file.seek(0);
                QByteArray data=file.read(0x400);
                filecmd.seek(0x00);
                filecmd.write(data);
                filecmd.seek(0x400);
                filecmd.write(buff,effectLen);
                filecmd.close();
            }
            file.close();
            return 1;
        }
        return 0;
    }
    catch(...)
    {
        return 0;
    }
}
