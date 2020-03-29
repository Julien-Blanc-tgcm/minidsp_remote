#include "nanoavrbasicsettings.h"

namespace eu
{
namespace tgcm
{
namespace minidsp_remote
{

NanoAVRBasicSettings::NanoAVRBasicSettings(QObject *parent) : QObject(parent)
{

}

int NanoAVRBasicSettings::volume() const
{
	return volume_;
}

int NanoAVRBasicSettings::currentPreset() const
{
	return currentPreset_;
}

int NanoAVRBasicSettings::currentHdmiInput() const
{
	return currentHdmiInput_;
}

bool NanoAVRBasicSettings::muted() const
{
	return muted_;
}

void NanoAVRBasicSettings::setVolume(int volume)
{
	if (volume_ == volume)
		return;

	volume_ = volume;
	emit volumeChanged(volume_);
}

void NanoAVRBasicSettings::setCurrentPreset(int currentPreset)
{
	if (currentPreset_ == currentPreset)
		return;

	currentPreset_ = currentPreset;
	emit currentPresetChanged(currentPreset_);
}

void NanoAVRBasicSettings::setCurrentHdmiInput(int currentHdmiInput)
{
	if (currentHdmiInput_ == currentHdmiInput)
		return;

	currentHdmiInput_ = currentHdmiInput;
	emit currentHdmiInputChanged(currentHdmiInput_);
}

void NanoAVRBasicSettings::setMuted(bool muted)
{
	if (muted_ == muted)
		return;

	muted_ = muted;
	emit mutedChanged(muted_);
}

}
}
}
