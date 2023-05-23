#include "Qt_CreateEditMiniserver.h"
#include "MyConstants.h"

Qt_CreateEditMiniserver::Qt_CreateEditMiniserver(const QString& title, QWidget* parent)
	: QDialog(parent)
{
    setWindowTitle(title);

    adminPasswordLineEdit->setEchoMode(QLineEdit::Password);
    
    QFont font = labelHint->font();
    font.setPointSize(font.pointSize() - 1); // Decrease the font size by 2 points
    font.setItalic(true); // Set the font style to italic
    labelHint->setFont(font);
    

    formLayout->addRow("Serial Number*:", serialNumberLineEdit);
    formLayout->addRow("Admin User:*", adminUserLineEdit);
    formLayout->addRow("Admin Password:*", adminPasswordLineEdit);
    formLayout->addRow("Local IP:", localIPLineEdit);

    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(labelHint);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    connect(okButton, &QPushButton::clicked, this, &Qt_CreateEditMiniserver::accept);
    connect(cancelButton, &QPushButton::clicked, this, &Qt_CreateEditMiniserver::reject);
}


Qt_CreateEditMiniserver::~Qt_CreateEditMiniserver()
{
    delete serialNumberLineEdit; 
    delete adminUserLineEdit; 
    delete adminPasswordLineEdit; 
    delete localIPLineEdit; 
    delete formLayout;
    delete buttonLayout;
    delete mainLayout;
    delete okButton;
    delete cancelButton;
    delete labelHint;
}

CMiniserver Qt_CreateEditMiniserver::createDialog(const QString& title, CMiniserver* miniserver, QList<CMiniserver>* list)
{
    Qt_CreateEditMiniserver dialog(title);
    if (miniserver) {
        dialog.serialNumberLineEdit->setText(QString::fromStdString(miniserver->getSerialNumber()));
        dialog.adminUserLineEdit->setText(QString::fromStdString(miniserver->getAdminUser()));
        dialog.adminPasswordLineEdit->setText(QString::fromStdString(miniserver->getAdminPassword()));
        dialog.localIPLineEdit->setText(QString::fromStdString(miniserver->getLocalIP()));
    }
    else {
        dialog.serialNumberLineEdit->setText(QString::fromStdString(MyConstants::Strings::MS_Default_partial_SN));
    }

    int dialogResult = dialog.exec();

    if (dialogResult == QDialog::Accepted) {
        QString serial = dialog.serialNumberLineEdit->text();
        serial = serial.remove(':');
        int lenght = serial.length();
        if (dialog.serialNumberLineEdit->text().remove(':').length() != MyConstants::Strings::MS_Default_dummy_SN.length()) {
            QMessageBox::information(nullptr, "Data not sufficient", QString::fromStdString(MyConstants::Strings::MessageBox_CraeteMSDialog_Fields_SerialNumber_Length));
            return CMiniserver();  // Return an empty CMiniserver object if SN lenght not correct
        }

        if (dialog.adminPasswordLineEdit->text().isEmpty() || dialog.adminUserLineEdit->text().isEmpty()) {
            QMessageBox::information(nullptr, "Data not sufficient", QString::fromStdString(MyConstants::Strings::MessageBox_CraeteMSDialog_Fields_Not_Filled));
            return CMiniserver(); // Return an empty CMiniserver object if username or password not provided

        }
        
        bool serialNumberalreadyInList = false;
        for (int i = 0; i < list->count(); i++) {
            if ((*list)[i].getSerialNumber() == serial.toStdString()) {
                serialNumberalreadyInList = true;
            }
        }
        if (serialNumberalreadyInList && !title.contains("Edit")) {
            QMessageBox::information(nullptr, "Serial Number duplicate", QString::fromStdString(MyConstants::Strings::MessageBox_SerialNumber_duplicate));
            return CMiniserver(); // Return an empty CMiniserver object if username or password not provided
        }

        CMiniserver result = dialog.getMiniserver();
        result.setSerialNumber(serial.toStdString());
        return result;
    }

    if (dialogResult == QDialog::Rejected) {
        if (miniserver) {
            return *miniserver;
        }
    }

    return CMiniserver(); // Return an empty CMiniserver object if the dialog was canceled
}

CMiniserver Qt_CreateEditMiniserver::getMiniserver()
{
    return CMiniserver(serialNumberLineEdit->text().toStdString(), 
        adminUserLineEdit->text().toStdString(),
        adminPasswordLineEdit->text().toStdString()
        , "", "TBD", "TBD", "darkblue", "/", "TBD", localIPLineEdit->text().toStdString(), "5");

;
}

