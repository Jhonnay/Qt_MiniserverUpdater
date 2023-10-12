#include "Qt_CleanOldConfigs.h"
#include <regex>


Qt_CleanOldConfigs::Qt_CleanOldConfigs(QString title, QString pathPrograms, QString pathProgramData, QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(title);
	vbox = new QVBoxLayout(this);
	this->pathPrograms = new QLabel(pathPrograms);
	this->pathProgramData = new QLabel(pathProgramData);
	hboxLabels = new QHBoxLayout();
	hboxLabels->addWidget(this->pathPrograms);
	hboxLabels->addWidget(this->pathProgramData);
	hboxLabels->setSpacing(50); 

	vbox->addLayout(hboxLabels);
	hboxScroll = new QHBoxLayout();
	hboxScroll->setSpacing(50);
	listWidgetPrograms = new QListWidget();
	listWidgetProgramData = new QListWidget();
	hboxScroll->addWidget(listWidgetPrograms);
	hboxScroll->addWidget(listWidgetProgramData);

	vbox->addLayout(hboxScroll);
    hboxSizes = new QHBoxLayout();
    hboxSizes->setSpacing(50);
    sizePrograms = new QLabel("Size: 0 kB");
    sizeProgramData = new QLabel("Size: 0 kB");
    hboxSizes->addWidget(sizePrograms);
    hboxSizes->addWidget(sizeProgramData);
    vbox->addLayout(hboxSizes);

	buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	vbox->addWidget(buttonBox);


    compareDirectories(pathPrograms, pathProgramData);
    uintmax_t initcleanProgramData = calculateCleanSize(pathProgramData);
    qDebug() << "Total Size: " << formatSize(initcleanProgramData);
    sizeProgramData->setText("Size: " + formatSize(initcleanProgramData));
}

Qt_CleanOldConfigs::~Qt_CleanOldConfigs()
{}


void Qt_CleanOldConfigs::compareDirectories(const QString& dirPath1, const QString& dirPath2) {
    QDir directory1(dirPath1);
    QDir directory2(dirPath2);

    // Get the list of folders in both directories
    QStringList folders1 = directory1.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList folders2 = directory2.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    QRegularExpression regex("(alpha|beta|release|loxone)", QRegularExpression::CaseInsensitiveOption);

    // Filter folders using the regular expression
    folders1 = folders1.filter(regex);
    folders2 = folders2.filter(regex);

    // Display the folder names
    qDebug() << "Folders in Directory 1 (" << dirPath1 << "):";
    for (const QString& folder : folders1) {
        qDebug() << folder;
    }
    
    qDebug() << "Folders in Directory 2 (" << dirPath2 << "):";
    for (const QString& folder : folders2) {
        qDebug() << folder;
    }
    
    listWidgetPrograms->addItems(folders1);
    listWidgetProgramData->addItems(folders2);

    QRegularExpression versionRegex("\\d+\\.\\d+\\.\\d+\\.\\d+");

    qDebug() << "Versions of " << dirPath1;
    //format folders1 with just the version number String
   
    for (int i = 0; i < folders1.count(); ++i) {
        QRegularExpressionMatchIterator matchIterator = versionRegex.globalMatch(folders1.at(i));
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            qDebug() << "Version: " << match.captured();
            folders1.replace(i, match.captured());
        }
    }

    qDebug() << "Versions of " << dirPath2;
    //check if folder from dir2 is contained in dir1
    for (int i = 0; i < listWidgetProgramData->count(); ++i) {
        //format folders2 with just the version number String
        QRegularExpressionMatchIterator matchIterator = versionRegex.globalMatch(folders2.at(i));
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            qDebug() << "Version: " << match.captured();
            folders2.replace(i, match.captured());
        }
        QListWidgetItem* item = listWidgetProgramData->item(i);
        if (item) {
            if (!folders1.contains(folders2.at(i))) {
                item->setBackground(Qt::red);
            }
        }
    }

}


uintmax_t Qt_CleanOldConfigs::getDirectorySize(const std::filesystem::path& dir) {
    uintmax_t size = 0;

    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        if (std::filesystem::is_regular_file(entry)) {
            size += std::filesystem::file_size(entry.path());
        }
        else if (std::filesystem::is_directory(entry)) {
            size += getDirectorySize(entry.path());
        }
    }
    
    return size;
}


uintmax_t Qt_CleanOldConfigs::calculateCleanSize(QString dir) {
    uintmax_t size = 0;
    QListWidget* list = dir.compare(pathPrograms->text()) ? listWidgetPrograms : listWidgetProgramData;
    
    for (int i = 0; i < listWidgetProgramData->count(); ++i) {
        QListWidgetItem* item = listWidgetProgramData->item(i);
        if (item) {
            if (item->background() == Qt::red) {
                std::filesystem::path path = dir.toStdString() + "\\" + item->text().toStdString();
                uintmax_t newSize = getDirectorySize(path);
                qDebug() << "Size: " << formatSize(newSize) << " Path: " << path.string();
                size += newSize;
            }
        }
    }

    return size;
}

QString Qt_CleanOldConfigs::formatSize(uintmax_t sizeInBytes) {
    if (sizeInBytes < 1000) {
        return QString::number(sizeInBytes) + " B";
    }
    else if (sizeInBytes < 1000 * 1000) {
        return QString::number(sizeInBytes / 1024.0, 'f', 2) + " KB";
    }
    else if (sizeInBytes < 1000 * 1000 * 1000) {
        return QString::number(sizeInBytes / (1024.0 * 1024.0), 'f', 2) + " MB";
    }
    else {
        return QString::number(sizeInBytes / (1024.0 * 1024.0 * 1024.0), 'f', 2) + " GB";
    }
}