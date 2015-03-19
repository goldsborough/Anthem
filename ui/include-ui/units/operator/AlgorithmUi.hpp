#ifndef ALGORITHMUI_HPP
#define ALGORITHMUI_HPP

#include <QAbstractButton>
#include <QSharedPointer>
#include <QVector>

class QPoint;
class QGridLayout;

class AlgorithmUi : public QAbstractButton
{
	Q_OBJECT

public:

	enum Unit { A, B, C, D };

	AlgorithmUi(QWidget* parent = nullptr);

	AlgorithmUi(const QPoint& a,
				const QPoint& b,
				const QPoint& c,
				const QPoint& d,
				QWidget* parent = nullptr);

	void setPosition(Unit unit, const QPoint& pos);

	QPoint getPosition(Unit unit) const;


	void setActive(bool state);

	bool isActive() const;

private:

	void setupUi();

	virtual void paintEvent(QPaintEvent* event) override;


	QVector<QSharedPointer<QPoint>> units_;

	QSharedPointer<QGridLayout> layout_;

	bool active_;

};

#endif /* ALGORITHMUI_HPP */
