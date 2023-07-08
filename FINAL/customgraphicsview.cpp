#include "customgraphicsview.h"
#include <QRandomGenerator>

CustomGraphicsView::CustomGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    // Set up the QGraphicsScene
    scene = new QGraphicsScene(this);
    setScene(scene);

    // Set the scene bounds
    scene->setSceneRect(-200, -200, 400, 400);

    // Seed the random number generator
    QRandomGenerator::global()->seed(QTime::currentTime().msec());

    // Create the wheel segments
    setupWheel();
}

void CustomGraphicsView::setupWheel()
{
    // Create the wheel segments
    int numSegments = 8;
    qreal angle = 0;
    qreal angleIncrement = 360.0 / numSegments;
    for (int i = 0; i < numSegments; ++i) {
        // Create a random color for each segment
        QColor color(QRandomGenerator::global()->bounded(256),
                     QRandomGenerator::global()->bounded(256),
                     QRandomGenerator::global()->bounded(256));

        // Create a QGraphicsEllipseItem representing the segment
        QGraphicsEllipseItem *segment = new QGraphicsEllipseItem(-100, -100, 200, 200);
        segment->setStartAngle(angle * 16);
        segment->setSpanAngle(angleIncrement * 16);
        segment->setBrush(color);
        scene->addItem(segment);

        // Increment the angle for the next segment
        angle += angleIncrement;
    }
}
