#ifndef QUCLEANUPTASK_H_
#define QUCLEANUPTASK_H_

#include "QUSimpleTask.h"

class QUCleanUpTask: public QUSimpleTask {
	Q_OBJECT
	
public:
	enum TaskMode {
		UnusedFiles,
		InvalidFileTags,
		RemoveEndTag
	};
	Q_DECLARE_FLAGS(TaskModes, TaskMode)

	QUCleanUpTask(TaskModes mode, QObject *parent = 0);
	
	virtual void startOn(QUSongInterface *song);
	virtual QList<QUSmartSetting*> smartSettings() const;
	
private:
	TaskModes _mode;
	mutable QList<QUSmartSetting*> _smartSettings;
};

#endif /*QUCLEANUPTASK_H_*/
