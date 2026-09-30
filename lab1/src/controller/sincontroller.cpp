#include "controller/sincontroller.h"

#include "model/sinmodel.h"
#include "view/inputdialog.h"

SinController::SinController(SinModel *model, QWidget *dialogParent, QObject *parent)
    : QObject(parent)
    , m_model(model)
    , m_dialogParent(dialogParent)
{
}

void SinController::onCalculateRequested()
{
    InputDialog dialog(m_dialogParent);

    // Восстановление последних введённых данных из модели.
    dialog.setInput(m_model->input());

    if (dialog.exec() != QDialog::Accepted)
        return;

    m_model->submit(dialog.input());
}
