/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *import_2;
    QAction *export_2;
    QAction *about;
    QAction *summ;
    QAction *maxi;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTableWidget *records;
    QMenuBar *menuBar;
    QMenu *menuImport;
    QMenu *menuHelp;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(473, 300);
        import_2 = new QAction(MainWindow);
        import_2->setObjectName(QString::fromUtf8("import_2"));
        import_2->setCheckable(false);
        import_2->setChecked(false);
        export_2 = new QAction(MainWindow);
        export_2->setObjectName(QString::fromUtf8("export_2"));
        about = new QAction(MainWindow);
        about->setObjectName(QString::fromUtf8("about"));
        summ = new QAction(MainWindow);
        summ->setObjectName(QString::fromUtf8("summ"));
        maxi = new QAction(MainWindow);
        maxi->setObjectName(QString::fromUtf8("maxi"));
        maxi->setCheckable(true);
        maxi->setChecked(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 5, 0, 0);
        records = new QTableWidget(centralWidget);
        records->setObjectName(QString::fromUtf8("records"));
        records->setEditTriggers(QAbstractItemView::NoEditTriggers);
        records->setAlternatingRowColors(true);
        records->setSelectionMode(QAbstractItemView::SingleSelection);
        records->setSelectionBehavior(QAbstractItemView::SelectRows);
        records->setRowCount(0);
        records->setColumnCount(0);
        records->horizontalHeader()->setStretchLastSection(true);
        records->verticalHeader()->setVisible(false);

        gridLayout->addWidget(records, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 473, 19));
        menuImport = new QMenu(menuBar);
        menuImport->setObjectName(QString::fromUtf8("menuImport"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuImport->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuImport->addAction(import_2);
        menuImport->addAction(export_2);
        menuImport->addAction(maxi);
        menuHelp->addAction(about);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Auth Logs Manager", nullptr));
        import_2->setText(QCoreApplication::translate("MainWindow", "Import", nullptr));
        export_2->setText(QCoreApplication::translate("MainWindow", "Export", nullptr));
        about->setText(QCoreApplication::translate("MainWindow", "About", nullptr));
        summ->setText(QCoreApplication::translate("MainWindow", "Fast Summary", nullptr));
        maxi->setText(QCoreApplication::translate("MainWindow", "Go Maximazed", nullptr));
        menuImport->setTitle(QCoreApplication::translate("MainWindow", "Data", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
