#include "ParameterWidgets.h"
#include "math/Permutation.h"
#include "fol/Foliation.h"
#include "math/WeighedTree.h"
#include "fol/FoliationRP2.h"
#include "windows/drawing/FoliationDrawing.h"

#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QStackedWidget>


void NewFoliationIntExchangeWidget2::onHelpButtonClickedSlot()
{
    onHelpButtonClicked();
}

void NewFoliationIntExchangeWidget2::onOkButtonClickedSlot()
{
    onOkButtonClicked();
}

void NewFoliationIntExchangeWidget2::onPreviewButtomClickedSlot()
{
    onPreviewButtomClicked();
}

void NewFoliationIntExchangeWidget2::disableOkButton(QString)
{
    okButton->setEnabled(false);
}









NewFoliationIntExchangeWidget::NewFoliationIntExchangeWidget(QWidget *parent) :
    QWidget(parent),
    pFoliation(nullptr)
{
    permutationLabel = new QLabel(tr("Permutation (e.g. 3 2 1 0):"));
    lengthsLabel = new QLabel(tr("Lengths (e.g. 1.2 0.3 0.43 0.12):"));
    twistLabel = new QLabel(tr("Twist (e.g. 0.5):"));

    permutationLineEdit = new QLineEdit;
    lengthsLineEdit = new QLineEdit;
    twistLineEdit = new QLineEdit;

    previewStackedWidget = new QStackedWidget;
    previewStackedWidget->addWidget(new FoliationDrawing(nullptr));
    previewStackedWidget->setCurrentIndex(0);

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

    helpButton = new QPushButton(tr("Help"));
    previewButton = new QPushButton(tr("Preview"));
    okButton = new QPushButton(tr("OK"));
    okButton->setEnabled(false);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(helpButton);
    buttonLayout->addSpacing(20);
    buttonLayout->addWidget(previewButton);
    buttonLayout->addStretch(1);

    QHBoxLayout* okLayout = new QHBoxLayout;
    okLayout->addStretch(1);
    okLayout->addWidget(okButton);

    connect(helpButton, SIGNAL(clicked()), this, SLOT(onHelpButtonClicked()));
    connect(previewButton, SIGNAL(clicked()), this, SLOT(onPreviewButtomClicked()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(onOkButtonClicked()));

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(permutationLayout);
    mainLayout->addLayout(lengthsLayout);
    mainLayout->addLayout(twistLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(previewStackedWidget);
    mainLayout->addLayout(okLayout);
    mainLayout->addStretch(1);

    setLayout(mainLayout);
}





void NewFoliationIntExchangeWidget::onHelpButtonClicked()
{
    QMessageBox msg;
    QString s;
    s = tr("A twisted interval exchange map on the unit interval is defined by the lengths of the intervals, "
         "a permutation on the same number of elements, and the twist.\n\n"
         "You can specify a permutation on n elements as a permutation of "
         "0, 1, ..., n-1, spearated by spaces.\n\n"
         "To specify the lengths of the intervals, list n positive real numbers "
         "separated by spaces. The sum of the lengths doesn't have to be 1, but "
         "they will be normalized to get an interval exchange of the unit interval.\n\n"
         "The twist may be any real number, will be normalized by the same factor as the lengths and taken modulo 1.");

    msg.setText(s);
    msg.exec();
}

void NewFoliationIntExchangeWidget::onOkButtonClicked()
{
    emit(foliation(pFoliation));
    pFoliation = nullptr;
    QWidget* lastWidget = previewStackedWidget->widget(1);
    previewStackedWidget->removeWidget(lastWidget);
    delete lastWidget;
    okButton->setEnabled(false);
}

void NewFoliationIntExchangeWidget::onPreviewButtomClicked()
{
    QStringList permutationStringList = permutationLineEdit->text().split(" ", QString::SkipEmptyParts);
    QStringList lengthsStringList = lengthsLineEdit->text().split(" ", QString::SkipEmptyParts);

    bool ok;

    std::vector<long double> lengths;
    for(QString s : lengthsStringList){
        lengths.push_back(s.toDouble(&ok));
        if(!ok){
            QMessageBox msg;
            msg.setText(tr("The lengths should be real numbers, you MORON!"));
            msg.exec();
            return;
        }
    }

    std::vector<std::size_t> permutationInput;
    for(QString s : permutationStringList){
        permutationInput.push_back(s.toUInt(&ok));
        if(!ok){
            QMessageBox msg;
            msg.setText(tr("The numbers in the permutation list should be non-negative integers."));
            msg.exec();
            return;
        }
    }

    long double twist = twistLineEdit->text().toDouble(&ok);
    if(!ok){
        QMessageBox msg;
        msg.setText(tr("The twist should be a single real number."));
        msg.exec();
        return;
    }


    try{
        balazs::Foliation* newFoliation = new balazs::Foliation(lengths, balazs::Permutation(permutationInput), twist);
        if(pFoliation){
            QWidget* lastWidget = previewStackedWidget->widget(1);
            previewStackedWidget->removeWidget(lastWidget);
            delete lastWidget;
            delete pFoliation;
        }
        pFoliation = newFoliation;
        previewStackedWidget->addWidget(new FoliationDrawing(newFoliation));
        previewStackedWidget->setCurrentIndex(1);
        okButton->setEnabled(true);
    }
    catch (const std::exception& e){
        QMessageBox msg;
        msg.setText(tr(e.what()));
        msg.exec();
    }
}

void NewFoliationIntExchangeWidget::disableOkButton(QString)
{
    okButton->setEnabled(false);
}
















NewFoliationRandomWidget::NewFoliationRandomWidget(QWidget *parent) :
    QWidget(parent)
{
    instructions = new QLabel(tr("Number of intervals:"));
    numIntervalSpinBox = new QSpinBox;
    numIntervalSpinBox->setMinimum(3);
    numIntervalSpinBox->setValue(6);

    generateButton = new QPushButton(tr("Generate"));

    previewStackedWidget = new QStackedWidget;
    previewStackedWidget->addWidget(new FoliationDrawing(nullptr));
    previewStackedWidget->setCurrentIndex(0);

    okButton = new QPushButton(tr("OK"));
    okButton->setEnabled(false);

    connect(generateButton, SIGNAL(clicked()), this, SLOT(onGenerateButtonClicked()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(onOkButtonClicked()));
    connect(numIntervalSpinBox, SIGNAL(valueChanged(int)), this, SLOT(disableOkButton(int)));


    QHBoxLayout* rowLayout = new QHBoxLayout;
    rowLayout->addWidget(instructions);
    rowLayout->addWidget(numIntervalSpinBox);
    rowLayout->addSpacing(10);
    rowLayout->addWidget(generateButton);
    rowLayout->addStretch(1);

    QHBoxLayout* bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch(1);
    bottomLayout->addWidget(okButton);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(rowLayout);
    mainLayout->addWidget(previewStackedWidget);
    mainLayout->addLayout(bottomLayout);
    mainLayout->addStretch(1);

    setLayout(mainLayout);
}


void NewFoliationRandomWidget::onGenerateButtonClicked()
{
    try{
        balazs::Foliation* newFoliation = new balazs::Foliation(numIntervalSpinBox->value(), balazs::random_tag());
        if(pFoliation){
            QWidget* lastWidget = previewStackedWidget->widget(1);
            previewStackedWidget->removeWidget(lastWidget);
            delete lastWidget;
            delete pFoliation;
        }
        pFoliation = newFoliation;
        previewStackedWidget->addWidget(new FoliationDrawing(newFoliation));
        previewStackedWidget->setCurrentIndex(1);
        okButton->setEnabled(true);
    }
    catch (const std::exception& e){
        QMessageBox msg;
        msg.setText(tr(e.what()));
        msg.exec();
    }
}

void NewFoliationRandomWidget::onOkButtonClicked()
{
    emit(foliation(pFoliation));
    pFoliation = nullptr;
    QWidget* lastWidget = previewStackedWidget->widget(1);
    previewStackedWidget->removeWidget(lastWidget);
    delete lastWidget;
    okButton->setEnabled(false);
}

void NewFoliationRandomWidget::disableOkButton(int)
{
    okButton->setEnabled(false);
}







NewFoliationAYWidget::NewFoliationAYWidget(QWidget *parent) :
    QWidget(parent)
{
    instructions = new QLabel(tr("Genus of the foliation:"));
    genusSpinBox = new QSpinBox;
    genusSpinBox->setMinimum(3);
    genusSpinBox->setValue(3);

    QHBoxLayout* genusLayout = new QHBoxLayout;
    genusLayout->addWidget(instructions);
    genusLayout->addWidget(genusSpinBox);
    genusLayout->addStretch(1);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(genusLayout);
    mainLayout->addStretch(1);

    setLayout(mainLayout);
}

//bool NewFoliationAYWidget::validate()
//{
//    try{
//        emit(foliation(new balazs::Foliation(genusSpinBox->value())));
//        return true;
//    }
//    catch (const std::exception& e){
//        QMessageBox msg;
//        msg.setText(tr(e.what()));
//        msg.exec();
//        return false;
//    }
//}







NewFoliationRP2WeighedTreeWidget::NewFoliationRP2WeighedTreeWidget(QWidget *parent) :
    QWidget(parent)
{
    instructions = new QLabel(tr("Weighed trees can be specified by a sequence of non-negative numbers. "
                                 "Pick a root for the tree, and draw it \"hanging down\" from the root. "
                                 "First, enter the weights of the edges hanging down from root from left to right, "
                                 "then enter 0. Proceed to the next generation, enter the weights hanging from the "
                                 "first vertex in the generation and enter 0, and so on until all weights are entered.\n"
                                 "E.g. the sequence 0.1 0.2 0.3 0 0.1 0.2 0.3 0 0.5 0.6 0 0.9 0.5 encodes a weighed "
                                 "tree with 11 vertices, 7 of them are leaves, 3 vertices are of degree 3, "
                                 "one vertex is of degree 4."));
    instructions->setWordWrap(true);
    weighedTreeLabel = new QLabel(tr("Weighed Tree defining sequence:"));
    weighedTreeLineEdit = new QLineEdit;

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(instructions);
    mainLayout->addWidget(weighedTreeLabel);
    mainLayout->addWidget(weighedTreeLineEdit);

    setLayout(mainLayout);
}

//bool NewFoliationRP2WeighedTreeWidget::validate()
//{
//    QStringList stringList = weighedTreeLineEdit->text().split(" ", QString::SkipEmptyParts);

//    bool ok;

//    std::vector<long double> weights;
//    for(QString s : stringList){
//        weights.push_back(s.toDouble(&ok));
//        if(!ok){
//            QMessageBox msg;
//            msg.setText(tr("The weights should be real numbers."));
//            msg.exec();
//            return false;
//        }
//    }

//    try{
//        emit(foliationRP2(new balazs::FoliationRP2(balazs::WeighedTree(weights))));
//        return true;
//    }
//    catch (const std::exception& e){
//        QMessageBox msg;
//        msg.setText(tr(e.what()));
//        msg.exec();
//        return false;
//    }
//}











NewFoliationRP2RandomWidget::NewFoliationRP2RandomWidget(QWidget *parent) :
    QWidget(parent)
{
    instructions = new QLabel(tr("A random foliation on RP2 is constructed from a random Weighed Tree. "
                                 "Specify the number of edges of the Weighed Tree."));
    instructions->setWordWrap(true);
    instructions->setAlignment(Qt::AlignJustify);

    numEdgesSpinBox = new QSpinBox;
    numEdgesSpinBox->setMinimum(3);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(instructions);
    mainLayout->addWidget(numEdgesSpinBox);
    setLayout(mainLayout);
}

//bool NewFoliationRP2RandomWidget::validate()
//{
//    try{
//        emit(foliationRP2(new balazs::FoliationRP2(numEdgesSpinBox->value())));
//        return true;
//    }
//    catch (const std::exception& e){
//        QMessageBox msg;
//        msg.setText(tr(e.what()));
//        msg.exec();
//        return false;
//    }
//}













NewFoliationRP2AYWidget::NewFoliationRP2AYWidget(QWidget *parent) :
    QWidget(parent)
{
    instructions = new QLabel(tr("The classical Arnoux-Yoccoz foliation on the projective plane, which lifts to a "
                                 "foliation on the genus 3 orientable surface, will be constructed."), this);
    instructions->setWordWrap(true);
}

//bool NewFoliationRP2AYWidget::validate()
//{
//    try{
//        emit(foliationRP2(new balazs::FoliationRP2()));
//        return true;
//    }
//    catch (const std::exception& e){
//        QMessageBox msg;
//        msg.setText(tr(e.what()));
//        msg.exec();
//        return false;
//    }
//}

NewFoliationSphereDiskWidget::NewFoliationSphereDiskWidget(QWidget *parent) :
    QWidget(parent)
{
}






NewFoliationSphereHLMWidget::NewFoliationSphereHLMWidget(QWidget *parent) :
    QWidget(parent)
{
}




NewFoliationSphereRandomWidget::NewFoliationSphereRandomWidget(QWidget *parent) :
    QWidget(parent)
{
}





