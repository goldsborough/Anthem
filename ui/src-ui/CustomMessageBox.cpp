#include "CustomMessageBox.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include <QPushButton>
#include <QIcon>

CustomMessageBox::CustomMessageBox(const QString& title,
                                   const QString& message,
                                   QWidget* parent,
                                   QIcon* icon)
: QDialog(parent), lastButton_(nullptr)
{
    QDialog::setWindowTitle(title);

    layout_ = new QGridLayout(parent);

    if (icon)
    {
        QLabel* iconLabel = new QLabel(this);

        iconLabel->setFixedSize(QSize(100,100));

        iconLabel->setPixmap(icon->pixmap(QSize(100,100)));

        layout_->addWidget(iconLabel,0,0,Qt::AlignCenter);
    }


    QLabel* messageLabel = new QLabel(message,this);

    layout_->addWidget(messageLabel);


    QDialog::setLayout(layout_);

    QDialog::setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

QPushButton* CustomMessageBox::lastButtonPressed() const
{
    return lastButton_;
}

void CustomMessageBox::addButton(QPushButton* button)
{
    button->setCursor(Qt::PointingHandCursor);

    // All button clicks end up in the current window being closed
    connect(button,&QPushButton::clicked, this, &CustomMessageBox::close);

    connect(button, &QPushButton::clicked,
            this, &CustomMessageBox::updateLastButton_);

    layout_->addWidget(button);
}

void CustomMessageBox::updateLastButton_()
{
    lastButton_ = dynamic_cast<QPushButton*>(QObject::sender());
}

void CustomMessageBox::addButton(QPushButton* button,
                                 int row,
                                 int column,
                                 int rowStretch,
                                 int colStretch,
                                 QFlags<Qt::AlignmentFlag> align)
{
    button->setCursor(Qt::PointingHandCursor);

    // All button clicks end up in the current window being closed
    connect(button,&QPushButton::clicked, this, &CustomMessageBox::close);

    connect(button, &QPushButton::clicked,
            this, &CustomMessageBox::updateLastButton_);

    // Icon and message are 0 and 1
    layout_->addWidget(button,
                       row,column,
                       rowStretch,colStretch,
                       align);
}

void CustomMessageBox::paintEvent(QPaintEvent*)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

