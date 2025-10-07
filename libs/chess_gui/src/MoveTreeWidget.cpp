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
    if (index.column() != MoveTreeModel::MoveNumberColumn) {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    auto opt = option;
    initStyleOption(&opt, index);
    auto originalText = opt.text;
    opt.text.clear();
    auto *style = (opt.widget != nullptr) ? opt.widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);

    auto textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &opt, opt.widget);
    QString prefix;
    auto number = originalText;
    bool hasPrefix = false;

    if (originalText.startsWith("▶")) {
        prefix = originalText.left(1);
        number = originalText.mid(1).trimmed();
        hasPrefix = true;
    }

    painter->save();
    painter->setFont(opt.font);
    painter->setPen(opt.palette.color(opt.state & QStyle::State_Selected ? QPalette::HighlightedText : QPalette::Text));

    if (hasPrefix) {
        QRect prefixRect = textRect;
        prefixRect.setWidth(prefix_width);
        painter->drawText(prefixRect, Qt::AlignLeft | Qt::AlignVCenter, prefix);
    }
    QRect numberRect = textRect;
    numberRect.setLeft(numberRect.left() + prefix_width);
    painter->drawText(numberRect, Qt::AlignLeft | Qt::AlignVCenter, number);
    painter->restore();
}

auto MoveTreeDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const -> QSize {
    QSize size = QStyledItemDelegate::sizeHint(option, index);

    if (index.column() == MoveTreeModel::MoveNumberColumn) {
        size.setWidth(size.width() + prefix_width);
    }

    return size;
}

MoveTreeWidget::MoveTreeWidget(QWidget *parent) : QTreeView(parent) {
    setupUI();

    connect(this, &QTreeView::clicked, this, &MoveTreeWidget::onItemClicked);
    connect(this, &QTreeView::doubleClicked, this, &MoveTreeWidget::onItemDoubleClicked);
}

auto MoveTreeWidget::setupUI() -> void {
    setItemDelegate(new MoveTreeDelegate(this));
    setAlternatingRowColors(true);
    setRootIsDecorated(true);
    setHeaderHidden(false);

    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);

    QPalette p = palette();
    p.setColor(QPalette::Base, QColor(245, 245, 245));
    setPalette(p);

    setFont(QFont("Inter", 10));
    header()->setStretchLastSection(true);
    header()->setSectionResizeMode(QHeaderView::Interactive);
}

auto MoveTreeWidget::setModel(MoveTreeModel *model) -> void {
    if (m_model != nullptr) {
        disconnect(selectionModel(), nullptr, this, nullptr);
    }
    m_model = model;
    QTreeView::setModel(model);
    if (m_model == nullptr) {
        return;
    }

    connect(selectionModel(), &QItemSelectionModel::selectionChanged, this, &MoveTreeWidget::onSelectionChanged);
    header()->setSectionResizeMode(MoveTreeModel::MoveNumberColumn, QHeaderView::ResizeToContents);
    header()->setSectionResizeMode(MoveTreeModel::WhiteMoveColumn, QHeaderView::Stretch);
    header()->setSectionResizeMode(MoveTreeModel::BlackMoveColumn, QHeaderView::Stretch);
}

auto MoveTreeWidget::onItemClicked(const QModelIndex &index) -> void {
    if (!index.isValid() || m_model == nullptr) {
        return;
    }

    auto cursor = m_model->cursor_from_index(index);
    if (cursor) {
        emit moveClicked(*cursor);
    }
}

auto MoveTreeWidget::onItemDoubleClicked(const QModelIndex &index) -> void {
    if (!index.isValid() || m_model == nullptr) {
        return;
    }
    auto cursor = m_model->cursor_from_index(index);
    if (cursor) {
        emit moveDoubleClicked(*cursor);
    }
}

auto MoveTreeWidget::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected) -> void {
    Q_UNUSED(deselected)
    if (!m_model) {
        return;
    }
    if (selected.isEmpty()) {
        emit selectionCleared();
        return;
    }
    QModelIndexList indexes = selected.indexes();
    if (indexes.isEmpty()) {
        emit selectionCleared();
        return;
    }
    QModelIndex index = indexes.first();
    for (const auto &idx : indexes) {
        if (idx.column() == 0) {
            index = idx;
            break;
        }
    }
    auto cursor = m_model->cursor_from_index(index);
    if (cursor) {
        emit moveSelected(*cursor);
    }
}

} // namespace chessgui
