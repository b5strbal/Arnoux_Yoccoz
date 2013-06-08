#ifndef PARAMETERSWIDGET_H
#define PARAMETERSWIDGET_H

#include <QWidget>
#include "windows/drawing/EmptyDrawing.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QMessageBox>

class QLabel;
class QSpinBox;
class QLineEdit;

namespace balazs{
    class Foliation;
    class FoliationRP2;
    class FoliationSphere;
}





template <class FoliationType>
class PreviewPage : public QWidget
{
public:
    explicit PreviewPage(QWidget *parent = 0);

protected:
    void onHelpButtonClicked();
    void onOkButtonClicked();
    void onPreviewButtomClicked();

private:
    virtual QString helpText() = 0;
    virtual FoliationType* createFoliation() = 0;
    virtual QWidget* createDrawing(const FoliationType*) = 0;
    virtual void emitFoliation(const FoliationType*) = 0;

    QPushButton* helpButton;
    QPushButton* previewButton;
    QPushButton* okButton;

    QStackedWidget* previewStackedWidget;

    FoliationType* pFoliation;
};



template <class FoliationType>
PreviewPage<FoliationType>::PreviewPage(QWidget *parent) :
    QWidget(parent),
    pFoliation(nullptr)
{
    helpButton = new QPushButton(tr("Help"));
    previewButton = new QPushButton(tr("Preview"));
    okButton = new QPushButton(tr("OK"));
    okButton->setEnabled(false);

    previewStackedWidget = new QStackedWidget;
    previewStackedWidget->addWidget(new EmptyDrawing);
    previewStackedWidget->setCurrentIndex(0);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(helpButton);
    buttonLayout->addSpacing(20);
    buttonLayout->addWidget(previewButton);
    buttonLayout->addStretch(1);

    QHBoxLayout* okLayout = new QHBoxLayout;
    okLayout->addStretch(1);
    okLayout->addWidget(okButton);

 //   connect(helpButton, SIGNAL(clicked()), this, SLOT(onHelpButtonClicked()));
 //   connect(previewButton, SIGNAL(clicked()), this, SLOT(onPreviewButtomClicked()));
 //   connect(okButton, SIGNAL(clicked()), this, SLOT(onOkButtonClicked()));

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(previewStackedWidget);
    mainLayout->addLayout(okLayout);
    mainLayout->addStretch(1);

    setLayout(mainLayout);
}

template <class FoliationType>
void PreviewPage<FoliationType>::onHelpButtonClicked()
{
    QMessageBox msg;
    msg.setText(helpText());
    msg.exec();
}

template <class FoliationType>
void PreviewPage<FoliationType>::onOkButtonClicked()
{
    emitFoliation(pFoliation);
    pFoliation = nullptr;
    QWidget* lastWidget = previewStackedWidget->widget(1);
    previewStackedWidget->removeWidget(lastWidget);
    delete lastWidget;
    okButton->setEnabled(false);
}


template <class FoliationType>
void PreviewPage<FoliationType>::onPreviewButtomClicked()
{
    try{
        FoliationType* newFoliation = createFoliation();
        if(pFoliation){
            QWidget* lastWidget = previewStackedWidget->widget(1);
            previewStackedWidget->removeWidget(lastWidget);
            delete lastWidget;
            delete pFoliation;
        }
        pFoliation = newFoliation;
        previewStackedWidget->addWidget(createDrawing(pFoliation));
        previewStackedWidget->setCurrentIndex(1);
        okButton->setEnabled(true);
    }
    catch (const std::exception& e){
        QMessageBox msg;
        msg.setText(tr(e.what()));
        msg.exec();
    }
}







class NewFoliationIntExchangeWidget2 : public PreviewPage<balazs::Foliation>
{
    Q_OBJECT
public:
    explicit NewFoliationIntExchangeWidget2(QWidget *parent = 0);
signals:
    void foliation(balazs::Foliation*);
private slots:
    void onHelpButtonClickedSlot();
    void onOkButtonClickedSlot();
    void onPreviewButtomClickedSlot();
    void disableOkButton(QString);
private:
    void updateOkButton();

    QLabel* permutationLabel;
    QLabel* lengthsLabel;
    QLabel* twistLabel;
    QLineEdit* permutationLineEdit;
    QLineEdit* lengthsLineEdit;
    QLineEdit* twistLineEdit;

    QPushButton* helpButton;
    QPushButton* previewButton;
    QPushButton* okButton;

    QStackedWidget* previewStackedWidget;

    balazs::Foliation* pFoliation;
};






class NewFoliationIntExchangeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NewFoliationIntExchangeWidget(QWidget *parent = 0);
signals:
    void foliation(balazs::Foliation*);
private slots:
    void onHelpButtonClicked();
    void onOkButtonClicked();
    void onPreviewButtomClicked();
    void disableOkButton(QString);
private:
    void updateOkButton();

    QLabel* permutationLabel;
    QLabel* lengthsLabel;
    QLabel* twistLabel;
    QLineEdit* permutationLineEdit;
    QLineEdit* lengthsLineEdit;
    QLineEdit* twistLineEdit;

    QPushButton* helpButton;
    QPushButton* previewButton;
    QPushButton* okButton;

    QStackedWidget* previewStackedWidget;

    balazs::Foliation* pFoliation;
};




class NewFoliationRandomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NewFoliationRandomWidget(QWidget *parent = 0);
signals:
    void foliation(balazs::Foliation*);
private slots:
    void onGenerateButtonClicked();
    void onOkButtonClicked();
    void disableOkButton(int);
private:
    QLabel* instructions;
    QSpinBox* numIntervalSpinBox;
    QPushButton* generateButton;
    QPushButton* okButton;

    QStackedWidget* previewStackedWidget;

    balazs::Foliation* pFoliation;
};




class NewFoliationAYWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NewFoliationAYWidget(QWidget *parent = 0);
signals:
    void foliation(balazs::Foliation*);
private:
    QLabel* instructions;
    QSpinBox* genusSpinBox;
};



class NewFoliationRP2WeighedTreeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NewFoliationRP2WeighedTreeWidget(QWidget *parent = 0);
signals:
    void foliationRP2(balazs::FoliationRP2*);
private:
    QLabel* instructions;
    QLabel* weighedTreeLabel;
    QLineEdit* weighedTreeLineEdit;
};


class NewFoliationRP2RandomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NewFoliationRP2RandomWidget(QWidget *parent = 0);
signals:
    void foliationRP2(balazs::FoliationRP2*);
private:
    QLabel* instructions;
    QSpinBox* numEdgesSpinBox;
};


class NewFoliationRP2AYWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NewFoliationRP2AYWidget(QWidget *parent = 0);
signals:
    void foliationRP2(balazs::FoliationRP2*);
private:
    QLabel* instructions;
};


class NewFoliationSphereDiskWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NewFoliationSphereDiskWidget(QWidget *parent = 0);
signals:
    void foliationSphere(balazs::FoliationSphere*);
private:
    QLabel* instructions;
};



class NewFoliationSphereHLMWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NewFoliationSphereHLMWidget(QWidget *parent = 0);
signals:
    void foliationSphere(balazs::FoliationSphere*);
private:
    QLabel* instructions;
};



class NewFoliationSphereRandomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NewFoliationSphereRandomWidget(QWidget *parent = 0);
signals:
    void foliationSphere(balazs::FoliationSphere*);
private:
    QLabel* instructions;
};

#endif // PARAMETERSWIDGET_H
