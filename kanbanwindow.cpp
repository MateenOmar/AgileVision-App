#include "kanbanwindow.h"
#include "ui_kanbanwindow.h"
#include <QTcpSocket>
#include <QTextStream>


Kanbanwindow::Kanbanwindow(QWidget *parent, QString userEmail, QString projectName) :
    QDialog(parent),
    ui(new Ui::Kanbanwindow)
{
    ui->setupUi(this);
    this->userEmail = userEmail;
    this->projectName = projectName;

    db2 = QSqlDatabase::addDatabase("QMYSQL");
    db2.setHostName("34.30.253.140");
    db2.setUserName("root");
    db2.setPassword("831Project");
    db2.setDatabaseName("AgileVisionDB");
    db2.open();

    QSqlQuery qry;
    qry.exec("SELECT * FROM Users WHERE email='" + userEmail +"'");
    qry.next();
    this->userFullName.append(qry.value(2).toString() + " " + qry.value(3).toString());

    db2.close();

    select_sql();

    select_sprint();

    select_Issues();

    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::readyRead, [&](){
        QTextStream T(socket);
        auto text = T.readAll();
        ui->messagesText->append(text);
    });

    setupServerConnection();

}

Kanbanwindow::~Kanbanwindow()
{
    delete ui;
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
    db2.open();
    QSqlDatabase::database().transaction();
    QSqlQuery qry_update2(db2);
    qry_update2.prepare("UPDATE Issues SET tname=? WHERE tname=?");
    qry_update2.addBindValue(newtname);
    qry_update2.addBindValue(tname);
    if (!qry_update2.exec()){
        qDebug() << qry_update2.lastError();
    }
    QSqlDatabase::database().commit();
    db2.close();
    db2.open();
    QSqlDatabase::database().transaction();
    QSqlQuery qry_update3(db2);
    qry_update3.prepare("UPDATE Sprints SET tname=? WHERE tname=?");
    qry_update3.addBindValue(newtname);
    qry_update3.addBindValue(tname);
    if (!qry_update3.exec()){
        qDebug() << qry_update3.lastError();
    }
    QSqlDatabase::database().commit();
    db2.close();


}

void Kanbanwindow::select_sql() {
    db2.open();
    QSqlQuery qry_select(db2);
    if (qry_select.exec("SELECT * FROM Tasks WHERE pname=\'" + projectName + "\';")){
        while(qry_select.next()){
            if (qry_select.value(2) == "BacklogList"){
                ui->BacklogList->addItem(qry_select.value(0).toString());
            } else if(qry_select.value(2) == "InProgressList"){
                ui->InProgressList->addItem(qry_select.value(0).toString());
            } else if(qry_select.value(2) == "InReviewList"){
                ui->InReviewList->addItem(qry_select.value(0).toString());
            } else if(qry_select.value(2) == "DoneList"){
                ui->DoneList->addItem(qry_select.value(0).toString());
            }
        }

    }
    db2.close();
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
            insert_sql(LineEdit_1->text(), TextEdit_1->toPlainText(), "BacklogList", projectName);
        } else {
            update_sql(item->text(), TextEdit_1->toPlainText(), column, projectName, LineEdit_1->text());
        }
        item->setText(LineEdit_1->text());

    }else {

    }

    return *item;
}

void Kanbanwindow::on_DoneList_itemDoubleClicked(QListWidgetItem *item)
{
    item->setText(update_item(item, "DoneList").text());
    refresh_all();
}

void Kanbanwindow::on_InReviewList_itemDoubleClicked(QListWidgetItem *item)
{
    item->setText(update_item(item, "InReviewList").text());
    refresh_all();
}

void Kanbanwindow::on_InProgressList_itemDoubleClicked(QListWidgetItem *item)
{
    item->setText(update_item(item, "InProgressList").text());
    refresh_all();
}

void Kanbanwindow::on_BacklogList_itemDoubleClicked(QListWidgetItem *item)
{
    item->setText(update_item(item, "BacklogList").text());
    refresh_all();
}

void Kanbanwindow::on_addTask_clicked()
{
    QListWidgetItem *item = new QListWidgetItem();
    item->setText("");
    item->setText(update_item(item, "new").text());
    ui->BacklogList->addItem(item);
}

void Kanbanwindow::refresh_all(){
    ui->newIssueList->clear();
    ui->completedIssueList->clear();
    select_Issues();

    ui->sprint1List->clear();
    ui->sprint2List->clear();
    ui->sprint3List->clear();
    select_sprint();

    ui->BacklogList->clear();
    ui->InProgressList->clear();
    ui->InReviewList->clear();
    ui->DoneList->clear();
    select_sql();
}

void Kanbanwindow::on_sendButton_clicked() {
    QTextStream T(socket);
    if(ui->messageText->text() != ""){
        T << this->userFullName << ": " << ui->messageText->text();
        socket->flush();
        ui->messageText->clear();
    }
}

void Kanbanwindow::setupServerConnection() {

    db2.open();
    QSqlQuery qry;
    qry.exec("SELECT chatIP FROM Projects WHERE pname='" + projectName +"'");
    qry.next();

    qDebug() << qry.value(0);

    QString hostAddress = qry.value(0).toString();
    quint16 port = 3333;

    socket->connectToHost(hostAddress, port);
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

void Kanbanwindow::on_sprint1Button_clicked()
{
    QListWidgetItem *item = new QListWidgetItem();
    item->setText("");
    item->setText(update_item(item, "new").text());
    insert_sprint(item->text(), 1, projectName);
    ui->sprint1List->addItem(item);
    ui->BacklogList->addItem(item);
}

void Kanbanwindow::on_sprint2Button_clicked()
{
    QListWidgetItem *item = new QListWidgetItem();
    item->setText("");
    item->setText(update_item(item, "new").text());
    insert_sprint(item->text(), 2, projectName);
    ui->sprint2List->addItem(item);
    ui->BacklogList->addItem(item);
}

void Kanbanwindow::on_sprint3Button_clicked()
{
    QListWidgetItem *item = new QListWidgetItem();
    item->setText("");
    item->setText(update_item(item, "new").text());
    insert_sprint(item->text(), 3, projectName);
    ui->sprint3List->addItem(item);
    ui->BacklogList->addItem(item);
}

void Kanbanwindow::insert_sprint(QString tname, int snum, QString pname){
    db2.open();
    QSqlDatabase::database().transaction();
    QSqlQuery qry_insert(db2);
    qry_insert.prepare("INSERT INTO Sprints (tname,snumber,pname) VALUES (?,?,?)");
    qry_insert.addBindValue(tname);
    qry_insert.addBindValue(snum);
    qry_insert.addBindValue(pname);
    if (!qry_insert.exec()){
        qDebug() << qry_insert.lastError();
    }
    QSqlDatabase::database().commit();
    db2.close();
}

void Kanbanwindow::select_sprint(){
    db2.open();
    QSqlQuery qry_select(db2);
    if (qry_select.exec("SELECT * FROM Sprints WHERE pname=\'" + projectName + "\';")){
        while(qry_select.next()){
            if (qry_select.value(1) == 1){
                ui->sprint1List->addItem(qry_select.value(0).toString());
            } else if(qry_select.value(1) == 2){
                ui->sprint2List->addItem(qry_select.value(0).toString());
            } else if(qry_select.value(1) == 3){
                ui->sprint3List->addItem(qry_select.value(0).toString());
            }
        }

    }
    db2.close();
}

void Kanbanwindow::on_commentButton_clicked()
{
    QString comment = ui->issueLine->text();
    QString old_comment;
    db2.open();
    QSqlQuery qry_select(db2);
    qry_select.prepare("SELECT * FROM Issues WHERE tname=?;");
    qry_select.addBindValue(ui->issueLabel->text());
    if (qry_select.exec()){
        while (qry_select.next()){
            old_comment = qry_select.value(1).toString();
        }
    }
    db2.close();
    QString new_comment;
    if (old_comment == ""){
        new_comment = this->userFullName + ": " + comment;
    }else {
        new_comment = old_comment + "\n\n" + this->userFullName + ": " + comment;
    }
    db2.open();
    QSqlDatabase::database().transaction();
    QSqlQuery qry_update(db2);
    qry_update.prepare("UPDATE Issues SET icomment=? WHERE tname=?");
    qry_update.addBindValue(new_comment);
    qry_update.addBindValue(ui->issueLabel->text());
    if (!qry_update.exec()){
        qDebug() << qry_update.lastError();
    }
    QSqlDatabase::database().commit();
    db2.close();
    ui->issueComment->setText(new_comment);
    ui->issueLine->clear();

}

void Kanbanwindow::on_newIssueList_itemDoubleClicked(QListWidgetItem *item)
{
    ui->issueLabel->setText(item->text());
    db2.open();
    QSqlQuery qry_select(db2);
    qry_select.prepare("SELECT * FROM Issues WHERE tname=?");
    qry_select.addBindValue(item->text());
    if (qry_select.exec()){
        while (qry_select.next()){
            ui->issueComment->setText(qry_select.value(1).toString());
        }
    }
    db2.close();
}

void Kanbanwindow::on_completedIssueList_itemDoubleClicked(QListWidgetItem *item)
{
    ui->issueLabel->setText(item->text());
    db2.open();
    QSqlQuery qry_select(db2);
    qry_select.prepare("SELECT * FROM Issues WHERE tname=?");
    qry_select.addBindValue(item->text());
    if (qry_select.exec()){
        while (qry_select.next()){
            ui->issueComment->setText(qry_select.value(1).toString());
        }
    }
    db2.close();
}

void Kanbanwindow::on_addIssue_clicked()
{
    QListWidgetItem *item = new QListWidgetItem();
    item->setText("");
    item->setText(update_item(item, "new").text());
    insert_Issues(item->text(), "", projectName, "no");
    ui->newIssueList->addItem(item);
    ui->BacklogList->addItem(item);
}

void Kanbanwindow::select_Issues(){
    db2.open();
    QSqlQuery qry_select(db2);
    if (qry_select.exec("SELECT * FROM Issues WHERE pname=\'" + projectName + "\';")){
        while(qry_select.next()){
            if (qry_select.value(3) == "yes"){
                ui->completedIssueList->addItem(qry_select.value(0).toString());
            } else if(qry_select.value(3) == "no"){
                ui->newIssueList->addItem(qry_select.value(0).toString());
            }
        }

    }
    db2.close();
}

void Kanbanwindow::insert_Issues(QString tname, QString icomment, QString pname, QString icomplete){
    db2.open();
    QSqlDatabase::database().transaction();
    QSqlQuery qry_insert(db2);
    qry_insert.prepare("INSERT INTO Issues (tname,icomment,pname,icomplete) VALUES (?,?,?,?)");
    qry_insert.addBindValue(tname);
    qry_insert.addBindValue(icomment);
    qry_insert.addBindValue(pname);
    qry_insert.addBindValue(icomplete);
    if (!qry_insert.exec()){
        qDebug() << qry_insert.lastError();
    }
    QSqlDatabase::database().commit();
    db2.close();
}

void Kanbanwindow::save_Issues(){
    for(int i = 0; i < ui->newIssueList->count(); i++){
        update_save_Issues(ui->newIssueList->item(i)->text(), "no");
    }
    for(int i = 0; i < ui->completedIssueList->count(); i++){
        update_save_Issues(ui->completedIssueList->item(i)->text(), "yes");
    }
}

void Kanbanwindow::update_save_Issues(QString tname, QString icomplete){
    db2.open();
    QSqlDatabase::database().transaction();
    QSqlQuery qry_update(db2);
    qry_update.prepare("UPDATE Issues SET icomplete=? WHERE tname=?");
    qry_update.addBindValue(icomplete);
    qry_update.addBindValue(tname);
    if (!qry_update.exec()){
        qDebug() << qry_update.lastError();
    }
    QSqlDatabase::database().commit();
    db2.close();
}

void Kanbanwindow::on_saveIssuesButton_clicked()
{
    save_Issues();
    QMessageBox::information(this, "Saved Changes", "All changes made in the software are now saved.");
}

void Kanbanwindow::on_refreshIssues_clicked()
{
    ui->newIssueList->clear();
    ui->completedIssueList->clear();
    select_Issues();
}

