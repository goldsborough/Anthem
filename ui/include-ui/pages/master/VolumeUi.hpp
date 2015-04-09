#ifndef VOLUMEUI_HPP
#define VOLUMEUI_HPP

#include <QWidget>

class VolumeUi : public QWidget
{
	Q_OBJECT

public:

	VolumeUi(QWidget* parent = nullptr);

private:

	void setupUi();

	virtual void paintEvent(QPaintEvent* event) override;
};

#endif /* VOLUMEUI_HPP */
