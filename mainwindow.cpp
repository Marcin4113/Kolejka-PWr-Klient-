#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::add_elem(const char *str)
{
	ui->listWidget->addItem(str);
}

void MainWindow::disable_elem(const int &pos)
{
	QListWidgetItem* item = ui->listWidget->item(pos);
	item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
}

