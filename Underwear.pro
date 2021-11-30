#-------------------------------------------------
#
# Project created by QtCreator 2016-01-26T13:16:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RifaUWM_UI
TEMPLATE = app


SOURCES += main.cpp\
        patternwoven.cpp \
    Windows/Settings/parametersetting.cpp \
    Windows/Settings/zerolevelsetting.cpp \
    Windows/Settings/yieldsetting.cpp \
    Windows/Testings/airvalvetesting.cpp \
    Windows/Testings/steppermotortesting.cpp \
    Windows/Testings/inputtesting.cpp \
    Windows/Testings/outputtesting.cpp \
    Windows/FileOperations/fileoperations.cpp \
    Windows/SystemMaintenance/systemmaintence.cpp \
    Windows/Settings/settingmain.cpp \
    Windows/Testings/testingmain.cpp \
    Windows/Menu/menu.cpp \
    Windows/Testings/actuatortesting.cpp \
    Communication/doubleportram.cpp \
    warningpatternwoven.cpp \
    warningactuator.cpp \
    Windows/Settings/machtypeselected.cpp \
    Windows/SystemMaintenance/updateselect.cpp \
    Windows/SystemMaintenance/controlui.cpp \
    Tool/KeyBoard/keybd.cpp \
    shuttlechoose.cpp \
    flatcloth.cpp \
    Windows/SystemMaintenance/ui.cpp \
    Windows/Settings/paraoutin.cpp \
    Communication/noanswer.cpp \
    Windows/FileOperations/sumneedleerror.cpp \
    Windows/Settings/motorsaveornot.cpp \
    Windows/FileOperations/resetornot.cpp \
    Windows/SystemMaintenance/systemlog.cpp \
    Windows/SystemMaintenance/timesetting.cpp \
    continueweave.cpp \
    Windows/SystemMaintenance/dialogjiesuo.cpp \
    Windows/SystemMaintenance/dialogsyssetiing.cpp \
    hirepurchase.cpp \
    mymessagebox.cpp \
    hirethread.cpp \
    Tool/KeyBoard/softwear.cpp \
    Windows/Settings/parametersaveinfo.cpp \
    Windows/WIFI/patternoperation.cpp \
    Windows/WIFI/wifimode.cpp \
    clickedqlabel.cpp \
    error/errorcode.cpp \
    error/neworder.cpp \
    QCom/qextserialport.cpp \
    Windows/WIFI/xrwifi.cpp \
    Windows/Settings/qluoansetting.cpp \
    Windows/Settings/qluoanangle.cpp \
    Windows/BTSRsetting/btsrmain.cpp \
    Windows/BTSRsetting/btsrsetid.cpp \
    Tool/KeyBoard/dialogcalculator.cpp \
    Windows/BTSRsetting/btsrparameter.cpp \
    Windows/BTSRsetting/btsrstudy.cpp \
    Windows/BTSRsetting/btsrstartstudy.cpp \
    Windows/Codesetting/codesettingmain.cpp \
    Windows/Codesetting/selsetcode.cpp \
    Windows/Settings/warningsetting.cpp \
    Windows/SystemMaintenance/warninghistory.cpp \
    Windows/SystemMaintenance/upgradefeedback.cpp \
    Windows/SystemMaintenance/xrfile.cpp \
    paramteroperation.cpp \
    Windows/Testings/faultdetection.cpp \
    textticker.cpp \
    Windows/Settings/runninginsetting.cpp \
    machineconfigure.cpp \
    modifyco.cpp \
    Windows/Settings/stitchadjust.cpp \
    Windows/SystemMaintenance/versionquery.cpp

win32{
    SOURCES +=QCom/qextserialport_win.cpp
}

unix{
    SOURCES +=QCom/qextserialport_unix.cpp
}

HEADERS  += patternwoven.h \
    Windows/Settings/parametersetting.h \
    Windows/Settings/zerolevelsetting.h \
    Windows/Settings/yieldsetting.h \
    Windows/Testings/airvalvetesting.h \
    Windows/Testings/steppermotortesting.h \
    Windows/Testings/inputtesting.h \
    Windows/Testings/outputtesting.h \
    Windows/FileOperations/fileoperations.h \
    Windows/SystemMaintenance/systemmaintence.h \
    Windows/Settings/settingmain.h \
    Windows/Testings/testingmain.h \
    Windows/Menu/menu.h \
    Windows/Testings/actuatortesting.h \
    Communication/doubleportram.h \
    warningpatternwoven.h \
    warningactuator.h \
    Windows/Settings/machtypeselected.h \
    Windows/SystemMaintenance/updateselect.h \
    Windows/SystemMaintenance/controlui.h \
    Tool/KeyBoard/keybd.h \
    shuttlechoose.h \
    flatcloth.h \
    Windows/SystemMaintenance/ui.h \
    Windows/Settings/paraoutin.h \
    Communication/noanswer.h \
    Windows/FileOperations/sumneedleerror.h \
    Windows/Settings/motorsaveornot.h \
    tucos.h \
    Windows/FileOperations/resetornot.h \
    Windows/SystemMaintenance/systemlog.h \
    Windows/SystemMaintenance/timesetting.h \
    continueweave.h \
    Windows/SystemMaintenance/dialogjiesuo.h \
    Windows/SystemMaintenance/dialogsyssetiing.h \
    hirepurchase.h \
    mymessagebox.h \
    hirethread.h \
    Tool/KeyBoard/softwear.h \
    Windows/Settings/parametersaveinfo.h \
    Windows/WIFI/patternoperation.h \
    Windows/WIFI/wifimode.h \
    Tool/KeyBoard/softwear.h \
    clickedqlabel.h \
    error/errorcode.h \
    error/neworder.h \
    QCom/qextserialport.h \
    QCom/qextserialport_global.h \
    QCom/qextserialport_p.h \
    Windows/WIFI/xrwifi.h \
    Windows/Settings/qluoansetting.h \
    Windows/Settings/qluoanangle.h \
    Windows/BTSRsetting/btsrmain.h \
    Windows/BTSRsetting/btsrsetid.h \
    Tool/KeyBoard/dialogcalculator.h \
    Windows/BTSRsetting/btsrparameter.h \
    Windows/BTSRsetting/btsrstudy.h \
    Windows/BTSRsetting/btsrstartstudy.h \
    Windows/Codesetting/codesettingmain.h \
    Windows/Codesetting/selsetcode.h \
    Windows/Settings/warningsetting.h \
    Windows/SystemMaintenance/warninghistory.h \
    Windows/SystemMaintenance/upgradefeedback.h \
    Windows/SystemMaintenance/xrfile.h \
    myheader.h \
    paramteroperation.h \
    Windows/Testings/faultdetection.h \
    textticker.h \
    Windows/Settings/runninginsetting.h \
    machineconfigure.h \
    modifyco.h \
    Windows/Settings/stitchadjust.h \
    Windows/SystemMaintenance/versionquery.h

FORMS    += patternwoven.ui \
    Windows/Settings/parametersetting.ui \
    Windows/Settings/zerolevelsetting.ui \
    Windows/Settings/yieldsetting.ui \
    Windows/Testings/airvalvetesting.ui \
    Windows/Testings/steppermotortesting.ui \
    Windows/Testings/inputtesting.ui \
    Windows/Testings/outputtesting.ui \
    Windows/FileOperations/fileoperations.ui \
    Windows/SystemMaintenance/systemmaintence.ui \
    Windows/Settings/settingmain.ui \
    Windows/Testings/testingmain.ui \
    Windows/Menu/menu.ui \
    Windows/Testings/actuatortesting.ui \
    warningpatternwoven.ui \
    warningactuator.ui \
    Windows/Settings/parasavemessage.ui \
    Windows/Settings/machtypeselected.ui \
    Windows/SystemMaintenance/updateselect.ui \
    Windows/SystemMaintenance/controlui.ui \
    Tool/KeyBoard/keybd.ui \
    shuttlechoose.ui \
    flatcloth.ui \
    Windows/SystemMaintenance/ui.ui \
    Windows/Settings/paraoutin.ui \
    Communication/noanswer.ui \
    Windows/FileOperations/sumneedleerror.ui \
    Windows/Settings/motorsaveornot.ui \
    Windows/FileOperations/resetornot.ui \
    Windows/SystemMaintenance/systemlog.ui \
    Windows/SystemMaintenance/timesetting.ui \
    continueweave.ui \
    Windows/SystemMaintenance/dialogjiesuo.ui \
    Windows/SystemMaintenance/dialogsyssetiing.ui \
    hirepurchase.ui \
    mymessagebox.ui \
    Windows/Settings/parametersaveinfo.ui \
    Windows/WIFI/wifimode.ui \
    Windows/WIFI/patternoperation.ui \
    Tool/KeyBoard/softwear.ui \
    error/neworder.ui \
    Windows/Settings/qluoansetting.ui \
    Windows/Settings/qluoanangle.ui \
    Windows/BTSRsetting/btsrmain.ui \
    Windows/BTSRsetting/btsrsetid.ui \
    Tool/KeyBoard/dialogcalculator.ui \
    Windows/BTSRsetting/btsrparameter.ui \
    Windows/BTSRsetting/btsrstudy.ui \
    Windows/BTSRsetting/btsrstartstudy.ui \
    Windows/Codesetting/codesettingmain.ui \
    Windows/Codesetting/selsetcode.ui \
    Windows/Settings/warningsetting.ui \
    Windows/SystemMaintenance/warninghistory.ui \
    Windows/SystemMaintenance/upgradefeedback.ui \
    Windows/Testings/faultdetection.ui \
    Windows/Settings/runninginsetting.ui \
    machineconfigure.ui \
    modifyco.ui \
    Windows/Settings/stitchadjust.ui \
    Windows/SystemMaintenance/versionquery.ui

RESOURCES += \
    Resources/picture.qrc

unix{
    LIBS+=../RIFA_UnderWear/libtucoS.a
}


DISTFILES += \
    �����޸ļ�¼.txt

