#ifndef AUDIOTAGTASKFACTORY_H_
#define AUDIOTAGTASKFACTORY_H_

#include <QObject>
#include <QDomDocument>

#include "QUScriptableTaskFactory.h"

class QUAudioTagTaskFactory: public QUScriptableTaskFactory {
	Q_OBJECT

public:
	QUAudioTagTaskFactory(QObject *parent = 0);

	virtual QString versionString() const { return "1.0.0"; }
	virtual QString name() const;
	virtual QString productName() const;

public slots:
	virtual int addConfiguration(QWidget *parent = 0);

protected:
	virtual QDir configurationDirectory();
	virtual QUTask* createTask(QDomDocument *configuration);
	virtual QMap<QString, QString> translationLocations() const;
};

#endif // AUDIOTAGTASKFACTORY_H_
