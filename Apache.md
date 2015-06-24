# Apache Fop #

From commandline it can transform xsl-fo file to pdf rtf tif ps and pcl
The Print Preview is only a symple command " Fop foo.fo -awt "

```


USAGE
Fop [options] [-fo|-xml] infile [-xsl file] [-awt|-pdf|-mif|-rtf|-tiff|-png|-pcl
|-ps|-txt|-at [mime]|-print] <outfile>
 [OPTIONS]
  -d                debug mode
  -x                dump configuration settings
  -q                quiet mode
  -c cfg.xml        use additional configuration file cfg.xml
  -l lang           the language to use for user information
  -r                relaxed/less strict validation (where available)
  -dpi xxx          target resolution in dots per inch (dpi) where xxx is a number
  -s                for area tree XML, down to block areas only
  -v                to show FOP version being used

  -o [password]     PDF file will be encrypted with option owner password
  -u [password]     PDF file will be encrypted with option user password
  -noprint          PDF file will be encrypted without printing permission
  -nocopy           PDF file will be encrypted without copy content permission
  -noedit           PDF file will be encrypted without edit content permission
  -noannotations    PDF file will be encrypted without edit annotation permission
  -pdfprofile prof  PDF file will be generated with the specified profile
                    (Examples for prof: PDF/A-1b or PDF/X-3:2003)

 [INPUT]
  infile            xsl:fo input file (the same as the next)
  -fo  infile       xsl:fo input file
  -xml infile       xml input file, must be used together with -xsl
  -atin infile      area tree input file
  -imagein infile   image input file
  -xsl stylesheet   xslt stylesheet

  -param name value <value> to use for parameter <name> in xslt stylesheet
                    (repeat '-param name value' for each parameter)

 [OUTPUT]
  outfile           input will be rendered as PDF into outfile
  -pdf outfile      input will be rendered as PDF (outfile req'd)
  -pdfa1b outfile   input will be rendered as PDF/A-1b compliant PDF
                    (outfile req'd, same as "-pdf outfile -pdfprofile PDF/A-1b")

  -awt              input will be displayed on screen
  -rtf outfile      input will be rendered as RTF (outfile req'd)
  -pcl outfile      input will be rendered as PCL (outfile req'd)
  -ps outfile       input will be rendered as PostScript (outfile req'd)
  -afp outfile      input will be rendered as AFP (outfile req'd)
  -tiff outfile     input will be rendered as TIFF (outfile req'd)
  -png outfile      input will be rendered as PNG (outfile req'd)
  -txt outfile      input will be rendered as plain text (outfile req'd)
  -at [mime] out    representation of area tree as XML (outfile req'd)
                    specify optional mime output to allow AT to be converted
                    to final format later
  -print            input file will be rendered and sent to the printer
                    see options with "-print help"
  -out mime outfile input will be rendered using the given MIME type
                    (outfile req'd) Example: "-out application/pdf D:\out.pdf"
                    (Tip: "-out list" prints the list of supported MIME types)
  -mif outfile      input will be rendered as MIF (FrameMaker) (outfile req'd)
                    Experimental feature - requires additional fop-sandbox.jar.
  -svg outfile      input will be rendered as an SVG slides file (outfile req'd)

                    Experimental feature - requires additional fop-sandbox.jar.

  -foout outfile    input will only be XSL transformed. The intermediate
                    XSL-FO file is saved and no rendering is performed.
                    (Only available if you use -xml and -xsl parameters)


 [Examples]
  Fop foo.fo foo.pdf
  Fop -fo foo.fo -pdf foo.pdf (does the same as the previous line)
  Fop -xml foo.xml -xsl foo.xsl -pdf foo.pdf
  Fop -xml foo.xml -xsl foo.xsl -foout foo.fo
  Fop bookmarksample2.fo  bookmarksample2.pdf
  Fop foo.fo -rtf foo.rtf
  Fop foo.fo -print or Fop -print foo.fo
  Fop foo.fo -awt


```





