#include "SettingsDialog.hpp"
#include "ComboBox.hpp"

#include <QPushButton>
#include <QComboBox>
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


	auto samplerateComboBox = new ComboBox(this);

	samplerateComboBox->addItems({"44100", "48000"});

	layout->addWidget(samplerateComboBox, 0, 1);


	/* --------- Default directory -------- */

    QLabel* defaultDirectoryLabel = new QLabel("Default directory", this);

    layout->addWidget(defaultDirectoryLabel,1,0);

    QPushButton* defaultDirectoryButton = new QPushButton("/Users/petergoldsborough/Documents", this);

    defaultDirectoryButton->setCursor(Qt::PointingHandCursor);

    layout->addWidget(defaultDirectoryButton, 1, 1);

	/* --------- OK PushButton -------- */

    QPushButton* okButton = new QPushButton("OK", this);

    okButton->setCursor(Qt::PointingHandCursor);

    layout->addWidget(okButton, 2, 0, 1, 2);

    connect(okButton, &QPushButton::clicked,
            this, &SettingsDialog::saveSettings_);

	/* --------- This Window -------- */

	//QDialog::setFixedSize(QDialog::sizeHint());

    QDialog::setLayout(layout);
}

void SettingsDialog::saveSettings_()
{
    QDialog::close();
}
