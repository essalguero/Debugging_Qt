#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Los componentes se crean en un fichero que genera qt automáticamente que se llama ui_mainwindow.h

#include <QMainWindow>
#include <QKeyEvent>

#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onItemAdded();
    void on_push_pButtonDelete();
    void on_push_pButtonComplete();
    void on_push_pButtonEdit();

    void on_triggered_Quit();
    void on_triggered_Redo();
    void on_triggered_Undo();

private:
    Ui::MainWindow *ui;

    int lastDeletedIndex = -1;
    std::string lastDeletedText = "";
    bool lastDeletedComplete = false;
    bool undone = false;

    void calculatePerformedTasks();

protected:
    void keyPressEvent(QKeyEvent* event);
};

#endif // MAINWINDOW_H
