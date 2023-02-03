#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QMap>
#include <QQuickImageProvider>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include <QSettings>
#include "calculation/calculationmapo.h"
#include "calculation/calculationjra.h"

//TODO: Saving all methods in one file if there some points?

//Fasade
class BackEnd : public QObject, public QQuickImageProvider, public IFrontendConnector
{
    Q_OBJECT

    public:
        explicit BackEnd(QQmlApplicationEngine& engine);

//        ~BackEnd(){};

        QImage requestImage(const QString &/*id*/, QSize *size, const QSize &/*requestedSize*/) override;

        Q_INVOKABLE
        void reset();

        Q_INVOKABLE
        void writeCoordinates(qreal x, qreal y);

        Q_INVOKABLE
        void writeCoordinates(const QString& pointName, qreal x, qreal y);

        Q_INVOKABLE
        void removePoint(const QString& pointName);

        Q_INVOKABLE
        void setCalibrationLength(qreal length);

        Q_INVOKABLE
        qreal getCalibrationLength();

        Q_INVOKABLE
        void undo();

        Q_INVOKABLE
        void redo();

        Q_INVOKABLE
        void clear();

        Q_INVOKABLE
        void clearAll();

        Q_INVOKABLE
        void saveData(const QString& path, const QString& name);

        Q_INVOKABLE
        void openFile(const QString& filePath);

        Q_INVOKABLE
        void saveFile(const QString& filePath);

        Q_INVOKABLE
        void saveFile();

        Q_INVOKABLE
        QString getFilePath();

        Q_INVOKABLE
        QString getFileName();

        Q_INVOKABLE
        QString getFileType();

        Q_INVOKABLE
        void invertImage();

        Q_INVOKABLE
        void setLanguage(const QString& lang);

        Q_INVOKABLE
        void setMethod(const QString& method);

        Q_INVOKABLE
        void setConfig(const QString& key, const QString& value);

        Q_INVOKABLE
        QString getConfig(const QString& key);

        Q_INVOKABLE
        bool isDirectoryValid(const QString& directory);

        void addPoint(const QString& pointName, bool status, const QString& description) override;
        void updatePoint(const QString& pointName, const QString& color, qreal x, qreal y, bool isTilted, bool isEntilted, bool isVisible, bool status) override;
        void connectPoints(const QString& pointName1, const QString& pointName2, const QString& color) override;
        void addResult(const QString& resultName, const QString& resultReference, const QString& description) override;
        void updateResult(const QString& resultName, const QString& resultValue, const QString& resultReference) override;
        void sendMsg(const QString& msg) override;
        void changeUndoState(bool isEnabled) override;
        void changeRedoState(bool isEnabled) override;

    signals:
        void pointAdded(const QString& pointName, bool status, const QString& description);
        void pointUpdated(const QString& pointName, const QString& color, qreal x, qreal y, bool isTilted, bool isEntilted, bool isVisible, bool status);
        void pointsConnected(const QString& pointName1, const QString& pointName2, const QString& color);
        void resultAdded(const QString& resultName, const QString& resultReference, const QString& description);
        void resultUpdated(const QString& resultName, const QString& resultValue, const QString& resultReference);
        void newMsg(const QString& msg);
        void methodChanged(const QString& method);
        void fileLoaded();
        void imageUpdated();
        void clearTables();
        void undoStateChanged(bool isEnabled);
        void redoStateChanged(bool isEnabled);
        void error(const QString& errorMsg);

    private:
        void loadData();

        QQmlApplicationEngine* const engine_;
        QTranslator qtTranslator_;
        CalculationMAPO  calculationMethodMAPO_;
        CalculationJRA  calculationMethodJRA_;
        CalculationBase* actualCalculationMethod_;
        QImage image_;
        QString filePath_;
        QString fileName_;
        QString fileType_;
        QSettings settings_;
};

#endif // BACKEND_H
