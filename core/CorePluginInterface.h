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

#ifndef COREPLUGININTERFACE_H
#define COREPLUGININTERFACE_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QVariant>

class QWidget;

namespace ninge {

  class CorePluginInterface : public QObject
  {
    Q_OBJECT

  public:
    virtual ~CorePluginInterface() {}

    // 需要实现的接口
  public:
    // 初始化方法, 会被插件管理器自动调用
    virtual void init() = 0;
    // 获取插件的名称, 插件管理器根据插件名称获取插件对象指针
    virtual QString pluginName() = 0;
    // 获取插件的版本号
    virtual QString pluginVersion() = 0;
    // 获取插件详情
    virtual QString pluginDetail() = 0;
    // 获取插件的设置界面, 如果插件没有可视化配置界面，则应该返回一个空指针
    virtual QWidget* pluginSettingWidget() = 0;
    // 获取插件的主界面，如果插件没有可视化主界面，则应该返回一个空指针
    virtual QWidget* pluginMainWidget() = 0;
    // 根据name获取一个插件的内含QObject对象, 如果没有当前名字的对象，则应返回一个空指针
    virtual QObject* pluginInnerObject(const QString &name) = 0;
    // 执行一条命令, 接受一个参数列表, 返回一个返回值
    virtual QVariant exec(const QString &command = QString(), const QList<QVariant> &arguments = QList<QVariant>()) = 0;
    // 插件在退出时应该做的额外操作, 会被插件管理器自动调用
    virtual void aboutToQuit() = 0;

  signals:
    // 向外部发送一条命令信号, 由插件管理器接收并做出相应的处理
    void sendCommand(const QString &plugin, const QString &command, const QList<QVariant> &arguments);
  };
}

Q_DECLARE_INTERFACE(ninge::CorePluginInterface, "ninge.CorePluginInterface/1.0")

#endif // COREPLUGININTERFACE_H
