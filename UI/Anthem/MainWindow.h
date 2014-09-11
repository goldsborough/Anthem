#ifndef __MainWindow__
#define __MainWindow__

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget* parent = 0);

private:

    void _initUI();
};

#endif /* defined(__MainWindow__) */
