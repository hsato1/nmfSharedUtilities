#ifndef NMFCHARTSURFACE_H
#define NMFCHARTSURFACE_H

#include <Q3DSurface>
#include <QtDataVisualization>
#include <QString>
#include <QStringList>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

#include <vector>

using namespace QtDataVisualization;

/**
 * @brief This class allows the user to display a 3d surface using the passed arguments for customization.
 */
class nmfChartSurface : public QObject
{

    Q_OBJECT

    QSurfaceDataProxy* m_surfaceProxy;
    QSurface3DSeries*  m_surfaceSeries;
    std::pair<int,int> m_minCoord;
    boost::numeric::ublas::matrix<double> m_heightValues;
    void m_swapYandZPointsInLabel(QPoint pt);

public:

    /**
     * @brief Describes a 3d surface plot
     * @param graph3D : the Qt 3D surface widget
     * @param xTitle : title for x-axis
     * @param yTitle : title for y-axis (the vertical axis)
     * @param zTitle : title for z_axis
     * @param xLabelFormat : format for the labels along the x-axis
     * @param zLabelFormat : format for the labels along the z-axis
     * @param reverseXAxis : boolean signifying if the user wants the x-axis reversed
     * @param reverseZAxis : boolean signifying if the user wants the z-axis reversed
     * @param rowValues : matrix containing the x values of the plot
     * @param columnValues : matrix containing the z values of the plot
     * @param heightValues : matrix containing the height values of plot (i.e. the y values)
     * @param showShadow : boolean signifying if the user wants to see the surface plot's shadow
     * @param manualSetScale : boolean signifying if the user wants to manually set the vertical scale
     * @param manualYMin : minimum y axis value on surface
     * @param manualYMax : maximum y axis value on surface
     */
    nmfChartSurface(
            Q3DSurface* graph3D,
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
            const double& manualYMax);

    virtual ~nmfChartSurface() {}

    /**
     * @brief Selects the center point on the 3d surface with Qt's blue marker ball
     */
    void selectCenterPoint();
    /**
     * @brief Selects the minimum point on the 3d surface with Qt's blue marker ball
     */
    void selectMinimumPoint();

};


#endif
