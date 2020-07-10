#ifndef CALCULATIONBASE_H
#define CALCULATIONBASE_H

#include <QHash>
#include <QVector>
#include <QPointF>
#include <QDebug>

#define CALIB_COLOR "#00FF00"
#define REGULAR_COLOR "#FF0000"
#define LINES_COLOR "#0000FF"

struct Point
{
    QPointF coordinates;
    bool isReady;
    bool isTilted;
    bool isEntilted;
    bool isVisible;
    QString color;
};

struct PointState
{
    Point point;
    QString pointName;
};

struct Result
{
    QString value;
    QString referenceValue;
    const char* description;
};

typedef QVector<PointState> PointsHistory;
typedef QHash<QString, Point> Points;
typedef QHash<QString, Result> ResultsTable;

struct WorkPoint
{
    Points::Iterator point;
    const char* description;
};

typedef QVector<WorkPoint> WorkPoints;


//TODO: Signals refactoring
class IFrontendConnector
{
    public:
        virtual ~IFrontendConnector() = default;
        virtual void addPoint(const QString& pointName, bool status, const QString& description) = 0;
        virtual void updatePoint(const QString& pointName, const QString& color, qreal x, qreal y, bool isTilted, bool isEntilted, bool isVisible, bool status) = 0;
        virtual void connectPoints(const QString& pointName1, const QString& pointName2, const QString& color) = 0;
        virtual void addResult(const QString& resultName, const QString& resultReference, const QString& description) = 0;
        virtual void updateResult(const QString& resultName, const QString& resultValue, const QString& resultReference) = 0;
        virtual void sendMsg(const QString& msg) = 0;
        virtual void changeUndoState(bool isEnabled) = 0;
        virtual void changeRedoState(bool isEnabled) = 0;
};

//TODO: separate init method and setResultsTable method
class CalculationBase
{
    public:
        //TODO: Setting point colour
        explicit CalculationBase(IFrontendConnector& frontendConnector);
        virtual ~CalculationBase();
        virtual void updateCalculation(const QString& lastPointName) = 0;
        void reset();
        void loadResultsTable();
        void loadPointsTable();
//        void loadLastCoordinates();
        void writeCoordinates(qreal x, qreal y);
        void writeCoordinates(const QString& pointName, qreal x, qreal y);
        void removePoint(const QString& pointName);
        void undo();
        void redo();
        void clear();
        void clearAll();
        void saveData(const QString& path, const QString& name);
        const WorkPoints& getPoints() const;
        void loadPoints(const QVector<Point>& points);

        static void setCalibrationLength(qreal length);
        static qreal getCalibrationLength();

    protected:
        void checkPointsConnection(const QString& checkPoint, const QString& pName1, const QString& pName2);
        void checkCalibration(const QString& checkPoint);
        virtual void checkAngle(const QString& checkPoint, const QString& dataName, const QString& pName1, const QString& pName2, const QString& pName3);
        virtual void checkDistance(const QString& checkPoint, const QString& dataName, const QString& pName1, const QString& pName2);
        virtual void checkRatio(const QString& checkPoint, const QString& dataName, const QString& pName1, const QString& pName2, const QString& pName3, const QString& pName4);
        virtual void checkProjection(const QString& checkPoint, const QString& dataName, const QString& pName1, const QString& pName2, const QString& pName3, const QString& pName4);
        virtual void checkCross(const QString& checkPoint, const QString& pointName, const QString& pName1, const QString& pName2, const QString& pName3, const QString& pName4);
        virtual void checkPerpendicular(const QString& checkPoint, const QString& pointName, const QString& pName0, const QString& pName1, const QString& pName2);
        void updateResult(const QString& resultName);
        void updatePoint(const QString& pointName);
        qreal getCalibrationValue();

        Points points_;
        WorkPoints workPoints_; //For points ordering
        ResultsTable resultsTable_; //TODO: Results description
        QVector<ResultsTable::Iterator> results_; //For results ordering

    private:
        void clearFrom(int number);
        void setState(const PointState& state);
        void saveState(const PointState& state);
        void recalculateIt();

        IFrontendConnector& frontendConnector_;
        qreal calibrationValue_ = 1; //mm/px
        WorkPoints::Iterator workPointsIt_;
        PointsHistory undoHistory_;
        PointsHistory redoHistory_;

        static qreal calibrationLength_; //FIXME: Solution without static member
};

#endif // CALCULATIONBASE_H
