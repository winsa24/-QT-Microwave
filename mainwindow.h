#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QState>
#include <QDial>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStateMachine>
#include <QDebug>
#include <QTimer>
#include <QPropertyAnimation>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void startCook();

private slots:
    void countup();
    void countdown();

    void setIdle();
    void setHour();
    void setMin();
    void setDial(int);

    void setPower();
    void setPowerDuration();
    void setMode();
    void setModeDuration();
    void setDefrostDuration();
    void startCooking();
    void autoAdd();
private:
    int s = 0, m = 0, h = 0, d = 60; //second, minute, hour, duration

signals:
    void cookfinished();

private:
    Ui::MainWindow *ui;
    QState *s11,*s12,*s13;
    QState *start;
    QState *idle;
    QState *pStatus, *pDuration, *mStatus, *mDuration, *dDuration;
    QDial *dial;
    QTimer *timer, *clock;
    QLineEdit *display;
    QLabel *door;
    QPushButton *Clock_button, *Stop_button, *Start_button, *Power_button, *Mode_button, *Defrost_button;

private:
    void setColor (QColor color);
    QColor color(){
      return Qt::black;
    }

private:
    QLabel *m_pLabel;
    QColor m_color;
    QPropertyAnimation *pAnimation;
};
#endif // MAINWINDOW_H
