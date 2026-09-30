#include <QApplication>

#include "controller/sincontroller.h"
#include "model/sinmodel.h"
#include "view/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SinModel model;
    MainWindow window(&model);
    SinController controller(&model, &window);

    // Единственная связь View -> Controller: намерение пользователя.
    QObject::connect(&window, &MainWindow::calculateRequested,
                     &controller, &SinController::onCalculateRequested);

    window.show();
    return app.exec();
}
