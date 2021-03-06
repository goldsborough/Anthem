#ifndef BROWSER_HPP
#define BROWSER_HPP

#include <QWidget>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QSortFilterProxyModel>

class QVariant;
class QTreeView;
class QHBoxLayout;
class QMenu;
class QPushButton;
class QLineEdit;

class IconProvider : public QFileIconProvider
{

public:

	virtual QIcon icon(IconType type) const override;

	virtual QIcon icon(const QFileInfo& item) const override;
};

class FileModel : public QFileSystemModel
{

public:

	explicit FileModel(QWidget* parent = nullptr);

	virtual QVariant data (const QModelIndex & index,
						   int role = Qt::DisplayRole) const override;
};

class Proxy : public QSortFilterProxyModel
{
public:

	explicit Proxy(QWidget* parent = nullptr);

	virtual bool filterAcceptsRow(int source_row,
								  const QModelIndex &source_parent) const override;
};

class Browser : public QWidget
{
	Q_OBJECT

public:

	explicit Browser(QWidget* parent = nullptr);

signals:

	void wavetableSelected(const QString& id) const;

private slots:

	void itemClicked(const QModelIndex& index);

private:

	Browser(bool isExpanded, QWidget* parent);


	virtual void paintEvent(QPaintEvent*) override;

	void setupUi();

	void setupBar();

	void setupExpansion();

	void setupFilter();

	void setupModel();

	void setupView();

	void setupContextMenu();


	void renameAction_();

	void deleteAction_();

	void newFolderAction_();


	QHBoxLayout* barLayout_;

	QLineEdit* filter_;

	FileModel* model_;

	Proxy* proxy_;

	QTreeView* view_;

	QMenu* context_;

	QDialog* expandedDialog_;

	QPushButton* expandButton_;


	bool isExpanded_;

};

#endif // BROWSER_HPP
