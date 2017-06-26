/**
 * Linker - GUI for making symlinks
 * Copyright (C) 2017 Tyler Steiman
 * Made with Qt
 * https://www.qt.io/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

void MainWindow::setTextElements()
{
    this->setFromText(ui->fromText->document()->toPlainText());
    this->setToText(ui->toText->document()->toPlainText());
}

void MainWindow::setFromText(QString text)
{
    this->fromText = text;
}

void MainWindow::setToText(QString text)
{
    this->toText = text;
}

QString MainWindow::getFromText()
{
    return this->fromText;
}

QString MainWindow::getToText()
{
    return this->toText;
}

bool MainWindow::checkElements()
{
    if (this->getFromText().trimmed() == "" || this->getToText().trimmed() == "") {
        ui->statusBar->showMessage("ERROR: Make sure you fill out both From and To!");
        return false;
    }

    if (!this->assertFileExists(this->getFromText())) {
        ui->statusBar->showMessage(QString::asprintf("File %s does not exist!", this->getFromText().toStdString().c_str()));
        return false;
    }

    if (this->assertFileExists(this->getToText())) {
        ui->statusBar->showMessage(QString::asprintf("File %s already exists!", this->getToText().toStdString().c_str()));
        return false;
    }

    return true;
}

void MainWindow::on_scanButton_clicked()
{
    this->setTextElements();
    if (this->checkElements()) {
        ui->statusBar->showMessage(QString::asprintf("File %s exists and is ready to be linked!", this->getFromText().toStdString().c_str()));
    }
}

void MainWindow::on_linkButton_clicked()
{
    this->setTextElements();
    if (this->checkElements()) {
        bool linkStatus = QFile::link(this->getFromText(), this->getToText());

        if (linkStatus) {
            ui->statusBar->showMessage("Link Successfully Created!");
        } else {
            ui->statusBar->showMessage("ERROR: Link NOT created... maybe Scan will help?");
        }
    }
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    this->setTextElements();
    QStringList lst = arg1.split(" -> ");

    this->getFromText() = arg1 == "" ? "" : lst[0];
    this->getToText()   = arg1 == "" ? "" : lst[1];

    ui->fromText->document()->setPlainText(this->getFromText());
    ui->toText->document()->setPlainText(this->getToText());
}
