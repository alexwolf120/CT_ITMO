#include "mineField.h"

mineField::mineField(QWidget* parent, int _width, int _height, int _mineNum) :
    QWidget(parent), xNum(_width), yNum(_height), mineNum(_mineNum), firstClick(true)
{
    randomRes = randomMines(mineNum, _width, _height);
    std::sort(randomRes.begin(), randomRes.end());

    ptrVector.resize(xNum * yNum);

    for (int i = 0; i < xNum * yNum; ++i)
    {
        ptrVector[i] = new bWidgets(this);
        ptrVector[i]->setXPos(i % xNum);
        ptrVector[i]->setYPos(i / xNum);
        connect(ptrVector[i], SIGNAL(explode()), this, SLOT(getExplode()));
        connect(ptrVector[i], SIGNAL(emptyBlock(int, int)), this, SLOT(getEmptyBlock(int, int)));
        connect(ptrVector[i], SIGNAL(flag(int, int, bool)), this, SLOT(getFlag(int, int, bool)));
        connect(ptrVector[i], SIGNAL(firstClick(int, int)), this, SLOT(handleFirstClick(int, int)));
        connect(ptrVector[i], SIGNAL(middleClick(int, int)), this, SLOT(getMiddleClick(int, int)));
        connect(ptrVector[i], SIGNAL(middleClickPressed(int, int)), this, SLOT(getMiddleClickPressed(int, int)));
    }

    setFixedSize(QSize(xNum * ptrVector[0]->width() + (xNum - 1) * 2, yNum * ptrVector[0]->width() + (yNum - 1) * 2));

    int w_count = 0;
    int h_count = 0;
    int len = ptrVector[0]->width();

    for (int i = 0; i < yNum; ++i)
    {
        for (int j = 0; j < xNum; ++j)
        {
            ptrVector[i * xNum + j]->setGeometry(j + w_count + 2, i + h_count + 2, len, len);
            w_count += len;
        }
        w_count = 0;
        h_count += len;
    }

    for (QVector< int >::iterator iter = randomRes.begin(); iter != randomRes.end(); ++iter)
    {
        ptrVector[*iter]->getBlocks()->setMine(true);

        int settingSquare[3][3];
        getSettingSquare(settingSquare, *iter, xNum);

        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                if (settingSquare[i][j] != -1)
                {
                    ptrVector[settingSquare[i][j]]->getBlocks()->setNearBy(
                        ptrVector[settingSquare[i][j]]->getBlocks()->getNearBy() + 1);
                }
            }
        }
    }
    emit flagCountChanged(flagCount);
}

void mineField::handleFirstClick(int xPos, int yPos)
{
    if (firstClick)
    {
        firstClick = false;
        int index = yPos * xNum + xPos;
        if (ptrVector[index]->getBlocks()->getMine())
        {
            randomRes = randomMines(mineNum, xNum, yNum);
            for (int i = 0; i < xNum * yNum; ++i)
            {
                ptrVector[i]->getBlocks()->setMine(false);
                ptrVector[i]->getBlocks()->setNearBy(0);
            }

            for (QVector< int >::iterator iter = randomRes.begin(); iter != randomRes.end(); ++iter)
            {
                ptrVector[*iter]->getBlocks()->setMine(true);

                int settingSquare[3][3];
                getSettingSquare(settingSquare, *iter, xNum);

                for (int i = 0; i < 3; ++i)
                {
                    for (int j = 0; j < 3; ++j)
                    {
                        if (settingSquare[i][j] != -1)
                        {
                            ptrVector[settingSquare[i][j]]->getBlocks()->setNearBy(
                                ptrVector[settingSquare[i][j]]->getBlocks()->getNearBy() + 1);
                        }
                    }
                }
            }
        }
    }
}

void mineField::getSettingSquare(int _square[3][3], int center, int _xNum)
{
    int x = center % _xNum;
    int y = center / _xNum;

    _square[0][0] = center - _xNum - 1;
    _square[0][1] = center - _xNum;
    _square[0][2] = center - _xNum + 1;
    _square[1][0] = center - 1;
    _square[1][1] = center;
    _square[1][2] = center + 1;
    _square[2][0] = center + _xNum - 1;
    _square[2][1] = center + _xNum;
    _square[2][2] = center + _xNum + 1;

    if (x == 0)
    {
        _square[0][0] = -1;
        _square[1][0] = -1;
        _square[2][0] = -1;
    }
    else if (x == xNum - 1)
    {
        _square[0][2] = -1;
        _square[1][2] = -1;
        _square[2][2] = -1;
    }

    if (y == 0)
    {
        _square[0][0] = -1;
        _square[0][1] = -1;
        _square[0][2] = -1;
    }
    else if (y == yNum - 1)
    {
        _square[2][0] = -1;
        _square[2][1] = -1;
        _square[2][2] = -1;
    }
}

int mineField::getXNum()
{
    return xNum;
}

int mineField::getYNum()
{
    return yNum;
}

int mineField::getMineNum()
{
    return mineNum;
}

void mineField::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    if (randomRes == flagBlocks)
    {
        emit gameOver(true);
    }
}

void mineField::showMines()
{
    for (auto i = 0; i < xNum * yNum; i++)
    {
        ptrVector[i]->getBlocks()->setSweep(true);
        ptrVector[i]->getBlocks()->setStatus(Blocks::Clicked);
    }
    for (auto iter = randomRes.begin(); iter != randomRes.end(); ++iter)
    {
        ptrVector[*iter]->getBlocks()->setSweep(true);
        ptrVector[*iter]->getBlocks()->setStatus(Blocks::Explode);
    }
    update();
}

void mineField::showwin()
{
    for (auto i = 0; i < xNum * yNum; i++)
    {
        ptrVector[i]->getBlocks()->setSweep(true);
        ptrVector[i]->getBlocks()->setStatus(Blocks::Clicked);
    }
    for (auto iter = randomRes.begin(); iter != randomRes.end(); ++iter)
    {
        ptrVector[*iter]->getBlocks()->setSweep(true);
        ptrVector[*iter]->getBlocks()->setStatus(Blocks::Flaged);
    }
    update();
}

QSize mineField::sizeHint() const
{
    if (!ptrVector.empty())
    {
        return QSize(xNum * ptrVector[0]->width(), yNum * ptrVector[0]->width());
    }
    else
    {
        return QSize(200, 400);
    }
}

void mineField::getExplode()
{
    emit gameOver(false);
}

void mineField::getFlag(int _xPos, int _yPos, bool _flag)
{
    int index = _xPos + _yPos * xNum;
    if (_flag)
    {
        if (!flagBlocks.contains(index))
        {
            flagBlocks.push_back(index);
            ++flagCount;
        }
    }
    else
    {
        auto iter = std::find(flagBlocks.begin(), flagBlocks.end(), index);
        if (iter != flagBlocks.end())
        {
            flagBlocks.erase(iter);
            --flagCount;
        }
    }

    std::sort(flagBlocks.begin(), flagBlocks.end());
    emit flagCountChanged(flagCount);
}

void mineField::getEmptyBlock(int _xPos, int _yPos)
{
    int settingSquare[3][3];
    getSettingSquare(settingSquare, _yPos * xNum + _xPos, xNum);
    Blocks* ptr;
    ptr = ptrVector[settingSquare[1][1]]->getBlocks();
    ptr->setSweep(true);
    ptr->setStatus(Blocks::Clicked);
    settingSquare[1][1] = -1;
    update();

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (settingSquare[i][j] != -1)
            {
                ptr = ptrVector[settingSquare[i][j]]->getBlocks();
                if (ptr->getNearBy() && !ptr->getMine())
                {
                    ptr->setSweep(true);
                    ptr->setStatus(Blocks::Clicked);
                }
                else
                {
                    if (!ptr->getSweeped() && !ptr->getMine())
                    {
                        getEmptyBlock(ptrVector[settingSquare[i][j]]->getXPos(), ptrVector[settingSquare[i][j]]->getYPos());
                    }
                }
            }
        }
    }
}

QVector< int > mineField::randomMines(int mineNumber, int _xNum, int _yNum)
{
    QVector< int > randomResult(mineNumber);
    srand(time(NULL));
    int total = _xNum * _yNum;

    for (int i = 0; i < mineNumber; ++i)
    {
        int num = rand() % total;
        if (randomResult.end() != std::find(randomResult.begin(), randomResult.end(), num))
        {
            --i;
        }
        else
        {
            randomResult[i] = num;
        }
    }

    return randomResult;
}

void mineField::lockAllBlocks()
{
    for (auto block : ptrVector)
    {
        block->getBlocks()->setLocked(true);
    }
}

void mineField::resetFlagCount()
{
    flagCount = 0;
    flagBlocks.clear();
    emit flagCountChanged(flagCount);
}

void mineField::getMiddleClick(int xPos, int yPos)
{
    int center = yPos * xNum + xPos;
    int settingSquare[3][3];
    getSettingSquare(settingSquare, center, xNum);

    int flagCount = 0;
    int closedCount = 0;
    QVector< int > closedBlocks;

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (settingSquare[i][j] != -1)
            {
                Blocks* ptr = ptrVector[settingSquare[i][j]]->getBlocks();
                if (ptr->getFlag())
                {
                    ++flagCount;
                }
                if (!ptr->getSweeped() && !ptr->getFlag())
                {
                    ++closedCount;
                    closedBlocks.push_back(settingSquare[i][j]);
                }
            }
        }
    }

    if (flagCount == ptrVector[center]->getBlocks()->getNearBy())
    {
        for (int index : closedBlocks)
        {
            Blocks* ptr = ptrVector[index]->getBlocks();
            ptr->setSweep(true);
            if (ptr->getMine())
            {
                ptr->setStatus(Blocks::Explode);
                emit gameOver(false);
            }
            else
            {
                ptr->setStatus(Blocks::Clicked);
                if (!ptr->getNearBy())
                {
                    getEmptyBlock(ptrVector[index]->getXPos(), ptrVector[index]->getYPos());
                }
            }
        }
    }
    else
    {
        for (int index : closedBlocks)
        {
            ptrVector[index]->getBlocks()->setStatus(Blocks::Normal);
        }
    }

    update();
}

void mineField::getMiddleClickPressed(int xPos, int yPos)
{
    int center = yPos * xNum + xPos;
    int settingSquare[3][3];
    getSettingSquare(settingSquare, center, xNum);

    QVector< int > closedBlocks;

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (settingSquare[i][j] != -1)
            {
                Blocks* ptr = ptrVector[settingSquare[i][j]]->getBlocks();
                if (!ptr->getSweeped() && !ptr->getFlag())
                {
                    closedBlocks.push_back(settingSquare[i][j]);
                }
            }
        }
    }

    for (int index : closedBlocks)
    {
        ptrVector[index]->getBlocks()->setStatus(Blocks::middle);
    }

    update();
}
