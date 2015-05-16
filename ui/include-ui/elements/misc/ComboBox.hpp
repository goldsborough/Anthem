#ifndef CUSTOMCOMBOBOX_HPP
#define CUSTOMCOMBOBOX_HPP

#include <QWidget>

class QString;
class QListView;
class QModelIndex;
class QStringList;
class QPushButton;
class QStandardItemModel;

class ComboBox : public QWidget
{
    Q_OBJECT

	Q_PROPERTY(Qt::Alignment alignment READ getAlignment WRITE setAlignment)

	Q_PROPERTY(int direction READ getDirection WRITE setDirection)

public:

	using index_t = unsigned short;

	enum Direction { UP, DOWN, RIGHT, LEFT };


	ComboBox(QWidget* parent = nullptr,
			 Direction direction = Direction::DOWN,
			 Qt::Alignment alignment = Qt::AlignCenter);


	index_t addItem(const QString& text);

	void addItems(const QStringList& list);


	void setCurrent(index_t index);

	QString getCurrent() const;

	index_t getCurrentIndex() const;


	void setDirection(int direction);

	int getDirection() const;


	void setAlignment(Qt::Alignment alignment);

	Qt::Alignment getAlignment() const;


private slots:

	void popup();

private:

	void paintEvent(QPaintEvent*) override;

	void resizeEvent(QResizeEvent* event) override;


	index_t current_;

	Qt::Alignment alignment_;

	int direction_;

	QPushButton* button_;

	QListView* view_;

	QStandardItemModel* model_;

};

#endif // CUSTOMCOMBOBOX_HPP

