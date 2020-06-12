#include "backend.h"
#include <QQmlApplicationEngine>
#include <QFile>

#define MAPO_CODE 0xBAF0

//TODO: Resending text fields to frontend after language switch

BackEnd::BackEnd(QGuiApplication &app)
    : QQuickImageProvider(QQuickImageProvider::Image), app_(&app), calculationMethodMAPO_(*this), actualCalculationMethod_(&calculationMethodMAPO_)
{
    //TODO: Check language load, if false - load English
    qtTranslator_.load(QLatin1String("QtLanguage_")+QLocale::system().name(), QLatin1String(":/"));
    app.installTranslator(&qtTranslator_);
}

QImage BackEnd::requestImage(const QString &/*id*/, QSize *size, const QSize &/*requestedSize*/)
{
    if(size) {
        *size = image_.size();
    }

    qDebug() << "Loading image";

    return image_;
}

void BackEnd::loadTable()
{
    clearTable();
    actualCalculationMethod_->loadTable();
}

void BackEnd::reset()
{
    calculationMethodMAPO_.reset();
    clearTable();
    //TODO: reseting other methods
}

void BackEnd::writeCoordinates(qreal x, qreal y)
{
    actualCalculationMethod_->writeCoordinates(x,y);
}

void BackEnd::setCalibrationLength(qreal length)
{
    CalculationBase::setCalibrationLength(length);
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

void BackEnd::movePoint(const QString& pointName, qreal x, qreal y)
{
    actualCalculationMethod_->movePoint(pointName, x, y);
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
            int code;
            stream >> code;
            if (code == MAPO_CODE)
            {
                //switch to MAPO method
            }
            else
            {
                file.close();
                qDebug() << "Incorrect trg file";
                return;
            }
            //reading points and image
            QVector<Point> points;
            for (auto i = 0; i < actualCalculationMethod_->getPoints().size(); ++i)
            {
                Point point;
                stream >> point.coordinates;
                stream >> point.isReady;
                stream >> point.color;
                stream >> point.isTilted;
                stream >> point.isEntilted;
                stream >> point.isVisible;
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
            image_ = image;
            fileLoaded();
            loadTable();
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
        fileLoaded();
        loadTable();
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

    int code;

    if (actualCalculationMethod_ == &calculationMethodMAPO_)
    {
         code = MAPO_CODE;
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
        stream << code;
        const QVector<Points::Iterator>& points = actualCalculationMethod_->getPoints();
        for (auto &p: points)
        {
            stream << p.value().coordinates;
            stream << p.value().isReady;
            stream << p.value().color;
            stream << p.value().isTilted;
            stream << p.value().isEntilted;
            stream << p.value().isVisible;
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
        imageUpdated();
    }
}

void BackEnd::updatePoint(const QString& pointName, const QString& color, qreal x, qreal y, bool isTilted, bool isEntilted, bool isVisible)
{
    pointUpdated(pointName, color, x, y, isTilted, isEntilted, isVisible);
}

void BackEnd::deletePoint(const QString& pointName)
{
    pointDeleted(pointName);
}

void BackEnd::connectPoints(const QString& pointName1, const QString& pointName2, const QString& color)
{
    pointsConnected(pointName1, pointName2, color);
}

void BackEnd::updateResult(const QString& resultName, const QString& resultValue, const QString& resultReference)
{
    resultUpdated(resultName, resultValue, resultReference);
}

void BackEnd::sendMsg(const QString& msg)
{
    newMsg(msg);
}

void BackEnd::changeUndoState(bool isEnabled)
{
    undoStateChanged(isEnabled);
}

void BackEnd::changeRedoState(bool isEnabled)
{
    redoStateChanged(isEnabled);
}
