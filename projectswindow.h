#ifndef PROJECTSWINDOW_H
#define PROJECTSWINDOW_H

#include <QMainWindow>
#include "kanbanwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ProjectsWindow; }
QT_END_NAMESPACE

class ProjectsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProjectsWindow(QWidget *parent = nullptr);
    ~ProjectsWindow();

private slots:
    void on_projectOpenBtn_1_clicked();

private:
    Ui::ProjectsWindow *ui;
};

#endif // PROJECTSWINDOW_H
