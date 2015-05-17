#ifndef OPERATORUI_HPP
#define OPERATORUI_HPP

#include <QWidget>

class Operator;
class QVBoxLayout;
class QWidget;
class QTabWidget;
class QPushButton;

class OperatorUi : public QWidget
{
    Q_OBJECT

public:

	explicit OperatorUi(QWidget* parent = nullptr);

private:

	void setupBar();

	void setupPrimary();

	void setupSecondary();

	void setupSelectTab();

	void setupCreateTab();


	void paintEvent(QPaintEvent* paintEvent);


	QVBoxLayout* layout_;

	QWidget* primary_;

	QTabWidget* secondary_;

	QPushButton* toggle_;

};

#endif // OPERATORUI_HPP
