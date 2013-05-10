#ifndef DOCUMENTATIONWINDOW_H
#define DOCUMENTATIONWINDOW_H

#include <QWidget>
#include <QMap>

class QListView;
class QTextEdit;
class QStringListModel;

class DocumentationWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DocumentationWindow(QWidget *parent = 0);
    
signals:
    
public slots:
    void setDisplayAreaText(const QModelIndex& index);
private:
    QListView* titleListView;
    QTextEdit* displayArea;
    QStringListModel* titleListModel;
    QMap<QString, QString> docItems;
};

#endif // DOCUMENTATIONWINDOW_H
