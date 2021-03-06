/*******************************************************************************
 * class GraphicsView
 *******************************************************************************
 * Copyright (C) 2007 by Peter Hohl
 * e-Mail: ppkciz@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *******************************************************************************/

#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QGraphicsView>
#include "GraphicsItemEdit.h"
#include "mounttextprivate.h"
#include "GraphicsScene.h"
#include <math.h>
#include <QPair>
#ifndef QT_NO_OPENGL
#include <QGLWidget>
#endif


#define QGRAPHICSVIEW_DEBUG


/* work on layer ID edit enable */
class LayerTestEvent : public QEvent
{
	//QEvent::User==1000
    
public:
    LayerTestEvent(int s, bool d)
	: QEvent( QEvent::User ),lid(s),dn(d)
    {  }
 int name() const { return lid; }
 bool done() const { return dn; }

private:
    int lid;
    bool dn;

};

/* qApp->postEvent(this, new LayerEvent(int,bool)); */



class GraphicsView : public QGraphicsView
{
     Q_OBJECT
//
public:
  GraphicsView( QWidget * parent  = 0 );
  ~GraphicsView();
  QRectF rectToScene();
  inline TextLayer *LayerModel() { return CurrentActive; }
  inline int LayerID() { return layerNr; } 
  QRectF boundingRect();
  qreal NextfromY();
  void insert( RichDoc e , bool cloned = false );  /* one layer insert */
  QMap<int,RichDoc> read();  /* read full pages */
  PageDoc getPage();
  void OpenPage( PageDoc e );
  TextLayer *CurrentActive;
  void NewLayer( const int type );
  GraphicsScene *scene;
signals:
   void MenuActivates(bool,QPair<int,int>);
   void GrepLayer(int);
public slots:
    void AppendDemo();
    void WorksOn(QGraphicsItem * item , qreal zindex );
    void notselect();
    void DisplayTop();
    void sceneScaleChanged(const QString &scale);
    void GoEditCurrentLayer();
    void NewLayer();
    void updateauto();
    void CloneCurrent();
    void removelayer( const int idx );
    void PasteLayer();
    void pageclear();
    void setGlobalBrush( QPixmap e );
    void PrintDoc();
    void onOtherInstanceMessage( const QString msg );
    void LaunchFile( const QString islayeror );
    void toggleOpenGL();
    void SaveAsPage();
    void OpenFilePageGroup();
    void CursorMargins( qreal left ,qreal right );

protected:
    QAction *openGlaction;
    int InitTopPositionAfterBorderPlay;
    GMarginScene *MarginController;
    QMap<int,TextLayer*> auto_li;
    qreal AreaHiTower;
    qreal SpaceAutoFloatHight;
    void RecordItem();
    QSettings setter;
    void contextMenuEvent ( QContextMenuEvent * e );
    void resizeEvent(QResizeEvent *event);
    void wheelEvent (QWheelEvent * event);
    ///////void drawForeground ( QPainter * painter, const QRectF & rect );
    void drawBackground( QPainter * painter, const QRectF & rect );
    void scaleView (qreal scaleFactor);
    void PrintSetup( bool enable );
    void closeEvent(QCloseEvent *event);
    int Lmm;
    int Rmm;
  int layerNr;
  qreal width;
  qreal height;
  uint layercount;
  QWidget *BigParent;
  QPixmap chessgrid;
  QRectF viewportLayer;   ////// viewportLayer.isNull()
private:
  void fillNullItem();



};








#endif // GRAPHICSVIEW_H






















//


