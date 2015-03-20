#ifndef ANTHEMUI_HPP
#define ANTHEMUI_HPP

#include <QWidget>

class AnthemUi : public QWidget
{
    Q_OBJECT

public:

	AnthemUi(QWidget* parent = nullptr);

private:

	void setupUi();

	virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ANTHEMUI_HPP
