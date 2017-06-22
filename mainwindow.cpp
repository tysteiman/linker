#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->fromText->document()->setPlainText("/var/run/mysqld/mysqld.sock");
    ui->toText->document()->setPlainText("/tmp/mysql.sock");

    ui->statusBar->showMessage("Waiting to link... Press Scan to begin!");

    this->setWindowTitle("Linker");
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::assertFileExists(QString fName)
{
    return QFile::exists(fName);
}

void MainWindow::on_scanButton_clicked()
{
    this->fromText = ui->fromText->document()->toPlainText();
    this->toText = ui->toText->document()->toPlainText();

    if (!this->assertFileExists(this->fromText)) {
        ui->statusBar->showMessage(QString::asprintf("File %s does not exist!", this->fromText.toStdString().c_str()));
        return;
    }

    if (this->assertFileExists(this->toText)) {
        ui->statusBar->showMessage(QString::asprintf("File %s already exists!", this->toText.toStdString().c_str()));
        return;
    }

    ui->statusBar->showMessage(QString::asprintf("File %s exists and is ready to be linked!", this->fromText.toStdString().c_str()));
}

void MainWindow::on_linkButton_clicked()
{
    this->fromText = ui->fromText->document()->toPlainText();
    this->toText = ui->toText->document()->toPlainText();

    bool linkStatus = QFile::link(this->fromText, this->toText);

    if (linkStatus) {
        ui->statusBar->showMessage("Link Successfully Created!");
    } else {
        ui->statusBar->showMessage("ERROR: Link NOT created... maybe Scan will help?");
    }
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    QStringList lst = arg1.split(" -> ");

    this->fromText = lst[0];
    this->toText = lst[1];

    ui->fromText->document()->setPlainText(this->fromText);
    ui->toText->document()->setPlainText(this->toText);
}
