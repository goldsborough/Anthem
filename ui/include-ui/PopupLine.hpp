#ifndef POPUPLINE_HPP
#define POPUPLINE_HPP

#include <QDialog>
#include <QString>
#include <QSize>

class QLineEdit;
class IconButton;

class PopupLine : public QDialog
{
    Q_OBJECT

public:

    PopupLine(QWidget* parent,
              IconButton* icon = nullptr,
              const QString& placeholderText = QString(),
              const QSize& size = QSize(500,55));


    QString getInput() const;

    void keyPressEvent(QKeyEvent* event);

    void paintEvent(QPaintEvent* event);

    void setIconButton(IconButton* icon);

    IconButton* getIconButton() const;

private:

    IconButton* icon_;

    QLineEdit* line_;

    QString text_;

};

#endif // POPUPLINE_HPP
