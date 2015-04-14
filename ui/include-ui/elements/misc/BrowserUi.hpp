#ifndef BROWSER_HPP
#define BROWSER_HPP

#include <QWidget>

class BrowserUi : public QWidget
{
	Q_OBJECT

public:

	BrowserUi(QWidget* parent = nullptr);

private:

	virtual void paintEvent(QPaintEvent*) override;

	void setupUi();
};

#endif // BROWSER_HPP
