# Fop MiniScribus XSL-FO #
> ## **(latest stable version 1.0.3)** ##
XSL Formatting Objects markup language WYSIWYG editor (xsl-fo).
XML document formatting which is most often used to generate PDFs.

[![](http://fop-miniscribus.googlecode.com/svn/trunk/fop_miniscribus.2.0.0/icon/foedit_45x45.png)](http://www.qt-apps.org/content/show.php/Fop+MiniScribus+XSL-Fo?content=67552)

Release the new MiniScribus Scribe forks version **2.0.0 beta**
Avaiable on SVN source or a win32 build on download section.
Use 7-zip  http://www.7-zip.org/ to open the win32 build it have a bz compression method
default window zip can not handle it.

## Build from source: ##
  1. Macx or linux

```
 /*  only libxslt libxml2 libiconv are need as external lib an QT version 4.4.1 > */
 svn co http://fop-miniscribus.googlecode.com/svn/trunk/fop_miniscribus.2.0.0/ scribe
 cd scribe
 ./configure
 make && make install
 /* after run foedit */
```

## Apache FOP (Formatting Objects Processor) install win 32/64: ##
To install java FOP installer now available at http://code.google.com/p/qtcocoon/downloads/list at end set PATH on your envoirment.



[Linux install](Gentoo.md)

Gentoo  ebuild http://bugs.gentoo.org/show_bug.cgi?id=237043



[Install on window](QT_Trolltech.md)

## Features: ##
  1. Many KeyBoard  shortcut to edit document
  1. PDF Bookmarks are rendered only on [Apache FOP (Formatting Objects Processor)](Apache.md) (Formatting Objects Processor) major as 0.90
  1. Import PDF/PS as Image if Ghostscript is on your PC
  1. Image Tiff , Jpeg , Gif , PNG


### Read only format: ###
  1. OpenOffice 1.1 file format **.sxw** **.stw**
  1. OpenOffice 2.4 file format **.odt** **.ott**
  1. As import xslt transform
  1. XHTML file format **.htm** **.html**

### Read and write format: ###
  1. FOP file **.fop** **.fop.gz** **.fo** **.fo.gz** **.xml**
  1. MiniScribus binary stream file **.page Internal binary format to not loast nothing on beta tester is a QTextDocument serializer
  1. Later i implement -mif|-rtf|-tiff|-png|-pcl |-ps|-txt| [Apache FOP](Apache.md) can make**



It can read and edit 95% from all sample file from Apache fop project
Only table / cell border and markers and page footer note not is possibel to display at this moment.

**Supported tag are:**
### page format ###
  1. fo:region-before
  1. fo:region-after
  1. fo:region-start
  1. fo:region-end

### PDF BookMark: ###
fo:bookmark-tree fo:bookmark


fo:simple-page-master
fo:block fo:table table-footer table-header fo:table-body fo:table-column fo:table-row fo:table-cell fo:block-container fo:inline fo:inline-container fo:basic-link fo:external-graphic fo:instream-foreign-object fo:list-block fo:list-item fo:list-item-body fo:list-item-label fo:root fo:character fo:leader fo:footnote fo:page-number-citation fo:page-number fo:float


The Application read all this Attributes on fo:block-container:

"position" "top" "left" "width" "min-height" "degree-rotation" "rotate" "reference-orientation" "opacity" "height" "background-color" "z-index" "id" "border-width" "border-color" "absolute-position" "top" "right" "bottom" "left" "background-attachment" "background-image" "background-repeat" "background-position-horizontal" "background-position-vertical" "border-before-color" "border-before-style" "border-before-width" "border-after-color" "border-after-style" "border-after-width" "border-start-color" "border-start-style" "border-start-width" "border-end-color" "border-end-style" "border-end-width"  "border-top-color" "border-top-style" "border-top-width" "border-bottom-color"
"border-bottom-style" "border-bottom-width" "border-left-color" "border-left-style"
"border-left-width" "border-right-color" "border-right-style" "border-right-width"
"padding-before" "padding-after" "padding-start" "padding-end" "padding-top" "padding-bottom" "padding-left" "padding-right" "margin-top" "margin-bottom" "margin-left" "margin-right" "space-before" "space-after" "start-indent" "end-indent" "block-progression-dimension" "break-after" "break-before" "clip" "display-align" "height" "id" "inline-progression-dimension" "intrusion-displace" "keep-together" "keep-with-next" "keep-with-previous" "overflow"  "border-style" "docversion" "l-lock"

If the attribute are know it handle and draw its, otherwise it resave the original attribute to not destroy document layer.

Bugs report please send it!
Bugs are unexpected defects, faults, flaws, or imperfections in a computer program. MiniScribus Software wants to know about all defects that may be encountered in the XSL-FO editor.

The are two way:
If you having a Gmail account go to http://code.google.com/p/fop-miniscribus/issues/list and write a new Issues, and write what steps will reproduce the problem? Window version ....

If you dont have a Gmail account report defects on PDF Form Ticket
http://ciz.ch/Ticket_Form.pdf .


**Know Bugs:**
Render or display more than 5-6 pages. QTextDocument has performance issues to
render and display it fast. A workaround is to slice/chapter your text and compose
xsl-fo source to a book or more as 100 pages and render on Apache FOP (Formatting
Objects Processor) to create professional PDF page
http://trolltech.com/developer/task-tracker/index_html?method=entry&id=135240

**Developer note:**
A component called QTextPanel that provides paginated text editing  is available on SVN
http://fop-miniscribus.googlecode.com/svn/trunk/QTextPanel/
Thanks to Luiz (http://www.qtcentre.org/forum/u-netuno-7350.html) who helped me to
make this sub-project (initially forked from MiniScribus).


### Changelog: ###
  1. 31.08.2008 release the first MiniScribus Scribe forks source
beta version 2.0.0
  1. 11.10.07 version 0.5.8 / Set table cell / paragraph to default 0 / not 12pt top and 12 bottom space from default QTextDocument Set only selectable active item at top zindex/zvalue on the active QGraphicsScene ( append subclass GraphicsScene) on this case not having intersection problem from Floating Layer.
  1. 14.10.07 version 0.6.1 enable to open xsl-fo file without block-container (on prepend page as on floating container helpfull to copy text and create new pages), draw page margin inside the work-area, append the fo:character tag    # 09.11.07 version 0.8.1 Fix Render PDF on QTextDocument Fontmetric, append US-Letter format
  1. 29.11.07 version 1.0.0  append main argument to print direct pdf file from fop xample:  ./MiniScribus -pdf destination.pdf sourcefile.fop other programm can access to pdf result and use miniscribus as template to invoice or other, other beta DB label programm Export fop file to XHTML ready ,  only rotate layer not possibel on html css. HTML export background-color from layer write opacity:%1; filter: alpha(opacity=%1); -moz-opacity:%1;  if alpha color chanel is more as 44. Prepare setup to import PostScript image on GhostScript. ref. http://qtforum.de/forum/viewtopic.php?t=5517 and generate barcode.
  1. 29.12.07  version 1.0.3  Handle font point size more as 99 , optimize GhostScript controll and better start of friend programm direct pdf render (rotate elements on 500 milli.sek) + make source compatible on MS Visual Studio 2008 beta2

### Donation ###
The software that I have written, and will continue to write. I love to write software, and if you have found it helpful, then you are most welcome. I will not actively solicit donations, but if you would like to make one, then note that a significant portion will, in turn, be forwared to the Apache Software Foundation. Any donations that you may choose to make are appreciated, on behalf of myself and the charities that I have associated with my account. [Thank four you help](http://sourceforge.net/project/project_donations.php?group_id=202496) & to push this software to stable.




[![](http://fop-miniscribus.googlecode.com/svn/trunk/fop_miniscribus.2.0.0/miniscribus2.0.0.png)](http://www.qt-apps.org/content/show.php/Fop+MiniScribus+XSL-Fo?content=67552)


