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

#include <QtGui/QGraphicsScene>

#include "NingeGal.h"
#include "Gal.h"
#include "ui_Gal.h"

Gal::Gal(NingeGal *gal,QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Gal),
  m_pGal(gal)
{
  ui->setupUi(this);

  m_pMainScene = new QGraphicsScene(0, 0, 640, 480);
  ui->graphicsView->setScene(m_pMainScene);
}

Gal::~Gal()
{
  delete ui;
}

void Gal::init()
{
  QList<QVariant> _args;

  _args.clear();
  _args << QVariant::fromValue<QObject *>(this) << "center";
  m_pGal->postCommand("ningeMain", "addWidget", _args);
}

QString Gal::pluginName()
{
  return "ningeGal";
}

QString Gal::pluginVersion()
{
  return "1.0";
}

QString Gal::pluginDetail()
{
  return "A Gal Game Graphic Engine. Show text and images with anime, music and so on...";
}

QWidget *Gal::pluginSettingWidget()
{
  return NULL;
}

QWidget *Gal::pluginMainWidget()
{
  return this;
}

QObject *Gal::pluginInnerObject(const QString &/*name*/)
{
  return NULL;
}

QVariant Gal::exec(const QString &command, const QList<QVariant> &arguments)
{
  if(command == "setText")
  {
  }
  else if (command == "hideText")
  {
  }
  else if (command == "addImage")
  {
  }
  else if (command == "removeImage")
  {
  }
  else if (command == "setBackground")
  {
  }
  else if (command == "setMusic")
  {
  }
  else if (command == "setSound")
  {
  }
  else if (command == "setEffect")
  {
  }
  else
  {
    return "command not found in ningeGal";
  }

  return "done";
}

void Gal::aboutToQuit()
{
}
