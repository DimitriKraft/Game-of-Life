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
    //connect buttons with functions
    connect(ui->startButton, SIGNAL(clicked()), grid,SLOT(start()));
    connect(ui->stopButton, SIGNAL(clicked()), grid,SLOT(stop()));
    connect(ui->resetButton, SIGNAL(clicked()), grid,SLOT(clearGrid()));
    connect(ui->setRandomCellsButton, SIGNAL(clicked()), grid,SLOT(setRandomCells()));
    connect(ui->colorChoser,SIGNAL(clicked()), grid, SLOT(setCellColor()));
    //add grid to mainwindow
    ui->horizontalLayout->addWidget(grid);
    //set ui elements
    ui->horizontalSlider->setValue(grid->getTimerInterval());
    ui->displayNumberOfGenerations->setText("Generation : " + QString::number(grid->getNumberOfGenerations()));
    ui->displayHighScore->setText("Highscore : " + QString::number(grid->getHighScore()));
    ui->displayAliveCells->setText("Lebende Zellen : "+QString::number(grid->getNumberOfAliveCells()));

    //refresh the label containing the number of generations
     connect(grid, SIGNAL(numberOfGenerationsChanged(bool)),this,SLOT(updateTextLabel()));
    //refresh the label containing the highscore of generations
     connect(grid, SIGNAL(HighScoreChanged(bool)),this,SLOT(updateHighScoreLabel()));
     connect(grid,SIGNAL(numberOfCellsChanged(bool)),this,SLOT(updateNumberOfAliveCells()));
     connect(grid,SIGNAL(colorChanged(bool)),this,SLOT(updateButtonColor()));
     //set style of labels
    ui->displayNumberOfGenerations->setStyleSheet("border:1px solid black; border-radius: 3px;background-color: transparent;");
    ui->displayHighScore->setStyleSheet("border:1px solid black; border-radius: 3px;background-color: transparent;");
    ui->displayAliveCells->setStyleSheet("border:1px solid black; border-radius: 3px;background-color: transparent;");
    //add icon to button
    QPixmap icon(20, 20);
    icon.fill(grid->getColor());
    ui->colorChoser->setIcon( QIcon(icon) );
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::updateTextLabel()
{
 ui->displayNumberOfGenerations->setText("Generation : "+QString::number(grid->getNumberOfGenerations()));
}

void MainWindow::updateHighScoreLabel()
{
 ui->displayHighScore->setText("Highscore : "+QString::number(grid->getHighScore()));
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    grid->setTimerInterval(position);
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    grid->setNumberOfCells(arg1);

}
void MainWindow::updateNumberOfAliveCells()
{
    ui->displayAliveCells->setText("Lebende Zellen : "+QString::number(grid->getNumberOfAliveCells()));
    ui->spinBox->setValue(grid->getNumberOfCells());
}
void MainWindow::updateButtonColor()
{

    QPixmap icon(20, 20);

    icon.fill(grid->getColor());
    ui->colorChoser->setIcon( QIcon(icon) );
}


