#ifndef QCHECKLIST
#define QCHECKLIST

#include <QWidget>
#include <QComboBox>
#include <QStandardItemModel>
#include <QEvent>
#include <QLineEdit>
#include <QListView>
#include <vector>
#include <QStyledItemDelegate>

using std::vector;

class QCheckList : public QComboBox
{
    Q_OBJECT

protected:
    bool eventFilter(QObject* object, QEvent* event);

private:
    QStandardItemModel* model;

    void updateText();
    int globalCheckState();

private slots:
    void on_modelDataChanged();
    void on_itemPressed(const QModelIndex &index);

public:
    QCheckList(QWidget* parent = 0);
    ~QCheckList();
    void setup();
    vector<bool> getChecked();
    void uncheckAll();

    class QCheckListStyledItemDelegate : public QStyledItemDelegate
    {

    public:
        QCheckListStyledItemDelegate(QObject* parent = 0) : QStyledItemDelegate(parent) {}
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    };

};

#endif // QCHECKLIST
