################################################
# Make file for CASIO fx-9860G SDK Addin
#
############################
# Directory defines
TCDIR = D:\CASIO\casio\OS\SH
OSDIR = D:\CASIO\casio\OS
APPDIR = D:\DOCUMENTS\Raphael\casio\code\SDK\projects\ConnectFX
OUTDIR = D:\DOCUMENTS\Raphael\casio\code\SDK\projects\ConnectFX\Debug

################
# Main Defines
SH_EXEDIR=$(TCDIR)\bin

# Hitachi SH C/C++ Compiler02 phase
SHCC02_EXE=shc.exe
SHCC02_DEP="$(OSDIR)\FX\include\fxlib.h" 

# Hitachi SH Assembler03 phase
SHASM03_EXE=asmsh.exe

# Hitachi OptLinker04 phase
SHLINK04_EXE=Optlnk.exe
SHLINK04_DEP="$(OSDIR)\FX\lib\fx9860G_library.lib"
SHLINK04_DEP2="$(OSDIR)\FX\lib\setup.obj"

#######################
# Files to build
FILE0=Test
FILESRC0="$(APPDIR)\$(FILE0).c"
FILEOBJ0="$(OUTDIR)\$(FILE0).obj"
FILE1=syscall
FILESRC1="$(APPDIR)\$(FILE1).src"
FILEOBJ1="$(OUTDIR)\$(FILE1).obj"
RFILE=FXADDINror
USERALLOBJ=$(FILEOBJ0) $(FILEOBJ1)

#######################
# nmake "all" statement

ALL:  SH_ENV \
	$(USERALLOBJ) \
	$(OUTDIR)\$(RFILE).bin \

####################
# Description blocks

!MESSAGE %3#C$z`&'0?
!MESSAGE
!MESSAGE Executing Hitachi SH C/C++ Compiler/Assembler phase
!MESSAGE

SH_ENV : 
	set SHC_INC=$(TCDIR)\include
	set PATH=$(TCDIR)\bin
	set SHC_LIB=$(TCDIR)\bin
	set SHC_TMP=$(OUTDIR)

$(FILEOBJ0) : $(FILESRC0) $(SHCC02_DEP)
	"$(SH_EXEDIR)\$(SHCC02_EXE)" -subcommand=<<
-cpu=sh3
-include="$(OSDIR)\FX\include","$(APPDIR)"
-objectfile=$(FILEOBJ0)
-show=source
-listfile="$(OUTDIR)\$(FILE0).lst"
-size
-noinline
-chgincpath
-errorpath
$(FILESRC0)
-lang=c
-nologo
-debug
<<

$(FILEOBJ1) : $(FILESRC1)
	"$(SH_EXEDIR)\$(SHASM03_EXE)" -subcommand=<<
$(FILESRC1)
-cpu=sh3
-endian=big
-round=zero
-denormalize=off
-include="$(APPDIR)"
-include="$(OSDIR)\FX\include"
-debug
-object=$(FILEOBJ1)
-literal=pool,branch,jump,return
-nologo
-chgincpath
-errorpath
<<

!MESSAGE
!MESSAGE Executing Hitachi OptLinker04 phase
!MESSAGE

"$(OUTDIR)\$(RFILE).bin" : $(USERALLOBJ) $(SHLINK04_DEP2) $(SHLINK04_DEP)
	"$(SH_EXEDIR)\$(SHLINK04_EXE)" -subcommand=<<
noprelink
sdebug
rom D=R
nomessage 
list "$(OUTDIR)\$(RFILE).map"
show symbol
nooptimize
start P_TOP,P,C,D,C$VTBL,C$INIT/0300200,B_BR_Size,B,R/08100000
fsymbol P
nologo
input $(USERALLOBJ)
input $(SHLINK04_DEP2)
library $(SHLINK04_DEP)
output "$(OUTDIR)\$(RFILE).abs"
-nomessage=1100
end
input "$(OUTDIR)\$(RFILE).abs"
form binary
output "$(OUTDIR)\$(RFILE).bin"
exit
<<

