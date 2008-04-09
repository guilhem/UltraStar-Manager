#include "main.h"
#include "QUDetailsTable.h"

#include "QUDropDownDelegate.h"
#include "QUTagItem.h"
#include "QUDetailItem.h"

#include <QStringList>
#include <QHeaderView>
#include <QFont>
#include <QMessageBox>

QUDetailsTable::QUDetailsTable(QWidget *parent): QTableWidget(parent) {
	this->setRowCount(20);
	this->setColumnCount(2);
	
	// setup headers
	this->verticalHeader()->hide();
	this->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	
	this->setHorizontalHeaderLabels(QStringList() << "Tag" << "Value");
	this->horizontalHeader()->setResizeMode(0, QHeaderView::Interactive);
	this->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
	
	// set value editor
	QUDropDownDelegate *comboDelegate = new QUDropDownDelegate(this);
	this->setItemDelegateForColumn(1, comboDelegate);
	
	// setup tag & value column
	this->initTagColumn();
	this->initValueColumn();
}

void QUDetailsTable::initTagColumn() {	
	this->initSeparator(tr("Info Tags"), 0);
	this->setItem(1, 0, new QUTagItem(QIcon(":/types/font.png"), tr("Title")));
	this->setItem(2, 0, new QUTagItem(QIcon(":/types/user.png"), tr("Artist")));
	this->setItem(3, 0, new QUTagItem(QIcon(":/types/language.png"), tr("Language")));
	this->setItem(4, 0, new QUTagItem(QIcon(":/types/edition.png"), tr("Edition")));
	this->setItem(5, 0, new QUTagItem(QIcon(":/types/genre.png"), tr("Genre")));
	this->setItem(6, 0, new QUTagItem(QIcon(":/types/date.png"), tr("Year")));
	this->setItem(7, 0, new QUTagItem(QIcon(":/types/creator.png"), tr("Creator")));
	
	this->initSeparator(tr("File Tags"), 8);
	this->setItem(9, 0, new QUTagItem(QIcon(":/types/music.png"), tr("MP3")));
	this->setItem(10, 0, new QUTagItem(QIcon(":/types/picture.png"), tr("Cover")));
	this->setItem(11, 0, new QUTagItem(QIcon(":/types/picture.png"), tr("Background")));
	this->setItem(12, 0, new QUTagItem(QIcon(":/types/film.png"), tr("Video")));

	this->initSeparator(tr("Control Tags"), 13);
	
	this->setItem(14, 0, new QUTagItem(QIcon(":/bullets/bullet_black.png"), tr("Videogap")));
	this->setItem(15, 0, new QUTagItem(QIcon(":/bullets/bullet_black.png"), tr("Start")));
	this->setItem(16, 0, new QUTagItem(QIcon(":/bullets/bullet_black.png"), tr("End")));
	this->setItem(17, 0, new QUTagItem(QIcon(":/bullets/bullet_black.png"), tr("Relative")));
	this->setItem(18, 0, new QUTagItem(QIcon(":/bullets/bullet_black.png"), tr("BPM")));
	this->setItem(19, 0, new QUTagItem(QIcon(":/bullets/bullet_black.png"), tr("Gap")));
}

void QUDetailsTable::initValueColumn() {
	// separator here
	
	this->setItem(1, 1, new QUDetailItem(TITLE_TAG));
	this->setItem(2, 1, new QUDetailItem(ARTIST_TAG));
	this->setItem(3, 1, new QUDetailItem(LANGUAGE_TAG));
	this->setItem(4, 1, new QUDetailItem(EDITION_TAG));
	this->setItem(5, 1, new QUDetailItem(GENRE_TAG));
	this->setItem(6, 1, new QUDetailItem(YEAR_TAG));
	this->setItem(7, 1, new QUDetailItem(CREATOR_TAG));

	// separator here
	
	this->setItem(9, 1, new QUDetailItem(MP3_TAG));
	this->setItem(10, 1, new QUDetailItem(COVER_TAG));
	this->setItem(11, 1, new QUDetailItem(BACKGROUND_TAG));
	this->setItem(12, 1, new QUDetailItem(VIDEO_TAG));
	
	// separator here

	this->setItem(14, 1, new QUDetailItem(VIDEOGAP_TAG));
	this->setItem(15, 1, new QUDetailItem(START_TAG));
	this->setItem(16, 1, new QUDetailItem(END_TAG));
	this->setItem(17, 1, new QUDetailItem(RELATIVE_TAG));
	this->setItem(18, 1, new QUDetailItem(BPM_TAG));
	this->setItem(19, 1, new QUDetailItem(GAP_TAG));
}

void QUDetailsTable::initSeparator(const QString &text, int row) {
	QTableWidgetItem *separator = new QTableWidgetItem(text);
	
	separator->setFlags(Qt::ItemIsEnabled);
	separator->setBackgroundColor(Qt::darkGray);
	separator->setTextColor(Qt::white);
	separator->setTextAlignment(Qt::AlignCenter);
	
	QFont font(separator->font());
	font.setBold(true);
	
	separator->setFont(font);
	
	this->setItem(row, 0, separator);
	this->setSpan(row, 0, 1, 2);
	
	this->verticalHeader()->setResizeMode(row, QHeaderView::Fixed);
	this->verticalHeader()->resizeSection(row, 16);
	
}

void QUDetailsTable::updateValueColumn(const QList<QUSongFile*> &songs) {	
	for(int i = 0; i < this->rowCount(); i++) {
		QUDetailItem *detailItem = dynamic_cast<QUDetailItem*>(this->item(i, 1));
		
		if(!detailItem)
			continue;
		
		detailItem->setSongs(songs);
	}
	
	// disable user interaction if no song is selected
	if(songs.isEmpty())
		this->setEnabled(false);
	else
		this->setEnabled(true);
}
