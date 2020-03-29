#ifndef NANOAVRCLIENT_H
#define NANOAVRCLIENT_H

#include <QObject>

#include <QAbstractSocket>

#include "nanoavrbasicsettings.h"

class QTcpSocket;

namespace eu
{
namespace tgcm
{
namespace minidsp_remote
{

class NanoAVRClient : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
	Q_PROPERTY(int connectionStatus READ connectionStatus WRITE setConnectionStatus NOTIFY connectionStatusChanged)
	Q_PROPERTY(NanoAVRBasicSettings* device READ device NOTIFY deviceChanged)
	QString address_;

	int connectionStatus_;

	NanoAVRBasicSettings* device_;

	QTcpSocket* socket_ = nullptr;

public:
	NanoAVRClient(QObject* parent = nullptr);
	QString address() const;
	int connectionStatus() const;

	NanoAVRBasicSettings* device() const;

public slots:
	void setAddress(QString address);
	void setConnectionStatus(int connectionStatus);

	void connectToDevice();
	/**
	 * @brief setVolume sets the volume, in the range [-255,0]
	 * @param volume
	 */
	void setVolume(int volume);

	/**
	 * @brief setHdmiInput sets the hdmi input, in the range [1-2]
	 * @param input
	 */
	void setHdmiInput(int input);

	/**
	 * @brief setEqualizerPreset sets the equalizer preset, in the range [1-4]
	 * @param preset
	 */
	void setEqualizerPreset(int preset);

	/**
	 * @brief setMute set the mute status (on for mute, off for unmute)
	 * @param mute
	 */
	void setMute(bool mute);

signals:
	void addressChanged(QString address);
	void connectionStatusChanged(int connectionStatus);
	void deviceChanged(NanoAVRBasicSettings* device);


private slots:
	void socketDisconnected_();
	void socketConnected_();
	void consumeRead_();
	void cannotConnect_(QAbstractSocket::SocketError);

private:
	void readInformation_();

	int pendingHdmiInput_;
	int pendingPreset_;
	bool pendingMuted_;
};

}
}
}

#endif // NANOAVRCLIENT_H
