#include <QDebug>

#include "board.h"

Board::Board() { clearBoard(); }

QPair<uint8_t, uint8_t> Board::parseSquareNameToMatrixPos(QString squareName) {
    QPair<uint8_t, uint8_t> matrixPos;

    switch (squareName[0].unicode()) {
        case 'a':
        case 'A':
            matrixPos.second = 0;
            break;
        case 'b':
        case 'B':
            matrixPos.second = 1;
            break;
        case 'c':
        case 'C':
            matrixPos.second = 2;
            break;
        case 'd':
        case 'D':
            matrixPos.second = 3;
            break;
        case 'e':
        case 'E':
            matrixPos.second = 4;
            break;
        case 'f':
        case 'F':
            matrixPos.second = 5;
            break;
        case 'g':
        case 'G':
            matrixPos.second = 6;
            break;
        case 'h':
        case 'H':
            matrixPos.second = 7;
            break;
        default:
            qDebug() << "Invalid square name!";
            return matrixPos;
    }

    matrixPos.first = BOARD_SIZE - (squareName[1].unicode() - 48);

    return matrixPos;
}

QString Board::parseMatrixPosToSquareName(QPair<uint8_t, uint8_t> matrixPos) {
    QString squareName;

    switch (matrixPos.second) {
        case 0:
            squareName += "A";
            break;
        case 1:
            squareName += "B";
            break;
        case 2:
            squareName += "C";
            break;
        case 3:
            squareName += "D";
            break;
        case 4:
            squareName += "E";
            break;
        case 5:
            squareName += "F";
            break;
        case 6:
            squareName += "G";
            break;
        case 7:
            squareName += "H";
            break;
        default:
            qDebug() << "Invalid matrix position!";
            return squareName;
    }

    squareName += QString::number(BOARD_SIZE - matrixPos.first);

    return squareName;
}

void Board::getAllAvailableMoves(QPair<uint8_t, uint8_t> queenPos) {
    clearBoard();

    int calcPos;

    for (int row = 0; row < BOARD_SIZE; ++row)
        m_board(row, queenPos.second) = true;

    for (int col = 0; col < BOARD_SIZE; ++col)
        m_board(queenPos.first, col) = true;

    for (int move = 0; move < BOARD_SIZE; ++move) {
        calcPos = queenPos.first - move + queenPos.second;

        if (calcPos < 0 || calcPos >= BOARD_SIZE)
            continue;

        m_board(move, calcPos) = true;
    }

    for (int move = 0; move < BOARD_SIZE; ++move) {
        calcPos = move - queenPos.first + queenPos.second;

        if (calcPos < 0 || calcPos >= BOARD_SIZE)
            continue;

        m_board(move, calcPos) = true;
    }


    m_board(queenPos.first, queenPos.second) = false;
}

void Board::clearBoard() {
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; ++i) {
        int row = i / BOARD_SIZE;
        int col = i % BOARD_SIZE;

        m_board(row, col) = false;
    }
}

const QGenericMatrix<BOARD_SIZE, BOARD_SIZE, bool> &Board::getMBoard() const { return m_board; }
