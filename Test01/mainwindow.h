#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_push_pButtonAdd();
    void on_push_pButtonDelete();
    void on_push_pButtonComplete();
    void on_push_pButtonEdit();

    void on_triggered_Quit();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
