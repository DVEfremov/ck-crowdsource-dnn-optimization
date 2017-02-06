#include "appmodels.h"

void ExperimentResult::reset()
{
    imagesCount = 0;
    totalTime = 0;
    timePerImage = 0;
    imagesPerSecond = 0;
    timePerBatch = 0;
    memoryPerImage = 0;
}

void ExperimentResult::accumulate(const ExperimentProbe* p)
{
    imagesCount++;
    totalTime += p->time;
    timePerImage = totalTime/imagesCount;
    imagesPerSecond = (timePerImage > 0)? 1/timePerImage: 0;
}

//-----------------------------------------------------------------------------

void ImagesDataset::buildCorrectnessMap()
{
    // TODO
}
