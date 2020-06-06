#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QMap>
#include <QQuickImageProvider>
#include <QTranslator>
#include <QGuiApplication>
#include "calculation/calculationmapo.h"


//Fasade
class BackEnd : public QObject, public QQuickImageProvider, public IFrontendConnector
{
    Q_OBJECT

    public:
        explicit BackEnd(QGuiApplication& app);

        QImage requestImage(const QString &/*id*/, QSize *size, const QSize &/*requestedSize*/) override;

        Q_INVOKABLE
        void reset();

//        Q_INVOKABLE
//        void loadTable();

        Q_INVOKABLE
        void writeCoordinates(qreal x, qreal y);

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
        void movePoint(const QString& pointName, qreal x, qreal y);

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

        void updatePoint(const QString& pointName, const QString& color, qreal x, qreal y, bool isTilted, bool isEntilted, bool isVisible) override;
        void deletePoint(const QString& pointName) override;
        void connectPoints(const QString& pointName1, const QString& pointName2, const QString& color) override;
        void updateResult(const QString& resultName, const QString& resultValue, const QString& resultReference) override;
        void sendMsg(const QString& msg) override;
        void changeUndoState(bool isEnabled) override;
        void changeRedoState(bool isEnabled) override;

    signals:
        void pointUpdated(const QString& pointName, const QString& color, qreal x, qreal y, bool isTilted, bool isEntilted, bool isVisible);
        void pointDeleted(const QString& pointName);
        void pointsConnected(const QString& pointName1, const QString& pointName2, const QString& color);
        void resultUpdated(const QString& resultName, const QString& resultValue, const QString& resultReference);
        void newMsg(const QString& msg);
        void fileLoaded();
        void imageUpdated();
        void clearTable();
        void undoStateChanged(bool isEnabled);
        void redoStateChanged(bool isEnabled);

    private:
        void loadTable();

        QGuiApplication* const app_;
        QTranslator qtTranslator_;
        CalculationMAPO  calculationMethodMAPO_;
        CalculationBase* actualCalculationMethod_;
        QImage image_;
        QString filePath_;
        QString fileName_;
        QString fileType_;
};

#endif // BACKEND_H
