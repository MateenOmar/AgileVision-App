#include "mainwindow.h"
#include "projectswindow.h"
#include "./ui_mainwindow.h"

#include <QMenuBar>
//QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->centralwidget);

//    db.setHostName("34.30.253.140");
//    db.setUserName("root");
//    db.setPassword("831Project");
//    db.setDatabaseName("AgileVisionDB");
//    db.close();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_loginButton_clicked()
{
//    QString emailText = ui->emailInput->text().trimmed();
//    QString passwordText = ui->passwordInput->text().trimmed();

//    QSqlQuery qry;

//    if (db.open()) {
//        if (qry.exec("SELECT * FROM Users WHERE email=\'" + emailText + "\' AND password=\'" + passwordText + "\'")){
//            QSqlDatabase::removeDatabase("AgileVisionDB");
            QMainWindow *nw = new ProjectsWindow();
            this->hide();
            nw->show();
//        }
//        else {
//            db.close();
//            ui->errorText->setText("Incorrect Email or Password.");
//            ui->errorText->setStyleSheet("QLabel { color : red; }");
//        }
//    }
//    else {
//        db.close();
//        QMessageBox::information(this, "Not Connected", "DB Not Connnected");
//    }

//    db.close();
}


void MainWindow::on_signupButton_clicked()
{
//    if(db.open()) {
//        QMessageBox::information(this, "Connection", "DB Connnected");

//        QSqlQuery qry;

//        if(qry.exec("SELECT * FROM Users;")){
//            while(qry.next()){
//                qDebug () << qry.value(0).toString();
//                qDebug () << qry.value(1).toString();
//            }
//        }
//    }
//    else {
//        QMessageBox::information(this, "Not Connected", "DB Not Connnected");
//    }

//    db.close();
}

