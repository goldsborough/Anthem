#include "AnthemUi.hpp"
#include "Menubar.hpp"
#include "OperatorPage.hpp"

#include <QTabWidget>
#include <QTabBar>
#include <QVBoxLayout>


#include <QDebug>



AnthemUi::AnthemUi(QWidget *parent)
	: QWidget(parent)
{
    setupUi();
}

void AnthemUi::setupUi()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->setMargin(0);

	layout->setContentsMargins(0,0,0,0);

	//layout->setSpacing(0);


	Menubar* menubar = new Menubar(this);

	qDebug() << "menubar: " << menubar->sizeHint();

	menubar->setSizePolicy(QSizePolicy::Expanding,
						   QSizePolicy::Expanding);

	layout->addWidget(menubar);

	qDebug() << menubar->rect();


	QTabWidget* pages = new QTabWidget(this);

	qDebug() << "tabwidget: " << pages->sizeHint();

	pages->setSizePolicy(QSizePolicy::Preferred,
						 QSizePolicy::Preferred);

    pages->tabBar()->setCursor(Qt::PointingHandCursor);

	layout->addWidget(pages);



    QWidget* lfoPage = new QWidget;

    lfoPage->setObjectName("ContainerPage");

    pages->addTab(lfoPage, "LFO");


    pages->addTab(new OperatorPage::ContainerPage(), "Operator");


    QWidget* masterPage = new QWidget;

    masterPage->setObjectName("ContainerPage");

    pages->addTab(masterPage, "Master");


    QWidget* effectPage = new QWidget;

    effectPage->setObjectName("ContainerPage");

    pages->addTab(effectPage, "Effect");


    QWidget* envelopePage = new QWidget;

    envelopePage->setObjectName("ContainerPage");

	pages->addTab(envelopePage, "Envelope");


	qDebug() << pages->rect();

	qDebug() << "window" << QWidget::sizeHint();

	qDebug() << QWidget::rect();

	QWidget::setSizePolicy(QSizePolicy::Fixed,
						   QSizePolicy::Fixed);

	QWidget::setWindowTitle("Anthem");
}
