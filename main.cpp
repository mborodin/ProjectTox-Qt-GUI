#include <QtGui/QApplication>
#include "QTox.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QTox foo;
    foo.show();
    return app.exec();
}
