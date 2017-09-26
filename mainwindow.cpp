#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QEventLoop>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    grid(new gridWidget(this))

{
    ui->setupUi(this);
this->setStyleSheet("background-color:white;");




    connect(ui->startButton, SIGNAL(clicked()), grid,SLOT(start()));
    connect(ui->stopButton, SIGNAL(clicked()), grid,SLOT(stop()));
    connect(ui->resetButton, SIGNAL(clicked()), grid,SLOT(clearGrid()));
    connect(ui->setRandomCellsButton, SIGNAL(clicked()), grid,SLOT(setRandomCells()));
    ui->horizontalLayout->addWidget(grid);
ui->horizontalSlider->setValue(grid->getTimerInterval());
    ui->displayNumberOfGenerations->setText("Generation : " + QString::number(grid->getNumberOfGenerations()));
    ui->displayHighScore->setText("Highscore: " + QString::number(grid->getHighScore()));
    //refresh the label containing the number of generations
    connect(grid, SIGNAL(numberOfGenerationsChanged(bool)),this,SLOT(updateTextLabel()));
    //refresh the label containing the highscore of generations
 connect(grid, SIGNAL(HighScoreChanged(bool)),this,SLOT(updateHighScoreLabel()));
    ui->displayNumberOfGenerations->setStyleSheet("border:1px solid black; border-radius: 3px;background-color: transparent;");
    ui->displayHighScore->setStyleSheet("border:1px solid black; border-radius: 3px;background-color: transparent;");
    //ui->initialNumberOfCells->setStyleSheet("border:1px solid black; border-radius: 3px;background-color: transparent;");
    //display the number of cells of the first generation
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::updateTextLabel()
{
 ui->displayNumberOfGenerations->setText("Generation: "+QString::number(grid->getNumberOfGenerations()));
}

void MainWindow::updateHighScoreLabel()
{
 ui->displayHighScore->setText("Highscore: "+QString::number(grid->getHighScore()));
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    grid->setTimerInterval(position);
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    grid->setNumberOfCells(arg1);

}


