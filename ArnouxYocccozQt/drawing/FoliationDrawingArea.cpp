#include "FoliationDrawingArea.h"
#include "../fol/Foliation.h"
#include <QPainter>
#include <QDebug>
//#include <QFont>

FoliationDrawingArea::FoliationDrawingArea(const balazs::Foliation &foliation, QWidget *parent) :
    QWidget(parent),
    m_foliation(foliation)
{
    setBackgroundRole(QPalette::Base);
    update();
}


void FoliationDrawingArea::setPermutationCheckBoxState(int state)
{
    permutationCheckBoxState = Qt::CheckState(state);
    update();
}

void FoliationDrawingArea::setLengthsCheckBoxState(int state)
{
    lengthsCheckBoxState = Qt::CheckState(state);
    update();
}

void FoliationDrawingArea::setFillingCheckBoxState(int state)
{
    fillingCheckBoxState = Qt::CheckState(state);
    update();
}

void FoliationDrawingArea::setPermFontSize(int size)
{
    permFontSize = size;
    update();
}

void FoliationDrawingArea::setLengthsFontSize(int size)
{
    lengthsFontSize = size;
    update();
}






void FoliationDrawingArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    paintFoliation(painter, width(), height());
  //  painter.setRenderHint(QPainter::Antialiasing);


}


void FoliationDrawingArea::paintFoliation(QPainter &painter, int w, int h)
{
    painter.resetTransform();
    painter.translate(w / 10.0, h / 10.0);

    double folW = w * 8 / 10.0;
    double folH = h * 8 / 10.0;

    painter.setPen(Qt::NoPen);
    if(fillingCheckBoxState == Qt::Checked){
        // Qt::GlobalColor has predefinied colors, 7-18 are not white/black/grey.

        for(std::size_t i = 0; i < m_foliation.numIntervals(); i++){
            QBrush brush(Qt::GlobalColor(i % 12 + 7), Qt::Dense4Pattern);
            painter.fillRect(QRectF(folW * m_foliation.topDivPoints()[i], 0,
                                    folW * m_foliation.intExchange().lengths()[i], folH), brush);

            std::size_t bottomIndex = m_foliation.intExchange().permutationWithMinimalTwist()[i];
            if(bottomIndex < m_foliation.numIntervals() - 1){
                painter.fillRect(QRectF(folW * m_foliation.bottomDivPoints()[bottomIndex], folH,
                                    folW * m_foliation.intExchange().lengths()[i], 0.05 * folH), brush);
            } else {
                double firstPartWidth = folW * (1 - static_cast<double>(m_foliation.bottomDivPoints()[bottomIndex]));
                painter.fillRect(QRectF(folW * m_foliation.bottomDivPoints()[bottomIndex], folH,
                                    firstPartWidth, 0.05 * folH), brush);
                painter.fillRect(QRectF(0, folH,
                                    folW * m_foliation.intExchange().lengths()[i] - firstPartWidth, 0.05 * folH), brush);
            }

        }


    }



    painter.setPen(QPen(Qt::black, 0, Qt::SolidLine));
    painter.drawRect(0, 0, folW, folH);

    for(std::size_t i = 0; i < m_foliation.numIntervals(); i++){
        double singularityHeight =
                folH * (m_foliation.indexOfSingularity()[i] + 1.0) / (m_foliation.numSingularities() + 1.0);

        QPointF topPoint(folW * m_foliation.topDivPoints()[i], 0);
        QPointF middlePoint(folW * m_foliation.topDivPoints()[i], singularityHeight);
        QPointF bottomPoint(folW * m_foliation.topDivPoints()[i], folH);

        painter.setPen(QPen(Qt::black, 0, Qt::SolidLine));
        painter.drawLine(topPoint, middlePoint);
        painter.setPen(QPen(Qt::black, 0, Qt::DotLine));
        painter.drawLine(bottomPoint, middlePoint);

        painter.save();
        painter.setPen(QPen(Qt::black, 4));
        painter.drawPoint(middlePoint);
        painter.restore();
    }

    painter.setPen(QPen(Qt::black, 0, Qt::DashLine));
    for(std::size_t i = 0; i < m_foliation.numIntervals(); i++){
        QPointF topPoint(folW * m_foliation.bottomDivPoints()[i], folH);
        QPointF bottomPoint(folW * m_foliation.bottomDivPoints()[i], 1.05 * folH);
        painter.drawLine(topPoint, bottomPoint);
    }



    painter.setFont(QFont("Times", permFontSize));

    if(permutationCheckBoxState == Qt::Checked){
        for(std::size_t i = 0; i < m_foliation.numIntervals(); i++){
            painter.drawText(QRectF(folW * m_foliation.topDivPoints()[i], 2,
                                           folW * m_foliation.intExchange().lengths()[i], folH),
                             Qt::AlignTop | Qt::AlignHCenter,
                             QString::number(i));

            std::size_t bottomIndex = m_foliation.intExchange().permutationWithMinimalTwist()[i];
            QRectF rect;
            if(bottomIndex < m_foliation.numIntervals() - 1 ||
                    balazs::distanceBetween(m_foliation.bottomDivPoints()[bottomIndex], 1) >=
                    static_cast<balazs::floating_point_type>(m_foliation.intExchange().lengths()[i]) / 2){
                rect = QRectF(folW * m_foliation.bottomDivPoints()[bottomIndex], folH + 2,
                              folW * m_foliation.intExchange().lengths()[i], 10000);
            } else {
                rect = QRectF(folW * m_foliation.bottomDivPoints()[bottomIndex] - folW, folH + 2,
                              folW * m_foliation.intExchange().lengths()[i], 10000);
            }

            painter.drawText(rect, Qt::AlignTop | Qt::AlignHCenter, QString::number(i));
        }
    }

    painter.setFont(QFont("Times", lengthsFontSize));

    if(lengthsCheckBoxState == Qt::Checked){
        for(std::size_t i = 0; i < m_foliation.numIntervals(); i++){
            painter.drawText(QRectF(folW * m_foliation.topDivPoints()[i], 0,
                                    folW * m_foliation.intExchange().lengths()[i], folH),
                             Qt::AlignBottom | Qt::AlignHCenter,
                             QString::number(static_cast<double>(
                                                 m_foliation.intExchange().lengths()[i])));
        }
    }




}
