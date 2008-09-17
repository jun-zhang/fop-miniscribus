#include <QtCore>
#include <QString>
#include <QtDebug>
#include <QDebug> 
#include <QApplication>
#include <QXmlQuery>
#include <QtGui>

/* read the contenet of a local file as qstring */
static inline QString ReadFileUtf8Xml( const QString fullFileName )
{
    QString inside = "";
    QFile file(fullFileName); 
    if (file.exists()) {
                if (file.open(QFile::ReadOnly | QFile::Text)) {
                    inside = QString::fromUtf8(file.readAll());
                    file.close();
                }
    }

return inside;
}


int main(int argc, char *argv[]) {
    QApplication a( argc, argv );
   
    qDebug() << "### init main void";
    QDateTime timer1( QDateTime::currentDateTime() ); 
    const QString localoutfile =  "outresult.html";
    
    
    QXmlQuery xquery(QXmlQuery::XSLT20);
    xquery.setFocus(QUrl("http://fop-miniscribus.googlecode.com/svn/trunk/doc/Xsltqt5/data.xml"));
    xquery.bindVariable("unixtime", QVariant(timer1.toTime_t()));
    xquery.setQuery(QUrl("http://fop-miniscribus.googlecode.com/svn/trunk/doc/Xsltqt5/style.xsl"));
    QFile out(localoutfile);
    out.open(QIODevice::WriteOnly);
    xquery.evaluateTo(&out);
    out.close();
    qDebug() << "### close file transform ";
    QTextDocument *d = new QTextDocument();
    d->setHtml ( ReadFileUtf8Xml(localoutfile) );
    QTextEdit t;
    t.show();
    t.setDocument ( d );
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
};