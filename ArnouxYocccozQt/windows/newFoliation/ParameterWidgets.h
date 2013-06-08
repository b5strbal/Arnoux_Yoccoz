#ifndef PARAMETERSWIDGET_H
#define PARAMETERSWIDGET_H

#include <QWidget>
#include "windows/drawing/EmptyDrawing.h"
#include "fol/Foliation.h"
#include "fol/FoliationSphere.h"
#include "fol/FoliationRP2.h"
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
    void insertOnTop(QLayout *layout);

    QPushButton* helpButton;
    QPushButton* previewButton;
    QPushButton* okButton;
private:
    virtual QString helpText() = 0;
    virtual FoliationType* createFoliation() = 0;
    virtual QWidget* createDrawing(const FoliationType&) = 0;
    virtual void emitFoliation(FoliationType*) = 0;

    QStackedWidget* previewStackedWidget;
    QVBoxLayout* mainLayout;

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

    mainLayout = new QVBoxLayout;
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
        previewStackedWidget->addWidget(createDrawing(*pFoliation));
        previewStackedWidget->setCurrentIndex(1);
        okButton->setEnabled(true);
    }
    catch (const std::exception& e){
        QMessageBox msg;
        msg.setText(tr(e.what()));
        msg.exec();
    }
}

template <class FoliationType>
void PreviewPage<FoliationType>::insertOnTop(QLayout *layout)
{
    mainLayout->insertLayout(0, layout);
}






class PreviewPageFoliation : public PreviewPage<balazs::Foliation>
{
    Q_OBJECT
public:
    explicit PreviewPageFoliation(QWidget *parent = 0);
signals:
    void foliation(balazs::Foliation*);
private slots:
    void onHelpButtonClickedSlot() { onHelpButtonClicked(); }
    void onOkButtonClickedSlot() { onOkButtonClicked(); }
    void onPreviewButtomClickedSlot() { onPreviewButtomClicked(); }
private:
    virtual void emitFoliation(balazs::Foliation* fol) { emit(foliation(fol)); }
    virtual QWidget* createDrawing(const balazs::Foliation& fol);
};





class PreviewPageFoliationSphere : public PreviewPage<balazs::FoliationSphere>
{
    Q_OBJECT
public:
    explicit PreviewPageFoliationSphere(QWidget *parent = 0);
signals:
    void foliationSphere(balazs::FoliationSphere*);
private slots:
    void onHelpButtonClickedSlot() { onHelpButtonClicked(); }
    void onOkButtonClickedSlot() { onOkButtonClicked(); }
    void onPreviewButtomClickedSlot() { onPreviewButtomClicked(); }
private:
    virtual void emitFoliation(balazs::FoliationSphere* fol) { emit(foliationSphere(fol)); }
    virtual QWidget* createDrawing(const balazs::FoliationSphere& fol);
};



class PreviewPageFoliationRP2 : public PreviewPage<balazs::FoliationRP2>
{
    Q_OBJECT
public:
    explicit PreviewPageFoliationRP2(QWidget *parent = 0);
signals:
    void foliationRP2(balazs::FoliationRP2*);
private slots:
    void onHelpButtonClickedSlot() { onHelpButtonClicked(); }
    void onOkButtonClickedSlot() { onOkButtonClicked(); }
    void onPreviewButtomClickedSlot() { onPreviewButtomClicked(); }
private:
    virtual void emitFoliation(balazs::FoliationRP2* fol) { emit(foliationRP2(fol)); }
    virtual QWidget* createDrawing(const balazs::FoliationRP2& fol);
};







class NewFoliationIntExchangeWidget : public PreviewPageFoliation
{
    Q_OBJECT
public:
    explicit NewFoliationIntExchangeWidget(QWidget *parent = 0);
private slots:
    void disableOkButton(QString);
private:
    virtual QString helpText();
    virtual balazs::Foliation* createFoliation();

    QLabel* permutationLabel;
    QLabel* lengthsLabel;
    QLabel* twistLabel;
    QLineEdit* permutationLineEdit;
    QLineEdit* lengthsLineEdit;
    QLineEdit* twistLineEdit;
};












class NewFoliationRandomWidget : public PreviewPageFoliation
{
    Q_OBJECT
public:
    explicit NewFoliationRandomWidget(QWidget *parent = 0);
private slots:
    void disableOkButton(int);
private:
    virtual QString helpText() { return "";}
    virtual balazs::Foliation* createFoliation();

    QLabel* numIntervalLabel;
    QSpinBox* numIntervalSpinBox;
};






class NewFoliationAYWidget : public PreviewPageFoliation
{
    Q_OBJECT
public:
    explicit NewFoliationAYWidget(QWidget *parent = 0);
private slots:
    void disableOkButton(int);
private:
    virtual QString helpText();
    virtual balazs::Foliation* createFoliation();

    QLabel* genusLabel;
    QSpinBox* genusSpinBox;
};



class NewFoliationRP2WeighedTreeWidget : public PreviewPageFoliationRP2
{
    Q_OBJECT
public:
    explicit NewFoliationRP2WeighedTreeWidget(QWidget *parent = 0);
private slots:
    void disableOkButton(QString);
private:
    virtual QString helpText();
    virtual balazs::FoliationRP2* createFoliation();

    QLabel* weighedTreeLabel;
    QLineEdit* weighedTreeLineEdit;
};


class NewFoliationRP2RandomWidget : public PreviewPageFoliationRP2
{
    Q_OBJECT
public:
    explicit NewFoliationRP2RandomWidget(QWidget *parent = 0);
private slots:
    void disableOkButton(int);
private:
    virtual QString helpText();
    virtual balazs::FoliationRP2* createFoliation();

    QLabel* numEdgesLabel;
    QSpinBox* numEdgesSpinBox;
};


class NewFoliationRP2AYWidget : public PreviewPageFoliationRP2
{
    Q_OBJECT
public:
    explicit NewFoliationRP2AYWidget(QWidget *parent = 0);
private:
    virtual QString helpText();
    virtual balazs::FoliationRP2* createFoliation();

    QLabel* instructions;
};


class NewFoliationSphereDiskWidget : public PreviewPageFoliationSphere
{
    Q_OBJECT
public:
    explicit NewFoliationSphereDiskWidget(QWidget *parent = 0);
private slots:
    void disableOkButton();
private:
    virtual QString helpText();
    virtual balazs::FoliationSphere* createFoliation();

    QLabel* instructions;
};



class NewFoliationSphereHLMWidget : public PreviewPageFoliationSphere
{
    Q_OBJECT
public:
    explicit NewFoliationSphereHLMWidget(QWidget *parent = 0);
private:
    virtual QString helpText();
    virtual balazs::FoliationSphere* createFoliation();

    QLabel* instructions;
};



class NewFoliationSphereRandomWidget : public PreviewPageFoliationSphere
{
    Q_OBJECT
public:
    explicit NewFoliationSphereRandomWidget(QWidget *parent = 0);
private slots:
    void disableOkButton();
private:
    virtual QString helpText();
    virtual balazs::FoliationSphere* createFoliation();

    QLabel* instructions;
};

#endif // PARAMETERSWIDGET_H
