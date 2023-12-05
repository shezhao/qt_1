#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QTest>
#include <QThread>
#include <QDebug>
#include <qdebug.h>
#include <Windows.h>
#include <QGuiApplication>
#include <QTimer>
#include <QLabel>
#include <QDialog>
#include <QFileDialog>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void read_in();
    void typeChar(const QChar& c);

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QLabel *secondsLabel;
    int remainingSeconds = 5;
};
#endif // MAINWINDOW_H
