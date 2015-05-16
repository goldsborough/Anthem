#ifndef MESSAGEBOX_HPP
#define MESSAGEBOX_HPP

#include <QDialog>
#include <QString>
#include <QFlags>

class QPushButton;
class QGridLayout;
class QLabel;

class MessageBox : public QDialog
{
    Q_OBJECT

public:

	MessageBox(QWidget* parent = nullptr);

	MessageBox(const QString& title,
					 const QString& message,
					 QWidget* parent = nullptr);

	MessageBox(const QString& title,
					 const QString& message,
					 const QString& details,
					 QWidget* parent = nullptr);


	QPushButton* ask();


	void setTitle(const QString& title);

	QString getTitle() const;


	void setMessage(const QString& message);

	QString getMessage() const;


	void setDetails(const QString& details);

	QString getDetails() const;


	void addButton(QPushButton* button);

    void addButton(QPushButton* button,
                   int row,
                   int column,
                   int rowStretch = 1,
                   int colStretch = 1,
                   QFlags<Qt::AlignmentFlag> align = Qt::AlignCenter);


	QPushButton* lastButtonPressed() const;

private slots:

	void update_();

private:

	void setupUi();

	void paintEvent(QPaintEvent* event);


    QGridLayout* layout_;

    QPushButton* lastButton_;


	QLabel* message_;

	QLabel* details_;
};


#endif // MESSAGEBOX_HPP
