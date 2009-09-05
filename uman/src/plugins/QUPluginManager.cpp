#include "QUPluginManager.h"
#include "QULogService.h"
#include "QUTaskFactoryProxy.h"

#include <QCoreApplication>
#include <QPluginLoader>
#include <QSettings>
#include <QLocale>

QUPluginManager::QUPluginManager(QObject *parent): QObject(parent) {}
QUPluginManager* QUPluginManager::_instance = 0;
QUPluginManager* QUPluginManager::instance() {
	if(_instance == 0) _instance = new QUPluginManager(0);
	return _instance;
}

/* ----------------------------------------------------------------- */

QDir QUPluginManager::pluginFolder() {
	QDir result = QCoreApplication::applicationDirPath();
	if(!result.cd("plugins"))
		logSrv->add(tr("Could NOT open default plugin folder."), QU::Warning);
	return result;
}

void QUPluginManager::reload() {
	unloadAllPlugins();
	loadAllPlugins();

	logSrv->add(tr("%1 plugins loaded.").arg(_plugins.size()), QU::Information);
	emit reloaded();
}

void QUPluginManager::setUiParent(QWidget *w) {
	if(w != _uiParent) {
		removeTaskFactoryProxies();
		_uiParent = w;
		createTaskFactoryProxies();
	}
}

void QUPluginManager::loadAllPlugins() {
	if(!_plugins.isEmpty()) {
		logSrv->add(tr("Plugin loading failed. Unload loaded plugins first."), QU::Error);
		return;
	}

	QFileInfoList pluginFiList = pluginFolder().entryInfoList(QStringList("*.*"), QDir::Files, QDir::Name);

	foreach(QFileInfo pluginFi, pluginFiList) {
		if(!QLibrary::isLibrary(pluginFi.filePath()))
			continue;

		QPluginLoader *ldr = new QPluginLoader(pluginFi.filePath(), this);
		if(!ldr->load()) {
			logSrv->add(ldr->errorString(), QU::Warning);
		} else {
			_plugins << ldr;
		}
	}

	createTaskFactoryProxies();
}

void QUPluginManager::unloadAllPlugins() {
	removeTaskFactoryProxies();

	foreach(QPluginLoader *plugin, _plugins)
		plugin->unload();

	qDeleteAll(_plugins);
	_plugins.clear();
}

void QUPluginManager::loadTranslationsForTasks() {
	QString language = QSettings().value("language", QLocale::system().name()).toString();

	foreach(QUTaskFactoryProxy *factoryProxy, _taskFactoryProxies) {
		// only translator for current language should be installed
		QMap<QString, QTranslator*> translations(factoryProxy->factory()->translations());
		if(translations.contains(language))
			qApp->installTranslator(translations.value(language));
	}
}

void QUPluginManager::unloadTranslationsForTasks() {
	QString language = QSettings().value("language", QLocale::system().name()).toString();

	foreach(QUTaskFactoryProxy *factoryProxy, _taskFactoryProxies) {
		// only translator for current language should be installed
		QMap<QString, QTranslator*> translations(factoryProxy->factory()->translations());
		if(translations.contains(language))
			qApp->removeTranslator(translations.value(language));
	}
}

void QUPluginManager::createTaskFactoryProxies() {
	if(!_taskFactoryProxies.isEmpty()) {
		logSrv->add(tr("Could not create plugin proxies. Unload them first."), QU::Error);
		return;
	}

	foreach(QPluginLoader *ldr, _plugins) {
		QUTaskFactory *factory = qobject_cast<QUTaskFactory*>(ldr->instance());
		if(factory) {
			_taskFactoryProxies << new QUTaskFactoryProxy(factory, _uiParent, this);
			connect(_taskFactoryProxies.last(), SIGNAL(configurationAdded()), this, SIGNAL(configurationAdded()));
		}
	}
	loadTranslationsForTasks();
}

void QUPluginManager::removeTaskFactoryProxies() {
	unloadTranslationsForTasks();

	qDeleteAll(_taskFactoryProxies);
	_taskFactoryProxies.clear();
}

void QUPluginManager::updateTaskFactoryProxies() {
	removeTaskFactoryProxies();
	createTaskFactoryProxies();
}
