
#include "nmfChartLineWithScatter.h"


nmfChartLineWithScatter::nmfChartLineWithScatter()
{
    m_tooltips.clear();
    m_CustomToolTip = new nmfToolTip();
}


void
nmfChartLineWithScatter::populateChart(
        QChart *chart,
        std::string &type,
        const std::string &style,
        const bool& isMohnsRho,
        const bool &ShowFirstPoint,
        const bool &AddScatter,
        const int &XOffset,
        const bool &xAxisIsInteger,
        const double &YMinSliderVal,
        const boost::numeric::ublas::matrix<double> &YAxisData,
        const boost::numeric::ublas::matrix<double> &ScatterData,
        const QStringList &RowLabels,
        const QStringList &ColumnLabels,
        std::string &MainTitle,
        std::string &XTitle,
        std::string &YTitle,
        const std::vector<bool> &GridLines,
        const int &Theme,
        const QColor &DashedLineColor,
        const QColor &ScatterColor,
        const std::string &LineColorValue,
        const std::string &lineColorName,
        const QList<QString>& multiRunLineLabels,
        const bool& showLegend)
{
    int numPointsInLine = 0;
    QLineSeries    *lineSeries    = nullptr;
    QScatterSeries *scatterSeries = nullptr;
    QString scatterSeriesName = "Obs Biomass";
    QPen pen;
    QColor  LineColor(QString::fromStdString(LineColorValue));
    QString LineColorName = QString::fromStdString(lineColorName);

    int XStartVal = (ShowFirstPoint) ? 0 : 1;
    XStartVal += XOffset;

    // Set main title
    QFont mainTitleFont = chart->titleFont();
    mainTitleFont.setPointSize(14);
    mainTitleFont.setWeight(QFont::Bold);
    chart->setTitleFont(mainTitleFont);
    chart->setTitle(QString::fromStdString(MainTitle));

    chart->legend()->hide();
    // Set current theme
    chart->setTheme(static_cast<QChart::ChartTheme>(Theme));
    //chart->removeAllSeries();
//std::cout << 1 << std::endl;
    // Load data into series and then add series to the chart
    numPointsInLine = YAxisData.size1();
    for (unsigned int line=0; line<YAxisData.size2(); ++line) {
        lineSeries = new QLineSeries();
        if (style == "DashedLine") {
            pen = lineSeries->pen();
            pen.setStyle(Qt::DashLine);
            pen.setColor(DashedLineColor);
            pen.setWidth(2);
            lineSeries->setPen(pen);
        } else {
            pen = lineSeries->pen();
            pen.setStyle(Qt::SolidLine);
            pen.setColor(LineColor);
            pen.setWidth(2);
            lineSeries->setPen(pen);
            //series->setColor(LineColor);
        }
        for (unsigned j=XStartVal-XOffset; j<numPointsInLine; ++j) {
            lineSeries->append(j+XOffset,YAxisData(j,line));
        }
        numPointsInLine = (isMohnsRho) ? numPointsInLine-1 : numPointsInLine;
        chart->addSeries(lineSeries);

        if (multiRunLineLabels.size() > 0) {
            lineSeries->setName(multiRunLineLabels[line]);
            m_tooltips[multiRunLineLabels[line]] = multiRunLineLabels[line];
        } else if (ColumnLabels.size() > 0) {
            lineSeries->setName(ColumnLabels[0]);
            m_tooltips[ColumnLabels[0]] = LineColorName;
        }

        disconnect(lineSeries,0,0,0);
        connect(lineSeries, SIGNAL(hovered(const QPointF&,bool)),
                this,       SLOT(callback_hoveredLine(const QPointF&,bool)));
    }
//std::cout << 2 << std::endl;

    if ((ScatterData.size1() != 0) && AddScatter) {
        scatterSeries = new QScatterSeries();
        scatterSeries->setMarkerSize(10);
        scatterSeries->setColor(ScatterColor);
        for (unsigned int j=XStartVal-XOffset; j<ScatterData.size1(); ++j) {
            scatterSeries->append(j+XOffset,ScatterData(j,0)); // Change 0 if more than one type of scatter points
        }
        chart->addSeries(scatterSeries);
        scatterSeries->setName(scatterSeriesName);
        if (m_tooltips.size() > 0) {
            m_tooltips[scatterSeriesName] = scatterSeriesName; //"Deep Sky Blue";
        }

        disconnect(scatterSeries,0,0,0);
        connect(scatterSeries, SIGNAL(hovered(const QPointF&,bool)),
                this,          SLOT(callback_hoveredScatter(const QPointF&,bool)));
    }
//std::cout << 3 << std::endl;

//    // Set main title
//    QFont mainTitleFont = chart->titleFont();
//    mainTitleFont.setPointSize(14);
//    mainTitleFont.setWeight(QFont::Bold);
//    chart->setTitleFont(mainTitleFont);
//    chart->setTitle(QString::fromStdString(MainTitle));

    // Setup X and Y axes
    chart->createDefaultAxes();
    QFont titleFont = chart->axes(Qt::Horizontal).back()->titleFont();
    titleFont.setPointSize(12);
    titleFont.setWeight(QFont::Bold);
    chart->axes(Qt::Horizontal).back()->setTitleFont(titleFont);
    chart->axes(Qt::Horizontal).back()->setTitleText(QString::fromStdString(XTitle));

    QValueAxis *currentAxisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).back());
    currentAxisY->setTitleFont(titleFont);
    currentAxisY->setTitleText(QString::fromStdString(YTitle));
    currentAxisY->applyNiceNumbers();
    double currentYMin = currentAxisY->min();
    if (YMinSliderVal >= 0) {
        currentAxisY->setMin(currentYMin*YMinSliderVal/100.0);
        currentAxisY->applyNiceNumbers();
    }
//std::cout << 4 << std::endl;

    QValueAxis *currentAxisX = qobject_cast<QValueAxis*>(chart->axes(Qt::Horizontal).back());
    currentAxisX->applyNiceNumbers();
    if (xAxisIsInteger) {
        currentAxisX->setLabelFormat("%d");
    }

    //   Set grid line visibility
    chart->axes(Qt::Horizontal).back()->setGridLineVisible(GridLines[0]);
    chart->axes(Qt::Vertical).back()->setGridLineVisible(GridLines[1]);

    // Show legend
    chart->legend()->setVisible(showLegend);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->legend()->setShowToolTips(true);
    chart->legend()->setMarkerShape(QLegend::MarkerShapeFromSeries);

    // Add hovered callbacks to set tooltips
    QList<QLegendMarker* > legendMarkers = chart->legend()->markers();
    for (QLegendMarker* legendMarker : legendMarkers) {
        disconnect(legendMarker,0,0,0);
        connect(legendMarker, SIGNAL(hovered(bool)),
                this,         SLOT(callback_hoveredLegend(bool)));
    }

}

void
nmfChartLineWithScatter::callback_hoveredLine(const QPointF& point, bool hovered)
{
    QString tooltip;
    QPoint pos = QCursor::pos();

    if (hovered) {
        tooltip = m_tooltips[qobject_cast<QLineSeries* >(QObject::sender())->name()];

        // Workaround code to keep tooltip up for 2 seconds.  See note below.
        m_CustomToolTip->move(pos.x(),pos.y()-20);
        m_CustomToolTip->setLabel(tooltip);
        m_CustomToolTip->show();
        QTimer::singleShot(nmfConstantsMSSPM::ToolTipDuration, this, SLOT(callback_HideTooltip()));

        // The following line doesn't keep the tooltip on for 2 seconds...just 1 second.
        // Could be a QT bug. As a workaround, I created a custom tooltip (m_CustomToolTip) and
        // and showing it in this method (above) and then hiding it with a singleShot timer.
        // QToolTip::showText(pos, tooltip, nullptr, QRect(), 2000);
    } else {
        // QToolTip::hideText();
        m_CustomToolTip->hide();
    }
}

void
nmfChartLineWithScatter::callback_hoveredScatter(const QPointF& point, bool hovered)
{
    QString tooltip;
    QPoint pos = QCursor::pos();

    if (hovered) {
        tooltip = m_tooltips[qobject_cast<QScatterSeries* >(QObject::sender())->name()];
        m_CustomToolTip->move(pos.x(),pos.y()-20);
        m_CustomToolTip->setLabel(tooltip);
        m_CustomToolTip->show();
        QTimer::singleShot(nmfConstantsMSSPM::ToolTipDuration, this, SLOT(callback_HideTooltip()));
        // QToolTip::showText(pos, tooltip, nullptr, QRect(), 2000);
    } else {
        // QToolTip::hideText();
        m_CustomToolTip->hide();
    }
}

void
nmfChartLineWithScatter::callback_hoveredLegend(bool hovered)
{
    QString tooltip;
    QPoint pos = QCursor::pos();

    if (hovered) {
        tooltip = m_tooltips[qobject_cast<QLegendMarker* >(QObject::sender())->label()];
        QToolTip::showText(pos, tooltip);
    } else {
        QToolTip::hideText();
    }
}

void
nmfChartLineWithScatter::callback_HideTooltip()
{
    // QToolTip::hideText();
    m_CustomToolTip->hide();
}
