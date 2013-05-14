#include "CreateFoliationWizard.h"
#include "DocumentationWindow.h"
#include "MainWindow.h"
#include "../fol/Foliation.h"
#include "../fol/FoliationRP2.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QComboBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QMessageBox>
#include <QSpinBox>

CreateFoliationWizard::CreateFoliationWizard(QWidget *parent) :
    QWizard(parent)
{
    setWindowTitle(tr("Create a foliation"));
    setPage(Page_Intro, new IntroPage);
    setPage(Page_Foliation, new FoliationPage);
    setPage(Page_Foliation_IntExchange, new FoliationIntExchangePage);
    setPage(Page_Foliation_AY, new FoliationAYPage);
    setPage(Page_Foliation_Random, new FoliationRandomPage);
    setPage(Page_FoliationRP2, new FoliationRP2Page);
    setPage(Page_FoliationRP2_WeighedTree, new FoliationRP2WeighedTreePage);
    setPage(Page_FoliationRP2_AY, new FoliationRP2AYPage);
    setPage(Page_FoliationRP2_Random, new FoliationRP2RandomPage);
    setPage(Page_FoliationSphere, new FoliationSpherePage);

    setStartId(Page_Intro);
    connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(fitContents(int)));


}







//-----------//
// Intropage //
//-----------//

IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Choose a surface"));
    instructions = new QLabel(tr("Which surface should be the underlying surface of the measured foliation?"));
    instructions->setWordWrap(true);

    foliationButton = new QRadioButton(tr("Orientable closed surface with positive genus"));
    foliationRP2Button = new QRadioButton(tr("Projective plane"));
    foliationSphereButton = new QRadioButton(tr("Sphere"));
    foliationButton->setChecked(true);
    foliationSphereButton->setCheckable(false);


    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(instructions);
    mainLayout->addWidget(foliationButton);
    mainLayout->addWidget(foliationRP2Button);
    mainLayout->addWidget(foliationSphereButton);

    setLayout(mainLayout);
}


int IntroPage::nextId() const
{
    if(foliationRP2Button->isChecked())
        return CreateFoliationWizard::Page_FoliationRP2;
    if(foliationButton->isChecked())
        return CreateFoliationWizard::Page_Foliation;
    return CreateFoliationWizard::Page_FoliationSphere;
}










//---------------//
// FoliationPage //
//---------------//

FoliationPage::FoliationPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Creating a foliation on an orientable surface"));
    instructions = new QLabel(tr("Choose the mode of construction:"));

    intExchangeButton = new QRadioButton(tr("Specify a twisted interval exchange map"));
    arnouxYoccoxButton = new QRadioButton(tr("An Arnoux-Yoccox foliation"));
    randomButton = new QRadioButton(tr("Random foliation"));
    intExchangeButton->setChecked(true);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(instructions);
    mainLayout->addWidget(intExchangeButton);
    mainLayout->addWidget(arnouxYoccoxButton);
    mainLayout->addWidget(randomButton);

    setLayout(mainLayout);
}

int FoliationPage::nextId() const
{
    if(intExchangeButton->isChecked())
        return CreateFoliationWizard::Page_Foliation_IntExchange;
    if(arnouxYoccoxButton->isChecked())
        return CreateFoliationWizard::Page_Foliation_AY;
    return CreateFoliationWizard::Page_Foliation_Random;
}




//--------------------------//
// FoliationIntExchangePage //
//--------------------------//

FoliationIntExchangePage::FoliationIntExchangePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Specify a twisted interval exchange map"));
    instructions = new QLabel(tr("A twisted interval exchange map on the unit interval is defined by the "
                                 "lengths of the intervals, "
                                 "a permutation on the same number of elements, and the twist."));
    instructions->setWordWrap(true);

    permutationInstructions = new QLabel(tr("You can specify a permutation on n elements as a permutation of "
                                            "0, 1, ..., n-1, spearated by spaces."));
    permutationInstructions->setWordWrap(true);

    lengthsInstructions = new QLabel(tr("To specify the lengths of the intervals, list n positive real numbers "
                                        "separated by spaces. The sum of the lengths doesn't have to be 1, but "
                                        "they will be normalized to get an interval exchange of the unit interval."));
    lengthsInstructions->setWordWrap(true);

    twistInstructions = new QLabel(tr("The twist may be any real number, but it will be taken modulo 1."));
    twistInstructions->setWordWrap(true);


    permutationLabel = new QLabel(tr("Permutation (e.g. 3 2 1 0):"));
    lengthsLabel = new QLabel(tr("Lengths (e.g. 1.2 0.3 0.43 0.12):"));
    twistLabel = new QLabel(tr("Twist (e.g. 0.5):"));
    permutationLineEdit = new QLineEdit;
    lengthsLineEdit = new QLineEdit;
    twistLineEdit = new QLineEdit;

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(instructions, 0, 0, 1, 2);
    mainLayout->addWidget(permutationInstructions, 1, 0, 1, 2);
    mainLayout->addWidget(permutationLabel, 2, 0);
    mainLayout->addWidget(permutationLineEdit, 2, 1);
    mainLayout->addWidget(lengthsInstructions, 3, 0, 1, 2);
    mainLayout->addWidget(lengthsLabel, 4, 0);
    mainLayout->addWidget(lengthsLineEdit, 4, 1);
    mainLayout->addWidget(twistInstructions, 5, 0, 1, 2);
    mainLayout->addWidget(twistLabel, 6, 0);
    mainLayout->addWidget(twistLineEdit, 6, 1);

    setLayout(mainLayout);
}

bool FoliationIntExchangePage::validatePage(){
    QStringList permutationStringList = permutationLineEdit->text().split(" ", QString::SkipEmptyParts);
    QStringList lengthsStringList = lengthsLineEdit->text().split(" ", QString::SkipEmptyParts);

    bool ok;

    std::vector<balazs::floating_point_type> lengths;
    for(QString s : lengthsStringList){
        lengths.push_back(s.toDouble(&ok));
        if(!ok){
            QMessageBox msg;
            msg.setText(tr("The lengths should be real numbers, you MORON!"));
            msg.exec();
            return false;
        }
    }

    std::vector<std::size_t> permutationInput;
    for(QString s : permutationStringList){
        permutationInput.push_back(s.toUInt(&ok));
        if(!ok){
            QMessageBox msg;
            msg.setText(tr("The numbers in the permutation list should be non-negative integers."));
            msg.exec();
            return false;
        }
    }

    balazs::floating_point_type twist = twistLineEdit->text().toDouble(&ok);
    if(!ok){
        QMessageBox msg;
        msg.setText(tr("The twist should be a single real number."));
        msg.exec();
        return false;
    }


    try{
        CreateFoliationWizard* wiz = dynamic_cast<CreateFoliationWizard*>(wizard());
        emit(wiz->foliation(new balazs::Foliation(lengths, balazs::Permutation(permutationInput), twist)));
        return true;
    }
    catch (const std::exception& e){
        QMessageBox msg;
        msg.setText(tr(e.what()));
        msg.exec();
        return false;
    }


}









//---------------------//
// FoliationRandomPage //
//---------------------//


FoliationRandomPage::FoliationRandomPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Random foliation"));
    instructions = new QLabel(tr("How many intervals should the twisted interval exchange permute?"));
    numIntervalSpinBox = new QSpinBox;
    numIntervalSpinBox->setMinimum(3);
    numIntervalSpinBox->setValue(6);

    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(instructions);
    mainLayout->addWidget(numIntervalSpinBox);

    setLayout(mainLayout);

}

bool FoliationRandomPage::validatePage(){
    try{
      //  emit(createRandomFoliation(numIntervalSpinBox->value()));
        return true;
    }
    catch (const std::exception& e){
        QMessageBox msg;
        msg.setText(tr(e.what()));
        msg.exec();
        return false;
    }
}









//-----------------//
// FoliationAYPage //
//-----------------//


FoliationAYPage::FoliationAYPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Creating an Arnoux-Yoccoz foliation"));
    instructions = new QLabel(tr("Genus of the foliation:"));
    genusSpinBox = new QSpinBox;
    genusSpinBox->setMinimum(3);
    genusSpinBox->setValue(3);

    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(instructions);
    mainLayout->addWidget(genusSpinBox);

    setLayout(mainLayout);
}

bool FoliationAYPage::validatePage(){
    try{
        CreateFoliationWizard* wiz = dynamic_cast<CreateFoliationWizard*>(wizard());
        emit(wiz->foliation(new balazs::Foliation(genusSpinBox->value())));
        return true;
    }
    catch (const std::exception& e){
        QMessageBox msg;
        msg.setText(tr(e.what()));
        msg.exec();
        return false;
    }

}




//------------------//
// FoliationRP2Page //
//------------------//

FoliationRP2Page::FoliationRP2Page(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Creating a foliation on the projective plane"));

    instructions = new QLabel(tr("Choose the mode of construction:"));

    weighedTreeButton = new QRadioButton(tr("Specify a weighed tree"));
    arnouxYoccoxButton = new QRadioButton(tr("The Arnoux-Yoccox foliation"));
    randomButton = new QRadioButton(tr("Random foliation"));
    weighedTreeButton->setChecked(true);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(instructions);
    mainLayout->addWidget(weighedTreeButton);
    mainLayout->addWidget(arnouxYoccoxButton);
    mainLayout->addWidget(randomButton);

    setLayout(mainLayout);
}


int FoliationRP2Page::nextId() const
{
    if(weighedTreeButton->isChecked())
        return CreateFoliationWizard::Page_FoliationRP2_WeighedTree;
    if(arnouxYoccoxButton->isChecked())
        return CreateFoliationWizard::Page_FoliationRP2_AY;
    return CreateFoliationWizard::Page_FoliationRP2_Random;
}


//-----------------------------//
// FoliationRP2WeighedTreePage //
//-----------------------------//

FoliationRP2WeighedTreePage::FoliationRP2WeighedTreePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Specify a Weighed Tree"));

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


bool FoliationRP2WeighedTreePage::validatePage(){
    QStringList stringList = weighedTreeLineEdit->text().split(" ", QString::SkipEmptyParts);

    bool ok;

    std::vector<balazs::floating_point_type> weights;
    for(QString s : stringList){
        weights.push_back(s.toDouble(&ok));
        if(!ok){
            QMessageBox msg;
            msg.setText(tr("The weights should be real numbers."));
            msg.exec();
            return false;
        }
    }

    try{
        CreateFoliationWizard* wiz = dynamic_cast<CreateFoliationWizard*>(wizard());
        emit(wiz->foliationRP2(new balazs::FoliationRP2(balazs::WeighedTree(weights))));
        return true;
    }
    catch (const std::exception& e){
        QMessageBox msg;
        msg.setText(tr(e.what()));
        msg.exec();
        return false;
    }
}



//------------------------//
// FoliationRP2RandomPage //
//------------------------//

FoliationRP2RandomPage::FoliationRP2RandomPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Random foliation on RP2"));

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


bool FoliationRP2RandomPage::validatePage(){
    try{
        CreateFoliationWizard* wiz = dynamic_cast<CreateFoliationWizard*>(wizard());
        emit(wiz->foliationRP2(new balazs::FoliationRP2(numEdgesSpinBox->value())));
        return true;
    }
    catch (const std::exception& e){
        QMessageBox msg;
        msg.setText(tr(e.what()));
        msg.exec();
        return false;
    }
}


//--------------------//
// FoliationRP2AYPage //
//--------------------//

FoliationRP2AYPage::FoliationRP2AYPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("The Arnoux-Yoccoz foliation on RP2"));

    instructions = new QLabel(tr("The classical Arnoux-Yoccoz foliation on the projective plane, which lifts to a "
                                 "foliation on the genus 3 orientable surface, will be constructed."), this);
    instructions->setWordWrap(true);
}

bool FoliationRP2AYPage::validatePage(){
    try{
        CreateFoliationWizard* wiz = dynamic_cast<CreateFoliationWizard*>(wizard());
        emit(wiz->foliationRP2(new balazs::FoliationRP2()));
        return true;
    }
    catch (const std::exception& e){
        QMessageBox msg;
        msg.setText(tr(e.what()));
        msg.exec();
        return false;
    }
}







//---------------------//
// FoliationSpherePage //
//---------------------//

FoliationSpherePage::FoliationSpherePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Specify the foliation on the sphere"));
}





