#include "dscrollplugin.h"
#include "dscrollconstants.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/editormanager/editormanager.h>

#include <texteditor/basetexteditor.h>

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>

#include <QtPlugin>

#include "dscrollbar.h"

using namespace dScroll::Internal;

dScrollPlugin::dScrollPlugin()
{
    // Create your members
}

dScrollPlugin::~dScrollPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

bool dScrollPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize function, a plugin can be sure that the plugins it
    // depends on have initialized their members.

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    QAction *action = new QAction(tr("dScroll action"), this);
    Core::Command *cmd = Core::ActionManager::registerAction(action, Constants::ACTION_ID,
                                                             Core::Context(Core::Constants::C_GLOBAL));
    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Alt+Meta+A")));
    connect(action, SIGNAL(triggered()), this, SLOT(triggerAction()));

    Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::MENU_ID);
    menu->menu()->setTitle(tr("dScroll"));
    menu->addAction(cmd);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

    Core::EditorManager* em = Core::EditorManager::instance();
    connect(em, SIGNAL(editorCreated(Core::IEditor*,QString)), SLOT(editorCreated(Core::IEditor*,QString)));

    return true;
}

void dScrollPlugin::editorCreated(Core::IEditor *editor, const QString &fileName)
{
    Q_UNUSED(fileName);
    editor->widget();
    TextEditor::BaseTextEditorWidget* baseEditor = qobject_cast<TextEditor::BaseTextEditorWidget*>(editor->widget());

    baseEditor->setVerticalScrollBar(new dScrollBar(baseEditor));
//    baseEditor->setVerticalScrollBar(new dScrollBar());
}

void dScrollPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

ExtensionSystem::IPlugin::ShutdownFlag dScrollPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

void dScrollPlugin::triggerAction()
{
    QMessageBox::information(Core::ICore::mainWindow(),
                             tr("dScroll Settings"),
                             tr("This will one day be settings for dScroll."));
}

Q_EXPORT_PLUGIN2(dScroll, dScrollPlugin)

