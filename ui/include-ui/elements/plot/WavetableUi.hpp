#ifndef WAVEFORM_HPP
#define WAVEFORM_HPP

#include <QWidget>
#include <QSharedPointer>

class QCustomPlot;

class WavetableUi : public QWidget
{
	Q_OBJECT

public:

	WavetableUi(QWidget* parent = nullptr);

private:

	virtual void paintEvent(QPaintEvent*) override;

	void setupUi();


	QSharedPointer<QCustomPlot> plot_;
};

#endif // WAVEFORM_HPP
