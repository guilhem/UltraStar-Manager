#ifndef QURENAMETASKFACTORY_H_
#define QURENAMETASKFACTORY_H_

#include <QObject>
#include <QDomDocument>
#include <QMap>

#include "QUScriptableTaskFactory.h"

class QURenameTaskFactory: public QUScriptableTaskFactory {
	Q_OBJECT

public:
	QURenameTaskFactory(QObject *parent = 0);

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

#endif // QURENAMETASKFACTORY_H_
