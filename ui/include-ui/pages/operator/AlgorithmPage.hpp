#ifndef ALGORITHMPAGE_HPP
#define ALGORITHMPAGE_HPP

#include <QWidget>
#include <QSharedPointer>

class QGridLayout;
class AlgorithmUi;

class AlgorithmPage : public QWidget
{
	Q_OBJECT

public:

	typedef std::size_t index_t;

	AlgorithmPage(QWidget* parent = nullptr);

signals:

	void algorithmChanged(index_t index) const;

	void algorithmDisabled() const;

private slots:

	void handle_(bool state);

private:

	void setupUi();

	QSharedPointer<QGridLayout> layout_;

	AlgorithmUi* algorithm_;
};

#endif // ALGORITHMPAGE_HPP
