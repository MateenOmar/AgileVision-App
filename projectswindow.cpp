#include "projectswindow.h"
#include "./ui_projectswindow.h"

ProjectsWindow::ProjectsWindow(QWidget *parent, QString userEmail) :
    QMainWindow(parent),
    ui(new Ui::ProjectsWindow)
{
    ui->setupUi(this);
    this->userEmail = userEmail;

    generateProjects();
}

ProjectsWindow::~ProjectsWindow()
{
    delete ui;
}

void ProjectsWindow::generateProjects(){
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("34.30.253.140");
    db.setUserName("root");
    db.setPassword("831Project");
    db.setDatabaseName("AgileVisionDB");
    db.open();
    QSqlQuery qry;
    qry.exec("SELECT * FROM AssignedProjects WHERE email='" + userEmail + "'");

    qry.next();
    ui->projectName_1->setText(qry.value(1).toString());
    qDebug() << qry.value(1);

    qry.next();
    ui->projectName_2->setText(qry.value(1).toString());
    qDebug() << qry.value(1);

    qry.next();
    ui->projectName_3->setText(qry.value(1).toString());
    qDebug() << qry.value(1);

    db.close();
    QSqlDatabase::removeDatabase(db.connectionName());
}


void ProjectsWindow::on_projectOpenBtn_1_clicked()
{
    Kanbanwindow *nw = new Kanbanwindow(nullptr, userEmail, ui->projectName_1->text());
    this->hide();
    nw->show();
}

void ProjectsWindow::on_projectOpenBtn_2_clicked()
{
    Kanbanwindow *nw = new Kanbanwindow(nullptr, userEmail, ui->projectName_2->text());
    this->hide();
    nw->show();
}


void ProjectsWindow::on_projectOpenBtn_3_clicked()
{
    Kanbanwindow *nw = new Kanbanwindow(nullptr, userEmail, ui->projectName_3->text());
    this->hide();
    nw->show();
}

void ProjectsWindow::on_actionSign_Out_triggered()
{
    QMainWindow *nw = new MainWindow();
    this->hide();
    nw->show();
}


