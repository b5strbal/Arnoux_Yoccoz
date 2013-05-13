#ifndef FOLIATIONDRAWINGAREA_H
#define FOLIATIONDRAWINGAREA_H

#include <QWidget>

namespace balazs{
    class Foliation;
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
    void setPermutationCheckBoxState(int state);
    void setLengthsCheckBoxState(int state);
    void setFillingCheckBoxState(int state);
    void setPermFontSize(int size);
    void setLengthsFontSize(int size);

protected:
    void paintEvent(QPaintEvent *event);
    
private:
    void paintFoliation(QPainter &painter, int w, int h);

    Qt::CheckState permutationCheckBoxState;
    Qt::CheckState lengthsCheckBoxState;
    Qt::CheckState fillingCheckBoxState;
    int permFontSize;
    int lengthsFontSize;

    const balazs::Foliation& m_foliation;
};




#endif // FOLIATIONDRAWINGAREA_H
