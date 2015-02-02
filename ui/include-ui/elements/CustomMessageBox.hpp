#ifndef MESSAGEBOX_HPP
#define MESSAGEBOX_HPP

#include <QDialog>
#include <QString>
#include <QFlags>

class QPushButton;
class QGridLayout;
class QIcon;

class CustomMessageBox : public QDialog
{
    Q_OBJECT

public:

    CustomMessageBox(const QString& title = QString(),
                     const QString& message = QString(),
                     QWidget* parent = nullptr,
                     QIcon* icon = nullptr);

    void paintEvent(QPaintEvent* event);

    void addButton(QPushButton* button);

    void addButton(QPushButton* button,
                   int row,
                   int column,
                   int rowStretch = 1,
                   int colStretch = 1,
                   QFlags<Qt::AlignmentFlag> align = Qt::AlignCenter);

    QPushButton* lastButtonPressed() const;

private slots:

    void updateLastButton_();

private:

    QGridLayout* layout_;

    QPushButton* lastButton_;

};


#endif // MESSAGEBOX_HPP
