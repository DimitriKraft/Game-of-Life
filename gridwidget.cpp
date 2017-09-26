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
#include "mainwindow.h"
gridWidget::gridWidget(QWidget *parent) :
    QWidget(parent),
    timer(new QTimer(this)),
    m_numberOfCells(20)

{
    timer->setInterval(400);

    //resizeGrid();
    grid = new bool[m_numberOfCells*m_numberOfCells];
    nextTestGrid = new bool[m_numberOfCells*m_numberOfCells];
    drawingTestGrid = new bool[m_numberOfCells*m_numberOfCells];
    changedTestGrid = new bool[m_numberOfCells*m_numberOfCells];
    memset(grid, false, sizeof(bool)*(m_numberOfCells * m_numberOfCells));
    memset(nextTestGrid, false, sizeof(bool)*(m_numberOfCells * m_numberOfCells));
    memset(drawingTestGrid, false, sizeof(bool)*(m_numberOfCells * m_numberOfCells));
    memset(changedTestGrid, false, sizeof(bool)*(m_numberOfCells * m_numberOfCells));
    connect(timer, SIGNAL(timeout()), this, SLOT(drawNewCells()));

}

gridWidget::~gridWidget()
{
    delete[] grid;
    delete[] nextTestGrid;
    delete[] drawingTestGrid;
    delete[] changedTestGrid;
}

void gridWidget::start()
{
    timer->start();
}void gridWidget::stop()
{
    timer->stop();
}


int gridWidget::getTimerInterval()
{
   return timer->interval();
}


 void gridWidget::clearGrid()
{
    for(int x = 0; x < m_numberOfCells;x++)
    {
        for(int y = 0; y < m_numberOfCells;y++)
        {

            grid[y*m_numberOfCells + x] = false;

           // newGrid[x][y] = false;
        }
    }
    m_numberOfGenerations =0;
    numberOfGenerationsChanged(true);
    timer->stop();
    update();
}


 void gridWidget::setRandomCells()
 {
     for(int x =0; x < m_numberOfCells;x++)
     {
         for(int y = 0; y < m_numberOfCells;y++)
         {
          //   newGrid[x][y] = false;
             grid[y*m_numberOfCells + x] = false;
             if(std::rand() % 10 == 1)
             {
            // newGrid[x][y] = true;
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



 void gridWidget::setNumberOfCells(int number)
 {
   m_numberOfCells = number;
   m_numberOfGenerations = 0;
   grid = new bool[m_numberOfCells*m_numberOfCells];
   nextTestGrid = new bool[m_numberOfCells*m_numberOfCells];
   drawingTestGrid = new bool[m_numberOfCells*m_numberOfCells];
   changedTestGrid = new bool[m_numberOfCells*m_numberOfCells];
   memset(grid, false, sizeof(bool)*(m_numberOfCells * m_numberOfCells));
   memset(nextTestGrid, false, sizeof(bool)*(m_numberOfCells * m_numberOfCells));
   memset(drawingTestGrid, false, sizeof(bool)*(m_numberOfCells * m_numberOfCells));
   memset(changedTestGrid, false, sizeof(bool)*(m_numberOfCells * m_numberOfCells));
   numberOfGenerationsChanged(true);
     timer->stop();
     clearGrid();
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


 bool gridWidget::cell_state(int x, int y)
 {
     //edge handling
     if (x < 0) x += m_numberOfCells;
     else if (x >= m_numberOfCells) x -= m_numberOfCells;
     if (y < 0) y += m_numberOfCells;
     else if (y >= m_numberOfCells) y -= m_numberOfCells;

     return grid[y*m_numberOfCells + x];
 }




 bool gridWidget::isAlive(int x, int y)
 {
   // int count = 0;
    int count =  cell_state(x,y+1) + cell_state(x,y-1) + cell_state(x-1,y-1) + cell_state(x-1,y) + cell_state(x-1,y+1) + cell_state(x+1,y-1) + cell_state(x+1,y) + cell_state(x+1,y+1);
     if(grid[y*m_numberOfCells + x] == true )
     {
             if (count != 2 && count != 3)
             {
                 return false;
             }
     }
     else{
         if(count == 3)
         {
             return true;
         }
     }
 }

 void gridWidget::paintEvent(QPaintEvent *)
 {
     QPainter p(this);
     drawGrid(p);
     drawCells(p);
     if(mouse_pressed_on == true)
     {
         drawMousePressedCells(p);
     }
 }



 void gridWidget::drawGrid(QPainter &p)
 {
     QRect borders(0, 0, width()-1, height()-1); // borders of the grid
     QColor gridColor = QColor(0,0,0); // color of the grid
    //
     gridColor.setAlpha(100);
     p.setPen(gridColor);
     double cellWidth = (double)width()/(m_numberOfCells); // width of the widget / number of cells at one row
     double cellHeight = (double)height()/(m_numberOfCells); // height of the widget / number of cells at one row
     for(double k = cellWidth; k <= width(); k += cellWidth)
         p.drawLine(k, 0, k, height());

     for(double k = cellHeight; k <= height(); k += cellHeight)
         p.drawLine(0, k, width(), k);
     p.drawRect(borders);
 }



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
        changedTestGrid[y*m_numberOfCells + x] = true;

   // newGrid[x][y]= !newGrid[x][y];
    //changedGrid[x][y] = true;
    update();
    //set drawing flag
    mouse_pressed_on = true;
    }
}
}

void gridWidget::mouseReleaseEvent(QMouseEvent *e)
{
        mouse_pressed_on = false; //stop drawing
        for(int y = 0; y < m_numberOfCells;y++)
        {
            for(int x =0;x < m_numberOfCells;x++)
            {
                changedTestGrid[y*m_numberOfCells + x] = false;//set changedgrid during drawing to false
                drawingTestGrid[y*m_numberOfCells + x] = false;  //set drawing grid to blank


            }
        }
        update();
}



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
     if(changedTestGrid[y*m_numberOfCells + x] == false)
     {
     //invert grid and drawing grid cell state
     grid[y*m_numberOfCells+x] = !grid[y*m_numberOfCells+x];
     drawingTestGrid[y*m_numberOfCells+x] = !drawingTestGrid[y*m_numberOfCells+x];
      //set cell to changed during drawing session to prevent multiple changing of states
     changedTestGrid[y*m_numberOfCells+x] = true;

     update();
     }
     }
    }
}




 void gridWidget::drawCells(QPainter &p)
 {
     double cellWidth = (double)width()/(m_numberOfCells);
     double cellHeight = (double)height()/(m_numberOfCells);
     for(int y=0; y < m_numberOfCells; y++)
     {
         for(int x=0; x < m_numberOfCells; x++)
         {
             if(grid[y*m_numberOfCells+x] == true)
             { // if there is any sense to paint it
                 qreal left = (qreal)(cellWidth*(x)); // margin from left
                 qreal top  = (qreal)(cellHeight*(y)); // margin from top
                 QRectF r(left, top, (qreal)cellWidth, (qreal)cellHeight);
                 p.fillRect(r, QBrush(Qt::black)); // fill cell with brush of main color
             }
         }
     }
 }

 void gridWidget::drawMousePressedCells(QPainter &p)
 {
     double cellWidth = (double)width()/(m_numberOfCells);
     double cellHeight = (double)height()/(m_numberOfCells);
     for(int y=0; y < m_numberOfCells; y++) {
         for(int x=0; x < m_numberOfCells; x++) {
             //if(newGrid[x][y] == true)
                 if(grid[y*m_numberOfCells+x] == true)
             { // if there is any sense to paint it
                 qreal left = (qreal)(cellWidth*(x)); // margin from left
                 qreal top  = (qreal)(cellHeight*(y)); // margin from top
                 QRectF r(left, top, (qreal)cellWidth, (qreal)cellHeight);
                 p.fillRect(r, QBrush(Qt::black)); // fill cell with brush of main color
                 if(drawingTestGrid[y*m_numberOfCells+x] == true)
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

 void gridWidget::drawNewCells()
 {


            if(m_numberOfGenerations < 0)
                 m_numberOfGenerations++;
             int notChanged=0;
             for(int y=0; y < m_numberOfCells; y++)
             {
                 for(int x=0; x < m_numberOfCells; x++)
                 {
                     grid[y*m_numberOfCells+x] = isAlive(x,y);


                    if(grid[y*m_numberOfCells+x] == nextTestGrid[y*m_numberOfCells+x])
                    {
                        notChanged++;
                    }
                    nextTestGrid[y*m_numberOfCells+x] = grid[y*m_numberOfCells+x];
                 }
             }
             if(notChanged == (m_numberOfCells)*(m_numberOfCells))
             {
                 QMessageBox::information(this,
                                          tr("Das Spiel hat den Sinn verloren!"),
                                          tr("Ende des Spiels, da keine neuen Generartionen entstehen."),
                                          QMessageBox::Ok);
                 m_numberOfGenerations = 0;
                 stop();
                 return;
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

