# Introduction #

MiniScribus implements the bookmarks as defined by the latest W3C Working Draft for XSL-FO 1.1.

The fo:bookmark-tree formatting object is used to hold list of access points within the document such as a table of contents, a list of figures or tables, etc. Each access point is called a bookmark. The fo:bookmark object is used to identify an access point, and to specify where that access point is within the current document or another external document. A given bookmark may be further subdivided into a sequence of (sub-)bookmarks to as many levels as the authors desire:

```
<fo:root xmlns:fo="http://www.w3.org/1999/XSL/Format">
    <fo:layout-master-set>
        <fo:simple-page-master master-name="LetterPage" margin="1in">
            <fo:region-body region-name="PageBody"/>
        </fo:simple-page-master>
    </fo:layout-master-set>
    <fo:bookmark-tree >
        <fo:bookmark internal-destination="toc" >
            <fo:bookmark-title >Bookmarks Example</fo:bookmark-title>
            <fo:bookmark internal-destination="chapter1">
                <fo:bookmark-title>Hello World</fo:bookmark-title>
            </fo:bookmark>
            <fo:bookmark internal-destination="chapter2">
                <fo:bookmark-title>Paragraphs</fo:bookmark-title>
            </fo:bookmark>
        </fo:bookmark>
    </fo:bookmark-tree>
    <fo:page-sequence master-reference="LetterPage" font="10pt Arial">
         <fo:title>MiniScribus ver. 2.0.0 - BookMark sample</fo:title>
        <fo:flow flow-name="PageBody" font-family="Arial Narrow" font-size="10pt">
        <fo:block id="toc" >Table of contents</fo:block>
        ...
        </fo:flow>
    </fo:page-sequence>
</fo:root>
```