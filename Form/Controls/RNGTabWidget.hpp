#ifndef RNGTABWIDGET_HPP
#define RNGTABWIDGET_HPP

#include <QTabWidget>

class RNGTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    RNGTabWidget(QWidget *parent = nullptr);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

signals:
    void transferSettingsTriggered();
    void transferFiltersTriggered();
};

#endif // RNGTABWIDGET_HPP
