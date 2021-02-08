#include "aftereffectsversion.h"
#include <QtDebug>

AfterEffectsVersion::AfterEffectsVersion(QString path, QObject *parent) : QObject(parent)
{
    _name = "";
    _path = path;
    _version = QVersionNumber();

    _isValid = false;

    init();
}

AfterEffectsVersion::~AfterEffectsVersion()
{
    restoreOriginalTemplates();
}

QVersionNumber AfterEffectsVersion::version() const
{
    return _version;
}

void AfterEffectsVersion::findDataPath()
{
    //try to find templates (the Ae data dir)
    QStringList dataPaths = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
    qDebug() << "Data paths: " + dataPaths.join("\n") ;
#ifdef Q_OS_WIN

    //search the right one
    foreach(QString dataPath, dataPaths)
    {
        dataPath = dataPath.replace("/" + QString(STR_COMPANYNAME) + "/" + QString(STR_PRODUCTNAME) ,"");
        dataPath = dataPath.replace("/Rainbox Laboratory/DuME","");

        dataPath = dataPath  + "/Adobe";

        qDebug() << "Searching Ae Data in: " + dataPath ;

        QDir test(dataPath);
        QFileInfoList testDirs = test.entryInfoList( QStringList("After Effects*"), QDir::Dirs );
        bool found = false;
        foreach( QFileInfo testDir, testDirs)
        {
            QString testPath = testDir.absoluteFilePath() +  "/" + QString::number( _version.majorVersion() ) + "." + QString::number( _version.minorVersion() );
            if (QDir( testPath ).exists())
            {
                _dataPath = testPath;
                found = true;
                break;
            }
        }
        if (found) break;
    }

#endif

    //TODO MAC OS
    qDebug() <<  "AE Data Location: " + _dataPath;
}

QString AfterEffectsVersion::dataPath() const
{
    return _dataPath;
}

void AfterEffectsVersion::init()
{
    QFile aerenderFile(_path);

    if (aerenderFile.exists())
    {
        _isValid = true;
        //get version
        QRegularExpression reVersion(".*After Effects ([^\\/\\\\]+)",QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatch match = reVersion.match(_path);
        QString version = "";
        if (match.hasMatch())
        {
            _name = match.captured(1);
        }

        emit newLog( "Found Ae version: " + version );

        //get real version number from aerender
        QProcess aerender(this);
        aerender.setProgram(aerenderFile.fileName());
        aerender.setArguments(QStringList("-help"));
        aerender.start();
        aerender.waitForFinished(3000);

        QString aeOutput = aerender.readAll();

        QRegularExpression reRealVersion(".*aerender version ([\\d.x]+)",QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption);
        match = reRealVersion.match(aeOutput);

        if (match.hasMatch())
        {
            emit newLog( "Found Ae version number: " + match.captured(1) );

            _name += " (" + match.captured(1) + ")";

            //get version from name
            QRegularExpression re(".* \\((\\d+)\\.(\\d+)(?:\\.(\\d+))?(?:x(\\d+))?\\)");
            //matchs the version numbers in these patterns:
            // XX (16.0x207)
            // XX (11.0.4x2)
            // XX (15.1.2)
            // XX (15.1)

            QRegularExpressionMatch match = re.match(_name);
            if (match.hasMatch())
            {
                QVector<int> v;
                //major - minor - micro - build
                v << match.captured(1).toInt() << match.captured(2).toInt() << match.captured(3).toInt() << match.captured(4).toInt();
                _version = QVersionNumber( v );

                findDataPath();
            }
        }
    }
}

bool AfterEffectsVersion::setDuMETemplates()
{
    //load render settings and output modules
    qDebug() << "Setting DuME After Effects render templates in " + _dataPath;

    //if found, replace templates
    if (_dataPath == "") return false;

    QDir aeDataDir(_dataPath);
    if (!aeDataDir.exists())
    {
        qDebug() << "Invalid Data Path";
        return false;
    }


    // if CS6 and before (<= 11.0), prefs file
    if (_version.majorVersion() <= 11)
    {
        QStringList settingsFilter("*-x64*");
        QStringList settingsFilePaths = aeDataDir.entryList(settingsFilter);

#ifdef QT_DEBUG
qDebug() << settingsFilePaths;
#endif

        //write our own settings
        foreach(QString settingsFilePath, settingsFilePaths)
        {
            if (!settingsFilePath.endsWith(".bak"))
            {
                QString bakPath = _dataPath + "/" + settingsFilePath + ".bak";
                QString origPath = _dataPath + "/" + settingsFilePath;

                qDebug() << "Replacing " + origPath;

                //rename original file
                FileUtils::move(origPath,bakPath);

                //copy our own
                FileUtils::copy(":/after-effects/cs6-prefs", origPath);
            }
        }
    }
    //if higher than CS6
    else
    {
        QStringList settingsFilter("*-indep-render*");
        QStringList outputFilter("*-indep-output*");

        QStringList settingsFilePaths = aeDataDir.entryList(settingsFilter);
        QStringList outputFilePaths = aeDataDir.entryList(outputFilter);

#ifdef QT_DEBUG
qDebug() << settingsFilePaths;
qDebug() << outputFilePaths;
#endif

        //write our own settings
        foreach(QString settingsFilePath, settingsFilePaths)
        {
            if (!settingsFilePath.endsWith(".bak"))
            {
                QString bakPath = _dataPath + "/" + settingsFilePath + ".bak";
                QString origPath = _dataPath + "/" + settingsFilePath;

                qDebug() << "Replacing " + origPath;

                //rename original file
                FileUtils::move(origPath,bakPath);

                //copy our own
                FileUtils::copy(":/after-effects/render" + QString::number( _version.majorVersion() ), origPath);
            }
        }

        //write our own modules
        foreach(QString outputFilePath, outputFilePaths)
        {
            if (!outputFilePath.endsWith(".bak"))
            {
                QString bakPath = _dataPath + "/" + outputFilePath + ".bak";
                QString origPath = _dataPath + "/" + outputFilePath;

                qDebug() << "Replacing " + origPath;

                //rename original file
                FileUtils::move(origPath,bakPath);

                //copy our own
                FileUtils::copy(":/after-effects/output", origPath);
            }
        }

    }

    qDebug() << "Templates succesfully added";

    return true;
}

void AfterEffectsVersion::restoreOriginalTemplates()
{
    //if found, replace templates
    if (_dataPath == "") return;

    QDir aeDataDir(_dataPath);
    if (!aeDataDir.exists()) return;

    // if CS6 and before (<= 11.0), prefs file
    if (_version.majorVersion() <= 11)
    {
        QStringList settingsFilter("*-x64*");
        QStringList settingsFilePaths = aeDataDir.entryList(settingsFilter);

        //remove our own settings
        foreach(QString settingsFilePath, settingsFilePaths)
        {
            QString bakPath = _dataPath + "/" + settingsFilePath;
            QString txtPath = _dataPath + "/" + settingsFilePath.replace(".bak","");


            qDebug() << "Restoring " + bakPath;


            //remove our own
            FileUtils::remove(txtPath);

            //rename original file
            FileUtils::move(bakPath, txtPath);
        }
    }
    //if higher than CS6
    else
    {
        QStringList settingsFilter("*-indep-render*.bak");
        QStringList outputFilter("*-indep-output*.bak");

        QStringList settingsFilePaths = aeDataDir.entryList(settingsFilter);
        QStringList outputFilePaths = aeDataDir.entryList(outputFilter);

        //remove our own settings
        foreach(QString settingsFilePath, settingsFilePaths)
        {
            QString bakPath = _dataPath + "/" + settingsFilePath;
            QString txtPath = _dataPath + "/" + settingsFilePath.replace(".bak","");

            qDebug() << "Restoring " + bakPath;

            //remove our own
            FileUtils::remove(txtPath);

            //rename original file
            FileUtils::move(bakPath, txtPath);
        }

        //remove our own modules
        foreach(QString outputFilePath, outputFilePaths)
        {
            QString bakPath = _dataPath + "/" + outputFilePath;
            QString txtPath = _dataPath + "/" + outputFilePath.replace(".bak","");


            qDebug() << "Restoring " + bakPath;

            //remove our own
            FileUtils::remove(txtPath);

            //rename original file
            FileUtils::move(bakPath, txtPath);
        }
    }
}

bool AfterEffectsVersion::isValid() const
{
    return _isValid;
}

QString AfterEffectsVersion::name() const
{
    return _name;
}

QString AfterEffectsVersion::path() const
{
    return QDir::toNativeSeparators(_path);
}
