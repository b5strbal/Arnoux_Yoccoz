#ifndef FOLIATIONDRAWINGAREA_H
#define FOLIATIONDRAWINGAREA_H

#include <QWidget>

namespace balazs{
    class Foliation;
    class SeparatrixSegment;
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
protected:
    void paintEvent(QPaintEvent *event);
    
private:
    void paint(QPainter &painter, int w, int h);
    void paintFoliation(QPainter &painter, int folW, int folH);
    void paintLengthLabels(QPainter &painter, int folW, int folH);
    void paintPermutationLabels(QPainter &painter, int folW, int folH);
    void paintFilling(QPainter &painter, int folW, int folH);
    void paintSepSegments(QPainter &painter, int folW, int folHh);

    bool permutationLabelsShown;
    bool lengthsLabelsShown;
    bool coloredFillingShown;
    int permFontSize;
    int lengthsFontSize;

    QList<balazs::SeparatrixSegment*> d;

    const balazs::Foliation& m_foliation;
};




#endif // FOLIATIONDRAWINGAREA_H