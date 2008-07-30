#ifndef SCRIBEAPI_H
#define SCRIBEAPI_H
#include <QtGui>
#include <QtCore>
#include <QDebug>
#include <QWidget>

#include "scribemime.h"
#include "SessionManager.h"
#include <QGraphicsView>
#include "_LayerExtended.h"



/*

file to display scene and view !!! 


*/



class GraphicsView : public QGraphicsView
{
     Q_OBJECT
//
public:
   GraphicsView( QWidget * parent  = 0 );
   ~GraphicsView();
   QRectF boundingRect();
   GraphicsScene *scene;
   QRectF rectToScene();
   TextLayer *BASE_TEXT;
   
signals:
   void SceneSwap();
public slots:
   void DisplayTop();
   void  ViewDisplay( const QRectF area );
private slots:

protected:
    void pageclear();
    QPixmap chessgrid;
    void PrintSetup( bool printok );
    void keyPressEvent(QKeyEvent *e );
    QSettings setter;
private:
    bool OnPrintRender;

};


class Panel : public QFrame
{
    Q_OBJECT
public:
    Panel( QWidget *parent = 0 );
    GraphicsView *view() const;
    GraphicsView *graphicsView;
public slots:

private slots:
    void resetView();
    void setResetButtonEnabled();
    void setupMatrix();
    void DisplayTop();
    void zoomIn();
    void zoomOut();
    void SceneChange();
    void toggleOpenGL();
private:
    uint tievents;
    QLabel *label;
    QToolButton *resetButton;
    QSlider *zoomSlider;
    QSlider *rotateSlider;
    QToolButton *printButton;
    QToolButton *openGlButton;
};











#endif // SCRIBEAPI_H

