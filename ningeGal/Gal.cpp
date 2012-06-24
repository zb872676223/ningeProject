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

#include <core/GlobalSetting.h>

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtGui/QGraphicsDropShadowEffect>
#include <QtGui/QMouseEvent>
#include <QtOpenGL/QGLWidget>

Gal::Gal(NingeGal *gal,QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Gal),
  m_pGal(gal)
{
  ui->setupUi(this);
  // 打开配置文件
  GlobalSetting _settings;
  // 读取配置文件
  QVariant _width = _settings.value("width", 800);
  QVariant _height = _settings.value("height", 600);
  QVariant _openGL = _settings.value("openGL", false);

  // 设置OpenGL支持
  if(_openGL.toBool())
  {
    ui->graphicsView->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
  }
//  // 设置视图大小
//  ui->graphicsView->setSceneRect(0, 0, _width.toInt(), _height.toInt());
  // 创建主场景
  m_pMainScene = new QGraphicsScene();
  m_pMainScene->setSceneRect(0, 0, _width.toInt(), _height.toInt());
  ui->graphicsView->setScene(m_pMainScene);
  // 创建背景
  m_pBackground = new GalPixmapItem();
  m_pBackground->setZValue(-100);
  m_pMainScene->addItem(m_pBackground);
  // 创建文字背景
  m_pTextBackground = new QGraphicsRectItem();
  m_pTextBackground->setRect(0, 0, _width.toInt()-40, _height.toInt()-40);
  m_pTextBackground->setPos(20, 20);
  m_pTextBackground->setBrush(QBrush(QColor(0,0,0,127)));
  m_pTextBackground->setPen(QPen(Qt::NoPen));
  m_pTextBackground->setZValue(100);
  m_pMainScene->addItem(m_pTextBackground);
  // 创建文字对象
  m_pText = new GalTextItem(m_pTextBackground);
  m_pText->setTextWidth(m_pTextBackground->rect().width());
  m_pText->setMaxHeight(m_pTextBackground->rect().height());
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
  if(command == "setCodec")
  {
    m_codec = arguments.value(0).toString();
  }
  else if(command == "setText")
  {
    setText(arguments.value(0).toString(), arguments.value(1).toString());
  }
  else if (command == "setInterval")
  {
    m_pText->setInterval(arguments.value(0).toInt());
  }
  else if (command == "start")
  {
    m_pText->start();
  }
  else if (command == "pause")
  {
    m_pText->pause();
  }
  else if (command == "clear")
  {
    m_pText->clear();
  }
  else if (command == "showText")
  {
    m_pTextBackground->show();
  }
  else if (command == "hideText")
  {
    m_pTextBackground->hide();
  }
  else if (command == "addImage")
  {
  }
  else if (command == "removeImage")
  {
  }
  else if (command == "setBackground")
  {
    setBackground(arguments.value(0).toString(), arguments.value(1).toString());
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

void Gal::mousePressEvent(QMouseEvent *event)
{
  if(event->button() == Qt::LeftButton)
  {
    switch(m_pText->state())
    {
    case -1:
      break;
    case 0:
      m_pText->start();
      break;
    case 1:
      m_pText->pause();
      break;
    case 2:
      m_pText->start();
      break;
    default:
      break;
    }
  }
}

void Gal::setBackground(const QString &backgroudUrl, const QString &/*effectUrl*/)
{
  m_pBackground->stop();
  if (backgroudUrl.endsWith(".gif"))
  {
    m_pBackground->setGif(backgroudUrl);
  }
  else
  {
    m_pBackground->setPixmap(backgroudUrl);
  }
  QSizeF _size = m_pMainScene->sceneRect().size() - m_pBackground->pixmap().size();

  m_pBackground->setPos(_size.width()/2, _size.height()/2);
}

void Gal::setText(const QString &textUrl, const QString &effectUrl)
{
  QTextStream _stream;

  QFile _file;
  _file.setFileName(textUrl);
  if(_file.open(QFile::ReadOnly))
  {
    _stream.setDevice(&_file);
    _stream.setCodec(m_codec.toLocal8Bit().data());
    m_pText->setText(_stream.readAll());
    _file.close();

    _file.setFileName(effectUrl);
    if(_file.open(QFile::ReadOnly))
    {
      _stream.setDevice(&_file);
      _stream.setCodec(m_codec.toLocal8Bit().data());
      m_pText->setEffect(_stream.readAll());
      _file.close();
    }
  }
}
