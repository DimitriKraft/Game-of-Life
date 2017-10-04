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
    void wheelEvent(QWheelEvent *e);
signals:
    void numberOfGenerationsChanged(bool ok);
    void HighScoreChanged(bool ok);
    void numberOfCellsChanged(bool ok);
    void colorChanged(bool ok);
//    void end();
public slots:
    QColor getColor();
    int getNumberOfCells();
    int getNumberOfAliveCells();
    void setNumberOfCells(int number);
    void setTimerInterval(int milliseconds);
    int getHighScore();
    int getTimerInterval();
    void start();
    void stop();

    void clearGrid();

    void setRandomCells();
    int getNumberOfGenerations();

    void setCellColor();
//    qreal scale{1};
private slots:
    void drawGrid(QPainter &p);
    void drawCells(QPainter &p);
    void nextGeneration();
    void drawMousePressedCells(QPainter &p);


private:
int m_numberOfCells;
int m_HighScore{0};
int m_initialGenerations;

bool mouse_pressed_on{false};
int m_numberOfGenerations{0};
int m_numberOfAliveCells{0};


bool *grid;
bool *nextGrid;
bool *drawingGrid;
bool *changedGrid;

void setCell(int x, int y);
bool cell_state(int, int);

//bool isDead(int x,int y);
QTimer *timer;
QColor color{Qt::black};

public:
 qreal scale{1};
};

#endif // GRIDWIDGET_H
