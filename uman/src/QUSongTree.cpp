#include "QUSongTree.h"
#include "QUSongItem.h"
#include "QUSongFile.h"
#include "QUMainWindow.h"

#include <QMessageBox>
#include <QUrl>
#include <QIcon>
#include <QMenu>
#include <QList>
#include <QRegExp>

#include "QUProgressDialog.h"

#define FILE_DROP_LIMIT 10

QUSongTree::QUSongTree(QWidget *parent): QTreeWidget(parent) {
	this->setAcceptDrops(true);
	this->setContextMenuPolicy(Qt::CustomContextMenu);
	
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
}

void QUSongTree::initHorizontalHeader() {
	QTreeWidgetItem *header = new QTreeWidgetItem();
	header->setText(0, QString("Folder (%1)").arg(QUMainWindow::BaseDir.path()));
	header->setIcon(0, QIcon(":/types/folder.png"));

	header->setIcon(1, QIcon(":/types/user.png"));
	header->setToolTip(1, tr("Shows whether your folder includes the artist correctly:<br><i>Artist - Title ...</i>"));
	header->setIcon(2, QIcon(":/types/font.png"));
	header->setToolTip(2, tr("Shows whether your folder includes the title correctly:<br><i>Artist - Title ...</i>"));
	header->setIcon(3, QIcon(":/types/music.png"));
	header->setToolTip(3, tr("Shows whether the song text file points to an <b>audio file</b> that can be found by UltraStar"));
	header->setIcon(4, QIcon(":/types/picture.png"));
	header->setToolTip(4, tr("Shows whether the song text file points to a <b>cover picture</b> that can be found by UltraStar"));
	header->setIcon(5, QIcon(":/types/picture.png"));
	header->setToolTip(5, tr("Shows whether the song text file points to a <b>background picture</b> that can be found by UltraStar"));
	header->setIcon(6, QIcon(":/types/film.png"));
	header->setToolTip(6, tr("Shows whether the song text file points to a <b>video file</b> that can be found by UltraStar"));

	header->setIcon(7, QIcon(":/types/unused_files.png"));
	header->setToolTip(7, tr("Shows whether your folder contains unused files."));
	
	header->setText(8, tr("Language"));
	header->setIcon(8, QIcon(":/types/language.png"));
	header->setText(9, tr("Edition"));
	header->setIcon(9, QIcon(":/types/edition.png"));
	header->setText(10, tr("Genre"));
	header->setIcon(10, QIcon(":/types/genre.png"));
	header->setText(11, tr("Year"));
	header->setIcon(11, QIcon(":/types/date.png"));
	header->setText(12, tr("Creator"));
	header->setIcon(12, QIcon(":/types/creator.png"));
	header->setText(13, tr("Comment"));
	header->setIcon(13, QIcon(":/types/comment.png"));
	
	this->setHeaderItem(header);	
}

bool QUSongTree::hasUnsavedChanges() const {
	bool result = false;
	
	for(int i = 0; i < this->topLevelItemCount(); i++) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(this->topLevelItem(i));
		
		if(songItem)
			result |= songItem->song()->hasUnsavedChanges();
	}
	
	return result;
}

/*!
 * Overloaded to clear hidden items.
 */
void QUSongTree::clear() {
	qDeleteAll(_hiddenItems);
	_hiddenItems.clear();
	QTreeWidget::clear();
}

void QUSongTree::saveUnsavedChanges() {
	QUProgressDialog dlg(tr("Saving unsaved changes..."), this->topLevelItemCount(), this);
	dlg.show();
	
	for(int i = 0; i < this->topLevelItemCount(); i++) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(this->topLevelItem(i));
		
		if(songItem) {
			dlg.update(songItem->song()->songFileInfo().fileName());
			
			if(songItem->song()->hasUnsavedChanges()) {
				songItem->song()->save(true);
				songItem->update();
			}
		}
	}
	
	emit itemSelectionChanged(); // update details
}

/*!
 * Hides all items from the tree which do not match the filter. An empty string
 * will remove the filter and show all items again. Selected items will be
 * unselected if hidden.
 */
void QUSongTree::filterItems(const QString &regexp) {
	this->addTopLevelItems(_hiddenItems);
	_hiddenItems.clear();

	if(regexp.isEmpty()) {	
		emit finished(tr("Filter removed. All songs are visible now."), QU::information);
		return;
	}
	
	QRegExp rx(regexp, Qt::CaseInsensitive);
	
	// collect all toplevel items
	QList<QTreeWidgetItem*> topLevelItems;
	for(int i = 0; i < this->topLevelItemCount(); i++)
		topLevelItems.append(this->topLevelItem(i));
	
	QUProgressDialog progress("Applying filter...", topLevelItems.size(), this);
	progress.setPixmap(":/control/filter.png");
	progress.show();
	
	foreach(QTreeWidgetItem *item, topLevelItems) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(item);
		
		if(songItem) {
			QUSongFile *song = songItem->song();
			
			progress.update(QString("%1 - %2").arg(song->artist()).arg(song->title()));
			
			if( !(
				song->artist().contains(rx) ||
				song->title().contains(rx) ||
				song->genre().contains(rx) ||
				song->year().contains(rx) ||
				song->edition().contains(rx) ||
				song->creator().contains(rx) ||
				song->language().contains(rx)
			) )
				_hiddenItems.append(this->takeTopLevelItem(this->indexOfTopLevelItem(item)));
		}
	}
	
	emit itemSelectionChanged(); // update details
	emit finished(QString(tr("Filter applied: \"%1\"")).arg(regexp), QU::information);
}

bool QUSongTree::dropMimeData (QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action) {
	if(data->urls().isEmpty())
		return false;
	
	if(!parent)
		return dropSongFiles(data->urls());
	
	QUSongItem *item = dynamic_cast<QUSongItem*>(parent);
	
	if(!item)
		return false;
	
	return this->copyFilesToSong(data->urls(), item);
}

QStringList QUSongTree::mimeTypes() const {
	return QStringList("text/uri-list");
}

/*!
 * Shows a context menu with actions for all selected items/songs.
 */
void QUSongTree::showContextMenu(const QPoint &point) {
	if(!this->itemAt(point))
		return; // no item clicked
	
	QMenu menu(this);
	
	menu.addAction(QIcon(":/control/refresh.png"), tr("Refresh"), this, SLOT(refreshSelectedItems()), QKeySequence::fromString("F5"));
	menu.addAction(QIcon(":/control/save.png"), tr("Save"), this, SLOT(saveSelectedSongs()), QKeySequence::fromString("Ctrl+S"));
	
	this->fillContextMenu(menu, point);
	
	menu.exec(this->mapToGlobal(point));
}

/*!
 * Appends actions to the menu according to the selected item.
 */
void QUSongTree::fillContextMenu(QMenu &menu, const QPoint &point) {
	QUSongItem *item = dynamic_cast<QUSongItem*>(this->currentItem());
	
	if(!item || item->isToplevel())
		return;
	
	menu.addSeparator();
	menu.addAction(QIcon(":/control/bin.png"), tr("Delete"), this, SLOT(deleteCurrentItem()), QKeySequence::fromString("Del"));
}

bool QUSongTree::copyFilesToSong(const QList<QUrl> &files, QUSongItem *item) {
	if(files.size() > FILE_DROP_LIMIT)
		if(QMessageBox::question(this, tr("Confirm Copy Operation"), QString(tr("You want to copy %1 files. Are you sure?")).arg(files.size()), 
			QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
			return false;
	
	bool dataUsed = false;
	
	QUProgressDialog dlg(QString("Copy & Use files for the song: \"%1 - %2\"...").arg(item->song()->artist()).arg(item->song()->title()), files.size(), this);
	dlg.setPixmap(":/marks/disk.png");
	dlg.show();
	
	foreach(QUrl url, files) {
		dlg.update(url.toLocalFile());
		
		if(!QFileInfo(url.toLocalFile()).isDir()) {
			item->song()->useExternalFile(url.toLocalFile());
			dataUsed = true;
		}
	}
	
	if(dataUsed) {
		item->song()->save();
		item->update();
		emit itemSelectionChanged(); // update details
	}
	
	return true;
}

/*!
 * Create new song directories for dropped song text files and copy that file to
 * the new created directory.
 */
bool QUSongTree::dropSongFiles(const QList<QUrl> &urls) {	
	foreach(QUrl url, urls) {
		QFileInfo fi(url.toLocalFile());
		
		if(fi.suffix() == SONG_FILE_SUFFIX) {
			QUSongFile tmp(fi.filePath());
			
			QString newSongDirName = QString("%1 - %2").arg(tmp.artist()).arg(tmp.title());
			
			if(QUMainWindow::BaseDir.mkdir(newSongDirName)) {
				QDir newSongDir(QUMainWindow::BaseDir); 
				if(newSongDir.cd(newSongDirName)) {
					QFileInfo newFi(newSongDir, fi.fileName());
					if(QFile::copy(fi.filePath(), newFi.filePath())) {
						QUSongFile *newSong = new QUSongFile(newFi.filePath());
						QUSongItem *newItem = new QUSongItem(newSong, true);
						this->addTopLevelItem(newItem);
						
						newItem->setSelected(true);
						
						emit finished(QString(tr("New song included to your song collection: \"%1 - %2\".")).arg(newSong->artist()).arg(newSong->title()), QU::information);
						emit songCreated(newSong);
					} else {
						emit finished(QString(tr("Could not copy song file \"%1\" to new song directory \"%2\"!")).arg(fi.fileName()).arg(newSongDirName), QU::warning);
					}
				} else {
					emit finished(QString(tr("Could not change to new song directory \"%1\"!")).arg(newSongDirName), QU::warning);
				}
			} else {
				emit finished(QString(tr("Could not create new song directory \"%1\"!")).arg(newSongDirName), QU::warning);
			}
		}
	}
	
	return true;
}

void QUSongTree::refreshSelectedItems() {
	QList<QTreeWidgetItem*> items = this->selectedItems();
	
	if(items.isEmpty())
		items.append(this->currentItem());
	
	QUProgressDialog dlg("Refreshing selected songs...", items.size(), this);
	dlg.setPixmap(":/types/folder.png");
	dlg.show();
	
	foreach(QTreeWidgetItem *item, items) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(item);
		
		if(songItem) {
			dlg.update(songItem->song()->songFileInfo().dir().dirName());
			
			songItem->update();
		}
	}
	
	emit itemSelectionChanged(); // update details
}

void QUSongTree::saveSelectedSongs() {
	QList<QTreeWidgetItem*> items = this->selectedItems();
	
	if(items.isEmpty())
		items.append(this->currentItem());
	
	QUProgressDialog dlg("Saving selected songs...", items.size(), this);
	dlg.show();
	
	foreach(QTreeWidgetItem *item, items) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(item);
		
		if(songItem) {
			dlg.update(songItem->song()->songFileInfo().fileName());
			
			songItem->song()->save(true);
			songItem->update();
		}
	}
	
	emit itemSelectionChanged(); // update details
}

void QUSongTree::keyPressEvent(QKeyEvent *event) {
	if(event->key() == Qt::Key_Delete) {		
		this->deleteCurrentItem();
	} else {
		QTreeWidget::keyPressEvent(event);
	}
}

/*!
 * Delete the current item, which should represent a file and not a folder
 * because folder items can be selected and are top-level.
 */
void QUSongTree::deleteCurrentItem() {
	QUSongItem *item = dynamic_cast<QUSongItem*>(this->currentItem());
	
	if(!item || item->isToplevel())
		return; // only allow to delete files which cannot be selected - no directories
	
	if( QMessageBox::question(this, tr("Confirm Delete Operation"), QString(tr("Do you really want to delete \"%1\"?")).arg(item->text(0)),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No )
		return;
	
	if(QFile::remove(QFileInfo(item->song()->songFileInfo().dir(), item->text(0)).filePath()))
		emit finished(QString(tr("The file \"%1\" was deleted successfully.")).arg(item->text(0)), QU::information);
	else
		emit finished(QString(tr("The file \"%1\" was NOT deleted.")).arg(item->text(0)), QU::warning);
	
	this->setCurrentItem(item->parent());
	item->update();
	
	emit itemSelectionChanged(); // update details
}
