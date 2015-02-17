#ifndef MODCONTROL_HPP
#define MODCONTROL_HPP

#include <QWidget>
#include <QString>

class ModControl : public QWidget
{
    Q_OBJECT

public:

    ModControl(const QString& title, QWidget* parent = nullptr);

    ~ModControl();

private:

    void setupUi();

    void paintEvent(QPaintEvent *);

    QString title_;
};

#endif // MODCONTROL_HPP
