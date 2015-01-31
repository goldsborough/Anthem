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

    void setIconButton(IconButton* icon);

    IconButton* getIconButton() const;

    void setText(const QString& text);

    QString getText() const;

    void setPlaceholderText(const QString& text);

    QString getPlaceholderText() const;

signals:

    void receivedInput(const QString& text);

private:

    void keyPressEvent(QKeyEvent* event);

    void paintEvent(QPaintEvent* event);

    IconButton* icon_;

    QLineEdit* line_;
};

#endif // POPUPLINE_HPP
