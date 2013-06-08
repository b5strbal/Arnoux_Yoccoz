#ifndef FOLIATIONDISKDRAWING_H
#define FOLIATIONDISKDRAWING_H

#include <QWidget>

namespace balazs{
    class FoliationDisk;
}


class FoliationDiskDrawing : public QWidget
{
    Q_OBJECT
public:
    explicit FoliationDiskDrawing(const balazs::FoliationDisk& fol, int size, QWidget *parent = 0);
    QSize sizeHint() const { return QSize(m_size, m_size); }
    int heightForWidth(int w) const{ return w; }
signals:
    
public slots:
    void setLengthLabelsShown(bool visible) { lengthLabelsShown = visible; }

private:
    void paintEvent(QPaintEvent *);

    int m_size;
    const balazs::FoliationDisk& foliationDisk;

    bool lengthLabelsShown;
};

#endif // FOLIATIONDISKDRAWING_H
