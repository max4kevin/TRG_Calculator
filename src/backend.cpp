#include <QFile>
#include <QDir>
#include <QGuiApplication>
#include "backend.h"

#define TRG_CODE 0xDEAF
#define VERSION_CODE 0x0001
#define MAPO_CODE 0xBAF0
#define JRA_CODE 0xD3AA

//TODO: Resending text fields to frontend after language switch
//TODO: logging!

BackEnd::BackEnd(QQmlApplicationEngine &engine)
    : QQuickImageProvider(QQuickImageProvider::Image)
    , engine_(&engine)
    , calculationMethodMAPO_(*this)
    , calculationMethodJRA_(*this)
    , settings_(QCoreApplication::applicationDirPath()+"/config.ini", QSettings::IniFormat)
{
    qDebug() << "Config filepath:" << settings_.fileName();
    settings_.beginGroup("Settings");
    //Checking settings
    if (!settings_.contains("method"))
    {
        settings_.setValue("method", "MAPO");
    }
    if (!settings_.contains("calibLength"))
    {
        settings_.setValue("calibLength", CalculationBase::getCalibrationLength());
    }
    if (!settings_.contains("saveDir"))
    {
        settings_.setValue("saveDir", "default");
    }
    if (!settings_.contains("theme"))
    {
        settings_.setValue("theme", "dark");
    }
    if (!settings_.contains("lang"))
    {
        settings_.setValue("lang", QLocale::system().name());
    }

    QString method = settings_.value("method").toString();
    if (method == calculationMethodJRA_.name())
    {
        actualCalculationMethod_ = &calculationMethodJRA_;
    }
    else
    {
        actualCalculationMethod_ = &calculationMethodMAPO_;
    }
    CalculationBase::setCalibrationLength(settings_.value("calibLength").toDouble());

    qtTranslator_.load(QLatin1String("QtLanguage_")+settings_.value("lang").toString(), QLatin1String(":/"));

    QCoreApplication::installTranslator(&qtTranslator_);
    settings_.endGroup();
}

QImage BackEnd::requestImage(const QString &/*id*/, QSize *size, const QSize &/*requestedSize*/)
{
    if(size) {
        *size = image_.size();
    }

    qDebug() << "Loading image";

    return image_;
}

void BackEnd::loadData()
{
    emit clearTables();
    actualCalculationMethod_->loadResultsTable();
    actualCalculationMethod_->loadPointsTable();
}

void BackEnd::reset()
{
    calculationMethodMAPO_.reset();
    calculationMethodJRA_.reset();
    emit clearTables();
    //TODO: reseting other methods
}

void BackEnd::writeCoordinates(qreal x, qreal y)
{
    actualCalculationMethod_->writeCoordinates(x,y);
}

void BackEnd::writeCoordinates(const QString& pointName, qreal x, qreal y)
{
    actualCalculationMethod_->writeCoordinates(pointName, x, y);
}

void BackEnd::removePoint(const QString &pointName)
{
    actualCalculationMethod_->removePoint(pointName);
}

void BackEnd::setCalibrationLength(qreal length)
{
    CalculationBase::setCalibrationLength(length);
    setConfig("calibLength", QString::number(length));
}

qreal BackEnd::getCalibrationLength()
{
    return CalculationBase::getCalibrationLength();
}

void BackEnd::undo()
{
    actualCalculationMethod_->undo();
}

void BackEnd::redo()
{
    actualCalculationMethod_->redo();
}

void BackEnd::clear()
{
    actualCalculationMethod_->clear();
}

void BackEnd::clearAll()
{
    actualCalculationMethod_->clearAll();
}

void BackEnd::saveData(const QString& path, const QString& name)
{
    actualCalculationMethod_->saveData(path, name);
}

void BackEnd::openFile(const QString& filePath)
{
    QString fileString = filePath.split("///").last();

    if (filePath.split(".").last() == "trg")
    {
        //Check first byte and switch to correct method
        QFile file(fileString);
        if(file.open(QIODevice::ReadOnly))
        {
            QDataStream stream(&file);
            stream.setVersion (QDataStream::Qt_4_7);
            uint16_t code;
            stream >> code;
            if (code != TRG_CODE)
            {
                file.close();
                qDebug() << "Incorrect trg file: trg code failed " << code;
                emit error(QCoreApplication::translate("main", "Incorrect file"));
                return;
            }
            stream >> code;
            qDebug() << "File version: " << code;

            stream >> code;
            if (code == MAPO_CODE) {
                actualCalculationMethod_ = &calculationMethodMAPO_;
            }
            else if (code == JRA_CODE) {
                actualCalculationMethod_ = &calculationMethodJRA_;
            } //TODO: else if - another methods
            else
            {
                file.close();
                qDebug() << "Incorrect trg file: method code failed " << code;
                emit error(QCoreApplication::translate("main", "Incorrect file"));
                return;
            }
            //reading points and image
            QVector<Point> points;
            for (auto i = 0; i < actualCalculationMethod_->getPoints().size(); ++i)
            {
                Point point;
                stream >> point.coordinates;
                stream >> point.isReady;
                stream >> point.isTilted;
                stream >> point.isEntilted;
                stream >> point.isVisible;
                stream >> point.color;
                points.append(point);
            }
            QImage image;
            stream >> image;
            if(stream.status() != QDataStream::Ok)
            {
                qDebug() << "File reading error";
                file.close();
                return;
            }

            emit methodChanged(actualCalculationMethod_->name());

            image_ = image;
            emit fileLoaded();
            loadData();
            actualCalculationMethod_->loadPoints(points);
            file.close();
        }
        else
        {
            qDebug() << "File opening error";
        }

    }
    else
    {
        reset();

        image_ = QImage(fileString);
        emit fileLoaded();
        loadData();
    }

    filePath_ = fileString;
    filePath_.replace(filePath_.split("/").last(), "");
    fileName_ = filePath.split("/").last();
    fileName_.replace("."+fileName_.split(".").last(), "");
    fileType_ = filePath.split(".").last();

    qDebug() << "File "+fileString+" opened";
    qDebug() << "FilePath: "+filePath_;
    qDebug() << "FileName: "+fileName_;
    qDebug() << "FileFormat: "+fileType_;
}

void BackEnd::saveFile(const QString& filePath)
{
    qDebug() << "Saving "+filePath+" file...";
    if (image_.isNull())
    {
        qDebug() << "Saving null image";
        return;
    }

    uint16_t code;

    if (actualCalculationMethod_ == &calculationMethodMAPO_)
    {
         code = MAPO_CODE;
    }
    else if (actualCalculationMethod_ == &calculationMethodJRA_) {
        code = JRA_CODE;
    }
    else
    {
        return;
    }
    QFile file(filePath);
    if(file.open(QIODevice::WriteOnly))
    {
        QDataStream stream(&file);
        stream.setVersion(QDataStream::Qt_4_7);
        stream << (uint16_t)TRG_CODE;
        stream << (uint16_t)VERSION_CODE;
        stream << code;
        const WorkPoints& points = actualCalculationMethod_->getPoints();
        for (auto &p: points)
        {
            stream << p.point.value().coordinates;
            stream << p.point.value().isReady;
            stream << p.point.value().isTilted;
            stream << p.point.value().isEntilted;
            stream << p.point.value().isVisible;
            stream << p.point.value().color;
        }
        stream << image_;
        if(stream.status() != QDataStream::Ok)
        {
            qDebug() << "File writing error";
        }
        else {
            filePath_ = filePath;
            filePath_.replace(filePath_.split("/").last(), "");
            fileName_ = filePath.split("/").last();
            fileName_.replace("."+fileName_.split(".").last(), "");
            fileType_ = filePath.split(".").last();
        }
    }
    else {
        qDebug() << "File creating error";
    }
    file.close();
}

void BackEnd::saveFile()
{
    saveFile(filePath_+fileName_+".trg");
}

QString BackEnd::getFilePath()
{
    return filePath_;
}

QString BackEnd::getFileName()
{
    return fileName_;
}

QString BackEnd::getFileType()
{
    return fileType_;
}

void BackEnd::invertImage()
{
    if (!image_.isNull())
    {
        image_.invertPixels();
        emit imageUpdated();
    }
}

void BackEnd::setLanguage(const QString &lang)
{
    if (!qtTranslator_.isEmpty())
    {
        QCoreApplication::removeTranslator(&qtTranslator_);
    }
    qtTranslator_.load(QLatin1String("QtLanguage_")+lang, QLatin1String(":/"));
    QCoreApplication::installTranslator(&qtTranslator_);
    engine_->retranslate();
    actualCalculationMethod_->loadPointsTable();
    actualCalculationMethod_->loadResultsTable();
    setConfig("lang", lang);
}

void BackEnd::setMethod(const QString &method)
{
    actualCalculationMethod_->deleteFrontendPoints();
    if (method == calculationMethodJRA_.name())
    {
        actualCalculationMethod_ = &calculationMethodJRA_;
    }
    else
    {
        actualCalculationMethod_ = &calculationMethodMAPO_;
    }
    setConfig("method", method);
    if (!image_.isNull())
    {
        //TODO: Copy calibration points to another method?
        loadData();
        actualCalculationMethod_->loadFrontendPoints();
    }
}

void BackEnd::setConfig(const QString &key, const QString &value)
{
    settings_.beginGroup("Settings");
    settings_.setValue(key, value);
    settings_.endGroup();
}

QString BackEnd::getConfig(const QString &key)
{
    settings_.beginGroup("Settings");
    QString value = settings_.value(key).toString();
    settings_.endGroup();
    return value;
}

bool BackEnd::isDirectoryValid(const QString &directory)
{
    return QDir(directory).exists();
}

void BackEnd::addPoint(const QString &pointName, bool status, const QString &description)
{
    emit pointAdded(pointName, status, description);
}

void BackEnd::updatePoint(const QString& pointName, const QString& color, qreal x, qreal y, bool isTilted, bool isEntilted, bool isVisible, bool status)
{
    emit  pointUpdated(pointName, color, x, y, isTilted, isEntilted, isVisible, status);
}

void BackEnd::connectPoints(const QString& pointName1, const QString& pointName2, const QString& color)
{
    emit pointsConnected(pointName1, pointName2, color);
}

void BackEnd::addResult(const QString &resultName, const QString &resultReference, const QString& description)
{
    emit resultAdded(resultName, resultReference, description);
}

void BackEnd::updateResult(const QString& resultName, const QString& resultValue, const QString& resultReference)
{
    emit resultUpdated(resultName, resultValue, resultReference);
}

void BackEnd::sendMsg(const QString& msg)
{
    emit newMsg(msg);
}

void BackEnd::changeUndoState(bool isEnabled)
{
    emit undoStateChanged(isEnabled);
}

void BackEnd::changeRedoState(bool isEnabled)
{
    emit redoStateChanged(isEnabled);
}
