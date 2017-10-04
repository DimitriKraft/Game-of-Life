#include "gridwidget.h"
#include <QMessageBox>
#include <QTimer>
#include <QMouseEvent>
#include <QDebug>
#include <QRectF>
#include <QPainter>
#include <QElapsedTimer>
#include <iostream>
#include <QEventLoop>
#include <QColorDialog>
#include "mainwindow.h"
gridWidget::gridWidget(QWidget *parent) :
    QWidget(parent),
    timer(new QTimer(this)),
    m_numberOfCells(20)

{
    timer->setInterval(400);
    grid = new bool[m_numberOfCells*m_numberOfCells];
    nextGrid = new bool[m_numberOfCells*m_numberOfCells];
    drawingGrid = new bool[m_numberOfCells*m_numberOfCells];
    changedGrid = new bool[m_numberOfCells*m_numberOfCells];
    memset(grid, false, sizeof(bool)*(m_numberOfCells * m_numberOfCells));
    memset(nextGrid, false, sizeof(bool)*(m_numberOfCells * m_numberOfCells));
    memset(drawingGrid, false, sizeof(bool)*(m_numberOfCells * m_numberOfCells));
    memset(changedGrid, false, sizeof(bool)*(m_numberOfCells * m_numberOfCells));
    connect(timer, SIGNAL(timeout()), this, SLOT(nextGeneration()));
    //not much changed
}

gridWidget::~gridWidget()
{
    delete[] grid;
    delete[] nextGrid;
    delete[] drawingGrid;
    delete[] changedGrid;
}
//slot to start the timer
void gridWidget::start()
{
    timer->start();
//slot to stop the timer
}void gridWidget::stop()
{
    timer->stop();
}


int gridWidget::getTimerInterval()
{
   return timer->interval();
}



//Opens a aialog to select a cell color
void gridWidget::setCellColor()
{
    QColorDialog dialog;
    QColor chosenColor = dialog.getColor();
    //check if color was selected
    if( chosenColor.isValid() == true)
    {
        color = chosenColor;
        colorChanged(true);
    }
}

QColor gridWidget::getColor()
{
    return color;
}

//set all cells to false, stop timer, resets the number of alive cells  and generations
 void gridWidget::clearGrid()
{
    for(int y = 0; y < m_numberOfCells;y++)
    {
        for(int x = 0; x < m_numberOfCells;x++)
        {
            grid[y*m_numberOfCells + x] = false;
        }
    }
    m_numberOfGenerations =0;
    m_numberOfAliveCells = 0;
    numberOfGenerationsChanged(true);
    numberOfCellsChanged(true);
    timer->stop();
    update();
}
 int gridWidget::getNumberOfAliveCells()
 {
     return m_numberOfAliveCells;
 }

//create a random generation
 void gridWidget::setRandomCells()
 {
     for(int y =0; y < m_numberOfCells;y++)
     {
         for(int x = 0; x < m_numberOfCells;x++)
         {
             grid[y*m_numberOfCells + x] = false;
             //set every 10th cell alive
             if(std::rand() % 10 == 1)
             {
             grid[y*m_numberOfCells + x] = true;
             }
         }
     }
     m_numberOfGenerations = 0;
     numberOfGenerationsChanged(true);
     timer->stop();
     update();
 }
 int gridWidget::getNumberOfGenerations()
 {
     return m_numberOfGenerations;
 }


//defines the grid by setting the cellnumber and allocating memory
 void gridWidget::setNumberOfCells(int number)
 {


   //set number of cells and initialize grid
   m_numberOfCells = number;
   m_numberOfGenerations = 0;
   m_numberOfAliveCells = 0;
   grid = new bool[(m_numberOfCells)*(m_numberOfCells)];
   nextGrid = new bool[m_numberOfCells*m_numberOfCells];
   drawingGrid = new bool[m_numberOfCells*m_numberOfCells];
   changedGrid = new bool[m_numberOfCells*m_numberOfCells];
   memset(grid, false, sizeof(bool)*(m_numberOfCells * m_numberOfCells));
   memset(nextGrid, false, sizeof(bool)*(m_numberOfCells * m_numberOfCells));
   memset(drawingGrid, false, sizeof(bool)*(m_numberOfCells * m_numberOfCells));
   memset(changedGrid, false, sizeof(bool)*(m_numberOfCells * m_numberOfCells));
   numberOfGenerationsChanged(true);
   numberOfCellsChanged(true);
     timer->stop();
  //  clearGrid();
     update();
 }
 int gridWidget::getNumberOfCells()
 {

     return m_numberOfCells;
 }
 void gridWidget::setTimerInterval(int milliseconds)
 {
     timer->setInterval(milliseconds);
 }

//get the current cell state and handling the edges
 bool gridWidget::cell_state(int  x, int  y)
 {
     //edge handling
     if (x < 0) x += m_numberOfCells;
     else if (x >= m_numberOfCells) x -= m_numberOfCells;
     if (y < 0) y += m_numberOfCells;
     else if (y >= m_numberOfCells) y -= m_numberOfCells;

     return grid[y*m_numberOfCells + x];
 }




//event to refresh the grid
 void gridWidget::paintEvent(QPaintEvent *)
 {
     //refresh the gui
     QPainter p(this);
     drawGrid(p);
     drawCells(p);

     if(mouse_pressed_on == true)
     {
         drawMousePressedCells(p);
     }
 }


//draw the grid
 void gridWidget::drawGrid(QPainter &p)
 {
     QRect borders(0, 0, width()-1, height()-1); // borders of the grid
     QColor gridColor = QColor(0,0,0); // color of the grid
     gridColor.setAlpha(20);
     p.setPen(gridColor);
     double cellWidth = (double)width()/(m_numberOfCells); // width of the widget / number of cells at one row
     double cellHeight = (double)height()/(m_numberOfCells); // height of the widget / number of cells at one row
     for(double k = cellWidth; k <= width(); k += cellWidth)
         p.drawLine(k, 0, k, height());

     for(double k = cellHeight; k <= height(); k += cellHeight)
         p.drawLine(0, k, width(), k);
     p.drawRect(borders);
 }

//increase/decrease gridsize via wheel
 void gridWidget::wheelEvent(QWheelEvent *e)
 {
if((m_numberOfCells >=20))
{
     setNumberOfCells(getNumberOfCells()+(e->delta()/30));
     e->accept();
}
if (m_numberOfCells < 20)
{
    setNumberOfCells(20);
}
 }

//set a cell via mousepress
void gridWidget::mousePressEvent(QMouseEvent *e)
 {
    if(timer->isActive() == false)
    {
    double cellWidth = (double)width()/(m_numberOfCells);
    double cellHeight = (double)height()/(m_numberOfCells);
    int x = floor(e->x()/cellWidth);
    int y = floor(e->y()/cellHeight);
    if(x <= m_numberOfCells-1 && y <= m_numberOfCells-1 && x >= 0 && y >= 0)
    {
        //stay in range!
        grid[y*m_numberOfCells + x] = !grid[y*m_numberOfCells + x];
        changedGrid[y*m_numberOfCells + x] = true;
    update();
    //set drawing flag
    mouse_pressed_on = true;
    }
}
}
//stop drawing mode
void gridWidget::mouseReleaseEvent(QMouseEvent *e)
{
        mouse_pressed_on = false; //stop drawing
        for(int y = 0; y < m_numberOfCells;y++)
        {
            for(int x =0;x < m_numberOfCells;x++)
            {
                changedGrid[y*m_numberOfCells + x] = false;//set changedgrid during drawing to false
                drawingGrid[y*m_numberOfCells + x] = false;  //set drawing grid to blank
            }
        }
        update();

}


//drawing mode while holding the mousebutton
void gridWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(mouse_pressed_on == true) //check if in drawing mode
    {
    //set x y corresponding to coordinates of cell
    double cellWidth = (double)width()/(m_numberOfCells);
    double cellHeight = (double)height()/(m_numberOfCells);
    int x = floor(e->x()/cellWidth);
    int y = floor(e->y()/cellHeight);
    //stay in range!
     if(x <= m_numberOfCells-1 && y <= m_numberOfCells-1 && x >= 0 && y >= 0)
     {
         //check if cell was already changed during one drawing session
     if(changedGrid[y*m_numberOfCells + x] == false)
     {
     //invert grid and drawing grid cell state
     grid[y*m_numberOfCells+x] = !grid[y*m_numberOfCells+x];
     drawingGrid[y*m_numberOfCells+x] = !drawingGrid[y*m_numberOfCells+x];
      //set cell to changed during drawing session to prevent multiple changing of states
     changedGrid[y*m_numberOfCells+x] = true;

     update();
     }
     }
    }
}



//draw cells
 void gridWidget::drawCells(QPainter &p)
 {

     m_numberOfAliveCells = 0;
     double cellWidth = (double)width()/(m_numberOfCells);
     double cellHeight = (double)height()/(m_numberOfCells);
     for(int y=0; y < m_numberOfCells; y++)
     {
         for(int x=0; x < m_numberOfCells; x++)
         {
             if(grid[y*m_numberOfCells+x] == true)
             { // if there is any sense to paint it
                 m_numberOfAliveCells++;

                 qreal left = (qreal)(cellWidth*x); // margin from left
                 qreal top  = (qreal)(cellHeight*y); // margin from top
                 QRectF r(left, top, (qreal)cellWidth, (qreal)cellHeight); 
                 p.fillRect(r, QBrush(color)); // fill cell with brush of main color
             }
         }
     }
     //refresh label in panel
      numberOfCellsChanged(true);

 }

//draw the red cells while in drawing mode
 void gridWidget::drawMousePressedCells(QPainter &p)
 {
     double cellWidth = (double)width()/(m_numberOfCells);
     double cellHeight = (double)height()/(m_numberOfCells);
     for(int y=0; y < m_numberOfCells; y++) {
         for(int x=0; x < m_numberOfCells; x++) {
                 if(grid[y*m_numberOfCells+x] == true)
             { // if there is any sense to paint it

                 qreal left = (qreal)(cellWidth*(x)); // margin from left
                 qreal top  = (qreal)(cellHeight*(y)); // margin from top
                 QRectF r(left, top, (qreal)cellWidth, (qreal)cellHeight);
                 p.fillRect(r, QBrush(color)); // fill cell with brush of main color
                 if(drawingGrid[y*m_numberOfCells+x] == true)
                 {
                     p.fillRect(r,QBrush(Qt::red));
                 }
             }
         }
     }
 }
int gridWidget::getHighScore()
{
 return m_HighScore;
}



void gridWidget::setCell(int x, int y)
{
    //count neighbours of cell
   int count =  cell_state(x,y+1) + cell_state(x,y-1) + cell_state(x-1,y-1) + cell_state(x-1,y) + cell_state(x-1,y+1) + cell_state(x+1,y-1) + cell_state(x+1,y) + cell_state(x+1,y+1);
   //determine status of cell

   if (((grid[y*m_numberOfCells + x] == true) && (count == 2)) || (count == 3))
   {
          nextGrid[y*m_numberOfCells + x] = true;
   }
   else
   {
   nextGrid[y*m_numberOfCells + x] = false;
   }
}







//set the next generation and check if the game got sense
 void gridWidget::nextGeneration()
 {

             int girdNotChangedCount=0;
             //loop through cells and set cells
             for(int y=0; y < m_numberOfCells; y++)
             {
                 for(int x=0; x < m_numberOfCells; x++)
                 {
                    //set cells for next generation
                    setCell(x,y);
                    //check if grid have changed
                    if(grid[y*m_numberOfCells+x] == nextGrid[y*m_numberOfCells+x])
                    {

                        girdNotChangedCount++;
                    }

                 }
             }


             if(girdNotChangedCount == (m_numberOfCells)*(m_numberOfCells))

             {
                 QMessageBox::information(this,
                                          tr("Das Spiel hat den Sinn verloren!"),
                                          tr("Ende des Spiels, da keine neuen Generartionen entstehen."),
                                          QMessageBox::Ok);
                 m_numberOfGenerations = 0;
                 stop();
                 return;
             }

             for(int y=0; y < m_numberOfCells; y++)
             {
                 for(int x=0; x < m_numberOfCells; x++)
                 {
                    //set next grid to old grid
                    grid[y*m_numberOfCells+x] = nextGrid[y*m_numberOfCells+x];
                 }
             }


             update();
             m_numberOfGenerations++;
             if(m_numberOfGenerations > m_HighScore)
             {
                 m_HighScore = m_numberOfGenerations;
                 HighScoreChanged(true);
             }
             numberOfGenerationsChanged(true);
         }

