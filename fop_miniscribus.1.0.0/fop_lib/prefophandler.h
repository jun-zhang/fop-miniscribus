#ifndef PREFOPHANDLER_H
#define PREFOPHANDLER_H
#include <iostream>
#include <stdio.h>
//////#include <unistd.h>
#include <iostream>
#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QApplication>
#include <QSvgRenderer>
#include <QWidget>

#include "ziphandle.h"
#include "network_get.h"
#include <QProgressDialog>
#include <QAbstractTextDocumentLayout>
#include "BasicConfig.h"
#include "network_get.h"


/* find gpl GhostScript path or exe */
extern inline QString getGSDefaultExeName()
{
	QString gsName;
    QString gVersion;
#if defined Q_WS_WIN
    QFileInfo Pinfo;
	// Try to locate GhostScript thanks to the qsetting
	 gsName = "gswin32c.exe";
     QSettings softs("HKEY_LOCAL_MACHINE\\Software",QSettings::NativeFormat);
     QStringList allsoftware = softs.childGroups();
     QStringList gsonly = allsoftware.filter(QRegExp("Ghostscript"));
     //////////qDebug() << "### gsonly " <<  gsonly;
         for (int i = 0; i < gsonly.size(); ++i) {
              const QString RealName = gsonly.at(i);  /* realpath */
                      if (RealName.contains("Ghostscript")) {
                       //////////qDebug() << "### soft " <<  RealName;
                       for(int e=1;e<99;e++)  {
                           /* check version 8 ++ 99 down */
                           gVersion = QString("8.%1").arg(100 - e);
                           if (softs.value(RealName+"/"+gVersion+"/GS_DLL").toString().size() > 6 ) {
                               Pinfo.setFile(softs.value(RealName+"/"+gVersion+"/GS_DLL").toString());
                               return gsName.prepend(Pinfo.absolutePath()+"/");
                           }
                           /* check version 7 ++ 99 down */
                           gVersion = QString("7.%1").arg(100 - e);
                           if (softs.value(RealName+"/"+gVersion+"/GS_DLL").toString().size() > 6 ) {
                               Pinfo.setFile(softs.value(RealName+"/"+gVersion+"/GS_DLL").toString());
                               return gsName.prepend(Pinfo.absolutePath()+"/");
                           }
                       }
               }
         }
    
    /* win not having GPL Ghostscript ! */
    gsName = "";  
    return gsName;         
#endif
#if defined Q_WS_MAC
     QStringList paths;
     paths << "/usr/local/bin/gs" << "/usr/bin/gs" << "/bin/gs" << "/bin/gs" << "/sw/bin/gs" << "/opt/bin/gs";
         QFileInfo gsp; 
          for (int i = 0; i < paths.size(); ++i)  {
                  gsp.setFile (paths.at(i));
                  if (gsp.exists()) {
                   return paths.at(i);
                  }                      
          }
	gsName = "gs";
    return gsName;
#endif
#if defined Q_WS_X11
    gsName = "gs";
	return gsName;
#endif
         /* forum http://www.qtcentre.org/forum/f-qt-programming-2/t-qsettings-read-only-avaiable-10254.html */
}

extern inline QString PathConvert( QString path ) 
{
    QString resolvepath;
    #if defined Q_WS_WIN
    //////resolvepath = QDir::convertSeparators(path);
    resolvepath = path.replace('/','\\');
    #endif
    resolvepath = path;
    
    qDebug() << "### resolvepath " << resolvepath; 
    return resolvepath;
}

/* find gpl GhostScript version  */
extern inline QString getGSVersion()
{
        QProcess process;
        process.setReadChannelMode(QProcess::MergedChannels);
        process.start( getGSDefaultExeName() ,  QStringList() << "-v");
          if (!process.waitForFinished()) {
          return QString();
          } else {
           return process.readAll().replace("\n","");
          }
}

extern inline int callGS( const QStringList & args )
{
    QProcess *proc = new QProcess(NULL);
    QStringList gspaths;
    QFileInfo gspath( getGSDefaultExeName() );
    gspaths.append(PathConvert(gspath.absolutePath() ));
    gspaths.append(PathConvert(gspath.absolutePath().replace("bin","lib") ));
    QStringList env = proc->systemEnvironment();
    env.replaceInStrings(QRegExp("^PATH=(.*)", Qt::CaseInsensitive), "PATH=\\1;"+gspaths.join(";"));
    proc->setEnvironment(env);
    
    QStringList ce = proc->environment();
    
    qDebug() << "### args " << args;
    
    proc->start(getGSDefaultExeName(),args);
	if (!proc->waitForFinished(30000)) {
        return -1;
    }   else {
        QString dd = proc->readAll();
        qDebug() << "### QProcess-> " << dd;   
    }
     int ex = proc->exitCode();
	 delete proc;
	 return ex;
}



extern inline QPixmap LoadPDF(QString fn, int Page, int w )
{
	QString tmp, cmd1, cmd2;
	const QString pdfFile = PathConvert(fn);
	const QString tmpFile = PathConvert(QDir::homePath()+"/sctodaytmps.png");
    const QString qttmpFile = QDir::homePath()+"/sctodaytmps.png";
	QPixmap pm;
    tmp.setNum(Page);
	int ret = -1;
	tmp.setNum(Page);
	QStringList args;
    args.append("-sDEVICE=png16m");
    args.append("-r72");
    args.append("-dGraphicsAlphaBits=4");
    args.append("-o");
	args.append(tmpFile);
    args.append("-dFirstPage="+tmp);
	args.append("-dLastPage="+tmp);
	args.append(pdfFile);
	ret = callGS(args);
    qDebug() << "### ret " << ret;
	if (ret == 0)
	{
		QPixmap tmpimage(qttmpFile);
        QPixmap penna = tmpimage.scaledToWidth(w);
        tmpimage.detach(); 
        QFile lastaction(qttmpFile);
        lastaction.remove();
        QPainter p;
		p.begin(&penna);
		p.setBrush(Qt::NoBrush);
		p.setPen(QPen(QBrush(Qt::black),2,Qt::SolidLine));
		p.drawRect(0, 0, penna.width(), penna.height());
		p.end();
        return penna;
	}
	return pm;
}






extern inline QPixmap LoadPS( QString fn )
{
	const QString pdfFile = PathConvert(fn);
	const QString tmpFile = PathConvert(QDir::homePath()+"/sctodaytmps.png");
    const QString qttmpFile = QDir::homePath()+"/sctodaytmps.png";
	QPixmap pm;
	int ret = -1;
	QStringList args;
    args.append("-sDEVICE=png16m");
    args.append("-r72");
    args.append("-dGraphicsAlphaBits=4");
    args.append("-o");
	args.append(tmpFile);
	args.append(pdfFile);
	ret = callGS(args);
    ////////qDebug() << "### ret " << ret;
	if (ret == 0)
	{
		  QPixmap tmpimage(qttmpFile);
          QFile lastaction(qttmpFile);
          lastaction.remove();
        
        return tmpimage;
	}
	return pm;
}







extern inline QByteArray  OpenGzipOneFileByte( QString fileName )
{
    QByteArray input;
     gzFile file;
      file = gzopen (fileName.toUtf8().data(), "rb");
      if(!file) {
        QMessageBox::critical(0, "Error",QString("Can't open file %1").arg(fileName));
        return input;
      }
      
      char buffer[1024];
      /////////QByteArray inputData;
      while(int readBytes =  gzread (file, buffer, 1024))
       {
        input.append(QByteArray(buffer, readBytes));
      }
      gzclose(file);
      return input;
}


#define _PARSE_DEBUG_FOP_ 1

typedef  QMap<int, QString> TypErnoMap;
typedef  QMap<QString,QFont> TypnotexistFontMap;  /* replace to other */
typedef  QMap<QString,QVariant> TypImageMap;   /* store image in if exist imagefilename|type , 
                                                 QByteArray or QString svg original before transform 
                                                to render on png */
#include <QObject>

class PageDimensionName : public QObject
{
    Q_OBJECT
    
public:
    PageDimensionName( const QString MasterName , const qreal widht , const qreal height , const QRectF margin , QColor bgcolor , int printerid ) 
    {
       pagemargin = margin; 
       na = MasterName;
       wi = widht;
       hi = height;
       bg = bgcolor;
       landscape = false;   /* default portrait */
       qtprinter = qtprinter;
       humandisplay = QString("%1mm x %2mm").arg(Pointo(widht,"mm")).arg(Pointo(height,"mm"));
    }
inline void display( const QString dd ) { humandisplay = dd; }
inline QString display() { return humandisplay; }

inline void SwapPage( bool e ) { landscape = e; }
inline int printer() { return qtprinter; }

inline qreal height() { 
    if (landscape) {
    return wi;
    } else {
    return hi; 
    }
}
inline qreal widht() { 
    if (landscape) {
    return hi;
    } else {
    return wi;
    }
}
inline QRectF drawpage() { return QRectF(0,0,widht(),height()); }
inline QRectF margin() { return pagemargin; }
inline void  setmargin( QRectF r ) { pagemargin=r; }
inline QString name() { return na; }
inline QColor bgcolor() { return bg; }

/* QRectF (xTopMargin,xRightMargin,xBottomMargin,xLeftMargin), */
/* QRectF ( qreal x, qreal y, qreal width, qreal height )  */

inline qreal internalwi() {
    /* leave margin wi */
    qreal maxwidht = wi - (pagemargin.y() + pagemargin.height());
    return maxwidht;
}
inline qreal internalhi() {
    /* leave margin wi */
    qreal maxhight = wi - (pagemargin.x() + pagemargin.width());
    return maxhight;
}

inline QRectF PrinterRec() {
    QRectF pageprint(0,0,wi,hi);
    return pageprint;
}
QString humandisplay;
QRectF pagemargin;
QString na;
QColor bg;
qreal wi;
qreal hi;
bool landscape;
int qtprinter;
};






class PageDB : public QObject
{
    Q_OBJECT
    
public:
    PageDB( QObject * parent );
    ~PageDB();
  void AppInsert( PageDimensionName *page );
  inline QList<PageDimensionName*>  Listing() { return Pages; }
  inline bool connect() { 
      return Pages.size();
  }
  inline PageDimensionName *last() { return Pages.last(); }
  inline void reload() { emit newdata(); }
  PageDimensionName *page( int index );
private:
QList<PageDimensionName*> Pages;
QObject *friendpanel;
signals:
  void newdata();
public slots:
};







#include <QTextBlockUserData>

class Fop_Block : public QTextBlockUserData
{
public:
    Fop_Block( int nr , QDomElement e , const QString tagname = "fo:block" );
    ~Fop_Block();
    inline int Get_id() { return id; }
    inline QString Get_XML() { return streamxml; }
    inline QDomElement Get_Dom() const { return element; }
int id;
QDomElement element;
QString streamxml;
};








/* ------------------------------ container class from any layer ---------------------------------*/
class Fop_Layer : public QObject
{
    Q_OBJECT
    Q_CLASSINFO( "author", "Peter Hohl" )
    Q_CLASSINFO( "link", "http://www.ciz.ch" )
    Q_CLASSINFO( "date", "2007/29/09" )
    Q_CLASSINFO( "version", "1.0.0a" )
    Q_CLASSINFO( "since", "1.0.0" )
    Q_CLASSINFO( "default_background", "#ffffff" )
    Q_CLASSINFO( "default_text", "#000000" )
    
public:
    


      Fop_Layer( QFileInfo fi  , int layernummer ) 
{
  fiplace = fi; 
  ImageMap.clear(); 
  nr = layernummer;  
  Zindex = 0;
  borderDicks =0;
  LockStatus = 0;
  Zrotate = 0;
  SetDocLayer();
}
inline void SetLock() { LockStatus = 1; }
inline bool GetLock() { return LockStatus; }
inline void SetRotate( int u ) { 
    Zrotate = u; 
    ///////////qDebug() << "### layer DeegresRotation->" << u;
    }
inline int GetRotate() { return Zrotate; }
inline void SetZindex( qreal v ) { Zindex = v; }
inline qreal GetZindex() { return Zindex; }
inline TypImageContainer GetResourceBlock() { return ImageMap; }
inline QColor Bocolor() { return BorderColor; }
inline int Id() { return nr; }
inline QString IdName() { return QString("layer_%1").arg(nr); }
inline QString TargetName() { return name; }
inline QColor Bgcolor() { return BackGroundColor; }
inline QRectF GetRectLayer() { return PlaceLayer; }  /* layer placement */
inline qreal Border() { return borderDicks; }   /* border */
inline QTextDocument * Qdoc()  { 
    
    QTextFrame  *Tframe = doc->rootFrame();
    QTextFrame::iterator it;
        for (it = Tframe->begin(); !(it.atEnd()); ++it) {
         QTextFrame *childFrame = it.currentFrame();
         QTextBlock para = it.currentBlock();
             if (childFrame)  {
                /* must not exist ! */
             }  else if (para.isValid()) {
                 if (para.userData()) {
                 QTextBlockUserData *foi =  para.userData();
                 /* static cast here */
                 Fop_Block *fopblock = static_cast<Fop_Block*>(foi);
                 ///////qDebug() << "### valid para  ###" << fopblock->Get_id() << " txt.->" << para.text();
                 } else {
                 ////////////qDebug() << "### !!!! not valid!  ###" << para.text();
                 //////////////qDebug() << "### ob id->  ###" << para.charFormatIndex();
                 ///////QTextObject *actual =  doc->object(para.charFormatIndex ());
                 /////para.clear();  /* delete unwanded !!! */
                 //////////actual->deleteLater();
                 }
             }
    }
    
return doc; 
}   /* border */
/* append image resource qtextdocument or background image */
void AppendImage( const QString resourcename , QVariant pix ) 
{
       QMapIterator<QString,QVariant> i(ImageMap);
         while (i.hasNext()) {
             i.next();
             if ( i.key() == resourcename)  {
             return;
             }
         } 
  ImageMap.insert(resourcename,pix);
}
void SetRealName( const QString names ) 
{
    name = names;
}
void SetDocLayer() 
{
    doc = new QTextDocument();
    QTextFrame  *Tframe = doc->rootFrame();
    QTextFrameFormat rootformats = Tframe->frameFormat();
    rootformats.setBottomMargin(0); 
    rootformats.setTopMargin(0);
    rootformats.setRightMargin(0);
    rootformats.setLeftMargin(0);
    rootformats.setPadding(1); 
    Tframe->setFrameFormat ( rootformats );
    
}
/* aLpha opacity from left/top */
void SetRects( qreal x, qreal y, 
                      qreal width, qreal height , 
                      qreal border , 
                      const  QString Bordercolorname , 
                      const  QString BGcolorname , 
                      int aLpha) 
{
    PlaceLayer = QRectF(x,y,width,height);
    
   /////////// qDebug() << "### QRectF " << PlaceLayer;
    ////////qDebug() << "### BGcolorname " << BGcolorname;
    
    QColor bgcolor(BGcolorname);
    ///////////qDebug() << "### BGcolorname aLpha yes ok..." << aLpha;
    bgcolor.setAlpha( aLpha );      
    BackGroundColor = bgcolor;   
    BorderColor = QColor(Bordercolorname);
    borderDicks = border;
}
    QRectF PlaceLayer;
    QFileInfo fiplace;
    QColor BackGroundColor;
    QColor BorderColor;
    qreal borderDicks;
    QTextDocument *doc;
    TypImageContainer ImageMap;
    int nr;
    
    int LockStatus;
    qreal Zindex;
    int Zrotate;
    QString name;
};

/* ------------------------------ container class from any layer ---------------------------------*/


/* ------------------------------ converter from fop to QTextDocument ----------------------------*/


class PreFopHandler : public QObject
{
    Q_OBJECT
    Q_CLASSINFO( "author", "Peter Hohl" )
    Q_CLASSINFO( "link", "http://www.ciz.ch" )
    Q_CLASSINFO( "date", "2007/29/09" )
    Q_CLASSINFO( "version", "1.0.0a" )
    Q_CLASSINFO( "since", "1.0.0" )
    Q_CLASSINFO( "default_background", "#ffffff" )
    Q_CLASSINFO( "default_text", "#000000" )
//
public:
    
typedef enum
{  
  TAG_NOT_KNOW = 404,
  PAGE_SETUP, /* fo:simple-page-master  405 */
  BLOCK_TAG,    /* fo:block  406*/
  TABLE_TAG,  /* fo:table 407*/
  TABLE_FOOTER, /* table-footer  408*/
  TABLE_HEADER, /* table-header 409*/
  TABLE_BODY, /* fo:table-body 410*/
  TABLE_COL, /* fo:table-column 411*/
  TABLE_ROW,   /* fo:table-row 412*/
  TABLE_CELL, /* fo:table-cell 413*/
  BLOCK_CONTAINER,  /* fo:block-container   floating layer 414*/ 
  INLINE_STYLE,  /* fo:inline 415*/
  LINK_DOC,  /* fo:basic-link 416*/
  IMAGE_SRC, /* fo:external-graphic   all supported qt4.4 image + tif 417*/
  IMAGE_INLINE,  /* fo:instream-foreign-object  svg inline only svg image grep 418*/
  LIST_BLOCK,   /* fo:list-block  ul/ol 419*/
  LIST_ITEM,   /* fo:list-item  li 420*/
  LIST_BODY,   /* fo:list-item-body 421*/
  LIST_LABEL,   /* fo:list-item-label  422*/
  FIRST_LAST_TAG,      /* fo:root 423*/
  FOCHAR      /* fo:character 424 */
} FOPTAGSINT;

/* frame iterator inside fo:block-container */

typedef enum
{  
  FRAME_ROOT = 414,
  FRAME_TDCELL,
  FRAME_NORMAL,
  FRAME_BYPASS,
  FRAME_UNKNOW
} FRAME_TYP;








    PreFopHandler();
    qreal Unit( const QString datain );
    void ReportError();
    inline void SetDB( PageDB *dbin ) { db = dbin; }
    FOPTAGSINT FoTag( const QDomElement e );
    void PaintFopBlockFormat( QDomElement e , QTextBlockFormat bf );  /* block */
    void PaintCharFormat( QDomElement e , QTextCharFormat bf = QTextCharFormat() );
    /////////////inline QList<PageDimensionName*> GetPageFormatter() { return FormatPages; }

    Qt::PenStyle StyleBorder( const QDomElement e );
    QPixmap PaintTableCellPixmap( QRectF r , QColor bg , QColor ma , qreal borderDicks , Qt::PenStyle styls = Qt::SolidLine  ); 
    qreal Get_Cell_Width( QTextTableFormat TableFormat , int position );  /* return cell Width if 
                                                                          is QTextLength::FixedLength  
                                                                       to enable paint cell border */
    qreal TakeOneBorder( const QDomElement e );
    qreal TakeOnePadding( const QDomElement e );
    QTextBlockFormat  DefaultMargin( QTextBlockFormat rootformats =  QTextBlockFormat() );
    void TableBlockMargin( QDomElement appender , const QString name = "table" );

    QTextCharFormat NewMixedBlockFormat( const QDomElement e  );
    QTextBlockFormat TextBlockFormFromDom( const QDomElement e , QTextBlockFormat pf = QTextBlockFormat() );
    QTextCharFormat  GlobalCharFormat( const QDomElement e , QTextCharFormat f =  QTextCharFormat() );

    bool BuildSvgInlineonDom( const QString urlrefimage , QDomElement e , QDomDocument doc  );

    QTextBlockFormat PaddingToNext( qreal paddingmargin , QTextBlockFormat rootformats = QTextBlockFormat());   /* grab padding from cell to bring on paragraph */
    QVariant ResourceOnSaveImage( const QString urlimage  );
    void ImageBlockAppend( TypImageContainer tocaster  );    /* incomming image to save ....  dest fop file */
    bool SaveImageExterPath( const QString urlrefimage , const QString fulldirlocalfile );   /* save image to file */
    TypImageContainer PromtToSaveImage;
    TypErnoMap ErnoMap;
    QString startdir;
    int ErrorSumm;
    int contare;
    TypnotexistFontMap notexistFontMap;
    QSettings setter;
    int errorreport;
    ////////QList<PageDimensionName*> FormatPages;
    PageDB *db;
private:
    
signals:
public slots:

};
//
#endif // PREFOPHANDLER_H

