SOURCES = QUAudioTagTaskFactory.cpp \
    QUAudioTagTask.cpp \
    ../../QUStringSupport.cpp \
    ../../QUSongSupport.cpp \
    ../shared/QUSimpleTask.cpp \
    ../shared/QUScriptableTask.cpp \
    ../shared/QUScriptableTaskFactory.cpp \
    QUAudioTagTaskDialog.cpp \
    QUAudioTagTaskSourceDelegate.cpp \
    ../shared/QUTaskDialog.cpp \
    ../shared/QUTaskDataTable.cpp \
    ../shared/QUTaskConditionDelegate.cpp \
    ../shared/QUTaskSourceDelegate.cpp \
    QUDefaultDelegate.cpp
HEADERS = QUAudioTagTaskFactory.h \
    QUAudioTagTask.h \
    ../../QUStringSupport.h \
    ../../QUSongSupport.h \
    ../shared/QUSimpleTask.h \
    ../shared/QUScriptableTask.h \
    ../shared/QUScriptableTaskFactory.h \
    QUAudioTagTaskDialog.h \
    QUAudioTagTaskSourceDelegate.h \
    ../shared/QUTaskDialog.h \
    ../shared/QUTaskDataTable.h \
    ../shared/QUTaskConditionDelegate.h \
    ../shared/QUTaskSourceDelegate.h \
    QUDefaultDelegate.h
FORMS += ../shared/QUTaskDialog.ui
INCLUDEPATH += . \
    ui \
    .. \
    ../.. \
    ../../song \
    ../shared \
    ../../../include/taglib
TARGET = audiotag
TEMPLATE = lib
CONFIG += plugin
CONFIG -= debug
DESTDIR = ../../../bin/wip/plugins
OBJECTS_DIR = tmp
MOC_DIR = tmp
QT += xml
win32 { 
    INCLUDEPATH += ../../../include/taglib
    LIBS += -L"../../../lib" \
        -ltag
}
unix { 
    INCLUDEPATH += /usr/include/taglib
    LIBS += -L"/usr/lib" \
        -ltag
}
