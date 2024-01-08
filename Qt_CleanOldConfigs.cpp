#include "Qt_CleanOldConfigs.h"
#include <regex>
#include <windows.h>
#include <QDesktopServices>
#include "CMiniserver.h"

#define VERSION_WITH_FOUR_SEGMENTS "10.2.3.26"


Qt_CleanOldConfigs::Qt_CleanOldConfigs(QString title, QString pathPrograms, QString pathProgramData, QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(title);
	vbox = new QVBoxLayout(this);
	btn_PathPrograms = new QPushButton(pathPrograms);
	btn_PathProgramData = new QPushButton(pathProgramData);
    this->btn_PathPrograms->setStyleSheet("text-decoration: underline; color: blue; text-align: left;");  // Make it look like a hyperlink
    this->btn_PathPrograms->setCursor(Qt::PointingHandCursor);  // Change cursor to a hand when hovering
    this->btn_PathProgramData->setStyleSheet("text-decoration: underline; color: blue; text-align: left;");  // Make it look like a hyperlink
    this->btn_PathProgramData->setCursor(Qt::PointingHandCursor);  // Change cursor to a hand when hovering
    // Set a style to make the QPushButton look like a label
    btn_PathPrograms->setFlat(true);
    btn_PathProgramData->setFlat(true);
	hboxLabels = new QHBoxLayout();
	hboxLabels->addWidget(this->btn_PathPrograms);
	hboxLabels->addWidget(this->btn_PathProgramData);
	hboxLabels->setSpacing(50); 
    

    connect(this->btn_PathPrograms, &QPushButton::clicked, [&]() {
        QString path = "file:///" + pathPrograms.replace("\\", "/");
        QDesktopServices::openUrl(QUrl::fromLocalFile(pathPrograms));
    });

    connect(this->btn_PathProgramData, &QPushButton::clicked, [&]() {
        QString path = "file:///" + pathProgramData.replace("\\", "/");
        QDesktopServices::openUrl(QUrl::fromLocalFile(pathProgramData));
    });


	vbox->addLayout(hboxLabels);
	hboxScroll = new QHBoxLayout();
	hboxScroll->setSpacing(50);
	listWidgetPrograms = new QListWidget();
    listWidgetProgramData = new QListWidget();
    listWidgetPrograms->setSelectionMode(QAbstractItemView::MultiSelection);
    listWidgetProgramData->setSelectionMode(QAbstractItemView::MultiSelection);
    btn_RefreshSizes = new QPushButton("🔁");
    hboxScroll->addWidget(listWidgetPrograms);
    //hboxScroll->addWidget(btn_RefreshSizes);
    hboxScroll->addWidget(listWidgetProgramData);
    hboxScroll->setAlignment(Qt::AlignVCenter);
    connect(btn_RefreshSizes, &QPushButton::pressed, this, &Qt_CleanOldConfigs::reCalculateSizes);
    vbox->addLayout(hboxScroll);

  
    hboxSizes = new QHBoxLayout();
    hboxSizes->setSpacing(50);
    sizePrograms = new QLabel("Size: 0 kB");
    sizeProgramData = new QLabel("Size: 0 kB");
    hboxSizes->addWidget(sizePrograms);
    hboxSizes->addWidget(sizeProgramData);
    vbox->addLayout(hboxSizes);

    // Define the items you want in your legend
    QCheckBox* legend1 = new QCheckBox("Folder automatically detected for cleaning");
    legend1->setCheckable(false);  // Make the checkbox not checkable
    legend1->setStyleSheet("QCheckBox::indicator { background-color: red; }");

    QCheckBox* legend2 = new QCheckBox("Folder manually added for cleaning");
    legend2->setCheckable(false);
    legend2->setStyleSheet("QCheckBox::indicator { background-color: #00FF00; }");

    QCheckBox* legend3 = new QCheckBox("Folder manually excluded from cleaning");
    legend3->setCheckable(false);
    legend3->setStyleSheet("QCheckBox::indicator { background-color: yellow; }");

    vbox->addWidget(legend1);
    vbox->addWidget(legend2);
    vbox->addWidget(legend3);


    btn_Clean = new QPushButton(tr("Clean Computer"));
    vbox->addWidget(btn_Clean);
    connect(btn_Clean, &QPushButton::pressed, this, &Qt_CleanOldConfigs::performCleaning);
    

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    vbox->addWidget(buttonBox);

    contextMenuListPrograms = new QMenu();
    actionListProgramsAddSelected = new QAction(tr("Add selected folders to Clean-up"));
    actionListProgramsCleaningProhibited = new QAction(tr("Do not clean this folder"));
    connect(actionListProgramsAddSelected, &QAction::triggered, this, &Qt_CleanOldConfigs::addSelectedProgramsFoldersToCleanUP);
    connect(actionListProgramsCleaningProhibited, &QAction::triggered, this, &Qt_CleanOldConfigs::prohibitCleaningPrograms);
    // Add the action to the context menu
    contextMenuListPrograms->addAction(actionListProgramsAddSelected);
    contextMenuListPrograms->addAction(actionListProgramsCleaningProhibited);

    // Set the context menu for the list widget
    listWidgetPrograms->setContextMenuPolicy(Qt::CustomContextMenu);

    // Connect the customContextMenuRequested signal to show the context menu
    connect(listWidgetPrograms, &QListWidget::customContextMenuRequested, this, [this](const QPoint& pos) {
        contextMenuListPrograms->exec(QCursor::pos());
        });

    contextMenuListProgramData = new QMenu();
    actionListProgramDataCleaningProhibited = new QAction(tr("Do not clean this folder"));
    actionListProgramDataAddSelected = new QAction(tr("Add selected folders to Clean-up"));
    connect(actionListProgramDataCleaningProhibited, &QAction::triggered, this, &Qt_CleanOldConfigs::prohibitCleaningProgramData);
    connect(actionListProgramDataAddSelected, &QAction::triggered, this, &Qt_CleanOldConfigs::addSelectedProgramDataFoldersToCleanUP);
    
    // Add the action to the context menu
    contextMenuListProgramData->addAction(actionListProgramDataAddSelected);
    contextMenuListProgramData->addAction(actionListProgramDataCleaningProhibited);

    // Set the context menu for the list widget
    listWidgetProgramData->setContextMenuPolicy(Qt::CustomContextMenu);

    // Connect the customContextMenuRequested signal to show the context menu
    connect(listWidgetProgramData, &QListWidget::customContextMenuRequested, this, [this](const QPoint& pos) {
        contextMenuListProgramData->exec(QCursor::pos());
        });

    InitializeDialogWithSizes(pathPrograms, pathProgramData);


    this->setStyleSheet(R"""(
        QListWidget {
            border: 0px;
            padding-left: 0px;
        }

        QListWidget::item:selected {
            
            padding-left: 10px;

        }


        

	)""");


    //this->setStyleSheet(R"""(
    //    QListWidget {
    //        margin: 
    //    }
    //    QListWidget::item {
    //        padding: 3px;
    //        border: 0px solid rgba(32, 148, 250, 1);
    //    }
    //
    //    QListWidget::item:selected {
    //        color: black; /* Maintain the text color */
    //        background-color: white; /* Maintain the background color */
    //        padding: 0px;
    //        border: 3px solid rgba(32, 148, 250, 1);
    //    }
    //    
    //
	//)""");
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


    //Mark Red
    for (int i = 0; i < listWidgetProgramData->count(); ++i) {
        QListWidgetItem* item = listWidgetProgramData->item(i);
        if (item) {
            //QString str = dirPath2 + "\\" + item->text();
            if (isVersionLessThan(folders2.at(i), VERSION_WITH_FOUR_SEGMENTS)) {
                continue;
            }
            if (!folders1.contains(folders2.at(i)) || item->text().at(0) == '_') { //mark red, if Folder starts with underscore '_' 
                item->setBackground(Qt::red);
                item->setForeground(Qt::white);
            }
            if (folders1.contains(folders2.at(i))) { //folder2 < 5MB: check if the small folder from "folders2" is contained in "folders1" and mark folder1 red
                if (getDirectorySize(std::filesystem::path(dirPath2.toStdString() + "\\" + item->text().toStdString())) < 5 * 1000 * 1000){
                    item->setBackground(Qt::red); //set programdata red
                    item->setForeground(Qt::white); //set programdata red          
                    int index = folders1.indexOf(folders2.at(i));
                    QListWidgetItem* itemList1 = listWidgetPrograms->item(index);
                    if (itemList1) {
                        itemList1->setBackground(Qt::red); //set programs red
                        itemList1->setForeground(Qt::white); //set programs red
                        
                    }
                }
            }
        }
    }
    for (int i = 0; i < listWidgetPrograms->count(); ++i) {
        QListWidgetItem* item = listWidgetPrograms->item(i);
        if (item) {
            //QString str = dirPath1 + "\\" + item->text();
            if (isVersionLessThan(folders1.at(i), VERSION_WITH_FOUR_SEGMENTS)) {
                continue;
            }
            if (!folders2.contains(folders1.at(i))   
                || getDirectorySize(std::filesystem::path(dirPath1.toStdString() + "\\" + item->text().toStdString())) < 5 * 1000 * 1000 ) {
                item->setBackground(Qt::red);
                item->setForeground(Qt::white);
            }
        }
    }

}

bool Qt_CleanOldConfigs::isVersionLessThan (const QString& version, const QString& referenceVersion) {
    QVersionNumber versionNumber = QVersionNumber::fromString(version);
    QVersionNumber referenceVersionNumber = QVersionNumber::fromString(referenceVersion);

    return versionNumber < referenceVersionNumber;
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
    QListWidget* list = (dirPath == btn_PathPrograms->text()) ? listWidgetPrograms : listWidgetProgramData;

    if (folders.isEmpty()) {
        return;
    }
    qDebug() << "Versions of " << dirPath;
    
    for (int i = 0; i < list->count(); ++i) {
        if (folders.at(i).at(0) == '_') { 
            continue; //do not include folder starting with underscore in the '_' reduced versions list. 
        }
        if (dirPath == btn_PathPrograms->text()) { //in Programs86 folder retreive the version directly from Config Exe
            QString path = dirPath + "\\" + folders.at(i) + "\\LoxoneConfig.exe";
            if (!QFile(path).exists()) {
                qDebug() << "Config Executable does not exist - paht: " + path;
                continue;
            }
            QString version = CConfig::getConfigFileVersionFormated(path);
            folders.replace(i, version);
            continue;
        }
        //otherwise use the Regex to retreive the version from foldername
        else if(folders.at(i).length() < 20){ //e.g. "Locone Config 7.1"
            QRegularExpression versionRegex("\\d+\\.\\d+");
            QRegularExpressionMatchIterator matchIterator = versionRegex.globalMatch(folders.at(i));
            while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                folders.replace(i, match.captured());
            }
        }
        else {
            QRegularExpression versionRegex("\\d+\\.\\d+\\.\\d+\\.\\d+");
            QRegularExpressionMatchIterator matchIterator = versionRegex.globalMatch(folders.at(i));
            while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                folders.replace(i, match.captured());
            }
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
    qDebug() << btn_PathPrograms->text();
    QListWidget* list = (dir == btn_PathPrograms->text()) ? listWidgetPrograms : listWidgetProgramData;
    
    for (int i = 0; i < list->count(); ++i) {
        QListWidgetItem* item = list->item(i);
        if (item) {
            if (item->background() == Qt::red || item->background() == Qt::green) {
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

void Qt_CleanOldConfigs::addSelectedProgramsFoldersToCleanUP() {
    QList<QListWidgetItem*> selectedItems = listWidgetPrograms->selectedItems();

    foreach(QListWidgetItem * item, selectedItems) {
        if (item->background() != Qt::red && item->background() != Qt::green) {
            qDebug() << "Performing action on item: " << item->text();
            item->setBackground(Qt::green);
            item->setForeground(Qt::black);
            std::filesystem::path path = btn_PathPrograms->text().toStdString() + "\\" + item->text().toStdString();
            uintmax_t newSize = getDirectorySize(path);
            qDebug() << "Size: " << formatSize(newSize) << " - " << btn_PathPrograms->text().toStdString() + "\\" + item->text().toStdString();
            qDebug() << "Size to be Cleaned Programs Before: " << formatSize(sizeCleanPrograms);
            sizeCleanPrograms += newSize;
            qDebug() << "Size to be Cleaned Programs After: " << formatSize(sizeCleanPrograms);

            sizePrograms->setText("Size: " + formatSize(sizeCleanPrograms));
            
            //automatically add folders in "ProgramData" if available
            QStringList folders2;
            for (int i = 0; i < listWidgetProgramData->count(); ++i) {
                QListWidgetItem* item = listWidgetProgramData->item(i);
                if (item) {
                    folders2.append(item->text());
                }
            }
            reduceDirectoryFilenamesToVersionString(btn_PathProgramData->text(), folders2);
            QString val = reduceFileNameToVersionString(item->text());
            qDebug() << val; 
            if (folders2.contains(val)) {
                int index = folders2.indexOf(val);
                if (index >= 0 && index < listWidgetProgramData->count()) {
                    QListWidgetItem* itemToChange = listWidgetProgramData->item(index);

                    if (itemToChange) {
                        itemToChange->setBackground(QBrush(Qt::green));
                        itemToChange->setForeground(QBrush(Qt::black));
                        QString val = btn_PathProgramData->text() + "/Loxone Config " + reduceFileNameToVersionString(item->text());
                        path = btn_PathProgramData->text().toStdString() + "/Loxone Config " + reduceFileNameToVersionString(item->text()).toStdString();
                        if (QDir(path).exists()) {
                            newSize = getDirectorySize(path);
                            qDebug() << "Size: " << formatSize(newSize) << " - " << btn_PathProgramData->text().toStdString() + "/Loxone Config " + reduceFileNameToVersionString(item->text()).toStdString();
                            qDebug() << "Size to be Cleaned ProgramData Before: " << formatSize(sizeCleanProgramData);
                            sizeCleanProgramData += newSize;
                            qDebug() << "Size to be Cleaned ProgramData After: " << formatSize(sizeCleanProgramData);
                            sizeProgramData->setText("Size: " + formatSize(sizeCleanProgramData));
                        }
                    }
                }
            }
        }
    }

    return;
}

/*
* folders in "Programs(x86) are not automatically added like in Qt_CleanOldConfigs::addSelectedProgramsFoldersToCleanUP
*/
void Qt_CleanOldConfigs::addSelectedProgramDataFoldersToCleanUP() {
    QList<QListWidgetItem*> selectedItems = listWidgetProgramData->selectedItems();
    foreach(QListWidgetItem * item, selectedItems) {
        if (item->background() != Qt::red && item->background() != Qt::green) {
            qDebug() << "Performing action on item: " << item->text();
            item->setBackground(Qt::green);
            item->setForeground(Qt::black);
            std::filesystem::path path = btn_PathProgramData->text().toStdString() + "\\" + item->text().toStdString();
            uintmax_t newSize = getDirectorySize(path);
            qDebug() << "Size: " << formatSize(newSize) << " - " << btn_PathProgramData->text().toStdString() + "\\" + item->text().toStdString();
            qDebug() << "Size to be Cleaned Programs Before: " << formatSize(sizeCleanProgramData);
            sizeCleanProgramData += newSize;
            qDebug() << "Size to be Cleaned Programs After: " << formatSize(sizeCleanProgramData);

            sizeProgramData->setText("Size: " + formatSize(sizeCleanProgramData));
        }
    }
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
    // Ask the user for confirmation
    QMessageBox confirmationBox;
    confirmationBox.setIcon(QMessageBox::Question);
    confirmationBox.setWindowTitle(tr("Confirmation"));
    confirmationBox.setText(tr("Are you sure you want permanently delete the folders?"));
    confirmationBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    // Check the user's response
    if (confirmationBox.exec() == QMessageBox::No) {
        return; // User pressed "No," so do not proceed with cleaning
    }
        
    if (!isRunningAsAdmin()) {
        QMessageBox::critical(nullptr, tr("Rights?"), tr("You do not have admin rights!\nRestart this application with admin rights. "));
        return;
    }

    //listWidgetPrograms
    bool firstCleaningSuccess = true;
    bool secondCleaningSuccess = true;
    firstCleaningSuccess = cleanFolder(btn_PathPrograms->text());
    secondCleaningSuccess = cleanFolder(btn_PathProgramData->text());
    uintmax_t totalSize = sizeCleanProgramData + sizeCleanPrograms;


    if (firstCleaningSuccess && secondCleaningSuccess) {
        QMessageBox::information(nullptr, tr("Config Clean"), tr("Successfully cleaned: ") + formatSize(totalSize));
    }

    reCalculateSizes();
}

bool Qt_CleanOldConfigs::cleanFolder(QString dir)
{
    if (!QDir(dir).exists()) {
        return false;
    }
    QListWidget* list = (dir == btn_PathPrograms->text()) ? listWidgetPrograms : listWidgetProgramData;
    bool cleanedEverything = true;
    
    int listCount = list->count();

    //reverse loop for deleting items
    for (int i = listCount-1; i >= 0; i--) { 
        QListWidgetItem* item = list->item(i);
    
        if (item) {
            if (item->background() == Qt::red || item->background() == Qt::green) {
                QString fullPath = (dir == btn_PathPrograms->text()) ? btn_PathPrograms->text() + "\\" + item->text() : btn_PathProgramData->text() + "/" + item->text();
                QDir directory = QDir(fullPath);
                if (directory.exists()) {
                    if (directory.removeRecursively()) {
                        qDebug() << "Directory deleted: " << fullPath;
                        QListWidgetItem* itemToDelete = list->takeItem(list->indexFromItem(item).row());
                        delete itemToDelete; // Free the memory
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
        else {
            cleanedEverything = false;
        }
    }

    return cleanedEverything;
}

void Qt_CleanOldConfigs::reCalculateSizes() {
    qDebug() << "Re-Calcualting sizes!"; 
    sizeCleanPrograms = calculateDirectorySizeOfQListWidget(btn_PathPrograms->text());
    sizeCleanProgramData = calculateDirectorySizeOfQListWidget(btn_PathProgramData->text());
    sizeProgramData->setText("Size: " + formatSize(sizeCleanProgramData));
    sizePrograms->setText("Size: " + formatSize(sizeCleanPrograms));
}

void Qt_CleanOldConfigs::prohibitCleaningPrograms() {
    QListWidgetItem* selectedItem = listWidgetPrograms->currentItem();
    if (selectedItem) {
        QString text = selectedItem->text();
        selectedItem->setBackground(Qt::yellow);
        selectedItem->setForeground(Qt::black);
        qDebug() << "Action performed on: " << text;
        reCalculateSizes();
    }
}

void Qt_CleanOldConfigs::prohibitCleaningProgramData() {
    QListWidgetItem* selectedItem = listWidgetProgramData->currentItem();
    if (selectedItem) {
        QString text = selectedItem->text();
        selectedItem->setBackground(Qt::yellow);
        selectedItem->setForeground(Qt::black);
        qDebug() << "Action performed on: " << text;
        reCalculateSizes();
    }
}