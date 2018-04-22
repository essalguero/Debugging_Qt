#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

#include "accion.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //Crear las conexiones con los slots correspondientes

    QObject::connect(ui->pButtonAdd, SIGNAL(clicked(bool)), this, SLOT(onItemAdded()));

    QObject::connect(ui->pButtonDelete, SIGNAL(clicked(bool)), this, SLOT(on_push_pButtonDelete()));

    QObject::connect(ui->pButtonComplete,SIGNAL(clicked(bool)), this, SLOT(on_push_pButtonComplete()));

    QObject::connect(ui->pButtonEdit,SIGNAL(clicked(bool)), this, SLOT(on_push_pButtonEdit()));


    QObject::connect(ui->actionQuit, SIGNAL(triggered(bool)), this, SLOT(on_triggered_Quit()));

    QObject::connect(ui->actionUndo, SIGNAL(triggered(bool)), this, SLOT(on_triggered_Undo()));

    QObject::connect(ui->actionRedo, SIGNAL(triggered(bool)), this, SLOT(on_triggered_Redo()));

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

void MainWindow::onItemAdded()
{
    if (ui->lEditTarea->text() != "")
    {
        ui->lWidgetTareas->addItem(ui->lEditTarea->text());

        Accion newAction;

        newAction.accionString = ui->lEditTarea->text().toStdString();
         newAction.actionPerformed = "insert";
        newAction.completed = false;
        newAction.listIndex = ui->lWidgetTareas->count() - 1;

        undoList.push_front(newAction);

        ui->actionRedo->setEnabled(false);
        ui->actionUndo->setEnabled(true);

        //Remove items from redo list
        redoList.clear();

        // Check if maxCapacity have been reached
        if (undoList.size() > MAX_UNDO)
            undoList.pop_back();

        calculatePerformedTasks();
    }
}


void MainWindow::on_push_pButtonDelete()
{
    if (ui->lWidgetTareas->selectedItems().size() != 0)
    {
        QListWidgetItem* itemToDelete = ui->lWidgetTareas->selectedItems().at(0);

        Accion deletedAction;

        deletedAction.accionString = itemToDelete->text().toStdString();
        deletedAction.completed = itemToDelete->font().strikeOut();
        deletedAction.listIndex = ui->lWidgetTareas->row(itemToDelete);
        deletedAction.actionPerformed = "delete";

        ui->lWidgetTareas->takeItem(ui->lWidgetTareas->row(itemToDelete));

        ui->lWidgetTareas->removeItemWidget(itemToDelete);

        delete itemToDelete;

        undoList.push_front(deletedAction);

        ui->actionRedo->setEnabled(false);
        ui->actionUndo->setEnabled(true);

        //Remove items from redo list
        redoList.clear();


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

    if (redoList.size() > 0)
    {

        Accion redoneAction = redoList.front();

        if (redoneAction.actionPerformed == "insert")
        {
            QString stringToInsert;
            stringToInsert = stringToInsert.fromStdString(redoneAction.accionString);

            ui->lWidgetTareas->insertItem(redoneAction.listIndex, stringToInsert);

            if (redoneAction.completed)
            {
                QListWidgetItem *itemIngested = ui->lWidgetTareas->item(redoneAction.listIndex);

                QFont doneFont = itemIngested->font();
                doneFont.setStrikeOut(true);
                itemIngested->setFont(doneFont);
            }
        }
        else if (redoneAction.actionPerformed == "delete")
        {
            ui->lWidgetTareas->takeItem(redoneAction.listIndex);
        }

        undoList.push_front(redoneAction);
        redoList.pop_front();

        if (redoList.size() == 0)
            ui->actionRedo->setEnabled(false);

        ui->actionUndo->setEnabled(true);

        calculatePerformedTasks();
    }
}

void MainWindow::on_triggered_Undo()
{
    if (undoList.size() != 0)
    {
        Accion undoneAction = undoList.front();

        if (undoneAction.actionPerformed == "insert")
        {
            ui->lWidgetTareas->takeItem(undoneAction.listIndex);
        }
        else if (undoneAction.actionPerformed == "delete")
        {
          QString stringToInsert;
           stringToInsert = stringToInsert.fromStdString(undoneAction.accionString);

           ui->lWidgetTareas->insertItem(undoneAction.listIndex, stringToInsert);

           if (undoneAction.completed)
           {
               QListWidgetItem *itemIngested = ui->lWidgetTareas->item(undoneAction.listIndex);

               QFont doneFont = itemIngested->font();
               doneFont.setStrikeOut(true);
               itemIngested->setFont(doneFont);
           }
        }

        redoList.push_front(undoneAction);
        undoList.pop_front();

        if (undoList.size() == 0)
            ui->actionUndo->setEnabled(false);

        ui->actionRedo->setEnabled(true);

        calculatePerformedTasks();
    }

}
