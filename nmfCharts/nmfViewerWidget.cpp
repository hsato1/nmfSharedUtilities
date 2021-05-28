/**
 * @file nmfViewerWidget.h
 * @brief GUI definition for the Image Viewer set of widgets
 *
 * This file contains the GUI definitions for the controls and widgets use in the Image
 * Viewer class.  This class allows the user to view images one by one or set a begin and end
 * image and flip through them in an animated fashion.
 *
 * @copyright
 * Public Domain Notice\n
 *
 * National Oceanic And Atmospheric Administration\n\n
 *
 * This software is a "United States Government Work" under the terms of the
 * United States Copyright Act.  It was written as part of the author's official
 * duties as a United States Government employee/contractor and thus cannot be copyrighted.
 * This software is freely available to the public for use. The National Oceanic
 * And Atmospheric Administration and the U.S. Government have not placed any
 * restriction on its use or reproduction.  Although all reasonable efforts have
 * been taken to ensure the accuracy and reliability of the software and data,
 * the National Oceanic And Atmospheric Administration and the U.S. Government
 * do not and cannot warrant the performance or results that may be obtained
 * by using this software or data. The National Oceanic And Atmospheric
 * Administration and the U.S. Government disclaim all warranties, express
 * or implied, including warranties of performance, merchantability or fitness
 * for any particular purpose.\n\n
 *
 * Please cite the author(s) in any work or product based on this material.
 *
 */

#include "nmfViewerWidget.h"


nmfViewerWidget::nmfViewerWidget(QWidget* parent,
                                 const QString& imagePath,
                                 nmfLogger* logger)
{
    m_Parent          = parent;
    m_ImagePath       = imagePath;
    m_Logger          = logger;
    m_SkipEndImages   = false;
    m_Timer           = new QTimer();
    m_FirstImageNum   = -1;
    m_LastImageNum    = -1;
    m_CurrentImageNum =  0;
    m_ImageDirection  =  1;
    m_TimeMicroSec    = 1000000;
    m_TimeMilliSec    = 1000;
    m_StopPlayback    = false;

    QVBoxLayout* vlayt1 = new QVBoxLayout();
    QVBoxLayout* vlayt2 = new QVBoxLayout();
    QVBoxLayout* vlayt3 = new QVBoxLayout();
    QVBoxLayout* vlayt4 = new QVBoxLayout();

    m_MainWidget        = new QWidget();
    m_MainFrame         = new QFrame();
    m_ViewerWidget      = new QWidget();
    m_ViewerTabW        = new QTabWidget();
    m_ViewerImageTab    = new QWidget();
    m_ViewerDataTab     = new QWidget();
    m_ViewerDataSubTabW = new QTabWidget();

    vlayt1->addWidget(m_MainFrame);
    vlayt2->addWidget(m_ViewerWidget);
    vlayt3->addWidget(m_ViewerTabW);
    vlayt4->addWidget(m_ViewerDataSubTabW);

    m_MainWidget->setLayout(vlayt1);
    m_MainFrame->setLayout(vlayt2);
    m_ViewerWidget->setLayout(vlayt3);
    m_ViewerDataTab->setLayout(vlayt4);

    m_ViewerTabW->addTab(m_ViewerImageTab,tr("Viewer"));
    m_ViewerTabW->addTab(m_ViewerDataTab, tr("Data"));

    m_MainFrame->setFrameShape(QFrame::Box);
    m_MainFrame->setFrameShadow(QFrame::Raised);
    m_MainFrame->setLineWidth(7);

    // setup signal and slot
    connect(m_Timer, SIGNAL(timeout()),
            this,    SLOT(callback_BouncePlayNextImage()));

    setupOutputScreenShotViewerWidgets();

}

nmfViewerWidget::~nmfViewerWidget() {

}

void
nmfViewerWidget::setImagePath(const QString& imagePath)
{
    m_ImagePath = imagePath;
}

void
nmfViewerWidget::addDataTab(
        const QString& title,
        QTableView* tableview)
{
    QVBoxLayout* vlayt  = new QVBoxLayout();
    QWidget*     newTab = new QWidget();

    m_ViewerDataSubTabW->addTab(newTab,title);
    vlayt->addWidget(tableview);
    newTab->setLayout(vlayt);
    tableview->setObjectName("TableView");
}

QString
nmfViewerWidget::getCurrentFilename()
{
    return m_ImageCMB->currentText();
}

QWidget*
nmfViewerWidget::getMainWidget()
{
    return m_MainWidget;
}

void
nmfViewerWidget::hideDataTab()
{
    m_ViewerTabW->removeTab(1);
}

void
nmfViewerWidget::hideFrame()
{
    m_MainFrame->setFrameShape(QFrame::NoFrame);
    m_MainFrame->setLineWidth(0);
    m_ViewerWidget->setContentsMargins(0,0,0,0);
    m_MainFrame->setContentsMargins(0,0,0,0);
    m_ViewerTabW->setContentsMargins(0,0,0,0);
    m_MainWidget->setContentsMargins(0,0,0,0);
}

void
nmfViewerWidget::initializeFirstAndLastImages()
{
  m_SetFirstImagePB->setChecked(false);
  m_SetLastImagePB->setChecked(false);
}

void
nmfViewerWidget::loadDataTables(QString fullFilename)
{
    int numRows;
    int numCols;
    int numTabs = m_ViewerDataSubTabW->count();
    QWidget* tabW;
    QTableView* tableView;
    QStandardItem* item;
    std::vector<boost::numeric::ublas::matrix<double> > data;
    QStandardItemModel* smodel;
    QStringList header;

    nmfUtilsQt::switchFileExtensions(fullFilename,".csv",{".jpg",".png"});

    // Read all the tabs' data from the data file
    bool readOK = readDataFile(fullFilename,header,data,numRows,numCols);
    if (data.size() == 0) {
        readOK = false;
    }

    for (int k=0; k<numTabs; ++k) {
        tabW      = m_ViewerDataSubTabW->widget(k);
        tableView = qobject_cast<QTableView*>(tabW->findChild<QTableView*>("TableView"));
        if (readOK) {
            tableView->show();
            smodel = new QStandardItemModel(numRows,numCols);
            for (int i=0; i<numRows; ++i) {
                for (int j=0; j<numCols; ++j) {
                    if (j == 0) {
                        item = (readOK) ? new QStandardItem(QString::number(data[k](i,j),'d',0)) :
                                          new QStandardItem("");
                    } else {
                        item = (readOK) ? new QStandardItem(QString::number(data[k](i,j),'f',3)) :
                                          new QStandardItem("");
                    }
                    item->setTextAlignment(Qt::AlignCenter);
                    smodel->setItem(i,j,item);
                }
            }
            smodel->setHorizontalHeaderLabels(header);
            tableView->setModel(smodel);
        } else {
            tableView->hide();
        }
    }

}

bool
nmfViewerWidget::okToOverwrite(QString filenameWithPath)
{
    QString msg;

    QFileInfo filenameNoPath(filenameWithPath);
    msg = "\nOK to overwrite: " + filenameNoPath.fileName() + "?\n";
    QMessageBox::StandardButton reply = QMessageBox::question(
                m_Parent,"Overwrite",msg,
                QMessageBox::No|QMessageBox::Yes,
                QMessageBox::No);
    return (reply == QMessageBox::Yes);
}

bool
nmfViewerWidget::readDataFile(
        const QString& filename,
        QStringList& header,
        std::vector<boost::numeric::ublas::matrix<double> >& data,
        int& numRows,
        int& numCols)
{
    bool foundHeader = false;
    bool addTheMatrix = false;
    int i,j;
    QString line;

    // Read .csv file and parse into data vector of matrices
    QFile file(filename);
    if (! file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    boost::numeric::ublas::matrix<double> tmpMtx;

    QTextStream in(&file);

    // The first 2 lines are for num species and num years
    line = in.readLine().trimmed();
    numCols = line.split(",")[1].trimmed().toInt() + 1; // +1 to include the first Year columns
    line = in.readLine().trimmed();
    numRows = line.split(",")[1].trimmed().toInt() + 1; // +1 to make the year range inclusive

    nmfUtils::initialize(tmpMtx,numRows,numCols);

    i = 0;
    tmpMtx.clear();
    while (!in.atEnd()) {
        line = in.readLine().trimmed();
        if (line.isEmpty() || (line[0] == "#") || (line[0] == "Y")) {
            if ((! foundHeader) && (line[0] == "Y")) {
                header = line.split(",");
                foundHeader = true;
            }
            if (addTheMatrix) {
                data.push_back(tmpMtx);
                i = 0;
                addTheMatrix = false;
            }
            tmpMtx.clear();
            continue;
        }
        QStringList parts = line.split(",");
        j = 0;
        for (QString part : parts) {
            tmpMtx(i,j++) = part.toDouble();
        }
        addTheMatrix = (i++ == numRows-1);
    }

    return true;
}

void
nmfViewerWidget::refreshList()
{
    QDir directory(m_ImagePath);
    QStringList images = directory.entryList(QStringList() << "*.jpg" << "*.png", QDir::Files);
    m_ImageCMB->clear();
    m_ImageCMB->addItems(images);
}

bool
nmfViewerWidget::resetAndCheckFirstAndLastImages()
{
    bool firstBeforeLast = true;
    QString msg;
    int numImages = m_ImageCMB->count();

    m_FirstImageNum = (m_FirstImageNum < 0) ? 0 : m_FirstImageNum;
    m_LastImageNum  = (m_LastImageNum  < 0) ? numImages-1 : m_LastImageNum;

    firstBeforeLast = (m_FirstImageNum < m_LastImageNum);
    if (! firstBeforeLast) {
        msg = "\nError: First image does not precede last image. Please re-select images.\n";
        QMessageBox::warning(m_Parent, "Error",msg,QMessageBox::Ok);
        m_SetFirstImagePB->setChecked(false);
        m_SetLastImagePB->setChecked(false);
    }
    return firstBeforeLast;
}

void
nmfViewerWidget::setupOutputScreenShotViewerWidgets()
{
    QString msg = "";
    QHBoxLayout* filenameLayt    = new QHBoxLayout();
    QLabel*      filenameLBL     = new QLabel("Output images:");
    m_MainViewerLayt  = new QVBoxLayout();
    m_ImageCMB        = new QComboBox();
    m_PixmapLBL       = new QLabel();
    m_DeletePB        = new QPushButton("del");
    m_RenamePB        = new QPushButton("ren");
    m_RefreshPB       = new QPushButton("ref");
    m_PlayForwardPB   = new QPushButton("▶❙");
    m_PlayReversePB   = new QPushButton("❙◀");
    m_PlayBouncePB    = new QPushButton("◀ ▶");
    m_SetFirstImagePB = new QPushButton("┣");
    m_SetLastImagePB  = new QPushButton("┫");
    m_SetSpeedSL      = new QSlider(Qt::Horizontal);
    m_ImageCMB->setFixedWidth(300);
    filenameLayt->addWidget(filenameLBL);
    filenameLayt->addWidget(m_ImageCMB);
    filenameLayt->addWidget(m_DeletePB);
    filenameLayt->addWidget(m_RenamePB);
    filenameLayt->addWidget(m_RefreshPB);
    filenameLayt->addSpacerItem(new QSpacerItem(10,0,QSizePolicy::Expanding,QSizePolicy::Fixed));
    filenameLayt->addWidget(m_SetFirstImagePB);
    filenameLayt->addWidget(m_PlayReversePB);
    filenameLayt->addWidget(m_PlayBouncePB);
    filenameLayt->addWidget(m_PlayForwardPB);
    filenameLayt->addWidget(m_SetLastImagePB);
    filenameLayt->addWidget(m_SetSpeedSL);
    m_MainViewerLayt->addLayout(filenameLayt);
    m_MainViewerLayt->addWidget(m_PixmapLBL);
    m_PixmapLBL->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_DeletePB->setToolTip("Delete the current output image");
    m_RefreshPB->setToolTip("Refreshes the output image list");
    m_RenamePB->setToolTip("Rename the current output image");
    m_PlayForwardPB->setToolTip("Displays the images from first to last");
    m_PlayReversePB->setToolTip("Displays the images from last to first");
    m_PlayBouncePB->setToolTip("Displays the first to last images back and forth until user clicks Stop");
    m_SetFirstImagePB->setToolTip("Sets the first image for playback");
    m_SetLastImagePB->setToolTip("Sets the last image for playback");
    m_SetSpeedSL->setToolTip("Sets the speed for image playback");
    msg  = "<strong><center>Mark First Image</center></strong>";
    msg += "<br>This button will select the currently viewed image as the first ";
    msg += "image to be viewed when the user clicks one of the play buttons.";
    msg += "</p>";
    m_SetFirstImagePB->setWhatsThis(msg);
    msg  = "<strong><center>Mark Last Image</center></strong>";
    msg += "<br>This button will select the currently viewed image as the last ";
    msg += "image to be viewed when the user clicks one of the play buttons.";
    msg += "</p>";
    m_SetLastImagePB->setWhatsThis(msg);
    msg  = "<strong><center>Set Playback Speed</center></strong>";
    msg += "<br>This slider sets the speed at which the images ";
    msg += "will be displayed. Moving the slider to the right ";
    msg += "increases the speed and moving it to the left decreases the speed.";
    msg += "</p>";
    m_SetSpeedSL->setWhatsThis(msg);
    msg  = "<strong><center>Bounce Play</center></strong>";
    msg += "<br>This button will repeatedly display the images from the first to the last ";
    msg += "and then back to the first. This back and forth display will continue until the user hits the Stop button. ";
    msg += "After it's pressed, the button becomes the Stop button.";
    msg += "</p>";
    m_PlayBouncePB->setWhatsThis(msg);
    msg  = "<strong><center>Play</center></strong>";
    msg += "<br>This button will display the images from the first to the last and then stop.";
    msg += "</p>";
    m_PlayForwardPB->setWhatsThis(msg);
    msg  = "<strong><center>Play Reverse</center></strong>";
    msg += "<br>This button will display the images from the last to the first and then stop.";
    msg += "</p>";
    m_PlayReversePB->setWhatsThis(msg);
    msg  = "<strong><center>Delete</center></strong>";
    msg += "<br>This button will delete the currently displayed image.  Once deleted, the image ";
    msg += "cannot be recovered.";
    msg += "</p>";
    m_DeletePB->setWhatsThis(msg);
    msg  = "<strong><center>Refresh</center></strong>";
    msg += "<br>This button will refresh the image list to the left.  The user may find this useful ";
    msg += "if they manually copy images into the output images directory and then want to refresh the ";
    msg += "image list without having to restart the application.";
    msg += "</p>";
    m_RefreshPB->setWhatsThis(msg);
    msg  = "<strong><center>Rename</center></strong>";
    msg += "<br>This button will allow the user to rename the currently displayed image.";
    msg += "</p>";
    m_RenamePB->setWhatsThis(msg);
    m_ImageCMB->setToolTip("List of output REMORA images");
    msg  = "<strong><center>Output REMORA Images</center></strong>";
    msg += "<br>This is the list of all previously saved REMORA output images. These images are saved ";
    msg += "in a different directory (a subdirectory actually) from the MSSPM output images.";
    msg += "</p>";
    m_ImageCMB->setWhatsThis(msg);
    filenameLBL->setToolTip("List of output REMORA images");
    msg  = "<strong><center>Output REMORA Images</center></strong>";
    msg += "<br>This is the list of all previously saved REMORA output images. These images are saved ";
    msg += "in a different directory (a subdirectory actually) from the MSSPM output images.";
    msg += "</p>";
    filenameLBL->setWhatsThis(msg);

    // Connect to Viewer tab
    m_ViewerImageTab->setLayout(m_MainViewerLayt);

    // Set default sizes
    m_DeletePB->setFixedWidth(40);
    m_RefreshPB->setFixedWidth(40);
    m_RenamePB->setFixedWidth(40);
    m_SetFirstImagePB->setCheckable(true);
    m_SetLastImagePB->setCheckable(true);
    m_PlayReversePB->setFixedWidth(40);
    m_PlayBouncePB->setFixedWidth(40);
    m_PlayForwardPB->setFixedWidth(40);
    m_SetFirstImagePB->setFixedWidth(40);
    m_SetLastImagePB->setFixedWidth(40);
    m_SetSpeedSL->setFixedWidth(200);
    m_SetSpeedSL->setValue(50);

    connect(m_ImageCMB,        SIGNAL(activated(QString)),
            this,              SLOT(callback_ImagesCMB(QString)));
    connect(m_DeletePB,        SIGNAL(clicked()),
            this,              SLOT(callback_DeletePB()));
    connect(m_RefreshPB,       SIGNAL(clicked()),
            this,              SLOT(callback_RefreshPB()));
    connect(m_RenamePB,        SIGNAL(clicked()),
            this,              SLOT(callback_RenamePB()));
    connect(m_PlayReversePB,   SIGNAL(clicked()),
            this,              SLOT(callback_PlayReversePB()));
    connect(m_PlayForwardPB,   SIGNAL(clicked()),
            this,              SLOT(callback_PlayForwardPB()));
    connect(m_PlayBouncePB,    SIGNAL(clicked()),
            this,              SLOT(callback_BounceSetupPlayPB()));
    connect(m_SetFirstImagePB, SIGNAL(toggled(bool)),
            this,              SLOT(callback_SetFirstImagePB(bool)));
    connect(m_SetLastImagePB,  SIGNAL(toggled(bool)),
            this,              SLOT(callback_SetLastImagePB(bool)));
    connect(m_SetSpeedSL,      SIGNAL(valueChanged(int)),
            this,              SLOT(callback_SetSpeedSL(int)));

    updateScreenShotViewer("");

}

void
nmfViewerWidget::showDataTab()
{
    m_ViewerTabW->addTab(m_ViewerDataTab, tr("Data"));
}

void
nmfViewerWidget::showFrame()
{
    m_MainFrame->setFrameShape(QFrame::Box);
    m_MainFrame->setFrameShadow(QFrame::Raised);
    m_MainFrame->setLineWidth(7);
}

void
nmfViewerWidget::stopPlayback()
{
    m_StopPlayback = true;
    m_PlayBouncePB->setText("◀ ▶");
    m_Timer->stop();
}

void
nmfViewerWidget::updateScreenShotViewer(QString filename)
{
    // Load filenames into combo box
    refreshList();

    if (filename.isEmpty()) {
        if (m_ImageCMB->count() > 0) {
            QString firstImage = m_ImageCMB->itemText(0);
            callback_ImagesCMB(firstImage);
        } else {
            m_PixmapLBL->clear();
        }
    } else {
        m_ImageCMB->blockSignals(true);
        m_ImageCMB->setCurrentText(filename);
        m_ImageCMB->blockSignals(false);
        callback_ImagesCMB(filename);
    }
}

void
nmfViewerWidget::callback_BouncePlayNextImage()
{
    m_ImageCMB->setCurrentIndex(m_CurrentImageNum);
    QString currentImage = m_ImageCMB->itemText(m_CurrentImageNum);
    callback_ImagesCMB(currentImage);

    m_CurrentImageNum += m_ImageDirection;

    if ((m_CurrentImageNum == m_FirstImageNum) ||
        (m_CurrentImageNum == m_LastImageNum)) {
            m_ImageDirection *= -1;
    }
}

void
nmfViewerWidget::callback_BounceSetupPlayPB()
{
    if (! resetAndCheckFirstAndLastImages()) {
        return;
    }
    if (m_PlayBouncePB->text() == "◀ ▶") {
        m_PlayBouncePB->setText("⬛");
        m_Timer->start(m_TimeMilliSec);
        m_CurrentImageNum = m_FirstImageNum;
    } else {
        stopPlayback();
    }
    m_ImageDirection = 1;
}

void
nmfViewerWidget::callback_DeletePB()
{
    QString msg;
    QString nextFilename = "";
    QMessageBox::StandardButton reply;

    if (m_ImageCMB->count() == 0) {
        QMessageBox::information(m_Parent, "No Output Images",
                                 "\nNo output image files to delete.\n",
                                 QMessageBox::Ok);
        return;
    }

    msg  = "\nThis will permanently delete the image: " + m_ImageCMB->currentText();
    msg += "\n\nOK to delete? ";
    msg += "\n\nN.B. This cannot be undone.";
    reply = QMessageBox::warning(m_Parent, tr("Image Delete"), tr(msg.toLatin1()),
                                  QMessageBox::No|QMessageBox::Yes,
                                  QMessageBox::Yes);
    if (reply == QMessageBox::Yes) {
        int numImages = m_ImageCMB->count();
        int curImage  = m_ImageCMB->currentIndex();
        int nextImage = 0;
        QString filename = m_ImageCMB->itemText(curImage);
        if (curImage == numImages-1) {
            nextImage = numImages-2;
        } else {
            nextImage = (curImage+1 > numImages-1) ? curImage : curImage+1;
        }
        if (nextImage >= 0) {
            nextFilename = m_ImageCMB->itemText(nextImage);
        }
        QFile::remove(QDir(m_ImagePath).filePath(filename));
        updateScreenShotViewer(nextFilename);
        initializeFirstAndLastImages();
    }
}

void
nmfViewerWidget::callback_ImagesCMB(QString filename)
{
    QString fullFilename = QDir(m_ImagePath).filePath(filename);
    QImage image;
    image.load(fullFilename);

    m_PixmapLBL->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_PixmapLBL->setScaledContents(true);
    m_PixmapLBL->setPixmap(QPixmap::fromImage(image));

    loadDataTables(fullFilename.replace("outputImages","outputData"));

}

void
nmfViewerWidget::callback_PlayForwardPB()
{
    QString currentImage;
    bool okToView = false;
    int numImages = m_ImageCMB->count();
    if (! resetAndCheckFirstAndLastImages()) {
        return;
    }
    m_StopPlayback = false;
    m_PlayBouncePB->setText("⬛");
    for (int i=0; i<numImages; ++i) {
        if (m_StopPlayback) {
            break;
        }
        currentImage = m_ImageCMB->itemText(i);
        if (m_FirstImageNum == i) {
            okToView = true;
        }
        if (okToView) {
            m_ImageCMB->setCurrentIndex(i);
            callback_ImagesCMB(currentImage);
            // Need this so as to ensure each screen shot finishes
            // before the next one starts.
            QApplication::sync();
//            boost::this_thread::sleep(boost::posix_time::microseconds(int(m_TimeMicroSec)));
            QThread::usleep((unsigned long)(m_TimeMicroSec));
//            usleep(m_TimeMicroSec);
        }
        if (m_LastImageNum == i) {
            okToView = false;
        }
    }
    m_PlayBouncePB->setText("◀ ▶");
}

void
nmfViewerWidget::callback_PlayReversePB()
{
    QString currentImage;
    bool okToView = false;
    int numImages = m_ImageCMB->count();
    if (! resetAndCheckFirstAndLastImages()) {
        return;
    }
    m_StopPlayback = false;
    m_PlayBouncePB->setText("⬛");
    for (int i=numImages-1; i>=0; --i) {
        if (m_StopPlayback) {
            break;
        }
        currentImage = m_ImageCMB->itemText(i);
        if (m_LastImageNum == i) {
            okToView = true;
            if (m_SkipEndImages)
                continue;
        }
        if ((m_FirstImageNum == i) && (m_SkipEndImages)) {
            okToView = false;
            continue;
        }

        if (okToView) {
            m_ImageCMB->setCurrentIndex(i);
            callback_ImagesCMB(currentImage);
            // Need this so as to ensure each screen shot finishes
            // before the next one starts.
            QApplication::sync();
//            boost::this_thread::sleep(boost::posix_time::microseconds(int(m_TimeMicroSec)));
            QThread::usleep((unsigned long)(m_TimeMicroSec));
//            usleep(m_TimeMicroSec);
        }
        if (m_FirstImageNum == i) {
            okToView = false;
        }
    }
    m_PlayBouncePB->setText("◀ ▶");
}

void
nmfViewerWidget::callback_RefreshPB()
{
    initializeFirstAndLastImages();
    updateScreenShotViewer(m_ImageCMB->currentText());
}

void
nmfViewerWidget::callback_RenamePB()
{
    bool ok;
    bool okToWrite = false;
    QString msg;
    QString renamedFilenameWithPath;
    QString currentText = m_ImageCMB->currentText();;
    QFileInfo fileInfo(currentText);
    QString currentSuffix = fileInfo.completeSuffix();
    QString currentFilenameWithPath;

    initializeFirstAndLastImages();

    msg  = "\nCurrent image: " + currentText;
    msg += "\n\nRename to:";
    QString renamedFilename = QInputDialog::getText(
                m_Parent, "Rename Image",
                msg, QLineEdit::Normal,
                currentText, &ok);
    renamedFilenameWithPath = QDir(m_ImagePath).filePath(renamedFilename);
    QFileInfo newfile(renamedFilenameWithPath);

    QString suffix = newfile.completeSuffix();
    if ((! suffix.endsWith("jpg")) && (! suffix.endsWith("png"))) {
        renamedFilename += ".jpg";
        renamedFilenameWithPath += ".jpg";
    }

    if (ok && !renamedFilenameWithPath.isEmpty()) {
        if (nmfUtilsQt::fileExists(renamedFilenameWithPath)) {
            okToWrite = okToOverwrite(renamedFilenameWithPath);
            if (okToWrite) {
                QFile::remove(renamedFilenameWithPath);
            }
        } else {
            okToWrite = true;
        }
        if (okToWrite) {
            currentFilenameWithPath = QDir(m_ImagePath).filePath(currentText);
            QFile::rename(currentFilenameWithPath,
                          renamedFilenameWithPath);
//            msg = "Couldn't rename " + currentFilenameWithPath + " to " +
//                    renamedFilenameWithPath;
//            m_Logger->logMsg(nmfConstants::Error,msg.toStdString());
            refreshList();
            m_ImageCMB->setCurrentText(renamedFilename);
        }
    }
}

void
nmfViewerWidget::callback_SetFirstImagePB(bool pressed)
{
    m_FirstImageNum = (pressed) ? m_ImageCMB->currentIndex() : 0;
    m_CurrentImageNum = m_FirstImageNum;
}

void
nmfViewerWidget::callback_SetLastImagePB(bool pressed)
{
    m_LastImageNum = (pressed) ? m_ImageCMB->currentIndex() :
                                 m_ImageCMB->count()-1;
}

void
nmfViewerWidget::callback_SetSpeedSL(int value)
{
    int sliderMax = m_SetSpeedSL->maximum();
    double val = double(sliderMax-value)/sliderMax;
    val = (MAX_SPEED-MIN_SPEED)*val + MIN_SPEED;
    m_TimeMicroSec = 1000000.0*val;
    m_TimeMilliSec =    1000.0*val;

    if (m_Timer->isActive()) {
        m_Timer->stop();
        m_Timer->start(m_TimeMilliSec);
    }
}
