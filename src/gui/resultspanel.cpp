#include "imageview.h"
#include "experimentcontext.h"
#include "resultspanel.h"
#include "utils.h"
#include "../ori/OriWidgets.h"

#include <QBoxLayout>
#include <QLabel>
#include <QVariant>

#define WORST_PREDICTED_IMAGE_W 160
#define WORST_PREDICTED_IMAGE_H 120

ResultsPanel::ResultsPanel(ExperimentContext *context, QWidget *parent) : QFrame(parent)
{
    setObjectName("resultsPanel");

    _context = context;
    connect(_context, &ExperimentContext::experimentStarted, this, &ResultsPanel::experimentStarted);
    connect(_context, &ExperimentContext::experimentResultReady, this, &ResultsPanel::experimentResultReady);

    _infoTimePerImage = makeInfoLabel();
    _infoImagesPerSec = makeInfoLabel("info-label-small");
    _infoMetricTop1 = makeInfoLabel();
    _infoMetricTop5 = makeInfoLabel();

    _worstPredictedImage = new ImageView(WORST_PREDICTED_IMAGE_W, WORST_PREDICTED_IMAGE_H);

    auto panelCounters = makePanel({ Utils::makeTitle("TIME PER\nIMAGE (FPS)"), _infoTimePerImage, _infoImagesPerSec });
    auto panelMetricTop1 = makePanel({ Utils::makeTitle("TOP-1"), _infoMetricTop1 });
    auto panelMetricTop5 = makePanel({ Utils::makeTitle("TOP-5"), _infoMetricTop5 });
    auto panelMetrics = Ori::Gui::layoutH(0, 0, { panelMetricTop1, panelMetricTop5 });
    auto panelWorstPrediction = makePanel({
        Utils::makeTitle("WORST PREDICTION"),
        Ori::Gui::layoutH(0, 0, { 0, _worstPredictedImage, 0}),
    });

    setLayout(Ori::Gui::layoutV(0, 0,
        { panelCounters, panelMetrics, panelWorstPrediction, 0 }));

    resetInfo();
}

QLabel* ResultsPanel::makeInfoLabel(const QString &role)
{
    auto label = new QLabel;
    label->setProperty("qss-role", role.isEmpty()? QString("info-label"): role);
    return label;
}

QFrame* ResultsPanel::makePanel(const std::initializer_list<QObject *> &items, const QString &objectName)
{
    auto panel = new QFrame;
    panel->setProperty("qss-role", "results-panel");
    panel->setObjectName(objectName);
    panel->setLayout(Ori::Gui::layoutV(0, 0, items));
    return panel;
}

void ResultsPanel::experimentStarted()
{
    resetInfo();
}

void ResultsPanel::experimentResultReady()
{
    auto r = _context->experimentResult();
    _infoTimePerImage->setText(QString::number(r.timePerImage, 'f', 2));
    _infoImagesPerSec->setText(QString(QStringLiteral("(%1)")).arg(r.imagesPerSecond, 0, 'f', 2));
    _infoMetricTop1->setText(QString::number(r.top1Metric, 'f', 2));
    _infoMetricTop5->setText(QString::number(r.top5Metric, 'f', 2));
    if (r.worstPredictionFlag)
    {
        _worstPredictedImage->loadImage(r.worstPredictedImage);
        _worstPredictedImage->setToolTip(QString(QStringLiteral("%1\nTop1: %2\nCorrect: %3"))
                                         .arg(r.worstPredictedImage)
                                         .arg(r.worstPredictionTop1.str())
                                         .arg(r.worstPredictionCorrect.str()));
    }
}

void ResultsPanel::resetInfo()
{
    _infoTimePerImage->setText("N/A");
    _infoImagesPerSec->clear();
    _infoMetricTop1->setText("N/A");
    _infoMetricTop5->setText("N/A");
}