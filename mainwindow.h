#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<gridwidget.h>

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
    void on_horizontalSlider_sliderMoved(int position);

    void on_spinBox_valueChanged(int arg1);
  public slots:
    void updateTextLabel();
    void updateHighScoreLabel();

private:
    Ui::MainWindow *ui;
    gridWidget *grid;
};

#endif // MAINWINDOW_H
