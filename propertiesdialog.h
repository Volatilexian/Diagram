#ifndef PROPERTIESDIALOG_H
#define PROPERTIESDIALOG_H

#include <QDialog>

class Node;
class QLabel;

class PropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    PropertiesDialog(Node *node, QWidget *parent = 0);
    
private slots:
    void buttonBoxAccepted();
    void textColorButtonClicked();
    void outlineColorButtonClicked();
    void backgroundColorButtonClicked();
    
private:
    void updateColorLabel(QLabel *label, const QColor &color);
    void chooseColor(QLabel *label, QColor *color);

    Node *node;
    QColor textColor;
    QColor outlineColor;
    QColor backgroundColor;
};

#endif // PROPERTIESDIALOG_H
