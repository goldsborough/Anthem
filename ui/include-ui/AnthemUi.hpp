#ifndef ANTHEMUI_HPP
#define ANTHEMUI_HPP

#include <QWidget>

class AnthemUi : public QWidget
{
    Q_OBJECT

public:

    AnthemUi(QWidget *parent = 0);

private:

    void setupUi();
};

#endif // ANTHEMUI_HPP
