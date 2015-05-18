#ifndef CREATOR_HPP
#define CREATOR_HPP

#include <QWidget>

class AdditiveWavetableUi;
class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
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

	void setupConnections();


	QHBoxLayout* layout_;

	QVBoxLayout* menu_;

	QPushButton* generate_;

	QPushButton* save_;

	QPushButton* sigma_;

	ComboBox* number_;

	ComboBox* bits_;


	AdditiveWavetableUi* wavetable_;

	PartialsUi* partials_;
};

#endif // CREATOR_HPP
