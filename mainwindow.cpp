#include "mainwindow.h"
#include "ui_mainwindow.h"


//#include <stdint.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <unistd.h>
//#include <errno.h>

#include <wiringPiSPI.h>

#include <QTime>
#include <QDate>
#include <QTimer>
#include <QDebug>
#include <cmath>


#define CHANNEL 0

void MainWindow::matrixInit()
{
    spiRegVal(MAX7219_REG_SHUTDOWN, 0, -1);
    spiRegVal(MAX7219_REG_SCANLIMIT, 7, -1);    // show all 8 digits
    spiRegVal(MAX7219_REG_DECODEMODE, 0, -1);   // use matrix (not digits)
    spiRegVal(MAX7219_REG_DISPLAYTEST, 0, -1);  // no display test
    spiRegVal(MAX7219_REG_SHUTDOWN, 1, -1);
    spiRegVal(MAX7219_REG_INTENSITY, 15, -1);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_changeCounter = 0;

    //set the number of LED-panels
    m_numberOfMatrices = 4;

    //if (wiringPiSPISetup(CHANNEL, 1000000) < 0) {

    if (wiringPiSPISetup(CHANNEL, 10000000) < 0) {
        fprintf (stderr, "SPI Setup failed: %s\n", strerror (errno));
        exit(errno);
    }

    //init the font for displaying the numbers
    initFont();
    matrixInit();
    clearDotMatrix();

    //test
   spiRegVal(1,1,1);
   spiRegVal(2,2,2);
   spiRegVal(3,3,3);
   spiRegVal(5,4,4);

   clearDotMatrix();

    spiRegVal(MAX7219_REG_INTENSITY, 14, -1);

    //refresh time every 3 seconds
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimerFinished()));
    timer->start(5000);



/*
while(true)
{
    int intensityCounter = 0;
    for (int i = 100; i < 350; ++i)
    {
        displayNumber(i);


        intensityCounter++;
        if(intensityCounter >14)
        {
            intensityCounter = 0;
        }

        sleep(1);
    }

}

*/
//    uint8_t bufTest[4];

//    bufTest[0] = 0;
//    bufTest[1] = 0;

//    bufTest[2] = 1;
//    bufTest[3] = binToDec("111");


//  bufTest[2] = 0;
//   bufTest[3] = 0;
//    bufTest[4] = 0;
//    bufTest[5] = 0;

//    wiringPiSPIDataRW(CHANNEL, bufTest, 4);
//    wiringPiSPIDataRW(CHANNEL, bufTest, 4);

    //spiRegVal(1, binToDec("11"));
   // spiRegVal(MAX7219_REG_NOOP,MAX7219_REG_NOOP);

   // spiRegVal(1, binToDec("1"));

//END_test

   // gpsManager *myGpsManager= new gpsManager(1);

  //  connect(myGpsManager, SIGNAL(speedAviable(int)), this, SLOT(gpsSpeedReceived(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::spiRegVal(uint8_t reg, uint8_t val, int matrix)
{

    //send to all matrices
    if(matrix == -1)
    {
        for (int i = 0; i < m_numberOfMatrices; ++i)
        {
             uint8_t buf[2];
             buf[0] = reg;
             buf[1] = val;
             wiringPiSPIDataRW(CHANNEL, buf, 2);
        }
    }
    else
    {
        u_int8_t buf[m_numberOfMatrices * 2];

        //intit buffer with zeros
        for (int i = 0; i < m_numberOfMatrices*2; ++i)
        {
            buf[i] =0;
        }

        buf[(m_numberOfMatrices-matrix)*2] = reg;
        buf[((m_numberOfMatrices-matrix)*2)+1] = val;

        wiringPiSPIDataRW(CHANNEL, buf, (m_numberOfMatrices * 2));
    }

   /*
    else if(matrix == 1)
    {
        uint8_t buf[6];
        buf[0] = 0;
        buf[1] = 0;

        buf[2] = 0;
        buf[3] = 0;

        buf[4] = reg;
        buf[5] = val;
        wiringPiSPIDataRW(CHANNEL, buf, 6);
    }
    else if(matrix == 2)
    {
        uint8_t buf[6];
        buf[0] = 0;
        buf[1] = 0;

        buf[2] = reg;
        buf[3] = val;

        buf[4] = 0;
        buf[5] = 0;
        wiringPiSPIDataRW(CHANNEL, buf, 6);
    }
    else if(matrix == 3)
    {
        uint8_t buf[6];

        buf[0] = reg;
        buf[1] = val;

        buf[2] = 0;
        buf[3] = 0;

        buf[4] = 0;
        buf[5] = 0;

            wiringPiSPIDataRW(CHANNEL, buf, 6);
    }
*/

        //sleep(0.1);
}


void MainWindow::clearDotMatrix()
{

//    for (int i = 1; i < 9; ++i)
//    {
//        spiRegVal(i,0,-1);
//    }

    for (int currentMatrix = 0; currentMatrix < m_numberOfMatrices+1; ++currentMatrix)
    {
        for (int currentColumn = 1; currentColumn < 9; ++currentColumn)
        {
            spiRegVal(currentColumn,0,currentMatrix);
        }
    }

//    for (int i = 1; i < 9; ++i)
//    {
//        spiRegVal(i,0,2);
//    }

}

int MainWindow::binToDec(QString bin)
{
    bool ok;
    return bin.toLongLong(&ok, 2);
}

QStringList MainWindow::rotateFont(QStringList tmpNumber)
{
    QStringList newNumberList;
    QString currentTmp;
    for (int i = 0; i < 3; ++i)
    {
        foreach (QString currentListElement, tmpNumber)
        {
            currentTmp.append(currentListElement.at(i));
        }
        newNumberList.append(currentTmp);
        currentTmp.clear();
    }
    return newNumberList;
}

void MainWindow::initFont()
{
    QStringList tmpNumber;
    QStringList tmpRotateNumber;
    //1:
         tmpNumber.append("010");
         tmpNumber.append("110");
         tmpNumber.append("010");
         tmpNumber.append("010");
         tmpNumber.append("010");

        //rotate font --> change rows and cols
         tmpRotateNumber = rotateFont(tmpNumber);
         m_numberHash.insert(1, tmpRotateNumber);
         tmpNumber.clear();
         tmpRotateNumber.clear();

        //2:
         tmpNumber.append("010");
         tmpNumber.append("101");
         tmpNumber.append("001");
         tmpNumber.append("010");
         tmpNumber.append("111");

         //rotate font --> change rows and cols
          tmpRotateNumber = rotateFont(tmpNumber);
          m_numberHash.insert(2, tmpRotateNumber);
          tmpNumber.clear();
          tmpRotateNumber.clear();

        //3:
         tmpNumber.append("111");
         tmpNumber.append("001");
         tmpNumber.append("011");
         tmpNumber.append("001");
         tmpNumber.append("111");

         //rotate font --> change rows and cols
          tmpRotateNumber = rotateFont(tmpNumber);
          m_numberHash.insert(3, tmpRotateNumber);
          tmpNumber.clear();
          tmpRotateNumber.clear();

        //4:
         tmpNumber.append("101");
         tmpNumber.append("101");
         tmpNumber.append("111");
         tmpNumber.append("001");
         tmpNumber.append("001");

         //rotate font --> change rows and cols
          tmpRotateNumber = rotateFont(tmpNumber);
          m_numberHash.insert(4, tmpRotateNumber);
          tmpNumber.clear();
          tmpRotateNumber.clear();

        //5:
         tmpNumber.append("111");
         tmpNumber.append("100");
         tmpNumber.append("111");
         tmpNumber.append("001");
         tmpNumber.append("111");

         //rotate font --> change rows and cols
          tmpRotateNumber = rotateFont(tmpNumber);
          m_numberHash.insert(5, tmpRotateNumber);
          tmpNumber.clear();
          tmpRotateNumber.clear();

        //6:
         tmpNumber.append("111");
         tmpNumber.append("100");
         tmpNumber.append("111");
         tmpNumber.append("101");
         tmpNumber.append("111");

         //rotate font --> change rows and cols
          tmpRotateNumber = rotateFont(tmpNumber);
          m_numberHash.insert(6, tmpRotateNumber);
          tmpNumber.clear();
          tmpRotateNumber.clear();

        //7:
         tmpNumber.append("111");
         tmpNumber.append("001");
         tmpNumber.append("010");
         tmpNumber.append("100");
         tmpNumber.append("100");

         //rotate font --> change rows and cols
          tmpRotateNumber = rotateFont(tmpNumber);
          m_numberHash.insert(7, tmpRotateNumber);
          tmpNumber.clear();
          tmpRotateNumber.clear();

        //8:
         tmpNumber.append("111");
         tmpNumber.append("101");
         tmpNumber.append("111");
         tmpNumber.append("101");
         tmpNumber.append("111");

         //rotate font --> change rows and cols
          tmpRotateNumber = rotateFont(tmpNumber);
          m_numberHash.insert(8, tmpRotateNumber);
          tmpNumber.clear();
          tmpRotateNumber.clear();

        //9:
         tmpNumber.append("111");
         tmpNumber.append("101");
         tmpNumber.append("111");
         tmpNumber.append("001");
         tmpNumber.append("111");

         //rotate font --> change rows and cols
          tmpRotateNumber = rotateFont(tmpNumber);
          m_numberHash.insert(9, tmpRotateNumber);
          tmpNumber.clear();
          tmpRotateNumber.clear();

        //0:
         tmpNumber.append("111");
         tmpNumber.append("101");
         tmpNumber.append("101");
         tmpNumber.append("101");
         tmpNumber.append("111");

         //rotate font --> change rows and cols
          tmpRotateNumber = rotateFont(tmpNumber);
          m_numberHash.insert(0, tmpRotateNumber);
          tmpNumber.clear();
          tmpRotateNumber.clear();
}

void MainWindow::dispTime()
{

    QTime timeObject = QTime::currentTime();

    int hour = timeObject.hour();

    QStringList zeroBinary = m_numberHash.value(0);

    int matrixLeft  = 3;
    int matrixRight = 4;

    if(hour < 10)
    {
        QStringList hourBinary = m_numberHash.value(hour);

        //left number part >>zero
        for (int i = 0; i < 3; ++i)
        {
            spiRegVal((8-i), binToDec(zeroBinary.at(i)), matrixLeft);
        }

        //right number part
        for (int i = 0; i < 3; ++i)
        {
            spiRegVal((4-i), binToDec(hourBinary.at(i)), matrixLeft);
        }

    }
    else
    {
        int hourLeft = QString::number(hour).left(1).toInt();
        int hourRight = QString::number(hour).right(1).toInt();

        QStringList hourLeftBin = m_numberHash.value(hourLeft);
        QStringList hourRightBin = m_numberHash.value(hourRight);

        //left number part
        for (int i = 0; i < 3; ++i)
        {
            spiRegVal((8-i), binToDec(hourLeftBin.at(i)), matrixLeft);
        }

        //mid number part
        for (int i = 0; i < 3; ++i)
        {
            spiRegVal((4-i), binToDec(hourRightBin.at(i)), matrixLeft);
        }
    }

    //hour minute separator
//    spiRegVal(1, 10, 2);
//    spiRegVal(8, 10, 1);
    spiRegVal(8, 4, matrixRight);
    spiRegVal(1, 4, matrixLeft);

    int minute = timeObject.minute();

    if(minute < 10)
    {
        QStringList minuteBinary = m_numberHash.value(minute);


        //left number part
        for (int i = 0; i < 3; ++i)
        {
            spiRegVal((7-i), binToDec(zeroBinary.at(i)), matrixRight);
        }

        //right number part
        for (int i = 0; i < 3; ++i)
        {
            spiRegVal((3-i), binToDec(minuteBinary.at(i)), matrixRight);
        }

    }
    else
    {

        int minuteLeft = QString::number(minute).left(1).toInt();
        int minuteRight = QString::number(minute).right(1).toInt();

        QStringList minuteLeftBin = m_numberHash.value(minuteLeft);
        QStringList minuteRightBin = m_numberHash.value(minuteRight);

        //left number part
        for (int i = 0; i < 3; ++i)
        {
            spiRegVal((7-i), binToDec(minuteLeftBin.at(i)), matrixRight);
        }

        //right number part
        for (int i = 0; i < 3; ++i)
        {
            spiRegVal((3-i), binToDec(minuteRightBin.at(i)), matrixRight);
        }

    }


}

void MainWindow::dispDate()
{

    int matrixLeft  = 2;
    int matrixRight = 1;



QDate dateObject = QDate::currentDate();

    int day = dateObject.day();

    QStringList zeroBinary = m_numberHash.value(0);


    if(day < 10)
    {
        QStringList dayBinary = m_numberHash.value(day);

        //left number part >>zero
        for (int i = 0; i < 3; ++i)
        {
            spiRegVal((8-i), binToDec(zeroBinary.at(i)), matrixRight);
        }

        //right number part
        for (int i = 0; i < 3; ++i)
        {
            spiRegVal((4-i), binToDec(dayBinary.at(i)), matrixRight);
        }

    }
    else
    {
        int dayLeft = QString::number(day).left(1).toInt();
        int dayRight = QString::number(day).right(1).toInt();

        QStringList dayLeftBin = m_numberHash.value(dayLeft);
        QStringList dayRightBin = m_numberHash.value(dayRight);

        //left number part
        for (int i = 0; i < 3; ++i)
        {
            spiRegVal((8-i), binToDec(dayLeftBin.at(i)), matrixRight);
        }

        //mid number part
        for (int i = 0; i < 3; ++i)
        {
            spiRegVal((4-i), binToDec(dayRightBin.at(i)), matrixRight);
        }
    }

    //day month separator
//    spiRegVal(1, 10, 2);
//    spiRegVal(8, 10, 1);

    spiRegVal(1, 1, matrixRight);

    int month = dateObject.month();

    if(month < 10)
    {
        QStringList monthBinary = m_numberHash.value(month);


        //left number part
        for (int i = 0; i < 3; ++i)
        {
            spiRegVal((7-i), binToDec(zeroBinary.at(i)), matrixLeft);
        }

        //right number part
        for (int i = 0; i < 3; ++i)
        {
            spiRegVal((3-i), binToDec(monthBinary.at(i)), matrixLeft);
        }

    }
    else
    {

        int monthLeft = QString::number(month).left(1).toInt();
        int monthRight = QString::number(month).right(1).toInt();

        QStringList monthLeftBin = m_numberHash.value(monthLeft);
        QStringList monthRightBin = m_numberHash.value(monthRight);

        //left number part
        for (int i = 0; i < 3; ++i)
        {
            spiRegVal((7-i), binToDec(monthLeftBin.at(i)), matrixLeft);
        }

        //right number part
        for (int i = 0; i < 3; ++i)
        {
            spiRegVal((3-i), binToDec(monthRightBin.at(i)), matrixLeft);
        }

    }

}

void MainWindow::dispSnake()
{

    QList<int> pointList;
    pointList << 1 << 2 << 4 << 8 << 16 ;

    for (int line = 0; line < 9; ++line)
    {
        int currentLine = (int)pow(2,line);

        for (int reg = 1; reg < 9; ++reg)
        {
            spiRegVal(reg, currentLine,1);

            usleep(5 * 1000);

            if(reg%3 == 0)
            {
                clearDotMatrix();
            }

        }


        for (int reg = 1; reg < 9; ++reg)
        {
            spiRegVal(reg, currentLine,2);

            usleep(5 * 1000);

            if(reg%3 == 0)
            {
                clearDotMatrix();
            }
        }

    }

}



void MainWindow::displayNumber(int number)
{
    clearDotMatrix();

    if((number>=10) & (number<100))
    {
         int number1 = QString::number(number).left(1).toInt();
         int number2 = QString::number(number).right(1).toInt();

         QStringList currentBinaryNumber1 = m_numberHash.value(number1);
         QStringList currentBinaryNumber2 = m_numberHash.value(number2);

         //left number part
         for (int i = 0; i < 3; ++i)
         {
             spiRegVal((8-i), binToDec(currentBinaryNumber1.at(i)),2);
         }
         //right number part
         for (int i = 0; i < 3; ++i)
         {
             spiRegVal((4-i), binToDec(currentBinaryNumber2.at(i)),2);
         }

//         spiRegVal(8, binToDec(currentBinaryNumber1.at(0)+"0" +currentBinaryNumber2.at(0)));
//         spiRegVal(7, binToDec(currentBinaryNumber1.at(1)+"0" +currentBinaryNumber2.at(1)));
//         spiRegVal(6, binToDec(currentBinaryNumber1.at(2)+"0" +currentBinaryNumber2.at(2)));
//         spiRegVal(5, binToDec(currentBinaryNumber1.at(3)+"0" +currentBinaryNumber2.at(3)));
//         spiRegVal(4, binToDec(currentBinaryNumber1.at(4)+"0" +currentBinaryNumber2.at(4)));

    }
    else if ((number<10)& (number>=0))
    {
        QStringList currentBinaryNumber = m_numberHash.value(number);

        for (int i = 0; i < 3; ++i)
        {
            spiRegVal((8-i), binToDec(currentBinaryNumber.at(i)),2);

        }


//        spi(7, binToDec(currentBinaryNumber.at(1)));
//        spi(6, binToDec(currentBinaryNumber.at(2)));

    }
    else if(number >=100)
    {
        int number1 = QString::number(number).left(1).toInt();
        int number2 = QString::number(number).mid(1,1).toInt();
        int number3 = QString::number(number).right(1).toInt();

        QStringList currentBinaryNumber1 = m_numberHash.value(number1);
        QStringList currentBinaryNumber2 = m_numberHash.value(number2);
        QStringList currentBinaryNumber3 = m_numberHash.value(number3);


        //left number part
        for (int i = 0; i < 3; ++i)
        {
            spiRegVal((8-i), binToDec(currentBinaryNumber1.at(i)),2);
        }

        //sleep(1.1);
        //mid number part
        for (int i = 0; i < 3; ++i)
        {
            spiRegVal((4-i), binToDec(currentBinaryNumber2.at(i)),2);
        }

        //sleep(1.1);
        //right number part
        for (int i = 0; i < 3; ++i)
        {
            spiRegVal((8-i), binToDec(currentBinaryNumber3.at(i)),1);
        }
    }
    else if(number <0)
    {
        //error occured, so clear display
        clearDotMatrix();
    }

}


void MainWindow::on_lineEditValue_returnPressed()
{
    int reg = ui->lineEditReg->text().toInt();
    //int value = ui->lineEditValue->text().toInt();
    bool ok;
    int value = ui->lineEditValue->text().toLongLong(&ok, 2);
    spiRegVal(reg, value,1);

}

void MainWindow::on_pushButton_clicked()
{
    clearDotMatrix();
}


void MainWindow::on_lineEditNumber_returnPressed()
{
    displayNumber(ui->lineEditNumber->text().toInt());
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    displayNumber(position);
    ui->lineEditNumber->setText(QString::number(position));
}

void MainWindow::on_pushButtonDispTime_clicked()
{
    dispTime();
}

void MainWindow::updateTimerFinished()
{
    clearDotMatrix();

    dispDate();
    dispTime();



    /*
    if(m_changeCounter== 0)
    {
        //dispSnake();
        dispTime();

    }
    else if(m_changeCounter == 1)
    {
        //dispSnake();
        dispDate();

        m_changeCounter = -1;
    }
    m_changeCounter ++;
*/
}
