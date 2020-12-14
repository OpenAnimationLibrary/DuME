#ifndef FFLANGUAGE_H
#define FFLANGUAGE_H

#include "FFmpeg/ffbaseobject.h"
#include "duqf-utils/language-utils.h"

class FFLanguage : public FFBaseObject
{
public:
    FFLanguage(QString name, QObject *parent = nullptr);
};

#endif // FFLANGUAGE_H
