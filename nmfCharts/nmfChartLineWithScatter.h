
#pragma once

#include <QChart>
#include <QLine>
#include <QLineSeries>
#include <QScatterSeries>
#include <QString>
#include <QStringList>
#include <QValueAxis>
#include <QLabel>
#include <QLegend>
#include <QLegendMarker>
#include <QToolTip>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

#include <vector>
#include "nmfUtilsQt.h"

QT_CHARTS_USE_NAMESPACE


class nmfChartLineWithScatter : public QObject
{

    Q_OBJECT

private:
    std::map<QString,QString> m_tooltips;
    nmfToolTip*               m_CustomToolTip;

public:
    nmfChartLineWithScatter();
    virtual ~nmfChartLineWithScatter() {}

    void populateChart(
            QChart  *chart,
            std::string &type,
            const std::string &style,
            const bool& isMohnsRho,
            const bool &skipFirstPoint,
            const bool &addScatter,
            const int  &XOffset,
            const bool &xAxisIsInteger,
            const double &YMin,
            const boost::numeric::ublas::matrix<double> &LineData,
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
            const std::string &LineColor,
            const std::string &lineColorName,
            const QList<QString>& multiRunLineLabels);
signals:

public slots:
    void callback_hoveredLegend(bool hovered);
    void callback_hoveredLine(const QPointF& point, bool hovered);
    void callback_hoveredScatter(const QPointF& point, bool hovered);
    void callback_HideTooltip();

};

