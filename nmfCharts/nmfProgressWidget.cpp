
//============================================================================
// Name        : nmfProgressWidget.cpp
// Author      : Ron Klasky
// Version     :
// Copyright   : NOAA - National Marine Fisheries Service
// Description : Widget containing logic for Progress Dock Widget
//============================================================================

#include "nmfProgressWidget.h"
#include "nmfConstants.h"
#include "nmfConstantsMSSPM.h"

#include <QtCharts/QChartView>
#include <QtCharts/QChart>

QT_CHARTS_USE_NAMESPACE

nmfProgressWidget::nmfProgressWidget(QTimer *theTimer,
                                     nmfLogger *theLogger,
                                     std::string runType,
                                     QString mainTitle,
                                     QString xTitle,
                                     QString yTitle,
                                     double xMin,
                                     double xMax,
                                     double xInc,
                                     double yMin,
                                     double yMax,
                                     double yInc)
{
    QString whatsThis;
    std::pair<double,double> xRange;
    std::pair<double,double> yRange;
    m_chart     = nullptr;
    m_chartView = nullptr;
    m_xInc = (xMax-xMin)/10;
    m_yInc = (yMax-yMin)/5;
    m_elapsedTime.clear();
    m_wasStopped = false;

    logger    = theLogger;
    m_timer   = theTimer;
    m_RunType = runType;
    m_lastX   = 0;

    // Create layouts and widgets
    hMainLayt   = new QHBoxLayout();
    vChartLayt  = new QVBoxLayout();
    hMinLayt    = new QHBoxLayout();
    hMaxLayt    = new QHBoxLayout();
    hRangeLayt  = new QHBoxLayout();
    yRangeLayt  = new QHBoxLayout();
    statusLayt  = new QHBoxLayout();
    vGroupLayt  = new QVBoxLayout();
    buttonLayt  = new QHBoxLayout();
    timeLayt    = new QHBoxLayout();
    pointsLayt  = new QHBoxLayout();
    m_chartView = new QChartView();
    controlsGB  = new QGroupBox();
    statusLBL   = new QLabel();
    timeLBL     = new QLabel("Elapsed Time:");
    pointLBL    = new QLabel("Points:");
    markersCB   = new QCheckBox("Markers");
    labelsCB    = new QCheckBox("Labels");
    validPointsCB = new QCheckBox("");
    yAxisLBL    = new QLabel("Y-Axis Range (Min, Max):");
    yMinSB      = new QDoubleSpinBox();
    yMaxSB      = new QDoubleSpinBox();
    rangeSetPB  = new QPushButton("[↕]");
    runStatusLBL= new QLabel("Completed (Run, SubRun):");
    timeLE      = new QLineEdit();
    runLE       = new QLineEdit();
    subRunLE    = new QLineEdit();
    rangeLBL    = new QLabel("Iterations (X) Axis:");
    minLBL      = new QLabel("Min:");
    maxLBL      = new QLabel("Max:");
    minSB       = new QSpinBox();
    maxSB       = new QSpinBox();
    clearPB     = new QPushButton("Clear Chart");
    stopPB      = new QPushButton("Stop");

    // Setup widgets
    timeLBL->setFixedWidth(100);
    timeLE->setFixedWidth(80);
    timeLE->setReadOnly(true);
    pointLBL->setFixedWidth(100);
    m_yAxisAdjusted = false;
    yMinSB->setDecimals(2);
    yMaxSB->setDecimals(2);
    yMinSB->setToolTip("Sets the minimum y-axis value.");
    yMinSB->setStatusTip("Sets the minimum y-axis value.");
    yMaxSB->setToolTip("Sets the maximum y-axis value.");
    yMaxSB->setStatusTip("Sets the maximum y-axis value.");
    yMinSB->setValue(m_yMin);
    yMinSB->setSingleStep(m_yInc);
    yMaxSB->setValue(m_yMax);
    yMaxSB->setSingleStep(m_yInc);
    yMaxSB->setMaximum(100000);
    yMinSB->setMaximum(100000);
    rangeSetPB->setToolTip("Automatically set the min,max range");
    rangeSetPB->setFixedSize(20,20);
    runLE->setReadOnly(true);
    subRunLE->setReadOnly(true);
    runLE->clear();
    subRunLE->clear();
    timeLBL->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    timeLE->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    runLE->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    subRunLE->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    // Add widgets to layouts
    yRangeLayt->addWidget(yMinSB);
    yRangeLayt->addWidget(yMaxSB);
    yRangeLayt->addWidget(rangeSetPB);
    yRangeLayt->addWidget(validPointsCB);
    statusLayt->addWidget(runLE);
    statusLayt->addWidget(subRunLE);
    vChartLayt->addWidget(m_chartView);
    vChartLayt->addWidget(statusLBL);
    hMainLayt->addLayout(vChartLayt);
    hMainLayt->addWidget(controlsGB);
    hRangeLayt->addWidget(minLBL);
    hRangeLayt->addWidget(minSB);
    hRangeLayt->addWidget(maxLBL);
    hRangeLayt->addWidget(maxSB);
    buttonLayt->addWidget(clearPB);
    buttonLayt->addWidget(stopPB);
    timeLayt->addWidget(timeLBL);
    timeLayt->addWidget(timeLE);
    timeLayt->addSpacerItem(new QSpacerItem(95,1,QSizePolicy::Fixed,QSizePolicy::Fixed));
    pointsLayt->addWidget(pointLBL);
    pointsLayt->addWidget(markersCB);
    pointsLayt->addWidget(labelsCB);
    vGroupLayt->addLayout(timeLayt);
    vGroupLayt->addLayout(pointsLayt);
    vGroupLayt->addWidget(yAxisLBL);
    vGroupLayt->addLayout(yRangeLayt);
    vGroupLayt->addWidget(runStatusLBL);
    vGroupLayt->addLayout(statusLayt);
    vGroupLayt->addSpacerItem(new QSpacerItem(1,2,QSizePolicy::Minimum,QSizePolicy::Expanding));
    vGroupLayt->addLayout(buttonLayt);

    // Get the chart as we'll need it later
    m_chart = m_chartView->chart();

    // Set defaults
    controlsGB->setLayout(vGroupLayt);
    controlsGB->setTitle("Controls");
    minSB->setSingleStep(10);
    maxSB->setSingleStep(10);
    minSB->setValue(0);
    maxSB->setValue(20);

//    m_chart->legend()->hide();
//    m_chart->legend()->show();
    m_chart->legend()->setAlignment(Qt::AlignRight);

    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chartView->setFrameShape(QFrame::Box);
    m_chart->setContentsMargins(-8,-8,-3,-3); // left, top, right, bottom
    pointLBL->setToolTip("Toggles the point labels which represent (iteration number, convergence value)");
    pointLBL->setStatusTip("Toggles the point labels which represent (iteration number, convergence value)");
    labelsCB->setToolTip("Toggles the point labels which represent (iteration number, convergence value)");
    labelsCB->setStatusTip("Toggles the point labels which represent (iteration number, convergence value)");
    markersCB->setToolTip("Toggles the point markers (i.e., filled circles)");
    markersCB->setStatusTip("Toggles the point markers (i.e., filled circles)");
    minSB->setToolTip("This is the minimum iteration value on the X axis.");
    minSB->setStatusTip("This is the minimum iteration value on the X axis.");
    minLBL->setToolTip("This is the minimum iteration value on the X axis.");
    minLBL->setStatusTip("This is the minimum iteration value on the X axis.");
    maxSB->setToolTip("This is the maximum iteration value on the X axis.");
    maxSB->setStatusTip("This is the maximum iteration value on the X axis.");
    maxLBL->setToolTip("This is the maximum iteration value on the X axis.");
    maxLBL->setStatusTip("This is the maximum iteration value on the X axis.");
    rangeLBL->setToolTip("Sets the Min/Max visible Iterations range for the chart.");
    rangeLBL->setStatusTip("Sets the Min/Max visible Iterations range for the chart.");
    stopPB->setToolTip("Stop the current model run.");
    stopPB->setStatusTip("Stop the current model run.");
    clearPB->setToolTip("Clears the chart of all previous plots.");
    clearPB->setStatusTip("Clears the chart of all previous plots.");
    validPointsCB->setToolTip("Show valid points only (i.e., omit 99999)");
    validPointsCB->setStatusTip("Show valid points only (i.e., omit 99999)");
    timeLBL->setToolTip("The current run's elapsed time in hh:mm:ss format");
    timeLBL->setStatusTip("The current run's elapsed time in hh:mm:ss format");
    timeLE->setToolTip("The current run's elapsed time in hh:mm:ss format");
    timeLE->setStatusTip("The current run's elapsed time in hh:mm:ss format");

    // Show empty grid
    QValueAxis  *newXAxis  = new QValueAxis();
    QValueAxis  *newYAxis  = new QValueAxis();
//  m_chart->setAxisX(newXAxis, new QLineSeries());
//  m_chart->setAxisY(newYAxis, new QLineSeries());
    nmfUtilsQt::setAxisX(m_chart,newXAxis,new QLineSeries());
    nmfUtilsQt::setAxisY(m_chart,newYAxis,new QLineSeries());

    xRange = getXRange();
    yRange = getYRange();
    m_chart->axes(Qt::Horizontal).back()->setRange(xRange.first,xRange.second);
    m_chart->axes(Qt::Vertical).back()->setRange(yRange.first,yRange.second);
//    newXAxis->applyNiceNumbers();
//    newYAxis->applyNiceNumbers();
    newXAxis->setLabelFormat("%d");
    adjustYAxisLabelPrecision(newYAxis, yRange.first, yRange.second);
//    if (yRange.second-yRange.first > 0.3)
//        newYAxis->setLabelFormat("%0.1f");
//    else
//        newYAxis->setLabelFormat("%0.2f");
    newXAxis->setTitleText(getXTitle());
    newYAxis->setTitleText(getYTitle());
    if (m_RunType == "MSSPM") {
        whatsThis = "<strong><center>Progress Chart</center></strong><p>This chart plots the Fitness Convergence Value ";
        whatsThis += "per Generation as calculated by the Genetic Algorithm. This value is equal to:</p> ";
        whatsThis += "<p>log [ (convergenceNumber - numGenerationsSinceBestFit) *<br>";
        whatsThis += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;bestOverallFitness ]</p>";
        whatsThis += "Once the difference is 0 the model is said to converge. The model will also end if ";
        whatsThis += "maxGenerations is reached.</p>";
    } else if (m_RunType == "MSVPA") {
        whatsThis = "<strong><center>Progress Chart</center></strong><p>This chart plots the convergence value per loop ";
        whatsThis += "iteration. Once the convergence value is less than 3, ";
        whatsThis += "the model is said to converge. ";
        whatsThis += "The convergence value is the number of differences between ";
        whatsThis += "the M2 matrix at iteration (i) and (i+1).</p>";
    } else if (m_RunType == "Forecast") {
        whatsThis = "<strong><center>Progress Chart</center></strong><p>This chart plots the Total Biomass per ";
        whatsThis += "Forecast year of the specified species.</p>";
    }

    m_chartView->setWhatsThis(whatsThis);
    m_chart->removeAllSeries();
    setXInc(xInc);
    setXRange(xMin,xMax);
    setYRange(yMin,yMax);
    setMaxNumGenerations(xMax);
    setXTitle(xTitle);
    setYTitle(yTitle);
    setMainTitle(mainTitle);
    setXRange(0,m_maxNumGenerations);
    setYRange(0,200);

//    newXAxis->setTickCount(getNumXTicks()+1);
//    newYAxis->setTickCount(getNumYTicks());
}

nmfProgressWidget::~nmfProgressWidget() {

}

void
nmfProgressWidget::showLegend()
{
    m_chart->legend()->show();
}

void
nmfProgressWidget::hideLegend()
{
    m_chart->legend()->hide();
}

void
nmfProgressWidget::setWhatsThis(QString whatsThis)
{
    m_chartView->setWhatsThis(whatsThis);
}

void
nmfProgressWidget::adjustYAxisLabelPrecision(QValueAxis *yAxis,
                                             double yMin, double yMax)
{
//    if (yMax-yMin <=  0.31) {
//        yAxis->setLabelFormat("%0.2f");
//    } else {
        yAxis->setLabelFormat("%0.2f");
//    }
}

int
nmfProgressWidget::getNumXTicks()
{
    return (m_xMax-m_xMin)/m_xInc;
}

int
nmfProgressWidget::getNumYTicks()
{
    return (m_yMax-m_yMin)/m_yInc;
}

QString
nmfProgressWidget::getMainTitle()
{
    return "<strong>"+m_mainTitle+"</strong>";
}

QString
nmfProgressWidget::getXTitle()
{
    return m_xTitle;
}

QString
nmfProgressWidget::getYTitle()
{
    return m_yTitle;
}

double
nmfProgressWidget::getXInc()
{
    return m_xInc;
}


double
nmfProgressWidget::getYInc()
{
    return m_yInc;
}


void
nmfProgressWidget::setXInc(const double& xInc)
{
    m_xInc = xInc;
    updateChart();
}

void
nmfProgressWidget::clearRunBoxes()
{
    runLE->setText("");
    subRunLE->setText("");
}

void
nmfProgressWidget::setRunBoxes(const int& run,
                               const int& subRun,
                               const int& numSubRuns)
{
    runLE->setText(QString::number(run));
    subRunLE->setText(QString::number(subRun) + " of " +
                      QString::number(numSubRuns));
}

void
nmfProgressWidget::setYAxisTitleScale(const QString title, const double yMin,
                                      const double yMax, const double yInc)
{
    m_yTitle = title;
    if (m_yAxisAdjusted) {
        m_yMin = yMinSB->value();
        m_yMax = yMaxSB->value();
        m_yInc = yMaxSB->singleStep();
    } else {
        m_yMin = yMin;
        m_yMax = yMax;
        m_yInc = yInc;
    }

    updateChart();
}

void
nmfProgressWidget::shouldYRangeEditsStick(bool state)
{
    m_yAxisAdjusted = state;
}

bool
nmfProgressWidget::ifYRangeEditsShouldStick()
{
    return m_yAxisAdjusted;
}

void
nmfProgressWidget::setXAxisTitleScale(const QString& title, const double& xMin,
                                      const double& xMax, const double& xInc)
{
    m_xTitle = title;
    m_xMin   = xMin;
    m_xMax   = xMax;
    m_xInc   = xInc;
    m_maxNumGenerations = xMax;
    updateChart();
}

void
nmfProgressWidget::setXRange(const double &xMin, const double &xMax)
{
    m_xMin = xMin;
    m_xMax = xMax;
    updateChart();
}

void
nmfProgressWidget::setYRange(const double &yMin, const double &yMax)
{
    if (m_yAxisAdjusted)
        return;

    m_yMin = yMin;
    m_yMax = yMax;
    updateChart();
}


void
nmfProgressWidget::setYInc(const double& yInc)
{
    if (m_yAxisAdjusted)
        return;

    m_yInc = yInc;
    updateChart();
}

std::pair<double,double>
nmfProgressWidget::getXRange()
{
    return std::make_pair(m_xMin,m_xMax);
}


std::pair<double,double>
nmfProgressWidget::getYRange()
{
    return std::make_pair(m_yMin,m_yMax);
}


void
nmfProgressWidget::setMainTitle(QString mainTitle)
{
    m_mainTitle = "<strong>" + mainTitle + "</strong>";
    updateChart();
}

void
nmfProgressWidget::setXTitle(const QString title)
{
    m_xTitle = title;
    updateChart();
}

void
nmfProgressWidget::setYTitle(const QString title)
{
    m_yTitle = title;
    updateChart();
}

void
nmfProgressWidget::setMaxNumGenerations(const int& MaxNumGenerations)
{
    m_maxNumGenerations = MaxNumGenerations;
    updateChart();
}

void
nmfProgressWidget::updateTime()
{
//    timeLE->setText(QString::fromStdString(nmfUtils::elapsedTimeCondensed(m_startTime)));
}

void
nmfProgressWidget::clearTime()
{
    timeLE->setText("");
}

void
nmfProgressWidget::updateChart()
{
    if (m_chart != nullptr) {

        // Update Controls;
        yMinSB->setValue(m_yMin);
        yMinSB->setSingleStep(m_yInc);
        yMaxSB->setValue(m_yMax);
        yMaxSB->setSingleStep(m_yInc);

        // Update Chart
        m_chart->setTitle(m_mainTitle);
//      m_chart->axes(Qt::Horizontal).back()->setRange(m_xMin, m_xMax); // Caused x-axis to rescale...not what's wanted
        m_chart->axes(Qt::Horizontal).back()->setTitleText(m_xTitle);
        m_chart->axes(Qt::Vertical).back()->setRange(m_yMin, m_yMax);
        m_chart->axes(Qt::Vertical).back()->setTitleText(m_yTitle);
        adjustYAxisLabelPrecision(qobject_cast<QValueAxis*>(m_chart->axes(Qt::Vertical).back()),
                                  m_yMin, m_yMax);
        m_chart->update();

    }
}

void
nmfProgressWidget::setupConnections() {

    stopPB->disconnect();
    clearPB->disconnect();
    labelsCB->disconnect();
    markersCB->disconnect();
    minSB->disconnect();
    maxSB->disconnect();
    rangeSetPB->disconnect();
    validPointsCB->disconnect();

//    connect(stopPB,     SIGNAL(clicked(bool)),        this, SLOT(callback_stopPB(bool)));
//connect(stopPB, &QPushButton::clicked, this, &nmfProgressWidget::callback_stopPB);
      connect(stopPB,     SIGNAL(clicked()), SLOT(close()));
    connect(clearPB,    SIGNAL(clicked()),            this, SLOT(callback_clearPB()));
    connect(labelsCB,   SIGNAL(stateChanged(int)),    this, SLOT(callback_labelsCB(int)));
    connect(markersCB,  SIGNAL(stateChanged(int)),    this, SLOT(callback_markersCB(int)));
    connect(minSB,      SIGNAL(valueChanged(int)),    this, SLOT(callback_minSB(int)));
    connect(maxSB,      SIGNAL(valueChanged(int)),    this, SLOT(callback_maxSB(int)));
    connect(yMinSB,     SIGNAL(valueChanged(double)), this, SLOT(callback_yMinSB(double)));
    connect(yMaxSB,     SIGNAL(valueChanged(double)), this, SLOT(callback_yMaxSB(double)));
    connect(rangeSetPB, SIGNAL(clicked()),            this, SLOT(callback_rangeSetPB()));

    connect(validPointsCB, SIGNAL(clicked(bool)),     this, SLOT(callback_validPointsCB(bool)));

} // end SetupConnections

void
nmfProgressWidget::stopTimer()
{
    m_timer->stop();
}

void
nmfProgressWidget::startTimer(int delayMillisec)
{
    logger->logMsg(nmfConstants::Normal,"Start " + m_RunType + " Progress Chart Timer");

    // Start Progress Chart's timer here
    m_timer->start(delayMillisec);
    startRun();

    clearTime();

}

bool
nmfProgressWidget::wasStopped()
{
    return m_wasStopped;
}

void
nmfProgressWidget::callback_stopPB(bool unused)
{
    logger->logMsg(nmfConstants::Normal,"Stop " + m_RunType + " Progress Chart Timer");

    emit StopTheRun();
    StopRun();
    m_wasStopped = true;

    if (m_RunType == "MSSPM") {
        updateChartDataLabel(nmfConstantsMSSPM::MSSPMProgressChartLabelFile,
                             "<b>Status:&nbsp;&nbsp;</b>User halted MSSPM run. Output data incomplete.");
    } else if (m_RunType == "MSVPA") {
        updateChartDataLabel(nmfConstantsMSVPA::MSVPAProgressChartLabelFile,
                             "<b>Status:&nbsp;&nbsp;</b>User halted MSVPA run. Output data incomplete.");
    } else if (m_RunType == "Forecast") {
        updateChartDataLabel(nmfConstantsMSVPA::ForecastProgressChartLabelFile,
                             "<b>Status:&nbsp;&nbsp;</b>User halted Forecast run. Output data incomplete.");
    }

} // end callback_stopPB

void
nmfProgressWidget::clearChart()
{
    callback_clearPB();
}

void
nmfProgressWidget::clearChartOnly()
{
    m_chart->removeAllSeries();
}

void
nmfProgressWidget::clearChartData(std::string filename)
{
    std::ofstream outputFileMSSPM(filename);
    outputFileMSSPM.close();
}

void
nmfProgressWidget::callback_clearPB() {

    m_chart->removeAllSeries();
    clearRunBoxes();

    // Initialize progress output file
    if (m_RunType == "MSSPM") {
        std::ofstream outputFile(nmfConstantsMSSPM::MSSPMProgressChartFile);
        outputFile.close();
    } else if (m_RunType == "MSVPA") {
        std::ofstream outputFile(nmfConstantsMSVPA::MSVPAProgressChartFile);
        outputFile.close();
    } else if (m_RunType == "Forecast") {
        std::ofstream outputFile(nmfConstantsMSVPA::ForecastProgressChartFile);
        outputFile.close();
    }

} // end callback_clearPB

void
nmfProgressWidget::callback_labelsCB(int state)
{
//    if (isStopped()) {
        QLineSeries *lineSeries;
        QList<QAbstractSeries *> allSeries = m_chart->series();
        for (int i=0; i<allSeries.count(); ++i) {
            lineSeries = qobject_cast<QLineSeries *>(allSeries[i]);
            lineSeries->setPointLabelsVisible(state == Qt::Checked);
        }
        m_chartView->update();
        m_chartView->repaint();
        m_chart->update();
//    }
} // end callback_labelsCB


void
nmfProgressWidget::callback_markersCB(int state)
{
//    if (isStopped()) {
        QLineSeries *lineSeries;
        QList<QAbstractSeries *> allSeries = m_chart->series();
        for (int i=0; i<allSeries.count(); ++i) {
            lineSeries = qobject_cast<QLineSeries *>(allSeries[i]);
            lineSeries->setPointsVisible(state == Qt::Checked);
        }
        m_chartView->update();
        m_chartView->repaint();
        m_chart->update();
//    }
} // end callback_markersCB


void
nmfProgressWidget::callback_minSB(int value) {

    if (isStopped()) {
        m_chart->axes(Qt::Horizontal).back()->setMin(value);
        m_chart->update();
    }

} // end callback_minSB

void
nmfProgressWidget::callback_maxSB(int value)
{
    if (isStopped()) {
        m_chart->axes(Qt::Horizontal).back()->setMax(value);
        m_chart->update();
    }

} // end callback_maxSB

void
nmfProgressWidget::callback_yMinSB(double value)
{
    if (yMinSB->value() >= yMaxSB->value()) {
        m_yMin = yMaxSB->value() - getYInc();
    } else {
        m_yMin = value;
    }

    updateChart();
}

void
nmfProgressWidget::callback_yMaxSB(double value)
{
    if (yMaxSB->value() <= yMinSB->value()) {
        m_yMax = yMinSB->value() + getYInc();
        yMaxSB->blockSignals(true);
        yMaxSB->setValue(m_yMax);
        yMaxSB->blockSignals(false);
    } else {
        m_yMax = value;
    }
    updateChart();
}

void
nmfProgressWidget::callback_lineHovered(QPointF point, bool state)
{
    QString msg;
    if (state) {
        msg = QString::number(point.x()) + "," +
                QString::number(point.y());
        m_chartView->setToolTip(msg);
    }
} // end callback_lineHovered

void
nmfProgressWidget::callback_rangeSetPB()
{
    QList<QAbstractSeries *> allSeries = m_chart->series();
    QLineSeries *lineSeries = nullptr;
    double value;
    double min =  99999;
    double max = -99999;

    for (int i=0; i<allSeries.count(); ++i) {
        lineSeries = qobject_cast<QLineSeries *>(allSeries[i]);
        if (lineSeries) {
            for (int j=0; j<lineSeries->count(); ++j) {
                value = lineSeries->at(j).y();
                min = (value < min) ? value : min;
                max = (value > max) ? value : max;
            }
        }
    }

    callback_yMaxSB(max);
    callback_yMinSB(min);
}

void
nmfProgressWidget::callback_validPointsCB(bool checked)
{
    callback_rangeSetPB();

    emit RedrawValidPointsOnly(checked,true);
}

bool
nmfProgressWidget::readValidPointsOnly()
{
    return validPointsCB->isChecked();
}

void
nmfProgressWidget::callback_scatterSeriesHovered(QPointF point,bool state)
{
    if (state) {
        QString newToolTip = QString::number(point.x()) + "," + QString::number(point.y());
        m_chart->setToolTip(newToolTip);
        //std::cout << "POINT: " << point.x() << ", " << point.y() << std::endl;
    } else {
        m_chart->setToolTip("");
    }

} // end callback_scatterSeriesHovered

std::string
nmfProgressWidget::getElapsedTime()
{
    return m_elapsedTime;
}

void
nmfProgressWidget::StopRun()
{
//    m_elapsedTime = nmfUtils::elapsedTime(m_startTime);
    emit StopTheTimer();

    if (m_RunType == "MSSPM") {
        std::ofstream outputFile(nmfConstantsMSSPM::MSSPMStopRunFile);
        outputFile << "StoppedByUser" << std::endl;
        outputFile << "unused" << std::endl;
        outputFile << m_elapsedTime << std::endl;
        outputFile << m_lastX << std::endl;
        outputFile.close();
   } else if (m_RunType == "MSVPA") {
        std::ofstream outputFile(nmfConstantsMSVPA::MSVPAStopRunFile);
        outputFile << "Stop" << std::endl;
        outputFile.close();
    } else if (m_RunType == "Forecast") {
        std::ofstream outputFile(nmfConstantsMSVPA::ForecastStopRunFile);
        outputFile << "Stop" << std::endl;
        outputFile.close();
    }

    logger->logMsg(nmfConstants::Bold,m_RunType + " Run - End");
    logger->logMsg(nmfConstants::Section,"================================================================================");


} // end StopRun

void
nmfProgressWidget::startRun()
{
//    m_startTime  = nmfUtils::startTimer();
    m_wasStopped = false;

    if (m_RunType == "MSSPM") {
        std::ofstream outputFile(nmfConstantsMSSPM::MSSPMStopRunFile);
        outputFile << "Start" << std::endl;
        outputFile.close();
    } else if (m_RunType == "MSVPA") {
        std::ofstream outputFile(nmfConstantsMSVPA::MSVPAStopRunFile);
        outputFile << "Start" << std::endl;
        outputFile.close();
    } else {
        std::ofstream outputFile(nmfConstantsMSVPA::ForecastStopRunFile);
        outputFile << "Start" << std::endl;
        outputFile.close();
    }
} // end StartRun

bool
nmfProgressWidget::isStopped()
{
    std::string cmd="";

    if (m_RunType == "MSSPM") {
        std::ifstream inputFile(nmfConstantsMSSPM::MSSPMStopRunFile);
        if (inputFile) {
            std::getline(inputFile,cmd);
        }
        inputFile.close();
    } else if (m_RunType == "MSVPA") {
        std::ifstream inputFile(nmfConstantsMSVPA::MSVPAStopRunFile);
        if (inputFile) {
            std::getline(inputFile,cmd);
        }
        inputFile.close();
    } else if (m_RunType == "Forecast") {
        std::ifstream inputFile(nmfConstantsMSVPA::ForecastStopRunFile);
        if (inputFile) {
            std::getline(inputFile,cmd);
        }
        inputFile.close();
    }

    return (cmd == "Stop")          ||
           (cmd == "StoppedByUser") ||
           (cmd == "StopAllOK")     ||
           (cmd == "StopIncomplete");
} // end isStopped


void
nmfProgressWidget::updateChartDataLabel(std::string inputLabelFileName,
                                        std::string overrideMsg)
{
    // Update progress chart label
    QString msg;
    std::string line;

    std::ifstream inputLabelFile(inputLabelFileName);
    std::getline(inputLabelFile,line);
    if (line.empty()) {
        msg.clear();
    } else {
        msg = "<b>Status:&nbsp;&nbsp;</b>" + QString::fromStdString(line);
    }
    if (overrideMsg.empty())
        statusLBL->setText(msg);
    else
        statusLBL->setText(QString::fromStdString(overrideMsg));
    inputLabelFile.close();

} // end updateChartDataLabel


void
nmfProgressWidget::readChartDataFile(std::string type,
                                     std::string inputFileName,
                                     std::string inputLabelFileName,
                                     bool validPointsOnly)
{
//std::cout << "====> readChartDataFile: " << validPointsOnly << std::endl;
    // Read progress file that has all of the convergence values in
    // it and draw a line chart.
    int maxXRange;
    int minXRange;
    int numPlots;
    std::pair<double,double> xRange;
    std::pair<double,double> yRange;
    QString mainTitle;
    QValueAxis *newXAxis = NULL;
    QValueAxis *newYAxis = NULL;
    std::string line;
    std::vector<std::string> parts;
    QLineSeries    *lseries = NULL;
    QList<QColor> colors = {QColor(100,100,255),
                            QColor(255,100,100),
                            QColor(0,0,0),
                            Qt::lightGray,
                            Qt::darkGray};

    updateChartDataLabel(inputLabelFileName,"");

    // try to hide QLineSeries from legend
    // RSK use QPen dash, dot variations with color for various line types
    // Show legend to the right.
//    m_chart->legend()->show();
//    m_chart->legend()->setAlignment(Qt::AlignRight);

    // This is so that if the user specifies the exact same plot again, it'll
    // draw as a new plot and not as a continuation of the original plot. Why?
    // This would be desirable if a person first runs a Forecast with 5 years and
    // then runs the exact same Forecast with 10 years.
    m_lastX=0;

    int x;
    double y;
    std::string species="";
    std::string theRunName="",theLastRunName="";
    std::string theForecastName="";
    std::string theScenarioName="";
    std::ifstream inputFile(inputFileName);
    std::vector<std::vector<std::string> > chartData;
    std::vector<std::string> strVector;
    std::vector<std::string> validLines;
    bool isFirstLine = true;

    if (type == "MSSPM") { // Means we're looking at MSSPM data

        validLines.clear();
        // Run through file and check and remove 99999 lines
        while (std::getline(inputFile,line)) {
            if (validPointsOnly) {
                boost::split(parts,line,boost::is_any_of(","));
                y = std::stod(parts[2]);
                if (y < 90000) {
                    validLines.push_back(line);
                }
            } else {
                validLines.push_back(line);
            }
        }

        // Load the validLine lines into chartData
        if (validLines.size() > 0) {
            strVector.clear();
            boost::split(parts,validLines[0],boost::is_any_of(","));
            theRunName = parts[0];
            strVector.push_back(validLines[0]);
            for (int i=1; i<int(validLines.size()); ++i) {
                boost::split(parts,validLines[i],boost::is_any_of(","));
                if (theRunName == parts[0]) {
                    strVector.push_back(validLines[i]);
                } else {
                    chartData.push_back(strVector);
                    strVector.clear();
                    strVector.push_back(validLines[i]);
                    theRunName = parts[0];
                }
            }
            chartData.push_back(strVector);
        }

        m_chart->removeAllSeries();
        for (int i=0; i<int(chartData.size()); ++i) {
            lseries = new QLineSeries();
            strVector = chartData[i];
            for (int j=0; j<int(strVector.size()); ++j) {
                boost::split(parts,strVector[j],boost::is_any_of(","));
                lseries->setName(QString::fromStdString(parts[0]));
                lseries->append(std::stoi(parts[1]),std::stod(parts[2]));
            }
            m_chart->addSeries(lseries);
        }
        m_chart->createDefaultAxes();

        m_chart->update(); // necessary for labels to paint completely
    }


    while (std::getline(inputFile,line)) {
        boost::split(parts,line,boost::is_any_of(","));

//        if (type == "MSVPA") { // Means we're looking at MSVPA data
//            theRunName = parts[0];
//            x = std::stoi(parts[1]);
//            //y = std::stoi(parts[2]);
//            if (isFirstLine || ((theRunName == theLastRunName) && (m_lastX < x))) {
//                //pointVector.push_back(QPoint(x,y));
//                strVector.push_back(line);
//            } else {
//                chartData.push_back(strVector);
//                strVector.clear();
//                strVector.push_back(line);
//            }
//            theLastRunName = theRunName;
//            m_lastX = x;
//            isFirstLine = false;
//        }
//        else

        if (type == "Forecast") { // Means we're looking at Forecast data
            theRunName    = parts[0];
            theForecastName = parts[1];
            theScenarioName = parts[2];
            x = std::stoi(parts[3]);
            //y = std::stoi(parts[5]);
            species = parts[4];
            if (isFirstLine || ((theRunName == theLastRunName) && (m_lastX < x))) {
                strVector.push_back(line);
            } else {
                chartData.push_back(strVector);
                strVector.clear();
                strVector.push_back(line);
            }
            theLastRunName = theRunName;
            m_lastX = x;
            isFirstLine = false;
        }
    }

    if (type != "MSSPM") {
        if (strVector.size() > 0) {
            chartData.push_back(strVector);
        }
    }
    numPlots = chartData.size();
    if (numPlots == 0)
        return;

    // Add points to series
    int numExistingPoints;
    int numPointsRead;
    QList<QAbstractSeries *> allSeries = m_chart->series();
//std::cout << "num allSeries: " << allSeries.size() << std::endl;

    if (type != "MSSPM") {

        if (allSeries.count() == 0) {
            lseries = new QLineSeries();
            // RSK Number aren't correct when they appear.  Could be a bug in 5.8, try it in 5.10
            //         connect(lseries, SIGNAL(hovered(QPointF,bool)),
            //                 this, SLOT(callback_lineHovered(QPointF,bool)));
            strVector = chartData[0];
            boost::split(parts,strVector[0],boost::is_any_of(","));
            lseries->setName(QString::fromStdString(parts[0]));
            if (type == "MSSPM") {
                lseries->append(std::stoi(parts[1]),std::stod(parts[2]));
            } else if (type == "MSVPA") {
                lseries->append(std::stoi(parts[1]),std::stoi(parts[2]));
            } else if (type == "Forecast") {
                lseries->append(std::stoi(parts[3]),std::stoi(parts[5]));
                species = parts[4];
            }
            m_chart->addSeries(lseries);
            m_chart->createDefaultAxes();
        } else if (allSeries.count() < numPlots) {
            lseries = new QLineSeries();
            //        connect(lseries, SIGNAL(hovered(QPointF,bool)),
            //                this, SLOT(callback_lineHovered(QPointF,bool)));
            strVector = chartData[allSeries.count()];
            boost::split(parts,strVector[0],boost::is_any_of(","));
            lseries->setName(QString::fromStdString(parts[0]));
            if (type == "MSSPM") {
                lseries->append(std::stoi(parts[1]),std::stod(parts[2]));
            } else if (type == "MSVPA")
                lseries->append(std::stoi(parts[1]),std::stoi(parts[2]));
            else if (type == "Forecast") {
                lseries->append(std::stoi(parts[3]),std::stoi(parts[5]));
                species = parts[4];
            }
            m_chart->addSeries(lseries);
            m_chart->createDefaultAxes();

        } else {
            lseries = qobject_cast<QLineSeries *>(allSeries[chartData.size()-1]);
            strVector = chartData[chartData.size()-1];
            numExistingPoints = lseries->count();
            numPointsRead     = strVector.size();
            if (numExistingPoints < numPointsRead) {
                for (int i=numExistingPoints; i<numPointsRead; ++i) {
                    boost::split(parts,strVector[i],boost::is_any_of(","));
                    lseries->setName(QString::fromStdString(parts[0]));
                    if (type == "MSSPM") {
                        lseries->append(std::stoi(parts[1]),std::stod(parts[2]));
                    } else if (type == "MSVPA")
                        lseries->append(std::stoi(parts[1]),std::stoi(parts[2]));
                    else if (type == "Forecast") {
                        lseries->append(std::stoi(parts[3]),std::stoi(parts[5]));
                        species = parts[4];
                    }
                }
            }
            // Necessary for the chart to update
            m_chart->update(); // necessary for labels to paint completely
        }
    }

    QLineSeries *lineSeries = nullptr;
    QColor whichColor;
    newXAxis = new QValueAxis();
    newYAxis = new QValueAxis();
    allSeries = m_chart->series();

    for (int i=0; i<allSeries.size(); ++i) {
        lineSeries = qobject_cast<QLineSeries *>(allSeries[i]);
        whichColor = colors[i % colors.count()];
        lineSeries->setPointsVisible(markersCB->isChecked());
        lineSeries->setPointLabelsVisible(labelsCB->isChecked());
        lineSeries->setPen(QPen(whichColor,2));
        lineSeries->setColor(whichColor);
//      m_chart->setAxisX(newXAxis, lineSeries);
//      m_chart->setAxisY(newYAxis, lineSeries);
        nmfUtilsQt::setAxisX(m_chart,newXAxis,lineSeries);
        nmfUtilsQt::setAxisY(m_chart,newYAxis,lineSeries);
    }

    // Find max y value in lineSeries
    int numPoints = lineSeries->count();
    double maxY = 0;
    double yValue;
    for (int i=0; i<numPoints; ++i) {
        yValue = lineSeries->at(i).y();
        if (yValue > maxY)
            maxY = yValue;
    }

    // Find max x value in lineSeries
    double maxX = 0;
    double xValue;
    for (int i=0; i<numPoints; ++i) {
        xValue = lineSeries->at(i).x();
        if (xValue > maxX)
            maxX = xValue;
    }

    // Set axis to be last set of series...should be OK I think...since
    // all series should have approximately the same ranges.
    int maxValue = maxSB->value();
//  newXAxis->setTickCount((m_xMax-m_xMin)/m_xInc);
    maxXRange = int(int(lineSeries->count()/10.0))*10.0+10;
    maxXRange = (maxXRange <= maxValue) ? maxValue : maxXRange;
    maxSB->setValue(maxXRange);
    minXRange = minSB->value();
    if (minXRange >= maxXRange) {
        minXRange = maxXRange-20;
        minSB->setValue(minXRange);
    }

    xRange = getXRange();
    yRange = getYRange();

    maxX = (maxX > xRange.second) ? maxX : xRange.second;

    m_chart->axes(Qt::Horizontal).back()->setRange(xRange.first,maxX);
    m_chart->axes(Qt::Vertical).back()->setRange(yRange.first,yRange.second);
//  newXAxis->applyNiceNumbers();
    newXAxis->setLabelFormat("%d");
    newYAxis->setLabelFormat("%0.2f");
    newXAxis->setTitleText(getXTitle());
    newYAxis->setTitleText(getYTitle());
    mainTitle = getMainTitle();
    if (type == "Forecast")
        mainTitle += QString::fromStdString(species);
    m_chart->setTitle(mainTitle);


//  newXAxis->setTickCount(getNumXTicks()+1);
//  newYAxis->setTickCount(getNumYTicks());
    return;

} // end readChartDataFile


