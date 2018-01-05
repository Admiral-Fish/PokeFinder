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

/**
 * @brief QComboBox with support of checkboxes
 * http://stackoverflow.com/questions/8422760/combobox-of-checkboxes
 */
class QCheckList : public QComboBox
{
    Q_OBJECT

public:
    static const int StateUnknown = 3;

private:
    QStandardItemModel* model;
    QString noneCheckedText = tr("Any");
    QString allCheckedText;
    QString unknownlyCheckedText;

signals:
    void GlobalCheckStateChanged(int);

public:
    QCheckList(QWidget* _parent = 0);
    ~QCheckList();
    void SetAllCheckedText(const QString &text);
    void SetNoneCheckedText(const QString &text);
    void SetUnknownlyCheckedText(const QString &text);
    QStandardItem *AddCheckItem(const QString &label, const QVariant &data, const Qt::CheckState checkState);
    void AddCheckItems(const std::vector<QString> &label, const QVariant &data, const Qt::CheckState checkState);
    std::vector<bool> GetChecked();
    void UncheckAll();
    int GlobalCheckState();

protected:
    bool eventFilter(QObject* _object, QEvent* _event);

private:
    void UpdateText();

private slots:
    void on_modelDataChanged();
    void on_itemPressed(const QModelIndex &index);

public:
    class QCheckListStyledItemDelegate : public QStyledItemDelegate
    {

    public:
        QCheckListStyledItemDelegate(QObject* parent = 0) : QStyledItemDelegate(parent) {}
        void paint(QPainter * painter_, const QStyleOptionViewItem & option_, const QModelIndex & index_) const;

    };
};

#endif // QCHECKLIST
