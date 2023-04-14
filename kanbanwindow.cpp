#include "kanbanwindow.h"
#include "ui_kanbanwindow.h"



Kanbanwindow::Kanbanwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Kanbanwindow)
{
    ui->setupUi(this);
    db2 = QSqlDatabase::addDatabase("QMYSQL");
    db2.setHostName("34.30.253.140");
    db2.setUserName("root");
    db2.setPassword("831Project");
    db2.setDatabaseName("AgileVisionDB");
    db2.open();
    db2.close();
    select_sql();
}

Kanbanwindow::~Kanbanwindow()
{
    delete ui;
}

void Kanbanwindow::close_sql(){

    for(int i = 0; i < ui->BacklogList->count(); i++){
        update_close_sql(ui->BacklogList->item(i)->text(), "BacklogList");
    }
    for(int i = 0; i < ui->InProgressList->count(); i++){
        update_close_sql(ui->InProgressList->item(i)->text(), "InProgressList");
    }
    for(int i = 0; i < ui->InReviewList->count(); i++){
        update_close_sql(ui->InReviewList->item(i)->text(), "InReviewList");
    }
    for(int i = 0; i < ui->DoneList->count(); i++){
        update_close_sql(ui->DoneList->item(i)->text(), "DoneList");
    }
}

void Kanbanwindow::update_close_sql(QString tname, QString column){
    db2.open();
    QSqlDatabase::database().transaction();
    QSqlQuery qry_update(db2);
    qry_update.prepare("UPDATE Tasks SET tcolumn=? WHERE tname=?");
    qry_update.addBindValue(column);
    qry_update.addBindValue(tname);
    if (!qry_update.exec()){
        qDebug() << qry_update.lastError();
    }
    QSqlDatabase::database().commit();
    db2.close();
}

void Kanbanwindow::insert_sql(QString tname, QString desc, QString column, QString pname) {
    db2.open();
    QSqlDatabase::database().transaction();
    QSqlQuery qry_insert(db2);
    qry_insert.prepare("INSERT INTO Tasks (tname,tdesc,tcolumn,pname) VALUES (?,?,?,?)");
    qry_insert.addBindValue(tname);
    qry_insert.addBindValue(desc);
    qry_insert.addBindValue(column);
    qry_insert.addBindValue(pname);
    if (!qry_insert.exec()){
        qDebug() << qry_insert.lastError();
    }
    QSqlDatabase::database().commit();
    db2.close();
}

void Kanbanwindow::update_sql(QString tname, QString desc, QString column, QString pname, QString newtname) {
    db2.open();
    QSqlDatabase::database().transaction();
    QSqlQuery qry_update(db2);
    qry_update.prepare("UPDATE Tasks SET tname=?,tdesc=?,tcolumn=?,pname=? WHERE tname=?");
    qry_update.addBindValue(newtname);
    qry_update.addBindValue(desc);
    qry_update.addBindValue(column);
    qry_update.addBindValue(pname);
    qry_update.addBindValue(tname);
    if (!qry_update.exec()){
        qDebug() << qry_update.lastError();
    }
    QSqlDatabase::database().commit();
    db2.close();

}

void Kanbanwindow::select_sql() {
    db2.open();
    QSqlQuery qry_select(db2);
    if (qry_select.exec("SELECT * FROM Tasks;")){
        while(qry_select.next()){
            if (qry_select.value(2).toString() == "BacklogList"){
                ui->BacklogList->addItem(qry_select.value(0).toString());
            } else if(qry_select.value(2).toString() == "InProgressList"){
                ui->InProgressList->addItem(qry_select.value(0).toString());
            } else if(qry_select.value(2).toString() == "InReviewList"){
                ui->InReviewList->addItem(qry_select.value(0).toString());
            } else if(qry_select.value(2).toString() == "DoneList"){
                ui->DoneList->addItem(qry_select.value(0).toString());
            }
        }

    }
    db2.close();
}

QListWidgetItem Kanbanwindow::update_item(QListWidgetItem *item, QString column) {

    dialog = new QDialog();
    dialog->setWindowModality(Qt::WindowModality::ApplicationModal);
    dialog->setMinimumHeight(480);
    dialog->setMinimumWidth(640);

    QLabel *Label_1 = new QLabel();
    Label_1->setText("Title :");
    Label_1->setGeometry(10,10,150,50);
    Label_1->setParent(dialog);
    Label_1->show();

    QLineEdit *LineEdit_1 = new QLineEdit();
    LineEdit_1->setToolTip("title");
    LineEdit_1->setText(item->text());
    LineEdit_1->setGeometry(150,10,150,50);
    LineEdit_1->setParent(dialog);
    LineEdit_1->show();

    QLabel *Label_2 = new QLabel();
    Label_2->setText("Description :");
    Label_2->setGeometry(10,60,150,50);
    Label_2->setParent(dialog);
    Label_2->show();

    QTextEdit *TextEdit_1 = new QTextEdit();
    TextEdit_1->setGeometry(150,70,150,100);
    TextEdit_1->setParent(dialog);
    if (column != "new"){
        db2.open();
        QSqlQuery qry_select(db2);
        qry_select.prepare("SELECT * FROM Tasks WHERE tname=?;");
        qry_select.addBindValue(item->text());
        qry_select.exec();
        while(qry_select.next()){
            TextEdit_1->setText(qry_select.value(1).toString());
        }
        db2.close();
    }
    TextEdit_1->show();

    QDialogButtonBox *buttonbox = new QDialogButtonBox(QDialogButtonBox::Save|QDialogButtonBox::Cancel);
    connect(buttonbox, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(buttonbox, SIGNAL(rejected()), dialog, SLOT(reject()));
    buttonbox->setParent(dialog);
    buttonbox->setGeometry(210,400,200,50);
    buttonbox->show();

    if (dialog->exec() == QDialog::Accepted){
        if (column == "new"){
            insert_sql(LineEdit_1->text(), TextEdit_1->toPlainText(), "BacklogList", "831 Project");
        } else {
            update_sql(item->text(), TextEdit_1->toPlainText(), column, "831 Project", LineEdit_1->text());
        }
        item->setText(LineEdit_1->text());

    }else {

    }

    return *item;
}

void Kanbanwindow::on_DoneList_itemDoubleClicked(QListWidgetItem *item)
{
    item->setText(update_item(item, "DoneList").text());
}



void Kanbanwindow::on_InReviewList_itemDoubleClicked(QListWidgetItem *item)
{
    item->setText(update_item(item, "InReviewList").text());
}


void Kanbanwindow::on_InProgressList_itemDoubleClicked(QListWidgetItem *item)
{
    item->setText(update_item(item, "InProgressList").text());
}


void Kanbanwindow::on_BacklogList_itemDoubleClicked(QListWidgetItem *item)
{
    item->setText(update_item(item, "BacklogList").text());
}


void Kanbanwindow::on_addTask_clicked()
{
    QListWidgetItem *item = new QListWidgetItem();
    item->setText("");
    item->setText(update_item(item, "new").text());
    ui->BacklogList->addItem(item);
}


void Kanbanwindow::on_saveButton_clicked()
{
    close_sql();
    QMessageBox::information(this, "Saved Changes", "All changes made in the software are now saved.");
}


void Kanbanwindow::on_refreshButton_clicked()
{
    ui->BacklogList->clear();
    ui->InProgressList->clear();
    ui->InReviewList->clear();
    ui->DoneList->clear();
    select_sql();
}

