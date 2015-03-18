#ifndef OPERATORPAGE_HPP
#define OPERATORPAGE_HPP

#include <QWidget>
#include <QTabWidget>

class OperatorPage : public QTabWidget
{
	Q_OBJECT

public:

	OperatorPage(QWidget* parent = nullptr);

private:

	void paintEvent(QPaintEvent* paintEvent);

	void setupUi();

};

#endif // OPERATORPAGE_HPP
