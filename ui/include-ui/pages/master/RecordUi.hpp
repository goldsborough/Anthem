#ifndef RECORDUI_HPP
#define RECORDUI_HPP

#include <QWidget>

class RecordUi : public QWidget
{
	Q_OBJECT

public:

	RecordUi(QWidget* parent = nullptr);

private:

	void setupUi();

	virtual void paintEvent(QPaintEvent* event) override;
};

#endif /* RECORDUI_HPP */
