#ifndef EXPERIMENTPANEL_H
#define EXPERIMENTPANEL_H

#include "../core/appmodels.h"
#include <QFrame>
#include <QProcess>

class ExperimentContext;
class FeaturesPanel;
class ImageResult;

QT_BEGIN_NAMESPACE
class QPushButton;
QT_END_NAMESPACE

class ExperimentPanel : public QFrame
{
    Q_OBJECT

public:

    explicit ExperimentPanel(ExperimentContext* context, QWidget *parent = 0);

    void updateExperimentConditions();

private slots:
    void startExperiment();
    void stopExperiment();
    void experimentStarted();
    void experimentFinished();
    void publishResults();
    void publishResultsFinished(int, QProcess::ExitStatus);
    void publishResultsError(QProcess::ProcessError);

private:
    ExperimentContext* _context;
    FeaturesPanel* _featuresPanel;
    QPushButton *_buttonStart, *_buttonStop;
    QPushButton *_buttonPublish;
    QPushButton *_buttonZoomIn;
    QPushButton *_buttonZoomOut;
    QPushButton *_buttonZoomActual;

    Program _program;
    Model _model;
    Dataset _dataset;
    qint64 _experiment_start_time;

    QProcess* _publisher;

    void enableControls(bool on);
    void adjustSidebar(QWidget* w);
};

#endif // EXPERIMENTPANEL_H
