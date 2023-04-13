#include "projectswindow.h"
#include "./ui_projectswindow.h"

ProjectsWindow::ProjectsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProjectsWindow)
{
    ui->setupUi(this);
}

ProjectsWindow::~ProjectsWindow()
{
    delete ui;
}



void ProjectsWindow::on_projectOpenBtn_1_clicked()
{
    Kanbanwindow *nw = new Kanbanwindow();
    this->hide();
    nw->show();
}

