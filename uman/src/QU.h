#ifndef QU_H_
#define QU_H_

#include <QObject>
#include <QFileInfo>

#define N_A "-"
#define NONE "(none)"

#define CHAR_UTF8_APPROX "\xe2\x89\x88"
#define CHAR_UTF8_NEQUAL "\xe2\x89\xa0"
#define CHAR_UTF8_DOT    "\xc2\xb7"

// used for time comparison and fixing
#define LOWER_TIME_BOUND_DEFAULT 15
#define UPPER_TIME_BOUND_DEFAULT 30

//! This class provides global flags and enumerations.
class QU: QObject {
	Q_OBJECT
public:
	QU(QObject *parent = 0);

	static QDir BaseDir; // UltraStar song folder

	enum EventMessageType {
		information,
		warning,
		help,
		saving,
		error
	};
	Q_DECLARE_FLAGS(EventMessageTypes, EventMessageType)

	enum PreparatoryTaskMode {
		autoAssignFiles,
		removeUnsupportedTags,
		fixAudioLength,
		roundGap
	};
	Q_DECLARE_FLAGS(PreparatoryTaskModes, PreparatoryTaskMode)

	enum LyricTaskMode {
		fixTimeStamps,
		fixSpaces,
		removeEmptySyllables,
		convertSyllablePlaceholder1,
		convertSyllablePlaceholder2
	};
	Q_DECLARE_FLAGS(LyricTaskModes, LyricTaskMode)

	enum CleanTaskMode {
		unusedFiles,
		invalidFileTags,
		removeEndTag
	};
	Q_DECLARE_FLAGS(CleanTaskModes, CleanTaskMode)

	enum FilterMode {
		informationTags = 0x01,
		fileTags = 0x02,
		controlTags = 0x04,
		customTags = 0x08,
		negateFilter = 0x10
	};
	Q_DECLARE_FLAGS(FilterModes, FilterMode)

	enum ScriptableTaskType {
		audioTagTask,
		renameTask
	};
	Q_DECLARE_FLAGS(ScriptableTaskTypes, ScriptableTaskType)

	enum ImageSize {
		mediumImage,
		largeImage
	};
	Q_DECLARE_FLAGS(ImageSizes, ImageSize)

	enum SpellState {
		spellingOk,
		spellingWarning,
		spellingError
	};
	Q_DECLARE_FLAGS(SpellStates, SpellState)

	enum ReportOption {
		reportPrependCurrentPath = 0x01,
		reportPrependUserData = 0x02,
		reportAppendLyrics = 0x04,
		reportLinkLyrics = 0x08,
		reportUseStyleSheet = 0x10
	};
	Q_DECLARE_FLAGS(ReportOptions, ReportOption)

	enum TaskDataType {
		undefined = -1,
		unknownTags = 0
	};
	Q_DECLARE_FLAGS(TaskDataTypes, TaskDataType)

	static bool fileTypeLessThan(const QFileInfo &fi1, const QFileInfo &fi2);  
};

#endif /*QU_H_*/
