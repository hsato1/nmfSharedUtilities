
/**
 * @file nmfUtilsQt.h
 * @brief Definition for Qt widgets and functions that operate on widgets.
 * @date Jul 26, 2017
 *
 * This file contains common Qt widgets and functions that operate on Qt widgets
 * as used by the MultiSpecies applications.
 *
 */

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <QApplication>
#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QtCharts/QChart>
#include <QClipboard>
#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QEventLoop>
#include <QFile>
#include <QFileDialog>
#include <QGridLayout>
#include <QInputDialog>
#include <QItemSelectionModel>
#include <QLabel>
#include <QLineEdit>
#include <QLineSeries>
#include <QMainWindow>
#include <QMessageBox>
#include <QMimeData>
#include <QModelIndexList>
#include <QProcess>
#include <QProgressDialog>
#include <QPushButton>
#include <QScrollBar>
#include <QSettings>
#include <QSpacerItem>
#include <QStackedBarSeries>
#include <QStandardItemModel>
#include <QString>
#include <QTableView>
#include <QTableWidget>
#include <QTabWidget>
#include <QTextEdit>
#include <QTextStream>
#include <QTimer>
#include <QTreeWidgetItem>
#include <QValueAxis>

#include "nmfUtils.h"
#include "nmfStructsQt.h"
#include "nmfConstantsMSCAA.h"
#include "nmfConstantsMSSPM.h"
#include "nmfConstantsMSVPA.h"
#include "nmfConstantsDonut.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/multi_array.hpp>
#include <boost/version.hpp>

typedef boost::multi_array<double, 3> Boost3DArrayDouble;
typedef boost::multi_array<double, 4> Boost4DArrayDouble;
typedef boost::multi_array<double, 5> Boost5DArrayDouble;

//struct ADMBFilesStruct {
//    QString dataFile;
//    QString parameterFile;
//    QString pinFile;
//    QString tplFile;
//    QString reportFile;
//    QString buildOutput;
//    QString runOutput;
//};


QT_CHARTS_USE_NAMESPACE

/**
 * @brief This class creates a dialog that contains a text edit widget displaying
 * the contents of the passed filename.  The dialog also includes a font combo button
 * which allows the user to change the font size of the displayed text. There's an OK
 * button to close the dialog.
 */
class nmfFileViewer : public QDialog {

    Q_OBJECT

    QTextEdit* m_TextEdit;

public:
    /**
     * @brief Constructor class for the file viewer dialog widget
     * @param parent : parent widget over which to display this dialog widget
     * @param filename : name of file to read and display in this widget's text edit box
     */
    nmfFileViewer(QWidget* parent = 0,
                  const char* filename = 0);

private slots:
    void callback_FontSizePB(QString fontSize);
};

class nmfToolTip : public QWidget {

    Q_OBJECT

    QLabel* m_Label;

public:
    nmfToolTip();
    ~nmfToolTip() {}
    void setLabel(const QString& label);
};

/*
// Currently unused and TBD. Will be further developed in
// upcoming releases.
class nmfTableView: public QTableView
{
    Q_OBJECT

public:
    explicit nmfTableView(QWidget *parent = 0);
//  virtual ~nmfTableView()=0;

    void copy();
    void paste();
    void pasteAll();
    void clearAll();
    void clear();
    void selectAllItems();
    void deselectAllItems();

    void keyPressEvent(QEvent* e);
};

class nmfTableView2: public QTableView
{
    Q_OBJECT

public:
    explicit nmfTableView2(QWidget *parent = 0);
//  virtual ~nmfTableView2()=0;

    void copy();
    void paste();
    void pasteAll();

    void clearAll();
    void clear();
    void selectAllItems();
    void deselectAllItems();
};
*/

/**
 * @brief Namespace containing Qt utility functions
 */
namespace nmfUtilsQt {

    /**
     * @brief Background color specification for a read only line edit widget
     */
//    const QString ReadOnlyLineEditBgColor     = "QLineEdit { background: rgb(250,240,230);}";
//    const QString ReadOnlyLineEditBgColorDark = "QLineEdit {background: rgb(103,110,113);}";
    const QString ReadOnlyLineEditBgColorLight = "QLineEdit {background: rgb(240,240,240);}";
    const QString ReadOnlyLineEditBgColorDark  = "QLineEdit {background: rgb(110,110,110);}";
    /**
     * @brief adds a widget tree item under the passed parent item
     * @param parent : parent widget item
     * @param label : text label for widget item
     */
    void addTreeItem(QTreeWidgetItem* parent,
                     QString          label);
    /**
     * @brief Adds a widget tree root item to the passed tree widget
     * @param parent : parent tree widget
     * @param label : text label for root tree widget item
     * @return Returns the newly created tree widget item
     */
    QTreeWidgetItem* addTreeRoot(QTreeWidget* parent,
                                 QString      label);

    /**
     * @brief Checks that all cells in a QTableView table are populated
     * @param tabWidget : parent tab widget of the QTableView widget
     * @param tableView : tableView widget whose cells are being checked
     * @param showError : boolean signifying if error should be shown with a popup
     * @return True if no errors, False if one or more cells are not populated
     */
    bool allCellsArePopulated(QTabWidget* tabWidget,
                              QTableView* tableView,
                              bool        showError);
    /**
     * @brief Checks that all cells in a QTableView table are populated
     * @param tabWidget : parent tab widget of the QTableView widget
     * @param tableView : tableView widget whose cells are being checked
     * @param showError : boolean signifying if error should be shown with a popup
     * @param skipFirstRow : boolean signifying user wants to skip checking the first row of the table
     * @return True if no errors, False if one or more cells are not populated
     */
    bool allCellsArePopulated(
            QTabWidget*   tabWidget,
            QTableWidget* tableWidget,
            bool          showError,
            bool          skipFirstRow);
    /**
     * @brief Checks to see if all of the values in the second model are greater than
     * or equal to all of the values in the first model (on a cell by cell basis)
     * @param smodelMin : the minimum model to check
     * @param smodelMax : the maximum model to check
     * @return True if all the max model cells are greater than or equal to the min model cells, else False
     */
    bool allMaxCellsGreaterThanMinCells(
            QStandardItemModel* smodelMin,
            QStandardItemModel* smodelMax);
    /**
     * @brief Calculates the sum of the widths of the column range
     * @param tableview : tableview whose column widths are to be measured
     * @param firstCol : first column in range to be measured
     * @param lastCol : last column in range to be measured
     * @return
     */
    int calculateMultiColumnWidth(QTableView* tableview,
                                  const int&  firstCol,
                                  const int&  lastCol);
    /**
     * @brief Creates the passed directories if they don't already exist
     * @param dir : typically a hidden application directory
     * @param dataDir : the application data directory
     * @param logDir : the application log directory
     */
    void checkForAndCreateDirectories(std::string dir,
                                      std::string dataDir,
                                      std::string logDir);
    /**
     * @brief Deletes log files after confirming with the user
     * @param name : name of application
     * @param logDir : directory in which to put the log files
     * @param logFilter : file filter for the log files
     */
    void checkForAndDeleteLogFiles(QString     name,
                                   std::string logDir,
                                   std::string logFilter);
    /**
     * @brief Removes any non-alphanumeric characters and replaces any space
     * characters with an underscore character
     * @param stringValue : the string value to check and modify if necessary
     */
    void checkForAndReplaceInvalidCharacters(QString& stringValue);
    /**
     * @brief Clears the current tableView selection or the entire tableView if there's no selection
     * @param qtApp : qt application
     * @param tableView : tableView widget
     * @return Returns error msg if there was an error, else returns the empty string
     */
    QString clear(QApplication* qtApp,
                  QTableView*   tableView);
    /**
     * @brief Clears every cell in the passed tableView widget
     * @param tableView : tableView widget to clear
     * @return Returns error message if there was an error, else returns the empty string
     */
    QString clearAll(QTableView* tableView);
    /**
     * @brief Clear the passed comma separated values (CSV) file
     * @param tableToClear : name of table to clear
     * @param projectDir : name of project directory
     * @param errorMsg : error message (empty string if no error, otherwise the error message)
     * @return True if there was an error, else False
     */
    bool clearCSVFile(std::string  tableToClear,
                      std::string  projectDir,
                      std::string& errorMsg);
    /**
     * @brief Clears the current selection
     * @param tableView : tableView whose selection is to be cleared
     * @param indexes : cell indexes to clear
     * @return Empty string (currently unused)
     */
    QString clearSelection(QTableView*     tableView,
                           QModelIndexList indexes);
    /**
     * @brief Clears the entire tableView of all the passed tableViews
     * @param tables : list of tableViews to clear
     */
    void clearTableView(const QList<QTableView*>& tables);
    /**
     * @brief Converts a vector of std::string types to a QStringList
     * @param stdStringVector : vector of std::string types
     * @param qStringlist : the returned (by reference) QStringList
     */
    void convertVectorToStrList(const std::vector<std::string>& stdStringVector,
                                QStringList& qStringlist);
    /**
     * @brief Copies the selected QTableView cells
     * @param qtApp : Qt application
     * @param tableView : tableView widget
     * @return Returns error message or empty string if there is no error
     */
    QString copy(QApplication* qtApp,
                 QTableView*   tableView);
    /**
     * @brief Creates a Qt settings file
     * @param windowsDir : windows directory for the location of
     * the settings file if running on a Windows operating system
     * @param name : prefix name for Windows settings file
     * @return
     */
    QSettings* createSettings(const std::string& windowsDir,
                              const QString&     name);
//    /**
//     * @brief delays program execution by the passed number of milliseconds
//     * @param milliseconds : number of milliseconds by which to delay program execution
//     */
//    void delayMSec(const int& milliseconds);
    /**
     * @brief deselects all cells in the passed tableView
     * @param tableView : tableView to deselect cells
     * @return Returns error message if an error was found, else returns an empty string
     */
    QString deselectAll(QTableView* tableView);
    /**
     * @brief Equalizes the lengths of the 2 passed strings by
     * padding the shorter with spaces at the beginning of the string
     * @param string1 : first string to compare
     * @param string2 : second string to compare
     */
    void equalizeQStringLengths(QString& string1,
                                QString& string2);
    /**
     * @brief Tests if field is empty or not
     * @return Returns boolean describing if argument is empty string
     */
    bool emptyField(QStringList fields);
    /**
     * @brief Checks to see if the passed in filename exists
     * @param filenameWithPath : file to check for existence
     * @return Boolean signifying if the passed in filename exists
     */
    bool fileExists(QString filenameWithPath);
    /**
     * @brief Gets the name of the current tab
     * @param tabWidget : tab widget containing the tab in question
     * @return Returns the name of the current tab
     */
    QString getCurrentTabName(QTabWidget* tabWidget);
    /**
     * @brief Get the index value for the names tab
     * @param tabWidget : tab widget containing the tab in question
     * @param tabName : the name of the tab whose index is to be returned
     * @return Returns the index of the named tab widget
     */
    int getTabIndex(QTabWidget* tabWidget, QString tabName);
    /**
     * @brief Determine if the passed string is an error message
     * @param errorMsg : return string from previous database call
     * @return True if errorMsg represents an error, False if it does not
     */
    bool isAnError(std::string errorMsg);
    /**
     * @brief Load a non time series .csv file into a QTableView
     * @param parentTabWidget : parent tab containing the QTableView object
     * @param tableView : QTableView that will contain the .csv data
     * @param type : type of tableview (i.e., "Guild" or "Species")
     * @param inputDataPath : default path for the input .csv file
     * @param inputFilename : CSV filename if default is not desired
     * @param errorMsg : error message produced during the load
     * @return : Boolean signifying a successful load (true) or an unsuccessful load (false)
     */
    bool loadGuildsSpeciesTableview(QTabWidget* parentTabWidget,
                                    QTableView* tableView,
                                    const QString& type,
                                    const QString& inputDataPath,
                                    const QString& inputFilename,
                                    QList<QString>& SpeciesGuilds,
                                    QString& errorMsg);
    /**
     * @brief Loads all of the multi-run file data into a vector of QStrings
     * @param DataStruct : struct containing all run parameters
     * @param MultiRunLines : contents of multi-run file
     * @param TotalIndividualRuns : total number of individual runs
     * @return True if a successful load, false otherwise
     */
    bool loadMultiRunData(const nmfStructsQt::ModelDataStruct& DataStruct,
                          std::vector<QString>& MultiRunLines,
                          int& TotalIndividualRuns);
    /**
     * @brief Load data from a .csv file into a QTableWidget
     * @param parentTabWidget : parent widget onto which any popup message will be displayed
     * @param tableWidget : table widget that will contain the csv data
     * @param inputDataPath : input directory containing the csv data
     * @param guildValues : QStringList containing all of the guilds
     * @param errorMsg : passes back any error message
     * @return Boolean with true for a successful load else false
     */
    bool loadTableWidgetData(QTabWidget* parentTabWidget,
                             QTableWidget* tableWidget,
                             const QString& inputDataPath,
                             const QStringList& guildValues,
                             QString& errorMsg);
    /**
     * @brief Load a time series .csv file into a QTableView
     * @param parentTabWidget : parent tab containing the QTableView object
     * @param tableView : QTableView that will contain the .csv data
     * @param inputDataPath : default path for the input .csv file
     * @param inputFilename : CSV filename if default is not desired
     * @param firstLineReadOnly : boolean to make the first line of the table read-only with gray background
     * @param scientificNotation : boolean to display data using scientific notation
     * @param nonZeroCell : row,col of first non-zero cell; used to set the pct range spin box where appropriate
     * @param errorMsg : error message produced during the load
     * @return : Boolean signifying a successful load (true) or an unsuccessful load (false)
     */
    bool loadTimeSeries(QTabWidget* parentTabWidget,
                        QTableView* tableView,
                        const QString& inputDataPath,
                        const QString& inputFilename,
                        const bool& firstLineReadOnly,
                        const bool& scientificNotation,
                        std::pair<int,int>& nonZeroCell,
                        QString& errorMsg);
    /**
     * @brief Copies the contents of the first model into the second model
     * @param originalModel : original model containing the data to be copied
     * @param copyModel : new model to receive a copy of the data from the original model
     */
    void modelCopy(QStandardItemModel* originalModel, QStandardItemModel* copyModel);
    /**
     * @brief Tests the passed fields to see if the parameters are out of range of their min, max fields
     * @param fields : list of parameters and their respective ranges (i.e., GrowthRate,GrowthRateMin,GrowthRateMax,...)
     * @param badParam : name of the out of range parameter
     * @return Boolean signifying if any of the passed parameters were outside of their respective ranges
     */
    bool outOfRange(QStringList fields, QString& badParam);
    /**
     * @brief Pastes what's been saved to the clipboard into the passed tableView
     * @param qtApp : Qt application from which to get the copy buffer
     * @param tableView : tableView to paste into
     * @return Returns error message if an error was found, else returns an empty string
     */
    QString paste(QApplication* qtApp,
                  QTableView*   tableView);
    /**
     * @brief Pastes the first cell of whatever is in the clipboard to every cell in the
     * tableView and then deselects the tableView
     * @param qtApp : Qt application from which to get the copy buffer
     * @param tableView : tableView to paste into
     * @return Returns error message if an error was found, else returns an empty string
     */
    QString pasteAll(QApplication* qtApp,
                     QTableView* tableView);
    /**
     * @brief Reloads the passed dataStruct with the passed line from the multi-run line file
     * @param dataStruct : the data struct to reload
     * @param MultiRunLine : a line from the multi-run line file
     */
    void reloadDataStruct(
            nmfStructsQt::ModelDataStruct& dataStruct,
            const QString& MultiRunLine);
    /**
     * @brief Removes the Qt settings file from the file system.
     * Useful for resetting the application GUI in case it disappears
     * (i.e., in case some bad GUI settings are saved)
     * @return True if delete worked OK, else False
     */
    bool removeSettingsFile();
    /**
     * @brief Effectively does a file rename...but not quite. It copies the first
     * to the second and then removes the first.  I did this as the std:rename
     * function didn't work on Windows...and a system() mv command caused a shell
     * to pop up briefly over the UI.
     * @param fileIn : input file to rename
     * @param fileOut : the renamed output file
     * @return Returns 1 if rename was successful, -1 if not successful
     */
    int rename(QString fileIn,
               QString fileOut);
    /**
     * @brief Loads the CSV file into the passed tableview's model
     * @param projectDir : directory where application files are written to
     * @param fileType : type of csv file (used as a check, so the user knows they're opening the correct type of file)
     * @param table : qtableview containing data to be written out
     * @param filename : if queryFilename = false, then this is the filename to be used; otherwise the method will query the user
     * @param numRows : number of rows read from file
     * @return Returns true if load was successful, else false
     */
    bool loadModelFromCSVFile(std::string projectDir,
                              std::string fileType,
                              QTableView* table,
                              QString filename,
                              int& numRows);
    /**
     * @brief Saves the data in the passed model to the appropriate directory and file
     * @param projectDir : directory where application files are written to
     * @param fileType : type of csv file (needed as a check so user can't open incorrect type of csv file)
     * @param tabName : name of the tab whose data is to be written out; method uses
     * it to determine what to name the vertical headers
     * @param table : qtableview containing data to be written out
     * @param queryFilename : true if method should query user for filename, false otherwise
     * @param theFilename : if queryFilename = false, then this is the filename to be used; otherwise the method will query the user
     */
    void saveModelToCSVFile(std::string projectDir,
                            std::string fileType,
                            std::string tabName,
                            QTableView* table,
                            bool queryFilename,
                            QString theFilename);
    /**
     * @brief Saves the data in the passed non time series but a table view model to a
     * csv file in the passed directory
     * @param parentTabWidget : parent table widget of table to be saved
     * @param smodel : table model containing data to be saved
     * @param inputDataPath : default path where .csv file is to be saved
     * @param outputFilename : name of the output filename with path
     * @param GuildName : guild names
     * @param GrowthRate : guild growth rates
     * @param GuildK : guild carrying capacities
     * @return true/false denoting successful or unsuccessful save
     */
    bool saveGuildsTableView(QTabWidget* parentTabWidget,
                       QStandardItemModel* smodel,
                       QString& inputDataPath,
                       QString& outputFilename,
                       QList<QString>& GuildName,
                       QList<QString>& GrowthRate,
                       QList<QString>& GuildK);
    /**
     * @brief Saves the data in the passed non time series but a table view model to a
     * csv file in the passed directory
     * @param parentTabWidget : parent table widget of table to be saved
     * @param smodel : table model containing data to be saved
     * @param inputDataPath : default path where .csv file is to be saved
     * @param outputFilename : name of the output filename with path
     * @param SpeciesName : list of Species names
     * @param SpeciesGuild : list of guilds each species is a member of
     * @param SpeciesInitialBiomass : list of species initial biomasses
     * @param SpeciesGrowthRate : list of species growth rates
     * @param SpeciesK : list of species carrying capacities
     * @return true/false denoting successful or unsuccessful save
     */
    bool saveSpeciesTableView(QTabWidget* parentTabWidget,
                              QStandardItemModel* smodel,
                              QString& inputDataPath,
                              QString& outputFilename,
                              QList<QString>& SpeciesName,
                              QList<QString>& SpeciesGuild,
                              QList<QString>& SpeciesInitialBiomass,
                              QList<QString>& SpeciesGrowthRate,
                              QList<QString>& SpeciesK);

    /**
     * @brief Saves the data in the passed model to a csv file in the passed directory
     * @param parentTabWidget : parent table widget of table to be saved
     * @param smodel : table model containing data to be saved
     * @param inputDataPath : default path where .csv file is to be saved
     * @param outputFilename : name of the output filename with path
     * @return true/false denoting successful or unsuccessful save
     */
    bool saveTimeSeries(QTabWidget* parentTabWidget,
                        QStandardItemModel* smodel,
                        QString& inputDataPath,
                        QString& outputFilename);
    /**
     * @brief Save the passed QTableWidget data to a .csv file
     * @param parentTabWidget : parent widget onto which will be displayed any popups
     * @param tableWidget : QTableWidget containing data to save as .csv file
     * @param inputDataPath : directory containing the project's .csv files
     * @param outputFilename : name of .csv output file if default name is not to be used
     */
    void saveTableWidgetData(QTabWidget* parentTabWidget,
                             QTableWidget* tableWidget,
                             QString& inputDataPath,
                             const QString& outputFilename);
    /**
     * @brief Selects all items in passed table view widget
     * @param tableView : table view widget in which to select all elements
     * @return Returns a message if an error found, else returns an empty string
     */
    QString selectAll(QTableView* tableView);
    /**
     * @brief Clears the passed textEdit widget if clear argument is True
     * @param textEdit : the textEdit widget to clear
     * @param clear : if True, the passed textEdit widget will be cleared, else it won't be cleared
     */
    void sendToOutputWindow(
            QTextEdit* textEdit,
            bool       clear);
    /**
     * @brief Sends text to output textEdit widget for display
     * @param textEdit : textEdit widget in which to place the passed text
     * @param label : text to display as bold text
     * @param content : text to display as normal text
     */
    void sendToOutputWindow(
            QTextEdit*  textEdit,
            std::string label,
            std::string content);
    /**
     * @brief Sends matrix to textEdit widget for display
     * @param textEdit : textEdit widget in which to place the passed widget
     * @param title : title of matrix to display
     * @param rowTitles : row labels of matrix to display
     * @param colTitles : column labels of matrix to display
     * @param outMatrix : output matrix of double type values to display
     * @param numDigits : number of characters to allocate for each matrix value
     * @param numDecimals : number of decimal places that each matrix value will have
     */
    void sendToOutputWindow(
            QTextEdit*               textEdit,
            std::string              title,
            std::vector<std::string> rowTitles,
            std::vector<std::string> colTitles,
            boost::numeric::ublas::matrix<double>& outMatrix,
            int                      numDigits,
            int                      numDecimals);
    /**
     * @brief Convenience function to reset an X axis (the original Qt call was deprecated)
     * @param chart : the chart on which the axis is to be placed
     * @param axisX : the new axis with which to replace the old one
     * @param series : the series to which the axis refers
     */
    void setAxisX(QChart*      chart,
                  QValueAxis*  axisX,
                  QLineSeries* series);
    void setAxisX(QChart*      chart,
                  QBarCategoryAxis*  axisX,
                  QStackedBarSeries* series);
    void setAxisX(QChart*            chart,
                  QBarCategoryAxis*  axisX,
                  QBarSeries*        series);
    /**
     * @brief Convenience function to reset a Y axis (the original Qt call was deprecated)
     * @param chart : the chart on which the axis is to be placed
     * @param axisX : the new axis with which to replace the old one
     * @param series : the series to which the axis refers
     */
    void setAxisY(QChart*      chart,
                  QValueAxis*  axisY,
                  QLineSeries* series);
    void setAxisY(QChart*      chart,
                  QValueAxis*  axisY,
                  QStackedBarSeries* series);
    void setAxisY(QChart*      chart,
                  QValueAxis*  axisY,
                  QBarSeries* series);
    /**
     * @brief Sets the min max range tables using the passed in pct value
     * @param pct : percent value to create the min and max range tables
     * @param tableView : initial table
     * @param tableView1 : minimum table
     * @param tableView2 : maximum table
     */
    void setMinMax(const double& pct,
                   QTableView* tableView,
                   QTableView* tableView1,
                   QTableView* tableView2);
    /**
     * @brief Sets the min max range on only the selected cells using the passed in pct value
     * @param pct : percent value to create the min and max range tables
     * @param tableView0 : initial table
     * @param tableView1 : minimum table
     * @param tableView2 : maximum table
     */
    void setMinMaxOnSelections(const double& pct,
                               QTableView* tableView0,
                               QTableView* tableView1,
                               QTableView* tableView2);
    /**
     * @brief Show the About dialog for the application
     * @param parent : parent needed so as to know how to position the popup
     * @param name : name of the application
     * @param operatingSystem : operating system application is running on
     * @param version : version of the application
     * @param specialAcknowledgement : special acknowledgement section
     * @param appMsg : main application message (created in calling routine);
     * lists all toolkits, versions, and links for more information
     */
    void showAboutWidget(
            QWidget* parent,
            const QString& name,
            const QString& operatingSystem,
            const QString& version,
            const QString& specialAcknowledgement,
            const QString& appMsg);
    /*
     * @brief Useful for changing .jpg,.png to .csv
     * @param filename : file name on which to modify the extension
     * @param newExt : the new file name extension
     * @param oldExts : list of old file name extensions to check for and change if found
     */
    void switchFileExtensions(QString& filename,
                              const QString& newExt,
                              const QStringList& oldExts);
    /**
     * @brief Convenience method to see if anything in either table has been selected
     * @param tableView1 : one tableview to check for selection
     * @param tableView2 : another tableview to check for selection
     * @return Returns true if a selection in either tableview has been found, else false
     */
    bool thereAreSelections(QTableView* tableView1,
                            QTableView* tableView2);
    /**
     * @brief Convenience method to take the transpose of a model
     * @param tv : tableview widget whose model to transpose
     */
    void transposeModel(QTableView* tv);
    /**
     * @brief Extracts the embedded tag from the passed filename
     * @param filename : name of file to extract tag from
     * @param tag : descriptor tag embedded in filename
     * @return Boolean : true - success, false - failure
     */
    bool extractTag(const QString filename,
                    QString& tag);
    /**
     * @brief Returns the current time
     * @return the current time
     */
    QDateTime getCurrentTime();
    /**
     * @brief Calculates the time elapsed between the passed startTime and the currentTime
     * @param startTime : The start time with which to calculate the elapsed time
     * @return The number of hours, minutes, or seconds that have elapsed (in a string format)
     */
    std::string elapsedTime(QDateTime startTime);
    /**
     * @brief Calculates the time elapsed between the passed startTime and the currentTime and displays it in a condensed format
     * @param startTime : The start time with which to calculate the elapsed time
     * @return A string represent the time passed in format: hh:mm:ss
     */
    std::string elapsedTimeCondensed(QDateTime startTime);

    /**
     * @brief Updates the passed QProgressDialog widget
     * @param logger : application logger
     * @param dlg : QProgressDialog widget
     * @param msg : message to display on QProgressDialog widget
     * @param pInc : QProgressDialog increment
     */
    void updateProgressDlg(nmfLogger* logger,
                           QProgressDialog* dlg,
                           const std::string& msg,
                           int& pInc);

    void setBackgroundLineEdit(QLineEdit* lineEdit,
                               const QString& backgroundStyleSheet);
} // end namespace
