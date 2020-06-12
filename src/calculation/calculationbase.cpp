//#define QT_NO_DEBUG_OUTPUT
#include "calculationbase.h"
#include "geometry.h"
#include <QFile>
#include <QGuiApplication>

CalculationBase::CalculationBase(IFrontendConnector& frontendConnector)
    :frontendConnector_(frontendConnector), workPointsIt_(workPoints_.begin())
{
    Point tempPoint = {QPoint(0, 0), false, CALIB_COLOR, false, false, true};
    workPoints_.append(points_.insert("Calibration Point 1", tempPoint));
    workPoints_.append(points_.insert("Calibration Point 2", tempPoint));
}

qreal CalculationBase::calibrationLength_ = 40;

CalculationBase::~CalculationBase(){}

void CalculationBase::loadTable()
{
    qDebug() << "Loading table";
    for (auto &it : results_)
    {
        frontendConnector_.updateResult(it.key(), it->value, it->referenceValue);
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
        saveState({workPointsIt_->key(), workPointsIt_->value()});
        workPointsIt_->value().coordinates = QPointF(x,y);
        workPointsIt_->value().isReady = true;
        updatePoint(workPointsIt_->key());
        qDebug() << workPointsIt_->key() << workPointsIt_->value().coordinates;
        updateCalculation(workPointsIt_->key());
        recalculateIt();
    }
    else
    {
        qDebug() << "End of points";
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
        saveState({pointName, points_[pointName]});
        points_[pointName].isReady = false;
        updateCalculation(pointName);
        frontendConnector_.deletePoint(pointName);
    }
}

void CalculationBase::undo()
{
    if (undoHistory_.size())
    {
        PointState state = undoHistory_.takeLast();
        redoHistory_.append({state.pointName, points_[state.pointName]});
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
        undoHistory_.append({state.pointName, points_[state.pointName]});
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

void CalculationBase::movePoint(const QString& pointName, qreal x, qreal y)
{
    if (points_[pointName].isReady)
    {
        saveState({pointName, points_[pointName]});
        points_[pointName].coordinates = QPointF(x, y);
        updateCalculation(pointName);
        qDebug() << pointName << points_[pointName].coordinates;
    }
    else
    {
        qDebug() << "Point "+pointName+" is not ready!";
    }
}

void CalculationBase::saveData(const QString& path, const QString& name)
{
    QFile file(path+name+"_data.dat");
    if (!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        qDebug() << "File open error";
        return;
    }
    if (file.exists())
    {
        //TODO: Call file dialog
        qDebug() << "File rewrited";
        file.resize(0);
    }
    QTextStream out(&file);
    out.setCodec("Windows-1251");
    for (ResultsTable::Iterator it = resultsTable_.begin(); it != resultsTable_.end(); ++it)
    {
        out << it.key() << "	" << it->value << "	" << it->referenceValue << endl;
    }
    out.flush();
    qDebug() << "File saved";
}

const QVector<Points::Iterator>& CalculationBase::getPoints() const
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
        it.value() = *p;
        updatePoint(it.key());
        updateCalculation(it.key());
        ++p;
    }
    recalculateIt();
    qDebug() << "Points loaded";
}

void CalculationBase::checkCalibration(const QString& checkPoint)
{
    if (checkPoint == "Calibration Point 1" || checkPoint == "Calibration Point 2")
    {
        if (points_["Calibration Point 1"].isReady && points_["Calibration Point 2"].isReady)
        {
            frontendConnector_.connectPoints("Calibration Point 1", "Calibration Point 2", CALIB_COLOR);
            calibrationValue_ = calibrationLength_/geometry::calculateDistance(points_["Calibration Point 1"].coordinates, points_["Calibration Point 2"].coordinates);
        }
    }
}

void CalculationBase::updateResult(const QString& resultName)
{
    frontendConnector_.updateResult(resultName, resultsTable_[resultName].value, resultsTable_[resultName].referenceValue);
}

void CalculationBase::updatePoint(const QString &pointName)
{
    if (points_[pointName].isReady)
    {
        frontendConnector_.updatePoint(pointName, points_[pointName].color, points_[pointName].coordinates.x(), points_[pointName].coordinates.y(), points_[pointName].isTilted, points_[pointName].isEntilted, points_[pointName].isVisible);
    }
    else
    {
        frontendConnector_.deletePoint(pointName);
    }
}

qreal CalculationBase::getCalibrationValue()
{
    return calibrationValue_;
}

void CalculationBase::clearFrom(int number)
{
    if ((number>=0)&&(number < workPoints_.size()))
    {
        undoHistory_.clear(); //FIXME: temp solution
        redoHistory_.clear();
        for (auto it = workPoints_.begin()+number; it != workPoints_.end(); ++it)
        {
            if (it->value().isReady)
            {
                it->value().isReady = false;
                updateCalculation(it->key());
                frontendConnector_.deletePoint(it->key());
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
        if (!workPointsIt_->value().isReady)
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
        frontendConnector_.sendMsg(QCoreApplication::translate("main", "Now drawing point: ") + workPointsIt_->key());
    }
}
