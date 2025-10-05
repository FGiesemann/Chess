/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#include "chessgui/MoveTreeWidget.h"

#include <QApplication>
#include <QHeaderView>

namespace chessgui {

MoveTreeDelegate::MoveTreeDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

void MoveTreeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    // Für andere Spalten: Standard-Rendering
    if (index.column() != MoveTreeModel::MoveNumberColumn) {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    // Standard-Rendering für Hintergrund, Selection, etc. (OHNE Text)
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    QString originalText = opt.text;
    opt.text.clear(); // Text nicht von Qt zeichnen lassen

    QStyle *style = opt.widget ? opt.widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);

    // Hole den eingerückten Textbereich von Qt
    QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &opt, opt.widget);

    // Text aufteilen
    QString text = originalText;
    QString prefix;
    QString number = text;
    bool hasPrefix = false;

    if (text.startsWith("▶")) {
        prefix = text.left(1);
        number = text.mid(1).trimmed();
        hasPrefix = true;
    }

    painter->save();
    painter->setFont(opt.font);
    painter->setPen(opt.palette.color(opt.state & QStyle::State_Selected ? QPalette::HighlightedText : QPalette::Text));

    const int prefixWidth = 15;

    if (hasPrefix) {
        // Zeichne Präfix in den ersten 15 Pixeln
        QRect prefixRect = textRect;
        prefixRect.setWidth(prefixWidth);
        painter->drawText(prefixRect, Qt::AlignLeft | Qt::AlignVCenter, prefix);

        // Zeichne Nummer nach dem Präfix
        QRect numberRect = textRect;
        numberRect.setLeft(numberRect.left() + prefixWidth);
        painter->drawText(numberRect, Qt::AlignLeft | Qt::AlignVCenter, number);
    } else {
        // Kein Präfix: rücke die Nummer um 15 Pixel ein
        QRect numberRect = textRect;
        numberRect.setLeft(numberRect.left() + prefixWidth);
        painter->drawText(numberRect, Qt::AlignLeft | Qt::AlignVCenter, number);
    }

    painter->restore();
}

auto MoveTreeDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const -> QSize {
    QSize size = QStyledItemDelegate::sizeHint(option, index);

    // Für die erste Spalte: füge 15 Pixel für das Präfix hinzu
    if (index.column() == MoveTreeModel::MoveNumberColumn) {
        size.setWidth(size.width() + 15);
    }

    return size;
}

MoveTreeWidget::MoveTreeWidget(QWidget *parent) : QTreeView(parent) {
    setItemDelegate(new MoveTreeDelegate(this));
    setAlternatingRowColors(true);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setHeaderHidden(false);

    QPalette p = palette();
    p.setColor(QPalette::Base, QColor(245, 245, 245));
    setPalette(p);

    setFont(QFont("Inter", 10));
}

auto MoveTreeWidget::setupModel(MoveTreeModel *model) -> void {
    if (model == nullptr)
        return;
    this->setModel(model);

    QHeaderView *header = this->header();
    header->setSectionResizeMode(MoveTreeModel::MoveNumberColumn, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(MoveTreeModel::WhiteMoveColumn, QHeaderView::Stretch);
    header->setSectionResizeMode(MoveTreeModel::BlackMoveColumn, QHeaderView::Stretch);
}

} // namespace chessgui
