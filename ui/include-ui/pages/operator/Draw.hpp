#ifndef DRAW_HPP
#define DRAW_HPP

#include "Creator.hpp"

class Draw : public Creator
{
	Q_OBJECT

public:

	explicit Draw(QWidget* parent);

private:

	virtual void paintEvent(QPaintEvent *) override;

	virtual void setupPlot() override;
};

#endif // DRAW_HPP
