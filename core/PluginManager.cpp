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

#include "PluginManager.h"
#include "CorePluginInterface.h"

#include <QStringListIterator>
#include <QDir>
#include <QLibrary>
#include <QPluginLoader>
#include <QApplication>
#include <QDebug>

using namespace ninge;

PluginManager * PluginManager::m_pInstance = NULL;

PluginManager * PluginManager::instance()
{
  if (m_pInstance)
  {
    return m_pInstance;
  }
  else
  {
    m_pInstance = new PluginManager();
    return m_pInstance;
  }
}

PluginManager::PluginManager(QObject *parent) :
    QObject(parent)
{
}

PluginManager::~PluginManager()
{
  unloadPlugins();
}

void PluginManager::loadPlugins()
{
  // 首先清除可能存在的插件
  unloadPlugins();
  // 读获取插件文件夹下的全部路径
  QStringList _pluginList;
  QStringList _searchPaths;
  // 添加默认查找目录
  QDir _path(qApp->applicationDirPath());
  _path.cd("plugins");
  _searchPaths << _path.absolutePath();
  // 开始搜索并记录全部插件的路径
  while(!_searchPaths.isEmpty())
  {
    QDir _dir(_searchPaths.takeFirst());
    QStringListIterator _fileIt(_dir.entryList(QDir::Files));
    while(_fileIt.hasNext())
    {
      QString _file = _fileIt.next();
      if(QLibrary::isLibrary(_file))
      {
        _pluginList << _dir.absoluteFilePath(_file);
      }
    }

    QStringListIterator _subdirIt(_dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
    while(_subdirIt.hasNext())
    {
      _searchPaths << _dir.absoluteFilePath(_subdirIt.next());
    }
  }
  // 逐个载入插件
  QStringListIterator _pluginIt(_pluginList);
  while(_pluginIt.hasNext())
  {
    QPluginLoader _loader(_pluginIt.next());
    if(!_loader.load())
    {
      // 插件载入失败
      qDebug() << QObject::tr("[%1]载入失败 : %2")
                  .arg(QFileInfo(_loader.fileName()).fileName())
                  .arg(_loader.errorString());
      continue;
    }

    if(CorePluginInterface *_plugin = qobject_cast<CorePluginInterface *>(_loader.instance()))
    {
      // 判断插件名字是否合法
      if (_plugin->pluginName() == "PluginManager")
      {
        // 存在非法名称插件, 报告问题
        qDebug() << QObject::tr("[%1]插件名称非法: %2")
                    .arg(QFileInfo(_loader.fileName()).fileName())
                    .arg(_plugin->pluginName());
        // 继续载入其他插件
        _loader.unload();
        continue;
      }
      // 判断是否拥有同名插件
      if(m_plugins.value(_plugin->pluginName()))
      {
        // 存在同名插件, 报告问题
        qDebug() << QObject::tr("[%1]载入时发现异常:已存在同名的插件,本插件将不会被载入")
                    .arg(QFileInfo(_loader.fileName()).fileName());
        // 为了安全起见, 不载入这个同名插件, 继续载入其余插件
        _loader.unload();
        continue;
      }
      // 将插件放入插件表
      m_plugins.insert(_plugin->pluginName(), _plugin);
      // 连接命令发送槽(插件使用这个信号向插件管理器发送命令)
      connect(_plugin, SIGNAL(sendCommand(QString, QString, QList<QVariant>)),
              this, SLOT(sendCommand(QString, QString, QList<QVariant>)));
      // 初始化插件
      _plugin->init();
      // 插件载入成功
      qDebug() << QObject::tr("[%1]载入成功!")
                  .arg(QFileInfo(_loader.fileName()).fileName());
    }
    else
    {
      // 插件实现的接口错误
      qDebug() << QObject::tr("[%1]解析失败 : 接口错误")
                  .arg(QFileInfo(_loader.fileName()).fileName());
      _loader.unload();
    }
  }
}

CorePluginInterface * PluginManager::getPlugin(const QString &pluginName)
{
  return m_plugins.value(pluginName);
}

void PluginManager::unloadPlugins()
{
  // 销毁前调用全部插件的准备退出函数
  QListIterator<CorePluginInterface *> _pluginIt(m_plugins.values());
  while(_pluginIt.hasNext())
  {
    _pluginIt.next()->aboutToQuit();
  }
  // 销毁插件对象
  qDeleteAll(m_plugins);
  // 清除插件列表
  m_plugins.clear();
}

void PluginManager::sendCommand(const QString &plugin, const QString &command, const QList<QVariant> &arguments)
{
  CorePluginInterface* _plugin = m_plugins.value(plugin);
  if (_plugin)
  {
    qDebug() << _plugin->exec(command, arguments);
  }
  else
  {
    qDebug() << "Command not found";
  }
}
