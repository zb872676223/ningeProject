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
#include <QtGui/QDesktopWidget>
#include <QtGui/QFileDialog>
#include <QtCore/QTime>

#include "core/CorePluginInterface.h"
#include "NingePlayer.h"

Player::Player(NingePlayer *player, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Player),
    m_pPlayer(player)
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

    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(onPlayerCustomContextMenuRequested(QPoint)));

    m_iconPlay = style()->standardIcon(QStyle::SP_MediaPlay);
    m_iconPause = style()->standardIcon(QStyle::SP_MediaPause);
    m_iconStop = style()->standardIcon(QStyle::SP_MediaStop);
    m_iconPrev = style()->standardIcon(QStyle::SP_MediaSkipBackward);
    m_iconNext = style()->standardIcon(QStyle::SP_MediaSkipForward);

    ui->play->setIcon(m_iconPlay);
    ui->stop->setIcon(m_iconStop);

    m_menu.setTitle(tr("&Player"));
    m_menu.addAction(tr("&Open File..."), this, SLOT(openFile()));
    m_menu.addSeparator();
    m_menu.addAction(tr("&Resize"), this, SLOT(onResize()));
    m_menu.addSeparator();
    m_menu.addAction(ui->action_Show_Control);

    ui->controlWidget->setVisible(false);
}

Player::~Player()
{
    delete ui;
}

void Player::init()
{
//      QList<QVariant> _args;

//      _args.clear();
//      _args << QVariant::fromValue<QObject *>(&m_menu);
//      m_pPlayer->postCommand("ningeMain", "addMenu", _args);

//      _args.clear();
//      _args << QVariant::fromValue<QObject *>(this) << "center";
//      m_pPlayer->postCommand("ningeMain", "addWidget", _args);
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
    if(command == "load")
    {
        if (arguments.size()>0)
        {
            QListIterator<QVariant> _itemIt(arguments);
            while(_itemIt.hasNext())
            {
                m_pMediaObject->enqueue(Phonon::MediaSource(_itemIt.next().toString()));
            }
        }
    }
    else if (command == "play")
    {
        if (arguments.size()>0)
        {
            m_pMediaObject->stop();
            m_pMediaObject->clearQueue();
            QListIterator<QVariant> _itemIt(arguments);
            while(_itemIt.hasNext())
            {
                m_pMediaObject->enqueue(Phonon::MediaSource(_itemIt.next().toString()));
            }
            play();
        }
    }
    else if (command == "pause")
    {
        pause();
    }
    else if (command == "stop")
    {
        stop();
    }
    else if (command == "showControl")
    {
        ui->controlWidget->setVisible(arguments.value(0).toBool());
    }
    else if (command == "showVideo")
    {
        ui->videoWidget->setVisible(arguments.value(0).toBool());
    }
    else if (command == "show")
    {
        show();
    }
    else
    {
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

void Player::openFile()
{
    QString _file = QFileDialog::getOpenFileName(this, tr("Open File"));
    if(!_file.isEmpty())
    {
        play(_file);
        //      m_pMediaObject->stop();
        //    m_pMediaObject->clearQueue();
        //    m_pMediaObject->enqueue(Phonon::MediaSource(_file));
        //    play();
    }
}

void Player::hasVideoChanged(bool changed)
{
    if(changed)
    {
        ui->videoWidget->setCurrentIndex(1);
    }
    else
    {
        ui->videoWidget->setCurrentIndex(0);
    }
}

void Player::playerStateChanged(Phonon::State newState, Phonon::State oldState)
{
    ui->play->setIcon(m_iconPlay);

    //  if(oldState == Phonon::LoadingState && newState != Phonon::ErrorState)
    //  {
    //    if (m_pMediaObject->hasVideo())
    //    {
    //      qApp->processEvents();
    //      resize(sizeHint());
    //      m_pPlayer->postCommand("ningeMain", "resize");
    //    }
    //  }

    // 单状态判断
    if (newState == Phonon::LoadingState)
    {
        ui->state->setText(tr("Loading..."));
    }
    else if (newState == Phonon::StoppedState)
    {
        ui->state->setText(tr("Stop"));
        setWindowFlags(windowFlags() & (~Qt::WindowStaysOnTopHint));
        show();
    }
    else if (newState == Phonon::PlayingState)
    {
        ui->state->setText(tr("Playing..."));
        ui->play->setIcon(m_iconPause);
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
        show();
    }
    else if (newState == Phonon::BufferingState)
    {
        ui->state->setText(tr("Buffing..."));
    }
    else if (newState == Phonon::PausedState)
    {
        ui->state->setText(tr("Pause"));
        setWindowFlags(windowFlags() & (~Qt::WindowStaysOnTopHint));
        show();
    }

    // 多状态/错误判断
    if (newState == Phonon::StoppedState && oldState == Phonon::LoadingState)
    {
        ui->state->setText(tr("Loading complete"));
    }
    else if (newState == Phonon::ErrorState && oldState == Phonon::LoadingState)
    {
        ui->state->setText(tr("Loading error"));
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
    QString _timeString;
    if (time || m_iTotalTime)
    {
        int _sec = time/1000;
        int _min = _sec/60;
        int _hour = _min/60;
        int _msec = time;

        QTime _playTime(_hour%60, _min%60, _sec%60, _msec%1000);
        _sec = m_iTotalTime / 1000;
        _min = _sec / 60;
        _hour = _min / 60;
        _msec = m_iTotalTime;

        QTime _totalTime(_hour%60, _min%60, _sec%60, _msec%1000);
        QString _timeFormat = "m:ss";
        if (_hour > 0)
            _timeFormat = "h:mm:ss";
        _timeString = _playTime.toString(_timeFormat);
        if (m_iTotalTime)
            _timeString += " / " + _totalTime.toString(_timeFormat);
    }
    ui->time->setText(_timeString);
}

void Player::totalTimeChanged(qint64 newTotalTime)
{
    m_iTotalTime = newTotalTime;
    tick(0);
}

void Player::onPlayerCustomContextMenuRequested(const QPoint &/*pos*/)
{
    m_menu.popup(QCursor::pos());
}

void Player::on_action_Show_Control_triggered(bool checked)
{
    ui->controlWidget->setHidden(!checked);
}

void Player::onResize()
{
    resize(sizeHint());
}
