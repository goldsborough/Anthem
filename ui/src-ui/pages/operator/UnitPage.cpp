#include "UnitPage.hpp"
#include "OperatorUi.hpp"

#include <QGridLayout>

UnitPage::UnitPage(QWidget* parent)
: QWidget(parent)
{
	setupUi();
}

void UnitPage::setupUi()
{
	QGridLayout* layout = new QGridLayout(this);

	for (size_t i = A; i <= D; ++i)
	{ operators_.append(new OperatorUi(this)); }

	layout->addWidget(operators_[A], 0, 0);

	layout->addWidget(operators_[B], 0, 1);

	layout->addWidget(operators_[C], 1, 0);

	layout->addWidget(operators_[D], 1, 1);
}
