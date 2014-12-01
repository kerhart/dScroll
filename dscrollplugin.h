#pragma once
#ifndef DSCROLL_H
#define DSCROLL_H

#include "dscroll_global.h"

#include <extensionsystem/iplugin.h>
#include <coreplugin/editormanager/ieditor.h>

//#include <QSharedPointer>

namespace dScroll {
namespace Internal {

class dScrollPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "dScroll.json")

public:

    dScrollPlugin();

    ~dScrollPlugin();

    ShutdownFlag aboutToShutdown();

    void extensionsInitialized();

    bool initialize(const QStringList &arguments, QString *errorString);

private slots:

    void editorCreated(Core::IEditor *editor, const QString &fileName);

    void triggerAction();
};

} // namespace Internal
} // namespace dScroll

#endif // DSCROLL_H

