#ifndef TXTAPI_H
#define TXTAPI_H

#include "scribemime.h"
#include "SessionManager.h"

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsScene>

class ScribePage : public QObject
{
     Q_OBJECT

public:
 
/* keyboard swap on depending format */

typedef enum
{  
  XHTML = 100,   /* normal html tag */
  FOP = 200,    /* XSL-FO tag <br/> tag find hack */
  OOO = 300    /* import from OpenOffice */
} FileHandlerType;


/* keyboard swap on depending format */

  explicit ScribePage( M_PageSize e );
  void setDocument ( const QTextDocument * document , FileHandlerType Type = FOP );
  QTextDocument *document() const;
  QTextCursor textCursor();

  void insertPixmap( QPixmap p );  /* on cursor insert */
  void RegisterImage( SPics e , bool insert );
  void ImageonCursor( const QString file );
  QString ImageFilterHaving() const;

  QLineF ViewBlinkedCursorLine();
  void InsertMimeDataOnCursor( const QMimeData *md );
  bool AllowedPosition( const QPointF inpos );

  /* event on incomming */
  void Controller_keyPressEvent ( QKeyEvent * e );
  void Controller_keyReleaseEvent ( QKeyEvent * e );
  bool procesevent( QEvent *e );

  QRectF CurrentBlockRect();  /* on view !not on document !!!!!! */
  inline M_PageSize  Model() const { return PAGE_MODEL; }
  void paint(QPainter * painter , const QStyleOptionGraphicsItem *option , QWidget *widget   );
  QRectF boundingRect();  /* all page and spacer to x y 00 */
  QRectF GroupboundingRect();  /* + shadow space */
  void SwapPageModel( M_PageSize e );
  inline bool Edit_On() { return edit_enable; }  /* same as cursor blink true / false */
  void setBlinkingCursorEnabled( bool enable );
protected:
  M_PageSize PAGE_MODEL;
  int CurrentSessionID;
  bool WorkREvent;
  QClipboard *clipboard;
  bool overwriteMode;  /* overwrite on write by cursor  */
  bool edit_enable;
  bool cursorOn;
  bool cursortime;
  uint timeline;
  uint eventline;
  bool cursorIsFocusIndicator;
  bool DragFill;

  QPointF PointPositionOnDoc;

  int OnPageClick;
  uint SceneTimerLine;
  QPointF LastReleasePoint;

  /* function */
  QPointF PageIndexTopLeft( const int index  );
  void DrawPage( const int index  , QPainter * painter , const int cursorpage );
  QLineF BlinkCursorLine(); /* on doc coordinate */
  QTextLine currentTextLine(const QTextCursor &cursor);
  void CursorMovetoPosition( const QPointF &pos );   /* inside document not view !!!! */
  qreal SlicedPage( const int page );
  void FrameHandler();
  QTextTableCell CellOnPosition( const int posi );
  QMimeData *createMimeDataFromSelection();
  bool StartDragOperation();

  QPointF traposin( const QPointF &pos );

  /* internal event */
  bool cursorMoveKeyEvent(QKeyEvent *e);
  void repaintCursor( bool allrect = false );
  void BaseMousePressEvent( const  QPointF posi , const QGraphicsSceneMouseEvent *epress );
  void BaseMoveEvent( const int cursorpos ,  QPointF moveposition );
  void BaseMouseReleaseEvent( const  QPointF posi , Qt::MouseButton button ); 
  void BaseDoubleClickEvent( const  QPointF position , const QGraphicsSceneMouseEvent * event ); 



private:
  /* cursor selection */
  int position_selection_start;
  int cursor_position;
  int Current_Page_Nr;
  QLineF CursorDrawLine;
  QTextFrameFormat root_format;
  qreal LeftBorderPadding;
  qreal BaseTextCursorBottom;
  QLineF BlinkedcursorDraw;
  QWidget *Gwi;  /* event widged from  !!!!!!!!!!!!!!!!!!!!!! */
  bool isBottomBlock;   /* if cursor at last block from page ???  */
  bool isTopBlock;   /* if cursor at first block from page ???  */
  bool FullDocSelect;
  /* core data */
  //////////QAbstractTextDocumentLayout *layout;
  QTextDocument *_d;
  QTextCursor C_cursor;
  QBasicTimer cursorTimeLine;  /* blink cursor time line */
  QBasicTimer trippleClickTimer;
  QBasicTimer dragClickTimer;
  FileHandlerType Op;
  
  QRect OverloadRectUpdate;
  QSettings setter;

  /* core data */
  /* document params */
  qreal Page_Width;
  qreal Page_Height;
  QRectF Page_Edit_Rect;
  int PageTotal;
  /* draw item */
  QPicture LayoutDraw;
  QTextCharFormat LastCharFormat;
  /* events */
  ////////////QPointF trapos( const QPointF &pos );   /* translate position from space */
  void timerEvent(QTimerEvent *event);

signals:
  void q_cursor_newPos();  /* +++  swap reformat qmenu from session */
  void q_visible(QRectF);
  void q_update(QRect);
  void q_startDrag(QPointF);
  void q_update_scene();  /* page changes ++ or -- */
  
private slots:
  ////////void SessionUserInput( int a = -1 , int b = -1 , int newchar = -1 );
  void int_clipboard_new();
  void cursorPosition( const QTextCursor curs );
  ///////void repaintCursor( bool allrect = false );
  //////////void WakeUpElasticSize( bool e = false );
  void EnsureVisibleCursor();
  void LayoutRepaint( const QRectF docrect );
  /* no qreal rect items */
  void redir_update( QRectF area );
  void in_image( int id );  /* remote image incomming */

public slots:
  void deleteSelected();
  void cut();
  void paste();
  void copy();
  void undo();
  void showhtml();
  void redo();
  void selectAll();
  void InsertImageonCursor();




};











//
#endif // TXTAPI_H
