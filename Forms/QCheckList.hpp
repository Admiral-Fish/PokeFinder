#ifndef QCHECKLIST
#define QCHECKLIST

#include <QWidget>
#include <QComboBox>
#include <QStandardItemModel>
#include <QLineEdit>
#include <QEvent>
#include <QStyledItemDelegate>
#include <QListView>
#include <vector>

using std::vector;

/**
 * @brief QComboBox with support of checkboxes
 * http://stackoverflow.com/questions/8422760/combobox-of-checkboxes
 */
class QCheckList : public QComboBox
{
    Q_OBJECT

public:
    static const int STATEUNKNOWN = 3;

private:
    QStandardItemModel* model;
    QString noneCheckedText = tr("Any");
    QString allCheckedText;
    QString unknownlyCheckedText;

signals:
    void globalCheckStateChanged(int);

public:
    QCheckList(QWidget* _parent = 0);
    ~QCheckList();
    void setAllCheckedText(const QString &text);
    void setNoneCheckedText(const QString &text);
    void setUnknownlyCheckedText(const QString &text);
    QStandardItem *addCheckItem(const QString &label, const QVariant &data, const Qt::CheckState checkState);
    void addCheckItems(const std::vector<QString> &label, const QVariant &data, const Qt::CheckState checkState);
    vector<bool> getChecked();
    void uncheckAll();
    int globalCheckState();

protected:
    bool eventFilter(QObject* object, QEvent* event);

private:
    void updateText();

private slots:
    void on_modelDataChanged();
    void on_itemPressed(const QModelIndex &index);

public:
    class QCheckListStyledItemDelegate : public QStyledItemDelegate
    {

    public:
        QCheckListStyledItemDelegate(QObject* parent = 0) : QStyledItemDelegate(parent) {}
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    };
};

#endif // QCHECKLIST
