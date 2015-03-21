#ifndef MODCONTROL_HPP
#define MODCONTROL_HPP

#include <QWidget>
#include <QString>

class ModDockUi;
class ModDial;
struct ModUnitUi;

class ModControl : public QWidget
{
    Q_OBJECT

public:

	typedef unsigned short index_t;

	ModControl(QWidget* parent = nullptr);

	ModControl(const QString& title,
			   index_t dockSize,
			   index_t wrap,
			   QWidget* parent = nullptr);

	void setTitle(const QString& title);

	QString getTitle() const;

signals:

	void depthChanged(index_t index, double value) const;

	void modUnitInserted(index_t index, const ModUnitUi& mod) const;

	void modUnitRemoved(index_t index) const;

private:

	void setupUi();


	const index_t wrap_;

	const index_t dockSize_;

	QString title_;
};

#endif // MODCONTROL_HPP
