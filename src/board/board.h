#ifndef QUEEN_MOVE_BOARD_H
#define QUEEN_MOVE_BOARD_H

#include <QString>
#include <QGenericMatrix>

#define BOARD_SIZE 8

class Board {
public:
    Board();

    ~Board() = default;

    static QPair<uint8_t, uint8_t> parseSquareNameToMatrixPos(QString squareName);

    static QString parseMatrixPosToSquareName(QPair<uint8_t, uint8_t> matrixPos);

    void getAllAvailableMoves(QPair<uint8_t, uint8_t> queenPos);

    void clearBoard();

    [[nodiscard]]
    const QGenericMatrix<BOARD_SIZE, BOARD_SIZE, bool> &getMBoard() const;

private:
    // Use board as matrix
    QGenericMatrix<BOARD_SIZE, BOARD_SIZE, bool> m_board;
};

#endif //QUEEN_MOVE_BOARD_H
