#include "audioinfo.h"

AudioInfo::AudioInfo(QObject *parent) : QObject(parent)
{
    _id = -1;
    _samplingRate = 0;
    _channels = "";
    _bitrate = 0;
    _codec = ffmpeg->audioEncoder("");
    _language = new FFLanguage("");
    _sampleFormat = ffmpeg->sampleFormat("");
}

AudioInfo::AudioInfo(QJsonObject obj, QObject *parent) : QObject(parent)
{
    _language = nullptr;
    _id = -1;
    setSamplingRate(obj.value("samplingRate").toInt(), true);
    setChannels(obj.value("channels").toString(), true);
    setBitrate(obj.value("bitrate").toInt(), true);
    setCodec(obj.value("codec").toObject(), true);
    setLanguage(obj.value("language").toObject().value("name").toString(), true);
    setSampleFormat(obj.value("sampleFormat").toObject(), true);
}

void AudioInfo::copyFrom(AudioInfo *other, bool silent)
{
    _id = other->id();
    _samplingRate = other->samplingRate();
    _channels = other->channels();
    _bitrate = other->bitrate();
    _codec = other->codec();
    delete _language;
    _language = other->language();
    _sampleFormat = other->sampleFormat();

    if(!silent) emit changed();
}

bool AudioInfo::isCopy()
{
    return _codec->name() == "copy";
}

QJsonObject AudioInfo::toJson()
{
    QJsonObject obj;
    obj.insert("samplingRate", _samplingRate);
    obj.insert("channels",_channels);
    obj.insert("bitrate", _bitrate);
    obj.insert( "codec", _codec->toJson());
    obj.insert("language", _language->toJson());
    obj.insert("sampleFormat", _sampleFormat->toJson());

    return obj;
}

int AudioInfo::samplingRate() const
{
    return _samplingRate;
}

void AudioInfo::setSamplingRate(int samplingRate, bool silent)
{
    _samplingRate = samplingRate;
    if(!silent) emit changed();
}

QString AudioInfo::channels() const
{
    return _channels;
}

void AudioInfo::setChannels(const QString &channels, bool silent)
{
    _channels = channels;
    if(!silent) emit changed();
}

qint64 AudioInfo::bitrate() const
{
    return _bitrate;
}

void AudioInfo::setBitrate(const qint64 &bitrate, bool silent)
{
    _bitrate = bitrate;
    if(!silent) emit changed();
}

FFCodec *AudioInfo::codec() const
{
    return _codec;
}

void AudioInfo::setCodec(FFCodec *codec, bool silent)
{
    _codec = codec;
    if(!silent) emit changed();
}

void AudioInfo::setCodec(QString name, bool silent)
{
    setCodec( ffmpeg->audioEncoder(name), silent);
}

void AudioInfo::setCodec(QJsonObject obj, bool silent)
{
    setCodec( obj.value("name").toString(), silent);
}

FFLanguage *AudioInfo::language() const
{
    return _language;
}

void AudioInfo::setLanguage(const QString &languageId, bool silent)
{
    delete _language;
    _language = new FFLanguage( languageId );
    if(!silent) emit changed();
}

int AudioInfo::id() const
{
    return _id;
}

void AudioInfo::setId(int id, bool silent)
{
    _id = id;
    if(!silent) emit changed();
}

FFSampleFormat *AudioInfo::sampleFormat() const
{
    return _sampleFormat;
}

void AudioInfo::setSampleFormat(FFSampleFormat *sampleFormat, bool silent)
{
    _sampleFormat = sampleFormat;
    if(!silent) emit changed();
}

void AudioInfo::setSampleFormat(QString name, bool silent)
{
    setSampleFormat(ffmpeg->sampleFormat(name), silent);
}

void AudioInfo::setSampleFormat(QJsonObject obj, bool silent)
{
    setSampleFormat( obj.value("name").toString(), silent);
}
