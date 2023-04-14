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

class QTcpSocket;

class Kanbanwindow : public QDialog
{
    Q_OBJECT

public:
    explicit Kanbanwindow(QWidget *parent = nullptr, QString userEmail = nullptr, QString projectName = nullptr);
    ~Kanbanwindow();

private slots:
    QListWidgetItem update_item(QListWidgetItem *item, QString column);

    void insert_sql(QString tname, QString desc, QString column, QString pname);

    void update_sql(QString tname, QString desc, QString column, QString pname, QString newtname);

    void select_sql();

    void on_DoneList_itemDoubleClicked(QListWidgetItem *item);

    void on_InReviewList_itemDoubleClicked(QListWidgetItem *item);

    void on_InProgressList_itemDoubleClicked(QListWidgetItem *item);

    void on_BacklogList_itemDoubleClicked(QListWidgetItem *item);

    void on_addTask_clicked();

    void on_sendButton_clicked();

    void setupServerConnection();

private:
    Ui::Kanbanwindow *ui;
    QDialog *dialog;
    QSqlDatabase db2;
    QTcpSocket *socket;
    QString userEmail;
    QString userFullName;
    QString projectName;
};

#endif // KANBANWINDOW_H
