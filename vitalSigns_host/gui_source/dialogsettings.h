#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>
#include <QSerialPort>



namespace Ui {
class DialogSettings;
}

class DialogSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSettings(QWidget *parent = 0);
    Ui::DialogSettings *ui;
    ~DialogSettings();

private:

private slots:
    void on_buttonBox_accepted();
//    void on_pushButton_clicked();
    void on_pushButton_Ok_clicked();

public slots:
   QString getUserComPortNum();
   QString getDataComPortNum();
   QString setUserComPortNum(QString portNum);
   QString setDataComPortNum(QString portNum);
   double getHeartWfm_yAxisMax();
   double getBreathWfm_yAxisMax();
};

#endif // DIALOGSETTINGS_H
