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
#include <QtCore/QSettings>

#include "NingeGal.h"
#include "Gal.h"
#include "ui_Gal.h"

#include "GalPixmapItem.h"
#include "GalTextItem.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtGui/QGraphicsDropShadowEffect>
#include <QtOpenGL/QGLWidget>

Gal::Gal(NingeGal *gal,QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Gal),
  m_pGal(gal)
{
  ui->setupUi(this);

  ui->graphicsView->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));

  // 读取并初始化配置
  QVariant _width(640);
  QVariant _height(480);
  QSettings _settings("ninge.cfg", QSettings::IniFormat);
  _width = _settings.value("width", _width);
  _settings.setValue("width", _width);
  _height = _settings.value("height", _height);
  _settings.setValue("height", _height);

  ui->graphicsView->setSceneRect(0, 0, _width.toInt(), _height.toInt());

  m_pMainScene = new QGraphicsScene();
  ui->graphicsView->setScene(m_pMainScene);

  GalPixmapItem *_item = new GalPixmapItem();
  _item->setGif(QString::fromUtf8("E:/IMG/图片/free_llama_running__3_by_MenInASuitcase.gif"));
  m_pMainScene->addItem(_item);

  QGraphicsRectItem *_textBackground = new QGraphicsRectItem();

  _textBackground->setRect(0, 0, _width.toInt()-40, _height.toInt()-40);
  _textBackground->setPos(20, 20);
  _textBackground->setBrush(QBrush(QColor(0,0,0,127)));
  _textBackground->setPen(QPen(Qt::NoPen));
  m_pMainScene->addItem(_textBackground);

  QFile _file(QString::fromUtf8("E:/资料/我的世界不可能这么平凡-删减版.txt"));
  _file.open(QFile::ReadOnly);
  GalTextItem *_text = new GalTextItem(_textBackground);
  QTextStream _stream(_file.readAll());
  _stream.setCodec("GB2312");
  _text->setText(_stream.readAll());

  _text->setTextWidth(_textBackground->rect().width());
  _text->setMaxHeight(_textBackground->rect().height());
  _text->setInterval(20);
  _text->start();
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
  else
  {
    return "command not found in ningeGal";
  }

  return "done";
}

void Gal::aboutToQuit()
{
}
