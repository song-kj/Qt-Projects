#include "codesettingmain.h"
#include "ui_codesettingmain.h"
#include "Windows/Codesetting/selsetcode.h"

CodesettingMain::CodesettingMain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CodesettingMain)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::FramelessWindowHint);
    Ram.to_PatternWoven_slot();
}

CodesettingMain::~CodesettingMain()
{
    delete ui;
}

void CodesettingMain::on_btn_selcode_clicked()
{
    SelSetCode *code=new SelSetCode(0x01);
    code->exec();
}

void CodesettingMain::on_btn_valvecode_clicked()
{
    SelSetCode *code=new SelSetCode(0x02);
    code->exec();
}

void CodesettingMain::on_btn_motorcode_clicked()
{
    SelSetCode *code=new SelSetCode(0x03);
    code->exec();
}

void CodesettingMain::on_btn_back_clicked()
{
    this->close();
}
