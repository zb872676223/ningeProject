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

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QVariant>
#include <QtCore/QStringList>

#include "core_global.h"

namespace ninge
{
  class CorePluginInterface;

  class CORESHARED_EXPORT PluginManager : public QObject  {
    Q_OBJECT

    // 屏蔽公共构造函数
    explicit PluginManager(QObject *parent = 0);

  public:
    ~PluginManager();
    // 声明静态单例
    static PluginManager* instance();

  public:
    // 载入指定目录下的插件(默认加载ningePlugins目录)
    void loadPlugins(const QString &dir="ningePlugins");
    // 卸载全部插件
    void unloadPlugins();
    // 根据插件名称得到相应的插件
    CorePluginInterface* getPlugin(const QString &pluginName);

  public slots:
    // 向插件发送一条命令
    QVariant sendCommand(const QString &plugin, const QString &command, const QList<QVariant> &arguments);

  private:
    // 插件Hash表
    QHash<QString, CorePluginInterface*> m_plugins;
    // 静态管理器对象指针
    static PluginManager *m_pInstance;
    // 非法插件名称列表
    QStringList m_illegalPluginNameList;
  };

}

#endif // PLUGINMANAGER_H
