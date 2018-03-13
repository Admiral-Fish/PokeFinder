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
    void changeEvent(QEvent*);

signals:
    void globalCheckStateChanged(int);

private:
    QStandardItemModel* model;
    QString noneCheckedText;
    QString allCheckedText;

    void updateText();

private slots:
    void on_modelDataChanged();
    void on_itemPressed(const QModelIndex &index);

public:
    QCheckList(QWidget* _parent = 0);
    ~QCheckList();
    void setup();
    void setAllCheckedText(const QString &text);
    void setNoneCheckedText(const QString &text);
    vector<bool> getChecked();
    void uncheckAll();
    int globalCheckState();

    class QCheckListStyledItemDelegate : public QStyledItemDelegate
    {

    public:
        QCheckListStyledItemDelegate(QObject* parent = 0) : QStyledItemDelegate(parent) {}
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    };

};

#endif // QCHECKLIST
