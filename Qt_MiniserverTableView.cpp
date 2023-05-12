#include "Qt_MiniserverTableView.h"

Qt_MiniserverTableView::Qt_MiniserverTableView(QWidget *parent)
	: QTableView(parent)
{
	ui.setupUi(this);
}

Qt_MiniserverTableView::~Qt_MiniserverTableView()
{}


Qt_MiniserverTableView::Qt_MiniserverTableView(QList<CMiniserver>* miniservers, QWidget* parent)
    : QTableView(parent), m_model(new CMiniserverTableModel(miniservers, this))
{
    setModel(m_model);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    // Set up the "Connect Config" push button delegate
    QStyledItemDelegate* buttonDelegate = new QStyledItemDelegate(this);
    setItemDelegateForColumn(6, buttonDelegate);
    connect(buttonDelegate, &QStyledItemDelegate::commitData, [this](QWidget* editor) {
        auto button = qobject_cast<QPushButton*>(editor);
        if (button)
        {
            // Perform the "Connect Config" action for the selected row
            // ...
        }
        });

    // Set up the config language ComboBox delegate
    CComboBoxLanguageDelegate* comboBoxDelegate = new CComboBoxLanguageDelegate(this);
    setItemDelegateForColumn(8, comboBoxDelegate);
}


