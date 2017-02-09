#include "viewhelpers.h"
#include <time.h>

namespace ViewHelpers {
    QPoint calculateCenter(QPoint point, const QPoint &oldCentre, double factor)
    {
        int diffX = oldCentre.x() - point.x();
        int diffY = oldCentre.y() - point.y();
        point.rx() += diffX * factor;
        point.ry() += diffY * factor;
        return point;
    }

    pstring getCurrentDate()
    {
        time_t now = time(NULL);
        char timeString[11];
        strftime(timeString, 11, "%Y/%m/%d", localtime(&now));
        return pstring(timeString);
    }

}
