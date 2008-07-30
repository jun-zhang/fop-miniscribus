#include "_LayerApi.h"

#include "_Image_Page_Struct.h"


ScribePage::ScribePage( M_PageSize e )
 : QObject(),_d(new QTextDocument),PageTotal(1),eventline(0),timeline(0),
 OnPageClick(1),cursortime(true),cursorOn(false),overwriteMode(false),FullDocSelect(false)
{
	M_PageSize DefaultSizeDoc;
	SceneTimerLine = 0;
	QTextDocument *dummy = new QTextDocument();
	dummy->setHtml ( ReadFile("a.html") );
	setDocument(dummy,FOP);
	SwapPageModel(DefaultSizeDoc);  /* dimension of page */
	Q_ASSERT(_d->pageSize().isValid());
}


/*
####################################################################################
################################ Paint section #####################################
####################################################################################
*/

void ScribePage::paint(QPainter * painter , const QStyleOptionGraphicsItem *option , QWidget *widget   )
{
	PageTotal = _d->pageCount();
	const int PageSumm = qBound (1,_d->pageCount(),MaximumPages);
	QTextFrame  *Tframe = _d->rootFrame();
	root_format = Tframe->frameFormat();
	const QRectF ActiveBlock = CurrentBlockRect();   /* discovery qtextcursor living page  Current_Page_Nr  */
	
	  painter->save();
	  painter->setPen( Qt::NoPen );
    painter->setBrush(Qt::lightGray);
		painter->drawRect(boundingRect());
	  painter->restore();
	 for (int i = 0; i < PageSumm; ++i)  {
		 const QPointF topleft = PageIndexTopLeft(i);  /* page top left point */
		 DrawPage(i,painter,i);
	 }
	
}

void ScribePage::DrawPage( const int index  , QPainter * painter , const int cursorpage )
{
		const QPointF topleft = PageIndexTopLeft(index);
	  QAbstractTextDocumentLayout::PaintContext CTX; 
	  CTX.palette.setColor(QPalette::Text, Qt::black);
	  const QRectF body = QRectF(0, topleft.y() ,Page_Edit_Rect.width(),Page_Edit_Rect.height()); /* on view */
	  QRectF view(0, index * body.height(), body.width(), body.height() );   /* on doc */
	
		if (index != cursorpage || !Edit_On()) {
		painter->save();
		painter->translate(body.left(), body.top() - index * body.height());
		painter->setClipRect(view);
    CTX.clip = view;
	  _d->documentLayout()->draw(painter,CTX);
    painter->restore();
    return;
		}
		
		/* draw cursor active page no !edit no print only display edit! */
		painter->save();
		painter->translate(body.left(), body.top() - index * body.height());
		painter->setClipRect(view);
    CTX.clip = view;
		
		QColor BackHightlight("#0072ab");
		BackHightlight.setAlpha(180);   /* original 150 */

	  
		CTX.palette.setColor(QPalette::Text, Qt::black);
		CTX.palette.setColor(QPalette::Highlight,BackHightlight);
		CTX.palette.setColor(QPalette::HighlightedText,Qt::white);
		CTX.selections;
		CTX.clip = view;
	  CTX.cursorPosition = -1;
		/* play cursor */
		
		  if (cursortime) {
				CTX.cursorPosition = C_cursor.position();
			 }
			 
			 
			if ( C_cursor.hasSelection()) {
				QAbstractTextDocumentLayout::Selection Internal_selection;
				Internal_selection.cursor = C_cursor;
				cursorIsFocusIndicator = true;
				QPalette::ColorGroup cg = cursorIsFocusIndicator ? QPalette::Active : QPalette::Inactive;
				Internal_selection.format.setBackground(CTX.palette.brush(cg, QPalette::Highlight));
				Internal_selection.format.setForeground(CTX.palette.brush(cg, QPalette::HighlightedText));
				Internal_selection.format.setProperty(QTextFormat::FullWidthSelection, true);
				CTX.selections.append(Internal_selection);
				
			}
			
		_d->documentLayout()->draw(painter,CTX);
		painter->restore();
			
			/*
			
			QLineF cursorLiner = BlinkCursorLine();
			QLineF cursorLiner2 = ViewBlinkedCursorLine();
			
			painter->save();
			painter->setPen( QPen(Qt::red,3) );
			painter->setBrush(Qt::red);
			painter->drawEllipse(cursorLiner.p2(),5,5);
			
			painter->setPen( QPen(Qt::green,3) );
			painter->setBrush(Qt::green);
			painter->drawEllipse(cursorLiner2.p2(),5,5);
			
			painter->restore();
			*/
			
			

			
			
}

QPointF ScribePage::PageIndexTopLeft( const int index  )
{
	const qreal fromTopY = index * Page_Edit_Rect.height();
	const qreal spacepage = index * InterSpace;
	return QPointF(0,fromTopY + spacepage);
}

/*
####################################################################################
################################ Paint section #####################################
####################################################################################


####################################################################################
################################ Ausiliary tool#####################################
####################################################################################

*/



void ScribePage::setDocument ( const QTextDocument * document , FileHandlerType Type )
{
    if (_d) {
		_d->clear();
		_d->disconnect(this);
	  _d->documentLayout()->disconnect(this);
		_d = 0;
		} else {
    _d = new QTextDocument(0);
    }
    
	
	clipboard = QApplication::clipboard();
	Op = Type;
  _d = const_cast<QTextDocument*>(document); 
  
        for (QTextBlock srcBlock = document->firstBlock(), dstBlock = _d->firstBlock();
             srcBlock.isValid() && dstBlock.isValid();
             srcBlock = srcBlock.next(), dstBlock = dstBlock.next()) {
            dstBlock.layout()->setAdditionalFormats(srcBlock.layout()->additionalFormats());
        }
				
	M_PageSize DefaultSizeDoc;
	SwapPageModel(DefaultSizeDoc);
  PageTotal = _d->pageCount();
  Q_ASSERT(PageTotal > 0);	
  C_cursor = QTextCursor(_d);
  C_cursor.setPosition(0,QTextCursor::MoveAnchor);
	setBlinkingCursorEnabled(true);
  ////////QObject::connect(clipboard, SIGNAL(dataChanged() ), this, SLOT(int_clipboard_new()));
				/////QObject::connect(_d, SIGNAL(modificationChanged(bool) ),this, SLOT(update() ));  /* paint area size? */
  QObject::connect(_d, SIGNAL(cursorPositionChanged(QTextCursor) ), this, SLOT(cursorPosition(QTextCursor) ));
  ///////////QObject::connect(_d, SIGNAL(cursorPositionChanged(QTextCursor) ), this, SLOT(NewCharformat(QTextCursor) ));
  ///////QObject::connect(_d, SIGNAL(contentsChanged()), this, SLOT(SessionUserInput()));
	///////QObject::connect(this, SIGNAL(q_cursor_newPos()), this, SLOT(SessionUserInput()));
	///////QObject::connect(this, SIGNAL(q_update(QRectF)), this, SLOT(redir_update(QRectF)));
  //////////QObject::connect(_d, SIGNAL(documentLayoutChanged()), this, SLOT(SessionUserInput()));
  /////////QObject::connect(_d, SIGNAL(contentsChange(int,int,int)), this, SLOT(SessionUserInput(int,int,int)));
				
	///////QObject::connect(_d->documentLayout(), SIGNAL(update(QRectF)), this, SLOT(LayoutRepaint(QRectF)));
				
				
				////////void QAbstractTextDocumentLayout::  update(QRectF) 
				
				
				
}


///////QObject::connect(_d->documentLayout(), SIGNAL(update(QRectF)), this, SLOT(LayoutRepaint(QRectF)));
/* big rect !!!!! */
void ScribePage::LayoutRepaint( const QRectF docrect )
{
	return;
	
	(void)boundingRect();   /* change page if need */
	if (!boundingRect().intersects(docrect)) {
	QRectF aread = boundingRect().intersected(docrect);
	redir_update(aread);
	return;
	}
	QRectF areadu = QRectF(  traposin(docrect.topLeft())  ,  docrect.size () );
	////////qDebug() << "### LayoutRepaint  b " << docrect;
	redir_update(areadu);
}



void ScribePage::redir_update( QRectF area )
{
	
	const QRect needed = area.toRect();
	if (needed.width() > boundingRect().width()) {
	return;
	}
	if (!boundingRect().toRect().intersects(needed)) {
	repaintCursor();
	return;
	}
	if (OverloadRectUpdate == needed) {
	/* free it on 400 ms */
	return;
	}
	OverloadRectUpdate = needed;
	////////qDebug() << "### redir_update  " << needed;
	emit q_update(needed);
}


void ScribePage::SwapPageModel( M_PageSize e )
{
	ApiSession *sx = ApiSession::instance();
	sx->SetPageFormat(e);
	PAGE_MODEL = e;
	QTextOption opt;
	opt.setUseDesignMetrics(true);
	opt.setTabStop(8);
	opt.setWrapMode ( QTextOption::WrapAtWordBoundaryOrAnywhere );
	_d->setDefaultTextOption(opt);
	
	PAGE_MODEL.HandlePrint(_d); /* set page format margin  */
  Q_ASSERT(_d->pageSize().isValid());
	_d->setUseDesignMetrics (true);
	Page_Width = PAGE_MODEL.G_regt.width();
  Page_Height = Page_Width * PAGE_MODEL.faktor();
  Page_Edit_Rect = QRectF(0,0,Page_Width,Page_Height);
  _d->setPageSize(QSizeF(Page_Width,Page_Height));
	Q_ASSERT(_d->pageSize().isValid());
  (void)_d->documentLayout(); /* reform margin wake up */
	PageTotal = _d->pageCount();
	q_update(boundingRect().toRect());
}

QRectF ScribePage::boundingRect()
{
	const int page = qBound(1,_d->pageCount(),MaximumPages);
	const qreal spacer = page + ((page - 1) * InterSpace );
	const qreal pagesummhi = page * Page_Height;
	if (PageTotal != page) {
		PageTotal = page;
		
		 for (int i = OnPageClick; i < _d->pageCount(); ++i)  {
		 const QPointF topleft = PageIndexTopLeft(i);
     QRectF PaperAreas(topleft,QSizeF(Page_Width,Page_Height));
			 emit q_update(PaperAreas.toRect());
	   }
		 emit q_update_scene();
	}
	
	////////////qDebug() << "### OnPageClick  " << OnPageClick;
	
  return QRectF(0,0,Page_Width,pagesummhi + spacer);
}

QRectF ScribePage::GroupboundingRect()
{
	const QRectF onlypage = boundingRect();
	return QRectF(0,0,onlypage.width() + BorderShadow,onlypage.height() + BorderShadow );
}


/* new cursor position .....  */
void ScribePage::cursorPosition( const QTextCursor curs )
{
	cursor_position = curs.position();
	LastCharFormat = curs.charFormat();  
	if (curs.isCopyOf(C_cursor)) {
      
	} else {
		 C_cursor.setPosition(curs.position());
		 cursor_position = C_cursor.position();
		 ////////WakeUpElasticSize();
	}
  
  
  /*
    qmenu go session 
	  remkake bold color checked ..... 
    
    */
    
	  emit q_cursor_newPos();
		EnsureVisibleCursor();
}







void ScribePage::setBlinkingCursorEnabled( bool enable )
{
     edit_enable = enable;
  
	   if (cursorOn) {
			   return;
		 }
		 cursorOn = enable;
    if (enable && QApplication::cursorFlashTime() > 0) {
        cursorTimeLine.start( QApplication::cursorFlashTime() / 2,this);
		}  else {
        cursorTimeLine.stop();
		}
}


QTextCursor ScribePage::textCursor() 
{
  return C_cursor;
}


QTextDocument *ScribePage::document() const
{
  return _d;
}



void ScribePage::timerEvent(QTimerEvent *event)
{
	
	if (!edit_enable) {
     return;
	}
    if (event->timerId() == cursorTimeLine.timerId()) {
			cursortime = cursortime == true ? false : true;
			repaintCursor();
			SceneTimerLine++;
				
						
						if (SceneTimerLine == 4) {
						SceneTimerLine = 0;
						emit q_update_scene();
						///////qDebug() << "### SceneTimerLine " << SceneTimerLine;
						}
						
						
						
						
    }
		
		if (cursor_position != C_cursor.position()) {
			cursor_position = C_cursor.position();
			emit q_cursor_newPos();
		}
}





QLineF ScribePage::BlinkCursorLine()
{
	QLineF CursorDrawLine(QPointF(0,0),QPointF(0,10));  /* error line */
	/* QTextDocument *_d;  */
	const QRectF xxtmp = _d->documentLayout()->blockBoundingRect(textCursor().block());
	QTextFrame  *Tframe = _d->rootFrame();
	root_format = Tframe->frameFormat();
	QTextLine TTline = currentTextLine(textCursor());
	if ( TTline.isValid() ) {
	   int pos = textCursor().position() - textCursor().block().position();
		 const qreal TextCursorStartTop = TTline.lineNumber() * TTline.height() + xxtmp.top();
		 const qreal TextCursorStartLeft = TTline.cursorToX(pos) + root_format.leftMargin() + root_format.padding();
		 CursorDrawLine = QLineF(QPointF(TextCursorStartLeft,TextCursorStartTop),QPointF(TextCursorStartLeft,TextCursorStartTop + TTline.height()));
	}
	return CursorDrawLine;
}


QLineF ScribePage::ViewBlinkedCursorLine()
{
	(void)CurrentBlockRect();
	QLineF cursorLiner = BlinkCursorLine();
	qreal increment = Current_Page_Nr * InterSpace;
	if (isBottomBlock) {
		increment = increment + root_format.bottomMargin() + root_format.padding() + root_format.topMargin();
	}
	cursorLiner.translate(QPointF(0,increment));
  return cursorLiner;
}


QRectF ScribePage::CurrentBlockRect()
{
	const QRectF xxtmp = _d->documentLayout()->blockBoundingRect(textCursor().block());
	QLineF cursorLiner = BlinkCursorLine();
	
	QTextFrame  *Tframe = _d->rootFrame();
	root_format = Tframe->frameFormat();
	
	const int page_a1 = cursorLiner.p2().y() / Page_Edit_Rect.height();
	const int page_a2 = cursorLiner.p2().y() / (Page_Edit_Rect.height() - root_format.bottomMargin() - root_format.padding() );
	isBottomBlock = page_a1 == page_a2 ? false : true;
	Current_Page_Nr = cursorLiner.p2().y() / Page_Edit_Rect.height();
	if (isBottomBlock) {
		Current_Page_Nr = qMax (page_a1,page_a2);
	}
	/////////////////////qDebug() << "### Current_Page_Nr " << Current_Page_Nr << " bottom->" << isBottomBlock;
	qreal spacer =  Current_Page_Nr * InterSpace;
	if (isBottomBlock) {
		spacer = spacer + root_format.bottomMargin() + root_format.padding() + root_format.topMargin();
	}
  const qreal largoblocco = qBound (PAGE_MODEL.width(),xxtmp.width(),PAGE_MODEL.width());
	qreal altoblocco = xxtmp.height() * 1.888888;
	if (isBottomBlock) {
		altoblocco = xxtmp.height() + root_format.bottomMargin() + root_format.padding() + root_format.topMargin();
	}
	QRectF blockrr(PAGE_MODEL.P_margin.height(),xxtmp.topLeft().y() + spacer , largoblocco , altoblocco );
  return blockrr;
}

qreal ScribePage::SlicedPage( const int page )
{
	return (page + 1) * Page_Edit_Rect.height();
	
}


void ScribePage::FrameHandler()
{
	const qreal spacer =  Current_Page_Nr * InterSpace;
	QTextFrame  *RootFrame = _d->rootFrame();
	
	const int selectionLength = qAbs(C_cursor.position() - C_cursor.anchor());
	
	//////////qDebug() << "### selectionLength  " << selectionLength;
	
	
	   if (C_cursor.currentTable()) {
        QTextTable *table = C_cursor.currentTable();
        QRectF tre = _d->documentLayout()->frameBoundingRect(table);
				emit q_update(QRect(tre.left() , tre.top() + spacer , tre.width() , tre.height()));
        return;
		 } else if ( C_cursor.currentFrame() && C_cursor.currentFrame() != RootFrame ) {
			  QTextFrame  *inlineFrame = C_cursor.currentFrame();
			  QRectF tre = _d->documentLayout()->frameBoundingRect(inlineFrame);
			  emit q_update(QRect(tre.left() , tre.top() + spacer , tre.width() * 1.8 , tre.height()));
		 } else if (selectionLength > 999 )  {
		   emit q_update_scene();
		 } else {
			 QRectF par = CurrentBlockRect();
			 /* large paragraph update !!!!!! */
			 emit q_update(QRect(0,par.top() - par.height(), Page_Width , par.height() * 3 ));
		 }

}

void ScribePage::EnsureVisibleCursor()
{
	const QRectF paragraphrect = CurrentBlockRect();
	QRectF large(-10,paragraphrect.top() - 25,Page_Width + 10,paragraphrect.height() + 100);
	emit q_visible(large);
}


QTextLine ScribePage::currentTextLine(const QTextCursor &cursor)
{
    const QTextBlock block = cursor.block();
    if (!block.isValid())
        return QTextLine();

    const QTextLayout *layout = block.layout();
    if (!layout)
        return QTextLine();
    const int relativePos = cursor.position() - block.position();
    return layout->lineForTextPosition(relativePos);
}

QPointF ScribePage::traposin( const QPointF &pos )
{
	if (!boundingRect().contains(pos)) {
	return QPointF(0,0);
	}
	
	const int currentpageisY =  pos.y() / Page_Height ;
	if (currentpageisY == 0) {
	return pos;
	}
	const qreal spacer = currentpageisY * InterSpace;   /* summ space */
	const qreal PosY = pos.y() - spacer; /* decrement space */
	return QPointF(pos.x(),PosY);
}


bool ScribePage::AllowedPosition( const QPointF inpos )
{
	bool permission = false;
	 for (int i = 0; i < _d->pageCount(); ++i)  {
		 QRectF sssx = PAGE_MODEL.PageInternal(i);
		 if (sssx.contains(inpos)) {
			 OnPageClick = i + 1;
			 return true;
		 }
	 }
	return permission;
}











void ScribePage::CursorMovetoPosition( const QPointF &pos )
{
	const int cursorPosFozze = _d->documentLayout()->hitTest(pos,Qt::FuzzyHit);
	if (cursorPosFozze != -1) {
	  C_cursor = QTextCursor(_d);
		C_cursor.setPosition(cursorPosFozze);
		cursor_position = cursorPosFozze;
		cursortime = true; /* fast display */
	}
	
}






/*

####################################################################################
################################ Ausiliary tool#####################################
####################################################################################


####################################################################################
################################ Input Event #######################################
####################################################################################

*/


void ScribePage::selectAll()
{
	 if (position_selection_start == C_cursor.anchor()) {
		return;
	 }
	
	C_cursor.select(QTextCursor::Document);
	cursor_position = C_cursor.position();
	position_selection_start = C_cursor.anchor();
	FullDocSelect = true;
	emit q_update_scene();
}

void ScribePage::deleteSelected()
{
    if (Edit_On()) {
    C_cursor.removeSelectedText();
		}
}

void ScribePage::cut()
{
	if (!Edit_On()) {
		return;
	}
	
    if (!C_cursor.hasSelection()) {
			QApplication::beep(); 
	    return;
		}
    copy();
    C_cursor.removeSelectedText();
	  /////SwapSelectionsCursor();
}

void ScribePage::paste()
{
	if (!Edit_On()) {
		return;
	}
	InsertMimeDataOnCursor(clipboard->mimeData());
	emit q_update_scene();
}

void ScribePage::int_clipboard_new()
{
	ApiSession *sx = ApiSession::instance();
	sx->SaveMimeTmp();   /* clone a copy on session before next incomming */
	/////qDebug() << "### clipboard in ";
}

void ScribePage::copy()
{
    if (!C_cursor.hasSelection()) {
			QApplication::beep(); 
	    return;
		}
    QMimeData *data = createMimeDataFromSelection();
    clipboard->setMimeData(data);
}


void ScribePage::undo()
{
	if (!Edit_On()) {
		return;
	}
	
	  _d->undo();
	////////qDebug() << "### undo ";
}

void ScribePage::redo()
{
	if (!Edit_On()) {
		return;
	}
	
	  _d->redo();
	 ////////////qDebug() << "### redo ";
}


bool ScribePage::cursorMoveKeyEvent(QKeyEvent *e)
{
    const QTextCursor oldSelection = C_cursor;
    const int oldCursorPos = oldSelection.position();

    QTextCursor::MoveMode mode = QTextCursor::MoveAnchor;
    QTextCursor::MoveOperation op = QTextCursor::NoMove;

		if (e->key() == Qt::Key_Right) {
            op = QTextCursor::Right;
    } else if (e->key() == Qt::Key_Left) {
            op = QTextCursor::Left;
    } else if (e == QKeySequence::MoveToPreviousChar) {
            op = QTextCursor::Left;
    }
    else if (e == QKeySequence::SelectNextChar) {
           op = QTextCursor::Right;
           mode = QTextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectPreviousChar) {
            op = QTextCursor::Left;
            mode = QTextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectNextWord) {
            op = QTextCursor::WordRight;
            mode = QTextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectPreviousWord) {
            op = QTextCursor::WordLeft;
            mode = QTextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectStartOfLine) {
            op = QTextCursor::StartOfLine;
            mode = QTextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectEndOfLine) {
            op = QTextCursor::EndOfLine;
            mode = QTextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectStartOfBlock) {
            op = QTextCursor::StartOfBlock;
            mode = QTextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectEndOfBlock) {
            op = QTextCursor::EndOfBlock;
            mode = QTextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectStartOfDocument) {
            op = QTextCursor::Start;
            mode = QTextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectEndOfDocument) {
            op = QTextCursor::End;
            mode = QTextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectPreviousLine) {
            op = QTextCursor::Up;
            mode = QTextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectNextLine) {
            op = QTextCursor::Down;
            mode = QTextCursor::KeepAnchor;
            {
                QTextBlock block = C_cursor.block();
                QTextLine line = currentTextLine(C_cursor);
                if (!block.next().isValid()
                    && line.isValid()
                    && line.lineNumber() == block.layout()->lineCount() - 1)
                    op = QTextCursor::End;
            }
    }
    else if (e == QKeySequence::SelectNextLine) {
            op = QTextCursor::Down;
            mode = QTextCursor::KeepAnchor;
            {
                QTextBlock block = C_cursor.block();
                QTextLine line = currentTextLine(C_cursor);
                if (!block.next().isValid()
                    && line.isValid()
                    && line.lineNumber() == block.layout()->lineCount() - 1)
                    op = QTextCursor::End;
            }
    }
    else if (e == QKeySequence::MoveToNextWord) {
            op = QTextCursor::WordRight; ///// 16777236
    }
    else if (e == QKeySequence::MoveToPreviousWord) {
            op = QTextCursor::WordLeft;
    } else if (e->key() == Qt::Key_Left) {
            op = QTextCursor::WordLeft;
    } else if (e->key() == Qt::Key_Right) {
            op = QTextCursor::WordRight;
    } else if (e == QKeySequence::MoveToEndOfBlock) {
            op = QTextCursor::EndOfBlock;
    }
    else if (e == QKeySequence::MoveToStartOfBlock) {
            op = QTextCursor::StartOfBlock;
    }
    else if (e == QKeySequence::MoveToNextLine) {
            op = QTextCursor::Down;
    }
    else if (e == QKeySequence::MoveToPreviousLine) {
            op = QTextCursor::Up;
    }
    else if (e == QKeySequence::MoveToPreviousLine) {
            op = QTextCursor::Up;
    }
    else if (e == QKeySequence::MoveToStartOfLine) {
            op = QTextCursor::StartOfLine;
    }
    else if (e == QKeySequence::MoveToEndOfLine) {
            op = QTextCursor::EndOfLine;
    }
    else if (e == QKeySequence::MoveToStartOfDocument) {
            op = QTextCursor::Start;
    }
    else if (e == QKeySequence::MoveToEndOfDocument) {
            op = QTextCursor::End;
    }

		
    else {
        return false;
    }
		/////////_d->adjustSize();   /* destroy page count */
		repaintCursor(true);
		
		const bool moved = C_cursor.movePosition(op,QTextCursor::MoveAnchor);
		if (moved) {
        if (C_cursor.position() != oldCursorPos) {
					cursor_position = C_cursor.position();
				  emit q_cursor_newPos();
				}
				
    }
    return true;
}

/* after key press */
void ScribePage::Controller_keyReleaseEvent ( QKeyEvent * e )
{
	/////////qDebug() << "### Controller_keyReleaseEvent";
	
	  if (trippleClickTimer.isActive()) {
		trippleClickTimer.stop();
	  }
	
	
	FrameHandler();  
	cursortime = true;
}

void ScribePage::Controller_keyPressEvent ( QKeyEvent * e )
{
	  ///////qDebug() << "### Controller_keyPressEvent ---doc  root   " << e->text();
		DragFill = false;
	  cursortime = false;
	  if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_S) {
		return;
		}
	
	
	
		if (trippleClickTimer.isActive()) {
		trippleClickTimer.stop();
	  }
		
		if (dragClickTimer.isActive()) {
		dragClickTimer.stop();
	  }
	
		LastCharFormat = C_cursor.charFormat();
		
		/* fast access */
		
		if (e == QKeySequence::SelectAll || (e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_A) {
            e->accept();
            selectAll();
            return;
    } else if (e == QKeySequence::Copy || (e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_C) {
            e->accept();
            copy();
            return;
    }  else if (e == QKeySequence::Paste || (e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_V) {
            e->accept();
            paste();
            return;
    } else if (e == QKeySequence::Cut || (e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_C) {
            e->accept();
            cut();
            return;
    }
		
		
		if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_B) {
			      LastCharFormat.setFontWeight(!C_cursor.charFormat().font().bold()  ? QFont::Bold : QFont::Normal);
						C_cursor.setCharFormat(LastCharFormat);
						e->accept();
						return;
		}
		if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_I) {
			      LastCharFormat.setFontItalic(!LastCharFormat.font().italic() ? true : false );
			      C_cursor.setCharFormat(LastCharFormat);
            e->accept();
						return;
		}
		
		if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_J) {
			      InsertImageonCursor();
            e->accept();
						return;
		}
		
		if ((e->modifiers() & Qt::AltModifier) && e->key() == Qt::Key_S) {
			      showhtml();
						return;
		}
		
		
		
		
		
		
		if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_U) {
            LastCharFormat.setUnderlineStyle(!LastCharFormat.font().underline() ? QTextCharFormat::SingleUnderline : QTextCharFormat::NoUnderline );
			      C_cursor.setCharFormat(LastCharFormat);
            e->accept();
						return;
		}
		if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_H) {
            C_cursor.insertFragment(QTextDocumentFragment::fromHtml("<hr>"));
            e->accept();
						return;
		}
		if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Z || e == QKeySequence::Undo) {
            e->accept();
            undo();
						return;
		}
		if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Y || e == QKeySequence::Redo ) {
            e->accept();
            redo();
						return;
		}
		
		
		//////////////return;
		
		/* move action on cursor ? */
		if (cursorMoveKeyEvent(e)) {
			 e->accept();
			 cursor_position = C_cursor.position();
			 EnsureVisibleCursor();  /* big steep move */
			
			 if (FullDocSelect) {
		    q_update_scene();
	      }
			
			 return;
		}
		
		/* fast access end  */
		
		if (e->key() == Qt::Key_Backspace && !(e->modifiers() & ~Qt::ShiftModifier)) {
			
        QTextBlockFormat blockFmt = C_cursor.blockFormat();
			
        QTextList *list = C_cursor.currentList();
        if (list && C_cursor.atBlockStart()) {
            list->remove(C_cursor.block());
        } else if (C_cursor.atBlockStart() && blockFmt.indent() > 0) {
            blockFmt.setIndent(blockFmt.indent() - 1);
            C_cursor.setBlockFormat(blockFmt);
        } else {
            C_cursor.deletePreviousChar();
        }
        goto accept;
    }  else if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
			
        if (e->modifiers() & Qt::ControlModifier) {
					  if (Op != FOP ) {
            C_cursor.insertText(QString(QChar::LineSeparator));
						} else {
						C_cursor.insertBlock();
						}
					
        } else {
            C_cursor.insertBlock();   /* default format can take from setting */
				}
        e->accept();
        goto accept;
    } else if (e == QKeySequence::Delete) {
			
			         if (C_cursor.hasSelection()) {
				           QString remtxt = C_cursor.selectedText();
									 for (int i = 0; i < remtxt.size(); ++i) {
										 C_cursor.deleteChar();
									 }
								 ////////////SwapSelectionsCursor();  /* clear */
						   } else {
								 C_cursor.deleteChar();
							 }
							 
			   goto accept;
		} else if (e == QKeySequence::DeleteEndOfWord) {
        C_cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
        C_cursor.deleteChar();
			  goto accept;
    } else if (e == QKeySequence::DeleteStartOfWord) {
        C_cursor.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor);
        C_cursor.deleteChar();
			  goto accept;
    }
    else if (e == QKeySequence::DeleteEndOfLine) {
        QTextBlock block = C_cursor.block();
        if (C_cursor.position() == block.position() + block.length() - 2) {
            C_cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
				} else {
            C_cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
            C_cursor.deleteChar();
				}
				goto accept;
    }  else {
			 /* go insert text ......................*/
			 goto process;
		}
		
		
		
		process:
    {
        QString text = e->text();
        if (!text.isEmpty() && (text.at(0).isPrint() || text.at(0) == QLatin1Char('\t'))) {
					
            if (overwriteMode && !C_cursor.hasSelection() && !C_cursor.atBlockEnd()) {
							C_cursor.deleteChar();
						}
						
            C_cursor.insertText(text);
						cursor_position = C_cursor.position();
						e->accept();
						
						if (FullDocSelect) {
		        q_update_scene();
	          }
						
						return;
						
        } else if (!text.isEmpty() && C_cursor.hasSelection() ) {
					     QString remove = C_cursor.selectedText(); 
					     for (int i = 0; i < remove.size(); ++i) {
								 C_cursor.deletePreviousChar();
							 }
							 
					     C_cursor.insertText(text);
							 
							 if (FullDocSelect) {
		           q_update_scene();
	             }
							 
						   return;
							 
				}  else {
            e->ignore();
					  QApplication::beep();
            return;
        }
    }
		
		
	
		/* ########## section accept ################*/
	accept:
	{
		 cursor_position = C_cursor.position();
	/* ########## section accept ################*/
	}
	
	
	if (FullDocSelect) {
	  q_update_scene();
	}
	
}






void ScribePage::repaintCursor( bool allrect )
{
	
	////////qDebug() << "### repaintCursor " << __LINE__ << eventline;
	
     if (allrect) {
			emit q_update_scene();
		  return;
		 }
     if (C_cursor.hasComplexSelection() && C_cursor.currentTable()) {
        QTextTable *table = C_cursor.currentTable();
        QRectF tablerect = _d->documentLayout()->frameBoundingRect(table);
				emit q_update(tablerect.toRect());
        return;
		 }
     QRectF paragraphrect = CurrentBlockRect();
		 //////////QRectF paragraphrect = PAGE_MODEL.PageInternal(OnPageClick);
     if (paragraphrect.isValid()) {
			 emit q_update(paragraphrect.toRect());
		 } else {
			 emit q_update(PAGE_MODEL.PageInternal(OnPageClick).toRect());
		 }
     
}



bool ScribePage::procesevent( QEvent *e )
{
	eventline++;
	bool FoundEvent = false;
	QGraphicsSceneEvent *ev;
	
	
	  switch (e->type()) {
			     ///// case QEvent::UngrabMouse:
						////////case QEvent::GrabMouse:
            case QEvent::GraphicsSceneMouseMove:
            case QEvent::GraphicsSceneMousePress:
            case QEvent::GraphicsSceneMouseRelease:
            case QEvent::GraphicsSceneMouseDoubleClick:
            case QEvent::GraphicsSceneContextMenu:
            case QEvent::GraphicsSceneHoverEnter:
            case QEvent::GraphicsSceneHoverMove:
            case QEvent::GraphicsSceneHoverLeave:
            case QEvent::GraphicsSceneHelp:
            case QEvent::GraphicsSceneDragEnter:
            case QEvent::GraphicsSceneDragMove:
            case QEvent::GraphicsSceneDragLeave:
            case QEvent::GraphicsSceneDrop: {
                ev = static_cast<QGraphicsSceneEvent *>(e);
                Gwi = ev->widget();
                break;
            }
            default: 
						return false;
						break;
        };
				
		if (!ev) {
		return false;
		}
		
	 if (e->type() == QEvent::UngrabMouse) {
		  //////////position_selection_start = -1;
			/////////////C_cursor.clearSelection();
		  //////////qDebug() << "### UngrabMouse   " << e->type();
		 FoundEvent = true;
	 } if (e->type() == QEvent::GrabMouse) {
		  //////////position_selection_start = -1;
			///////C_cursor.clearSelection();
		  ////////qDebug() << "### GrabMouse   " << e->type();
		 FoundEvent = true;
	 } else if (ev->type() == QEvent::GraphicsSceneDragMove) {
		 DragFill = true;
		 QGraphicsSceneDragDropEvent *prespos = static_cast<QGraphicsSceneDragDropEvent *>(e);
		 const QPointF posi = traposin(prespos->pos());
		 CursorMovetoPosition(posi);
		 FrameHandler();
		 FoundEvent = true;
	 }  else if (ev->type() == QEvent::GraphicsSceneDrop && DragFill) {
		 ///////////qDebug() << "### QEvent::GraphicsSceneDrop   " << e->type();
		 QGraphicsSceneDragDropEvent *prespos = static_cast<QGraphicsSceneDragDropEvent *>(e);
		 const QPointF posi = traposin(prespos->pos());
		 C_cursor.clearSelection();
		 CursorMovetoPosition(posi);
		 
					if (dragClickTimer.isActive() ) {
					dragClickTimer.stop();
					}
					DragFill = false;
					position_selection_start = -1;
					
		 /* clear selection and paste */
		 InsertMimeDataOnCursor(prespos->mimeData());
		 FrameHandler();
		 FoundEvent = true;
	 }   else if (ev->type() == QEvent::GraphicsSceneMousePress) {
		 QGraphicsSceneMouseEvent *prespos = static_cast<QGraphicsSceneMouseEvent *>(e);
		 BaseMousePressEvent(traposin(prespos->pos()),prespos);
		 FoundEvent = true;
		 FullDocSelect = false;
	 } else if (ev->type() == QEvent::GraphicsSceneMouseDoubleClick) {
		 QGraphicsSceneMouseEvent *dblclickpos = static_cast<QGraphicsSceneMouseEvent *>(e);
		 const QPointF posi = traposin(dblclickpos->pos());
		 BaseDoubleClickEvent(posi,dblclickpos);
		 FoundEvent = true;
		 FullDocSelect = false;
	 } else if (ev->type() == QEvent::GraphicsSceneMouseMove) {
		 QGraphicsSceneMouseEvent *movepos = static_cast<QGraphicsSceneMouseEvent *>(e);
		 const QPointF posi = traposin(movepos->pos());
		 const int CursorPosition = _d->documentLayout()->hitTest(posi,Qt::FuzzyHit);
		 BaseMoveEvent(CursorPosition,posi);
		 FoundEvent = true;
	 } else if (ev->type() == QEvent::GraphicsSceneMouseRelease) {
		 QGraphicsSceneMouseEvent *movepos = static_cast<QGraphicsSceneMouseEvent *>(e);
		 const QPointF posi = traposin(movepos->pos());
		 BaseMouseReleaseEvent(posi,movepos->button());
		 FoundEvent = true;
	 }
	 
	 
	 if (FoundEvent) {
	 e->setAccepted ( true );
	 }
	 return FoundEvent;
}


void ScribePage::BaseDoubleClickEvent( const  QPointF posi , const QGraphicsSceneMouseEvent * event )  
{
	if (event->buttons() != Qt::LeftButton) {
		return;
	}
	const QTextCursor oldSelection = C_cursor;
	PointPositionOnDoc = posi;
	
	CursorMovetoPosition(posi);
	position_selection_start = -1;
	DragFill = false;
	C_cursor.clearSelection();
	QTextLine line = currentTextLine(C_cursor);
	bool doEmit = false;
	
	
	if (line.isValid() && line.textLength()) {
		C_cursor.select(QTextCursor::WordUnderCursor);
		doEmit = true;
	}
	 trippleClickTimer.start(qApp->doubleClickInterval(),this);
	 if (doEmit) {
		 repaintCursor();
	 }

}

bool ScribePage::StartDragOperation()
{
	DragFill = false;	
	if (!C_cursor.hasSelection()) {
	return DragFill;
	}
	
	QMimeData *data = createMimeDataFromSelection();
	                 if (data) {
									  QApplication::clipboard()->setMimeData(data);
                    QDrag *drag = new QDrag(Gwi);
                    drag->setMimeData(data); 
										drag->setHotSpot(QPoint(-25,-25));
                    const QPixmap playdragicon = ImagefromMime(data);
                    if (!playdragicon.isNull()) {
											drag->setPixmap(playdragicon);
                          //////drag->setPixmap(playdragicon.scaled(100,100,Qt::IgnoreAspectRatio));
											///////////drag->setDragCursor(playdragicon, Qt::CopyAction);
											///////////drag->setDragCursor(playdragicon, Qt::MoveAction);
                    }
										
										if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
                    ////////qDebug() << "### Launch Init Drag xxx " << prespos->pos();
										DragFill = true;
										emit q_startDrag(PointPositionOnDoc);
										}
									}
	return DragFill;
}


void ScribePage::BaseMousePressEvent( const  QPointF posi , const QGraphicsSceneMouseEvent *epress )  
{
	const int selectionLength = qAbs(C_cursor.position() - C_cursor.anchor());
	
	
	PointPositionOnDoc = posi;
	
	if (trippleClickTimer.isActive() && selectionLength > 0 ) {
		  /////////
		  DragFill = StartDragOperation();
		  if (DragFill)  {
				if ( epress->modifiers() == Qt::ControlModifier) {
						C_cursor.removeSelectedText();
				}
			 trippleClickTimer.stop();
			 dragClickTimer.start(qApp->doubleClickInterval(),this);
	    }
	}
	/////////qDebug() << "### BaseMousePressEvent  " << selectionLength;
	if (dragClickTimer.isActive() && selectionLength > 0) {
	//////////qDebug() << "### start drag  ";
		DragFill = StartDragOperation();
		if (DragFill)  {
				if ( epress->modifiers() == Qt::ControlModifier) {
						C_cursor.removeSelectedText();
				}
	  } else {
			position_selection_start = -1;
			C_cursor.clearSelection();
			dragClickTimer.stop();
			CursorMovetoPosition(posi);
		}
	return;
	}
	
	if (trippleClickTimer.isActive()) {
		trippleClickTimer.stop();
	}
	
	
	CursorMovetoPosition(posi);
	position_selection_start = C_cursor.position();
	DragFill = false;
	C_cursor.clearSelection();
}


void ScribePage::BaseMouseReleaseEvent( const  QPointF posi , Qt::MouseButton button )  
{
	////////qDebug() << "### BaseMouseReleaseEvent  ";
	
	if (dragClickTimer.isActive()) {
		  position_selection_start = -1;
			C_cursor.clearSelection();
		  dragClickTimer.stop();
			CursorMovetoPosition(posi);
		  return;
	}
	
	
	
		const int TMPCursorPosition = _d->documentLayout()->hitTest(posi,Qt::FuzzyHit);
	  ////////  if ( TMPCursorPosition != C_cursor.position() || TMPCursorPosition != C_cursor.anchor()  ) {
	  const int selectionLength = qAbs(C_cursor.position() - C_cursor.anchor());
	  DragFill = false;
	  PointPositionOnDoc = posi;
	  
	
	  if (button == Qt::MidButton) {
			/* paste if  having mime data */
			position_selection_start = -1;
			C_cursor.clearSelection();
			CursorMovetoPosition(posi);
			///// paste();
			return;
		} else if (button == Qt::LeftButton) {
			
			    if (!C_cursor.hasSelection()) {
						position_selection_start = -1;
					} else {
						///////////qDebug() << "### selectionLength  " << selectionLength;
						/* can start drag here ???????? */
						LastReleasePoint = posi;
						dragClickTimer.start(qApp->doubleClickInterval(),this);
					}
					
		///////qDebug() << "### position_selection_start  " << position_selection_start;
		/////////qDebug() << "### C_cursor.position() " << C_cursor.position();
		///////////////qDebug() << "### C_cursor.anchor()  " << C_cursor.anchor();
		} else {
			C_cursor.clearSelection();
			CursorMovetoPosition(posi);
			position_selection_start = -1;
		}
		
}



void ScribePage::BaseMoveEvent( const int cursorpos ,  QPointF moveposition )  
{
	const int cursorPosFozze = cursorpos;
	
	cursortime = false;
	
	const int stopat = qMax(position_selection_start,cursorPosFozze); 
	const int startat = qMin(position_selection_start,cursorPosFozze);
	
	
	if (position_selection_start >= 0 && cursorPosFozze >= 0 && !C_cursor.currentTable()) {
		/////////////qDebug() << "### selezione  " << position_selection_start <<  " to "  << cursorPosFozze;
		 if (stopat == cursorPosFozze) {
			 /* move >>>>>>>>>>>>>>>>>>>>>>>>>  */
			       C_cursor.setPosition(startat);
						 for (int i = startat; i < cursorPosFozze; ++i) {
						 C_cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
						 }
						 cursor_position = stopat;
						 FrameHandler();
						 LastReleasePoint = moveposition;
						 return;
		 }  else {
			 
			       C_cursor.setPosition(stopat);
						 const int diffs = stopat - cursorPosFozze;
			       for (int i = 0; i < diffs; ++i) {
								C_cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
							}
						 cursor_position = stopat;
						 FrameHandler();
						 LastReleasePoint = moveposition;
						 return;
		 }
		
		
		
	}
	
	 if ( C_cursor.currentTable() ) {
			QTextTable *table = C_cursor.currentTable();
			QTextTableCell firstcell = CellOnPosition(startat);
			QTextTableCell lastcell = CellOnPosition(stopat);
			if ( firstcell.isValid() && lastcell.isValid() ) {
			int fcellrow = firstcell.row();
			int fcellcool = firstcell.column();
			int numRows = qBound(1,lastcell.row() - fcellrow,table->rows());
			int numColumns = qBound(1,lastcell.column() - fcellcool,table->columns());
			///////qDebug() << "### nnrow / nncool " << numRows << numColumns;
			C_cursor.selectedTableCells(&fcellrow,&numRows,&fcellcool,&numColumns);
			C_cursor.setPosition(firstcell.firstPosition());
			C_cursor.setPosition(lastcell.lastPosition(), QTextCursor::KeepAnchor);
			cursor_position = stopat;
			FrameHandler();
			LastReleasePoint = moveposition;
			return;
			}
		}
		
		
	position_selection_start =-1;
	LastReleasePoint = QPointF(-1,-1);
}


/*

####################################################################################
################################ Input Event #######################################
####################################################################################

*/






QTextTableCell ScribePage::CellOnPosition( const int posi )
{
	////////////qDebug() << "### cell  OnPosition " << posi;
	       if (posi != -1) {
	        QTextCursor tmpcursor(_d);
					tmpcursor.setPosition(posi);
	        if ( !tmpcursor.currentTable() ) {
						return QTextTableCell();
					}
					return tmpcursor.currentTable()->cellAt(tmpcursor);
				 }  else {
					 return QTextTableCell();
				 }
}



QMimeData *ScribePage::createMimeDataFromSelection()
{
	 QTextCharFormat base = C_cursor.charFormat();
	 QString txt;
	 if (C_cursor.hasSelection()) {
		 txt = C_cursor.selectedText();
	 }
	 
	 QTextImageFormat pico = base.toImageFormat(); 
	 if (pico.isValid()) { 
		           QVariant xx = pico.property(_IMAGE_PICS_ITEM_); 
		           if (!xx.isNull()) {
								   SPics pic = xx.value<SPics>();
								   QList<SPics> li;
								                li.append(pic);
								   QString Sdd = SaveImageGroup(li);
								   QMimeData *mimeData = new QMimeData;
                   mimeData->setData("application/x-picslists",Sdd.toUtf8());
								   return mimeData;
							 }
							 
				     ///////const QString hrefadress = pico.name();
	 }
 
	const QTextDocumentFragment fragment(C_cursor);
	if ( fragment.isEmpty() && txt.size() > 0 ) {
		QMimeData *xm = new QMimeData();
		xm->setText(txt);
		return xm;
	} else if (fragment.isEmpty() && txt.isEmpty() ) {
		QMimeData *xm = new QMimeData();
		xm->setText(QString("...Selection...Error..."));
		return xm;
	} else {
	return new QTextEditMimeData(fragment);    /////QTextDocumentFragment::fromHtml(md->html())
	}
}


void ScribePage::InsertMimeDataOnCursor( const QMimeData *md )
{
	QTextDocumentFragment fragment;
	
	
	     if ( md->hasUrls() )  {
          QList<QUrl> urls = md->urls();
          for ( int i = 0; i < urls.size(); ++i ) { 
          QUrl gettyurl(urls.at(i));
						   /* window is slow to write  event is to fast */
				       //////////qDebug() << "### gettyurl " << gettyurl.toLocalFile();
						
              if (gettyurl.scheme() == "file") {
                  ImageonCursor(gettyurl.toLocalFile()); 
              } else if (gettyurl.scheme() == "http") {
										Gloader *grephttp = new Gloader();
										grephttp->Setting(this,i,gettyurl); 
										grephttp->start(QThread::LowPriority);
                  
              }
              
          }
					
				C_cursor.clearSelection();
        repaintCursor(true);
        return;          
      }
	
	
	
	
		
		if (md->hasFormat(QLatin1String("application/x-picslists")) ) {
			      QByteArray dd = md->data("application/x-picslists"); 
			      QList<SPics> li = OpenImageGroup(QString(dd));
			      for (int i=0; i<li.size(); i++) {
                 SPics conni = li[i];
							   RegisterImage(conni,true);
						}
			  C_cursor.clearSelection();
        repaintCursor(true);
			 return;
		}
		
		if ( md->hasImage() ) {
         QDateTime timer1( QDateTime::currentDateTime() );
         const QString TimestampsMs = QString("%1-%2-image").arg(timer1.toTime_t()).arg(timer1.toString("zzz"));
         QPixmap aspixmape = qvariant_cast<QPixmap>(md->imageData());
				 if (!aspixmape.isNull()) {
					insertPixmap(aspixmape);
				}
				C_cursor.clearSelection();
        repaintCursor(true);
				return;
    }
		
		
		QString cosa;
		bool hasData;
		
		if (md->hasFormat(QLatin1String("application/x-qrichtext")) ) {
			  cosa = md->data(QLatin1String("application/x-qrichtext"));
        // x-qrichtext is always UTF-8 (taken from Qt3 since we don't use it anymore).
        fragment = QTextDocumentFragment::fromHtml(QString::fromUtf8(md->data(QLatin1String("application/x-qrichtext"))));
        hasData = true;
			  //////////qDebug() << "### application/x-qrichtext ";
    } else if (md->formats().contains("text/html")) {
			  ///////////qDebug() << "### text/html";
			  cosa = QString::fromUtf8(md->data(QLatin1String("text/html")));
        fragment = QTextDocumentFragment::fromHtml(md->html());
			  //////////qDebug() << "### cosa " << cosa;
			  C_cursor.insertFragment(fragment);
        C_cursor.clearSelection();
        repaintCursor(true);
			  EnsureVisibleCursor();
			  return;
    } else if (md->formats().contains("text/plain")) {
			  cosa = QString::fromUtf8(md->data(QLatin1String("text/plain")));
        fragment = QTextDocumentFragment::fromPlainText(cosa);
			  ///////////qDebug() << "### text/plain";
        hasData = true;
    }
		
		
    if (hasData && cosa.size() > 0 ) {
        C_cursor.insertFragment(fragment);
        C_cursor.clearSelection();
        repaintCursor(true);
			  EnsureVisibleCursor();
		} else {
			QApplication::beep();
		}
		
}





void ScribePage::insertPixmap( QPixmap p )
{
        QPixmap scaledsimage;
        QTextFrame  *Tframe = _d->rootFrame();
        QTextFrameFormat rootformat= Tframe->frameFormat();
				const int margininside = rootformat.leftMargin() + rootformat.rightMargin() + rootformat.padding();
        int limitwiimage = Page_Width;
    
				 if (limitwiimage > boundingRect().width()) {
					  limitwiimage = boundingRect().width() - margininside ;
				 }
         
         if (p.width() > limitwiimage) {
             /* question widht */
					   bool ok;
             int iw = QInputDialog::getInteger(0, tr("Image to width in layer dimension!"),
                                      tr("Point:"), limitwiimage, 20, limitwiimage, 1, &ok);
					if (iw > 0 && ok) {
						scaledsimage = p.scaledToWidth( iw );
					} else {
					return;
					}
          
          
        } else {
           scaledsimage = p; 
        }
        
         QByteArray bytes;
         QBuffer buffer(&bytes);
         buffer.open(QIODevice::WriteOnly);
         if (_DRAWMODUS_WEB_ == 1) {
				 scaledsimage.save(&buffer,"JPG",70);
				 } else {
         scaledsimage.save(&buffer,"PNG",100);
				 }
        
         QDateTime timer1( QDateTime::currentDateTime() );
         const QString TimestampsMs = QString("%1-%2-image").arg(timer1.toTime_t()).arg(timer1.toString("zzz"));
				
         if (!scaledsimage.isNull()) {
         const QString nami = Imagename(TimestampsMs);
         SPics  xpix;
         xpix.name = nami;
				 if (_DRAWMODUS_WEB_ == 1) {
				 xpix.extension = QByteArray("JPG");
				 } else {
         xpix.extension = QByteArray("PNG");
				 }
				 /* bytes having data images */
				 xpix.set_pics(scaledsimage);
				 RegisterImage(xpix,true);
			   }
}




void  ScribePage::RegisterImage( SPics e , bool insert )
{
    QApplication::restoreOverrideCursor();
	  SPics base;
	
    bool ok;
	  if (e.info == base.info) {
       QString txtinfo = QInputDialog::getText(0, tr("Image Description to blind people."),tr("Description:"), QLineEdit::Normal,e.name, &ok);
       if (txtinfo.size() > 0) {
       e.info = txtinfo.left(110);
       }
	  } else {
			/* having desc  !!!  */
		}
		ApiSession *sx = ApiSession::instance();
    sx->ImagePageList.insert(e.name,e);
    _d->addResource(QTextDocument::ImageResource,QUrl(e.name),e.pix());
    if (insert) {
        QTextImageFormat format;
        format.setName( e.name );
        format.setHeight ( e.pix().height() );
        format.setWidth ( e.pix().width() );
        format.setToolTip(e.info);
        format.setProperty(_IMAGE_PICS_ITEM_,e);
        textCursor().insertImage( format );
    }
}


void  ScribePage::InsertImageonCursor()
{
    QString file = QFileDialog::getOpenFileName(0, tr( "Choose Image to insert..." ), QString(setter.value("LastDir").toString()) , ImageFilterHaving() );
    if ( file.isEmpty() ) {
    return;
    }
    setter.setValue("LastDir",file.left(file.lastIndexOf("/"))+"/");
    ImageonCursor(file);
}



void  ScribePage::ImageonCursor( const QString file )
{
     QDateTime timer1( QDateTime::currentDateTime() );
     const QString TimestampsMs = QString("%1-%2-image").arg(timer1.toTime_t()).arg(timer1.toString("zzz"));
     QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
     QFileInfo fixurl(file);
     const QString extension =  fixurl.completeSuffix().toLower();
     QByteArray              derangedata;
     QPixmap                 resultimage;
     QPixmap                 scaledsimage;
     QUrl                    imgresor;
     
        QFile f(fixurl.absoluteFilePath());
        if (f.open(QIODevice::ReadOnly)) {
            derangedata = f.readAll();
            f.close();
        }
  
        
        //////////////qDebug() << "### image" << derangedata.size();
        if (derangedata.size() < 1) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(0, tr( "Alert! image format." ), tr( "Unable to read file %1" ).arg(fixurl.fileName()) );  
        return;
        }
        /* read image */
        if (extension.contains("svg")) {
        resultimage = RenderPixmapFromSvgByte(  derangedata );
        } else if (extension.contains("ps") || extension.contains("eps")) {
            QApplication::restoreOverrideCursor();
            QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        resultimage = LoadPS( fixurl.absoluteFilePath() );
        } else if (extension.contains("pdf")) {
            QApplication::restoreOverrideCursor();
            int page = QInputDialog::getInteger(0, tr("Render Page Nr."),tr("Page:"),1, 1, 100, 1);
            int large = QInputDialog::getInteger(0, tr("Page scaled to width"),tr("Point unit:"),400, 10, 3000, 10);
            if (page > 0 && large > 0) {
                QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
                resultimage = LoadPDF( fixurl.absoluteFilePath() ,page,large);
            } else {
                QApplication::restoreOverrideCursor();
            return;
            }
        }  else {
        resultimage.loadFromData( derangedata );
        }
        
        /* read in */
        if ( resultimage.isNull() ) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(0, tr( "Alert! image format." ), tr( "Unable to render to image file %1 from type %2." ).arg(fixurl.fileName()).arg(extension) ); 
        return;
        }
        
         
         QByteArray bytes;
         QBuffer buffer(&bytes);
         buffer.open(QIODevice::WriteOnly);
         QApplication::restoreOverrideCursor();
				 int limitwiimage = Page_Width;
				 if (limitwiimage > boundingRect().width()) {
					 limitwiimage = boundingRect().width();
				 }
				if (resultimage.width() > limitwiimage) {
             /* question widht */
           scaledsimage = resultimage.scaledToWidth( limitwiimage );
        } else {
           scaledsimage = resultimage.scaledToWidth( resultimage.width() ); 
        }
				
				
				
        if (_DRAWMODUS_WEB_ == 1) {
				scaledsimage.save(&buffer,"JPG",70);
				} else {
        scaledsimage.save(&buffer,"PNG",100);
				}
				
        if (!scaledsimage.isNull()) {
        const QString nami = Imagename(fixurl.baseName());
        SPics  xpix;
        xpix.name = nami;
				if (_DRAWMODUS_WEB_ == 1) {
				xpix.extension = QByteArray("JPG");
				} else {
        xpix.extension = QByteArray("PNG");
				}
				/* bytes having data images */
				xpix.set_pics(scaledsimage);
				
				RegisterImage(xpix,true);
				}
        QApplication::restoreOverrideCursor();
}








QString ScribePage::ImageFilterHaving() const
{
  QString filter;
  filter = tr( "All supported Types" ) + " (";
  QList<QByteArray> formats = QImageReader::supportedImageFormats();
  for ( int x = 0; x < formats.count(); ++x ) {

    filter += QString( "*.%1" ).arg( QString( formats[ x ] ) );
    if ( x != formats.count() - 1 )
      filter += " ";
  }
  filter += ");;";
  QString filterSimple;
  double gsversion = getGSVersion();
  //////qDebug() << "### args " << gsversion;
  filterSimple += tr( "Scalable Vector Graphics" ) + " (*.svg *.svg.gz);;";
  if (gsversion > 6.5) {
   filterSimple += tr( "PostScript Vector Graphics" ) + " (*.ps *.eps);;"; 
   filterSimple += tr( "PDF Page units" ) + " (*.pdf);;"; 
  }
  
  for ( int y = 0; y < formats.count(); ++y ) {

    QString form( formats[ y ] );
    if ( form == "bmp" )
      filterSimple += tr( "Windows Bitmap" ) + " (*.bmp)";
    else if ( form == "gif" )
      filterSimple += tr( "Graphic Interchange Format" ) + " (*.gif)";
    else if ( form == "jpeg" || form == "jpg" ) {

      if ( !filterSimple.contains( form ) )
        filterSimple += tr( "Joint Photographic Experts Group" ) + " (*.jpeg *.jpg)";
    } else if ( form == "mng" )
      filterSimple += tr( "Multiple-image Network Graphics" ) + " (*.mng)";
    else if ( form == "png" )
      filterSimple += tr( "Portable Network Graphics" ) + " (*.png)";
    else if ( form == "pbm" || form == "ppm" ) {

      if ( !filterSimple.contains( form ) )
        filterSimple += tr( "Portable Bitmap" ) + " (*.pbm *.ppm)";
    } else if ( form == "pgm" )
      filterSimple += tr( "Portable Graymap" ) + " (*.pgm)";
    else if ( form == "xbm" || form == "xpm" ) {

      if ( !filterSimple.contains( form ) )
        filterSimple += tr( "X11 Bitmap" ) + " (*.xbm *.xpm)";
    } else if ( form == "ico" )
      filterSimple += tr( "Icon Image File Format" ) + " (*.ico)";
    else if ( form == "jp2" || form == "j2k" ) {

      if ( !filterSimple.contains( form ) )
        filterSimple += tr( "JPEG 2000" ) + " (*.jp2 *.j2k)";
    } else if ( form == "tif" || form == "tiff" ) {

      if ( !filterSimple.contains( form ) )
        filterSimple += tr( "Tagged Image File Format" ) + " (*.tif *.tiff)";
    } else
      filterSimple += tr( "Unknown Format" ) + QString( " (*.%1)" ).arg( form );

    if ( y != formats.count() - 1 && !filterSimple.endsWith( ";;" ) )
      filterSimple += ";;";
  }
  filterSimple = filterSimple.left( filterSimple.length() - 2 );
  filter += filterSimple;
  return filter;
}



void ScribePage::in_image( int id )
{
	   QApplication::restoreOverrideCursor();
	   LoadGetImage *ht = qobject_cast<LoadGetImage *>(sender());
	   if (ht) {
	   QPixmap  imagen = ht->pics();
	   insertPixmap(imagen);
		 }
}



void ScribePage::showhtml()
{
	XMLTextEdit *sHtml = new XMLTextEdit(0);
	sHtml->setWindowFlags ( Qt::Window );
	
	QDomDocument *fh = new QDomDocument();
	if (fh->setContent (_d->toHtml("utf-8"),false)) {
		sHtml->setPlainText( fh->toString(1) );
	} else {
		sHtml->setPlainText( "Not conform!" );
	}
	sHtml->show();
	
	
}











