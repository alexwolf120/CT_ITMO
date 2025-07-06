#ifndef BLOCK_H
#define BLOCK_H

#include "Blocks.h"
#include <QTimer>
#include <QWidget>
#include <QPixmap>
#include <QVector>
#include <QPainter>
#include <QEvent>
#include <QPaintEvent>
#include <QMouseEvent>

class bWidgets : public QWidget
{
    Q_OBJECT

public:
    enum nearByColor {
        Zero = 0xffffff,
        One = 0x0436F6,
        Two = 0x13F604,
        Three = 0xF60704,
        Four = 0xF604C7,
        Five = 0xF6BA04,
        Six = 0x919191,
        Seven = 0x0FF8FA,
        Eight = 0x000000
    };

    nearByColor colorGroup[9] = {
            Zero, One, Two,
            Three, Four, Five,
            Six, Seven, Eight
        };

    bWidgets(QWidget* parent = nullptr);
    ~bWidgets();

    Blocks* getBlocks();

    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    void setXPos(int _xPos);
    void setYPos(int _yPos);
    int getXPos();
    int getYPos();

signals:
    void explode();
    void flag(int, int, bool);
    void emptyBlock(int, int);
    void firstClick(int xPos, int yPos);
    void middleClick(int xPos, int yPos);
    void middleClickPressed(int xPos, int yPos);

private:
    static QVector<QPixmap> pixmaps;
    static void initPixmaps();
    QPen pen;
    QFont font;

    int xPos;
    int yPos;

    Blocks* pimpl;
};

#endif // BLOCK_H
