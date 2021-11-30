#include "softwear.h"
#include "ui_softwear.h"
#include <QDebug>
Softwear::Softwear(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Softwear)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
   // this->setAttribute(Qt::WA_TranslucentBackground,true);//背景透明
    //this->setStyleSheet("background-color: rgb(255, 255, 127)");
    this->setGeometry(8,219,781,261);

    connect(ui->one_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->two_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->three_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->four_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->five_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));

    connect(ui->six_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->seven_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->eight_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));

    connect(ui->nine_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->zero_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));

    connect(ui->a_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->b_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));

    connect(ui->c_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->d_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));

    connect(ui->e_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->g_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->f_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));

    connect(ui->h_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->i_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->j_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->k_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));

    connect(ui->l_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->m_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->n_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));

    connect(ui->o_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->p_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->q_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));

    connect(ui->r_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->s_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->t_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));

    connect(ui->u_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->v_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->w_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->x_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));

    connect(ui->y_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->z_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->Plus_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->Minus_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));

    connect(ui->Oppose_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->LeftBrace_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->RightBrace_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->Or_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));

    connect(ui->Colon_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->DoubleQuotes_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->LessThan_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));
    connect(ui->MoreThan_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));

    connect(ui->Question_btn,SIGNAL(clicked(bool)),this,SLOT(dealWithClick()));

     button = new QPushButton();
     ABool = false;
     ShiftBool = false;

     AEnterOrNot = true;
     ShiftEnterOrNot = true;
}

Softwear::~Softwear()
{
    delete button;
    delete ui;
}
void Softwear::dealWithClick()
{
    button=(QPushButton*)sender();
    int flag = button->whatsThis().toInt();
    switch (flag) {

    case 1: number = StrTransform("~"); break;

    case 2: number = StrTransform("1"); break;
    case 3: number = StrTransform("2"); break;
    case 4: number = StrTransform("3"); break;
    case 5: number = StrTransform("4"); break;
    case 6: number = StrTransform("5"); break;
    case 7: number = StrTransform("6"); break;
    case 8: number = StrTransform("7"); break;
    case 9: number = StrTransform("8"); break;
    case 10:number = StrTransform("9"); break;
    case 11:number = StrTransform("0"); break;

    case 12:number = StrTransform("-"); break;
    case 13:number = StrTransform("+"); break;
    case 14:number = StrTransform("{") ; break;
    case 15:number = StrTransform("}"); break;
    case 16:number = StrTransform("|"); break;
    case 17:number = StrTransform(":"); break;
    case 18:number = StrTransform("\"");break;
    case 19:number = StrTransform("<"); break;
    case 20:number = StrTransform(">"); break;
    case 21:number = StrTransform("?"); break;

    case 22:number = AaTransform('q'); break;
    case 23:number = AaTransform('w'); break;
    case 24:number = AaTransform('e'); break;
    case 25:number = AaTransform('r'); break;
    case 26:number = AaTransform('t'); break;
    case 27:number = AaTransform('y'); break;
    case 28:number = AaTransform('u'); break;

    case 29:number = AaTransform('i'); break;
    case 30:number = AaTransform('o'); break;
    case 31:number = AaTransform('p'); break;
    case 32:number = AaTransform('a'); break;
    case 33:number = AaTransform('s'); break;
    case 34:number = AaTransform('d'); break;
    case 35:number = AaTransform('f'); break;
    case 36:number = AaTransform('g'); break;
    case 37:number = AaTransform('h'); break;
    case 38:number = AaTransform('j'); break;
    case 39:number = AaTransform('k'); break;
    case 40:number = AaTransform('l'); break;
    case 41:number = AaTransform('z'); break;
    case 42:number = AaTransform('x'); break;

    case 43:number = AaTransform('c'); break;
    case 44:number = AaTransform('v'); break;
    case 45:number = AaTransform('b'); break;
    case 46:number = AaTransform('n'); break;
    case 47:number = AaTransform('m'); break;
    default:break;
    }

    showNumber=showNumber.append(number);
    ui->input_le->setText(showNumber);
}
/*开启大写*/
void Softwear::on_A_btn_2_clicked()
{
    if(AEnterOrNot)
    {
        AEnterOrNot = false;
        ABool = true;
        ui->A_btn_2->setStyleSheet("QPushButton{"
                                   "font: 24pt;"
                                   "background-color: rgb(107, 107, 107);"
                                   "border-color: rgb(46, 67, 255);	"
                                   "color:rgb(255,0,0);"
                                   "border-style:outside;"
                                   "border-radius:8px"
                                   "}"
                                   "QPushButton::pressed{"
                                   "background-color: rgb(50, 50, 50);"
                                   "border-color: rgb(20, 26, 138);"
                                   "}"
                                   "QPushButton::disabled{"
                                   "background-color: rgb(46, 67, 255);"
                                   "border-color: rgb(46, 67, 255);"
                                   "}");
    }
    else
    {
        AEnterOrNot = true;
        ABool = false;
        ui->A_btn_2->setStyleSheet("QPushButton{"
                                   "font: 24pt;"
                                   "background-color: rgb(107, 107, 107);"
                                   "border-color: rgb(46, 67, 255);	"
                                   "color:rgb(255,255,255);"
                                   "border-style:outside;"
                                   "border-radius:8px"
                                   "}"
                                   "QPushButton::pressed{"
                                   "background-color: rgb(50, 50, 50);"
                                   "border-color: rgb(20, 26, 138);"
                                   "}"
                                   "QPushButton::disabled{"
                                   "background-color: rgb(46, 67, 255);"
                                   "border-color: rgb(46, 67, 255);"
                                   "}");
    }
}
/*Shift键*/
void Softwear::on_Shift_btn_clicked()
{
    if(ShiftEnterOrNot)
    {
        ShiftEnterOrNot = false;
        ShiftBool = true;
        ui->Shift_btn->setStyleSheet("QPushButton{"
                                   "font: 24pt;"
                                   "background-color: rgb(107, 107, 107);"
                                   "border-color: rgb(46, 67, 255);	"
                                   "color:rgb(255,0,0);"
                                   "border-style:outside;"
                                   "border-radius:8px"
                                   "}"
                                   "QPushButton::pressed{"
                                   "background-color: rgb(50, 50, 50);"
                                   "border-color: rgb(20, 26, 138);"
                                   "}"
                                   "QPushButton::disabled{"
                                   "background-color: rgb(46, 67, 255);"
                                   "border-color: rgb(46, 67, 255);"
                                   "}");
    }
    else
    {
        ShiftEnterOrNot = true;
        ShiftBool = false;
        ui->Shift_btn->setStyleSheet("QPushButton{"
                                   "font: 24pt;"
                                   "background-color: rgb(107, 107, 107);"
                                   "border-color: rgb(46, 67, 255);	"
                                   "color:rgb(255,255,255);"
                                   "border-style:outside;"
                                   "border-radius:8px"
                                   "}"
                                   "QPushButton::pressed{"
                                   "background-color: rgb(50, 50, 50);"
                                   "border-color: rgb(20, 26, 138);"
                                   "}"
                                   "QPushButton::disabled{"
                                   "background-color: rgb(46, 67, 255);"
                                   "border-color: rgb(46, 67, 255);"
                                   "}");
    }
}
/*Enter键*/
void Softwear::on_Enter_btn_clicked()
{
    emit setNumber(showNumber);
    reject();
}
/*Esc键*/
void Softwear::on_Esc_btn_clicked()
{
    reject();
}
/*Back键*/
void Softwear::on_Back_btn_clicked()
{
    showNumber= showNumber.left(showNumber.size()-1);
    ui->input_le->setText(showNumber);
}
QString Softwear::AaTransform(QChar Aa)
{
    QString str;
    if(ABool)
    {
       return str.append(Aa.toUpper());
    }
    else
    {
        return str.append(Aa);
    }
}
QString Softwear::StrTransform(QString sh)
{
    if(ShiftBool)
    {
       if(sh == "~")
       {
           sh = "`";
       }
       else if(sh == "1")
       {
           sh = "!";
       }
       else if(sh == "2")
       {
           sh = "@";
       }
       else if(sh == "3")
       {
           sh = "#";
       }
       else if(sh == "4")
       {
           sh = "$";
       }
       else if(sh == "5")
       {
           sh = "%";
       }
       else if(sh == "6")
       {
           sh = "^";
       }
       else if(sh == "7")
       {
           sh = "&";
       }
       else if(sh == "8")
       {
           sh = "*";
       }
       else if(sh == "9")
       {
           sh = "(";
       }
       else if(sh == "0")
       {
           sh = ")";
       }
       else if(sh == "-")
       {
           sh = "_";
       }
       else if(sh == "+")
       {
           sh = "=";
       }
       else if(sh == "{")
       {
           sh = "[";
       }

       else if(sh == "}")
       {
           sh = "]";
       }
       else if(sh == "|")
       {
           sh = "\\";
       }
       else if(sh == ":")
       {
           sh = ";";
       }
       else if(sh == "\"")
       {
           sh = "'";
       }
       else if(sh == "<")
       {
           sh = ",";
       }
       else if(sh == ">")
       {
           sh = ".";
       }
       else if(sh == "?")
       {
           sh = "/";
       }
    }
        return sh;
}
