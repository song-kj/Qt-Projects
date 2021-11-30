#ifndef MYHEADER_H
#define MYHEADER_H
#include "Communication/doubleportram.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <QDateTime>
#include "paramteroperation.h"
#define Path_parameterfile "/xrfile/Parameter.er"  //参数文件路径
#define Path_parameterfile_backup "/xrfile/Parameter.backup"  //参数文件路径
#define Path_loacl "/home/root/"   //本地花型文件存放目录
#define Path_myusb "/media/myusb/"  //USB目录
#define Path_machine "/xrfile/Machine.ini"  //机器配置文件路径
#define Path_hirepurchase "/xrfile/SystemMaintenance.txt"//系统维护文件路径
#define Path_patternUCO "/home/root/temp.uco"  //uco文件路径
#define parameter_Count 1260     //参数总个数

//实时参数
struct Real_timeParameter
{
    quint8 knitstate;
    quint8 head_position;   //机头位置
    quint16 curpulse;       //当前脉冲
    quint16 curstep;        //当前步段
    quint16 totalstep;      //总步段
    quint16 curspeed;       //当前速度
    quint16 setspeed;       //设定速度
    quint16 curcycle;       //当前循环
    quint16 totalcycle;     //总循环
    quint16 curneedle;      //当前针位
    quint16 curneedleTemp;  //当前针位（临时）
    quint16 totalneedel;    //总针数
    quint16 curangle;       //当前角度
    quint16 curcircle;      //当前圈
    quint16 totalcircle;    //总圈数
    quint16 progress;       //当前进度
    quint16 curyield;       //当前产量
    quint16 sumyield;       //总产量
    quint16 tempera;        //温度
    qint16 motorstate[9];   //电机状态值
    QString singletime;     //单件耗时
    QString machineNeedleType;//机器型号
    int zero;               //零位基准
    bool isRunnging;
};

//机器参数
struct machine_Parameter
{
    quint16 needletype;                 //1.机型针号
    quint16 totalneedles;               //2.总针数
    quint16 actuator_interval;          //3.选针器间隔
    quint16 machine_zeor_needle;        //4.机器零位针
    quint16 actuator_zero_needle;       //5.选针零位针
    quint16 actuator_num;               //6.选针器个数
    quint16 total_puls;                 //7.总脉冲数
    quint16 actuator_adjust;            //8.选针调整
    quint16 fixed_needle;               //9.固定出针
    quint16 jog_speed;                  //10.点动转速
    quint16 speed_limt;                 //11.限速度值
    quint16 max_speed;                  //12.最大速度
    quint16 yield_current;              //13.完成产量
    quint16 yield_setted;               //14.目标产量
    quint16 oil_interval;               //15.喷油间隔
    quint16 oil_times;                  //16.喷油次数
    quint16 oil_interval2;              //17.喷油间隔2
    quint16 oil_times2;                 //18.喷油次数2
    quint16 actuatorType;               //19.选针器类型
    quint16 zero_impluse;               //20.零位脉冲
    quint16 valve_start_time;           //21.气阀启动时间
    quint16 valve_pressure;             //22.气阀高压比
    quint16 alarm_setted[8];            //23-30.报警设置（屏蔽）
    quint16 actuator_position[16];      //31-46.选针器位置
    quint16 motor_adjust[8];            //47-54.压针微调值
    quint16 motor_step[200];
    quint16 motor_degree[200];
    quint16 motor_number[200];
    quint16 motor_position[200];
    quint16 speed_step[70];
    quint16 speed_value[70];
    quint16 speed_ramp[70];
    quint16 eco_stepstart[50];
    quint16 eco_stepend[50];
    quint16 eco_value[50];

    quint16 runInLimitSpeed;            //磨合期限速度
    quint16 heatTemperature;            //热机温度
    quint16 heatLimtspeed;              //热机限速
    quint16 heatTemperature2;           //热机温度2
    quint16 heatLimtspeed2;             //热机限速2
    quint16 heatClothNum;               //热机件数
    quint16 heatFalg;                   //热机标志
    quint16 fixZeroOffset;              //机械零位偏差角度
    quint16 temp[38];                   //预留
};

//系统维护
struct system_Maintenance{
    quint8 idInt[10];
    int remaining_days;
    QString id;
    QString equipmentID;                //设备识别码
    QString starttime;
    QString endtime;
    QString CompanyCode;
    QString CompanyCodeAsciiLength;
    QString CompanyCodeAscii;
    QString version_MainCTL;
    QString version_SmtrDRV;
    QString version_VlvDRVL;
    QString version_ActaDRV;
    bool expired;
    bool setting_mark;
};

//机器磨合
struct RunningIn{
    bool mark;
    int days;
    quint32 startTime;
};

//编织状态
enum KnitState{
    knitNULL,
    knitRest,
    knitting,
    knitFlat,
};

enum KeyData{
    key_null,
    key_f0,
    key_f1,
    key_f2,
    key_f3,
    key_f4,
    key_f5,
    key_f6,
    key_f7,
    key_f8,
    key_f9,
    key_uphead,
    key_uphead_cancel,
    key_down_head,
    key_down_head_cancel,
    key_1,
    key_2,
    key_3,
    key_4,
    key_5,
    key_6,
    key_7,
    key_8,
    key_9,
    key_0,
    key_point,
    key_plus_minus,
    key_light,
    key_oil,
    key_delete,
    key_esc,
    key_left,
    key_right,
    key_up,
    key_down,
    key_ok,
    key_insert,
    key_pgup,
    key_pgdown,
    key_end,
    key_menu,
    key_onoff,
    key_help,
    key_insert1,
    key_insert2,
    key_insert3,
    key_insert4,
    key_insert5
};

extern DoublePortRam Ram;
extern Real_timeParameter realtime_parameter;//实时参数
extern machine_Parameter machine_Pars;//机器参数
extern ParamterOperation para_operation;//参数文件操作类
extern system_Maintenance sys_maintenance;
extern KeyData serialPortKey;    //串口按键键值
extern KeyData serialPortKeyTemp;   //
extern KnitState knitState;      //编织状态
extern RunningIn runningIn;      //机器磨合参数
extern int startToTransmitPara;  //参数传递标志
extern int machineSize;
extern QDateTime lastTime;
extern QString projectCOName;//CO工程名称
extern bool checkInterval();
extern bool zeroSetted;
extern bool updateUI;
extern bool b_sendPYF_Par;
extern QString b_headerUp_Down;
#endif // MYHEADER_H
