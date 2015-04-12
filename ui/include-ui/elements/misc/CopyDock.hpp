#ifndef COPYDOCK_HPP
#define COPYDOCK_HPP

#include <QLabel>
#include <QString>
#include <QPoint>

class ModUnit
{};

class CopyDock : public QLabel
{
    Q_OBJECT

public:

    CopyDock(const QString& text,
             QWidget* parent);

    ModUnit* getModUnit() const;

    bool hasModUnit() const;

    void removeModUnit();

public slots:

   void setModUnit(ModUnit* mod);

private slots:

   void showContextMenu(const QPoint& pos);

private:

   void setupUi();

   ModUnit* mod_;
};

#endif // COPYDOCK_HPP
