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
#include <QIcon>

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
    this->fromText = ui->fromText->document()->toPlainText();
    this->toText   = ui->toText->document()->toPlainText();
}

bool MainWindow::checkElements()
{
    if (this->fromText.trimmed() == "" || this->toText.trimmed() == "") {
        ui->statusBar->showMessage("ERROR: Make sure you fill out both From and To!");
        return false;
    }

    if (!this->assertFileExists(this->fromText)) {
        ui->statusBar->showMessage(QString::asprintf("File %s does not exist!", this->fromText.toStdString().c_str()));
        return false;
    }

    if (this->assertFileExists(this->toText)) {
        ui->statusBar->showMessage(QString::asprintf("File %s already exists!", this->toText.toStdString().c_str()));
        return false;
    }

    return true;
}

void MainWindow::on_scanButton_clicked()
{
    this->setTextElements();
    if (this->checkElements()) {
        ui->statusBar->showMessage(QString::asprintf("File %s exists and is ready to be linked!", this->fromText.toStdString().c_str()));
    }
}

void MainWindow::on_linkButton_clicked()
{
    this->setTextElements();
    if (this->checkElements()) {
        bool linkStatus = QFile::link(this->fromText, this->toText);

        if (linkStatus) {
            ui->statusBar->showMessage("Link Successfully Created!");
        } else {
            ui->statusBar->showMessage("ERROR: Link NOT created... maybe Scan will help?");
        }
    }
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    QStringList lst = arg1.split(" -> ");

    this->fromText = arg1 == "" ? "" : lst[0];
    this->toText   = arg1 == "" ? "" : lst[1];

    ui->fromText->document()->setPlainText(this->fromText);
    ui->toText->document()->setPlainText(this->toText);
}
