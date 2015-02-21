#ifndef MODDOCKUI_HPP
#define MODDOCKUI_HPP

#include <QWidget>
#include <QVector>

class ModDock;
class ModItemUi;
class QString;
struct ModUnitUi;

class ModDockUi : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(int wrap READ getWrap WRITE setWrap)

public:

    typedef unsigned short index_t;

    ModDockUi(QWidget* parent = nullptr);

    ModDockUi(index_t dockSize,
              index_t wrap = 2,
              QWidget* parent = nullptr);

    ~ModDockUi();

    void setWrap(index_t wrap);

    index_t getWrap() const;

signals:

    void itemHovered(index_t index) const;

    void depthChanged(index_t index, double value) const;

    void modUnitChanged(index_t index, const ModUnitUi& mod) const;

private slots:

    void emitDepthChanged(double value) const;

    void emitModUnitChanged(const ModUnitUi& mod) const;

    void emitItemHovered() const;

private:

    void setupUi();

    index_t wrap_;

    QVector<ModItemUi*> items_;

};

#endif // MODDOCKUI_HPP
