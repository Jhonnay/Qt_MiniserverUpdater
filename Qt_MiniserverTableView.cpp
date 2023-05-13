#include "Qt_MiniserverTableView.h"


Qt_MiniserverTableView::Qt_MiniserverTableView(QWidget *parent)
	: QTableView(parent)
{
    Qt_MiniserverTableView(NULL, this); //does not work
}



Qt_MiniserverTableView::Qt_MiniserverTableView(QList<CMiniserver>* miniservers, QWidget* parent)
    : QTableView(parent), m_model(new CMiniserverTableModel(miniservers, this))
{
    setModel(m_model);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setMinimumWidth(35);
    setSortingEnabled(true);
    //horizontalHeader()->setSectionsMovable(true);     //set Header moveable
    
    //Test
    //QPalette palette;
    //palette.setColor(QPalette::Highlight, QColor(200, 225, 255)); // set light blue color for selection
    //setPalette(palette);
    //QString styleSheet = "QTableView::item:selected { selection-background-color: rgba(255, 255, 255, 10); }";
    //setStyleSheet(styleSheet);


    // Set up the "Connect Config" push button delegate
    CConnectConfigButtonDelegate* buttonDelegate = new CConnectConfigButtonDelegate(this);
    setItemDelegateForColumn(6, buttonDelegate);
    connect(buttonDelegate, &CConnectConfigButtonDelegate::clicked, this, &Qt_MiniserverTableView::handleConnectConfigClicked);
    
    // Set up the config language ComboBox delegate
    CComboBoxLanguageDelegate* comboBoxDelegate = new CComboBoxLanguageDelegate(this);
    setItemDelegateForColumn(8, comboBoxDelegate);
    resizeColumnsToContents();
}


Qt_MiniserverTableView::~Qt_MiniserverTableView()
{}


void Qt_MiniserverTableView::handleConnectConfigClicked(const QModelIndex& index) {
    int row = index.row();
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button)
        return;

        QModelIndex _index = indexAt(button->geometry().center());
    if (!index.isValid())
        return;

    const CMiniserver& miniserver = m_model->miniserverlist->at(_index.row());
    emit  ConnectConfigClicked(_index, miniserver);
}
