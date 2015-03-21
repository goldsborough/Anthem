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

    ModControl(const QString& title, QWidget* parent = nullptr);

    ~ModControl();

	void setTitle(const QString& title);

	QString getTitle() const;

signals:

    void depthChanged(unsigned short index, double value) const;

	void modUnitInserted(unsigned short index, const ModUnitUi& mod) const;

	void modUnitRemoved(unsigned short index) const;

private:

	void setupUi();

	QString title_;
};

#endif // MODCONTROL_HPP
