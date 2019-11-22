#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qimage.h"
#include "qpainter.h"
#include "qfontdialog.h"
#include "qdebug.h"
#include "qrgb.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qh=16;
    wh=1;
    font_w = 16;
    font_h = 16;
    m_font.setFamily("SimSun");
    m_font.setPixelSize(14); //设置字号32,以像素为单位
    m_font.setWeight (50); //设置字型,不加粗
    m_font.setItalic(false); //设置字型,不倾斜
    m_font.setUnderline(false); //设置字型,无下划线
    draw_flag = false;
    fp = fopen("data.bin","wb+");
    code[0]=0xa0+qh;
    code[1]=0xa0+wh;
    this->ui->radioButton->setChecked(true);
    this->ui->radioButton_4->setChecked(true);
    offsetx=0;
    offsety=0;
    en_cn_flag = false;
    en_code='A';
    this->setWindowTitle(QString::fromLocal8Bit("字模提取"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *p)
{
    QWidget *f = this->ui->widget;
    int x1 = f->x()+(f->width()-font_w)/2;
    int y1 = f->y()+(f->height()-font_h)/2;
    {
        int w=font_w;
        int h=font_h;
        QImage img(w,h,QImage::Format_ARGB32);
        QPainter pt;
        pt.begin(&img);
            pt.setFont(m_font);
            pt.eraseRect(img.rect());
            img.fill(Qt::white);
            QRect r = img.rect();
            r.translate(offsetx,offsety);
            if(!en_cn_flag)
                pt.drawText(r, QString::fromLocal8Bit(code));
            else
                pt.drawText(r, QString("%1").arg(en_code));
        pt.end();

        QPainter painter(this);
        QPixmap map(QPixmap::fromImage(img));
        painter.drawPixmap(x1,y1,w,h,map);
        if(draw_flag){
            grab_pixel(&img);
            draw_flag = false;
        }
    }
}


void MainWindow::on_pushButton_clicked()
{
    m_timer = startTimer(300);
    en_cnt=32;
}

void MainWindow::timerEvent(QTimerEvent *t)
{
    if(!en_cn_flag){//中文
        code[0]=qh+0xa0;
        code[1]=wh+0xa0;
        wh++;
        if(wh>94){
            qh++;
            wh=1;
            //killTimer(m_timer);
//            if(fp){
//                fflush(fp);
//                fclose(fp);
//            }
        }
        draw_flag=true;
        update();
        if(qh==56){
            killTimer(m_timer);
            if(fp){
                fflush(fp);
                fclose(fp);
            }
        }
    }else{//英文
        en_code=en_cnt;
        en_cnt++;
        if(en_cnt==128){
             killTimer(m_timer);
             if(fp){
                 fflush(fp);
                 fclose(fp);
                 qDebug()<<"end!";
             }
        }
        draw_flag=true;
        update();
    }
}

void MainWindow::grab_pixel(QImage *img)
{
    QRect rect = img->rect();
    unsigned char xch=0;
    for(int i=0;i<rect.height();i++){
        QRgb *line = (QRgb *)img->scanLine(i);
        xch=0;
        int cnt=0;
        for(int j=0;j<rect.width();j++){
            xch = xch<<1;
            if((qRed(line[j]) == 0) && (qGreen(line[j])==0) && (qBlue(line[j])==0)){
                //ch[n] |= 0x01;
                xch |= 0x01;
            }
            cnt++;
            if(cnt==8 && fp)
            {
                //char str[10]={0};
                //sprintf(str,"%02x",xch);
                //fwrite(str,1,strlen(str),fp);
                fwrite(&xch,1,1,fp);
                cnt=0;
                xch=0;
            }
        }
    }
}


void MainWindow::on_pushButton_3_clicked()
{
    bool ok;
    if(!en_cn_flag)
        m_font = QFontDialog::getFont(&ok, QFont(QString::fromLocal8Bit("微软雅黑"), 20), this);
    else
        m_font = QFontDialog::getFont(&ok, QFont(QString("Arial"), 20), this);

    update();
}

void MainWindow::on_radioButton_6_clicked()
{
    font_w = 8;
    font_h = 16;
    update();
}

void MainWindow::on_radioButton_clicked()
{
    font_w = 16;
    font_h = 16;
    update();
}

void MainWindow::on_radioButton_2_clicked()
{
    font_w = 32;
    font_h = 32;
    update();
}

void MainWindow::on_radioButton_3_clicked()
{
    font_w = 48;
    font_h = 48;
    update();
}

void MainWindow::on_pushButton_2_clicked()
{
    killTimer(m_timer);
    if(fp){
        fflush(fp);
        fclose(fp);
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    if(offsety>0)
        offsety--;
    update();
}

void MainWindow::on_pushButton_4_clicked()
{
    if(offsetx>0)
        offsetx--;
    update();
}

void MainWindow::on_pushButton_5_clicked()
{
    if(offsetx<font_w)
        offsetx++;
    update();
}

void MainWindow::on_pushButton_7_clicked()
{
    if(offsety<font_h)
        offsety++;
    update();
}

void MainWindow::on_radioButton_5_clicked()
{
    en_cn_flag = true;
    update();
}

void MainWindow::on_radioButton_4_clicked()
{
    en_cn_flag = false;
    update();
}



void MainWindow::on_pushButton_8_clicked()
{
    bool ok;
    font_w = this->ui->lineEdit->text().toInt(&ok,10);
    font_h = this->ui->lineEdit_2->text().toInt(&ok,10);
    update();
}
