#ifndef FOLIATIONDRAWINGAREA_H
#define FOLIATIONDRAWINGAREA_H

#include <QWidget>

namespace balazs{
    class Foliation;
    class SeparatrixSegment;
    class TransverseCurveData;

}


class FoliationDrawingArea : public QWidget
{
    Q_OBJECT
public:
    explicit FoliationDrawingArea(const balazs::Foliation &foliation, QWidget *parent = 0);
    
    QSize minimumSizeHint() const { return QSize(200, 200); }
    QSize sizeHint() const { return QSize(500, 500); }



signals:
    
public slots:
    void setPermutationLabels(bool visible);
    void setLengthsLabels(bool visible);
    void setColoredFilling(bool visible);
    void setTouchingSepSegments(bool visible);
    void drawSepSegment(const balazs::SeparatrixSegment* pSegment);
    void drawTransverseCurve(const balazs::TransverseCurveData* pTCData);
protected:
    void paintEvent(QPaintEvent *event);
    
private:
    void paint(QPainter &painter, int w, int h);

    bool permutationLabelsShown;
    bool lengthsLabelsShown;
    bool coloredFillingShown;
    bool touchingSepSegmentsShown;

    int permFontSize;
    int lengthsFontSize;

    const balazs::SeparatrixSegment* pSepSegment;
    const balazs::TransverseCurveData* pTransverseCurveData;
    const balazs::Foliation& m_foliation;
};

//void paintFoliation(const balazs::Foliation& foliation, QPainter &painter, int folW, int folH);
//void paintLengthLabels(const balazs::Foliation& foliation, QPainter &painter, int folW, int folH);
//void paintPermutationLabels(const balazs::Foliation& foliation, QPainter &painter, int folW, int folH);
//void paintFilling(const balazs::Foliation& foliation, QPainter &painter, int folW, int folH);
//void paintSepSegment(const balazs::SeparatrixSegment& sepSegment, QPainter &painter, int folW, int folH);
//void paintTransverseCurve(const balazs::TransverseCurve& tc, QPainter &painter, int folW, int folH);
//void paintDisjointIntervals(const balazs::DisjointIntervals dIntervals, QPainter &painter, int folW, int folH);



#endif // FOLIATIONDRAWINGAREA_H
