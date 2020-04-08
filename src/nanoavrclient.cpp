#include "nanoavrclient.h"

#include <QTcpSocket>

#include <query.h>
#include <reply.h>

namespace eu
{
namespace tgcm
{
namespace minidsp_remote
{

NanoAVRClient::NanoAVRClient(QObject* parent) : QObject{parent},
    device_{new NanoAVRBasicSettings{this}},
    timer_{new QTimer{this}}
{
	connect(timer_, &QTimer::timeout, this, &NanoAVRClient::timerElapsed_);
	timer_->setSingleShot(true);
}

QString NanoAVRClient::address() const
{
	return address_;
}

int NanoAVRClient::connectionStatus() const
{
	return connectionStatus_;
}

NanoAVRBasicSettings *NanoAVRClient::device() const
{
	return device_;
}

void NanoAVRClient::setAddress(QString address)
{
	if (address_ == address)
		return;

	address_ = address;
	emit addressChanged(address_);
}

void NanoAVRClient::setConnectionStatus(int connectionStatus)
{
	if (connectionStatus_ == connectionStatus)
		return;

	connectionStatus_ = connectionStatus;
	emit connectionStatusChanged(connectionStatus_);
}

void NanoAVRClient::connectToDevice()
{
	if(socket_ != nullptr)
	{
		disconnect(socket_, &QTcpSocket::disconnected, this, &NanoAVRClient::socketDisconnected_);
		socket_->deleteLater();
	}
	setConnectionStatus(0);
	socket_ = new QTcpSocket(this);
	connect(socket_, &QTcpSocket::connected, this, &NanoAVRClient::socketConnected_);
	connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(cannotConnect_(QAbstractSocket::SocketError)));
	connect(socket_, &QTcpSocket::readyRead, this, &NanoAVRClient::consumeRead_);
	socket_->connectToHost(address_, 5333);
}

void NanoAVRClient::setVolume(int volume)
{
	if(connectionStatus() == 2 && volume > -256 && volume <= 0)
	{
		auto ret = minidsp::Query::changeVolumeQuery(volume);
		executeNextQuery_(ret);
	}
}

void NanoAVRClient::setHdmiInput(int input)
{
	if(connectionStatus() == 2 && input > 0 && input < 3)
	{
		auto ret = minidsp::Query::switchHdmiInputQuery(static_cast<minidsp::HDMIInput>(input - 1));
		pendingHdmiInput_ = input;
		executeNextQuery_(ret);
	}
}

void NanoAVRClient::setEqualizerPreset(int preset)
{
	if(connectionStatus() == 2 && preset > 0 && preset < 5 &&
	        preset != device()->currentPreset())
	{
		auto ret = minidsp::Query::switchEQPresetQuery(static_cast<minidsp::EQPreset>(preset - 1));
		pendingPreset_ = preset;
		executeNextQuery_(ret);
	}
}

void NanoAVRClient::setMute(bool mute)
{
	if(connectionStatus() == 2)
	{
		auto ret = minidsp::Query::muteQuery(mute);
		pendingMuted_ = mute;
		executeNextQuery_(ret);
	}
}

void NanoAVRClient::socketDisconnected_()
{
	setConnectionStatus(0);

}

void NanoAVRClient::socketConnected_()
{
	setConnectionStatus(1);
	readInformation_();
}

void NanoAVRClient::consumeRead_()
{
	auto ret = socket_->readAll();
	if(connectionStatus_ == 1)
		setConnectionStatus(2);
	minidsp::Reply rep{reinterpret_cast<std::uint8_t*>(ret.data()), static_cast<std::uint32_t>(ret.size())};
	if(rep.type() == minidsp::Reply::Type::DeviceInformationReply)
	{
		device_->setCurrentHdmiInput(static_cast<int>(rep.hdmiInput()) + 1);
		device_->setCurrentPreset(static_cast<int>(rep.preset()) + 1);
		device_->setMuted(rep.muted());
		device_->setVolume(rep.volume());
	}
	else if(rep.type() == minidsp::Reply::Type::VolumeReply)
	{
		device_->setVolume(rep.volume());
	}
	else if(rep.type() == minidsp::Reply::Type::SwitchHdmiInputReply)
	{
		device_->setCurrentHdmiInput(pendingHdmiInput_);
	}
	else if(rep.type() == minidsp::Reply::Type::SwitchPresetReply)
	{
		device_->setCurrentPreset(pendingPreset_);
	}
	else if(rep.type() == minidsp::Reply::Type::MuteReply)
	{
		device_->setMuted(pendingMuted_);
	}
}

void NanoAVRClient::cannotConnect_(QAbstractSocket::SocketError)
{
	setConnectionStatus(-1);
}

void NanoAVRClient::readInformation_()
{
	auto q = minidsp::Query::deviceInformationQuery();
	//(void)q;
	executeNextQuery_(q);
}

void NanoAVRClient::executeNextQuery_(minidsp::Query::Message const& message)
{
	if(timer_->isActive())
	{
		addPendingQuery_(message);
	}
	else if(pendingQueries_.size() > 0) // should not happen, but in case...
	{
		addPendingQuery_(message);
		restartTimer_();
		auto query = pendingQueries_.front();
		pendingQueries_.erase(pendingQueries_.begin());
		executeQuery_(query);
	}
	else
	{
		restartTimer_();
		executeQuery_(message);
	}
}

void NanoAVRClient::restartTimer_()
{
	timer_->start(/*std::chrono::milliseconds{ */100 /*}*/);
}

void NanoAVRClient::executeQuery_(minidsp::Query::Message const& message)
{
	socket_->write(reinterpret_cast<char const*>(message.data()), message.size());
}

void NanoAVRClient::timerElapsed_()
{
	if(pendingQueries_.size() > 0) // execute the next pending query
	{
		restartTimer_();
		auto query = pendingQueries_.front();
		pendingQueries_.erase(pendingQueries_.begin());
		executeQuery_(query);
	}
	// else queue empty, nothing to do
}

void NanoAVRClient::addPendingQuery_(minidsp::Query::Message const& message)
{
	for(auto it = pendingQueries_.begin(); it != pendingQueries_.end(); /*iterator increment inside loop body*/)
	{
		if(minidsp::Query::queryCommand(message) == minidsp::Query::queryCommand(*it))
		{
			it = pendingQueries_.erase(it);
		}
		else
			++it;
	}
	pendingQueries_.push_back(message);
}

}
}
}
