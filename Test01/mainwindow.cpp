#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

#include "accion.h"

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

        /*lastDeletedIndex = ui->lWidgetTareas->row(itemToDelete);
        lastDeletedText = itemToDelete->text().toStdString();
        lastDeletedComplete = itemToDelete->font().strikeOut();
        */

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
    //ui->lWidgetTareas->removeItemWidget(lastDeletedItem);

    std::cout << "Undo Action raised" << std::endl;

    if (redoList.size() > 0)
    {
        /*QListWidgetItem* itemToDelete = ui->lWidgetTareas->item(lastDeletedIndex);

        ui->lWidgetTareas->takeItem(ui->lWidgetTareas->row(itemToDelete));

        ui->lWidgetTareas->removeItemWidget(itemToDelete);

        delete itemToDelete;

        undone = false;

        ui->actionRedo->setEnabled(false);
        ui->actionUndo->setEnabled(true);*/

        Accion redoneAction = redoList.front();

        if (redoneAction.actionPerformed == "insert")
        {
            std::cout << "Inserting item in position: "<<  redoneAction.accionString << " " << redoneAction.listIndex << std::endl;
            QString stringToInsert;
            stringToInsert = stringToInsert.fromStdString(redoneAction.accionString);

            ui->lWidgetTareas->insertItem(redoneAction.listIndex, stringToInsert);

            if (redoneAction.completed)
            {
                std::cout << "Action was completed" << std::endl;
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
    /*if (lastDeletedIndex != -1 && !undone)
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

    }*/

    std::cout << "Undo Action raised" << std::endl;

    if (undoList.size() != 0)
    {
        Accion undoneAction = undoList.front();
        std::cout << "Action from list: " << undoneAction.actionPerformed << " -> " << undoneAction.accionString << std::endl;

        if (undoneAction.actionPerformed == "insert")
        {
            std::cout << "Undoing Insert" << std::endl;

            ui->lWidgetTareas->takeItem(undoneAction.listIndex);
        }
        else if (undoneAction.actionPerformed == "delete")
        {
           std::cout << "Undoing Delete" << std::endl;

           //QListWidgetItem newItem;
           //QString textString;
           //textString.fromStdString(undoneAction.accionString);
           //newItem.setText(textString);

           std::cout << "Inserting item in position: "<<  undoneAction.accionString << " " << undoneAction.listIndex << std::endl;
           QString stringToInsert;
           stringToInsert = stringToInsert.fromStdString(undoneAction.accionString);

           ui->lWidgetTareas->insertItem(undoneAction.listIndex, stringToInsert);

           if (undoneAction.completed)
           {
               std::cout << "Action was completed" << std::endl;
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
    } else {

        std::cout << "Undo action list is empty" << std::endl;
    }




}

