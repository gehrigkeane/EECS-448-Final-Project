#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include <stdio.h>
#include <QtSql/QtSql>
#include <QDebug>
#include <QFormLayout>
#include <QDialogButtonBox>

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

//#include "C:\\SystemProgramming\\Qt-5.3\\sql\\connection.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//
//create simple dialog that displays a button and has an ok button
//
//    QDialog dialog(this);
//    QFormLayout form(&dialog);
//    QLabel *message = new QLabel("Please select the Login button");
//    form.addWidget(message);
//    QDialogButtonBox buttonBox(QDialogButtonBox::Ok, &dialog);
//    form.addRow(&buttonBox);
//    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
//    dialog.exec();

//
//run database generation code
//
//    if (!createConnection())
//            exit(1);

//
//runs loginBuuton call which opens the login dialog
//
    on_loginButton_clicked();

//
//simple database example
//
//    this->hide();
//    QSqlDatabase db;
//    db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName("users.db3");
//    db.open();
//    db.
//    this->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_logoutButton_clicked()
{
    exit(0);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  Login Dialog Dialog
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MainWindow::on_loginButton_clicked()
{
    QDialog dialog(this);
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add some text above the fields
    form.addRow(new QLabel("Please enter credentials:"));

    // Add the lineEdits with their respective labels
    QLineEdit *userEdit = new QLineEdit(&dialog);
    QString userString = QString("Username:");
    form.addRow(userString, userEdit);

    //create password edit, set echo mode to password for password entry
    QLineEdit *passEdit = new QLineEdit(&dialog);
    passEdit->setEchoMode(QLineEdit::Password);
    QString passString = QString("Password:");
    form.addRow(passString, passEdit);

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QPushButton *regButton = new QPushButton();
    QPushButton *okButton = new QPushButton();
    QPushButton *cancelButton = new QPushButton();
    regButton->setText("Register");
    okButton->setText("Ok");
    cancelButton->setText("Cancel");

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(regButton);
    layout->addWidget(okButton);
    layout->addWidget(cancelButton);

    //QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(layout);
    QObject::connect(regButton, SIGNAL(clicked()), this, SLOT(registerAccount()));
    QObject::connect(okButton, SIGNAL(clicked()), &dialog, SLOT(accept()));
    QObject::connect(cancelButton, SIGNAL(clicked()), this, SLOT(exitFunction()));

    // ok button pressed enters the if statement
    // cancel button closes application
    // register button opens register dialog
    if (dialog.exec() == QDialog::Accepted) {
        // If the user didn't dismiss the dialog, do something with the fields
        ui->nameLabel->setText(userEdit->text());
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  Call if you want to connect a slot to close the program
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MainWindow::exitFunction()
{
    exit(1);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  Add account Dialog
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MainWindow::registerAccount()
{
    QDialog dialog(this);
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add the lineEdits with their respective labels
    QLineEdit *userEdit = new QLineEdit(&dialog);
    QString userString = QString("Username:");
    form.addRow(userString, userEdit);

    //create password edit, set echo mode to password for password entry
    QLineEdit *passEdit = new QLineEdit(&dialog);
    QString passString = QString("Password:");
    form.addRow(passString, passEdit);

    QLineEdit *conPassEdit = new QLineEdit(&dialog);
    QString conPassString = QString("Confirm Password:");
    form.addRow(conPassString, conPassEdit);

    //Add buttons to the register dialog
    QPushButton *okButton = new QPushButton();
    QPushButton *cancelButton = new QPushButton();
    okButton->setText("Ok");
    cancelButton->setText("Cancel");

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(okButton);
    layout->addWidget(cancelButton);

    form.addRow(layout);
    QObject::connect(okButton, SIGNAL(clicked()), &dialog, SLOT(accept()));
    QObject::connect(cancelButton, SIGNAL(clicked()), &dialog, SLOT(close()));

    if (dialog.exec() == QDialog::Accepted) {
        //add the user to the database

        //check passwords are equivalent
        if (passEdit->text() == conPassEdit->text() && userEdit->text() != "")
        {
            std::string usr = userEdit->text().toStdString();
            std::string pss = passEdit->text().toStdString();
            insertUser(usr, pss);
        }
        else
        {

        }//incorrect entry of passwords
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  insert User into database
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MainWindow::insertUser(std::string usr, std::string pss)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("users.db3");
    if (!db.open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
            qApp->tr("Unable to establish a database connection.\n"
                     "This example needs SQLite support. Please read "
                     "the Qt SQL driver documentation for information how "
                     "to build it.\n\n"
                     "Click Cancel to exit."), QMessageBox::Cancel);
    }

//    QSqlQuery query;
//    query.exec("create table users (id int primary key, "
//               "username varchar(20), password varchar(20))");
//    query.exec("select * from users");
//    query.last();
//    std::string size = std::to_string(query.at() + 2);
//    std::string temp = "insert into users values(" + size + ", '" + usr + "', '" + pss + "')";
//    QString qtemp = QString::fromStdString(temp);
//    qDebug(temp.c_str());
//    query.exec(qtemp);


    QSqlQuery query1;
    QString queryString = "SELECT username FROM users WHERE username='" + QString::fromStdString(usr) + "'";
    query1.exec(queryString);

    QSqlRecord record = query1.record();
    qDebug() << record.value(0).toString();

    if (query1.record().value(0).toString() == "")
    {
        qDebug() << "Exists";
    }
    else
    {
        qDebug() << "new entry";
    }

//    while (query1.next()) {
//        QSqlRecord record = query1.record();
//        qDebug() << "Username : " << record.value(0).toString();
//    }

    db.close();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  connect to database initially - mainly referrence code
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool MainWindow::createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("users.db3");
    if (!db.open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
            qApp->tr("Unable to establish a database connection.\n"
                     "This example needs SQLite support. Please read "
                     "the Qt SQL driver documentation for information how "
                     "to build it.\n\n"
                     "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }

    QSqlQuery query;
    query.exec("create table users (id int primary key, "
               "username varchar(20), password varchar(20))");
    query.exec("insert into users values(1, 'Danny', '1234')");
    query.exec("insert into users values(2, 'Christine', '1234')");
    query.exec("insert into users values(3, 'Lars', '1234')");
    query.exec("insert into users values(4, 'Roberto', '1234')");
    query.exec("insert into users values(5, 'Maria', '1234')");

    query.exec("drop table items");
//    query.exec("create table items (id int primary key,"
//                                             "imagefile int,"
//                                             "itemtype varchar(20),"
//                                             "description varchar(100))");
//    query.exec("insert into items "
//               "values(0, 0, 'Qt',"
//               "'Qt is a full development framework with tools designed to "
//               "streamline the creation of stunning applications and  "
//               "amazing user interfaces for desktop, embedded and mobile "
//               "platforms.')");
//    query.exec("insert into items "
//               "values(1, 1, 'Qt Quick',"
//               "'Qt Quick is a collection of techniques designed to help "
//               "developers create intuitive, modern-looking, and fluid "
//               "user interfaces using a CSS & JavaScript like language.')");
//    query.exec("insert into items "
//               "values(2, 2, 'Qt Creator',"
//               "'Qt Creator is a powerful cross-platform integrated "
//               "development environment (IDE), including UI design tools "
//               "and on-device debugging.')");
//    query.exec("insert into items "
//               "values(3, 3, 'Qt Project',"
//               "'The Qt Project governs the open source development of Qt, "
//               "allowing anyone wanting to contribute to join the effort "
//               "through a meritocratic structure of approvers and "
//               "maintainers.')");

//    query.exec("create table images (itemid int, file varchar(20))");
//    query.exec("insert into images values(0, 'images/qt-logo.png')");
//    query.exec("insert into images values(1, 'images/qt-quick.png')");
//    query.exec("insert into images values(2, 'images/qt-creator.png')");
//    query.exec("insert into images values(3, 'images/qt-project.png')");
    db.close();
    return true;
}
