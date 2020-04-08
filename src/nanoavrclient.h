#ifndef NANOAVRCLIENT_H
#define NANOAVRCLIENT_H

#include "nanoavrbasicsettings.h"

#include <QAbstractSocket>
#include <QObject>
#include <QTimer>
#include <QVector>

#include <query.h>

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

	/**
	 * Rate limit timer, used to limit the number of queries per second we send to the device
	 */
	QTimer* timer_ = nullptr;

	/**
	 * Stores the pending queries, because we don't want to stress the device, sending it too many queries.
	 * We limit to 1 query every 100ms
	 */
	QVector<minidsp::Query::Message> pendingQueries_;

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

	void timerElapsed_();

private:
	void readInformation_();

	int pendingHdmiInput_;
	int pendingPreset_;
	bool pendingMuted_;

	/**
	 * Execute the given query, or add it to the list of pending queries, depending on the current
	 * state of the rate limiter
	 */
	void executeNextQuery_(minidsp::Query::Message const& message);
	/**
	 * Restart the rate limiter timer
	 */
	void restartTimer_();
	/**
	 * Execute the query (should be called only by the timer event and executeNextQuery_
	 */
	void executeQuery_(minidsp::Query::Message const& message);

	/**
	 * Adds a query to the pending list. Merge similar queries together
	 * (ie, will only send a single volume change query for multiple pending
	 * changes). Should only be called by executeNextQuery_
	 */
	void addPendingQuery_(minidsp::Query::Message const& message);
};

}
}
}

#endif // NANOAVRCLIENT_H
