
#include "nmfChartLine.h"
#include <nmfConstants.h>
#include <nmfConstantsMSSPM.h>


nmfChartLine::nmfChartLine()
{
    m_Tooltips.clear();

    // Create custom tool tip
    m_CustomToolTip = new nmfToolTip();
}

void
nmfChartLine::clear(QChart* chart)
{
    chart->removeAllSeries();
}

void
nmfChartLine::populateChart(
        QChart*            chart,
        std::string&       type,
        const std::string& style,
        const bool&        ShowFirstPoint,
        const bool&        ShowLegend,
        const double&      XOffset,
        const bool&        XAxisIsInteger,
        const double&      YMinSliderVal,
        const bool&        LeaveGapsWhereNegative,
        const boost::numeric::ublas::matrix<double> &YAxisData,
        const QStringList& RowLabels,
        const QStringList& ColumnLabels,
        const QStringList& HoverLabels,
        std::string&       MainTitle,
        std::string&       XTitle,
        std::string&       YTitle,
        const std::vector<bool>& GridLines,
        const int&         Theme,
        const QColor&      LineColor,
        const std::string& LineColorName,
        const double&      XInc)
{

    std::cout << "In Populate Chart function" << std::endl;
    //bool showLegend = (ColumnLabels.size() > 0);
    QLineSeries *series = nullptr;
    QPen pen;
    int XStartVal = (ShowFirstPoint) ? 0 : 1;
    XStartVal += XOffset;
    double yVal;
    QString lineColorName = QString::fromStdString(LineColorName);
    QString hoverLabel;
    int NumXValues = YAxisData.size1();

    // Set main title
    QFont mainTitleFont = chart->titleFont();
    mainTitleFont.setPointSize(14);
    mainTitleFont.setWeight(QFont::Bold);
    chart->setTitleFont(mainTitleFont);
    chart->setTitle(QString::fromStdString(MainTitle));

    chart->legend()->hide();

    // Set current theme
    chart->setTheme(static_cast<QChart::ChartTheme>(Theme));

    if (type == "FreeLine") { // A FreeLine doesn't use default integers for x axis
          series = new QLineSeries();

          // For now...just use allow line
          pen = series->pen();
          pen.setStyle(Qt::SolidLine);
          pen.setColor(LineColor); //LineColors[0]);
          pen.setWidth(2);
          series->setPen(pen);

        // Load Data
        for (unsigned i=0; i<YAxisData.size2(); ++i) {
                series->append(YAxisData(0,i),
                               YAxisData(1,i));
        }

        chart->addSeries(series);
        series->setPointsVisible();

    } else if (type == "Line") {

        // Load data into series and then add series to the chart
        for (unsigned line=0; line<YAxisData.size2(); ++line) {
            series = new QLineSeries();
            hoverLabel = (unsigned(HoverLabels.size()) <= line) ? "" : HoverLabels[line];
            if (hoverLabel.isEmpty()) {
                hoverLabel = "No Label Set";
            }

            m_HoverLabels[hoverLabel] = hoverLabel;

            // Using the hover help QString as the name of the series so that I can
            // find later on the hover help that corresponds to the series the user is
            // hovering over.
            series->setName(hoverLabel);

            pen = series->pen();
            if (lineColorName == "MonteCarloSimulation") {
                pen.setColor(LineColor);
            } else if (lineColorName == "No Uncertainty Variations") {
                 pen.setColor(Qt::black);
            } else {
                pen.setColor(QColor(QString::fromStdString(nmfConstants::LineColors[line%nmfConstants::LineColors.size()])));
            }
            pen.setWidth(2);
            if (style == "DottedLine") {
                pen.setStyle(Qt::DotLine);
            } else if (style == "DashedLine") {
                pen.setStyle(Qt::DashLine);
            } else {
                pen.setStyle(Qt::SolidLine);
            }
            series->setPen(pen);

            for (int j=XStartVal-XOffset; j<NumXValues; ++j) {
                yVal = YAxisData(j,line);
                if (yVal != nmfConstants::NoValueDouble) {
                    series->append(XOffset+j*XInc,yVal);
                }
            }

            chart->addSeries(series);

            if (ColumnLabels.size() > 0) {
                m_Tooltips[ColumnLabels[0]] = lineColorName;
            }
            disconnect(series,0,0,0);

            connect(series, SIGNAL(hovered(const QPointF&,bool)),
                    this,   SLOT(callback_hoveredLine(const QPointF&,bool)));
        }
    }

    // Setup X and Y axes
    chart->createDefaultAxes();
    QAbstractAxis *axisX = chart->axes(Qt::Horizontal).back();
    //QValueAxis* chartXaxis = new QValueAxis();
    QFont titleFont = axisX->titleFont();

    //QFont titleFont2 = chartXaxis->titleFont();

    titleFont.setPointSize(12);
    titleFont.setWeight(QFont::Bold);

//    titleFont2.setPointSize(12);
//    titleFont2.setWeight(QFont::Bold);

    QValueAxis *currentAxisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).back());
    QValueAxis* AxisY = new QValueAxis();
    currentAxisY->setTitleFont(titleFont);
    currentAxisY->setTitleText(QString::fromStdString(YTitle));
    currentAxisY->applyNiceNumbers();
    double currentYMin = currentAxisY->min();
    std::cout << "In Populate Chart function" << std::endl;
    std::cout << currentYMin << std::endl;
    if (YMinSliderVal >= 0) {
        currentAxisY->setMin(currentYMin*YMinSliderVal/100.0); // RSK - modify this
        AxisY->setMin(currentYMin*YMinSliderVal/100.0);
        AxisY->applyNiceNumbers();
        currentAxisY->applyNiceNumbers();
    }


    if (LeaveGapsWhereNegative) {
        double currentYMax = currentAxisY->max();
        std::cout << "The current YMax is: " << currentYMax << std::endl;
        // Replace any y = -1 found in series with 99999
        // Then restore the previous y max value
        // This should have the effect of omitting -1 values but allowing in range values
        // if they appear any time after the -1 values in the series
        QLineSeries* lineSeries;
        QPointF seriesPoint;
        for (QAbstractSeries* series : chart->series()) {
            lineSeries = qobject_cast<QLineSeries*>(series);
            for (int i=0; i<lineSeries->count(); ++i) {
                seriesPoint = lineSeries->at(i);
                if (seriesPoint.y() == nmfConstantsMSSPM::NoFishingMortality) {
                    lineSeries->replace(i,QPointF(seriesPoint.x(),nmfConstantsMSSPM::MaxOutOfBounds));
                }

            }
        }
        currentAxisY->setMax(currentYMax);
        AxisY->setMax(currentYMax);
    }


    QValueAxis *currentAxisX = qobject_cast<QValueAxis*>(chart->axes(Qt::Horizontal).back());
    currentAxisX->setTitleFont(titleFont);
    currentAxisX->setTitleText(QString::fromStdString(XTitle));
    currentAxisX->applyNiceNumbers();
    if (XAxisIsInteger) {
        currentAxisX->setLabelFormat("%d");
    }

    // Set range so plot completely fills out chart in the x-direction
    currentAxisX->setRange(XStartVal,XStartVal+(NumXValues-1)*XInc);

//    chart->removeAxis(chart->axes(Qt::Vertical).back());
//    chart->addAxis(AxisY, Qt::AlignBottom);
//    chart->addAxis(currentAxisX, Qt::AlignLeft);
    // Set grid line visibility

    std::cout << "Checking the GridLines\n" << GridLines[0] << std::endl;
    std::cout << GridLines[1] << std::endl;
    chart->axes(Qt::Horizontal).back()->setGridLineVisible(GridLines[0]);
    chart->axes(Qt::Vertical).back()->setGridLineVisible(GridLines[1]);

    // Set legend visibility
    chart->legend()->setVisible(ShowLegend);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->legend()->setShowToolTips(true);
    chart->legend()->setMarkerShape(QLegend::MarkerShapeFromSeries);
}

void
nmfChartLine::callback_hoveredLine(const QPointF& point, bool hovered)
{
    QString tooltip="";
    QPoint pos = QCursor::pos();

    if (hovered) {
        if (m_HoverLabels.size() > 0) {
            tooltip = m_HoverLabels[qobject_cast<QLineSeries* >(QObject::sender())->name()];
        }
        else if (m_Tooltips.size() > 0) {
            tooltip = m_Tooltips[qobject_cast<QLineSeries* >(QObject::sender())->name()];
        }

        // Workaround code to keep tooltip up for 2 seconds.  See note below.
        m_CustomToolTip->move(pos.x(),pos.y()-20);
        m_CustomToolTip->setLabel(tooltip);
        m_CustomToolTip->show();
        QTimer::singleShot(nmfConstantsMSSPM::ToolTipDuration, this, SLOT(callback_HideTooltip()));

        // The following line doesn't keep the tooltip on for 5 seconds...just 1 second.
        // Could be a QT bug. As a workaround, I created a custom tooltip (m_CustomToolTip) and
        // and showing it in this method (above) and then hiding it with a singleShot timer.
        // QToolTip::showText(pos, tooltip, nullptr, QRect(), 5000); // 5000 milliseconds = 5 seconds
    } else {
        // QToolTip::hideText();
        m_CustomToolTip->hide();
    }

}

void
nmfChartLine::callback_HideTooltip()
{
    // QToolTip::hideText();
    m_CustomToolTip->hide();
}

