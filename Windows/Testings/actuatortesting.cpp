#include "actuatortesting.h"
#include "ui_actuatortesting.h"

extern QByteArray temp;
ActuatorTesting::ActuatorTesting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ActuatorTesting)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    sum=-1;
    sum_single_Act=-1;
    sum_interval_Act=-1;
    sum_whole_Act=-1;
    sum_interval_whole_Act=-1;
    ManualAndDebugSwitch=-1;
    mark=1;
    pre_i=100;
    UpDown_single=true;
    Sum_single=0;
    UpDown_whole=true;
    Sum_whole=0;
    Sum_singinterval=0;
    interval_single=true;
    Sum_wholeinterval=0;
    interval_whole=true;

    singleOld=true;
    whloeOld=true;
    wholeCir=true;
    singleCir=true;
    PreRoad=0;
    Road=6;
    whichWidget=0;
    mod=1;
    single_updown_or_single_debug=0;
    whole_updown_or_whole_debug=0;
    click=new QPushButton();
    ui->speed_lb->setText(QString::fromUtf8("1"));
    ui->speed_lb->setStyleSheet("QLabel{font:bold;font-size:22px;color:red}");
    for(int i=0;i<8;i++)//手动测试模块的控件
    {
        manual[i]=new QPushButton(this);
        if(i<4)
            manual[i]->setGeometry(0,100+i*95,48,91);
        else
            manual[i]->setGeometry(753,100+(i-4)*95,48,91);
        manual[i]->hide();
    }

    for(int i=0;i<6;i++)//循环测试和侦错测试模块的控件
    {
        circulation[i]=new QPushButton(this);
        debug[i]=new QPushButton(this);
        if(i<4)
        {
            circulation[i]->setGeometry(0,100+i*95,48,91);
            debug[i]->setGeometry(0,100+i*95,48,91);
        }

        else
        {
            circulation[i]->setGeometry(753,100+(i-4)*285,48,91);
            debug[i]->setGeometry(753,100+(i-4)*285,48,91);
        }
        circulation[i]->hide();
        debug[i]->hide();

    }
    manualtest_Act_pb=new QPushButton(this);   //手动测试
    circulatetest_Act_pb=new QPushButton(this);//循环测试
    debugtest_Act_pb=new QPushButton(this);    //侦错测试
    empty1_Act_pb=new QPushButton(this);       //空白1
    empty2_Act_pb=new QPushButton(this);       //空白2
    plus_Act_pb=new QPushButton(this);         //加速
    minus_Act_pb=new QPushButton(this);        //减速
    back_Act_pb=new QPushButton(this);         //返回
    modtest_Act_pb=new QPushButton(this);

    manualtest_Act_pb->setGeometry(0,100,48,91);
    circulatetest_Act_pb->setGeometry(0,195,48,91);
    debugtest_Act_pb->setGeometry(0,290,48,91);
    empty1_Act_pb->setGeometry(0,385,48,91);
    modtest_Act_pb->setGeometry(0,5,48,91);

    empty2_Act_pb->setGeometry(753,290,48,91);
    plus_Act_pb->setGeometry(753,100,48,91);
    minus_Act_pb->setGeometry(753,195,48,91);
    back_Act_pb->setGeometry(753,385,48,91);

    manualtest_Act_pb->setText(QString::fromUtf8("手\n动\n测\n试"));
    circulatetest_Act_pb->setText(QString::fromUtf8("循\n环\n测\n试"));
    modtest_Act_pb->setText(tr("单\n刀\n老\n化"));
    // debugtest_Act_pb->setText(QString::fromUtf8("侦\n错\n测\n试"));

    plus_Act_pb->setText(QString::fromUtf8("加\n\n速"));
    minus_Act_pb->setText(QString::fromUtf8("减\n\n速"));
    back_Act_pb->setText(QString::fromUtf8("返\n\n回"));

    manualtest_Act_pb->setStyleSheet("QPushButton{border-radius:10;background-color: rgb(100, 100, 100);font-size:18px;color:rgb(255, 255, 0);}");
    circulatetest_Act_pb->setStyleSheet("QPushButton{border-radius:10;background-color: rgb(100, 100, 100);font-size:18px;color:rgb(255, 255, 0);}");
    debugtest_Act_pb->setStyleSheet("QPushButton{border-radius:10;background-color: rgb(100, 100, 100);font-size:18px;color:rgb(255, 255, 0);}");
    manualtest_Act_pb->setStyleSheet("QPushButton{border-radius:10;background-color: rgb(100, 100, 100);font-size:18px;color:rgb(255, 255, 0);}");
    plus_Act_pb->setStyleSheet("QPushButton{border-radius:10;background-color: rgb(100, 100, 100);font-size:18px;color:rgb(255, 255, 0);}");
    minus_Act_pb->setStyleSheet("QPushButton{border-radius:10;background-color: rgb(100, 100, 100);font-size:18px;color:rgb(255, 255, 0);}");
    back_Act_pb->setStyleSheet("QPushButton{border-radius:10;background-color: rgb(100, 100, 100);font-size:18px;color:rgb(255, 255, 0);}");
    empty1_Act_pb->setStyleSheet("QPushButton{border-radius:10;background-color: rgb(100, 100, 100);font-size:18px;color:rgb(255, 255, 0);}");
    empty2_Act_pb->setStyleSheet("QPushButton{border-radius:10;background-color: rgb(100, 100, 100);font-size:18px;color:rgb(255, 255, 0);}");
    modtest_Act_pb->setStyleSheet("QPushButton{border-radius:10;background-color: rgb(100, 100, 100);font-size:18px;color:rgb(255, 255, 0);}");

    ui->info_pb->setStyleSheet("QPushButton{border-width:0;border-style:outset;background: transparent;font-size:20px;color:red;}");
    ui->previouspage_Act_pb->setStyleSheet("QPushButton{background-color: rgb(27, 150, 208);;font-size:22px;color:white}");
    ui->nextpage_Act_pb->setStyleSheet("QPushButton{background-color: rgb(27, 150, 208);;font-size:22px;color:white}");
    connect(manual[0],SIGNAL(clicked()),this,SLOT(singleupdown_slot()));  //单路上下
    connect(manual[1],SIGNAL(clicked()),this,SLOT(singleinterval_slot()));//单路正隔
    connect(manual[2],SIGNAL(clicked()),this,SLOT(wholeupdown_slot()));   //全路上下
    connect(manual[3],SIGNAL(clicked()),this,SLOT(wholeinterval_slot())); //全路正隔
    connect(manual[7],SIGNAL(clicked()),this,SLOT(backfrommanul_slot())); //返回自手动测试
    connect(circulation[0],SIGNAL(clicked()),this,SLOT(singlematuring_slot()));
    connect(circulation[1],SIGNAL(clicked()),this,SLOT(wholematuring_slot()));
    connect(circulation[2],SIGNAL(clicked()),this,SLOT(singlecirculation_slot()));
    connect(circulation[3],SIGNAL(clicked()),this,SLOT(wholecirculation_slot()));
    connect(circulation[5],SIGNAL(clicked()),this,SLOT(backfromcirculation_slot())); //返回自循环测试
    connect(debug[0],SIGNAL(clicked()),this,SLOT(singledebug_slot())); //单路侦错
    connect(debug[1],SIGNAL(clicked()),this,SLOT(wholedebug_slot())); //全路侦错
    connect(debug[2],SIGNAL(clicked()),this,SLOT(debuglist_slot())); //侦错记录
    connect(debug[3],SIGNAL(clicked()),this,SLOT(debugreset_slot())); //侦错清零
    connect(debug[4],SIGNAL(clicked()),this,SLOT(selflearning_slot())); //自我学习
    connect(debug[5],SIGNAL(clicked()),this,SLOT(backfromdebug_slot())); //返回自侦错测试
    connect(manualtest_Act_pb,SIGNAL(clicked()),this,SLOT(manualtest_slot()));//手动测试
    connect(back_Act_pb,SIGNAL(clicked()),this,SLOT(back_slot()));//返回
    connect(circulatetest_Act_pb,SIGNAL(clicked()),this,SLOT(circulatetest_slot()));//循环测试
    //  connect(debugtest_Act_pb,SIGNAL(clicked()),this,SLOT(debugtest_slot()));//侦错测试
    connect(plus_Act_pb,SIGNAL(clicked()),this,SLOT(plus_slot()));//加速
    connect(minus_Act_pb,SIGNAL(clicked()),this,SLOT(minus_slot()));//减速
    connect(modtest_Act_pb,SIGNAL(clicked(bool)),this,SLOT(modtest_Act_pb_clicked()));
    modtest_Act_pb->setVisible(false);

    startSelectRoad=0;
    showGUI(0,6);
    show_actuator(0,6);
    showRED(0,6);
    touchShow_ActuatorTesting=new QLabel(this);
    touchShow_ActuatorTesting->setStyleSheet("QLabel{background:transparent;}");
    touchStart_ActuatorTesting=false;

    timer_act=new QTimer(this);
    connect(timer_act,SIGNAL(timeout()),this,SLOT(update_slot()));
    timer_act->start(10);
}
ActuatorTesting::~ActuatorTesting()
{
    delete ui;
}
void ActuatorTesting::update_slot()
{
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        quint8 key=temp[0];
        temp.clear();
        if(key==38)
        {
            on_previouspage_Act_pb_clicked();
        }
        if(key==40)
        {
            on_nextpage_Act_pb_clicked();
        }
        if(key==39)
        {
            if(PreRoad==0)
            {
                if(istring<6)
                {
                    framecover[istring]->click();
                }
                else
                {
                    framecover[5]->click();
                }
            }
            if(PreRoad==6)
            {
                if(istring<12)
                {
                    framecover[istring]->click();
                }
                else
                {
                    framecover[11]->click();
                }
            }
            if(PreRoad==12)
            {
                if(istring<16)
                {
                    framecover[istring]->click();
                }
                else
                {
                    framecover[15]->click();
                }
            }
        }
        if(key==29)
        {
            if(PreRoad==0)
            {
                if(istring<7&&istring>1)
                {
                    framecover[istring-2]->click();
                }
                else
                {
                    framecover[0]->click();
                }
            }
            if(PreRoad==6)
            {
                if(istring<13&&istring>7)
                {
                    framecover[istring-2]->click();
                }
                else
                {
                    framecover[6]->click();
                }
            }
            if(PreRoad==12)
            {
                if(istring<17&&istring>13)
                {
                    framecover[istring-2]->click();
                }
                else
                {
                    framecover[12]->click();
                }
            }
        }
        if(key==1)
        {
            if(modtest_Act_pb->isVisible())
                modtest_Act_pb->click();
        }
        if(key==2)
        {
            if(whichWidget==0)
            {
                manualtest_slot();
            }
            else if(whichWidget==1)
            {

                singleupdown_slot();
            }
            else if(whichWidget==2)
            {

                singlematuring_slot();
            }
            else if(whichWidget==3)
            {

                singledebug_slot();
            }
        }
        if(key==3)
        {
            if(whichWidget==0)
            {

                circulatetest_slot();
            }
            else if(whichWidget==1)
            {

                singleinterval_slot();
            }
            else if(whichWidget==2)
            {
                wholematuring_slot();
            }
            else if(whichWidget==3)
            {
                wholedebug_slot();
            }
        }
        if(key==4)
        {
            if(whichWidget==0)
            {
                debugtest_slot();
            }
            else if(whichWidget==1)
            {
                wholeupdown_slot();
            }
            else if(whichWidget==2)
            {
                singlecirculation_slot();
            }
            else  if(whichWidget==3)
            {
                debuglist_slot();
            }
        }
        if(key==5)
        {
            if(whichWidget==1)
            {
                wholeinterval_slot();
            }
            else  if(whichWidget==2)
            {
                wholecirculation_slot();
            }

        }
        if(key==7)
        {
            if(whichWidget==3)
            {
                selflearning_slot();
            }
            if(whichWidget==0||whichWidget==2||whichWidget==3)
            {
                plus_slot();
            }

        }
        if(key==8)
        {
            if(whichWidget==0||whichWidget==2||whichWidget==3)
            {
                minus_slot();
            }
        }

        if(key==28||key==10)
        {
            if(whichWidget==0)
            {

                back_slot();
            }
            else if(whichWidget==1)
            {

                backfrommanul_slot();
            }
            else if(whichWidget==2)
            {

                backfromcirculation_slot();
            }
            else if(whichWidget==3)
            {

                backfromdebug_slot();
            }
        }
    }
}
void ActuatorTesting::showGUI(int first,int last)
{
    for( int i=first;i<last;i++)
    {
        k=10;
        ActuatotRoad[i]=new QLabel(ui->widget);
        ActuatotRoad[i]->setAlignment(Qt::AlignHCenter);
        ActuatotRoad[i]->setStyleSheet("QLabel{font-size:20px;color:blue}");
        ActuatotRoad[i]->setText(QString::fromUtf8("第%1路").arg(i+1));
        frame[i]=new ClickedQLabel(ui->widget);
        framecover[i]=new QPushButton(ui->widget);
        if(last==16)
            frame[i]->hide();
        for( int j=0;j<16;j++)
        {
            pin[i][j]=new QPushButton(frame[i]);
            pin[i][j]->setCheckable(true);
            pin[i][j]->setStyleSheet("font-size:20px;background-color: rgb(120, 120, 120);color:black");
            pin[i][j]->setStyle(QStyleFactory::create("Windows"));
            connect(pin[i][j], SIGNAL(clicked()), this, SLOT(showframe()));
            if(i<9)
            {
                if(j>7)
                {
                    k=k-2;
                    pin[i][j]->setText(tr("%1").arg(j+k));
                    pin[i][j]->setToolTip(QString(tr("0%1%2")).arg(i+1).arg(j+k));
                }
                else
                {
                    pin[i][j]->setText(tr("%1").arg(j+1));
                    pin[i][j]->setToolTip(QString(tr("0%1%2")).arg(i+1).arg(j+1));
                }
            }
            else
            {
                if(j>7)
                {
                    k=k-2;
                    pin[i][j]->setText(tr("%1").arg(j+k));
                    pin[i][j]->setToolTip(QString(tr("%1%2")).arg(i+1).arg(j+k));
                }
                else
                {
                    pin[i][j]->setText(tr("%1").arg(j+1));
                    pin[i][j]->setToolTip(QString(tr("%1%2")).arg(i+1).arg(j+1));

                }
            }

            if(j<8)
            {
                pin[i][j]->setGeometry(4,j*40+4,38,38);
            }
            else
            {
                pin[i][j]->setGeometry(44,(j-8)*40+4,38,38);
            }
        }

    }
}
/*创建一个透明QFrame覆盖在每一路上，使得第一次点击时buttun不会有反应*/
void ActuatorTesting::showRED(int pre,int next)
{
    for(int i=pre;i<next;i++)
    {

        framecover[i]->show();
        framecover[i]->setGeometry((i-pre)*110+30,30,95,333);
        framecover[i]->setStyleSheet("QPushButton{background:transparent;border:2px solid white}");
        connect(framecover[i], SIGNAL(clicked()), this, SLOT(FrameSelectedFirst_slot()));
        framecover[i]->setToolTip(QString::fromUtf8("%1").arg(i+1));
    }
    if(startSelectRoad==1)
    {
        for( int cc=0;cc<16;cc++)
        {
            if(cc<pre||cc>=next)//0------6-------12
            {
                framecover[cc]->hide();
            }
            else
            {
                framecover[cc]->show();
            }
        }
    }
    if(PreRoad==0)
    {
        pre_i=1;
        istring=1;
        frame[0]->setStyleSheet("ClickedQLabel{border:2px solid red}");
    }
    if(PreRoad==6)
    {

        pre_i=7;
        istring=7;
        frame[6]->setStyleSheet("ClickedQLabel{border:2px solid red}");
    }
    if(PreRoad==12)
    {

        pre_i=13;
        istring=13;
        frame[12]->setStyleSheet("ClickedQLabel{border:2px solid red}");
    }
}
/**先显示选中路的红色**/
void ActuatorTesting::FrameSelectedFirst_slot()
{
    click=(QPushButton *)sender();
    click->setStyleSheet("QPushButton{background:transparent;border:2px solid white}");
    click->setGeometry(0,10,30,30);
    istring=click->toolTip().toInt();
    if(pre_i!=istring)
    {
        if(pre_i<17)
        {
            frame[pre_i-1]->setStyleSheet("ClickedQLabel{border:2px solid white}");
            framecover[pre_i-1]->setGeometry((pre_i-1-PreRoad)*110+30,30,95,333);

        }
        frame[istring-1]->setStyleSheet("ClickedQLabel{border:2px solid red}");
    }
    pre_i=istring;
}

//单刀老化
void ActuatorTesting::modtest_Act_pb_clicked()
{
    qDebug()<<istring<<mod;
    Ram.SendData[0]=0x12;
    Ram.SendData[1]=0xA2;
    Ram.SendData[2]=0x03;
    Ram.SendData[3]=istring;
    Ram.SendData[4]=mod;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
}

/*选针器显示*/
void ActuatorTesting::show_actuator(int pre,int next)
{
    if(startSelectRoad==1)
    {
        for(int i=0;i<16;i++)
        {
            for(int j=0;j<16;j++)
            {
                if(i<pre||i>=next)   //0  6  12  16
                {
                    frame[i]->hide();

                    ActuatotRoad[i]->hide();
                }
                else
                {
                    frame[i]->show();

                    ActuatotRoad[i]->show();
                }
            }
        }
        for(int i=0;i<16;i++)
        {
            if(i!=pre+1)
            {
                frame[i]->setStyleSheet("ClickedQLabel{border:2px solid white}");
            }
        }
    }

    for(int i=pre;i<next;i++)
    {
        ActuatotRoad[i]->setGeometry(45+110*(i-pre),8,71,31);
        frame[i]->setGeometry((i-pre)*110+35,36,86,326);
    }
}
/*上一页*/
void ActuatorTesting::on_previouspage_Act_pb_clicked()
{
    if(startSelectRoad==0)
        showGUI(6,16);
    startSelectRoad=1;
    PreRoad=PreRoad-6;
    Road=Road-6;
    if(PreRoad>=0&&PreRoad<6)
    {
        show_actuator(PreRoad,Road);
        showRED(PreRoad,Road);
    }

    if(PreRoad==6)
    {

        show_actuator(PreRoad,Road);
        showRED(PreRoad,Road);
    }
    if(PreRoad<0)
    {
        PreRoad=0;
        Road=6;
    }
}
/*下一页*/
void ActuatorTesting::on_nextpage_Act_pb_clicked()
{
    if(startSelectRoad==0)
        showGUI(6,16);
    startSelectRoad=1;
    PreRoad=PreRoad+6;   //  0  6 12
    Road=Road+6;         //  6  12 18
    if(PreRoad<12)
    {

        show_actuator(PreRoad,Road);
        showRED(PreRoad,Road);
    }
    if(PreRoad==12)
    {

        show_actuator(12,16);
        showRED(PreRoad,16);
    }
    if(PreRoad>12)
    {
        PreRoad=12;
        Road=18;
    }
    qDebug()<<PreRoad<<Road;

}

/**某路选中**单刀动作**/
void ActuatorTesting::showframe()
{
    sum++;
    if(sum>1)
        sum=0;
    QPushButton *buttun=new QPushButton();
    buttun=(QPushButton *)sender();
    istring=(buttun->toolTip()).left(2).toInt();//lushu
    mod= buttun->toolTip().remove(0,2).toInt();//daohao
    if(ManualAndDebugSwitch==0)
    {
        Ram.SendData[0]=0x12;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=0x01;
        Ram.SendData[3]=0x01;
        Ram.SendData[4]=istring;
        Ram.SendData[5]=mod;
        Ram.SendData[6]=sum;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
    if(ManualAndDebugSwitch==1)
    {
        Ram.SendData[0]=0x12;
        Ram.SendData[1]=0xA3;
        Ram.SendData[2]=0x01;
        Ram.SendData[3]=istring;
        Ram.SendData[4]=mod;
        Ram.SendData[5]=sum;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();

    }
}

//单路上下manual[0]槽函数
void ActuatorTesting::singleupdown_slot()
{
    single_updown_or_single_debug=1;
    SingleUpDown();

    sum_single_Act++;
    if( sum_single_Act>1)
        sum_single_Act=0;

    Ram.SendData[0]=0x12;
    Ram.SendData[1]=0xA1;
    Ram.SendData[2]=0x01;
    Ram.SendData[4]=istring;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    if(sum_single_Act==0)
        Ram.SendData[3]=0x03;//全下
    else
        Ram.SendData[3]=0x02;//全上
    Ram.UI_to_RTC();


}
//全路上下manual[2]槽函数
void ActuatorTesting::wholeupdown_slot()
{
    if(startSelectRoad==0)
        showGUI(6,16);
    startSelectRoad=1;
    whole_updown_or_whole_debug=1;
    WholeUpDown();
    sum_whole_Act++;
    if( sum_whole_Act>1)
        sum_whole_Act=0;
    Ram.SendData[0]=0x12;
    Ram.SendData[1]=0xA1;
    Ram.SendData[2]=0x02;
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    if(sum_whole_Act==0)
        Ram.SendData[3]=0x03;//全下
    else
        Ram.SendData[3]=0x02;//全上
    Ram.UI_to_RTC();
    qDebug()<<sum_whole_Act;
}
/**单路上下自定义函数**/
int ActuatorTesting::SingleUpDown()
{
    if(Sum_single==0)
    {
        UpDown_single=true;
    }
    else
    {
        UpDown_single=!UpDown_single;
    }
    if(single_updown_or_single_debug==1)
    {
        if(UpDown_single)
        {
            manual[0]->setText(QString::fromUtf8("单\n路\n上"));
            ui->info_pb->setText(QString::fromUtf8("单路上..."));
        }
        else
        {
            manual[0]->setText(QString::fromUtf8("单\n路\n下"));
            ui->info_pb->setText(QString::fromUtf8("单路下..."));
        }
        single_updown_or_single_debug=0;
    }

    Sum_single++;
    for(int k=0;k<16;k++)
    {
        pin[istring-1][k]->setChecked(UpDown_single);
    }
    return istring;
}
/*全路上下自定义函数*/
void ActuatorTesting::WholeUpDown()
{
    if(Sum_whole==0)
    {
        UpDown_whole=true;
    }

    else
    {
        UpDown_whole=!UpDown_whole;
    }
    if(whole_updown_or_whole_debug==1)
    {
        if(UpDown_whole)
        {
            manual[2]->setText(QString::fromUtf8("全\n路\n上"));
            ui->info_pb->setText(QString::fromUtf8("全路上..."));
        }
        else
        {
            manual[2]->setText(QString::fromUtf8("全\n路\n下"));
            ui->info_pb->setText(QString::fromUtf8("全路下..."));
        }
        whole_updown_or_whole_debug=0;
    }
    Sum_whole++;
    // qDebug()<<UpDown_whole<<Sum_whole;
    for(int i=0;i<16;i++)
    {
        //frame[i]->setStyleSheet("ClickedQLabel{background:transparent;border:2px solid red}");
        for(int j=0;j<16;j++)
            pin[i][j]->setChecked(UpDown_whole);
    }
}
//单路正隔manual[1]
void ActuatorTesting::singleinterval_slot()
{
    if(Sum_singinterval==0)
        interval_single=true;
    else
        interval_single=!interval_single;
    Sum_singinterval++;
    if(interval_single)
    {
        manual[1]->setText(QString::fromUtf8("单\n路\n正\n隔"));
        ui->info_pb->setText(QString::fromUtf8("单路正隔..."));
    }
    else
    {
        manual[1]->setText(QString::fromUtf8("单\n路\n反\n隔"));
        ui->info_pb->setText(QString::fromUtf8("单路反隔..."));
    }
    for(int k=7;k>=1;k=k-2)
    {
        pin[istring-1][k]->setChecked(interval_single);
        pin[istring-1][k+8]->setChecked(interval_single);
        pin[istring-1][k-1]->setChecked(!interval_single);
        pin[istring-1][k+7]->setChecked(!interval_single);
    }
    sum_interval_Act++;
    if(sum_interval_Act>1)
        sum_interval_Act=0;
    Ram.SendData[0]=0x12;
    Ram.SendData[1]=0xA1;
    Ram.SendData[2]=0x01;
    Ram.SendData[4]=istring;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    qDebug()<<sum_interval_Act;
    if(sum_interval_Act==0)
        Ram.SendData[3]=0x04;//正隔
    else
        Ram.SendData[3]=0x05;//反隔
    Ram.UI_to_RTC();
}
//全路正隔manual[3]
void ActuatorTesting::wholeinterval_slot()
{
    if(startSelectRoad==0)
        showGUI(6,16);
    startSelectRoad=1;
    if(Sum_wholeinterval==0)
    {
        interval_whole=true;
    }
    else
    {
        interval_whole=!interval_whole;
    }
    Sum_wholeinterval++;
    if(interval_whole)
    {
        manual[3]->setText(QString::fromUtf8("全\n路\n正\n隔"));
        ui->info_pb->setText(QString::fromUtf8("全路正隔..."));
    }
    else
    {
        manual[3]->setText(QString::fromUtf8("全\n路\n反\n隔"));
        ui->info_pb->setText(QString::fromUtf8("全路反隔..."));
    }

    for(int i=0;i<16;i++)
    {
        for(int k=7;k>=1;k=k-2)
        {
            pin[i][k]->setChecked(interval_whole);
            pin[i][k+8]->setChecked(interval_whole);
            pin[i][k-1]->setChecked(!interval_whole);
            pin[i][k+7]->setChecked(!interval_whole);
        }
    }
    sum_interval_whole_Act++;
    if(sum_interval_whole_Act>1)
        sum_interval_whole_Act=0;
    Ram.SendData[0]=0x12;
    Ram.SendData[1]=0xA1;
    Ram.SendData[2]=0x02;
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    //qDebug()<<sum_interval_whole_Act;
    if(sum_interval_whole_Act==0)
        Ram.SendData[3]=0x04;//正隔
    else
        Ram.SendData[3]=0x05;//反隔
    Ram.UI_to_RTC();

}
/*手动测试*/
void ActuatorTesting::manualtest_slot()
{
    ui->info_pb->setText(QString::fromUtf8("手动测试..."));

    manual[0]->setText(QString::fromUtf8("单\n路\n上\n下"));
    manual[1]->setText(QString::fromUtf8("单\n路\n正\n隔"));
    manual[2]->setText(QString::fromUtf8("全\n路\n上\n下"));
    manual[3]->setText(QString::fromUtf8("全\n路\n正\n隔"));
    manual[7]->setText(QString::fromUtf8("返\n\n回"));
    for(int man=0;man<8;man++)
    {
        manual[man]->setStyleSheet("QPushButton{border-radius:10;background-color: rgb(100, 100, 100);font-size:18px;color:rgb(255, 255, 0);}");
    }

    whichWidget=1;
    ManualAndDebugSwitch=0;//dandaoshiqizuoyong
    manualtest_Act_pb->hide();
    circulatetest_Act_pb->hide();
    debugtest_Act_pb->hide();
    empty1_Act_pb->hide();
    empty2_Act_pb->hide();
    plus_Act_pb->hide();
    minus_Act_pb->hide();
    back_Act_pb->hide();
    for(int i=0;i<6;i++)//循环测试和侦错测试模块的控件
    {
        circulation[i]->hide();
        debug[i]->hide();
    }
    for(int i=0;i<8;i++)
    {
        manual[i]->show();
    }
}
/**循环测试槽函数**/
void ActuatorTesting::circulatetest_slot()
{
    whichWidget=2;
    circulation[0]->setText(QString::fromUtf8("单\n路\n老\n化"));
    circulation[1]->setText(QString::fromUtf8("全\n路\n老\n化"));
    circulation[2]->setText(QString::fromUtf8("单\n路\n循\n环"));
    circulation[3]->setText(QString::fromUtf8("全\n路\n循\n环"));
    circulation[5]->setText(QString::fromUtf8("返\n\n回"));
    modtest_Act_pb->setVisible(true);
    for(int cir=0;cir<6;cir++)
    {
        circulation[cir]->setStyleSheet("QPushButton{border-radius:10;background-color: rgb(100, 100, 100);font-size:18px;color:rgb(255, 255, 0);}");
    }

    manualtest_Act_pb->hide();
    circulatetest_Act_pb->hide();
    debugtest_Act_pb->hide();
    empty1_Act_pb->hide();
    empty2_Act_pb->hide();
    back_Act_pb->hide();
    for(int i=0;i<6;i++)
        circulation[i]->show();
}
/*侦错测试槽函数*/
void ActuatorTesting::debugtest_slot()
{
    // whichWidget=3;
    //    debug[0]->setText(QString::fromUtf8("单\n路\n侦\n错"));
    //    debug[1]->setText(QString::fromUtf8("全\n路\n侦\n错"));
    //    debug[2]->setText(QString::fromUtf8("侦\n错\n记\n路"));
    //   // debug[3]->setText(QString::fromUtf8("侦\n错\n清\n零"));
    //    debug[4]->setText(QString::fromUtf8("自\n我\n学\n习"));
    //    debug[5]->setText(QString::fromUtf8("返\n\n回"));
    //    for(int de=0;de<6;de++)
    //    {
    //         debug[de]->setStyleSheet("QPushButton{border-radius:10;background-color: rgb(100, 100, 100);font-size:20px;color:rgb(255, 255, 0);}");
    //    }

    //    ManualAndDebugSwitch=1;
    //    manualtest_Act_pb->hide();
    //    circulatetest_Act_pb->hide();
    //    debugtest_Act_pb->hide();
    //    empty1_Act_pb->hide();
    //    empty2_Act_pb->hide();

    //    back_Act_pb->hide();
    //    for(int i=0;i<6;i++)
    //        debug[i]->show();

}
/*加速*/
void ActuatorTesting::plus_slot()
{
    ui->info_pb->setText(QString::fromUtf8("加  速..."));
    int plus=ui->speed_lb->text().toInt();
    plus++;
    if(plus<10)
    {
        ui->speed_lb->setText(QString::number(plus));
    }
    else
    {
        plus=9;
        ui->speed_lb->setText(QString::number(plus));
    }
    Ram.SendData[0]=0x12;
    Ram.SendData[1]=0xA5;
    Ram.SendData[2]=plus;
    Ram.SendData[3]=0x00;
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();


}
/*减速*/
void ActuatorTesting::minus_slot()
{
    ui->info_pb->setText(QString::fromUtf8("减  速..."));
    int minus=ui->speed_lb->text().toInt();
    minus--;
    if(minus>0)
    {
        ui->speed_lb->setText(QString::number(minus));

    }
    else
    {
        minus=1;
        ui->speed_lb->setText(QString::number(minus));
    }
    Ram.SendData[0]=0x12;
    Ram.SendData[1]=0xA5;
    Ram.SendData[2]=minus;
    Ram.SendData[3]=0x00;
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
}
/**返回**回归编织状态**/
void ActuatorTesting::back_slot()
{
    timer_act->stop();
    Ram.SendData[0]=0x12;
    Ram.SendData[1]=0xAA;
    Ram.SendData[2]=0x00;
    Ram.SendData[3]=0x00;
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
    Ram.to_TestingMain_slot();
    this->close();
}
/*返回自手动测试*/
void ActuatorTesting::backfrommanul_slot()
{
    whichWidget=0;
    for(int i=0;i<8;i++)
        manual[i]->hide();
    manualtest_Act_pb->show();
    circulatetest_Act_pb->show();
    debugtest_Act_pb->show();
    empty1_Act_pb->show();
    empty2_Act_pb->show();
    plus_Act_pb->show();
    minus_Act_pb->show();
    back_Act_pb->show();
    modtest_Act_pb->setVisible(false);

}
/*返回循环测试*/
void ActuatorTesting::backfromcirculation_slot()
{
    whichWidget=0;
    for(int i=0;i<6;i++)
        circulation[i]->hide();
    manualtest_Act_pb->show();
    circulatetest_Act_pb->show();
    debugtest_Act_pb->show();
    empty1_Act_pb->show();
    empty2_Act_pb->show();
    // plus_Act_pb->show();
    // minus_Act_pb->show();
    back_Act_pb->show();
    modtest_Act_pb->setVisible(false);
}
/*返回自侦错测试*/
void ActuatorTesting::backfromdebug_slot()
{
    whichWidget=0;
    for(int i=0;i<6;i++)
        debug[i]->hide();
    manualtest_Act_pb->show();
    circulatetest_Act_pb->show();
    debugtest_Act_pb->show();
    empty1_Act_pb->show();
    empty2_Act_pb->show();
    // plus_Act_pb->show();
    //minus_Act_pb->show();
    back_Act_pb->show();
}
/*单路侦错*/
void ActuatorTesting::singledebug_slot()
{
    single_updown_or_single_debug=0;
    SingleUpDown();
    Ram.SendData[0]=0x12;
    Ram.SendData[1]=0xA3;
    Ram.SendData[1]=0x02;
    Ram.SendData[1]=istring;
    Ram.SendData[1]=0x00;
    Ram.SendData[1]=0x00;
    Ram.SendData[1]=0x00;
    Ram.SendData[1]=0x00;
    Ram.SendData[1]=0x00;
    Ram.UI_to_RTC();
}
/*全路侦错*/
void ActuatorTesting::wholedebug_slot()
{
    if(startSelectRoad==0)
        showGUI(6,16);
    startSelectRoad=1;
    whole_updown_or_whole_debug=0;
    WholeUpDown();
    Ram.SendData[0]=0x12;
    Ram.SendData[1]=0xA3;
    Ram.SendData[1]=0x03;
    Ram.SendData[1]=0xFF;
    Ram.SendData[1]=0x00;
    Ram.SendData[1]=0x00;
    Ram.SendData[1]=0x00;
    Ram.SendData[1]=0x00;
    Ram.SendData[1]=0x00;
    Ram.UI_to_RTC();
}
/*侦错记录*/
void ActuatorTesting::debuglist_slot()
{

}
/*侦错清零*/
void ActuatorTesting::debugreset_slot()
{

}
/*自我学习*/
void ActuatorTesting::selflearning_slot()
{

}
/*单路老化*/
void ActuatorTesting::singlematuring_slot()
{

    //if(singleOld)
    //{
    ui->info_pb->setText(QString::fromUtf8("单路老化..."));
    //  circulation[0]->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:red}");
    Ram.SendData[0]=0x12;
    Ram.SendData[1]=0xA2;
    Ram.SendData[2]=0x01;
    Ram.SendData[3]=0x01;
    Ram.SendData[4]=istring;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
    //        singleOld=false;
    //    }
    //    else
    //    {
    //          circulation[0]->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:rgb(255, 255, 0);}");
    //        singleOld=true;
    //    }


}
/*全路老化*/
void ActuatorTesting::wholematuring_slot()
{

    //    if(whloeOld)
    //    {
    ui->info_pb->setText(QString::fromUtf8("全路老化..."));
    //        circulation[1]->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:red}");
    //        whloeOld=false;
    Ram.SendData[0]=0x12;
    Ram.SendData[1]=0xA2;
    Ram.SendData[2]=0x02;
    Ram.SendData[3]=0x01;
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
    //    }
    //    else
    //    {
    //        circulation[1]->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:rgb(255, 255, 0);}");
    //        whloeOld=true;
    //    }

}
/*单路循环*/
void ActuatorTesting::singlecirculation_slot()
{

    //    if(singleCir)
    //    {
    ui->info_pb->setText(QString::fromUtf8("单路循环..."));
    //circulation[2]->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:red}");
    //singleCir=false;
    Ram.SendData[0]=0x12;
    Ram.SendData[1]=0xA2;
    Ram.SendData[2]=0x01;
    Ram.SendData[3]=0x02;
    Ram.SendData[4]=istring;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
    //    }
    //    else
    //    {
    //        singleCir=true;
    //        circulation[2]->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:white}");
    //    }

}
/*全路循环*/
void ActuatorTesting::wholecirculation_slot()
{
    //    if(wholeCir)
    //    {
    circulation[3]->setEnabled(false);
    ui->info_pb->setText(QString::fromUtf8("全路循环..."));
    //        circulation[3]->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:red}");
    //        wholeCir=false;
    Ram.SendData[0]=0x12;
    Ram.SendData[1]=0xA2;
    Ram.SendData[2]=0x02;
    Ram.SendData[3]=0x02;
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
    circulation[3]->setEnabled(true);
    //    }
    //    else
    //    {
    //         wholeCir=true;
    //         circulation[3]->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:white}");
    //    }
}
void  ActuatorTesting::touchShowTime_ActuatorTesting()
{
    //    if(touchLaunch==1)
    //    {
    //        touchShow_ActuatorTesting->setGeometry(0,0,1,1);
    //        touchStart_ActuatorTesting=true;
    //    }
    //    else if(touchLaunch==0)
    //    {
    //        touchShow_ActuatorTesting->setGeometry(0,0,800,480);
    //        touchStart_ActuatorTesting=false;
    //    }
}
