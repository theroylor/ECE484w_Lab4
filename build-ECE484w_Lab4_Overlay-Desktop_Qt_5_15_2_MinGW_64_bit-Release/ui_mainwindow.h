/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QPushButton *pushButton_composite;
    QLabel *label_base;
    QLabel *label_composite;
    QPushButton *pushButton_base;
    QPushButton *pushButton_overlay;
    QLabel *label_overlay;
    QPushButton *pushButton_base_send;
    QPushButton *pushButton_overlay_send;
    QCheckBox *checkBox_overlay_toggle;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 342);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayoutWidget = new QWidget(centralwidget);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 34, 782, 285));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_composite = new QPushButton(gridLayoutWidget);
        pushButton_composite->setObjectName(QString::fromUtf8("pushButton_composite"));

        gridLayout->addWidget(pushButton_composite, 0, 2, 1, 1);

        label_base = new QLabel(gridLayoutWidget);
        label_base->setObjectName(QString::fromUtf8("label_base"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_base->sizePolicy().hasHeightForWidth());
        label_base->setSizePolicy(sizePolicy);
        label_base->setMinimumSize(QSize(256, 256));
        label_base->setBaseSize(QSize(256, 256));
        label_base->setFrameShadow(QFrame::Plain);
        label_base->setScaledContents(false);
        label_base->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_base, 1, 0, 1, 1, Qt::AlignHCenter|Qt::AlignVCenter);

        label_composite = new QLabel(gridLayoutWidget);
        label_composite->setObjectName(QString::fromUtf8("label_composite"));
        sizePolicy.setHeightForWidth(label_composite->sizePolicy().hasHeightForWidth());
        label_composite->setSizePolicy(sizePolicy);
        label_composite->setMinimumSize(QSize(256, 256));
        label_composite->setBaseSize(QSize(0, 0));
        label_composite->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_composite, 1, 2, 1, 1, Qt::AlignHCenter|Qt::AlignVCenter);

        pushButton_base = new QPushButton(gridLayoutWidget);
        pushButton_base->setObjectName(QString::fromUtf8("pushButton_base"));

        gridLayout->addWidget(pushButton_base, 0, 0, 1, 1);

        pushButton_overlay = new QPushButton(gridLayoutWidget);
        pushButton_overlay->setObjectName(QString::fromUtf8("pushButton_overlay"));

        gridLayout->addWidget(pushButton_overlay, 0, 1, 1, 1);

        label_overlay = new QLabel(gridLayoutWidget);
        label_overlay->setObjectName(QString::fromUtf8("label_overlay"));
        sizePolicy.setHeightForWidth(label_overlay->sizePolicy().hasHeightForWidth());
        label_overlay->setSizePolicy(sizePolicy);
        label_overlay->setMinimumSize(QSize(256, 256));
        label_overlay->setBaseSize(QSize(0, 0));
        label_overlay->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_overlay, 1, 1, 1, 1, Qt::AlignHCenter|Qt::AlignVCenter);

        pushButton_base_send = new QPushButton(centralwidget);
        pushButton_base_send->setObjectName(QString::fromUtf8("pushButton_base_send"));
        pushButton_base_send->setGeometry(QRect(10, 10, 256, 21));
        pushButton_overlay_send = new QPushButton(centralwidget);
        pushButton_overlay_send->setObjectName(QString::fromUtf8("pushButton_overlay_send"));
        pushButton_overlay_send->setGeometry(QRect(270, 10, 256, 21));
        checkBox_overlay_toggle = new QCheckBox(centralwidget);
        checkBox_overlay_toggle->setObjectName(QString::fromUtf8("checkBox_overlay_toggle"));
        checkBox_overlay_toggle->setGeometry(QRect(590, 10, 141, 19));
        checkBox_overlay_toggle->setChecked(false);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 20));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton_composite->setText(QCoreApplication::translate("MainWindow", "Save Composite Image", nullptr));
        label_base->setText(QCoreApplication::translate("MainWindow", "No Base Image Loaded", nullptr));
        label_composite->setText(QCoreApplication::translate("MainWindow", "Please load both base and overlay images.", nullptr));
        pushButton_base->setText(QCoreApplication::translate("MainWindow", "Load Base Image", nullptr));
        pushButton_overlay->setText(QCoreApplication::translate("MainWindow", "Load Overlay Image", nullptr));
        label_overlay->setText(QCoreApplication::translate("MainWindow", "No Overlay Image Loaded", nullptr));
        pushButton_base_send->setText(QCoreApplication::translate("MainWindow", "Send Base Image", nullptr));
        pushButton_overlay_send->setText(QCoreApplication::translate("MainWindow", "Send Overlay Image", nullptr));
        checkBox_overlay_toggle->setText(QCoreApplication::translate("MainWindow", "Enable overlay on DE10", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
