#include "dialogsettings.h"
#include "ui_dialogsettings.h"
#include <QSerialPortInfo>
#include <QDebug>


QString dataComPortNum, userComPortNum;

DialogSettings::DialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);
    QString portName;
    QString portDescription;

    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
        {
        portName = serialPortInfo.portName();
        portDescription = serialPortInfo.description();
            if (portDescription.contains("User", Qt::CaseInsensitive))
            {
                ui->comboBox_UserUART->addItem(serialPortInfo.portName());
            }

            if (portDescription.contains("Data", Qt::CaseInsensitive))
            {
                ui->comboBox_DataPort->addItem(serialPortInfo.portName());
            }
        }
}

DialogSettings::~DialogSettings()
{
    delete ui;
}

void DialogSettings::on_buttonBox_accepted()
{
     dataComPortNum = ui->comboBox_DataPort->currentText();
     userComPortNum = ui->comboBox_UserUART->currentText();
}


QString DialogSettings::getUserComPortNum()
{
  return ui->comboBox_UserUART->currentText();
}

QString DialogSettings::getDataComPortNum()
{
  return ui->comboBox_DataPort->currentText();
}

double DialogSettings::getHeartWfm_yAxisMax()
{
    return ui->doubleSpinBox_BreathWfm_yAxisMax->value();
}

double DialogSettings::getBreathWfm_yAxisMax()
{
    return ui->doubleSpinBox_BreathWfm_yAxisMax->value();
}


QString DialogSettings::setUserComPortNum(QString portNum)
{
    ui->comboBox_UserUART->addItem(portNum);
    return ui->comboBox_UserUART->currentText();
}

QString DialogSettings::setDataComPortNum(QString portNum)
{
  ui->comboBox_DataPort->addItem(portNum);
  return ui->comboBox_DataPort->currentText();
}


void DialogSettings::on_pushButton_Ok_clicked()
{

}
