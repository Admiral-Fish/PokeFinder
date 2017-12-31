#include "QCheckList.hpp"

QCheckList::QCheckList(QWidget *_parent) : QComboBox(_parent)
{
    m_model = new QStandardItemModel();
    setModel(m_model);

    setEditable(true);
    lineEdit()->setReadOnly(true);
    lineEdit()->installEventFilter(this);
    setItemDelegate(new QCheckListStyledItemDelegate(this));

    connect(lineEdit(), &QLineEdit::selectionChanged, lineEdit(), &QLineEdit::deselect);
    connect((QListView*) view(), SIGNAL(pressed(QModelIndex)), this, SLOT(on_itemPressed(QModelIndex)));
    connect(m_model, SIGNAL(dataChanged(QModelIndex, QModelIndex, QVector<int>)), this, SLOT(on_modelDataChanged()));
}

QCheckList::~QCheckList()
{
    delete m_model;
}

void QCheckList::setAllCheckedText(const QString &text)
{
    m_allCheckedText = text;
    updateText();
}

void QCheckList::setNoneCheckedText(const QString &text)
{
    m_noneCheckedText = text;
    updateText();
}

void QCheckList::setUnknownlyCheckedText(const QString &text)
{
    m_unknownlyCheckedText = text;
    updateText();
}

QStandardItem *QCheckList::addCheckItem(const QString &label, const QVariant &data, const Qt::CheckState checkState)
{
    QStandardItem* item = new QStandardItem(label);
    item->setCheckState(checkState);
    item->setData(data);
    item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);

    m_model->appendRow(item);

    updateText();

    return item;
}

void QCheckList::addCheckItems(const QStringList &label, const QVariant &data, const Qt::CheckState checkState)
{
    for (auto i = 0; i < label.length(); i++)
    {
        QStandardItem* item = new QStandardItem(label[i]);
        item->setCheckState(checkState);
        item->setData(data);
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);

        m_model->appendRow(item);
    }
    updateText();
}

std::vector<bool> QCheckList::getChecked()
{
    std::vector<bool> result;

    if (globalCheckState() == Qt::Unchecked)
    {
        for (auto i = 0; i < m_model->rowCount(); i++)
        {
            result.push_back(true);
        }
    }
    else
    {
        for (auto i = 0; i < m_model->rowCount(); i++)
        {
            if (m_model->item(i)->checkState() == Qt::Checked)
            {
                result.push_back(true);
            }
            else
            {
                result.push_back(false);
            }
        }
    }
    return result;
}

void QCheckList::uncheckAll()
{
    for (auto i = 0; i < m_model->rowCount(); i++)
    {
        m_model->item(i)->setData(Qt::Unchecked, Qt::CheckStateRole);
    }
}

/**
 * @brief Computes the global state of the checklist :
 *      - if there is no item: StateUnknown
 *      - if there is at least one item partially checked: StateUnknown
 *      - if all items are checked: Qt::Checked
 *      - if no item is checked: Qt::Unchecked
 *      - else: Qt::PartiallyChecked
 */
int QCheckList::globalCheckState()
{
    int nbRows = m_model->rowCount(), nbChecked = 0, nbUnchecked = 0;

    if (nbRows == 0)
    {
        return StateUnknown;
    }

    for (int i = 0; i < nbRows; i++)
    {
        if (m_model->item(i)->checkState() == Qt::Checked)
        {
            nbChecked++;
        }
        else if (m_model->item(i)->checkState() == Qt::Unchecked)
        {
            nbUnchecked++;
        }
        else
        {
            return StateUnknown;
        }
    }

    return nbChecked == nbRows ? Qt::Checked : nbUnchecked == nbRows ? Qt::Unchecked : Qt::PartiallyChecked;
}

bool QCheckList::eventFilter(QObject *_object, QEvent *_event)
{
    if (_object == lineEdit() && _event->type() == QEvent::MouseButtonPress)
    {
        showPopup();
        return true;
    }

    return false;
}

void QCheckList::updateText()
{
    QString text;

    switch (globalCheckState())
    {
    case Qt::Checked:
        text = m_allCheckedText;
        break;

    case Qt::Unchecked:
        text = m_noneCheckedText;
        break;

    case Qt::PartiallyChecked:
        for (int i = 0; i < m_model->rowCount(); i++)
        {
            if (m_model->item(i)->checkState() == Qt::Checked)
            {
                if (!text.isEmpty())
                {
                    text+= ", ";
                }

                text+= m_model->item(i)->text();
            }
        }
        break;

    default:
        text = m_unknownlyCheckedText;
    }

    lineEdit()->setText(text);
}

void QCheckList::on_modelDataChanged()
{
    updateText();
    emit globalCheckStateChanged(globalCheckState());
}

void QCheckList::on_itemPressed(const QModelIndex &index)
{
    QStandardItem* item = m_model->itemFromIndex(index);

    if (item->checkState() == Qt::Checked)
    {
        item->setCheckState(Qt::Unchecked);
    }
    else
    {
        item->setCheckState(Qt::Checked);
    }
}

void QCheckList::QCheckListStyledItemDelegate::paint(QPainter *painter_, const QStyleOptionViewItem &option_, const QModelIndex &index_) const
{
    QStyleOptionViewItem & refToNonConstOption = const_cast<QStyleOptionViewItem &>(option_);
    refToNonConstOption.showDecorationSelected = false;
    QStyledItemDelegate::paint(painter_, refToNonConstOption, index_);
}
