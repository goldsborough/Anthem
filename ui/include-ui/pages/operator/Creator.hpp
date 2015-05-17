#ifndef CREATOR_HPP
#define CREATOR_HPP

#include <QWidget>

class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
class Wavetable;
class ComboBox;
class Plot;

class Creator : public QWidget
{
	Q_OBJECT

public:

	explicit Creator(QWidget* parent = nullptr);

	virtual ~Creator();


signals:

	void wavetableGenerated(const Wavetable& wavetable) const;

protected:

	virtual void setupMenu();

	virtual void setupPlot() = 0;


	QHBoxLayout* layout_;

	QVBoxLayout* menu_;

	QPushButton* generate_;

	QPushButton* save_;

	Plot* plot_;

	ComboBox* bits_;
};

#endif // CREATOR_HPP
