#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->pButtonAdd,SIGNAL(clicked()), this, SLOT(on_push_pButtonAdd()));

    QObject::connect(ui->pButtonDelete,SIGNAL(clicked()), this, SLOT(on_push_pButtonDelete()));

    QObject::connect(ui->pButtonComplete,SIGNAL(clicked()), this, SLOT(on_push_pButtonComplete()));

    QObject::connect(ui->pButtonEdit,SIGNAL(clicked()), this, SLOT(on_push_pButtonEdit()));


    QObject::connect(ui->actionQuit,SIGNAL(triggered(bool)), this, SLOT(on_triggered_Quit()));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_push_pButtonAdd()
{
    if (ui->lEditTarea->text() != "")
    {
        ui->lWidgetTareas->addItem(ui->lEditTarea->text());
    }
}

void MainWindow::on_push_pButtonDelete()
{
    if (ui->lWidgetTareas->selectedItems().size() != 0)
    {
        QListWidgetItem* itemToDelete = ui->lWidgetTareas->selectedItems().at(0);
        ui->lWidgetTareas->takeItem(ui->lWidgetTareas->row(itemToDelete));
        ui->lWidgetTareas->removeItemWidget(itemToDelete);

        delete itemToDelete;
    }
}

void MainWindow::on_push_pButtonComplete()
{
    if (ui->lWidgetTareas->selectedItems().size() != 0)
    {
        QListWidgetItem* itemToChage = ui->lWidgetTareas->currentItem();

        QFont newFont = itemToChage->font();

        if (newFont.strikeOut())
        {
            newFont.setStrikeOut(false);
        }
        else
        {
            newFont.setStrikeOut(true);
        }

        itemToChage->setFont(newFont);
    }

    int numberItems = 0;
    for (int i = 0; i < ui->lWidgetTareas->count(); ++i)
    {
        QListWidgetItem* item = ui->lWidgetTareas->item(i);
        if (item->font().strikeOut())
            numberItems++;
    }

    float valueToSet = (float)numberItems / (float)ui->lWidgetTareas->count();
    ui->pBarProgress->setValue((int)(valueToSet * 100));
}

void MainWindow::on_push_pButtonEdit()
{

    if (ui->lWidgetTareas->selectedItems().size() != 0)
    {
        QListWidgetItem* editingItem = ui->lWidgetTareas->currentItem();

        editingItem->setFlags(editingItem->flags() | Qt::ItemIsEditable);
        ui->lWidgetTareas->editItem(ui->lWidgetTareas->currentItem());

        // Set item as not editable
        editingItem->setFlags(editingItem->flags() & 125);

    }
}

void MainWindow::on_triggered_Quit()
{
    qDebug("foo");

    quick_exit(0);
}
