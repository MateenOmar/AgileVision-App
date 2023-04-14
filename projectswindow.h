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
    explicit ProjectsWindow(QWidget *parent = nullptr, QString userEmail = nullptr);
    ~ProjectsWindow();

private slots:
    void generateProjects();
    void on_projectOpenBtn_1_clicked();
    void on_projectOpenBtn_2_clicked();
    void on_projectOpenBtn_3_clicked();
    void on_actionSign_Out_triggered();

private:
    Ui::ProjectsWindow *ui;
    QString userEmail;
    QSqlDatabase db;
};

#endif // PROJECTSWINDOW_H
