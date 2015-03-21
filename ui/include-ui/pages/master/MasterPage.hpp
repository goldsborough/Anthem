#ifndef MASTERPAGE_HPP
#define MASTERPAGE_HPP

#include <QWidget>

class MasterPage : public QWidget
{
	Q_OBJECT

public:

	MasterPage(QWidget* parent = nullptr);

private:

	void setupUi();

	virtual void paintEvent(QPaintEvent*) override;
};

#endif /* MASTERPAGE_HPP */
