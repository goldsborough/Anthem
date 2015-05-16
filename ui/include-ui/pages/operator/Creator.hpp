#ifndef CREATOR_HPP
#define CREATOR_HPP

#include <QWidget>

class QHBoxLayout;
class QTabWidget;

class Creator : public QWidget
{
	Q_OBJECT

public:

	Creator(QWidget* parent = nullptr);

private:

	void paintEvent(QPaintEvent*) override;

	void setupContainer();

	void setupMenu();

	void setupPartials();

	void setupDraw();


	QHBoxLayout* layout_;

	QTabWidget* container_;
};

#endif // CREATOR_HPP
