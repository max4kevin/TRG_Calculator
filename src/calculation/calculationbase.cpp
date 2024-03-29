//#define QT_NO_DEBUG_OUTPUT
#include "calculationbase.h"
#include "geometry.h"
#include <QFile>
#include <QGuiApplication>

CalculationBase::CalculationBase(IFrontendConnector& frontendConnector, const QString &name)
    : frontendConnector_(frontendConnector)
    , name_(name)
    , workPointsIt_(workPoints_.begin())
{
    Point tempPoint = {QPoint(0, 0), false, false, false, true, CALIB_COLOR};

    workPoints_.append({points_.insert("Calibration 1", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "First calibration point for pixels to millimeters translation coefficient calculation")});

    workPoints_.append({points_.insert("Calibration 2", tempPoint),
    QT_TRANSLATE_NOOP("Calculation", "Second calibration point for pixels to millimeters translation coefficient calculation")});
}

qreal CalculationBase::calibrationLength_ = 40;

CalculationBase::~CalculationBase(){}

void CalculationBase::loadResultsTable()
{
    qDebug() << "Loading results table";
    for (auto &it : results_)
    {
        frontendConnector_.addResult(it.key(), it.value().referenceValue, QCoreApplication::translate("Calculation", it.value().description));
    }
}

void CalculationBase::loadPointsTable()
{
    qDebug() << "Loading points table";
    for (auto &it : workPoints_)
    {
        frontendConnector_.addPoint(it.point.key(), it.point.value().isReady, QCoreApplication::translate("Calculation", it.description));
    }
    recalculateIt(); //For actual msg
}

void CalculationBase::loadFrontendPoints()
{
    qDebug() << "Loading points";
    for (auto &it : workPoints_)
    {
        updatePoint(it.point.key());
    }
    //Separate cycle to prevent errors because of frontend nonexisting points
    for (auto &it : workPoints_)
    {
        updateCalculation(it.point.key());
    }
}

//FIXME: Crutch
void CalculationBase::deleteFrontendPoints()
{
    qDebug() << "Deleting points";
    for (auto &key : points_.keys())
    {
        frontendConnector_.updatePoint(key, "", 0, 0, false, false, false, false);
        qDebug() << "Deleting point" << key;
    }
}

void CalculationBase::reset()
{
    qDebug() << "Reseting calculation";
    undoHistory_.clear();
    redoHistory_.clear();
    frontendConnector_.changeUndoState(false);
    frontendConnector_.changeRedoState(false);
    clearAll();
}

void CalculationBase::writeCoordinates(qreal x, qreal y)
{
    if (!points_.size())
    {
        qDebug() << "writeCoordinates(): points_.size() == 0";
        return;
    }
    if (workPointsIt_ != workPoints_.end())
    {
        saveState({workPointsIt_->point.value(), workPointsIt_->point.key()});
        workPointsIt_->point.value().coordinates = QPointF(x,y);
        workPointsIt_->point.value().isReady = true;
        updatePoint(workPointsIt_->point.key());
        qDebug() << workPointsIt_->point.key() << workPointsIt_->point.value().coordinates;
        updateCalculation(workPointsIt_->point.key());
        recalculateIt();
    }
    else
    {
        qDebug() << "End of points";
    }
}

void CalculationBase::writeCoordinates(const QString& pointName, qreal x, qreal y)
{
    if (points_.contains(pointName))
    {
        saveState({points_[pointName], pointName});
        points_[pointName].coordinates = QPointF(x, y);
        points_[pointName].isReady = true;
        updatePoint(pointName);
        qDebug() << pointName << points_[pointName].coordinates;
        updateCalculation(pointName);
        recalculateIt();
    }
}

void CalculationBase::setCalibrationLength(qreal length)
{
    calibrationLength_ = length;
}

qreal CalculationBase::getCalibrationLength()
{
    return calibrationLength_;
}

void CalculationBase::checkPointsConnection(const QString& checkPoint, const QString &pName1, const QString &pName2)
{
    if (checkPoint == pName1 || checkPoint == pName2)
    {
        if (points_[pName1].isReady && points_[pName2].isReady)
        {
            frontendConnector_.connectPoints(pName1, pName2, LINES_COLOR);
        }
    }
}

void CalculationBase::removePoint(const QString& pointName)
{
    if (points_.find(pointName) != points_.end())
    {
        saveState({points_[pointName], pointName});
        points_[pointName].isReady = false;
        updateCalculation(pointName);
        updatePoint(pointName);
        recalculateIt();
    }
}

void CalculationBase::undo()
{
    if (undoHistory_.size())
    {
        PointState state = undoHistory_.takeLast();
        redoHistory_.append({points_[state.pointName], state.pointName});
        frontendConnector_.changeRedoState(true);
        setState(state);
        if (!undoHistory_.size())
        {
            frontendConnector_.changeUndoState(false);
        }
    }
}

void CalculationBase::redo()
{
    if (redoHistory_.size())
    {
        PointState state = redoHistory_.takeLast();
        undoHistory_.append({points_[state.pointName], state.pointName});
        frontendConnector_.changeUndoState(true);
        setState(state);
        if (!redoHistory_.size())
        {
            frontendConnector_.changeRedoState(false);
        }
    }
}

void CalculationBase::clear()
{
    clearFrom(2);
}

void CalculationBase::clearAll()
{
    clearFrom(0);
}

//TODO: Error messages
void CalculationBase::saveData(const QString& path, const QString& name)
{
    QFile file(path+name+"_"+name_+"_data.csv");
    if (!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        qDebug() << "File open error";
        return;
    }
    if (file.exists())
    {
        //TODO: Call message dialog
        qDebug() << "File rewrited";
        file.resize(0);
    }
    QTextStream out(&file);
    out.setCodec("Windows-1251");
    for (auto &result: results_)
    {
        out << result.key() << "," << result->value << "," << result->referenceValue << Qt::endl;
    }
    out.flush();
    qDebug() << "File saved";
}

const WorkPoints& CalculationBase::getPoints() const
{
    return workPoints_;
}

void CalculationBase::loadPoints(const QVector<Point>& points)
{
    if (points.size() != workPoints_.size()) {
        qDebug() << "Error: wrong points vector size!";
    }
    reset();
    auto p = points.begin();
    for (auto &it: workPoints_)
    {
        it.point.value() = *p;
        updatePoint(it.point.key());
        updateCalculation(it.point.key());
        ++p;
    }
    recalculateIt();
    qDebug() << "Points loaded";
}

void CalculationBase::checkCalibration(const QString& checkPoint)
{
    if (checkPoint == "Calibration 1" || checkPoint == "Calibration 2")
    {
        if (points_["Calibration 1"].isReady && points_["Calibration 2"].isReady)
        {
            frontendConnector_.connectPoints("Calibration 1", "Calibration 2", CALIB_COLOR);
            calibrationValue_ = calibrationLength_/geometry::calculateDistance(points_["Calibration 1"].coordinates, points_["Calibration 2"].coordinates);
        }
    }
}

void CalculationBase::checkAngle(const QString& checkPoint, const QString &dataName, const QString &pName1, const QString &pName2, const QString &pName3)
{
    if (checkPoint == pName1 || checkPoint == pName2 || checkPoint == pName3)
    {
        if ( points_[pName1].isReady && points_[pName2].isReady && points_[pName3].isReady )
        {
            resultsTable_[dataName].value =
                    QString::number(geometry::calculateAngleDeg(points_[pName1].coordinates, points_[pName2].coordinates, points_[pName3].coordinates), 'f', 2);
        }
        else
        {
            resultsTable_[dataName].value = "";
        }
        updateResult(dataName);
    }
}

void CalculationBase::checkDistance(const QString& checkPoint, const QString &dataName, const QString &pName1, const QString &pName2)
{
    if (checkPoint == pName1 || checkPoint == pName2
        || checkPoint == "Calibration 1" || checkPoint == "Calibration 2")
    {
        if ( points_[pName1].isReady && points_[pName2].isReady
             && points_["Calibration 1"].isReady && points_["Calibration 2"].isReady)
        {
            resultsTable_[dataName].value =
                    QString::number(geometry::calculateDistance(points_[pName1].coordinates, points_[pName2].coordinates)*getCalibrationValue(), 'f', 2);
        }
        else
        {
            resultsTable_[dataName].value = "";
        }
        updateResult(dataName);
    }
}

void CalculationBase::checkRatio(const QString& checkPoint, const QString &dataName, const QString &pName1, const QString &pName2, const QString &pName3, const QString &pName4)
{
    if (checkPoint == pName1 || checkPoint == pName2 || checkPoint == pName3 || checkPoint == pName4
        || checkPoint == "Calibration 1" || checkPoint == "Calibration 2")
    {
        if ( points_[pName1].isReady && points_[pName2].isReady && points_[pName3].isReady && points_[pName4].isReady
             && points_["Calibration 1"].isReady && points_["Calibration 2"].isReady)
        {
            qreal dist1 = geometry::calculateDistance(points_[pName1].coordinates, points_[pName2].coordinates);
            qreal dist2 = geometry::calculateDistance(points_[pName3].coordinates, points_[pName4].coordinates);
            resultsTable_[dataName].value = QString::number(dist1/dist2*100, 'f', 2);
        }
        else
        {
            resultsTable_[dataName].value = "";
        }
        updateResult(dataName);
    }
}

void CalculationBase::checkProjection(const QString& checkPoint, const QString &dataName, const QString &pName1, const QString &pName2, const QString &pName3, const QString &pName4)
{
    if (checkPoint == pName1 || checkPoint == pName2 || checkPoint == pName3 || checkPoint == pName4
        || checkPoint == "Calibration 1" || checkPoint == "Calibration 2")
    {
        if ( points_[pName1].isReady && points_[pName2].isReady && points_[pName3].isReady && points_[pName4].isReady
             && points_["Calibration 1"].isReady && points_["Calibration 2"].isReady)
        {
            resultsTable_[dataName].value =
                    QString::number(geometry::calculateProjection(points_[pName1].coordinates, points_[pName2].coordinates,
                                                                  points_[pName3].coordinates, points_[pName4].coordinates)*getCalibrationValue(), 'f', 2);
        }
        else
        {
            resultsTable_[dataName].value = "";
        }
        updateResult(dataName);
    }
}

void CalculationBase::checkCross(const QString& checkPoint, const QString &pointName, const QString &pName1, const QString &pName2, const QString &pName3, const QString &pName4)
{
    if (checkPoint == pName1 || checkPoint == pName2 || checkPoint == pName3 || checkPoint == pName4)
    {
        if ( points_[pName1].isReady && points_[pName2].isReady && points_[pName3].isReady && points_[pName4].isReady )
        {
            geometry::Line line1(points_[pName1].coordinates, points_[pName2].coordinates);
            geometry::Line line2(points_[pName3].coordinates, points_[pName4].coordinates);
            points_[pointName].coordinates = geometry::getCross(line1, line2);
            points_[pointName].isReady = true;
        }
        else
        {
            points_[pointName].isReady = false;
        }
        updatePoint(pointName);
        updateCalculation(pointName);
    }
}

void CalculationBase::checkPerpendicular(const QString& checkPoint, const QString &pointName, const QString &pName0, const QString &pName1, const QString &pName2)
{
    if (checkPoint == pName0 || checkPoint == pName1 || checkPoint == pName2)
    {
        if ( points_[pName0].isReady && points_[pName1].isReady && points_[pName2].isReady )
        {
            geometry::Line line1(points_[pName1].coordinates, points_[pName2].coordinates);
            points_[pointName].coordinates = geometry::getPerpendicularBase(points_[pName0].coordinates, line1);
            points_[pointName].isReady = true;
        }
        else
        {
            points_[pointName].isReady = false;
        }
        updatePoint(pointName);
        updateCalculation(pointName);
    }
}

void CalculationBase::checkBisect(const QString& checkPoint, const QString &pointName, const QString &pName1, const QString &pName2, const QString &pName3)
{
    if (checkPoint == pName1 || checkPoint == pName2 || checkPoint == pName3)
    {
        if ( points_[pName1].isReady && points_[pName2].isReady && points_[pName3].isReady )
        {
            points_[pointName].coordinates = geometry::getBisectorBase(points_[pName1].coordinates, points_[pName2].coordinates, points_[pName3].coordinates);
            points_[pointName].isReady = true;
        }
        else
        {
            points_[pointName].isReady = false;
        }
        updatePoint(pointName);
        updateCalculation(pointName);
    }
}

void CalculationBase::checkMiddlePoint(const QString& checkPoint, const QString &pointName, const QString &pName1, const QString &pName2)
{
    if (checkPoint == pName1 || checkPoint == pName2)
    {
        if ( points_[pName1].isReady && points_[pName2].isReady)
        {
            points_[pointName].coordinates = geometry::getMiddlePoint(points_[pName1].coordinates, points_[pName2].coordinates);
            points_[pointName].isReady = true;
        }
        else
        {
            points_[pointName].isReady = false;
        }
        updatePoint(pointName);
        updateCalculation(pointName);
    }
}

void CalculationBase::checkAddPerpendicular(const QString& checkPoint, const QString &pointName, const QString &pName1, const QString &pName2)
{
    if (checkPoint == pName1 || checkPoint == pName2)
    {
        if ( points_[pName1].isReady && points_[pName2].isReady)
        {
            points_[pointName].coordinates = geometry::getPerpendicularPoint(points_[pName1].coordinates, points_[pName2].coordinates);
            points_[pointName].isReady = true;
        }
        else
        {
            points_[pointName].isReady = false;
        }
        updatePoint(pointName);
        updateCalculation(pointName);
    }
}

void CalculationBase::updateResult(const QString& resultName)
{
    frontendConnector_.updateResult(resultName, resultsTable_[resultName].value, resultsTable_[resultName].referenceValue);
}

void CalculationBase::updatePoint(const QString &pointName)
{
    frontendConnector_.updatePoint(pointName, points_[pointName].color, points_[pointName].coordinates.x(), points_[pointName].coordinates.y(),
    points_[pointName].isTilted, points_[pointName].isEntilted, points_[pointName].isVisible, points_[pointName].isReady);
}

qreal CalculationBase::getCalibrationValue()
{
    return calibrationValue_;
}

void CalculationBase::clearFrom(int number)
{
    if ((number>=0)&&(number < workPoints_.size()))
    {
//        undoHistory_.clear(); //FIXME: temp solution
        redoHistory_.clear();
        for (auto it = workPoints_.begin()+number; it != workPoints_.end(); ++it)
        {
            if (it->point.value().isReady)
            {
                saveState({it->point.value(), it->point.key()});
                it->point.value().isReady = false;
                updateCalculation(it->point.key());
                updatePoint(it->point.key());
            }
        }
        recalculateIt();
    }
    else
    {
        qDebug() << "Error: clearing from " << number << "while size ="<<workPoints_.size();
    }
}

void CalculationBase::setState(const PointState& state)
{
    points_[state.pointName] = state.point;
    updatePoint(state.pointName);
    recalculateIt();
    updateCalculation(state.pointName);
}

void CalculationBase::saveState(const PointState& state)
{
    if (redoHistory_.size())
    {
        redoHistory_.clear();
    }
    undoHistory_.append(state);
    frontendConnector_.changeRedoState(false);
    frontendConnector_.changeUndoState(true);
}

void CalculationBase::recalculateIt()
{
    //Searching for next writable point
    workPointsIt_ = workPoints_.begin();
    if (!points_.size())
    {
        qDebug() << "recalculateIt(): points_.size() == 0";
        return;
    }
    while (workPointsIt_ != workPoints_.end())
    {
        if (!workPointsIt_->point.value().isReady)
        {
            break;
        }
        ++workPointsIt_;
    }
    if (workPointsIt_ == workPoints_.end())
    {
        frontendConnector_.sendMsg(QCoreApplication::translate("main","Drawing done"));
    }
    else
    {
        frontendConnector_.sendMsg(QCoreApplication::translate("main", "Now drawing point: ") + workPointsIt_->point.key());
    }
}

const QString &CalculationBase::name() const
{
    return name_;
}
