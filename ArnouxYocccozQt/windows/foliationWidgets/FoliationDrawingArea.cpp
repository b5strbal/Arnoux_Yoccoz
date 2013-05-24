#include "FoliationDrawingArea.h"
#include "../../fol/Foliation.h"
#include "../../scc/SeparatrixSegment.h"
#include "../../scc/TransverseCurve.h"
#include "../../scc/DisjointIntervals.h"
#include "../../scc/SepSegmentDatabase.h"
#include <QPainter>
//#include <QFont>

FoliationDrawingArea::FoliationDrawingArea(const balazs::Foliation &foliation, QWidget *parent) :
    QWidget(parent),
    pSepSegment(nullptr),
    pTransverseCurve(nullptr),
    m_foliation(foliation)
{
    permFontSize = lengthsFontSize = 10;
    setBackgroundRole(QPalette::Base);
    update();
}



void FoliationDrawingArea::setPermutationLabels(bool visible)
{
    permutationLabelsShown = visible;
    update();
}

void FoliationDrawingArea::setLengthsLabels(bool visible)
{
    lengthsLabelsShown = visible;
    update();
}

void FoliationDrawingArea::setColoredFilling(bool visible)
{
    coloredFillingShown = visible;
    update();
}

void FoliationDrawingArea::setTouchingSepSegments(bool visible)
{
    touchingSepSegmentsShown = visible;
    update();
}

void FoliationDrawingArea::drawSepSegment(const balazs::SeparatrixSegment *pSegment)
{
    pSepSegment = pSegment;
    update();
}

void FoliationDrawingArea::drawTransverseCurve(const balazs::TransverseCurve *ptc)
{
    pTransverseCurve = ptc;
    update();
}






void FoliationDrawingArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    paint(painter, width(), height());
  //  painter.setRenderHint(QPainter::Antialiasing);


}



inline double heightOfSingularity(const balazs::Foliation& foliation, std::size_t sepIndex, int folH)
{
    return  folH * (foliation.indexOfSingularity()[sepIndex] + 1.0) / (foliation.numSingularities() + 1.0);
}




void paintFoliation(const balazs::Foliation& foliation, QPainter &painter, int folW, int folH)
{
    painter.save();
    painter.setPen(Qt::SolidLine);
    painter.drawRect(0, 0, folW, folH);

    for(std::size_t i = 0; i < foliation.numIntervals(); i++){
        double singularityHeight = heightOfSingularity(foliation, i, folH);

        QPointF topPoint(folW * foliation.topDivPoints()[i], 0);
        QPointF middlePoint(folW * foliation.topDivPoints()[i], singularityHeight);
        QPointF bottomPoint(folW * foliation.topDivPoints()[i], folH);

        painter.setPen(Qt::SolidLine);
        painter.drawLine(topPoint, middlePoint);
        painter.setPen(Qt::DotLine);
        painter.drawLine(bottomPoint, middlePoint);

        painter.save();
        painter.setPen(QPen(Qt::black, 4));
        painter.drawPoint(middlePoint);
        painter.restore();
    }

    painter.setPen(Qt::DashLine);
    for(std::size_t i = 0; i < foliation.numIntervals(); i++){
        QPointF topPoint(folW * foliation.bottomDivPoints()[i], folH);
        QPointF bottomPoint(folW * foliation.bottomDivPoints()[i], 1.05 * folH);
        painter.drawLine(topPoint, bottomPoint);
    }
    painter.restore();
}

void paintLengthLabels(const balazs::Foliation& foliation, QPainter &painter, int folW, int folH)
{
    for(std::size_t i = 0; i < foliation.numIntervals(); i++){
        painter.drawText(QRectF(folW * foliation.topDivPoints()[i], 0,
                                folW * foliation.intExchange().lengths()[i], folH),
                         Qt::AlignBottom | Qt::AlignHCenter,
                         QString::number(static_cast<double>(
                                             foliation.intExchange().lengths()[i])));
    }
}

void paintPermutationLabels(const balazs::Foliation& foliation, QPainter &painter, int folW, int folH)
{
    for(std::size_t i = 0; i < foliation.numIntervals(); i++){
        painter.drawText(QRectF(folW * foliation.topDivPoints()[i], 2,
                                folW * foliation.intExchange().lengths()[i], folH),
                         Qt::AlignTop | Qt::AlignHCenter,
                         QString::number(i));

        std::size_t bottomIndex = foliation.intExchange().permutationWithMinimalTwist()[i];
        QRectF rect;
        if(bottomIndex < foliation.numIntervals() - 1 ||
                balazs::distanceBetween(foliation.bottomDivPoints()[bottomIndex], 1) >=
                static_cast<long double>(foliation.intExchange().lengths()[i]) / 2){
            rect = QRectF(folW * foliation.bottomDivPoints()[bottomIndex], folH + 2,
                          folW * foliation.intExchange().lengths()[i], 10000);
        } else {
            rect = QRectF(folW * foliation.bottomDivPoints()[bottomIndex] - folW, folH + 2,
                          folW * foliation.intExchange().lengths()[i], 10000);
        }

        painter.drawText(rect, Qt::AlignTop | Qt::AlignHCenter, QString::number(i));
    }
}

void paintFilling(const balazs::Foliation& foliation, QPainter &painter, int folW, int folH)
{
    painter.save();
    painter.setPen(Qt::NoPen);
    // Qt::GlobalColor has predefinied colors, 7-18 are not white/black/grey.

    for(std::size_t i = 0; i < foliation.numIntervals(); i++){
        QBrush brush(Qt::GlobalColor(i % 12 + 7), Qt::Dense4Pattern);
        painter.fillRect(QRectF(folW * foliation.topDivPoints()[i], 0,
                                folW * foliation.intExchange().lengths()[i], folH), brush);

        std::size_t bottomIndex = foliation.intExchange().permutationWithMinimalTwist()[i];
        if(bottomIndex < foliation.numIntervals() - 1){
            painter.fillRect(QRectF(folW * foliation.bottomDivPoints()[bottomIndex], folH,
                                    folW * foliation.intExchange().lengths()[i], 0.05 * folH), brush);
        } else {
            double firstPartWidth = folW * (1 - static_cast<double>(foliation.bottomDivPoints()[bottomIndex]));
            painter.fillRect(QRectF(folW * foliation.bottomDivPoints()[bottomIndex], folH,
                                    firstPartWidth, 0.05 * folH), brush);
            painter.fillRect(QRectF(0, folH,
                                    folW * foliation.intExchange().lengths()[i] - firstPartWidth, 0.05 * folH), brush);
        }

    }
    painter.restore();
}



void paintSepSegment(const balazs::SeparatrixSegment &sepSegment, QPainter &painter, int folW, int folH)
{
    // initializing the subsegment of depth 1
    balazs::SeparatrixSegment segment(sepSegment.foliation(), index(sepSegment));
    double shift = sepSegment.hDirection() == balazs::HDirection::Left ? -1.0 : 1.0;
    double singularityHeight = heightOfSingularity(sepSegment.foliation(), segment.startingSingularity(), folH);
    double w = folW * static_cast<double>(sepSegment.foliation().topDivPoints()[segment.startingSingularity()]);

    if(segment.vDirection() == balazs::VDirection::Down){
        painter.drawLine(w + shift, singularityHeight, w + shift, folH);

        while(segment.depth() < sepSegment.depth()){
            painter.drawLine(w + shift, folH, w + shift, 1.05 * folH);
            segment.lengthen();
            w = folW * static_cast<double>(segment.endpoint().number());
            painter.drawLine(w + shift, 0, w + shift, folH);
        }

    } else {
        double startingHeight = singularityHeight;

        while(segment.depth() <= sepSegment.depth()){
            painter.drawLine(w + shift, startingHeight, w + shift, 0);
            w = folW * static_cast<double>(segment.endpoint().number());
            painter.drawLine(w + shift, folH, w + shift, 1.05 * folH);
            segment.lengthen();
            startingHeight = folH;
        }
    }
}


void paintDisjointIntervals(const balazs::DisjointIntervals& dIntervals, QPainter &painter, int folW, int folH)
{
    if(!dIntervals.wrapsAroundEnds()){
        for(std::size_t i = 0; i < dIntervals.endpoints().size(); i += 2){
            painter.drawLine(folW * dIntervals.endpoints()[i], folH, folW * dIntervals.endpoints()[i + 1], folH);
        }
    } else {
        for(std::size_t i = 0; i < dIntervals.endpoints().size() - 2; i += 2){
            painter.drawLine(folW * dIntervals.endpoints()[i + 1], folH, folW * dIntervals.endpoints()[i + 2], folH);
        }
        painter.drawLine(0, folH, folW * dIntervals.endpoints()[0], folH);
        painter.drawLine(folW, folH, folW * dIntervals.endpoints().back(), folH);
    }
}


void paintTransverseCurve(const balazs::TransverseCurve& tc, QPainter &painter, int folW, int folH)
{
    for(auto pSepSegment : tc.sepSegmentCollection()){
        paintSepSegment(*pSepSegment, painter, folW, folH);
    }
    paintDisjointIntervals(tc.disjointIntervals(), painter, folW, folH);
}

void paintTouchingSepSegments(const balazs::TransverseCurve& tc, QPainter &painter, int folW, int folH)
{
    for(std::size_t i = 0; i < tc.foliation().numIntervals(); i++){
        paintSepSegment(tc.touchingSepSegment({balazs::HDirection::Right,
                                                   balazs::VDirection::Down, i}, balazs::SepSegmentDatabase::Centered), painter, folW, folH);
        paintSepSegment(tc.touchingSepSegment({balazs::HDirection::Right,
                                                   balazs::VDirection::Up, i}, balazs::SepSegmentDatabase::Centered), painter, folW, folH);
    }
}




void FoliationDrawingArea::paint(QPainter &painter, int w, int h)
{
    painter.resetTransform();
    painter.translate(w / 10.0, h / 10.0);

    double folW = w * 8 / 10.0;
    double folH = h * 8 / 10.0;

    if(coloredFillingShown){
        paintFilling(m_foliation, painter, folW, folH);
    }

    painter.setPen(Qt::SolidLine);
    paintFoliation(m_foliation, painter, folW, folH);

    if(lengthsLabelsShown){
        painter.setFont(QFont("Times", lengthsFontSize));
        paintLengthLabels(m_foliation, painter, folW, folH);
    }

    if(permutationLabelsShown){
        painter.setFont(QFont("Times", permFontSize));
        paintPermutationLabels(m_foliation, painter, folW, folH);
    }

    if(pSepSegment){
        painter.setPen(QPen(Qt::blue, 2));
        paintSepSegment(*pSepSegment, painter, folW, folH);
    }

    if(pTransverseCurve){
        painter.setPen(QPen(Qt::red, 3));
        paintTransverseCurve(*pTransverseCurve, painter, folW, folH);

        if(touchingSepSegmentsShown){
            painter.setPen(QPen(Qt::darkGreen, 2, Qt::DashDotLine));
            paintTouchingSepSegments(*pTransverseCurve, painter, folW, folH);
        }
    }

}
