#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPrinter>
#include <QDate>
#include <QPrintDialog>
#include <QDesktopWidget>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(printLabel()));
    QDate day = QDate::currentDate();
    ui->spinBoxMonth->setValue(day.month());
    ui->spinBoxYear->setValue(day.year());

    ui->buttonBox->setFocus();


    QPoint center = qApp->desktop()->screenGeometry(this).center();

    QRect r = rect();
    r.moveCenter(center);
    move(r.topLeft());

}

void MainWindow::printLabel()
{
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("/tmp/labels.pdf");

    QPrintDialog printDialog(&printer);
    if (printDialog.exec() != QDialog::Accepted) {
        return;
    }
    painter.begin(&printer);

    QDate day;
    int month = ui->spinBoxMonth->value();
    int year = ui->spinBoxYear->value();
    int printed = 0;
    for(int i=1;i<=31;i++)
    {
        day.setDate(year,month,i);
        if(!day.isValid() || day.dayOfWeek() == 7)
            continue;
        printed++;

        if(printed%9 ==0)
        {
            printer.newPage();
            printed = 1;
        }

        QString text;

        if( day.dayOfWeek()  < 5)
        {
            text = "Semanal - ";
            text.append(QString::number(day.dayOfWeek()));
            text.append(" - ");
            text.append(day.toString("dd/MM/yyyy"));
        }
        else
        {
            text = "Full - ";
            text.append(QString::number(day.dayOfWeek()-4));
            text.append(" - ");
            text.append(day.toString("dd/MM/yyyy"));
        }


        int pos_x = 0;
        int factor = (printed-1)/2;

        int pos_y = 235*factor;

        if(printed%2 != 0)
            pos_x = 0;
        else
            pos_x = 265;


        QFont f = painter.font();
        f.setFamily("Helvetica [Cronyx]");
        f.setPointSize(15);
        f.setBold(true);
        painter.setFont(f);
        QRect rectOne( pos_x,5+pos_y,265,235 );
        painter.setBrush( Qt::NoBrush);
        painter.setPen(Qt::NoPen);
        painter.drawRect(rectOne);
        painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap,    Qt::RoundJoin));
        painter.drawText(rectOne,Qt::AlignHCenter,text);
        QRect rect( pos_x,0+pos_y,265,235 );
        painter.drawRect(rect);
        if(pos_x == 0)
            painter.drawLine(pos_x,35+pos_y,265,35+pos_y);
        else
            painter.drawLine(pos_x,35+pos_y,265*2,35+pos_y);
        painter.drawText(rect,Qt::AlignCenter,text);

    }
    painter.end();
    close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
