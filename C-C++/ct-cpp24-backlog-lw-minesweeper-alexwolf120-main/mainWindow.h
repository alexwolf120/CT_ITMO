#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include "mineField.h"

#include <QMenuBar>
#include <QLineEdit>
#include <QLabel>
#include <QAction>
#include <QStatusBar>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QAbstractButton>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QResizeEvent>

class mainWindow: public QWidget
{
    Q_OBJECT

public:
    mainWindow(QWidget *parent = Q_NULLPTR);
    ~mainWindow();
    QSize sizeHint() const;

private Q_SLOTS:
    void resetMineField(int xNum, int yNum, int MineNum);
    void defaultSetting(bool checked);
    void getGameOver(bool _lifeStatus);
    void newGame(bool checked);
    void updateFlagCounter(int flagCount);

private:
    QMenuBar* menuBar;
    QMenu* menu;
    QStatusBar* statusBar;
    QVBoxLayout* mainLayout;
    QVBoxLayout* contentLayout;
    QAction *langRus, *langEng, *actSave, *actRevert;
    mineField* field;

    mineField* preField;

    QAction* userDefine;

    QAction* easy;
    QAction* middle;
    QAction* hard;
    QAction* new_game;

    QLabel* flagCounterLabel;
};

#endif // _MAIN_WINDOW_H_
