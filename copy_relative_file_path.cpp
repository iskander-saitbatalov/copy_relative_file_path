#include "copy_relative_file_path.h"
#include "copy_relative_file_pathconstants.h"

#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/icontext.h>
#include <coreplugin/icore.h>
#include <utils/filepath.h>
#include <utils/stringutils.h>

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>

namespace qstring_utils
{
QString cut_from (QString str, QString from)
{
  auto index = str.indexOf (from);
  if (index != -1)
    {
      auto str_length = str.length ();
      auto from_length = from.length ();
      return str.sliced (index + from_length, str_length - from_length - index);
    }

  return str;
}
}  // namespace qstring_utils

namespace copy_relative_file_path
{
namespace internal
{
plugin::plugin () = default;
plugin::~plugin () = default;

bool plugin::initialize (const QStringList &arguments, QString *errorString)
{
  Q_UNUSED (arguments)
  Q_UNUSED (errorString)

  Core::ActionContainer *menu = Core::ActionManager::createMenu (Constants::MENU_ID);
  menu->menu ()->setTitle (tr ("Copy Relative File Path"));

  {
    auto copy_relative_file_path_action = new QAction (tr ("Copy Relative File Path"), this);
    Core::Command *cmd = Core::ActionManager::
        registerAction (copy_relative_file_path_action, Constants::COPY_RELARIVE_PATH_ACTION_ID, Core::Context (Core::Constants::C_GLOBAL));
    cmd->setDefaultKeySequence (QKeySequence (tr ("Ctrl+Alt+S")));
    connect (copy_relative_file_path_action, &QAction::triggered, this, &plugin::copy_relative_file_path);
    menu->addAction (cmd);
  }

  {
    auto copy_relative_file_path__with_include_action = new QAction (tr ("Copy Relative File Path With Include"), this);
    Core::Command *cmd = Core::ActionManager::registerAction (
        copy_relative_file_path__with_include_action, Constants::COPY_RELARIVE_PATH_WITH_INCLUDE_ACTION_ID,
        Core::Context (Core::Constants::C_GLOBAL));
    cmd->setDefaultKeySequence (QKeySequence (tr ("Ctrl+Alt+R")));
    connect (copy_relative_file_path__with_include_action, &QAction::triggered, this, &plugin::copy_relative_file_path_with_include);
    menu->addAction (cmd);
  }

  Core::ActionManager::actionContainer (Core::Constants::M_TOOLS)->addMenu (menu);

  return true;
}

void plugin::extensionsInitialized () {}

ExtensionSystem::IPlugin::ShutdownFlag plugin::aboutToShutdown () { return SynchronousShutdown; }

void plugin::copy_relative_file_path ()
{
  auto filepath = get_current_relative_file_path ();
  if (filepath.isEmpty ())
    return;

  Utils::setClipboardAndSelection (filepath);
}

void plugin::copy_relative_file_path_with_include ()
{
  auto filepath = get_current_relative_file_path ();
  if (filepath.isEmpty ())
    return;

  Utils::setClipboardAndSelection (QString ("#include \"%1\"").arg (filepath));
}

QString plugin::get_current_relative_file_path ()
{
  Core::IEditor *editor = Core::EditorManager::currentEditor ();
  if (!editor)
    return {};

  Core::DocumentModel::Entry *entry = Core::DocumentModel::entryForDocument (editor->document ());
  if (!entry)
    return {};
#if (IDE_VERSION_MAJOR >= 10)
  auto full_file_path = entry->filePath ().toUserOutput ();
#else
  auto full_file_path = entry->fileName ().toUserOutput ();
#endif
  auto from_src = qstring_utils::cut_from (full_file_path, "/src/");
  auto from_include = qstring_utils::cut_from (from_src, "/include/");
  return from_include;
}
}  // namespace internal
}  // namespace copy_relative_file_path
