#include "hirepurchase.h"
#include "ui_hirepurchase.h"
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QMessageBox>
#include "myheader.h"
QString Deadline;
int Flag;
extern bool StartFlag;
Hirepurchase::Hirepurchase(QWidget *parent) :
    QWidget(parent)
{
    analyseFlag=-1;//
    result="";
    fileNameStart="NULL";
    handCode="";
    mythread=new hireThread();
    connect(mythread,SIGNAL(setInfoWhetherExpire(bool)),this,SLOT(getInfoWhetherExpire(bool)));

}

Hirepurchase::~Hirepurchase()
{

}
bool Hirepurchase::startHirpurchase(QString deadline)
{
    if(!deadline.isEmpty())
    {
//        Deadline=deadline;
//        Flag=1;
//        mythread->start();
        if(compareDate(deadline))
        {
            emit setExpireReminder(false);//未到期发送
        }
        else
        {
            emit setExpireReminder(true);//到期时发送
        }
        return true;
    }
    else
    {
        return false;
    }
}
//配置系统
bool Hirepurchase::sysetemSetting(QString filename)
{
    if(QFile::exists(filename))
    {
        analyseFlag=0;
        analyzeFile(filename);
        return  true;
    }
    else
    {
        return false;
    }
}

void Hirepurchase::setData(QString length ,QString code)
{
    companyCodeAsciiLength=length;
    companyCodeAscii=code;
}

QString Hirepurchase::decryption(QString str)
{
    //加密后的随机数
    uchar code_de[8];
    bool ok;
    for(int i=0;i<str.length()/2;i++)
    {
        code_de[i%8]=str.mid(i*2,2).toUInt(&ok,16);
    }

    uchar key[4];
    for(int i=0;i<companyCodeAscii.length()/2;i++)
    {
        key[i%4]=companyCodeAscii.mid(i*2,2).toUInt(&ok,16);
    }

    //解密
    for(int i=0;i<8;i++)
    {
        code_de[i]=key[i%4]^code_de[i];
    }
    uchar code_t[4];
    uchar code_a[4];//
    code_t[0]=code_de[0];
    code_t[1]=code_de[4];
    code_t[2]=code_de[3];
    code_t[3]=code_de[6];
    code_a[0]=code_de[5]-code_t[0];
    code_a[1]=code_de[7]-code_t[1];
    code_a[2]=code_de[1]-code_t[2];
    code_a[3]=code_de[2]-code_t[3];
    str="";
    QString str_t="";
    for (int i=0;i<4;i++)
    {
        str+=QString("%1").arg(code_a[i],2,16,QChar('0'));
        str_t+=QString("%1").arg(code_t[i],2,16,QChar('0'));
    }
    return str+str_t;
}

//解锁
int Hirepurchase::unlockedAuto(QString sysId, QString cor, QString keyOraddr)
{
    int rst=0;
    if(!(sysId.isEmpty())&&!(keyOraddr.isEmpty())&&!(cor.isEmpty()))
    {
        analyseFlag=1;
        analyzeFile(keyOraddr);
        if(QString::compare(sysId,sysIDEnding,Qt::CaseInsensitive)==0)
        {
            if(cor.length()!=0&&companyCodeEnding.length()!=0&&cor.length()!=companyCodeEnding.length())
            {
                if(cor.length()<companyCodeEnding.length())
                    companyCodeEnding=companyCodeEnding.mid(0,cor.length());
            }
            if(cor==companyCodeEnding)
            {
                if(compareDate(sysDeadlineTimeEnding))
                {
                    rst=1;
                }
                else
                {
                    rst=4;
                }
            }
            else
            {
                rst=3;
            }
        }
        else
        {
           rst=2;
        }
    }
    else
    {
        rst=0;
    }
    return rst;
}
//比较是否过期
bool Hirepurchase::compareDate(QString date)
{
    QDateTime time = QDateTime::currentDateTime();
    QString currentTime = time.toString("yyyyMMdd");
    int year = date.left(4).toInt()-currentTime.left(4).toInt();
    if(year>0)
    {
        return true;
    }
    else if(year<0)
    {
        return false;
    }
    else
    {
        QString monthPreDate = date.remove(0,4);
        QString monthPreCurrentTime = currentTime.remove(0,4);
        int month = monthPreDate.left(2).toInt()-monthPreCurrentTime.left(2).toInt();
        if(month>0)
        {
            return true;
        }
        else if(month<0)
        {
            return false;
        }
        else
        {
            int day = date.right(2).toInt()-currentTime.right(2).toInt();
            if(day>=0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
//高低位互换
qint32 Hirepurchase::HigherLowerConvert(qint32 original)
{
    qint8 lowerLower = original&0x000000ff;
    qint8 lower = (original&0x0000ff00)>>8;
    qint8 higher = (original&0x00ff0000)>>16;
    qint8 higherHigher = (original&0xff000000)>>24;
    return lowerLower<<24|(lower<<16)|(higher<<8)|(higherHigher);
}
//解析加密文件
void Hirepurchase::analyzeFile(QString filepath)
{
    QFile file(filepath);//解锁文件路径
    if(file.open(QIODevice::ReadWrite))//读取ID号 开始日期 截止日期 线程开始的标示位
    {
        qint32 keyL;qint32 IVL;quint8 KEY_byte;quint8 IV_byte;QByteArray KEY;QByteArray IV;
        quint8 Ciphertext_byte;QByteArray Ciphertext;
        QDataStream in(&file);
        in>>keyL;//密钥1长度
        keyL = HigherLowerConvert(keyL);
        in>>IVL;
        IVL = HigherLowerConvert(IVL);


       // qDebug()<<"keyL=="<<keyL<<"IVL=="<<IVL;

        KEY.resize(keyL);
        IV.resize(IVL);
        Ciphertext.resize(file.size()-8-keyL-IVL);
        for(int i=0;i<keyL;i++)
        {
            in>>KEY_byte;
            KEY[i] = KEY_byte;//密钥1的内容
           // qDebug("KEY----------------- is %d,%x",QVariant((unsigned char)KEY[i]).toInt(),QVariant((unsigned char)KEY[i]).toInt());
        }
        for(int j=0;j<IVL;j++)
        {
            in>>IV_byte;
            IV[j] = IV_byte;//密钥2的内容
            //qDebug("IV----------------- is %d,%x",QVariant((unsigned char)IV[j]).toInt(),QVariant((unsigned char)IV[j]).toInt());
        }
        for(int k=0;k<file.size()-8-keyL-IVL;k++)
        {
            in>>Ciphertext_byte;
            Ciphertext[k]=Ciphertext_byte;
           // qDebug("Ciphertext----------------- is %d,%x",QVariant((unsigned char)Ciphertext[k]).toInt(),QVariant((unsigned char)Ciphertext[k]).toInt());
        }

        QString ss= decryptKey(KEY,IV,Ciphertext);
        //qDebug()<<"ss-----"<<ss;
        if(analyseFlag==0)
        {
            companyCodeLength= ss.left(2).toInt();
            ss=ss.remove(0,2);

            macLength=ss.left(2).toInt();
            ss=ss.remove(0,2);

            idLength = ss.left(2).toInt();
            ss=ss.remove(0,2);

            startTLength=ss.left(2).toInt();
            ss=ss.remove(0,2);

            endTLength = ss.left(2).toInt();
            ss=ss.remove(0,2);

            sysDeadlineTime=ss.right(endTLength);
            ss.chop(endTLength);

            sysStartTime = ss.right(startTLength);
            ss.chop(startTLength);

            sysID=ss.right(idLength);
            ss.chop(idLength);

            mac=ss.right(macLength);
            ss.chop(macLength);
            companyCode=ss.right(companyCodeLength);
            companyCodeAscii.resize(0);
            companyCodeAsciiLength.resize(0);
            companyCodeAscii=QString(companyCode.toAscii().toHex());
            if(companyCodeAscii.size()<10)
            {
                companyCodeAsciiLength=tr("0").append(QString::number(companyCodeAscii.size()));
            }
            else
            {
                companyCodeAsciiLength=QString::number(companyCodeAscii.size());
            }
        }
        else if(analyseFlag==1)
        {
            int codeEndL=ss.left(2).toInt();
            ss=ss.remove(0,2);

            int macEndL=ss.left(2).toInt();
            ss=ss.remove(0,2);

            int sysIdEndL=ss.left(2).toInt();
            ss=ss.remove(0,2);

            int endTEndL=ss.left(2).toInt();
            ss=ss.remove(0,2);

            sysDeadlineTimeEnding = ss.right(endTEndL);
            ss.chop(endTEndL);

            sysIDEnding = ss.right(sysIdEndL);
            ss.chop(sysIdEndL);

            macEnding=ss.right(macEndL);
            ss.chop(macEndL);

            companyCodeEnding=ss.right(codeEndL);
        }
        file.close();
    }
}
//手动解锁时，生产随机码，并生成密钥
QString Hirepurchase::createRandomCode()
{
    return encryption();
    result.resize(0);
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    QString str;
    QString num;
    str.clear();
    num.clear();
    int random;
    for(int i=0;i<16;i++)
    {
        random = qrand()%16;
        if(random==10)
        {
            num="a";
        }
        else if(random==11)
        {
            num="b";
        }
        else if(random==12)
        {
            num="c";
        }
        else if(random==13)
        {
            num="d";
        }
        else if(random==14)
        {
            num="e";
        }
        else if(random==15)
        {
            num="f";
        }
        else
        {
            num=QString::number(random);
        }

        str =str.append(num);
    }
  //  qDebug()<<"str----------------"<<str;

    QByteArray code =  hexToString(str);
    QByteArray codeRep;
    codeRep.resize(16);
    codeRep[0]=code[0];
    codeRep[1]=code[4];
    codeRep[2]=code[8];
    codeRep[3]=code[12];
    codeRep[4]=code[1];
    codeRep[5]=code[5];
    codeRep[6]=code[9];

    codeRep[7]=code[13];
    codeRep[8]=code[2];
    codeRep[9]=code[6];
    codeRep[10]=code[10];

    codeRep[11]=code[14];
    codeRep[12]=code[3];
    codeRep[13]=code[7];

    codeRep[14]=code[11];
    codeRep[15]=code[15];

    QByteArray key;
    key.resize(8);
    for(int i=0;i<8;i++)
    {
       //qDebug("code----------------- is %d,%x",QVariant((unsigned char)code[i]).toInt(),QVariant((unsigned char)code[i]).toInt());
       key[i]=(quint8)(codeRep.at(2*i)*238+codeRep.at(2*i+1));
       //qDebug("key----------------- is %d,%x",QVariant((unsigned char)key[i]).toInt(),QVariant((unsigned char)key[i]).toInt());
    }
    QString strr;
    for(int k=0;k<8;k++)
    {
        strr=QString::number(key.at(k),16);
        if(strr.size()>2)
        {
            strr=strr.right(2);
        }
        else if(strr.size()<2)
        {
            strr=tr("0").append(strr);
        }
        result=result.append(strr);
    }

    QString AA;
    AA.resize(0);
    AA=companyCodeAscii;
    QString BB;
    BB.resize(0);
    BB=companyCodeAsciiLength;
    handCode=AA.append(result).append(BB);

    //qDebug()<<result<<handCode.size()<<handCode;
    //result.clear();
    return str;//返回随机码
}
//自动解密解析密文
QString Hirepurchase::decryptKey(QByteArray key, QByteArray iv, QByteArray ciphertext)
{
    QByteArray keyLetter("HZXRFZJXYXGS");
    QByteArray keyNum;
    keyNum.resize(8);
    keyNum[0]=2;
    keyNum[1]=8;
    keyNum[2]=0;
    keyNum[3]=0;
    keyNum[4]=2;
    keyNum[5]=1;
    keyNum[6]=5;
    keyNum[7]=9;


    for (int i = 0; i < key.length() - 4; i += 4)
    {
        key[i + 3] = key[i + 3] ^keyLetter[i%keyLetter.length()];
    }
    for (int i = 0; i < key.length(); i++)
    {
        key[i] = key[i] ^keyLetter[i%keyLetter.length()];
    }
    for (int i = 0; i < iv.length() - 4; i += 2)
    {
        iv[i + 2] = iv[i + 2] ^keyNum[i%keyNum.length()];
    }
    for (int i = 0; i < iv.length(); i++)
    {
        iv[i] = iv[i] ^keyNum[i%keyNum.length()];
    }


    for (int i = 0; i < iv.length(); i++)
    {
       // qDebug("iv----------------- is %c,%x",QVariant((unsigned char)iv[i]).toInt(),QVariant((unsigned char)iv[i]).toInt());
    }
    for (int i = 0; i < key.length(); i++)
    {
        //qDebug("key----------------- is %c,%x",QVariant((unsigned char)key[i]).toInt(),QVariant((unsigned char)key[i]).toInt());
    }


    for (int i = 0; i < ciphertext.length();i++)
    {
        ciphertext[i] = ciphertext[i] ^iv[i%iv.length()];
        ciphertext[i] =  ciphertext[i] ^key[i%key.length()];


       // qDebug("%c,%x",QVariant((unsigned char)ciphertext[i]).toInt(),QVariant((unsigned char)ciphertext[i]).toInt());
    }
    QString sp;//4000 11000 5000 e000 8000
    if(analyseFlag==0)
    {
        for(int m=0;m<5;m++)
        {
    //0 1 2 3  4 5 6 7  8 9 10 11  12 13 14 15  16 17 18 19
            if(QString::number(ciphertext[4*m]).length()<2)
            {
                sp.append(tr("0").append(QString::number(ciphertext[4*m],16)));
            }
            else
            {
                sp= sp.append(QString::number(ciphertext[4*m]));
            }
        }
    }
    else if(analyseFlag==1)
    {
        for(int m=0;m<4;m++)
        {

            if(QString::number(ciphertext[4*m]).length()<2)
            {
                sp.append(tr("0").append(QString::number(ciphertext[4*m],16)));
            }
            else
            {
                sp= sp.append(QString::number(ciphertext[4*m]));
            }
        }
    }

    char ch[10];
    for(int n=0;n<ciphertext.size();n++)
    {
        quint8 a=(quint8)QString::number(ciphertext[n]).toInt();
        sprintf(ch,"%c",a);
        QString chtStr(ch);
        if(!chtStr.isEmpty())
        {
            sp=sp.append(chtStr);
        }
    }
    return  sp;
}

QByteArray Hirepurchase::hexToString(QString str)
{
    QByteArray arr;QByteArray num;
    arr.clear();
    num.resize(16);

    int length = str.length();
    for(int i=0;i<length;i++)
    {
        if(str.left(1).contains("0"))
        {
            num[i]=(char)0;
        }
        else if(str.left(1).contains("1"))
        {
            num[i]=(char)1;
        }
        else if(str.left(1).contains("2"))
        {
            num[i]=(char)2;
        }
        else if(str.left(1).contains("3"))
        {
            num[i]=(char)3;
        }
        else if(str.left(1).contains("4"))
        {
            num[i]=(char)4;
        }
        else if(str.left(1).contains("5"))
        {
            num[i]=(char)5;
        }
        else if(str.left(1).contains("6"))
        {
            num[i]=(char)6;
        }

        else if(str.left(1).contains("7"))
        {
            num[i]=(char)7;
        }
        else if(str.left(1).contains("8"))
        {
            num[i]=(char)8;
        }
        else if(str.left(1).contains("9"))
        {
            num[i]=(char)9;
        }
        else if(str.left(1).contains("a"))
        {
            num[i]=(char)10;
        }
        else if(str.left(1).contains("b"))
        {
            num[i]=(char)11;
        }
        else if(str.left(1).contains("c"))
        {
            num[i]=(char)12;
        }
        else if(str.left(1).contains("d"))
        {
            num[i]=(char)13;
        }
        else if(str.left(1).contains("e"))
        {
            num[i]=(char)14;
        }
        else if(str.left(1).contains("f"))
        {
            num[i]=(char)15;
        }
        str = str.remove(0,1);

    }
    return num;
}

QString Hirepurchase::encryption()
{
    unsigned char code_rand[4] = {0x12,0x13,0x14,0x15};//随机数

    //    QTime time;
    //    time= QTime::currentTime();
    //    qsrand(time.msec()+time.second()*1000);
#ifdef Q_OS_LINUX
        qsrand(time(NULL));
#endif
        int random;
        for(int i=0;i<4;i++)
        {
            random = qrand()%255;
            code_rand[i]=random;
        }


        unsigned char key[4];

        bool ok;
        QString str=companyCodeAscii;
        if(str!="")
        {
            for(int i=0;i<4;i++)
            {
                if(str.length()<i*2)
                    key[i]=key[i%(str.length()/2)];
                else
                    key[i]=str.mid(i*2,2).toUInt(&ok,16);
            }
        }
        uchar code_rand_e[4];//加密后的随机数
        //加密过程
        for (int i=0;i<4;i++)
        {
            code_rand_e[i]=code_rand[i]^key[i];
        }
        str="";
        for (int i=0;i<4;i++)
        {
            str+=QString("%1").arg(code_rand_e[i],2,16,QChar('0'));
        }
        return str;
}

//验证密钥是否正确
bool Hirepurchase::produceKeyCode(QString KeyCode)
{
    if(!result.isEmpty())
    {
        //qDebug()<<handCode;
        //qDebug()<<KeyCode.size()<<handCode.size();
        if(QString::compare(KeyCode,handCode,Qt::CaseInsensitive)==0)
        {
            Flag=0;
            StartFlag=false;
            return true;
        }
        else
        {
          //  result.clear();
            return false;
        }
    }
    else
    {
       // result.clear();
        return false;
    }
}

void Hirepurchase::getInfoWhetherExpire(bool mark)
{
    if(mark)
    {
        qDebug()<<"daoqi";
       emit setExpireReminder(true);
    }
    else
    {
        qDebug()<<"weidaoqi";
        emit setExpireReminder(false);
    }
}
