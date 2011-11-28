/**************************************************************************
 *
 * Copyright (C) 2011 Ninsun
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *************************************************************************/

#include <QtGui/QApplication>
#include <QtCore/QTranslator>
#include <QtCore/QLocale>
#include <QtGui/QWidget>

#include "core/Core.h"
#include "core/CorePluginInterface.h"
#include "core/PluginManager.h"

int main(int argc, char *argv[])
{
  // 设置程序样式
  QApplication::setStyle("cleanlooks");

  QApplication a(argc, argv);
  a.setApplicationName("ninge");

  // 翻译Qt库
  QTranslator qtTranslator;
  qtTranslator.load("qt_" + QLocale::system().name(), ":/res/translations");
  a.installTranslator(&qtTranslator);

  // 翻译程序
  QTranslator myappTranslator;
  myappTranslator.load("ninge_" + QLocale::system().name(), ":/res/translations");
  a.installTranslator(&myappTranslator);

  // 获取插件管理器对象, 载入插件
  ninge::PluginManager *_pluginManager = Core::getPluginManager();
  _pluginManager->loadPlugins();

  // 获取主窗口插件, 显示主窗口
  ninge::CorePluginInterface *_ningeMain = _pluginManager->getPlugin("ningeMain");
  _ningeMain->pluginMainWidget()->show();

  return a.exec();
}
