#include "filebar.hpp"

#include <QtGui>
#include <QtCore>

#include <QDebug>

Filebar::Filebar(QWidget* parent)
: QWidget(parent),
  dir_(QDir::homePath(),"*.anthem")
{
    if (! dir_.cd("Documents"))
    { qWarning("Could not change directory"); }

    files_ = dir_.entryInfoList();

    currFile_ = files_.begin();

    initUI_();
}

void Filebar::initUI_()
{
    fileLabel_ = new QLabel(currFile_->baseName(),this);
    fileLabel_->setObjectName("fileLabel");
    fileLabel_->setFixedSize(192,50);
    fileLabel_->move(96,15);

    upButton_ = new QPushButton("Up",this);
    upButton_->setObjectName("upButton");
    upButton_->move(16,12);

    connect(upButton_, SIGNAL(clicked(bool)), this, SLOT(up()));

    downButton_ = new QPushButton("Down",this);
    downButton_->setObjectName("downButton");
    downButton_->move(16,45);

    connect(downButton_, SIGNAL(clicked(bool)), this, SLOT(down()));

    this->setFixedSize(384,80);
}

void Filebar::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void Filebar::up()
{
    qDebug() << "Up" << endl;

    if (currFile_ + 1 < files_.end())
    {
        fileLabel_->setText((++currFile_)->baseName());
        // read file ...
    }
}

void Filebar::down()
{
    qDebug() << "Down" << endl;

    if (currFile_ != files_.begin())
    {
        fileLabel_->setText((--currFile_)->baseName());
        // read file ...
    }
}
