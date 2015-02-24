#ifndef MODDOCKUI_HPP
#define MODDOCKUI_HPP

#include <QWidget>
#include <QVector>

class QGridLayout;

class ModDock;
class ModItemUi;
struct ModUnitUi;

class ModDockUi : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(int wrap READ getWrap WRITE setWrap)

public:

    typedef unsigned short index_t;


    ModDockUi(QWidget* parent = nullptr);

	ModDockUi(index_t dockSize,
			  QWidget* parent);

    ModDockUi(index_t dockSize,
			  index_t wrap,
              QWidget* parent = nullptr);


	void setDockSize(index_t size);

	index_t getDockSize() const;


	void addSpot();

	void removeSpot();


    void setWrap(index_t wrap);

    index_t getWrap() const;

signals:

    void itemHovered(index_t index) const;

    void depthChanged(index_t index, double value) const;

	void modUnitInserted(index_t index, const ModUnitUi& mod) const;

	void modUnitRemoved(index_t index) const;

private slots:

    void emitDepthChanged(double value) const;

	void emitModUnitInserted(const ModUnitUi& mod) const;

	void emitModUnitRemoved() const;

    void emitItemHovered() const;

private:

    void setupUi();

    index_t wrap_;

    QVector<ModItemUi*> items_;

	QGridLayout* layout_;

};

#endif // MODDOCKUI_HPP
