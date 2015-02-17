#ifndef MODDOCKUI_HPP
#define MODDOCKUI_HPP

#include <QWidget>
#include <QVector>

class ModDock;
class ModDockItem;

class ModDockUi : public QWidget
{
    Q_OBJECT

public:

    typedef unsigned short index_t;

    ModDockUi(index_t dockSize,
              index_t wrap = 2,
              QWidget* parent = nullptr);

    ~ModDockUi();

    void setWrap(unsigned short wrap);

    unsigned short getWrap() const;

signals:

    //void itemUnderMouse(index_t index) const;

    void itemValueChanged(index_t index, double value) const;

private slots:

    void emitItemValueChanged(double value);

private:

    void paintEvent(QPaintEvent *);

    void setupUi();

    index_t wrap_;

    QVector<ModDockItem*> items_;

};

#endif // MODDOCKUI_HPP
