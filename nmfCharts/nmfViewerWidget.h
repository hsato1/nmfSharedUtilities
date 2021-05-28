
/**
 @file nmfViewerWidget.h
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief This file contains the definition for the screenshot image viewer widget.
 @date Feb 16, 2018
 */

#ifndef NMFVIEWERWIDGET_H
#define NMFVIEWERWIDGET_H

#include <QApplication>
#include <QComboBox>
#include <QDir>
#include <QHBoxLayout>
#include <QImage>
#include <QInputDialog>
#include <QLabel>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QSpacerItem>
#include <QTimer>
#include <QThread>              /*added QThread to reduce dependancy on boost*/
#include <QVBoxLayout>

#include "nmfLogger.h"
#include "nmfUtilsQt.h"
#include <chrono>
#include <thread>
#include <boost/thread/thread.hpp>

/**
 * @brief This class is an image viewer class that allows the user to sequence through
 * a series of images. Using the VCR-like controls, the user can mark a start image and
 * an end image and cycle through the images in between, automatically, in an animated fashion.
 * <br><br>
 * To use this class, set a child widget to be the viewer's main widget.
 * <br><br>
 * For example, if the parent widget is:
 * <br><br>
 * DockWidgetParent, call:
 * <br><br>
 * viewerWidget = nmfViewerWidget(...)<br>
 * DockWidgetParent->setWidget(viewerWidget->getMainWidget());
 *
 */
class nmfViewerWidget : public QObject
{

    Q_OBJECT

private:
    const double MIN_SPEED = 0.1;
    const double MAX_SPEED = 4.0;
    nmfLogger*   m_Logger;
    bool         m_SkipEndImages;
    bool         m_StopPlayback;
    int          m_FirstImageNum;
    int          m_LastImageNum;
    int          m_CurrentImageNum;
    int          m_ImageDirection;
    double       m_TimeMicroSec;
    double       m_TimeMilliSec;
    QComboBox*   m_ImageCMB;
    QFrame*      m_MainFrame;
    QLabel*      m_PixmapLBL;
    QPushButton* m_DeletePB;
    QPushButton* m_RefreshPB;
    QPushButton* m_RenamePB;
    QPushButton* m_PlayForwardPB;
    QPushButton* m_PlayReversePB;
    QPushButton* m_PlayBouncePB;
    QPushButton* m_SetFirstImagePB;
    QPushButton* m_SetLastImagePB;
    QSlider*     m_SetSpeedSL;
    QString      m_ImagePath;
    QTabWidget*  m_ViewerTabW;
    QTabWidget*  m_ViewerDataSubTabW;
    QTimer*      m_Timer;
    QVBoxLayout* m_MainViewerLayt;
    QWidget*     m_Parent;
    QWidget*     m_MainWidget;
    QWidget*     m_ViewerWidget;
    QWidget*     m_ViewerImageTab;
    QWidget*     m_ViewerDataTab;
    QWidget*     m_ViewerTab1;

    void initializeFirstAndLastImages();
    void loadDataTables(QString fullFilename);
    bool okToOverwrite(QString filenameWithPath);
    bool readDataFile(
            const QString& filename,
            QStringList& header,
            std::vector<boost::numeric::ublas::matrix<double> >& data,
            int& numRows,
            int& numCols);
    bool resetAndCheckFirstAndLastImages();
    void setupOutputScreenShotViewerWidgets();

public:
    /**
     * @brief Class constructor for the image viewer user interface
     * @param parent : parent widget in which to place this user interface
     * @param imagePath : directory in which images are located
     * @param logger : pointer to logger
     */
    nmfViewerWidget(QWidget* parent,
                    const QString& imagePath,
                    nmfLogger* logger);
   ~nmfViewerWidget();


    /**
     * @brief Adds a subtab to the viewer's Data tab
     * @param title : title of tab
     * @param tableview : tableview to be displayed in tab
     */
    void addDataTab(const QString& title,
                    QTableView* tableview);
    /**
     * @brief Gets the currently displayed image file name
     * @return The current image name
     */
    QString getCurrentFilename();
    /**
     * @brief Returns the main widget that contains all of the image viewers widgets
     * @return Parent widget to all of the image viewers widgets
     */
    QWidget* getMainWidget();
    /**
     * @brief Hides the data tab from the viewer
     */
    void hideDataTab();
    /**
     * @brief Hides the frame around the viewer
     */
    void hideFrame();
    /**
     * @brief Re-reads images from the image directory and reloads the image combobox. Useful
     * if someone puts images into the image directory outside of the application.
     */
    void refreshList();
    /**
     * @brief Sets the image path for the viewer if it's changed after the constructor is called
     * @param imagePath : directory where captured images are kept
     */
    void setImagePath(const QString& imagePath);
    /**
     * @brief Shows the data tab in the viewer
     */
    void showDataTab();
    /**
     * @brief Show a previously hidden frame and restore it to its 3d appearance
     */
    void showFrame();
    /**
     * @brief Stops any image playback
     */
    void stopPlayback();
    /**
     * @brief Refreshes image viewer and sets current image to the passed in file
     * @param filename : image file to be viewed
     */
    void updateScreenShotViewer(QString filename);

public Q_SLOTS:
    /**
     * @brief Callback invoked by a timer after the user has clicked the Bounce Play button
     */
    void callback_BouncePlayNextImage();
    /**
     * @brief Callback invoked to setup the Bounce Play functionality once the user clicks the
     * Bounce Play button
     */
    void callback_BounceSetupPlayPB();
    /**
     * @brief Callback invoked when the user clicks the Delete Image button
     */
    void callback_DeletePB();
    /**
     * @brief Callback invoked when the user selects from the image combo box
     * @param filename : current image set in the image combo box
     */
    void callback_ImagesCMB(QString filename);
    /**
     * @brief Callback invoked when the user clicks the Play Forward button
     */
    void callback_PlayForwardPB();
    /**
     * @brief Callback invoked when the user clicks the Play Reverse button
     */
    void callback_PlayReversePB();
    /**
     * @brief Callback invoked when the user clicks the Refresh Image button
     */
    void callback_RefreshPB();
    /**
     * @brief Callback invoked when the user clicks the Rename Image button
     */
    void callback_RenamePB();
    /**
     * @brief Callback invoked when the user clicks the Set First Image button
     * @param pressed : state of the toggle button (true if pressed, false if unpressed)
     */
    void callback_SetFirstImagePB(bool pressed);
    /**
     * @brief Callback invoked when the user clicks the Set Last Image button
     * @param pressed : state of the toggle button (true if pressed, false if unpressed)
     */
    void callback_SetLastImagePB(bool pressed);
    /**
     * @brief Callback invoked when the user manipulates the Playback Speed slider
     * @param value : value of the slider set by the user
     */
    void callback_SetSpeedSL(int value);

};


#endif // NMFVIEWERWIDGET_H
