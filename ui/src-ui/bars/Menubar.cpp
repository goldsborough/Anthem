#include "Menubar.hpp"
#include "Projectbar.hpp"
#include "CopyDock.hpp"
#include "IconButton.hpp"
#include "SettingsDialog.hpp"

#include <QHBoxLayout>
#include <QString>

Menubar::Menubar(QWidget* parent)
: QWidget(parent)
{
    setupUi();

    setVolume(0.749);
}

void Menubar::setupUi()
{
    QHBoxLayout* layout = new QHBoxLayout(this);


    CopyDock* dockA = new CopyDock("A", this);

    layout->addWidget(dockA);


    CopyDock* dockB = new CopyDock("B", this);

    layout->addWidget(dockB);


    layout->addWidget(new QWidget(this));


    projectBar_ = new Projectbar(this);

    layout->addWidget(projectBar_);


    layout->addWidget(new QWidget(this));


    volumeLabel_ = new QLabel(this);

    volumeLabel_->setObjectName("VolumeLabel");

    layout->addWidget(volumeLabel_);


    IconButton* settingsButton = new IconButton(QIcon(":/icons/settings.png"),
                                                QIcon(":/icons/settings-active.png"),
                                                this);

    layout->addWidget(settingsButton);

    settingsButton->setObjectName("SettingsButton");

    SettingsDialog* settings = new SettingsDialog(this);

    connect(settingsButton, &IconButton::clicked,
            settings, &SettingsDialog::show);


    QWidget::setLayout(layout);
}

void Menubar::setVolume(double volume)
{
    volume_ = volume;

    // Convert volume value to string
    QString alpha = QString::number(volume_);

    // Below an amplitude value of 0.75 the color is turquoise, else red
    QString color = (volume_ < 0.75) ? "67, 212, 180, " : "220, 72, 80, ";

    volumeLabel_->setStyleSheet("background-color: rgba(" + color + alpha + ")");
}

double Menubar::getVolume() const
{
    return volume_;
}
