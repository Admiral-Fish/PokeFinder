#include "QCheckList.hpp"

QCheckList::QCheckList(QWidget *_parent) : QComboBox(_parent)
{
    model = new QStandardItemModel();
    setModel(model);

    setEditable(true);
    lineEdit()->setReadOnly(true);
    lineEdit()->installEventFilter(this);

    connect((QListView*) view(), SIGNAL(pressed(QModelIndex)), this, SLOT(on_itemPressed(QModelIndex)));
    connect(model, SIGNAL(dataChanged(QModelIndex, QModelIndex, QVector<int>)), this, SLOT(on_modelDataChanged()));
}

QCheckList::~QCheckList()
{
    delete model;
}

void QCheckList::setup()
{
    for (int i = 0; i < model->rowCount(); i++)
    {
        QStandardItem *item = model->item(i);
        item->setCheckState(Qt::Unchecked);
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    }
}

void QCheckList::setAllCheckedText(const QString &text)
{
    allCheckedText = text;
    updateText();
}

void QCheckList::setNoneCheckedText(const QString &text)
{
    noneCheckedText = text;
    updateText();
}

vector<bool> QCheckList::getChecked()
{
    vector<bool> result;

    if (globalCheckState() == Qt::Unchecked)
    {
        for (auto i = 0; i < model->rowCount(); i++)
            result.push_back(true);
    }
    else
    {
        for (auto i = 0; i < model->rowCount(); i++)
        {
            if (model->item(i)->checkState() == Qt::Checked)
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
    for (auto i = 0; i < model->rowCount(); i++)
        model->item(i)->setData(Qt::Unchecked, Qt::CheckStateRole);
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
    int nbRows = model->rowCount(), nbChecked = 0, nbUnchecked = 0;

    if (nbRows == 0)
        return 3;

    for (int i = 0; i < nbRows; i++)
    {
        if (model->item(i)->checkState() == Qt::Checked)
        {
            nbChecked++;
        }
        else if (model->item(i)->checkState() == Qt::Unchecked)
        {
            nbUnchecked++;
        }
        else
        {
            return 3;
        }
    }

    return nbChecked == nbRows ? Qt::Checked : nbUnchecked == nbRows ? Qt::Unchecked : Qt::PartiallyChecked;
}

bool QCheckList::eventFilter(QObject *object, QEvent *event)
{
    if (object == lineEdit() && event->type() == QEvent::MouseButtonPress)
    {
        showPopup();
        return true;
    }

    return false;
}

void QCheckList::changeEvent(QEvent *event)
{
    if (event != NULL)
    {
        switch (event->type())
        {
            case QEvent::LanguageChange:
                updateText();
                break;
            default:
                break;
        }
    }
}

void QCheckList::updateText()
{
    QString text;

    switch (globalCheckState())
    {
        case Qt::Checked:
            text = tr("Any");
            break;

        case Qt::Unchecked:
            text = tr("Any");
            break;

        case Qt::PartiallyChecked:
            for (int i = 0; i < model->rowCount(); i++)
            {
                if (model->item(i)->checkState() == Qt::Checked)
                {
                    if (!text.isEmpty())
                        text+= ", ";

                    text += itemText(i);
                }
            }
            break;

        default:
            text = tr("Any");
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
    QStandardItem* item = model->itemFromIndex(index);

    if (item->checkState() == Qt::Checked)
    {
        item->setCheckState(Qt::Unchecked);
    }
    else
    {
        item->setCheckState(Qt::Checked);
    }
}

void QCheckList::QCheckListStyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem &refToNonConstOption = const_cast<QStyleOptionViewItem &>(option);
    refToNonConstOption.showDecorationSelected = false;
    QStyledItemDelegate::paint(painter, refToNonConstOption, index);
}
