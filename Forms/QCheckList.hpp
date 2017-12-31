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
    QStandardItemModel* m_model;
    QString m_noneCheckedText = tr("Any");
    QString m_allCheckedText;
    QString m_unknownlyCheckedText;

signals:
    void globalCheckStateChanged(int);

public:
    QCheckList(QWidget* _parent = 0);

    ~QCheckList();

    void setAllCheckedText(const QString &text);

    void setNoneCheckedText(const QString &text);

    void setUnknownlyCheckedText(const QString &text);

    QStandardItem *addCheckItem(const QString &label, const QVariant &data, const Qt::CheckState checkState);

    void addCheckItems(const QStringList &label, const QVariant &data, const Qt::CheckState checkState);

    std::vector<bool> getChecked();

    void uncheckAll();

    int globalCheckState();

protected:
    bool eventFilter(QObject* _object, QEvent* _event);

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

        void paint(QPainter * painter_, const QStyleOptionViewItem & option_, const QModelIndex & index_) const;
    };
};

#endif // QCHECKLIST
