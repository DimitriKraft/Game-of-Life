#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H

#include <QWidget>
#include <vector>


class gridWidget : public QWidget
{
    Q_OBJECT

public:
   explicit gridWidget(QWidget *parent = 0);
    ~gridWidget();


  //mouse click events
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
signals:
    void numberOfGenerationsChanged(bool ok);
    void HighScoreChanged(bool ok);
//    void end();
public slots:

    int getNumberOfCells();
    void setNumberOfCells(int number);
    void setTimerInterval(int milliseconds);
    int getHighScore();
    int getTimerInterval();
    void start();
    void stop();

    void clearGrid();

    void setRandomCells();
    int getNumberOfGenerations();


private slots:
    void drawGrid(QPainter &p);
    void drawCells(QPainter &p);
    void drawNewCells();
    void drawMousePressedCells(QPainter &p);


private:
int m_numberOfCells;
int m_HighScore{0};
int m_initialGenerations;

bool mouse_pressed_on{false};
int m_numberOfGenerations{0};


bool *grid;
bool *nextTestGrid;
bool *drawingTestGrid;
bool *changedTestGrid;

bool isAlive(int x, int y);
  bool cell_state(int,int);

//bool isDead(int x,int y);
QTimer *timer;


};

#endif // GRIDWIDGET_H
