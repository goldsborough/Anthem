#include "IconButton.hpp"

#include <QString>
#include <QSize>

IconButton::IconButton(const QIcon& icon,
                       QWidget* parent)
: IconButton(icon, icon, parent)
{ }

IconButton::IconButton(const QIcon& standard,
                       const QIcon& active,
                       QWidget *parent)
: QPushButton(parent),
  standard_(new QIcon(standard)),
  active_(new QIcon(active)),
  size_(new QSize), factor_(1.1)
{
    QPushButton::setIcon(*standard_);

    QPushButton::setCursor(Qt::PointingHandCursor);

    connect(this, &QPushButton::pressed,
            [=] () { QPushButton::setIconSize(*size_ * factor_);
                     QPushButton::setIcon(*active_); });

    connect(this, &QPushButton::released,
            [=] () { QPushButton::setIconSize(*size_);
                     QPushButton::setIcon(*standard_); });
}

void IconButton::setIconFactor(double factor)
{
    factor_ = factor;
}

double IconButton::getIconFactor() const
{
    return factor_;
}

void IconButton::setIconSize(const QSize &size)
{
    *size_ = size;

    QPushButton::setIconSize(*size_);
}

void IconButton::setStandardIcon(const QIcon& icon)
{
    *standard_ = icon;
}

QIcon IconButton::getStandardIcon() const
{
    return *standard_;
}

void IconButton::setActiveIcon(const QIcon& icon)
{
    *active_ = icon;
}

QIcon IconButton::getActiveIcon() const
{
    return *active_;
}

void IconButton::setIconWidth(int width)
{
    size_->setWidth(width);

    QPushButton::setIconSize(*size_);
}

int IconButton::getIconWidth() const
{
    return size_->width();
}

void IconButton::setIconHeight(int height)
{
    size_->setHeight(height);

    QPushButton::setIconSize(*size_);
}

int IconButton::getIconHeight() const
{
    return size_->height();
}
