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

#include "Main.h"
#include "ui_Main.h"
#include "NingeMain.h"
#include "core/GlobalSetting.h"

#include <QtCore/QDebug>
#include <QtGui/QMouseEvent>

Main::Main(NingeMain *main, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::Main),
  m_pMain(main)
{
  ui->setupUi(this);
    ui->menubar->setVisible(false);
}

Main::~Main()
{
    delete ui;
}

void Main::init()
{
    // 打开配置文件
    GlobalSetting _settings;
    // 读取配置
    QVariant _width = _settings.value("width", 800);
    QVariant _height = _settings.value("height", 600);
    setFixedSize(_width.toInt(), _height.toInt());
}

QString Main::pluginName()
{
    return "ningeMain";
}

QString Main::pluginVersion()
{
    return "1.0";
}

QString Main::pluginDetail()
{
    return "Main Window";
}

QWidget *Main::pluginSettingWidget()
{
    return NULL;
}

QWidget *Main::pluginMainWidget()
{
    return this;
}

QObject *Main::pluginInnerObject(const QString & /*name*/)
{
    return NULL;
}

QVariant Main::exec(const QString &command, const QList<QVariant> &arguments)
{
    if (command == "addMenu")
    {
      if (arguments.size() == 1)
      {
          QMenu* _menu = qobject_cast<QMenu*>(arguments.first().value<QObject *>());
          if(_menu)
          {
              ui->menubar->insertMenu(ui->menu_About->menuAction(), _menu);
          }
      }
    }
    else if (command == "addWidget")
    {
      if (arguments.size() == 2)
      {
          QWidget* _widget = qobject_cast<QWidget*>(arguments.value(0).value<QObject *>());
          QString _place = arguments.value(1).toString();
          if(_widget && !_place.isEmpty())
          {
              if(_place == "center")
              {
                ui->centralStack->addWidget(_widget);
                ui->centralStack->setCurrentWidget(_widget);
              }
          }
      }
    }
    else if (command == "resize")
    {
      qApp->processEvents();
      QSize _size = arguments.value(0).toSize();
      if(_size.isValid() && !_size.isEmpty())
      {
//        setMinimumSize(0, 0);
//        setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
//        resize(_size);
//        setMinimumSize(_size);
//        setMaximumSize(_size);
        setFixedSize(_size);
      }
    }
    else if(command == "show")
    {
        show();
    }
    else
    {
      return "command not found in ningeMain";
    }

    return "done";
}

void Main::aboutToQuit()
{
}

void Main::on_actionAbout_ninge_triggered()
{

}

void Main::on_actionAbout_Qt_triggered()
{
  qApp->aboutQt();
}

void Main::closeEvent(QCloseEvent * /*event*/)
{
//  m_pMain->postCommand("PluginManager", "exit");
}
