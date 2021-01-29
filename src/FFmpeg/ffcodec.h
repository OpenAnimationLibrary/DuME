﻿#ifndef FFMPEGCODEC_H
#define FFMPEGCODEC_H

#include "ffbaseobject.h"
#include "ffpixformat.h"
#include "duqf-utils/utils.h"

#include <QString>

class FFCodec : public FFBaseObject
{
    Q_OBJECT
public:

    /**
     * @brief The type of media handled by the codec
     */
    enum Ability { Audio = 1 << 0,
                   Video = 1 << 1,
                   Subtitle = 1 << 2,
                   Encoder = 1 << 3,
                   Decoder = 1 << 4,
                   Lossy = 1 << 5,
                   Lossless = 1 << 6,
                   IFrame = 1 << 7
                 };
    Q_DECLARE_FLAGS(Abilities, Ability)

    enum Capability { Speed = 1 << 0,
                      Tuning = 1 << 1,
                      Quality = 1 << 2,
                      BitrateType = 1 << 3,
                      Profile = 1 << 4
                     };
    Q_DECLARE_FLAGS(Capabilities, Capability)


    /**
     * @brief FFmpegCodec Constructs a codec instance for FFmpeg
     * @param name The internal name used by FFmpeg
     * @param prettyName The pretty name used for user interaction
     */
    FFCodec(QString name, QString prettyName = "", QObject *parent = nullptr);

    /**
     * @brief FFmpegCodec Constructs a codec instance for FFmpeg
     * @param name The internal name used by FFmpeg
     * @param prettyName The pretty name used for user interaction
     * @param abilities The abilities of the codec
     */
    FFCodec(QString name, QString prettyName, Abilities abilities, QObject *parent = nullptr);

    /**
     * @brief isVideo Is this a video codec
     * @return
     */
    bool isVideo() const;
    /**
     * @brief isAudio Is this an audio codec
     * @return
     */
    bool isAudio() const;
    /**
     * @brief isEncoder Can this codec be used for encoding
     * @return The encoding ability
     */
    bool isEncoder() const;
    /**
     * @brief isDecoder Can this codec be used for decoding
     * @return The decoding ability
     */
    bool isDecoder() const;
    /**
     * @brief isLossy Does this codec use lossy compression
     * @return Lossy compression ability
     */
    bool isLossy() const;
    /**
     * @brief isLossless Does this codec use lossless compression
     * @return Lossless compression ability
     */
    bool isLossless() const;
    /**
     * @brief isIframe Is this codec intra-frame
     * @return I-Frame ability
     */
    bool isIframe() const;

    void setDecoder(bool decoder = true);
    void setEncoder(bool encoder = true);
    void setAudio(bool audio = true);
    void setVideo(bool video = true);
    void setLossy(bool lossy = true);
    void setLossless(bool lossless = true);
    void setIframe(bool iframe = true);
    void setAbilities(const Abilities &abilities);

    bool useSpeed();
    void setSpeedCapability(bool useSpeed = true);
    bool useTuning();
    void setTuningCapability(bool useTuning = true);
    bool useQuality();
    void setQualityCapability(bool useQuality = true);
    bool useBitrateType();
    void setBitrateTypeCapability(bool useType = true);
    bool useProfile();
    void setProfileCapability(bool useProfile = true);
    void setCapabilities(const Capabilities &capabilities);

    QList<FFPixFormat *> pixFormats() const;
    void addPixFormat(FFPixFormat *pixFormat);

    FFPixFormat *defaultPixFormat() const;
    FFPixFormat *defaultPixFormat(bool withAlpha);
    FFPixFormat *pixFormatWithAlpha(FFPixFormat *pf, bool alpha);
    void setDefaultPixFormat(FFPixFormat *defaultPixFormat);
    void setDefaultPixFormat();

    QList<FFBaseObject *> profiles() const;
    FFBaseObject *profile(QString name);

    QString qualityParam() const;
    QString qualityValue( int quality );

    QString speedParam() const;
    QString speedValue(int speed );

    static FFCodec *getDefault(QObject *parent = nullptr);

    QList<FFBaseObject *> tunings() const;
    FFBaseObject *tuning(QString name);


private:
    void init();

    Abilities _abilities;
    Capabilities _capabilities;
    QList<FFPixFormat *> _pixFormats;
    FFPixFormat *_defaultPixFormat;

    QString _qualityParam;
    void setQualityParam();

    QString _speedParam;
    void setSpeedParam();

    QList<FFBaseObject*> _tunings;
    QList<FFBaseObject*> _profiles;

protected:

};

Q_DECLARE_OPERATORS_FOR_FLAGS(FFCodec::Abilities)

#endif // FFMPEGCODEC_H
