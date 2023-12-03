#pragma once

#include "copy_relative_file_path_global.h"

#include <extensionsystem/iplugin.h>

namespace copy_relative_file_path
{
namespace internal
{
class plugin : public ExtensionSystem::IPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA (IID "org.qt-project.Qt.QtCreatorPlugin" FILE "CopyRelativeFilePath.json")

public:
  plugin ();
  ~plugin () override;

protected:
  bool initialize (const QStringList &arguments, QString *errorString) override;
  void extensionsInitialized () override;
  ShutdownFlag aboutToShutdown () override;

private:
  void copy_relative_file_path ();
  void copy_relative_file_path_with_include ();
  static QString get_current_relative_file_path ();
};
}  // namespace Internal
}  // namespace Copy_Relative_File_Path
