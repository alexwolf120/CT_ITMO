#ifndef _MINE_FIELD_H_
#define _MINE_FIELD_H_

#include "bWidgets.h"
#include <cstdlib>
#include <ctime>
#include <QWidget>
#include <QVector>
#include <QMessageBox>
#include <QMouseEvent>

class mineField :
    public QWidget
{
    Q_OBJECT

public:
    mineField(QWidget* parent = nullptr,
        int _width = 10, int _height = 10, int _mineNum = 10);

    void getSettingSquare(int _square[3][3], int center, int _xNum);
    int  getXNum();
    int  getYNum();
    int  getMineNum();
    void mouseReleaseEvent(QMouseEvent* event);
    void restart();
    void showMines();
    void showwin();
    void resetFlagCount();

    QSize sizeHint() const;

public Q_SLOTS:
    void getExplode();
    void getFlag(int _xPos, int _yPos, bool _flag);
    void getEmptyBlock(int _xPos, int _yPos);
    void handleFirstClick(int xPos, int yPos);
    void lockAllBlocks();
    void getMiddleClick(int xPos, int yPos);
    void getMiddleClickPressed(int xPos, int yPos);

Q_SIGNALS:
    void gameOver(bool _lifeStatus);
    void flagCountChanged(int);


private:
    QVector<bWidgets*> ptrVector;
    QVector<int> randomRes;
    QVector<int> flagBlocks;
    int xNum;
    int yNum;
    int mineNum;
    bool firstClick;
    int flagCount;

    QVector<int> randomMines(int mineNumber, int _xNum, int _yNum);
};

#endif
