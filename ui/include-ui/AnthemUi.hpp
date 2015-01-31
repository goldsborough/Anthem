#ifndef ANTHEMUI_HPP
#define ANTHEMUI_HPP

#include <QMainWindow>

class AnthemUi : public QMainWindow
{
    Q_OBJECT

public:

    AnthemUi(QWidget *parent = 0);

private:

    void setupUi();
};

#endif // ANTHEMUI_HPP
