#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->pButtonAdd,SIGNAL(clicked()), this, SLOT(onItemAdded()));

    QObject::connect(ui->pButtonDelete,SIGNAL(clicked()), this, SLOT(on_push_pButtonDelete()));

    QObject::connect(ui->pButtonComplete,SIGNAL(clicked()), this, SLOT(on_push_pButtonComplete()));

    QObject::connect(ui->pButtonEdit,SIGNAL(clicked()), this, SLOT(on_push_pButtonEdit()));


    QObject::connect(ui->actionQuit,SIGNAL(triggered(bool)), this, SLOT(on_triggered_Quit()));

    QObject::connect(ui->actionUndo,SIGNAL(triggered(bool)), this, SLOT(on_triggered_Undo()));

    QObject::connect(ui->actionRedo,SIGNAL(triggered(bool)), this, SLOT(on_triggered_Redo()));

}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (focusWidget() == ui->lWidgetTareas)
    {
        if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace)
        {
            on_push_pButtonDelete();
        }
        else if (event->key() == Qt::Key_Return)
        {
            on_push_pButtonEdit();
        }
        else if (event->key() == Qt::Key_Enter)
        {
            on_push_pButtonComplete();
        }
    }
    else if (focusWidget() == ui->lEditTarea)
    {
        if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
            onItemAdded();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onItemAdded()
{
    if (ui->lEditTarea->text() != "")
    {
        ui->lWidgetTareas->addItem(ui->lEditTarea->text());

        calculatePerformedTasks();
    }
}

void MainWindow::calculatePerformedTasks()
{
    int numberItems = 0;
    int totalNumberItems = ui->lWidgetTareas->count();

    if (0 == totalNumberItems)
        ui->pBarProgress->setValue(0);
    else
    {
        for (int i = 0; i < totalNumberItems; ++i)
        {
            QListWidgetItem* item = ui->lWidgetTareas->item(i);
            if (item->font().strikeOut())
                numberItems++;
        }


        float valueToSet = (float)numberItems / totalNumberItems;
        ui->pBarProgress->setValue((int)(valueToSet * 100));
    }
}

void MainWindow::on_push_pButtonDelete()
{
    if (ui->lWidgetTareas->selectedItems().size() != 0)
    {
        QListWidgetItem* itemToDelete = ui->lWidgetTareas->selectedItems().at(0);


        lastDeletedIndex = ui->lWidgetTareas->row(itemToDelete);
        lastDeletedText = itemToDelete->text().toStdString();
        lastDeletedComplete = itemToDelete->font().strikeOut();
        ui->lWidgetTareas->takeItem(ui->lWidgetTareas->row(itemToDelete));

        ui->lWidgetTareas->removeItemWidget(itemToDelete);

        delete itemToDelete;

        undone = false;

        ui->actionRedo->setEnabled(false);
        ui->actionUndo->setEnabled(true);

        calculatePerformedTasks();
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

        calculatePerformedTasks();
    }


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

void MainWindow::on_triggered_Redo()
{
    //ui->lWidgetTareas->removeItemWidget(lastDeletedItem);
    if (lastDeletedIndex != -1 && undone)
    {
        QListWidgetItem* itemToDelete = ui->lWidgetTareas->item(lastDeletedIndex);

        ui->lWidgetTareas->takeItem(ui->lWidgetTareas->row(itemToDelete));

        ui->lWidgetTareas->removeItemWidget(itemToDelete);

        delete itemToDelete;

        undone = false;

        ui->actionRedo->setEnabled(false);
        ui->actionUndo->setEnabled(true);

        calculatePerformedTasks();
    }
}

void MainWindow::on_triggered_Undo()
{
    if (lastDeletedIndex != -1 && !undone)
    {
        ui->lWidgetTareas->insertItem(lastDeletedIndex, QString::fromStdString(lastDeletedText));
       // ui->lWidgetTareas->addItem(QString::fromStdString(lastDeletedText));
        undone = true;

        if (lastDeletedComplete)
        {
            QListWidgetItem* addedItem = ui->lWidgetTareas->item(lastDeletedIndex);
            QFont newFont = addedItem->font();

            newFont.setStrikeOut(true);

            addedItem->setFont(newFont);
        }

        ui->actionRedo->setEnabled(true);
        ui->actionUndo->setEnabled(false);

        calculatePerformedTasks();

    }




}

