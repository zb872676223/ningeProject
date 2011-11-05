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

#include "Player.h"
#include "ui_Player.h"

#include <phonon/MediaObject>
#include <phonon/MediaSource>

#include <QtGui/QMessageBox>

Player::Player(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Player)
{
    ui->setupUi(this);

    m_pMediaObject = ui->videoPlayer->mediaObject();
    m_pAudioOutput = ui->videoPlayer->audioOutput();

    ui->seekSlider->setMediaObject(m_pMediaObject);
    ui->volumeSlider->setAudioOutput(m_pAudioOutput);

    connect(m_pMediaObject, SIGNAL(hasVideoChanged(bool)),
            this, SLOT(hasVideoChanged(bool)));
    connect(m_pMediaObject, SIGNAL(stateChanged(Phonon::State, Phonon::State)),
            this, SLOT(playerStateChanged(Phonon::State,Phonon::State)));
    connect(m_pMediaObject, SIGNAL(finished()),
            this, SLOT(finished()));
    connect(m_pMediaObject, SIGNAL(tick(qint64)),
            this, SLOT(tick(qint64)));
    connect(m_pMediaObject, SIGNAL(totalTimeChanged(qint64)),
            this, SLOT(totalTimeChanged(qint64)));
}

Player::~Player()
{
  delete ui;
}

void Player::init()
{
}

QString Player::pluginName()
{
  return "ningePlayer";
}

QString Player::pluginVersion()
{
  return "1.0";
}

QString Player::pluginDetail()
{
  return "A simple player to play audio and video.";
}

QWidget * Player::pluginSettingWidget()
{
  return NULL;
}

QWidget * Player::pluginMainWidget()
{
  return this;
}

QObject * Player::pluginInnerObject(const QString &/*name*/)
{
  return NULL;
}

QVariant Player::exec(const QString &command, const QList<QVariant> &arguments)
{
  if (command == "play") {
    if (arguments.size()>0) {
      m_pMediaObject->clearQueue();
      play(arguments.first().toString());
      QListIterator<QVariant> _itemIt(arguments);
      _itemIt.next();
      while(_itemIt.hasNext()) {
        m_pMediaObject->enqueue(Phonon::MediaSource(_itemIt.next().toString()));
      }
    }
  } else if (command == "pause") {
    pause();
  } else if (command == "stop") {
    stop();
  } else {
    return "command not found in ningePlayer";
  }

  return "done";
}

void Player::aboutToQuit()
{
}

void Player::play()
{
  if(m_pMediaObject->state() == Phonon::PlayingState)
  {
    pause();
  }
  else
  {
    ui->videoPlayer->play();
  }
}

void Player::play(const QString &url)
{
  ui->videoPlayer->play(Phonon::MediaSource(url));
}

void Player::pause()
{
  ui->videoPlayer->pause();
}

void Player::stop()
{
  ui->videoPlayer->stop();
}

void Player::hasVideoChanged(bool changed)
{
  if(changed) {
    ui->videoWidget->setCurrentIndex(1);
  } else {
    ui->videoWidget->setCurrentIndex(0);
  }
}

void Player::playerStateChanged(Phonon::State newState, Phonon::State oldState)
{
  ui->play->setIcon(QIcon(":/ningePlayer/resource/image/media-playback-start.svg"));

  // 单状态判断
  if (newState == Phonon::LoadingState)
  {
    ui->state->setText(tr("Loading..."));
  }
  else if (newState == Phonon::StoppedState)
  {
    ui->state->setText(tr("Stop"));
  }
  else if (newState == Phonon::PlayingState)
  {
    ui->state->setText(tr("Playing..."));
    ui->play->setIcon(QIcon(":/ningePlayer/resource/image/media-playback-pause.svg"));
  }
  else if (newState == Phonon::BufferingState)
  {
    ui->state->setText(tr("Buffing..."));
  }
  else if (newState == Phonon::PausedState)
  {
    ui->state->setText(tr("Pause"));
  }

  // 多状态/错误判断
  if (newState == Phonon::StoppedState && oldState == Phonon::LoadingState)
  {
    ui->state->setText(tr("Ready..."));
  }
  else if (newState == Phonon::ErrorState && oldState == Phonon::LoadingState)
  {
    ui->state->setText(tr("Load error"));
    QMessageBox::critical(NULL, tr("Load error"), ui->videoPlayer->mediaObject()->errorString());
  }
  else if (newState == Phonon::ErrorState)
  {
    ui->state->setText(tr("Error"));
    QMessageBox::critical(NULL, tr("Error"), ui->videoPlayer->mediaObject()->errorString());
  }
}

void Player::finished()
{
  ui->videoPlayer->stop();
}

void Player::tick(qint64 time)
{
  QString _now = QString("%1:%2 / %3:%4")
      .arg(time/60000, 0, 'f', 0, '0')
      .arg(time%60000/1000, 2, 'f', 0, '0')
      .arg(m_iTotalTime/60000, 0, 'f', 0, '0')
      .arg(m_iTotalTime%60000/1000, 2, 'f', 0, '0');
  ui->time->setText(_now);
}

void Player::totalTimeChanged(qint64 newTotalTime)
{
  m_iTotalTime = newTotalTime;
  tick(0);
}
