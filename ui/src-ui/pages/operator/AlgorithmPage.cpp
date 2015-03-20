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
	// Layout for each algorithm
	// consisting of the row and column
	// of each operator node in the algorithm
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

	// Iterate backwards so that algorithm_ points
	// to the first item, which is full serial FM,
	// avoids having to re-set the pointer after,
	// which would involve casting from QLayoutItem
	for (int i = pos.size() - 1; i >= 0; --i)
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

		// Uncheck the old algorithm
		// before we check the new one
		// Does not emit toggled()
		algorithm_->setChecked(false);
	}

	// If the request is to activate the algorithm
	if (state)
	{
		// Reset the algorithm_ pointer
		algorithm_ = dynamic_cast<AlgorithmUi*>(sender());

		// Change its appearance to that
		// of the active algorithm
		algorithm_->setActive(true);

		// Let the OperatorPage know that the algorithm changed
		emit algorithmChanged(layout_->indexOf(algorithm_));
	}

	// If the request was to uncheck the
	// currently activated algorithm
	else
	{
		// Reset the current algorithm pointer
		algorithm_ = nullptr;

		// Let the OperatorPage know that all
		// algorithms are inactive right now
		emit algorithmDisabled();
	}
}
