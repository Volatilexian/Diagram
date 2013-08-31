#ifndef PROPERTIESDIALOG_H
#define PROPERTIESDIALOG_H

#include <QDialog>

class Node;
class QLabel;
class QGroupBox;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;

class PropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    PropertiesDialog(Node *node, QWidget *parent = 0);
    
private slots:
    void okButtonClicked();
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

    QGroupBox *position;
    QGroupBox *attributes;
    QLabel *xLabel;
    QLabel *yLabel;
    QSpinBox *xSpinBox;
    QSpinBox *ySpinBox;
    QLabel *textLabel;
    QLabel *textColorLabel;
    QLabel *outlineColorLabel;
    QLabel *backgroundColorLabel;
    QLineEdit *textEdit;
    QPushButton *chooseTextColor;
    QPushButton *chooseOutlineColor;
    QPushButton *chooseBackgroundColor;
    QPushButton *okButton;
    QPushButton *cancenButton;

    QHBoxLayout *positionLayout;
    QGridLayout *attributesLayout;
    QHBoxLayout *buttonsLayout;
    QVBoxLayout *mainLayout;
};

#endif // PROPERTIESDIALOG_H
