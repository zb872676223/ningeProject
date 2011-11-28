/*************************************************************************
 *
 * Copyright (C) 2011 Ninsun, All rights reserved.
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

#ifndef MAIN_H
#define MAIN_H

#include <QtGui/QMainWindow>
#include <QtCore/QVariant>

namespace Ui {
class Main;
}

class Main : public QMainWindow
{
    Q_OBJECT

public:
    explicit Main(QWidget *parent = 0);
    ~Main();

public:
  // 初始化方法, 会被插件管理器自动调用
  void init();
  // 获取插件的名称, 插件管理器根据插件名称获取插件对象指针
  QString pluginName();
  // 获取插件的版本号
  QString pluginVersion();
  // 获取插件详情
  QString pluginDetail();
  // 获取插件的设置界面, 如果插件没有可视化配置界面，则应该返回一个空指针
  QWidget* pluginSettingWidget();
  // 获取插件的主界面，如果插件没有可视化主界面，则应该返回一个空指针
  QWidget* pluginMainWidget();
  // 根据name获取一个插件的内含QObject对象, 如果没有当前名字的对象，则应返回一个空指针
  QObject* pluginInnerObject(const QString &);
  // 执行一条命令, 接受一个参数列表, 返回一个返回值
  QVariant exec(const QString &command = QString(), const QList<QVariant> &arguments = QList<QVariant>());
  // 插件在退出时应该做的额外操作, 会被插件管理器自动调用
  void aboutToQuit();

private slots:
  void on_actionAbout_ninge_triggered();

  void on_actionAbout_Qt_triggered();

private:
    Ui::Main *ui;
};

#endif // MAIN_H
