#ifndef MACROUI_HPP
#define MACROUI_HPP

#include <QWidget>
#include <QSharedPointer>

class Macro;
class QString;
struct ModUnitUi;

class MacroUi : public QWidget
{
	Q_OBJECT

public:

	MacroUi(QWidget* parent = nullptr);

	MacroUi(const QString& id,
			QWidget* parent = nullptr);


	void setId(const QString& id);

	QString getId() const;

private:

	virtual void paintEvent(QPaintEvent*) override;

	void setupUi();


	QSharedPointer<Macro> macro_;

	QSharedPointer<ModUnitUi> mod_;
};

#endif // MACROUI_HPP
