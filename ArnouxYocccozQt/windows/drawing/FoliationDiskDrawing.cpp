#include "FoliationDiskDrawing.h"
#include "fol/FoliationDisk.h"
#include <QPainter>
#include <cmath>
#include <numeric>

const double pi = 3.14159265359;

FoliationDiskDrawing::FoliationDiskDrawing(const balazs::FoliationDisk &fol, int size, QWidget *parent) :
    QWidget(parent),
    m_size(size),
    foliationDisk(fol)
{
    QSizePolicy policy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    policy.setHeightForWidth(true);
    setSizePolicy(policy);
    lengthLabelsShown = true;
}


QPointF circlePoint(double diameter, double unitIntervalPoint)
{
    double angle = unitIntervalPoint * 2 * pi;
    return QPointF(diameter/2 * (1 - sin(angle)), diameter/2 * (1 - cos(angle)));
}


QPointF average(const std::vector<QPointF>& points)
{
    return std::accumulate(points.begin(), points.end(), QPointF(0, 0)) / points.size();
}


void paintFoliationDisk(const balazs::FoliationDisk& foliationDisk, QPainter& painter, double diameter){

    painter.drawEllipse(0, 0, diameter, diameter);

    std::vector<QPointF> divPoints;
    divPoints.reserve(foliationDisk.numSeparatrices());
    for(std::size_t i = 0; i < foliationDisk.numSeparatrices(); i++){
        divPoints.push_back(circlePoint(diameter, foliationDisk.intervalPairing().divPoints()[i]));
    }

    painter.save();
    painter.setPen(QPen(Qt::black, 4));
    for(const QPointF& point : divPoints){
        painter.drawPoint(point);
    }
    painter.restore();

    for(std::size_t i = 0; i < foliationDisk.singularities().size(); i++){
        if(foliationDisk.singularities()[i].size() == 1){
            QPointF divpoint = divPoints[foliationDisk.singularities()[i][0]];
            QPointF nextpoint = divPoints[(foliationDisk.singularities()[i][0] + 1) % foliationDisk.numSeparatrices()];
            QPointF prevpoint = divPoints[(foliationDisk.singularities()[i][0] + foliationDisk.numSeparatrices() - 1) %
                    foliationDisk.numSeparatrices()];

            QPointF singularity = average({divpoint, nextpoint, prevpoint});

            painter.save();
            painter.setPen(QPen(Qt::black, 4));
            painter.drawPoint(singularity);
            painter.restore();

            painter.drawLine(divpoint, singularity);

        } else {
            std::vector<QPointF> vertices;
            vertices.reserve(foliationDisk.singularities()[i].size());
            for(std::size_t index : foliationDisk.singularities()[i]){
                vertices.push_back(divPoints[index]);
            }
            QPointF hub = average(vertices);

            painter.save();
            painter.setPen(QPen(Qt::black, 4));
            painter.drawPoint(hub);
            painter.restore();

            for(const QPointF& point : vertices){
                painter.drawLine(point, hub);
            }
        }
    }
}


void paintLabels(const balazs::FoliationDisk& foliationDisk, QPainter& painter, double diameter){
    QFont font = painter.font();
    font.setPointSize(10);
    painter.setFont(font);
    for(std::size_t i = 0; i < foliationDisk.numSeparatrices(); i++){
        double midpoint = foliationDisk.intervalPairing().divPoints()[i] + foliationDisk.intervalPairing().lengths()[i]/2;
        QPointF midpoint2d = circlePoint(diameter, midpoint);

        // adjusting placement of text if it is on top on bottom of the circle
        double angle = midpoint * 2 * pi;
        if(fabs(sin(angle)) < 0.5){
            midpoint2d.setX(midpoint2d.x() - 15);
        }
        painter.drawText(midpoint2d, QString::number(static_cast<double>(foliationDisk.intervalPairing().lengths()[i])));
    }
}



void FoliationDiskDrawing::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    double diameter = std::min(width(), height()) * 8 / 10.0;

    painter.translate(width()/2 - diameter/2, height()/2 - diameter/2);

    paintFoliationDisk(foliationDisk, painter, diameter);

    if(lengthLabelsShown){
        paintLabels(foliationDisk, painter, diameter);
    }
}
