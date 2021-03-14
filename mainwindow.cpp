#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    door = ui->door_L;
    pAnimation = new QPropertyAnimation(this, "color");
    pAnimation->setDuration(5000);
    pAnimation->setStartValue(QColor(240, 0, 0));
    pAnimation->setKeyValueAt(0.5,QColor(240, 0, 200));
    pAnimation->setEndValue(QColor(240, 0, 0));
    pAnimation->setLoopCount(-1);

    display = ui->display;

    Clock_button = ui->Clock_btn;
    Stop_button = ui->Stop_btn;
    Start_button = ui->Start_btn;
    Power_button = ui->Power_btn;
    Mode_button = ui->Mode_btn;
    Defrost_button = ui->Defrost_btn;


    QStateMachine *mac = new QStateMachine;
    QState *s1 = new QState( );
    mac->addState(s1);


    // s11, s12, s13 are grouped in s1
    s11 = new QState(s1);
    s12 = new QState(s1);
    s13 = new QState(s1);
    //stop button
    s1->addTransition(Stop_button, SIGNAL(clicked( )), s1);

    //clock: s11: idle / s12: set hour / s13: set minute
    s11->addTransition(Clock_button, SIGNAL(clicked( )), s12);
    s12->addTransition(Clock_button, SIGNAL(clicked( )), s13);
    s13->addTransition(Clock_button, SIGNAL(clicked( )), s11);

    connect(s11, SIGNAL(entered()), this, SLOT(setIdle()));
    connect(s12, SIGNAL(entered()), this, SLOT(setHour()));
    connect(s13, SIGNAL(entered()), this, SLOT(setMin()));

    s1->setInitialState(s11);


    start = new QState(s1);
    start->addTransition(Start_button, SIGNAL(clicked( )), start);
    s11->addTransition(Start_button, SIGNAL(clicked( )), start); //auto start
    connect(start, SIGNAL(entered()), this, SLOT(startCooking()));
    connect(Start_button, SIGNAL(clicked()), this, SLOT(autoAdd())); //add 60s

    //power button
    pStatus = new QState(s1);
    pDuration = new QState(s1);
    s11->addTransition(Power_button, SIGNAL(clicked( )), pStatus);
    pStatus->addTransition(Power_button, SIGNAL(clicked( )), pDuration);
    pDuration->addTransition(Start_button, SIGNAL(clicked( )), start);
    connect(pStatus, SIGNAL(entered()), this, SLOT(setPower()));
    connect(pDuration, SIGNAL(entered()), this, SLOT(setPowerDuration()));
    //mode button
    mStatus = new QState(s1);
    mDuration = new QState(s1);
    s11->addTransition(Mode_button, SIGNAL(clicked( )), mStatus);
    mStatus->addTransition(Mode_button, SIGNAL(clicked( )), mDuration);
    mDuration->addTransition(Start_button, SIGNAL(clicked( )), start);
    connect(mStatus, SIGNAL(entered()), this, SLOT(setMode()));
    connect(mDuration, SIGNAL(entered()), this, SLOT(setModeDuration()));
    //defrost button
    dDuration = new QState(s1);
    s11->addTransition(Defrost_button, SIGNAL(clicked( )), dDuration);
    dDuration->addTransition(Start_button, SIGNAL(clicked( )), start);
    connect(dDuration, SIGNAL(entered()), this, SLOT(setDefrostDuration()));

    //timeout backto initial state
    start->addTransition(this,SIGNAL(cookfinished()),s11);

    mac->setInitialState(s1);
    mac->start( );


    dial = ui->dial;

    connect(dial, SIGNAL(sliderMoved(int)),this,SLOT(setDial(int)));

    timer = new QTimer(this);
    clock = new QTimer(this);
    connect(clock, SIGNAL(timeout()), this, SLOT(countup()));
    connect(timer, SIGNAL(timeout()), this, SLOT(countdown()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

//clock
void MainWindow:: countup(){
    display = ui->display;
    s++;
    if(s == 60){
        m++;
        s=0;
    }
    if(m == 60){
        h++;
        m=0;
    }
    if(h == 24){
        //clock->stop();
    }
    QString h_str = QString("%1").arg(h, 2, 10, QLatin1Char('0'));
    QString m_str = QString("%1").arg(m, 2, 10, QLatin1Char('0'));
    QString s_str = QString("%1").arg(s, 2, 10, QLatin1Char('0'));
    display->setText(h_str + ":" + m_str + ":" + s_str);

}
//duration
void MainWindow:: countdown(){
    QString str = QString("%1").arg(d, 2, 10, QLatin1Char('0'));
    d --;
    display->setText("Cooking Time Left: " + str);
    if(d < 0){
        timer->stop();
        emit cookfinished();
    }
}

//clock
void MainWindow:: setHour(){
    clock->stop();
    display = ui->display;
    display->setText("Set Hour: 0"); //go to dial
    Start_button->setEnabled(false);
    Power_button->setEnabled(false);
    Mode_button->setEnabled(false);
    Defrost_button->setEnabled(false);

}
void MainWindow:: setMin(){
    clock->stop();
    display->setText("Set Minute: 0"); //go to dial
    Start_button->setEnabled(false);
    Power_button->setEnabled(false);
    Mode_button->setEnabled(false);
    Defrost_button->setEnabled(false);
}
void MainWindow:: setPower(){
    clock->stop();
    display->setText("Set Power: 0 %"); //go to dial
    Clock_button->setEnabled(false);
    Power_button->setEnabled(true);
    Mode_button->setEnabled(false);
    Defrost_button->setEnabled(false);
    Start_button->setEnabled(false);
    Stop_button->setEnabled(true);

}
void MainWindow:: setPowerDuration(){
    display->setText("Set Duration: 00"); //go to dial
    Clock_button->setEnabled(false);
    Power_button->setEnabled(false);
    Mode_button->setEnabled(false);
    Defrost_button->setEnabled(false);
    Start_button->setEnabled(true);
    Stop_button->setEnabled(true);
}
void MainWindow:: setMode(){
    clock->stop();
    display->setText("Set Mode: microwave"); //go to dial
    Clock_button->setEnabled(false);
    Power_button->setEnabled(false);
    Mode_button->setEnabled(true);
    Defrost_button->setEnabled(false);
    Start_button->setEnabled(false);
    Stop_button->setEnabled(true);
}
void MainWindow:: setModeDuration(){
    display->setText("Set Duration: 00"); //go to dial
    Clock_button->setEnabled(false);
    Power_button->setEnabled(false);
    Mode_button->setEnabled(false);
    Defrost_button->setEnabled(false);
    Start_button->setEnabled(true);
    Stop_button->setEnabled(true);
}
void MainWindow:: setDefrostDuration(){
    clock->stop();
    display->setText("Set Weight: 00"); //go to dial
    Clock_button->setEnabled(false);
    Power_button->setEnabled(false);
    Mode_button->setEnabled(false);
    Defrost_button->setEnabled(false);
    Start_button->setEnabled(true);
    Stop_button->setEnabled(true);
}

void MainWindow:: startCooking(){
    door->setStyleSheet("QLabel { background-color : red; }");
    pAnimation->start();

    QString str = QString("%1").arg(d, 2, 10, QLatin1Char('0'));
    display->setText("Cooking Time Left: " + str);
    clock->stop();
    timer->start(1000); // 每隔1s

    Clock_button->setEnabled(false);
    Power_button->setEnabled(false);
    Mode_button->setEnabled(false);
    Defrost_button->setEnabled(false);
    Start_button->setEnabled(true);
    Stop_button->setEnabled(true);
}


void MainWindow:: setDial(int dial_val){
    QString str = QString("%1").arg(dial_val, 2, 10, QLatin1Char('0'));

    if (s11->active()) { //idle

    }
    else if (s12->active()) { //set hour
        dial->setMinimum(00);
        dial->setMaximum(24);
        display->setText("Set Hour: " + str);
        h = dial_val;
    }else if (s13->active()) { //set minute
        dial->setMinimum(00);
        dial->setMaximum(60);
        m = dial_val;
        display->setText("Set Minute: " + str);
    }else if(pStatus->active()){ //set power
        dial->setMinimum(00);
        dial->setMaximum(100);
        display->setText("Set Power: " + str + " %");
    }else if(pDuration->active()){
        dial->setMinimum(00);
        dial->setMaximum(60);
        d = dial_val;
        display->setText("Set Duration: " + str + " s");
    }else if(mStatus->active()){
        dial->setMinimum(0);
        dial->setMaximum(2);
        switch(dial_val){
        case 0:
            display->setText("Set Mode: microwave");
            break;
        case 1:
            display->setText("Set Mode: grill");
            break;
        case 2:
            display->setText("Set Mode: microwave+grill");
            break;
        }
    }else if(mDuration->active()){
        dial->setMinimum(00);
        dial->setMaximum(60);
        display->setText("Set Duration: " + str + " s");
        d = dial_val;
    }else if(dDuration->active()){
        dial->setMinimum(50);
        dial->setMaximum(5000);
        display->setText("Set Weight: " + str + " g");
        d = dial_val / 10;
    }
}

void MainWindow:: setIdle(){
    door->setStyleSheet("QLabel { background-color : darkgrey; }");
    pAnimation->stop();
    timer->stop();
    clock->start(1000); //display clock count up
    d = 60;
    Clock_button->setEnabled(true);
    Power_button->setEnabled(true);
    Mode_button->setEnabled(true);
    Defrost_button->setEnabled(true);
    Start_button->setEnabled(true);
    Stop_button->setEnabled(true);
}

void MainWindow:: autoAdd(){
    if(start->active()){
        d += 60;
    }
}
void MainWindow::setColor(QColor color){
    m_color = color;
    QString strQSS = QString("background-color: rgba(%1, %2, %3);").arg(m_color.red()).arg(m_color.green()).arg(m_color.blue());
    door->setStyleSheet(strQSS);
}
