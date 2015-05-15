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

    OperatorUi(QWidget* parent = nullptr);

private:

	void setupBar();

	void setupPrimary();

	void setupSecondary();

	void setupWavesTab();

	void setupCustomTab();


	void paintEvent(QPaintEvent* paintEvent);


	QVBoxLayout* layout_;

	QWidget* primary_;

	QTabWidget* secondary_;

	QPushButton* toggle_;

};

#endif // OPERATORUI_HPP
