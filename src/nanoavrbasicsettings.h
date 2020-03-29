#ifndef NANOAVRBASICSETTINGS_H
#define NANOAVRBASICSETTINGS_H

#include <QObject>

namespace eu
{
namespace tgcm
{
namespace minidsp_remote
{

class NanoAVRBasicSettings : public QObject
{
	Q_OBJECT

	/**
	 * Volume, in the range [-255, 0], where 0 is the max volume and -255 is the min
	 */
	Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
	/**
	 * Current equalizer preset, in the range [1-4]
	 */
	Q_PROPERTY(int currentPreset READ currentPreset WRITE setCurrentPreset NOTIFY currentPresetChanged)
	/**
	 * Current HDMI input, in the range [1-2]
	 */
	Q_PROPERTY(int currentHdmiInput READ currentHdmiInput WRITE setCurrentHdmiInput NOTIFY currentHdmiInputChanged)
	/**
	 * Whether the sound is muted (true) or not
	 */
	Q_PROPERTY(bool muted READ muted WRITE setMuted NOTIFY mutedChanged)
	int volume_;

	int currentPreset_;

	int currentHdmiInput_;

	bool muted_;

public:
	explicit NanoAVRBasicSettings(QObject *parent = nullptr);

	int volume() const;

	int currentPreset() const;

	int currentHdmiInput() const;

	bool muted() const;

signals:

	void volumeChanged(int volume);

	void currentPresetChanged(int currentPreset);

	void currentHdmiInputChanged(int currentHdmiInput);

	void mutedChanged(bool muted);

public slots:
	void setVolume(int volume);
	void setCurrentPreset(int currentPreset);
	void setCurrentHdmiInput(int currentHdmiInput);
	void setMuted(bool muted);
};

}
}
}
#endif // NANOAVRBASICSETTINGS_H
