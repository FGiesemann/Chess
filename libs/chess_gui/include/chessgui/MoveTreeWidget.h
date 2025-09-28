/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#ifndef CHESSGUI_MOVETREEWIDGET_H
#define CHESSGUI_MOVETREEWIDGET_H

#include <QTreeView>

#include "chessgui/MoveTreeModel.h"

namespace chessgui {

class MoveTreeWidget : public QTreeView {
    Q_OBJECT
public:
    explicit MoveTreeWidget(QWidget *parent = nullptr);
    ~MoveTreeWidget() override = default;

    void setupModel(MoveTreeModel *model);
};

} // namespace chessgui

#endif
