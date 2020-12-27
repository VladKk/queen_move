#ifndef QUEEN_MOVE_MAIN_WIN_H
#define QUEEN_MOVE_MAIN_WIN_H

#include <QMainWindow>
#include <QPushButton>

#include "../board/board.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWin; }
QT_END_NAMESPACE

class MainWin final : public QMainWindow {
Q_OBJECT

public:
    explicit MainWin(QWidget *parent = nullptr);

    ~MainWin() final;

private slots:

    void getAvailableSquaresTextView();

    void getAvailableSquaresGraphicalView();

    void switchView();

    void switchMode();

    void actions();

private:
    Ui::MainWin *ui;

    Board *brd;

    QPushButton *getButtonFromGrid(int x, int y);

    void clearBoard(QPushButton *btn);

    bool findSquareName(const QString &squareName);
};

#endif //QUEEN_MOVE_MAIN_WIN_H
