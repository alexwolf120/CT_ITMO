#include "bWidgets.h"

QVector< QPixmap > bWidgets::pixmaps;

bWidgets::bWidgets(QWidget* parent) : QWidget(parent), xPos(0), yPos(0), pimpl(new Blocks)
{
    initPixmaps();
    pen.setWidthF(5);
    font.setPointSize(15);

    setFixedSize(pixmaps[0].size());
}

void bWidgets::initPixmaps()
{
    pixmaps.resize(6);
    bWidgets::pixmaps[Blocks::Normal].load(":/resourse/start.jpg");
    bWidgets::pixmaps[Blocks::Clicked].load(":/resourse/click.jpg");
    bWidgets::pixmaps[Blocks::Flaged].load(":/resourse/flag.png");
    bWidgets::pixmaps[Blocks::Explode].load(":/resourse/min2.jpg");
    bWidgets::pixmaps[Blocks::middle].load(":/resourse/mine_hover.png");
    bWidgets::pixmaps[Blocks::Question].load(":/resourse/question.png");
}

bWidgets::~bWidgets()
{
    delete pimpl;
}

Blocks* bWidgets::getBlocks()
{
    return pimpl;
}
void bWidgets::setXPos(int _xPos)
{
    xPos = _xPos;
}

void bWidgets::setYPos(int _yPos)
{
    yPos = _yPos;
}

int bWidgets::getXPos()
{
    return xPos;
}

int bWidgets::getYPos()
{
    return yPos;
}

void bWidgets::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    pen.setColor(QColor(colorGroup[pimpl->getNearBy()]));
    painter.setPen(pen);
    painter.setFont(font);

    painter.drawPixmap(rect(), pixmaps[pimpl->getStatus()]);

    if (pimpl->getSweeped() && pimpl->getNearBy() && !pimpl->getMine())
    {
        painter.drawText(QRect(15, 5, 30, 30), QString("%1").arg(pimpl->getNearBy()));
    }
}

void bWidgets::mousePressEvent(QMouseEvent* event)
{
    if (!pimpl->getSweeped() && !pimpl->getFlag() && !pimpl->getLocked() && !pimpl->getQuestioned())
    {
        pimpl->setStatus(Blocks::Clicked);
    }
    else if (event->button() == Qt::MiddleButton && pimpl->getSweeped() && !pimpl->getLocked())
    {
        emit middleClickPressed(xPos, yPos);
    }
    QWidget::mousePressEvent(event);
    update();
}

void bWidgets::mouseReleaseEvent(QMouseEvent* event)
{
    if (rect().contains(event->pos()))
    {
        if (event->button() == Qt::LeftButton && !pimpl->getFlag() && !pimpl->getLocked() && !pimpl->getQuestioned())
        {
            if (!pimpl->getSweeped())
            {
                emit firstClick(xPos, yPos);
            }
            pimpl->setSweep(true);
            if (pimpl->getMine())
            {
                pimpl->setStatus(Blocks::Explode);
                emit explode();
            }
            else
            {
                pimpl->setStatus(Blocks::Clicked);
                if (!pimpl->getNearBy())
                {
                    emit emptyBlock(xPos, yPos);
                }
            }
        }
        else if (event->button() == Qt::RightButton && !pimpl->getSweeped() && !pimpl->getLocked())
        {
            if (pimpl->getFlag())
            {
                pimpl->setFlag(false);
                pimpl->setQuestioned(true);
                pimpl->setStatus(Blocks::Question);
            }
            else if (pimpl->getQuestioned())
            {
                pimpl->setQuestioned(false);
                pimpl->setStatus(Blocks::Normal);
            }
            else
            {
                pimpl->setFlag(true);
                pimpl->setStatus(Blocks::Flaged);
            }
            emit flag(xPos, yPos, pimpl->getFlag());
        }
        else if (event->button() == Qt::MiddleButton && pimpl->getSweeped() && !pimpl->getLocked())
        {
            emit middleClick(xPos, yPos);
        }
    }
    else
    {
        if (!pimpl->getLocked())
        {
            pimpl->setStatus(Blocks::Normal);
        }
    }
    QWidget::mouseReleaseEvent(event);
    update();
}
