#ifndef PANUI_HPP
#define PANUI_HPP

#include <QWidget>

class PanUi : public QWidget
{
	Q_OBJECT

public:

	PanUi(QWidget* parent = nullptr);

private:

	void setupUi();

	virtual void paintEvent(QPaintEvent* event) override;
};

#endif /* PANUI_HPP */
