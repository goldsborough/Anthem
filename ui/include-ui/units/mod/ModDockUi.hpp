#ifndef MODDOCKUI_HPP
#define MODDOCKUI_HPP

#include <QWidget>
#include <QVector>

class ModDock;
class ModItemUi;
class QString;

class ModDockUi : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(int wrap READ getWrap WRITE setWrap)

public:

    typedef unsigned short index_t;

    ModDockUi(index_t dockSize,
              index_t wrap = 2,
              QWidget* parent = nullptr);

    ~ModDockUi();


    void addModUnit(ModItemUi* unit);


    ModItemUi* getModUnit(index_t index) const;

    ModItemUi* getModUnit(const QString& text) const;


    void removeModUnit(index_t index);

    void removeModUnit(const QString& text);


    void setWrap(index_t wrap);

    index_t getWrap() const;


    index_t size() const;

    void clear();

signals:

    //void itemUnderMouse(index_t index) const;

    void itemValueChanged(index_t index, double value) const;

private slots:

    void emitItemValueChanged(double value);

private:

    void setupUi();

    index_t wrap_;

    QVector<ModItemUi*> units_;

};

#endif // MODDOCKUI_HPP
