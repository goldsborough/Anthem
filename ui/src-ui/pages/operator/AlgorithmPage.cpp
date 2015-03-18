#include "AlgorithmPage.hpp"
#include "AlgorithmUi.hpp"

#include <QGridLayout>

AlgorithmPage::AlgorithmPage(QWidget* parent)
: QWidget(parent)
{
	setupUi();
}

void AlgorithmPage::setupUi()
{
	QGridLayout* layout = new QGridLayout(this);

	for (int i = 0; i < 12; ++i)
	{
		layout->addWidget(new AlgorithmUi({0,0}, {0,1}, {1,0}, {2,0}, this),
						  i >= 6, i % 6);
	}
}
