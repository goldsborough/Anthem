#include "Menubar.hpp"
#include "Projectbar.hpp"
#include "CopyDock.hpp"
#include "IconButton.hpp"
#include "SettingsDialog.hpp"

#include <QDebug>
#include <QString>

Menubar::Menubar(QWidget* parent)
: QWidget(parent)
{
    setupUi();

    setVolume(0.749);
}

void Menubar::setupUi()
{
    /* --------- Projectbar -------- */

    projectBar_ = new Projectbar(this);

    projectBar_->move(312,16);

    /* --------- CopyDocks-------- */

    CopyDock* dockA = new CopyDock("A", this);

    dockA->move(16,16);

    CopyDock* dockB = new CopyDock("B", this);

    dockB->move(116,16);

    /* --------- Volume label -------- */

    volumeLabel_ = new QLabel(this);

    volumeLabel_->setObjectName("VolumeLabel");

    volumeLabel_->setFixedSize(80, 80);

    volumeLabel_->move(825, 16);

    /* --------- Settings button -------- */

    IconButton* settingsButton = new IconButton(":/icons/settings.png",
                                                ":/icons/settings-active.png",
                                                new QSize(50, 50),
                                                new QSize(55, 55),
                                                this);
    settingsButton->move(925, 16);

    settingsButton->setObjectName("SettingsButton");

    SettingsDialog* settings = new SettingsDialog(this);

    connect(settingsButton, &IconButton::clicked,
            settings, &SettingsDialog::show);


    /* --------- This Window -------- */

    this->setFixedSize(1024,128);
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
