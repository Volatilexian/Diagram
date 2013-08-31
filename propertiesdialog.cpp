#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QColorDialog>
#include <QPixmap>

#include "propertiesdialog.h"
#include "node.h"

PropertiesDialog::PropertiesDialog(Node *node, QWidget *parent = 0) :
    QDialog(parent)
{
    position = new QGroupBox(tr("Position"));
    xLabel = new QLabel(tr("&X"));
    xSpinBox = new QSpinBox;
    xLabel->setBuddy(xSpinBox);
    yLabel = new QLabel(tr("&Y"));
    ySpinBox = new QSpinBox;
    yLabel->setBuddy(ySpinBox);
    positionLayout = new QHBoxLayout;
    positionLayout->addWidget(xLabel);
    positionLayout->addWidget(xSpinBox);
    positionLayout->addWidget(yLabel);
    positionLayout->addWidget(ySpinBox);
    position->setLayout(positionLayout);

    attributes = new QGroupBox(tr("Attributes"));
    textLabel = new QLabel(tr("&Text:"));
    textEdit = new QLineEdit;
    textLabel->setBuddy(textEdit);
    textColorLabel = new QLabel(tr("Text Color:"));
    chooseTextColor = new QPushButton(tr("Choose..."));
    outlineColorLabel = new QLabel(tr("Outline Color:"));
    chooseOutlineColor = new QPushButton(tr("Choose..."));
    backgroundColorLabel = new QLabel(tr("background Color:"));
    chooseBackgroundColor = new QPushButton(tr("Choose..."));
    attributesLayout = new QGridLayout;
    attributesLayout->addWidget(textLabel, 0, 0);
    attributesLayout->addWidget(textEdit, 0, 1);
    attributesLayout->addWidget(textColorLabel, 1, 0);
    attributesLayout->addWidget(chooseTextColor, 1, 1);
    attributesLayout->addWidget(outlineColorLabel, 2, 0);
    attributesLayout->addWidget(chooseOutlineColor, 2, 1);
    attributesLayout->addWidget(backgroundColorLabel, 3, 0);
    attributesLayout->addWidget(chooseBackgroundColor, 3, 1);
    attributes->setLayout(attributesLayout);

    okButton = new QPushButton(tr("OK"));
    cancenButton = new QPushButton(tr("Cancel"));
    buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancenButton);

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(position);
    mainLayout->addWidget(attributes);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    this->node = node;
    xSpinBox->setValue(int(node->x()));
    ySpinBox->setValue(int(node->y()));
    textEdit->setText(node->text());
    textColor = node->textColor();
    outlineColor = node->outlineColor();
    backgroundColor = node->backgroundColor();
    updateColorLabel(textColorLabel, textColor);
    updateColorLabel(outlineColorLabel, outlineColor);
    updateColorLabel(backgroundColorLabel, backgroundColor);

    connect(chooseTextColor, SIGNAL(clicked()), this, SLOT(textColorButtonClicked()));
    connect(chooseOutlineColor, SIGNAL(clicked()), this, SLOT(outlineColorButtonClicked()));
    connect(chooseBackgroundColor, SIGNAL(clicked()), this, SLOT(backgroundColorButtonClicked()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
}


void PropertiesDialog::chooseColor(QLabel *label, QColor *color)
{
    QPixmap pixmap(16, 16);
    pixmap.fill(color);
    label->setPixmap(pixmap);
}


void PropertiesDialog::updateColorLabel(QLabel *label, const QColor &color)
{
    QColor newColor = QColorDialog::getColor(*color, this);
    if(newColor.isValid())
    {
        *color = newColor;
        updateColorLabel(label, *color);
    }
}

void PropertiesDialog::okButtonClicked()
{
    node->setPos(xSpinBox->value(), ySpinBox->value());
    node->setText(textEdit->text());
    node->setTextColor(textColor);
    node->setOutlineColor(outlineColor);
    node->setBackgroundColor(backgroundColor);
    node->update();
    QDialog::accept();
}

void PropertiesDialog::textColorButtonClicked()
{
    chooseColor(textColorLabel, &textColor);
}

void PropertiesDialog::outlineColorButtonClicked()
{
    chooseColor(outlineColorLabel, &outlineColor);
}

void PropertiesDialog::backgroundColorButtonClicked()
{
    chooseColor(backgroundColorLabel, &backgroundColor);
}
