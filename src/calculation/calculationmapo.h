#ifndef CALCULATIONMAPO_H
#define CALCULATIONMAPO_H

#include "calculation/calculationbase.h"

class CalculationMAPO: public CalculationBase
{
    public:
        explicit CalculationMAPO(IFrontendConnector& frontendConnector);
        ~CalculationMAPO() override;

        void updateCalculation(const QString& lastPointName) override;

    private:
        void checkAngle(const QString& checkPoint, const QString& dataName, const QString& pName1, const QString& pName2, const QString& pName3);
        void checkDistance(const QString& checkPoint, const QString& dataName, const QString& pName1, const QString& pName2);
        void checkRatio(const QString& checkPoint, const QString& dataName, const QString& pName1, const QString& pName2, const QString& pName3, const QString& pName4);
        void checkProjection(const QString& checkPoint, const QString& dataName, const QString& pName1, const QString& pName2, const QString& pName3, const QString& pName4);
        void checkCross(const QString& checkPoint, const QString& pointName, const QString& pName1, const QString& pName2, const QString& pName3, const QString& pName4);
        void checkPerpendicular(const QString& checkPoint, const QString& pointName, const QString& pName0, const QString& pName1, const QString& pName2);
        void checkBisect(const QString& checkPoint, const QString& pointName, const QString& pName1, const QString& pName2, const QString& pName3);
        void checkMiddlePoint(const QString& checkPoint, const QString& pointName, const QString& pName1, const QString& pName2);
        void checkAddPerpendicular(const QString& checkPoint, const QString& pointName, const QString& pName1, const QString& pName2);
};

#endif // CALCULATIONMAPO_H
