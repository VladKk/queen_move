#include <QDebug>
#include <QMessageBox>
#include <QRegularExpressionValidator>

#include "main_win.h"
#include "./ui_main_win.h"

// TODO: obstacle system

MainWin::MainWin(QWidget *parent)
        : QMainWindow(parent),
          ui(new Ui::MainWin),
          brd(new Board) {
    setWindowIcon(QIcon(":/icons/mainWinIcon.ico"));

    ui->setupUi(this);

    connect(ui->buttonGetSquares, SIGNAL(pressed()), this, SLOT(getAvailableSquaresTextView()));

    // Connect all the squares to slot (start from [1;1] because first row and column are square marks)
    for (int i = 1; i <= BOARD_SIZE; ++i) {
        for (int j = 1; j <= BOARD_SIZE; ++j)
            connect(getButtonFromGrid(i, j), SIGNAL(pressed()), this, SLOT(getAvailableSquaresGraphicalView()));
    }

    connect(ui->modeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(switchMode()));

    connect(ui->radioButtonTextView, SIGNAL(toggled(bool)), this, SLOT(switchView()));
    connect(ui->radioButtonGraphicalView, SIGNAL(toggled(bool)), this, SLOT(switchView()));

    connect(ui->actionAbout_Author, SIGNAL(triggered(bool)), this, SLOT(actions()));
    connect(ui->actionAbout_Program, SIGNAL(triggered(bool)), this, SLOT(actions()));
    connect(ui->actionRefresh, SIGNAL(triggered(bool)), this, SLOT(actions()));
    connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(actions()));

    connect(ui->buttonExit, SIGNAL(pressed()), this, SLOT(close()));

    ui->radioButtonTextView->setChecked(true);

    ui->queenPlacement->setPlaceholderText("Enter here queen position");

    // Toggle text view
    emit ui->radioButtonTextView->toggled(true);

    // Unselect any of modes
    ui->modeSelector->setCurrentIndex(0);
    switchMode();
}

MainWin::~MainWin() {
    delete brd;
    delete ui;
}

void MainWin::getAvailableSquaresTextView() {
    // Check if user entered text
    if (ui->queenPlacement->text().isEmpty()) {
        ui->statusBar->showMessage("Enter square name!", 5000);
        return;
    }

    // Get queen position
    QPair<uint8_t, uint8_t> queenPos = Board::parseSquareNameToMatrixPos(ui->queenPlacement->text());

    // Check if next move is valid (used only for game mode)
    if (ui->modeSelector->currentIndex() == 2) {
        bool validMove;

        validMove = MainWin::findSquareName(ui->queenPlacement->text());

        if (!validMove && !ui->availableSquares->toPlainText().isEmpty()) {
            qDebug() << "Invalid move!";
            ui->statusBar->showMessage("Invalid move!", 5000);
            return;
        }
    }

    qDebug() << "Queen Pos: " << ui->queenPlacement->text() << " (string), " << queenPos << " (matrix pos)";

    brd->getAllAvailableMoves(queenPos);
    ui->availableSquares->clear();

    qDebug() << "Available moves:";

    ui->availableSquares->setText("Available moves:\n");

    // Get all available moves and parse them to square name
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

    // Get position of pressed button in grid ([--i; --j] for board)
    ui->gridLayout->getItemPosition(ui->gridLayout->indexOf(square), &buttonPos.first, &buttonPos.second, &_, &_);
    --buttonPos.first;
    --buttonPos.second;

    qDebug() << "Queen Pos: " << Board::parseMatrixPosToSquareName(buttonPos) << " (string), " << buttonPos
             << " (matrix pos)";

    brd->getAllAvailableMoves(buttonPos);

    QPushButton *btn = nullptr;

    clearBoard(btn);

    // Set crown icon on pressed button
    btn = getButtonFromGrid(buttonPos.first + 1, buttonPos.second + 1);
    btn->setIcon(QIcon(":/icons/crown.ico"));
    btn->setIconSize(QSize(48, 48));

    qDebug() << "Available moves:";

    // Get all available moves and color appropriate buttons in red
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            btn = getButtonFromGrid(i + 1, j + 1);

            if (brd->getMBoard()(i, j)) {
                qDebug() << "(" << i << ", " << j << ")";

                btn->setStyleSheet("background-color: red;");
                btn->setEnabled(true);
            }

            // If game mode is selected, disable other buttons
            if (ui->modeSelector->currentIndex() == 2 && !brd->getMBoard()(i, j))
                btn->setDisabled(true);

            btn->update();
        }
    }
}

void MainWin::switchView() {
    auto *btn = (QRadioButton *) sender();

    QPushButton *square = nullptr;

    // Disable board and enable text input/output
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
    }
     // Disable text input/output and enable board
    else if (btn == ui->radioButtonGraphicalView) {
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
    }
    // Something gone wrong
    else {
        qDebug() << "Switch gone wrong!";
        ui->statusBar->showMessage("Switch gone wrong!", 5000);
        return;
    }
}

void MainWin::switchMode() {
    QPushButton *square;

    switch (ui->modeSelector->currentIndex()) {
        // All modes are unselected (disable everything)
        case 0:
            ui->radioButtonTextView->setDisabled(true);
            ui->radioButtonGraphicalView->setDisabled(true);
            ui->queenPlacement->setDisabled(true);
            ui->buttonGetSquares->setDisabled(true);
            ui->availableSquares->setDisabled(true);

            for (int i = 1; i <= BOARD_SIZE; ++i) {
                for (int j = 1; j <= BOARD_SIZE; ++j) {
                    square = getButtonFromGrid(i, j);
                    square->setDisabled(true);
                }
            }

            ui->queenPlacement->setValidator(nullptr);

            break;

        // One of the modes was selected (enable everything and choose either text or graphical view)
        case 1:
        case 2:
            ui->radioButtonGraphicalView->setEnabled(true);
            ui->radioButtonTextView->setEnabled(true);
            ui->queenPlacement->setEnabled(true);
            ui->buttonGetSquares->setEnabled(true);
            ui->availableSquares->setEnabled(true);

            for (int i = 1; i <= BOARD_SIZE; ++i) {
                for (int j = 1; j <= BOARD_SIZE; ++j) {
                    square = getButtonFromGrid(i, j);
                    square->setEnabled(true);
                }
            }

            if (ui->radioButtonTextView->isChecked())
                    emit ui->radioButtonTextView->toggled(true);
            else if (ui->radioButtonGraphicalView->isChecked())
                    emit ui->radioButtonGraphicalView->toggled(true);
            else {
                qDebug() << "Invalid view!";
                ui->statusBar->showMessage("Invalid view!", 5000);
                return;
            }

            // TODO: add this regex for obstacles ^[A-H|a-h][1-8](\s\[([A-H|a-h][1-8],\s)*[A-H|a-h][1-8]\])?$

            // Set validator for valid text input
            ui->queenPlacement->setValidator(
                    new QRegularExpressionValidator(QRegularExpression("^[A-H|a-h][1-8]$"), this));

            break;

        // Something gone wrong
        default:
            qDebug() << "Invalid mode!";
            ui->statusBar->showMessage("Invalid mode!", 5000);
            return;
    }

    // Clear everything
    ui->availableSquares->clear();
    ui->queenPlacement->clear();

    clearBoard(square);
}

void MainWin::actions() {
    auto *action = (QAction *) sender();

    // Show information about author
    if (action == ui->actionAbout_Author) {
        QMessageBox::information(this, "Queen Move",
                                 "Find me in the Web!\n"
                                 "Telegram: @vlad_is_real\n"
                                 "GMail: vladislav.kolyadenko@gmail.com\n"
                                 "Instagram: @ncks_gwc");
    }
    // Show information about program
    else if (action == ui->actionAbout_Program) {
        QMessageBox::information(this, "Queen Move",
                                 "Simple program to calculate available moves for queen\n"
                                 "May have a lot of bugs and mistakes\n"
                                 "Used technologies:\n"
                                 "C++, Qt, CMake");
    }
    // Clear all
    else if (action == ui->actionRefresh) {
        QPushButton *btn = nullptr;

        ui->availableSquares->clear();
        ui->queenPlacement->clear();

        clearBoard(btn);
    }
    // Exit the program
    else if (action == ui->actionExit)
        QApplication::quit();

    // Something gone wrong
    else {
        qDebug() << "Invalid action!";
        ui->statusBar->showMessage("Invalid action!", 5000);
        return;
    }
}

void MainWin::clearBoard(QPushButton *btn) {
    // Delete crown icon and return previous colors to buttons
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
    // Get button from grid using its position
    QLayoutItem *item = ui->gridLayout->itemAtPosition(x, y);
    QWidget *widget = item->widget();
    auto *button = dynamic_cast<QPushButton *>(widget);

    return button;
}

bool MainWin::findSquareName(const QString &squareName) {
    QPair<uint8_t, uint8_t> squarePos = Board::parseSquareNameToMatrixPos(squareName);

    // Check if entered square name is presented in valid moves
    for (uint8_t i = 0; i < BOARD_SIZE; ++i) {
        for (uint8_t j = 0; j < BOARD_SIZE; ++j) {
            if (brd->getMBoard()(squarePos.first, squarePos.second))
                return true;
        }
    }

    return false;
}