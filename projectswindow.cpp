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


