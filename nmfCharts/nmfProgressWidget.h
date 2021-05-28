
/**
 @file nmfProgressWidget.h
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief This file contains the definition for the Progress Chart dock widget.
 @date Feb 16, 2018
 */

#ifndef NMFPROGRESSWIDGET_H
#define NMFPROGRESSWIDGET_H

#include <QtCharts/QChartView>
#include <QtCharts/QChart>

#include <QAbstractSeries>
#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QLineSeries>
#include <QObject>
#include <QPushButton>
#include <QMessageBox>
#include <QScatterSeries>
#include <QSlider>
#include <QSpinBox>
#include <QSpacerItem>
#include <QTimer>
#include <QValueAxis>

#include "nmfUtils.h"
#include "nmfUtilsQt.h"
#include "nmfLogger.h"


QT_CHARTS_USE_NAMESPACE

class nmfProgressWidget : public QObject
{

    Q_OBJECT

private:
    nmfLogger   *logger;
    QChartView  *m_chartView;
    QChart      *m_chart;

    void adjustYAxisLabelPrecision(QValueAxis *yAxis, double yMin, double yMax);

public:
    QGroupBox   *controlsGB;
    QHBoxLayout *hMainLayt;
    QVBoxLayout *vChartLayt;
    QHBoxLayout *hMinLayt;
    QHBoxLayout *hMaxLayt;
    QHBoxLayout *hRangeLayt;
    QHBoxLayout *yRangeLayt;
    QHBoxLayout *statusLayt;
    QVBoxLayout *vGroupLayt;
    QHBoxLayout *buttonLayt;
    QHBoxLayout *timeLayt;
    QHBoxLayout *pointsLayt;
    QLabel      *statusLBL;
    QLabel      *timeLBL;
    QLabel      *pointLBL;
    QLabel      *rangeLBL;
    QLabel      *minLBL;
    QLabel      *maxLBL;
    QLabel      *yAxisLBL;
    QLabel      *runStatusLBL;
    QCheckBox   *markersCB;
    QCheckBox   *labelsCB;
    QCheckBox   *validPointsCB;
    QDoubleSpinBox *yMinSB;
    QDoubleSpinBox *yMaxSB;
    QSpinBox    *minSB;
    QSpinBox    *maxSB;
    QPushButton *clearPB;
    QPushButton *stopPB;
    QPushButton *rangeSetPB;
    QLineEdit   *timeLE;
    QLineEdit   *runLE;
    QLineEdit   *subRunLE;

    QTimer*     m_timer;
//    std::chrono::_V2::system_clock::time_point m_startTime;
    int         m_maxNumGenerations;
    QString     m_mainTitle;
    QString     m_xTitle;
    QString     m_yTitle;
    double      m_xMin;
    double      m_xMax;
    double      m_yMin;
    double      m_yMax;
    double      m_xInc;
    double      m_yInc;
    bool        m_yAxisAdjusted;
    int         m_lastX;
    std::string m_RunType;
    std::string m_elapsedTime;
    bool        m_wasStopped;

    nmfProgressWidget(QTimer *theTimer,
                      nmfLogger *logger,
                      std::string runType,
                      QString Title,
                      QString XAxisLabel,
                      QString YAxisLabel,
                      double xMin, double xMax, double xInc,
                      double yMin, double yMax, double yInc);
   ~nmfProgressWidget();

    void setupConnections();
    void StopRun();
    void startRun();
    bool isStopped();
    void startTimer(int delayMS);
    void stopTimer();
    void updateChart();
    void readChartDataFile(std::string type,
                           std::string inputFileName,
                           std::string inputLabelFileName,
                           bool validPointsOnly);
    void updateChartDataLabel(std::string inputLabelFileName,
                              std::string overrideMsg);
    void setMaxNumGenerations(const int& newMaxNumGenerations);
    void setMainTitle(QString mainTitle);
    void setXRange(const double& xMin,const double& xMax);
    void setYRange(const double& yMin,const double& yMax);
    void setXTitle(const QString title);
    void setYTitle(const QString title);
    void setYAxisTitleScale(const QString title,
                            const double  yMin,
                            const double  yMax,
                            const double  yInc);
    void setXAxisTitleScale(const QString& title,
                            const double&  xMin,
                            const double&  xMax,
                            const double&  xInc);
    void setRunBoxes(const int& run,
                     const int& subRun,
                     const int& numSubRuns);
    QString getMainTitle();
    QString getXTitle();
    QString getYTitle();
    int getNumXTicks();
    int getNumYTicks();
    std::pair<double,double> getXRange();
    std::pair<double,double> getYRange();
    void setXInc(const double& xInc);
    void setYInc(const double& yInc);
    double getXInc();
    double getYInc();
    void setWhatsThis(QString whatsThis);
    void clearChart();
    void clearChartOnly();
    void clearRunBoxes();
    void shouldYRangeEditsStick(bool state);
    bool ifYRangeEditsShouldStick();
    bool readValidPointsOnly();
    void hideLegend();
    void showLegend();
    void updateTime();
    void clearTime();
    std::string getElapsedTime();
    bool wasStopped();
    void clearChartData(std::string filename);

signals:
    void StopTheRun();
    void StopTheTimer();
    void RedrawValidPointsOnly(bool checked, bool clear);

public Q_SLOTS:
    void callback_stopPB(bool unused);
    void callback_clearPB();
    void callback_labelsCB(int state);
    void callback_markersCB(int state);
    void callback_minSB(int value);
    void callback_maxSB(int value);
    void callback_lineHovered(QPointF point, bool state);
    void callback_scatterSeriesHovered(QPointF point, bool state);
    void callback_yMinSB(double value);
    void callback_yMaxSB(double value);
    void callback_rangeSetPB();
    void callback_validPointsCB(bool checked);
};


#endif // NMFPROGRESSWIDGET_H
