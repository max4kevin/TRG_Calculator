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
    QString description;
};

struct PointState
{
    QString pointName;
    Point point;
};

struct Result
{
    QString value;
    QString referenceValue;
};

typedef QVector<PointState> PointsHistory;
typedef QHash<QString, Point> Points;
typedef QHash<QString, Result> ResultsTable;

class IFrontendConnector
{
    public:
        virtual ~IFrontendConnector() = default;
        virtual void updatePoint(const QString& pointName, const QString& color, qreal x, qreal y, bool isTilted, bool isEntilted, bool isVisible, const QString& description, bool status) = 0;
        virtual void connectPoints(const QString& pointName1, const QString& pointName2, const QString& color) = 0;
        virtual void updateResult(const QString& resultName, const QString& resultValue, const QString& resultReference) = 0;
        virtual void sendMsg(const QString& msg) = 0;
        virtual void changeUndoState(bool isEnabled) = 0;
        virtual void changeRedoState(bool isEnabled) = 0;
};

//TODO: Manual removing points
//TODO: separate init method and setResultsTable method
class CalculationBase
{
    public:
        //TODO: Setting point colour
        explicit CalculationBase(IFrontendConnector& frontendConnector);
        virtual ~CalculationBase();
        virtual void updateCalculation(const QString& lastPointName) = 0;
        void reset();
        void loadTable();
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
        const QVector<Points::Iterator>& getPoints() const;
        void loadPoints(const QVector<Point>& points);

        static void setCalibrationLength(qreal length);
        static qreal getCalibrationLength();

    protected:
        void checkPointsConnection(const QString& checkPoint, const QString& pName1, const QString& pName2);
        void checkCalibration(const QString& checkPoint);
        void updateResult(const QString& resultName);
        void updatePoint(const QString& pointName);
        qreal getCalibrationValue();

        Points points_;
        QVector<Points::Iterator> workPoints_; //For points ordering
        ResultsTable resultsTable_;
        QVector<ResultsTable::Iterator> results_; //For results ordering

    private:
        void clearFrom(int number);
        void setState(const PointState& state);
        void saveState(const PointState& state);
        void recalculateIt();

        IFrontendConnector& frontendConnector_;
        qreal calibrationValue_ = 1; //mm/px
        QVector<Points::Iterator>::Iterator workPointsIt_;
        PointsHistory undoHistory_;
        PointsHistory redoHistory_;

        static qreal calibrationLength_; //FIXME: Solution without static member
};

#endif // CALCULATIONBASE_H
