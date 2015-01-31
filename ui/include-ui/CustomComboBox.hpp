#ifndef CUSTOMCOMBOBOX_HPP
#define CUSTOMCOMBOBOX_HPP

#include <QPushButton>
#include <QString>
#include <QStringList>

class QMenu;

class CustomComboBox : public QPushButton
{
    Q_OBJECT

public:

    CustomComboBox(QWidget* parent = nullptr);

    void addItem(const QString& item);

    void addItems(const QStringList& list);

    QString getCurrentItem() const;

private slots:

    void setCurrentItem_();

private:

    QMenu* menu_;

};

#endif // CUSTOMCOMBOBOX_HPP
