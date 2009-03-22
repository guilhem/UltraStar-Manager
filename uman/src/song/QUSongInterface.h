#ifndef QUSONGINTERFACE_H_
#define QUSONGINTERFACE_H_

#define SONG_API_VERSION 1

#include <QObject>

class QString;
class QStringList;
class QFileInfo;

#define TITLE_TAG      "TITLE"
#define ARTIST_TAG     "ARTIST"
#define LANGUAGE_TAG   "LANGUAGE"
#define EDITION_TAG    "EDITION"
#define GENRE_TAG      "GENRE"
#define YEAR_TAG       "YEAR"
#define CREATOR_TAG    "CREATOR"
#define MP3_TAG        "MP3"
#define COVER_TAG      "COVER"
#define BACKGROUND_TAG "BACKGROUND"
#define VIDEO_TAG      "VIDEO"
#define VIDEOGAP_TAG   "VIDEOGAP"
#define START_TAG      "START"
#define END_TAG        "END"
#define RELATIVE_TAG   "RELATIVE"
#define BPM_TAG        "BPM"
#define GAP_TAG        "GAP"

class QUSongNoteInterface: public QObject {
	Q_OBJECT

public:
	enum Type {
		normal,
		golden,
		freestyle,
	};
	Q_DECLARE_FLAGS(Types, Type)

	QUSongNoteInterface(QObject *parent = 0): QObject(parent) {}

	virtual int timestamp() const = 0;
	virtual void setTimestamp(int t) = 0;

	virtual int duration() const = 0;
	virtual void setDuration(int d) = 0;

	virtual int pitch() const = 0;
	virtual void setPitch(int p) = 0;

	virtual Types type() const = 0;
	virtual void setType(Types t) = 0;

	virtual QString syllable() const = 0;
	virtual void setSyllable(const QString &s) = 0;

	virtual void resetTrailingSpaces(int prefixCount, int suffixCount) = 0;
};

class QUSongLineInterface: public QObject {
	Q_OBJECT

public:
	QUSongLineInterface(QObject *parent = 0): QObject(parent) {}

	virtual void addNote(QUSongNoteInterface *note) = 0;
	virtual QList<QUSongNoteInterface*>& notes() = 0;

	virtual int outTime() const = 0;
	virtual void setOutTime(int t) = 0;

	virtual int inTime() const = 0;
	virtual void setInTime(int t) = 0;

	virtual bool useOutTime() const = 0;
	virtual bool useInTime() const = 0;
	virtual void removeInTime() = 0;
};

class QUSongInterface: public QObject {
    Q_OBJECT

    // normal tag properties
	Q_PROPERTY(QString artist READ artist)
	Q_PROPERTY(QString title READ title)
	Q_PROPERTY(QString mp3 READ mp3)
	Q_PROPERTY(QString bpm READ bpm)
	Q_PROPERTY(QString gap READ gap)
	Q_PROPERTY(QString video READ video)
	Q_PROPERTY(QString videogap READ videogap)
	Q_PROPERTY(QString cover READ cover)
	Q_PROPERTY(QString background READ background)
	Q_PROPERTY(QString start READ start)
	Q_PROPERTY(QString language READ language)
	Q_PROPERTY(QString relative READ relative)
	Q_PROPERTY(QString edition READ edition)
	Q_PROPERTY(QString genre READ genre)
	Q_PROPERTY(QString year READ year)
	Q_PROPERTY(QString end READ end)
	Q_PROPERTY(QString creator READ creator)
	// additional properties
	Q_PROPERTY(QString dir READ dir)
	Q_PROPERTY(QString path READ path)
	Q_PROPERTY(QString filePath READ filePath)
	Q_PROPERTY(QString relativeFilePath READ relativeFilePath)
	Q_PROPERTY(QString txt READ txt)
	Q_PROPERTY(QString lengthTotal READ lengthEffectiveFormatted)
	Q_PROPERTY(QString speed READ speedFormatted)

	Q_PROPERTY(bool hasMp3 READ hasMp3)
	Q_PROPERTY(bool hasCover READ hasCover)
	Q_PROPERTY(bool hasBackground READ hasBackground)
	Q_PROPERTY(bool hasVideo READ hasVideo)
	Q_PROPERTY(bool isSongChecked READ isSongChecked)

public:
    QUSongInterface(QObject *parent = 0): QObject(parent) {}

    virtual void log(const QString &message, int type) = 0;

	virtual QString artist() const = 0;
	virtual QString title() const = 0;
	virtual QString mp3() const = 0;
	virtual QString bpm() const = 0;
	virtual QString gap() const = 0;
	virtual QString video() const = 0;
	virtual QString videogap() const = 0;
	virtual QString cover() const = 0;
	virtual QString background() const = 0;
	virtual QString start() const = 0;
	virtual QString language() const = 0;
	virtual QString relative() const = 0;
	virtual QString edition() const = 0;
	virtual QString genre() const = 0;
	virtual QString year() const = 0;
	virtual QString end() const = 0;
	virtual QString creator() const = 0;

	virtual QString customTag(const QString &tag) const = 0;

	virtual QString dir() const = 0;
	virtual QString path() const = 0;
	virtual QString filePath() const = 0;
	virtual QString relativeFilePath() const = 0;
	virtual QString txt() const = 0;

	virtual bool hasMp3() const = 0;
	virtual bool hasCover() const = 0;
	virtual bool hasBackground() const = 0;
	virtual bool hasVideo() const = 0;
	virtual bool isSongChecked() const = 0;

	virtual int length() = 0;
	virtual int lengthMp3() const = 0;
	virtual int lengthEffective() const = 0;
	virtual double syllablesPerSecond(bool firstCalc = false) = 0;

	virtual QString lengthEffectiveFormatted() const = 0;
	virtual QString speedFormatted() = 0;

	virtual QStringList lyrics() const = 0;

	virtual QFileInfo songFileInfo() const = 0;

	virtual QFileInfo mp3FileInfo() const = 0;
	virtual QFileInfo coverFileInfo() const = 0;
	virtual QFileInfo backgroundFileInfo() const = 0;
	virtual QFileInfo videoFileInfo() const = 0;

	virtual void setInfo(const QString &key, const QString &value) = 0;

	virtual void renameSongDir(const QString &newName) = 0;
	virtual void renameSongTxt(const QString &newName) = 0;
	virtual void renameSongMp3(const QString &newName) = 0;
	virtual void renameSongCover(const QString &newName) = 0;
	virtual void renameSongBackground(const QString &newName) = 0;
	virtual void renameSongVideo(const QString &newName) = 0;

	virtual void useID3TagForArtist() = 0;
	virtual void useID3TagForTitle() = 0;
	virtual void useID3TagForGenre() = 0;
	virtual void useID3TagForYear() = 0;

	virtual void removeUnsupportedTags() = 0;

	virtual void useExternalFile(const QString &filePath) = 0;
	virtual void autoSetFiles() = 0;
	virtual void autoSetFile(const QFileInfo &fi, bool force = false) = 0;

	virtual void deleteUnusedFiles() = 0;
	virtual void clearInvalidFileTags() = 0;

	virtual void moveAllFiles(const QString &newRelativePath) = 0;

	virtual void fixAudioLength() = 0;
	virtual void roundGap() = 0;
	virtual void removeEndTag() = 0;

	virtual QList<QUSongLineInterface*>& loadMelody() = 0;
	virtual void clearMelody() = 0;
	virtual void saveMelody() = 0;
};

#endif /* QUSONGINTERFACE_H_ */