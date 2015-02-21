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

signals:

    void depthChanged(unsigned short index, double value) const;

    void modUnitChanged(unsigned short index, const ModUnitUi& mod) const;

private:

    void setupUi();

    virtual void paintEvent(QPaintEvent *) override;

    QString title_;

    ModDial* dial_;

    ModDockUi* dock_;
};

#endif // MODCONTROL_HPP
