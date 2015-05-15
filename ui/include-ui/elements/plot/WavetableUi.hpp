#ifndef WAVEFORM_HPP
#define WAVEFORM_HPP

#include <QWidget>
#include <QSharedPointer>

class QCustomPlot;
class QCPGraph;
class QColor;
class QString;
class QReal;

class WavetableUi : public QWidget
{
	Q_OBJECT


	Q_PROPERTY(double margin READ getMargin WRITE setMargin)


	Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor)


	Q_PROPERTY(QColor lineColor READ getLineColor WRITE setLineColor)

	Q_PROPERTY(double lineWidth READ getLineWidth WRITE setLineWidth)


	Q_PROPERTY(bool gridShown READ getGridShown WRITE setGridShown)

	Q_PROPERTY(QColor gridColor READ getGridColor WRITE setGridColor)

	Q_PROPERTY(double gridWidth READ getGridWidth WRITE setGridWidth)


	Q_PROPERTY(QColor zeroColor READ getZeroColor WRITE setZeroColor)

	Q_PROPERTY(double zeroWidth READ getZeroWidth WRITE setZeroWidth)

public:

	WavetableUi(QWidget* parent = nullptr);


	void setWavetable(const QString& id);

	QString getWavetableId() const;


	void setMargin(double margin);

	double getMargin() const;


	void setBackgroundColor(const QColor& color);

	QColor getBackgroundColor() const;


	void setLineColor(const QColor& color);

	QColor getLineColor() const;


	void setLineWidth(double width);

	double getLineWidth() const;


	void setGridShown(bool shown);

	bool getGridShown() const;


	void setGridColor(const QColor& color);

	QColor getGridColor() const;


	void setGridWidth(double width);

	double getGridWidth() const;


	void setZeroColor(const QColor& color);

	QColor getZeroColor() const;


	void setZeroWidth(double width);

	double getZeroWidth() const;


	void clear();

signals:

	void phaseChanged(int degrees) const;

private:

	static QSharedPointer<QVector<double>> x_;

	static QSharedPointer<QVector<double>> initializeX();


	void setupPlot();


	QCustomPlot* plot_;

	QCPGraph* graph_;

	bool gridShown_;

	double margin_;

	double conversion_;


	QSharedPointer<QVector<double>> y_;

	QSharedPointer<QString> id_;

	QSharedPointer<QColor> background_;

	QSharedPointer<QPen> line_;

	QSharedPointer<QPen> grid_;

	QSharedPointer<QPen> zero_;
};

#endif // WAVEFORM_HPP
