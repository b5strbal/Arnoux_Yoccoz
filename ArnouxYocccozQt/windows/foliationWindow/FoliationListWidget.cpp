#include "FoliationListWidget.h"

FoliationListWidget::FoliationListWidget(QWidget *parent) :
    QListWidget(parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
}
