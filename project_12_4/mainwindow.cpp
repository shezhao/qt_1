#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //QTimer* timer = new QTimer(this);
    //修改定时器精度
    //timer->setTimerType(QTimer::PreciseTimer);
    //按钮的点击事件

    // connect(ui->loopBtn,&QPushButton::clicked,this,[=]()
    //         {
    //             //启动定时器
    //             if(timer->isActive())
    //             {
    //                 timer->stop();//关闭定时器
    //                 ui->loopBtn->setText("开始");

    //             }
    //             else
    //             {
    //                 ui->loopBtn->setText("关闭");
    //                 timer->start(1000);//1000ms == 1s
    //             }
    //         } );

    // connect(timer,&QTimer::timeout,this,[=]()
    //         {
    //     QTime tm = QTime::currentTime();
    //     QString  tmstr= tm.toString("hh:mm:ss.zzz");
    //     //设置要显示的时间


    //     ui->curTime->setText(tmstr);

    //         });
    //timer = new QTimer(this);
    //connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);
    //timer->start(1000);  // 设置计时间隔为1秒

    secondsLabel = new QLabel("点击开始4s后模拟输出", this);
    secondsLabel->setGeometry(10, 10, 200, 30);

    connect(ui->ButtonFile,&QToolButton::clicked,this,[=](){
        QString path =QFileDialog::getOpenFileName(this,"OPENFILE","c:/","(*.txt)");
        ui->lineEditfile->setText(path);
            ui->textEditfile->clear();
        if(!(path).isEmpty()){
            QFile file(path);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                qWarning("Could not open file");
            }
            QTextStream in(&file);

            while (!in.atEnd())
            {
                QString character = in.readAll();
                ui->textEditfile->setText(character);
            }
            file.close();
            timer = new QTimer(this);

            connect(ui->loopBtn,&QPushButton::clicked,this,[=]()
                {
                        //启动定时器
                        if(timer->isActive())
                        {
                            timer->stop();//关闭定时器
                            ui->loopBtn->setText("开始");

                        }
                        else
                        {
                            ui->loopBtn->setText("暂停");
                            timer->start(1000);//1000ms == 1s
                        }
                });

            connect(timer, &QTimer::timeout, this, [=](){
                // 更新剩余秒数并更新界面
                remainingSeconds--;
                secondsLabel->setText("请耐心等待: " + QString::number(remainingSeconds)+"s ");

                if (remainingSeconds == 0)
                {
                    remainingSeconds=5;
                    timer->stop();
                    // 计时器倒计时结束后执行其他操作
                    QFile file(path);
                    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                        secondsLabel->setText("读取失败！！！");
                    }
                    int num=0;//初始化LCD number

                    QTextStream in(&file);
                    while (!in.atEnd()) {

                        QString line = in.readLine();
                        for (QChar c : line) {
                            typeChar(c);
                            num++;
                            ui->lcdNumber->display(num);//每有一个字符加一
                        }

                        // 模拟按下Enter键
                        INPUT enter;
                        enter.type = INPUT_KEYBOARD;
                        enter.ki.wVk = VK_RETURN;
                        enter.ki.dwFlags = 0;
                        enter.ki.time = 0;
                        enter.ki.dwExtraInfo = 0;
                        SendInput(1, &enter, sizeof(INPUT));
                        enter.ki.dwFlags = KEYEVENTF_KEYUP;
                        SendInput(1, &enter, sizeof(INPUT));

                        QThread::msleep(200);
                    }

                    secondsLabel->setText("完成!");
                    ui->loopBtn->setText("开始");
                    timer->stop();
                    file.close();
                }
            });
        }
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow:: typeChar(const QChar& c)
{

    bool isUpper = c.isUpper();

    if (isUpper) {
        INPUT shift;
        shift.type = INPUT_KEYBOARD;
        shift.ki.wVk = VK_SHIFT;
        shift.ki.dwFlags = 0;
        SendInput(1, &shift, sizeof(INPUT));
    }

    INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = 0;
    input.ki.wScan = c.unicode();
    input.ki.dwFlags = KEYEVENTF_UNICODE;
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;
    SendInput(1, &input, sizeof(INPUT));
    input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));

    if (isUpper) {
        INPUT shift;
        shift.type = INPUT_KEYBOARD;
        shift.ki.wVk = VK_SHIFT;
        shift.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &shift, sizeof(INPUT));
    }

    QTest::qWait(150);

}


