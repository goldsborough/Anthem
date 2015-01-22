#ifndef FILEBAR_HPP
#define FILEBAR_HPP

#include <QWidget>
#include <QFileInfoList>
#include <QDir>

class QLabel;
class QPaintEvent;
class QPushButton;

class Filebar : public QWidget
{
    Q_OBJECT

public:

    explicit Filebar(QWidget *parent = 0);

    void paintEvent(QPaintEvent* event);

public slots:

    void up();

    void down();

private:

    void initUI_();

    QLabel* fileLabel_;

    QPushButton* upButton_;

    QPushButton* downButton_;

    QDir dir_;

    QFileInfoList files_;

    QFileInfoList::iterator currFile_;

};

#endif // FILEBAR_HPP
