#ifndef CREATEFOLIATIONWIZARD_H
#define CREATEFOLIATIONWIZARD_H

#include <QWizard>
#include "../fol/Foliation.h"
#include "../fol/FoliationRP2.h"


class QLabel;
class QRadioButton;
class QLineEdit;
class QHBoxLayout;
class QComboBox;
class QSpinBox;
namespace balazs{
    class Foliation;
    class FoliationRP2;
}

class CreateFoliationWizard : public QWizard
{
    Q_OBJECT
public:
    enum { Page_Intro,
           Page_Foliation, Page_Foliation_IntExchange, Page_Foliation_Random, Page_Foliation_AY,
           Page_FoliationRP2, Page_FoliationRP2_WeighedTree, Page_FoliationRP2_AY, Page_FoliationRP2_Random,
           Page_FoliationSphere, Page_FoliationSphere_Random,
           };

    explicit CreateFoliationWizard(QWidget *parent = 0);
    
signals:
    
public slots:
    void fitContents(int) { adjustSize(); }
};


//-----------//
// Intropage //
//-----------//

class IntroPage : public QWizardPage
{
    Q_OBJECT

public:
    IntroPage(QWidget *parent = 0);
    int nextId() const;

private:
    QLabel *instructions;


    QRadioButton *foliationButton;
    QRadioButton *foliationRP2Button;
    QRadioButton *foliationSphereButton;

    QComboBox *genusComboBox;
};







//---------------//
// FoliationPage //
//---------------//

class FoliationPage : public QWizardPage
{
    Q_OBJECT

public:
    FoliationPage(QWidget *parent = 0);
    int nextId() const;
private:
    QLabel *instructions;

    QRadioButton *randomButton;
    QRadioButton *intExchangeButton;
    QRadioButton *arnouxYoccoxButton;
};




//--------------------------//
// FoliationIntExchangePage //
//--------------------------//

class FoliationIntExchangePage : public QWizardPage
{
    Q_OBJECT

public:
    FoliationIntExchangePage(QWidget *parent = 0);
    int nextId() const { return -1; }
    bool validatePage();

signals:
    void foliation(balazs::Foliation newFoliation);

public slots:


private:
    QLabel* instructions;
    QLabel *permutationInstructions;
    QLabel *lengthsInstructions;
    QLabel *twistInstructions;
    QLabel* permutationLabel;
    QLabel* lengthsLabel;
    QLabel* twistLabel;
    QLineEdit* permutationLineEdit;
    QLineEdit* lengthsLineEdit;
    QLineEdit* twistLineEdit;
    QComboBox* numIntervalsComboBox;
};





//---------------------//
// FoliationRandomPage //
//---------------------//

class FoliationRandomPage : public QWizardPage
{
    Q_OBJECT

public:
    FoliationRandomPage(QWidget *parent = 0);
    int nextId() const { return -1; }
    bool validatePage();

signals:
    void foliation(balazs::Foliation newFoliation);

private:
    QLabel* instructions;
    QSpinBox* numIntervalSpinBox;
};





//-----------------//
// FoliationAYPage //
//-----------------//

class FoliationAYPage : public QWizardPage
{
    Q_OBJECT

public:
    FoliationAYPage(QWidget *parent = 0);
    int nextId() const { return -1; }
    bool validatePage();

signals:
    void foliation(balazs::Foliation newFoliation);

private:
    QLabel* instructions;
    QSpinBox* genusSpinBox;
};





//------------------//
// FoliationRP2Page //
//------------------//

class FoliationRP2Page : public QWizardPage
{
    Q_OBJECT

public:
    FoliationRP2Page(QWidget *parent = 0);
    int nextId() const;
private:
    QLabel *instructions;

    QRadioButton *randomButton;
    QRadioButton *weighedTreeButton;
    QRadioButton *arnouxYoccoxButton;
};


//-----------------------------//
// FoliationRP2WeighedTreePage //
//-----------------------------//

class FoliationRP2WeighedTreePage : public QWizardPage
{
    Q_OBJECT

public:
    FoliationRP2WeighedTreePage(QWidget *parent = 0);
    int nextId() const { return -1; }
    bool validatePage();
signals:
    void foliationRP2(balazs::FoliationRP2 newFoliationRP2);
private:
    QLabel* instructions;
    QLabel* weighedTreeLabel;
    QLineEdit* weighedTreeLineEdit;
};



//------------------------//
// FoliationRP2RandomPage //
//------------------------//

class FoliationRP2RandomPage : public QWizardPage
{
    Q_OBJECT

public:
    FoliationRP2RandomPage(QWidget *parent = 0);
    int nextId() const { return -1; }
    bool validatePage();
signals:
    void foliationRP2(balazs::FoliationRP2 newFoliationRP2);
private:
    QLabel* instructions;
    QSpinBox* numEdgesSpinBox;
};




//--------------------//
// FoliationRP2AYPage //
//--------------------//

class FoliationRP2AYPage : public QWizardPage
{
    Q_OBJECT

public:
    FoliationRP2AYPage(QWidget *parent = 0);
    int nextId() const { return -1; }
    bool validatePage();
signals:
    void foliationRP2(balazs::FoliationRP2 newFoliationRP2);

private:
    QLabel* instructions;

};






//---------------------//
// FoliationSpherePage //
//---------------------//

class FoliationSpherePage : public QWizardPage
{
    Q_OBJECT

public:
    FoliationSpherePage(QWidget *parent = 0);
   // int nextId() const;

private:

};















#endif // CREATEFOLIATIONWIZARD_H
