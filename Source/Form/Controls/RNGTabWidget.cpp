#include "RNGTabWidget.hpp"
#include <QAction>
#include <QContextMenuEvent>
#include <QMenu>

RNGTabWidget::RNGTabWidget(QWidget *parent) : QTabWidget(parent)
{
    auto *transferSettings = new QAction(tr("Transfer Settings to Generator"), this);
    connect(transferSettings, &QAction::triggered, this, [=] { emit transferSettingsTriggered(); });
    addAction(transferSettings);

    auto *transferFilters = new QAction(tr("Transfer Filters to Generator"), this);
    connect(transferFilters, &QAction::triggered, this, [=] { emit transferFiltersTriggered(); });
    addAction(transferFilters);
}

void RNGTabWidget::contextMenuEvent(QContextMenuEvent *event)
{
    if (currentIndex() == 1)
    {
        QMenu::exec(actions(), event->globalPos(), nullptr, this);
    }
}
