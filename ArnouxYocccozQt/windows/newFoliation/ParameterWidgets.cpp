#include "ParameterWidgets.h"
#include "math/Permutation.h"
#include "fol/Foliation.h"
#include "math/WeighedTree.h"
#include "fol/FoliationRP2.h"
#include "windows/drawing/FoliationDrawing.h"
#include "windows/drawing/FoliationRP2Drawing.h"
#include "windows/drawing/FoliationSphereDrawing.h"

#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QStackedWidget>




PreviewPageFoliation::PreviewPageFoliation(QWidget *parent)
    :   PreviewPage<balazs::Foliation>(parent)
{
    connect(helpButton, SIGNAL(clicked()), this, SLOT(onHelpButtonClickedSlot()));
    connect(previewButton, SIGNAL(clicked()), this, SLOT(onPreviewButtomClickedSlot()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(onOkButtonClickedSlot()));
}





QWidget *PreviewPageFoliation::createDrawing(const balazs::Foliation &fol)
{
    return new FoliationDrawing(fol, QSize(500, 500));
}



PreviewPageFoliationSphere::PreviewPageFoliationSphere(QWidget *parent)
    :   PreviewPage<balazs::FoliationSphere>(parent)
{
    connect(helpButton, SIGNAL(clicked()), this, SLOT(onHelpButtonClickedSlot()));
    connect(previewButton, SIGNAL(clicked()), this, SLOT(onPreviewButtomClickedSlot()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(onOkButtonClickedSlot()));
}


QWidget *PreviewPageFoliationSphere::createDrawing(const balazs::FoliationSphere &fol)
{
    return new FoliationSphereDrawing(fol);
}


PreviewPageFoliationRP2::PreviewPageFoliationRP2(QWidget *parent)
    :   PreviewPage<balazs::FoliationRP2>(parent)
{
     connect(helpButton, SIGNAL(clicked()), this, SLOT(onHelpButtonClickedSlot()));
     connect(previewButton, SIGNAL(clicked()), this, SLOT(onPreviewButtomClickedSlot()));
     connect(okButton, SIGNAL(clicked()), this, SLOT(onOkButtonClickedSlot()));
}


QWidget *PreviewPageFoliationRP2::createDrawing(const balazs::FoliationRP2 &fol)
{
    return new FoliationRP2Drawing(fol, 500);
}







NewFoliationIntExchangeWidget::NewFoliationIntExchangeWidget(QWidget *parent) :
    PreviewPageFoliation(parent)
{
    permutationLabel = new QLabel(tr("Permutation (e.g. 3 2 1 0):"));
    lengthsLabel = new QLabel(tr("Lengths (e.g. 1.2 0.3 0.43 0.12):"));
    twistLabel = new QLabel(tr("Twist (e.g. 0.5):"));

    permutationLineEdit = new QLineEdit;
    lengthsLineEdit = new QLineEdit;
    twistLineEdit = new QLineEdit;

    QHBoxLayout* permutationLayout = new QHBoxLayout;
    permutationLayout->addWidget(permutationLabel);
    permutationLayout->addWidget(permutationLineEdit);

    QHBoxLayout* lengthsLayout = new QHBoxLayout;
    lengthsLayout->addWidget(lengthsLabel);
    lengthsLayout->addWidget(lengthsLineEdit);

    QHBoxLayout* twistLayout = new QHBoxLayout;
    twistLayout->addWidget(twistLabel);
    twistLayout->addWidget(twistLineEdit);

    connect(permutationLineEdit, SIGNAL(textEdited(QString)), this, SLOT(disableOkButton(QString)));
    connect(lengthsLineEdit, SIGNAL(textEdited(QString)), this, SLOT(disableOkButton(QString)));
    connect(twistLineEdit, SIGNAL(textEdited(QString)), this, SLOT(disableOkButton(QString)));

    insertOnTop(twistLayout);
    insertOnTop(lengthsLayout);
    insertOnTop(permutationLayout);

    setTabOrder(permutationLineEdit, lengthsLineEdit);
    setTabOrder(lengthsLineEdit, twistLineEdit);
}

void NewFoliationIntExchangeWidget::disableOkButton(QString)
{
    okButton->setEnabled(false);
}

QString NewFoliationIntExchangeWidget::helpText()
{
    return tr("A twisted interval exchange map on the unit interval is defined by the lengths of the intervals, "
              "a permutation on the same number of elements, and the twist.\n\n"
              "You can specify a permutation on n elements as a permutation of "
              "0, 1, ..., n-1, spearated by spaces.\n\n"
              "To specify the lengths of the intervals, list n positive real numbers "
              "separated by spaces. The sum of the lengths doesn't have to be 1, but "
              "they will be normalized to get an interval exchange of the unit interval.\n\n"
              "The twist may be any real number, will be normalized by the same factor as the lengths and taken modulo 1.");
}


balazs::Foliation *NewFoliationIntExchangeWidget::createFoliation()
{
    QStringList permutationStringList = permutationLineEdit->text().split(" ", QString::SkipEmptyParts);
    QStringList lengthsStringList = lengthsLineEdit->text().split(" ", QString::SkipEmptyParts);

    bool ok;

    std::vector<long double> lengths;
    for(QString s : lengthsStringList){
        lengths.push_back(s.toDouble(&ok));
        if(!ok){
            throw(std::runtime_error("The lengths should be real numbers, you MORON!"));
        }
    }

    std::vector<std::size_t> permutationInput;
    for(QString s : permutationStringList){
        permutationInput.push_back(s.toUInt(&ok));
        if(!ok){
            throw(std::runtime_error("The numbers in the permutation list should be non-negative integers."));
        }
    }

    long double twist = twistLineEdit->text().toDouble(&ok);
    if(!ok){
        QMessageBox msg;
        throw(std::runtime_error("The twist should be a single real number."));
    }

    return new balazs::Foliation(lengths, balazs::Permutation(permutationInput), twist);
}















NewFoliationRandomWidget::NewFoliationRandomWidget(QWidget *parent) :
    PreviewPageFoliation(parent)
{
    numIntervalLabel = new QLabel(tr("Number of intervals:"));
    numIntervalSpinBox = new QSpinBox;
    numIntervalSpinBox->setMinimum(3);
    numIntervalSpinBox->setValue(6);

    connect(numIntervalSpinBox, SIGNAL(valueChanged(int)), this, SLOT(disableOkButton(int)));

    QHBoxLayout* rowLayout = new QHBoxLayout;
    rowLayout->addWidget(numIntervalLabel);
    rowLayout->addWidget(numIntervalSpinBox);
    rowLayout->addStretch(1);

    insertOnTop(rowLayout);
    helpButton->hide();
}



void NewFoliationRandomWidget::disableOkButton(int)
{
    okButton->setEnabled(false);
}

balazs::Foliation *NewFoliationRandomWidget::createFoliation()
{
    return new balazs::Foliation(numIntervalSpinBox->value(), balazs::random_tag());
}







NewFoliationAYWidget::NewFoliationAYWidget(QWidget *parent) :
    PreviewPageFoliation(parent)
{
    genusLabel = new QLabel(tr("Genus of the foliation:"));
    genusSpinBox = new QSpinBox;
    genusSpinBox->setMinimum(3);
    genusSpinBox->setValue(3);

    QHBoxLayout* genusLayout = new QHBoxLayout;
    genusLayout->addWidget(genusLabel);
    genusLayout->addWidget(genusSpinBox);
    genusLayout->addStretch(1);

    connect(genusSpinBox, SIGNAL(valueChanged(int)), this, SLOT(disableOkButton(int)));

    insertOnTop(genusLayout);
    helpButton->hide();
}

void NewFoliationAYWidget::disableOkButton(int)
{
    okButton->setEnabled(false);
}

QString NewFoliationAYWidget::helpText()
{
    return tr("");
}

balazs::Foliation *NewFoliationAYWidget::createFoliation()
{
    return new balazs::Foliation(genusSpinBox->value(), balazs::arnoux_yoccoz_tag());
}











NewFoliationRP2WeighedTreeWidget::NewFoliationRP2WeighedTreeWidget(QWidget *parent) :
    PreviewPageFoliationRP2(parent)
{
    weighedTreeLabel = new QLabel(tr("Weighed Tree defining sequence:"));
    weighedTreeLineEdit = new QLineEdit;

    QHBoxLayout* rowLayout = new QHBoxLayout;
    rowLayout->addWidget(weighedTreeLabel);
    rowLayout->addWidget(weighedTreeLineEdit);

    connect(weighedTreeLineEdit, SIGNAL(textEdited(QString)), this, SLOT(disableOkButton(QString)));

    insertOnTop(rowLayout);
}

void NewFoliationRP2WeighedTreeWidget::disableOkButton(QString)
{
    okButton->setEnabled(false);
}

QString NewFoliationRP2WeighedTreeWidget::helpText()
{
    return tr("Weighed trees can be specified by a sequence of non-negative numbers. "
              "Pick a root for the tree, and draw it \"hanging down\" from the root. "
              "First, enter the weights of the edges hanging down from root from left to right, "
              "then enter 0. Proceed to the next generation, enter the weights hanging from the "
              "first vertex in the generation and enter 0, and so on until all weights are entered.\n"
              "E.g. the sequence 0.1 0.2 0.3 0 0.1 0.2 0.3 0 0.5 0.6 0 0.9 0.5 encodes a weighed "
              "tree with 11 vertices, 7 of them are leaves, 3 vertices are of degree 3, "
              "one vertex is of degree 4.");
}

balazs::FoliationRP2 *NewFoliationRP2WeighedTreeWidget::createFoliation()
{
    QStringList stringList = weighedTreeLineEdit->text().split(" ", QString::SkipEmptyParts);

    bool ok;

    std::vector<long double> weights;
    for(QString s : stringList){
        weights.push_back(s.toDouble(&ok));
        if(!ok){
            throw(std::runtime_error("The weights should be real numbers."));
        }
    }

    return new balazs::FoliationRP2(balazs::WeighedTree(weights));
}













NewFoliationRP2RandomWidget::NewFoliationRP2RandomWidget(QWidget *parent) :
    PreviewPageFoliationRP2(parent)
{
    numEdgesLabel = new QLabel(tr("Number of edges of weighed tree:"));
    numEdgesSpinBox = new QSpinBox;
    numEdgesSpinBox->setMinimum(3);

    QHBoxLayout* rowLayout = new QHBoxLayout;
    rowLayout->addWidget(numEdgesLabel);
    rowLayout->addWidget(numEdgesSpinBox);
    rowLayout->addStretch(1);

    connect(numEdgesSpinBox, SIGNAL(valueChanged(int)), this, SLOT(disableOkButton(int)));

    insertOnTop(rowLayout);
}

void NewFoliationRP2RandomWidget::disableOkButton(int)
{
    okButton->setEnabled(false);
}

QString NewFoliationRP2RandomWidget::helpText()
{
    return tr("A random foliation on RP2 is constructed from a random Weighed Tree. "
              "Specify the number of edges of the Weighed Tree.");
}

balazs::FoliationRP2 *NewFoliationRP2RandomWidget::createFoliation()
{
    return new balazs::FoliationRP2(numEdgesSpinBox->value());
}














NewFoliationRP2AYWidget::NewFoliationRP2AYWidget(QWidget *parent) :
    PreviewPageFoliationRP2(parent)
{
    instructions = new QLabel(tr("The classical Arnoux-Yoccoz foliation on the projective plane, which lifts to a "
                                 "foliation on the genus 3 orientable surface:"));
    instructions->setWordWrap(true);
    QHBoxLayout* rowLayout = new QHBoxLayout;
    rowLayout->addWidget(instructions);

    insertOnTop(rowLayout);
    helpButton->hide();

    onPreviewButtomClicked(); // display the foliation right away
}

QString NewFoliationRP2AYWidget::helpText()
{
    return tr("");
}

balazs::FoliationRP2 *NewFoliationRP2AYWidget::createFoliation()
{
    return new balazs::FoliationRP2();
}







NewFoliationSphereDiskWidget::NewFoliationSphereDiskWidget(QWidget *parent) :
    PreviewPageFoliationSphere(parent)
{
}

void NewFoliationSphereDiskWidget::disableOkButton()
{
}

QString NewFoliationSphereDiskWidget::helpText()
{
    return tr("");
}

balazs::FoliationSphere *NewFoliationSphereDiskWidget::createFoliation()
{
}






NewFoliationSphereHLMWidget::NewFoliationSphereHLMWidget(QWidget *parent) :
    PreviewPageFoliationSphere(parent)
{
}

QString NewFoliationSphereHLMWidget::helpText()
{
    return tr("");
}

balazs::FoliationSphere *NewFoliationSphereHLMWidget::createFoliation()
{
}




NewFoliationSphereRandomWidget::NewFoliationSphereRandomWidget(QWidget *parent) :
    PreviewPageFoliationSphere(parent)
{
}

void NewFoliationSphereRandomWidget::disableOkButton()
{
}

QString NewFoliationSphereRandomWidget::helpText()
{
    return tr("");
}

balazs::FoliationSphere *NewFoliationSphereRandomWidget::createFoliation()
{
}






