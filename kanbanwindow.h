#ifndef KANBANWINDOW_H
#define KANBANWINDOW_H

#include <QDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QDialogButtonBox>
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include "mainwindow.h"

namespace Ui {
class Kanbanwindow;
}

class Kanbanwindow : public QDialog
{
    Q_OBJECT

public:
    explicit Kanbanwindow(QWidget *parent = nullptr);
    ~Kanbanwindow();

private slots:
    QListWidgetItem update_item(QListWidgetItem *item, QString column);

    void insert_sql(QString tname, QString desc, QString column, QString pname);

    void update_sql(QString tname, QString desc, QString column, QString pname, QString newtname);

    void select_sql();

    void close_sql();

    void update_close_sql(QString tname, QString column);

    void on_DoneList_itemDoubleClicked(QListWidgetItem *item);

    void on_InReviewList_itemDoubleClicked(QListWidgetItem *item);

    void on_InProgressList_itemDoubleClicked(QListWidgetItem *item);

    void on_BacklogList_itemDoubleClicked(QListWidgetItem *item);

    void on_addTask_clicked();

    void on_saveButton_clicked();

    void on_refreshButton_clicked();

private:
    Ui::Kanbanwindow *ui;
    QDialog *dialog;
    QSqlDatabase db2;

};

#endif // KANBANWINDOW_H
