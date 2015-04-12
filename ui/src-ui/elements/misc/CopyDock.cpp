#include "CopyDock.hpp"

#include <QMenu>

CopyDock::CopyDock(const QString& text,
                   QWidget* parent)
: QLabel(text, parent)
{
	setupUi();
}

void CopyDock::setupUi()
{
	QLabel::setObjectName("CopyDock");

	QLabel::setCursor(Qt::PointingHandCursor);

	QLabel::setEnabled(false);

	QLabel::setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this, &QLabel::customContextMenuRequested,
			this, &CopyDock::showContextMenu);
}

void CopyDock::showContextMenu(const QPoint& pos)
{
    QMenu* menu = new QMenu(this);

    QAction* removeAction = new QAction("Empty dock  ", this);

    connect(removeAction, &QAction::triggered,
            this, &CopyDock::removeModUnit);

    menu->addAction(removeAction);

    menu->popup(QLabel::mapToGlobal(pos));
}

void CopyDock::setModUnit(ModUnit* mod)
{
    mod_ = mod;

    QLabel::setEnabled(true);
}

ModUnit* CopyDock::getModUnit() const
{
    return mod_;
}

void CopyDock::removeModUnit()
{
    mod_ = nullptr;

    QLabel::setEnabled(false);
}

bool CopyDock::hasModUnit() const
{
    // Automatic cast to bool
    // if mod is not null
    return mod_;
}
