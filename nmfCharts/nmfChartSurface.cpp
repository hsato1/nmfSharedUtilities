
#include "nmfChartSurface.h"

#include <QEvent>
#include <QWidget>

nmfChartSurface::nmfChartSurface(Q3DSurface* graph3D,
                                 const QString& xTitle,
                                 const QString& yTitle,
                                 const QString& zTitle,
                                 const QString& xLabelFormat,
                                 const QString& zLabelFormat,
                                 const bool&    reverseXAxis,
                                 const bool&    reverseZAxis,
                                 const boost::numeric::ublas::matrix<double> &rowValues,
                                 const boost::numeric::ublas::matrix<double> &columnValues,
                                 const boost::numeric::ublas::matrix<double> &heightValues,
                                 const bool& showShadow,
                                 const bool& manuallySetScale,
                                 const double& manualYMin,
                                 const double& manualYMax)
{
    double x,y,z;
    double maxY=0;
    int nrows = heightValues.size1();
    int ncols = heightValues.size2();
    double minY = 10000000.0;
    double yMin = manualYMin;
    double yMax = manualYMax;
    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    m_minCoord = std::make_pair(0,0);
    m_heightValues = heightValues;


    dataArray->reserve(nrows);
    m_surfaceProxy  = new QSurfaceDataProxy();
    m_surfaceSeries = new QSurface3DSeries(m_surfaceProxy);

    for (int col = 0; col < ncols; ++col) {
        QSurfaceDataRow *newCol = new QSurfaceDataRow(nrows);
        for (int row = 0; row < nrows; ++row) {
            x = rowValues(row,col);
            y = heightValues(row,col);
            z = columnValues(row,col);
            if (y < minY) {
                m_minCoord = std::make_pair(col,row); //,col); // Needed to swap row and col here to match the data ordering
                minY = y;
            }
            maxY = (y > maxY) ? y : maxY;
            (*newCol)[row].setPosition(QVector3D(x, y, z));
        }
        *dataArray << newCol;
    }

    if (showShadow) {
        graph3D->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftHigh);
    } else {
        graph3D->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);
    }

    // Set up surface titles
    graph3D->axisX()->setTitle(xTitle);
    graph3D->axisX()->setTitleVisible(true);
    graph3D->axisY()->setTitle(yTitle);
    graph3D->axisY()->setTitleVisible(true);
    graph3D->axisZ()->setTitle(zTitle);
    graph3D->axisZ()->setTitleVisible(true);
    graph3D->axisX()->setLabelFormat(xLabelFormat);
    graph3D->axisZ()->setLabelFormat(zLabelFormat);
    graph3D->axisX()->setLabelAutoRotation(45.00);
    graph3D->axisY()->setLabelAutoRotation(45.00);
    graph3D->axisZ()->setLabelAutoRotation(45.00);

    // Remove any existing series and add new series
    for (QSurface3DSeries* series : graph3D->seriesList()) {
        graph3D->removeSeries(series);
    }
    m_surfaceProxy->resetArray(dataArray);
    graph3D->addSeries(m_surfaceSeries);

    // Some logic here to get "nicer" numbers along the vertical scale.
    if (manuallySetScale) {
        yMax = (yMax <= yMin) ? (yMin+1.0) : yMax;
        graph3D->axisY()->setRange(yMin,yMax);
    } else {
        int power = int(std::log10(maxY));             // Ex. let maxY = 304 -> power = 2
        power = std::pow(10,power);                    // Ex. power = 100
        double upperLim = power*(int(maxY/power + 1)); // Ex. 100*int(304/100+1) => upperLim = 400
        if (upperLim > 0) {
            graph3D->axisY()->setRange(0,upperLim);
        } else {
            graph3D->axisY()->setRange(0,1);
        }
    }

    // Reverse the points along the axis if desired
    graph3D->axisX()->setReversed(reverseXAxis);
    graph3D->axisZ()->setReversed(reverseZAxis);

    // Customize the surface label
    m_surfaceSeries->setItemLabelFormat(QStringLiteral("(@xLabel, @zLabel): @yLabel"));

}

void
nmfChartSurface::selectCenterPoint()
{
    int centerCol = m_surfaceSeries->dataProxy()->columnCount()/2;
    int centerRow = m_surfaceSeries->dataProxy()->rowCount()/2;
    m_surfaceSeries->setSelectedPoint(QPoint(centerRow,centerCol));
}

void
nmfChartSurface::selectMinimumPoint()
{
    m_surfaceSeries->setSelectedPoint(QPoint(m_minCoord.first,
                                             m_minCoord.second));
}



