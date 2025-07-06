#include "mainWindow.h"

mainWindow::mainWindow(QWidget* parent) :
    QWidget(parent), menuBar(new QMenuBar(this)), statusBar(new QStatusBar(this)), mainLayout(new QVBoxLayout(this)),
    contentLayout(new QVBoxLayout), field(new mineField(this)), flagCounterLabel(new QLabel(this))
{
    setLayout(mainLayout);
    mainLayout->addWidget(menuBar);
    mainLayout->addLayout(contentLayout);
    contentLayout->addWidget(field);
    mainLayout->addWidget(statusBar);
    preField = 0;

    menuBar->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    statusBar->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    QMenu* settings = menuBar->addMenu(QString("Menu"));

    QMenu* difficulty = settings->addMenu(QString("Setting"));
    userDefine = difficulty->addAction(QString("Custom"));
    easy = difficulty->addAction(QString("Beginner 10 x 10"));
    middle = difficulty->addAction(QString("Intermediate  16 * 16"));
    hard = difficulty->addAction(QString("Expert 30 * 16"));
    new_game = menuBar->addAction(QString("New game"));

    connect(userDefine, SIGNAL(triggered(bool)), this, SLOT(defaultSetting(bool)));
    connect(easy, SIGNAL(triggered(bool)), this, SLOT(defaultSetting(bool)));
    connect(middle, SIGNAL(triggered(bool)), this, SLOT(defaultSetting(bool)));
    connect(hard, SIGNAL(triggered(bool)), this, SLOT(defaultSetting(bool)));
    connect(field, SIGNAL(gameOver(bool)), this, SLOT(getGameOver(bool)));
    connect(new_game, SIGNAL(triggered(bool)), this, SLOT(newGame(bool)));
    connect(field, SIGNAL(flagCountChanged(int)), this, SLOT(updateFlagCounter(int)));

    QHBoxLayout* topLayout = new QHBoxLayout;
    topLayout->addWidget(menuBar);
    topLayout->addStretch();
    topLayout->addWidget(flagCounterLabel);
    mainLayout->insertLayout(0, topLayout);
    updateFlagCounter(0);
}

mainWindow::~mainWindow() {}

QSize mainWindow::sizeHint() const
{
    return field->sizeHint();
}

void mainWindow::resetMineField(int xNum, int yNum, int MineNum)
{
    if (MineNum >= xNum * yNum)
    {
        QMessageBox::warning(this, QString("Error"), QString("The number of mines should not exceed the number of cells"));
    }
    else
    {
        disconnect(field, SIGNAL(gameOver(bool)), this, SLOT(getGameOver(bool)));

        contentLayout->removeWidget(field);
        field->setParent(Q_NULLPTR);

        if (preField)
        {
            delete preField;
            preField = field;
        }
        else
        {
            preField = field;
        }

        field = new mineField(this, xNum, yNum, MineNum);
        contentLayout->addWidget(field);
        adjustSize();

        connect(field, SIGNAL(gameOver(bool)), this, SLOT(getGameOver(bool)));
        connect(field, SIGNAL(flagCountChanged(int)), this, SLOT(updateFlagCounter(int)));
        field->resetFlagCount();
    }
}

void mainWindow::defaultSetting(bool checked)
{
    Q_UNUSED(checked);
    QAction* act = qobject_cast< QAction* >(QObject::sender());
    if (act == easy)
    {
        resetMineField(10, 10, 10);
    }
    else if (act == middle)
    {
        resetMineField(16, 16, 40);
    }
    else if (act == hard)
    {
        resetMineField(30, 16, 99);
    }
    else if (act == userDefine)
    {
        QDialog dialog;
        QDialogButtonBox* btn = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
        connect(btn, SIGNAL(accepted()), &dialog, SLOT(accept()));
        connect(btn, SIGNAL(rejected()), &dialog, SLOT(reject()));
        dialog.setWindowTitle(QString("Setting"));

        QVBoxLayout* mainDialogLayout = new QVBoxLayout(&dialog);

        QHBoxLayout* layout_xNum = new QHBoxLayout;
        QHBoxLayout* layout_yNum = new QHBoxLayout;
        QHBoxLayout* layout_mineNum = new QHBoxLayout;

        mainDialogLayout->addLayout(layout_xNum);
        mainDialogLayout->addLayout(layout_yNum);
        mainDialogLayout->addLayout(layout_mineNum);
        mainDialogLayout->addWidget(btn);

        QLabel* label_xNum = new QLabel(QString("Width:"), &dialog);
        QLineEdit* edit_xNum = new QLineEdit(&dialog);
        edit_xNum->setFixedWidth(80);
        layout_xNum->addWidget(label_xNum);
        layout_xNum->addWidget(edit_xNum);

        QLabel* label_yNum = new QLabel(QString("Height:"), &dialog);
        QLineEdit* edit_yNum = new QLineEdit(&dialog);
        edit_yNum->setFixedWidth(80);
        layout_yNum->addWidget(label_yNum);
        layout_yNum->addWidget(edit_yNum);

        QLabel* label_mineNum = new QLabel(QString("Mines:"), &dialog);
        QLineEdit* edit_mineNum = new QLineEdit(&dialog);
        edit_mineNum->setFixedWidth(80);
        layout_mineNum->addWidget(label_mineNum);
        layout_mineNum->addWidget(edit_mineNum);

        dialog.exec();

        if (dialog.result() == QDialog::Rejected)
        {
            dialog.close();
        }
        else if (dialog.result() == QDialog::Accepted)
        {
            resetMineField(edit_xNum->text().toInt(), edit_yNum->text().toInt(), edit_mineNum->text().toInt());
        }
    }
}

void mainWindow::getGameOver(bool _lifeStatus)
{
    QMessageBox msg;
    QPushButton* show = msg.addButton(QString(u8"Show"), QMessageBox::AcceptRole);
    QPushButton* newGame = msg.addButton(QString(u8"New game"), QMessageBox::AcceptRole);

    if (_lifeStatus)
    {
        msg.setWindowTitle(QString(u8"You Win"));
        field->showwin();
        msg.exec();
    }
    else
    {
        msg.setWindowTitle(QString(u8"You Die"));
        field->showMines();
        msg.exec();
    }
    if (msg.clickedButton() == show && _lifeStatus)
    {
        field->showwin();
        field->lockAllBlocks();
    }
    else if (msg.clickedButton() == show && !_lifeStatus)
    {
        field->showMines();
        field->lockAllBlocks();
    }
    else if (msg.clickedButton() == newGame)
    {
        resetMineField(field->getXNum(), field->getYNum(), field->getMineNum());
    }
}

void mainWindow::newGame(bool checked)
{
    Q_UNUSED(checked);
    resetMineField(field->getXNum(), field->getYNum(), field->getMineNum());
}

void mainWindow::updateFlagCounter(int flagCount)
{
    int remainingMines = field->getMineNum() - flagCount;
    flagCounterLabel->setText(QString("Осталось мин: %1").arg(remainingMines));
}
