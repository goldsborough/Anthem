#ifndef CREATOR_HPP
#define CREATOR_HPP

#include <QWidget>

class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
class WavetableUi;
class PartialsUi;
class ComboBox;

class Creator : public QWidget
{
	Q_OBJECT

public:

	explicit Creator(QWidget* parent = nullptr);


signals:

protected:

	virtual void paintEvent(QPaintEvent *) override;

	void setupMenu();


	QHBoxLayout* layout_;

	QVBoxLayout* menu_;

	QPushButton* generate_;

	QPushButton* save_;

	QPushButton* sigma_;

	ComboBox* number_;

	ComboBox* bits_;


	WavetableUi* wavetable_;

	PartialsUi* partials_;
};

#endif // CREATOR_HPP
