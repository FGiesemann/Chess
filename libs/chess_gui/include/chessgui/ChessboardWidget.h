/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#ifndef CHESSGUI_CHESSBOARDWIDGET_H
#define CHESSGUI_CHESSBOARDWIDGET_H

#include <QGraphicsScene>
#include <QGraphicsView>

namespace chessgui {

class ChessboardWidget : public QGraphicsView {
    Q_OBJECT
public:
    explicit ChessboardWidget(QWidget *parent = nullptr);
protected:
    void resizeEvent(QResizeEvent *event) override;
private:
    void drawBoard();

    QGraphicsScene *m_scene;
};

} // namespace chessgui

#endif
