#include "Qt_CleanOldConfigs.h"
#include <regex>
#include <windows.h>


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
    listWidgetPrograms->setSelectionMode(QAbstractItemView::MultiSelection);
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
    clean = new QPushButton(tr("Clean Computer"));
    vbox->addWidget(clean);
    connect(clean, &QPushButton::pressed, this, &Qt_CleanOldConfigs::performCleaning);

	buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	vbox->addWidget(buttonBox);

    contextMenuListPrograms = new QMenu(); 
    actionContextMenuListPrograms = new QAction(tr("Add selected folders to Clean-up"));
    connect(actionContextMenuListPrograms, &QAction::triggered, this, &Qt_CleanOldConfigs::addSelectedFoldersToCleanUP);

    // Add the action to the context menu
    contextMenuListPrograms->addAction(actionContextMenuListPrograms);

    // Set the context menu for the list widget
    listWidgetPrograms->setContextMenuPolicy(Qt::CustomContextMenu);

    // Connect the customContextMenuRequested signal to show the context menu
    connect(listWidgetPrograms, &QListWidget::customContextMenuRequested, this, [this](const QPoint& pos) {
        contextMenuListPrograms->exec(QCursor::pos());
        });


    InitializeDialogWithSizes(pathPrograms, pathProgramData);


    this->setStyleSheet(R"""(
        QListWidget::item: {
            background-color: lightblue; /* Background color when item is selected */
        }
      
        

	)""");
}

void Qt_CleanOldConfigs::InitializeDialogWithSizes(QString& pathPrograms, QString& pathProgramData)
{
    compareDirectories(pathPrograms, pathProgramData);
    sizeCleanProgramData = calculateDirectorySizeOfQListWidget(pathProgramData);
    qDebug() << "ProgramData Total Size: " << formatSize(sizeCleanProgramData);
    sizeProgramData->setText("Size: " + formatSize(sizeCleanProgramData));

    sizeCleanPrograms = calculateDirectorySizeOfQListWidget(pathPrograms);
    qDebug() << "Programs Total Size: " << formatSize(sizeCleanPrograms);
    sizePrograms->setText("Size: " + formatSize(sizeCleanPrograms));
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

    //displayAllFolders(dirPath1, folders1, dirPath2, folders2);
    
    listWidgetPrograms->addItems(folders1);
    listWidgetProgramData->addItems(folders2);
    reduceDirectoryFilenamesToVersionString(dirPath1, folders1);
    reduceDirectoryFilenamesToVersionString(dirPath2, folders2);

    
   
    for (int i = 0; i < listWidgetProgramData->count(); ++i) {
        
        QListWidgetItem* item = listWidgetProgramData->item(i);
        if (item) {
            if (!folders1.contains(folders2.at(i)) || item->text().at(0) == '_') { //mark red, if Folder starts with underscore '_'
                item->setBackground(Qt::red);
                item->setForeground(Qt::white);
            }
        }
    }

    for (int i = 0; i < listWidgetPrograms->count(); ++i) {
        QListWidgetItem* item = listWidgetPrograms->item(i);
        if (item) {
            if (!folders2.contains(folders1.at(i))) {
                item->setBackground(Qt::red);
                item->setForeground(Qt::white);
            }
        }
    }

}

void Qt_CleanOldConfigs::displayAllFolders(const QString& dirPath1, QStringList& folders1, const QString& dirPath2, QStringList& folders2)
{
    // Display the folder names
    qDebug() << "Folders in Directory 1 (" << dirPath1 << "):";
    for (const QString& folder : folders1) {
        qDebug() << folder;
    }

    qDebug() << "Folders in Directory 2 (" << dirPath2 << "):";
    for (const QString& folder : folders2) {
        qDebug() << folder;
    }
}

void Qt_CleanOldConfigs::reduceDirectoryFilenamesToVersionString(const QString& dirPath, QStringList& folders)
{
    QRegularExpression versionRegex("\\d+\\.\\d+\\.\\d+\\.\\d+");
    QListWidget* list = (dirPath == pathPrograms->text()) ? listWidgetPrograms : listWidgetProgramData;

    if (folders.isEmpty()) {
        return;
    }
    qDebug() << "Versions of " << dirPath;
    
    for (int i = 0; i < list->count(); ++i) {
        if (folders.at(i).at(0) == '_') { 
            continue; //do not include folder starting with underscore in the '_' reduced versions list. 
        }
        QRegularExpressionMatchIterator matchIterator = versionRegex.globalMatch(folders.at(i));
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            //qDebug() << "Version: " << match.captured();
            folders.replace(i, match.captured());
        }

    }
}

QString Qt_CleanOldConfigs::reduceFileNameToVersionString(QString filename)
{
    QRegularExpression versionRegex("\\d+\\.\\d+\\.\\d+\\.\\d+");
    
    QRegularExpressionMatchIterator matchIterator = versionRegex.globalMatch(filename);
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        return  match.captured();
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


uintmax_t Qt_CleanOldConfigs::calculateDirectorySizeOfQListWidget(QString dir) {
    uintmax_t size = 0;
    qDebug() << pathPrograms->text();
    QListWidget* list = (dir == pathPrograms->text()) ? listWidgetPrograms : listWidgetProgramData;
    
    for (int i = 0; i < list->count(); ++i) {
        QListWidgetItem* item = list->item(i);
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

void Qt_CleanOldConfigs::addSelectedFoldersToCleanUP() {
    QList<QListWidgetItem*> selectedItems = listWidgetPrograms->selectedItems();

    foreach(QListWidgetItem * item, selectedItems) {
        //QBrush backgroundBrush = item->background();
        //QColor backgroundColor = backgroundBrush.color();
        //
        //// Get the background color as a text (color name)
        //QString backgroundColorText = backgroundColor.name();
        if (item->background() != Qt::red && item->background() != Qt::green) {
            qDebug() << "Performing action on item: " << item->text();
            item->setBackground(Qt::green);
            //item->setForeground(Qt::white);
            std::filesystem::path path = pathPrograms->text().toStdString() + "\\" + item->text().toStdString();
            uintmax_t newSize = getDirectorySize(path);
            qDebug() << "Size: " << formatSize(newSize) << " - " << pathPrograms->text().toStdString() + "\\" + item->text().toStdString();
            qDebug() << "Size to be Cleaned Programs Before: " << formatSize(sizeCleanPrograms);
            sizeCleanPrograms += newSize;
            qDebug() << "Size to be Cleaned Programs After: " << formatSize(sizeCleanPrograms);

            sizePrograms->setText("Size: " + formatSize(sizeCleanPrograms));
            
            QStringList folders2;
            for (int i = 0; i < listWidgetProgramData->count(); ++i) {
                QListWidgetItem* item = listWidgetProgramData->item(i);
                if (item) {
                    folders2.append(item->text());
                }
            }
            reduceDirectoryFilenamesToVersionString(pathProgramData->text(), folders2);
            QString val = reduceFileNameToVersionString(item->text());
            qDebug() << val; 
            if (folders2.contains(val)) {
                int index = folders2.indexOf(val);
                if (index >= 0 && index < listWidgetProgramData->count()) {
                    QListWidgetItem* itemToChange = listWidgetProgramData->item(index);

                    if (itemToChange) {
                        itemToChange->setBackground(QBrush(Qt::green));
                        QString val = pathProgramData->text() + "/Loxone Config " + reduceFileNameToVersionString(item->text());
                        path = pathProgramData->text().toStdString() + "/Loxone Config " + reduceFileNameToVersionString(item->text()).toStdString();
                        newSize = getDirectorySize(path);
                        qDebug() << "Size: " << formatSize(newSize) << " - " << pathProgramData->text().toStdString() + "/Loxone Config " + reduceFileNameToVersionString(item->text()).toStdString();
                        qDebug() << "Size to be Cleaned ProgramData Before: " << formatSize(sizeCleanProgramData);
                        sizeCleanProgramData += newSize;
                        qDebug() << "Size to be Cleaned ProgramData After: " << formatSize(sizeCleanProgramData);

                        sizeProgramData->setText("Size: " + formatSize(sizeCleanProgramData));
                    }
                }
            }
            


        }
    }

    return;
}

bool Qt_CleanOldConfigs::isRunningAsAdmin() {
    HANDLE hToken = NULL;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION elevation;
        DWORD size;
        if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &size)) {
            CloseHandle(hToken);
            return elevation.TokenIsElevated;
        }
        CloseHandle(hToken);
    }
    return false;
}

void Qt_CleanOldConfigs::performCleaning() {

    if (!isRunningAsAdmin()) {
        QMessageBox::critical(nullptr, tr("Rights?"), tr("You do not have admin rights!\nRestart this application with admin rights. "));
        return;
    }
    //listWidgetPrograms
    bool firstCleaningSuccess = true;
    bool secondCleaningSuccess = true;
    firstCleaningSuccess = cleanFolder(pathPrograms->text());
    secondCleaningSuccess = cleanFolder(pathProgramData->text());
    uintmax_t totalSize = sizeCleanProgramData + sizeCleanPrograms;


    if (firstCleaningSuccess && secondCleaningSuccess) {
        QMessageBox::information(nullptr, tr("Config Clean"), tr("Successfully cleaned: ") + formatSize(totalSize));
        return;
    }

}

bool Qt_CleanOldConfigs::cleanFolder(QString dir)
{
    if (!QDir(dir).exists()) {
        return false;
    }
    QListWidget* list = (dir == pathPrograms->text()) ? listWidgetPrograms : listWidgetProgramData;
    bool cleanedEverything = true;
    for (int i = 0; i < list->count(); i++) {
        QListWidgetItem* item = list->item(i);

        if (item) {
            if (item->background() == Qt::red || item->background() == Qt::green) {
                QString fullPath = (dir == pathPrograms->text()) ? pathPrograms->text() + "\\" + item->text() : pathProgramData->text() + "/" + item->text();
                QDir directory = QDir(fullPath);
                if (directory.exists()) {
                    if (directory.removeRecursively()) {
                        qDebug() << "Directory deleted: " << fullPath;
                    }
                    else {
                        QMessageBox::warning(nullptr, "Warning", tr("Could not delete: ") + fullPath);
                        cleanedEverything = false;
                    }
                } 
                else {
                    QMessageBox::warning(nullptr, "Warning", tr("Directory does not exist: ") + fullPath);
                    cleanedEverything = false;
                }
            }
        }
        else{
            cleanedEverything = false;
        }
    }
}
