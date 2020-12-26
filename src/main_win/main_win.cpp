#include <QDebug>
#include <QMessageBox>

#include "main_win.h"
#include "./ui_main_win.h"

MainWin::MainWin(QWidget *parent)
        : QMainWindow(parent),
          ui(new Ui::MainWin),
          brd(new Board) {
    setWindowIcon(QIcon(":/icons/mainWinIcon.ico"));

    ui->setupUi(this);

    connect(ui->buttonGetSquares, SIGNAL(pressed()), this, SLOT(getAvailableSquaresTextView()));

    for (int i = 1; i <= BOARD_SIZE; ++i) {
        for (int j = 1; j <= BOARD_SIZE; ++j)
            connect(getButtonFromGrid(i, j), SIGNAL(pressed()), this, SLOT(getAvailableSquaresGraphicalView()));
    }

    connect(ui->radioButtonTextView, SIGNAL(toggled(bool)), this, SLOT(switchView()));
    connect(ui->radioButtonGraphicalView, SIGNAL(toggled(bool)), this, SLOT(switchView()));

    connect(ui->actionAbout_Author, SIGNAL(triggered(bool)), this, SLOT(actions()));
    connect(ui->actionAbout_Program, SIGNAL(triggered(bool)), this, SLOT(actions()));
    connect(ui->actionRefresh, SIGNAL(triggered(bool)), this, SLOT(actions()));
    connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(actions()));

    connect(ui->buttonExit, SIGNAL(pressed()), this, SLOT(close()));

    ui->radioButtonTextView->setChecked(true);

    ui->queenPlacement->setPlaceholderText("Enter here queen position");

    emit ui->radioButtonTextView->toggled(true);
}

MainWin::~MainWin() {
    delete brd;
    delete ui;
}

void MainWin::getAvailableSquaresTextView() {
    std::pair<uint8_t, uint8_t> queenPos = Board::parseSquareNameToMatrixPos(ui->queenPlacement->text());

    if (ui->queenPlacement->text().isEmpty()) {
        ui->statusBar->showMessage("Enter square name!", 5000);
        return;
    }

    if ((queenPos.first == 0 && queenPos.second == 0) || ui->queenPlacement->text().size() > 2) {
        ui->statusBar->showMessage("Invalid square name!", 5000);
        return;
    }

    qDebug() << "Queen Pos: " << ui->queenPlacement->text() << " (string), " << queenPos << " (matrix pos)";

    brd->getAllAvailableMoves(queenPos);
    ui->availableSquares->clear();

    qDebug() << "Available moves:";

    ui->availableSquares->setText("Available moves:\n");

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (brd->getMBoard()(i, j)) {
                qDebug() << "(" << i << ", " << j << ")";
                ui->availableSquares->setText(
                        ui->availableSquares->toPlainText().append(
                                Board::parseMatrixPosToSquareName(QPair<uint8_t, uint8_t>(i, j)) + "\t"));
            }
        }

        ui->availableSquares->setText(ui->availableSquares->toPlainText().append("\n"));
    }
}

void MainWin::getAvailableSquaresGraphicalView() {
    int _;
    QPair<int, int> buttonPos;
    auto *square = (QPushButton *) sender();

    ui->gridLayout->getItemPosition(ui->gridLayout->indexOf(square), &buttonPos.first, &buttonPos.second, &_, &_);
    --buttonPos.first;
    --buttonPos.second;

    qDebug() << "Queen Pos: " << Board::parseMatrixPosToSquareName(buttonPos) << " (string), " << buttonPos
             << " (matrix pos)";

    brd->getAllAvailableMoves(buttonPos);

    QPushButton *btn = nullptr;

    clearBoard(btn);

    btn = getButtonFromGrid(buttonPos.first + 1, buttonPos.second + 1);
    btn->setIcon(QIcon(":/icons/crown.ico"));
    btn->setIconSize(QSize(48, 48));

    qDebug() << "Available moves:";

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (brd->getMBoard()(i, j)) {
                qDebug() << "(" << i << ", " << j << ")";

                btn = getButtonFromGrid(i + 1, j + 1);
                btn->setStyleSheet("background-color: red;");
                btn->update();
            }
        }
    }
}

void MainWin::switchView() {
    auto *btn = (QRadioButton *) sender();

    QPushButton *square = nullptr;

    if (btn == ui->radioButtonTextView) {
        clearBoard(square);

        for (int i = 1; i <= BOARD_SIZE; ++i) {
            for (int j = 1; j <= BOARD_SIZE; ++j) {
                square = getButtonFromGrid(i, j);
                square->setDisabled(true);
            }
        }

        ui->queenPlacement->setEnabled(true);
        ui->availableSquares->setEnabled(true);
        ui->buttonGetSquares->setEnabled(true);
    } else if (btn == ui->radioButtonGraphicalView) {
        ui->availableSquares->clear();
        ui->queenPlacement->clear();
        ui->availableSquares->setDisabled(true);
        ui->queenPlacement->setDisabled(true);
        ui->buttonGetSquares->setDisabled(true);

        for (int i = 1; i <= BOARD_SIZE; ++i) {
            for (int j = 1; j <= BOARD_SIZE; ++j) {
                square = getButtonFromGrid(i, j);
                square->setEnabled(true);
            }
        }
    } else {
        qDebug() << "Switch gone wrong!";
        ui->statusBar->showMessage("Switch gone wrong!", 5000);
        return;
    }
}

void MainWin::actions() {
    auto *action = (QAction *) sender();

    if (action == ui->actionAbout_Author) {
        QMessageBox::information(this, "Queen Move",
                                 "Find me in the Web!\n"
                                 "Telegram: @vlad_is_real\n"
                                 "GMail: vladislav.kolyadenko@gmail.com\n"
                                 "Instagram: @ncks_gwc");
    } else if (action == ui->actionAbout_Program) {
        QMessageBox::information(this, "Queen Move",
                                 "Simple program to calculate available moves for queen\n"
                                 "May have a lot of bugs and mistakes\n"
                                 "Used technologies:\n"
                                 "C++, Qt, CMake");
    } else if (action == ui->actionRefresh) {
        QPushButton *btn = nullptr;

        ui->availableSquares->clear();
        ui->queenPlacement->clear();

        clearBoard(btn);
    } else if (action == ui->actionExit)
        QApplication::quit();
    else {
        qDebug() << "Invalid action!";
        ui->statusBar->showMessage("Invalid action!", 5000);
        return;
    }
}

void MainWin::clearBoard(QPushButton *btn) {
    for (int i = 1; i <= BOARD_SIZE; ++i) {
        for (int j = 1; j <= BOARD_SIZE; ++j) {
            btn = getButtonFromGrid(i, j);

            btn->setIcon(QIcon());

            if (((i % 2 != 0) && (j % 2 == 0)) || ((i % 2 == 0) && (j % 2 != 0)))
                btn->setStyleSheet("background-color: black;");
            else
                btn->setStyleSheet("background-color: white;");

            btn->update();
        }
    }
}

QPushButton *MainWin::getButtonFromGrid(int x, int y) {
    QLayoutItem *item = ui->gridLayout->itemAtPosition(x, y);
    QWidget *widget = item->widget();
    auto *button = dynamic_cast<QPushButton *>(widget);

    return button;
}