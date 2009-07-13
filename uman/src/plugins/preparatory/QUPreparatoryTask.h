#ifndef QUPREPARATORYTASK_H_
#define QUPREPARATORYTASK_H_

#include "QUSimpleTask.h"

#include <QList>
#include <QStringList>

class QUSmartSetting;

class QUPreparatoryTask : public QUSimpleTask {
	Q_OBJECT
	
public:
	enum TaskMode {
		AutoAssignFiles,
		RemoveUnsupportedTags,
		FixAudioLength,
		RoundGap
	};
	Q_DECLARE_FLAGS(TaskModes, TaskMode)

	QUPreparatoryTask(TaskModes mode, QObject *parent = 0);
	~QUPreparatoryTask();
	
	virtual void startOn(QUSongInterface *song);
	virtual QList<QUSmartSetting*> smartSettings() const;
	virtual void provideData(const QVariant &data, TaskDataTypes type = UnsupportedTags);

private:
	TaskModes _mode;

	mutable QList<QUSmartSetting*> _smartSettings;
	QStringList _unsupportedTags;

	void autoSetFiles(QUSongInterface *song, const QString &coverPattern, const QString &backgroundPattern);
};

#endif /*QUPREPARATORYTASK_H_*/
