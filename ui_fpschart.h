/********************************************************************************
** Form generated from reading UI file 'fpschart.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FPSCHART_H
#define UI_FPSCHART_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE

class Ui_FpsChart
{
public:
    QCustomPlot *mPlot;

    void setupUi(QDialog *FpsChart)
    {
        if (FpsChart->objectName().isEmpty())
            FpsChart->setObjectName(QString::fromUtf8("FpsChart"));
        FpsChart->resize(1298, 661);
        mPlot = new QCustomPlot(FpsChart);
        mPlot->setObjectName(QString::fromUtf8("mPlot"));
        mPlot->setGeometry(QRect(29, 19, 1251, 631));

        retranslateUi(FpsChart);

        QMetaObject::connectSlotsByName(FpsChart);
    } // setupUi

    void retranslateUi(QDialog *FpsChart)
    {
        FpsChart->setWindowTitle(QCoreApplication::translate("FpsChart", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FpsChart: public Ui_FpsChart {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FPSCHART_H
