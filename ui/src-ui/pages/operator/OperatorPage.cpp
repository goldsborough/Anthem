#include "OperatorPage.hpp"
#include "UnitPage.hpp"
#include "AlgorithmPage.hpp"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>
#include <QPushButton>

namespace OperatorPage
{
    ContainerPage::ContainerPage(QWidget* parent)
        : QTabWidget(parent)
    {
        QTabWidget::setObjectName("ContainerPage");

        setupUi();
    }

    void ContainerPage::setupUi()
    {
        QTabWidget::addTab(new OperatorPage::UnitPage(), "Units");

        QTabWidget::addTab(new OperatorPage::AlgorithmPage(), "Algorithms");

        QTabWidget::setTabPosition(QTabWidget::South);

        QTabWidget::tabBar()->setCursor(Qt::PointingHandCursor);
    }

    void ContainerPage::paintEvent(QPaintEvent*)
    {
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }


}
