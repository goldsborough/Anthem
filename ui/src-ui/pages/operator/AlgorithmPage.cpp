#include "AlgorithmPage.hpp"
#include "AlgorithmUi.hpp"

#include <QGridLayout>
#include <QVector>

AlgorithmPage::AlgorithmPage(QWidget* parent)
: QWidget(parent),
  layout_(new QGridLayout(this)),
  algorithm_(nullptr)
{
	setupUi();
}

void AlgorithmPage::setupUi()
{
	// Algorithm layouts
	QVector<QVector<QPoint>> pos = {{{0, 1}, {1, 1}, {2, 1}, {3, 1}},
									{{0, 0}, {0, 2}, {1, 1}, {2, 1}},
									{{0, 0}, {1, 0}, {1, 2}, {3, 1}},
									{{0, 1}, {1, 0}, {1, 2}, {2, 1}},
									{{0, 0}, {1, 0}, {2, 0}, {2, 2}},
									{{0, 1}, {1, 1}, {2, 0}, {2, 2}},
									{{0, 0}, {0, 1}, {0, 2}, {1, 1}},
									{{0, 0}, {0, 2}, {1, 0}, {1, 2}},
									{{0, 1}, {1, 0}, {1, 1}, {1, 2}},
									{{0, 0}, {1, 0}, {1, 1}, {1, 2}},
									{{0, 1}, {1, 0}, {1, 2}, {1, 3}},
									{{0, 0}, {0, 1}, {0, 2}, {0, 3}}};

	for (int i = 0; i < pos.size(); ++i)
	{
		algorithm_ = new AlgorithmUi(pos[i][0],
									 pos[i][1],
									 pos[i][2],
									 pos[i][3],
									 this);

		connect(algorithm_, &AlgorithmUi::toggled,
				this, &AlgorithmPage::handle_);

		layout_->addWidget(algorithm_, i / 4, i  % 4);
	}

	// Does not emit toggled()
	algorithm_->setChecked(true);

	algorithm_->setActive(true);
}

void AlgorithmPage::handle_(bool state)
{
	// Unset old algorithm
	if (algorithm_)
	{
		algorithm_->setActive(false);

		// Does not emit toggled()
		algorithm_->setChecked(false);
	}

	if (state)
	{
		algorithm_ = dynamic_cast<AlgorithmUi*>(sender());

		algorithm_->setActive(true);

		emit algorithmChanged(layout_->indexOf(algorithm_));
	}

	else
	{
		algorithm_ = nullptr;

		emit algorithmDisabled();
	}
}
