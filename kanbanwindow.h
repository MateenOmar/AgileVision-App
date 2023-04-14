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

    void close_sql();

    void update_close_sql(QString tname, QString column);

    void on_DoneList_itemDoubleClicked(QListWidgetItem *item);

    void on_InReviewList_itemDoubleClicked(QListWidgetItem *item);

    void on_InProgressList_itemDoubleClicked(QListWidgetItem *item);

    void on_BacklogList_itemDoubleClicked(QListWidgetItem *item);

    void on_addTask_clicked();

    void on_sendButton_clicked();

    void setupServerConnection();

    void on_saveButton_clicked();

    void on_refreshButton_clicked();

    void on_sprint1Button_clicked();

    void on_sprint2Button_clicked();

    void on_sprint3Button_clicked();

    void insert_sprint(QString tname, int snum, QString pname);
    void select_sprint();


    void on_commentButton_clicked();

    void on_newIssueList_itemDoubleClicked(QListWidgetItem *item);

    void on_completedIssueList_itemDoubleClicked(QListWidgetItem *item);

    void on_addIssue_clicked();

    void select_Issues();
    void insert_Issues(QString tname, QString icomment, QString pname, QString icomplete);

    void on_saveIssuesButton_clicked();

    void save_Issues();

    void update_save_Issues(QString tname, QString icomplete);

    void on_refreshIssues_clicked();

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
