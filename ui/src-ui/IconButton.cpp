#include "iconbutton.hpp"

#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QString>
#include <QSize>

#include <QDebug>

IconButton::IconButton(const QString& path,
                       QSize* size,
                       QWidget* parent)
: IconButton(path,path,size,size,parent)
{ }

IconButton::IconButton(const QString& standardPath,
                       const QString& activePath,
                       QSize* standardSize,
                       QSize* activeSize,
                       QWidget *parent)
: QPushButton(parent),
  standard_(new QIcon(standardPath)), active_(new QIcon(activePath)),
  standardSize_(standardSize), activeSize_(activeSize)
{
    QPushButton::setIcon(*standard_);

    QPushButton::setIconSize(*standardSize_);

    QPushButton::setFixedSize(*standardSize);

    QPushButton::setCursor(Qt::PointingHandCursor);

    connect(this, SIGNAL(pressed()), this, SLOT(setActiveIcon()));

    connect(this, SIGNAL(released()), this, SLOT(setStandardIcon()));
}

void IconButton::setActiveIcon()
{
    QPushButton::setIcon(*active_);

    QPushButton::setIconSize(*activeSize_);

}

void IconButton::setStandardIcon()
{
    QPushButton::setIcon(*standard_);

    QPushButton::setIconSize(*standardSize_);
}

QIcon* IconButton::getStandardIcon() const
{
    return standard_;
}

QIcon* IconButton::getActiveIcon() const
{
    return active_;
}

QSize* IconButton::getStandardSize() const
{
    return standardSize_;
}

QSize* IconButton::getActiveSize() const
{
    return activeSize_;
}

IconButton::~IconButton()
{
    delete standard_;
    delete active_;

    delete standardSize_;
    delete activeSize_;
}
