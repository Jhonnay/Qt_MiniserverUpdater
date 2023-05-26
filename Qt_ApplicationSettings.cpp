#include "Qt_ApplicationSettings.h"
#include "MyConstants.h"

Qt_ApplicationSettings::Qt_ApplicationSettings(const QString& title, ApplicationSettings* settings, QWidget* parent)
	: QDialog(parent), m_settings(settings)
{
	setWindowTitle(title);
	layout = new QVBoxLayout(this);
	layout->addWidget(label1);

	lineEditDefaulMiniserverList = new QLineEdit();
	lineEditDefaulMiniserverList->setDisabled(true);
	layout->addWidget(lineEditDefaulMiniserverList);

	connect(browseButton1, &QPushButton::clicked, [this]() {
		QString roamingPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

		QString filePath = QFileDialog::getOpenFileName(this, tr("Open Miniserver List"), roamingPath, tr("JSON files (*.json)"));
		if (!filePath.isEmpty()) {
			//TODO: Check if valid Miniserverlist Json
			lineEditDefaulMiniserverList->setText(filePath);
		}
		});
	layout->addWidget(browseButton1);
	layout->addWidget(label2);
	lineEditDefaultConfigExe = new QLineEdit();
	lineEditDefaultConfigExe->setDisabled(true);
	layout->addWidget(lineEditDefaultConfigExe);

	connect(browseButton2, &QPushButton::clicked, [this]() {
		QString loxoneFolderPath = (qgetenv("PROGRAMFILES(X86)"));
		loxoneFolderPath.append("\\Loxone");
		QString filePath = QFileDialog::getOpenFileName(this, tr("Open Config Executable"), loxoneFolderPath, tr("Executable files (LoxoneConfig.exe)"));
		if (!filePath.isEmpty()) {
			//TODO: Check if valid Miniserver Exe
			lineEditDefaultConfigExe->setText(filePath);
		}
		});
	layout->addWidget(browseButton2);

	layout->addWidget(checkBoxDefaultMiniserverList);
	layout->addWidget(checkBoxDefaultConfigExe);

	buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
	layout->addWidget(buttonBox);

	
}


Qt_ApplicationSettings::~Qt_ApplicationSettings()
{
	delete lineEditDefaulMiniserverList;
	delete lineEditDefaultConfigExe;
	delete checkBoxDefaultMiniserverList;
	delete checkBoxDefaultConfigExe;
	delete m_settings;
	delete label1;
	delete browseButton1;
	delete label2;
	delete browseButton2;
	delete layout;
	delete buttonBox;
}

ApplicationSettings Qt_ApplicationSettings::createDialog(const QString& title, ApplicationSettings* settings, QWidget* parent)
{
	Qt_ApplicationSettings dialog(title);
	if (settings == nullptr) { settings = new ApplicationSettings(); };
	dialog.lineEditDefaulMiniserverList->setText(QString::fromStdString(settings->getStrDefaultConfigurationPath()));
	dialog.lineEditDefaultConfigExe->setText(QString::fromStdString(settings->getStrDefaultConfigPath()));
	dialog.checkBoxDefaultMiniserverList->setChecked(settings->getBUseDefaultConfiguration());
	dialog.checkBoxDefaultConfigExe->setChecked(settings->getBUseDefaultConfig());
	//dialog.resize(400,250);

	int dialogResult = dialog.exec();

	if (dialogResult == QDialog::Accepted) {
		if (dialog.checkBoxDefaultMiniserverList->isChecked() && dialog.lineEditDefaulMiniserverList->text() == "not set" ) {
			QMessageBox::warning(nullptr, "Warning", QString::fromStdString(MyConstants::Strings::MessageBox_ApplicationSettingsPaths_not_correct));
			QMessageBox::information(nullptr, "Information", QString::fromStdString(MyConstants::Strings::MessageBox_Applicationsettings_Not_saved));
			return ApplicationSettings(); // Return an empty CMiniserver object if username or password not provided
		}

		if (dialog.checkBoxDefaultConfigExe->isChecked() && dialog.lineEditDefaultConfigExe->text() == "not set" ) {
			QMessageBox::warning(nullptr, "Warning", QString::fromStdString(MyConstants::Strings::MessageBox_ApplicationSettingsPaths_not_correct));
			QMessageBox::information(nullptr, "Information", QString::fromStdString(MyConstants::Strings::MessageBox_Applicationsettings_Not_saved));
			return ApplicationSettings(); // Return an empty CMiniserver object if username or password not provided
		}

		QString roamingPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
		QDir dir(roamingPath);
		if (!dir.exists()) {
			QMessageBox::warning(nullptr, "Application Settings", "Could not open path: " + roamingPath);
			dir.mkpath(roamingPath);
			if (!dir.exists()) {
				QMessageBox::warning(nullptr, "Application Settings", "Could not create Path: " + roamingPath);
				
				return ApplicationSettings();
			}
			else {
				QMessageBox::information(nullptr, "Application Settings", "Successfully created path: " + roamingPath);
			}

		}
		//TODO::all off this can be prevented, if I make the Aplication Settings Textboxes not editable. 
		bool miniserverlistValid = QFileInfo(dialog.lineEditDefaulMiniserverList->text()).exists();
		bool configexeValid = QFileInfo(dialog.lineEditDefaultConfigExe->text()).exists();
		if (dialog.lineEditDefaulMiniserverList->text() == "not set" ) { miniserverlistValid = true; }
		if (dialog.lineEditDefaultConfigExe->text() == "not set") { configexeValid = true; };

		if (!miniserverlistValid || !configexeValid) {
			QMessageBox::warning(nullptr, "Warning", QString::fromStdString(MyConstants::Strings::MessageBox_ApplicationSettingsPaths_not_correct));
			return ApplicationSettings();
		}

		return dialog.getApplicationSettings();
	}

	if (dialogResult == QDialog::Rejected) {
		QMessageBox::information(nullptr, "Information", QString::fromStdString(MyConstants::Strings::MessageBox_Applicationsettings_Not_saved));
		return ApplicationSettings(); // Return an empty CMiniserver object if username or password not provided

	}
	
	return ApplicationSettings();
}

ApplicationSettings Qt_ApplicationSettings::getApplicationSettings() const
{
	ApplicationSettings settings;
	settings.setStrDefaultConfigurationPath(lineEditDefaulMiniserverList->text().toStdString());
	settings.setStrDefaultConfigPath(lineEditDefaultConfigExe->text().toStdString());
	settings.setBUseDefaultConfiguration(checkBoxDefaultMiniserverList->isChecked());
	settings.setBUseDefaultConfig(checkBoxDefaultConfigExe->isChecked());
	return settings;
}
