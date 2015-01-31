#include "SettingsDialog.hpp"
#include "CustomComboBox.hpp"

#include <QPushButton>
#include <QGridLayout>
#include <QLabel>

SettingsDialog::SettingsDialog(QWidget *parent)
: QDialog(parent)
{
    setupUi();
}

void SettingsDialog::setupUi()
{
    QGridLayout* layout = new QGridLayout(this);

    /* --------- Samplerate -------- */

    QLabel* samplerateLabel = new QLabel("Samplerate", this);

    layout->addWidget(samplerateLabel, 0, 0);

    CustomComboBox* samplerateComboBox = new CustomComboBox(this);

    samplerateComboBox->addItems({"44100", "48000", "92000", "12345678"});

    layout->addWidget(samplerateComboBox, 0, 1);

    /* --------- Default directory -------- */

    QLabel* defaultDirectoryLabel = new QLabel("Default directory", this);

    layout->addWidget(defaultDirectoryLabel,1,0);

    QPushButton* defaultDirectoryButton = new QPushButton("/Users/petergoldsborough/Documents", this);

    layout->addWidget(defaultDirectoryButton, 1, 1);

    /* --------- Cancel PushButton -------- */

    QPushButton* cancelButton = new QPushButton("Cancel", this);

    layout->addWidget(cancelButton, 2, 0);

    connect(cancelButton, &QPushButton::clicked,
            this, &SettingsDialog::close);

    /* --------- OK PushButton -------- */

    QPushButton* okButton = new QPushButton("OK", this);

    layout->addWidget(okButton, 2, 1);

    connect(okButton, &QPushButton::clicked,
            this, &SettingsDialog::saveSettings_);

    /* --------- This Window -------- */

    QDialog::setFixedWidth(400);

    QDialog::setLayout(layout);
}

void SettingsDialog::saveSettings_()
{
    QDialog::close();
}
