#ifndef FOLIATIONDRAWINGAREA_H
#define FOLIATIONDRAWINGAREA_H

#include <QWidget>

namespace balazs{
    class Foliation;
    class SeparatrixSegment;
    class TransverseCurve;
}


class FoliationDrawing : public QWidget
{
    Q_OBJECT
public:
    explicit FoliationDrawing(const balazs::Foliation& foliation, const QSize& sizehint, QWidget *parent = 0);
    
    QSize minimumSizeHint() const { return QSize(200, 200); }
    QSize sizeHint() const { return m_sizeHint; }
    
public slots:
    void setPermutationLabels(bool visible);
    void setLengthsLabels(bool visible);
    void setColoredFilling(bool visible);
    void setTouchingSepSegments(bool visible);
    void drawSepSegment(const balazs::SeparatrixSegment* pSegment);
    void drawTransverseCurve(const balazs::TransverseCurve* ptc);
    
private:
    void paintEvent(QPaintEvent *event);

    QSize m_sizeHint;

    bool permutationLabelsShown;
    bool lengthsLabelsShown;
    bool coloredFillingShown;
    bool touchingSepSegmentsShown;

    int permFontSize;
    int lengthsFontSize;

    const balazs::SeparatrixSegment* pSepSegment;
    const balazs::TransverseCurve* pTransverseCurve;
    const balazs::Foliation& m_foliation;
};


#endif // FOLIATIONDRAWINGAREA_H
