#include "QCheckList.hpp"

QCheckList::QCheckList(QWidget *parent) : QComboBox(parent)
{
    model = new QStandardItemModel();
    setModel(model);

    setEditable(true);
    lineEdit()->setReadOnly(true);
    lineEdit()->installEventFilter(this);
    setItemDelegate(new QCheckListStyledItemDelegate(this));

    connect(lineEdit(), &QLineEdit::selectionChanged, lineEdit(), &QLineEdit::deselect);
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

vector<bool> QCheckList::getChecked()
{
    vector<bool> result;

    if (globalCheckState() == Qt::Unchecked || globalCheckState() == Qt::Checked)
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
        model->item(i)->setCheckState(Qt::Unchecked);
}

int QCheckList::globalCheckState()
{
    int total = model->rowCount(), checked = 0, unchecked = 0;

    for (int i = 0; i < total; i++)
    {
        if (model->item(i)->checkState() == Qt::Checked)
        {
            checked++;
        }
        else if (model->item(i)->checkState() == Qt::Unchecked)
        {
            unchecked++;
        }
    }

    return checked == total ? Qt::Checked : unchecked == total ? Qt::Unchecked : Qt::PartiallyChecked;
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
                        text += ", ";

                    text += model->item(i)->text();
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
