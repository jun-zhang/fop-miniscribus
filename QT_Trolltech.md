# QT Install Instruction Window OS #

  * First get Git  http://git.or.cz/#download  win32 or else binary
  * Install Subversion on window get http://tortoisesvn.tigris.org/
  * Read the basic Git guide http://linux.yyz.us/git-howto.html  http://www.sourcemage.org/Git_Guide
  * Open a Git Shell and cd to path to build

```

git clone --depth 1 git://labs.trolltech.com/qt-snapshot
## to update if new source is avaiable write on the same folder
git pull


```



  * Install OpenSSL to having remote access on secure server

Download the latest OpenSSL source (you can find the latest source at: http://www.openssl.org/source/ , get the one that is named " openssl-0.9..tar.gz". Unpack it to c:\dev\ (it will create a directory called openssl-x.y.z)
Download and install ActivePerl. You can find it here: http://www.activestate.com/ chose free item.
Only window ++ VS Search a file called MASMsetup.EXE result url must contains ww.microsoft.com/download and install this ms Assembler


  * Open a new shell visual studio (so that %PATH% includes ActivePerl).


```

   cd \dev
   cd openssl<Press tab and it will autocomplete to, as of this writing, openssl-0.9.8h
   perl Configure VC-WIN32 --prefix=c:\\dev\\OpenSSL
   ms\do_masm
   nmake -f ms\ntdll.mak
   nmake -f ms\ntdll.mak install 

```

  * Create or change your visual studio start bat envoirment vs.bat

```
@echo off
SET QTDIR=C:\dev\qt_4.4.1vs
SET MYSQL=c:\MySQL
SET OPENSSL=C:\dev\OpenSSL
CALL "C:\dev\vs\VC\vcvarsall.bat" x86
SET INCLUDE=C:\dev\ms_sdk\Include;%OPENSSL%\include;%OPENSSL%\include\openssl;C:\dev\externallib\include;%INCLUDE%
SET LIB=C:\dev\ms_sdk\Lib;%OPENSSL%\lib;C:\dev\externallib\lib;%LIB%
SET PATH=%QTDIR%\bin;%OPENSSL%\bin;%MYSQL%\lib\opt;C:\dev\cmake\bin;%PATH%
  

```

  * And now build QT

```
 configure  -openssl  *other option*
```

> Remember only cd to qtdir and git pull to get only the difference from new source code
> if new code is incomming only write nmake, and you are update.
> Now to build Miniscribus create a new dir and right click SVN Checkout
> enter http://fop-miniscribus.googlecode.com/svn/trunk/fop_miniscribus.2.0.0/ open a     shell on this dir and build (  qmake and at end  nmake )








