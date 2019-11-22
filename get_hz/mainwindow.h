#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QFont"
#include "qtimer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);
private slots:
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

    void on_radioButton_5_clicked();

    void on_radioButton_4_clicked();

    void on_radioButton_6_clicked();

    void on_pushButton_8_clicked();

private:
    void grab_pixel(QImage *img);
private:
    Ui::MainWindow *ui;
    QFont m_font;
    char code[2];
    char en_code;
    int  en_cnt;
    int qh;
    int wh;
    int m_timer;
    int font_w;
    int font_h;
    bool draw_flag;
    FILE *fp;
    int offsetx;
    int offsety;
    bool en_cn_flag;
};

#endif // MAINWINDOW_H
