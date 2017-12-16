!include "MUI2.nsh"
!include WinMessages.nsh

Name "CoolExplorer"
!define VERSION "0.7[build 245]"
!define APP_NAME fct
!define WND_TITLE "CoolExplorer"
!define termMsg "Installer cannot stop running ${WND_TITLE}.$\nClick YES to terminate application."

OutFile "CoolExplorer_${VERSION}.exe"
InstallDir "$PROGRAMFILES\CoolExplorer"
LicenseData ".\u_rbin\license.txt"


!insertmacro MUI_LANGUAGE "English" ;first language is the default language
!insertmacro MUI_LANGUAGE "Polish"
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_RESERVEFILE_LANGDLL

;!define MUI_HEADERIMAGE
;!define MUI_HEADERIMAGE_BITMAP "$INSTDIR\banner_install.bmp" ; optional
!define MUI_ABORTWARNING

;LoadLanguageFile "${NSISDIR}\Contrib\Language Files\Polish.nlf"
Page license
Page directory
Page instfiles
UninstPage uninstConfirm
UninstPage instfiles

LangString Name ${LANG_ENGLISH} "English"
LangString Name ${LANG_POLISH} "Polish"
LangString Name ${LANG_GERMAN} "German"

Section ""
    Delete $INSTDIR\lang\* ; delete self (see explanation below why this works)
	SetOutPath $INSTDIR
	File /r ".\u_rbin\CoolExplorer.exe"
;	File /r ".\u_rbin\CoolViewer.dll"
	File /r ".\u_rbin\license.txt"
	File /r ".\u_rbin\ReadMe.rtf"
	File /r ".\u_rbin\ReleaseNote.txt"
	SetOutPath $INSTDIR\lang
	File /r ".\u_rbin\lang\English.lng"
	File /r ".\u_rbin\lang\Polish.lng"
    File /r ".\u_rbin\lang\German by Vacon.lng"
SetOutPath $INSTDIR
;shortcut on desktop

CreateShortCut "$DESKTOP\CoolExplorer.lnk" "$INSTDIR\CoolExplorer.exe" 
CreateShortCut "$QUICKLAUNCH\CoolExplorer.lnk" "$INSTDIR\CoolExplorer.exe" 
CreateDirectory "$SMPROGRAMS\CoolExplorer"
CreateShortCut "$SMPROGRAMS\CoolExplorer\CoolExplorer.lnk" "$INSTDIR\CoolExplorer.exe" \
  "" "$INSTDIR\CoolExplorer.exe" 0 SW_SHOWNORMAL 
CreateShortCut "$SMPROGRAMS\CoolExplorer\uninstaller.lnk" "$INSTDIR\uninstaller.exe" \
  "" "$INSTDIR\uninstaller.exe" 0 SW_SHOWNORMAL 



;WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Product" "VERSION" "CoolExplorer"
WriteUninstaller $INSTDIR\uninstaller.exe
SectionEnd

Section "Uninstall"

  Delete $INSTDIR\lang\* ; delete self (see explanation below why this works)
  Delete $INSTDIR\uninstaller.exe ; delete self (see explanation below why this works)
  Delete $INSTDIR\* ; delete self (see explanation below why this works)
  RMDir $INSTDIR\lang
  RMDir $INSTDIR
  Delete "$DESKTOP\CoolExplorer.lnk"
  Delete "$QUICKLAUNCH\CoolExplorer.lnk"
  Delete "$SMPROGRAMS\CoolExplorer\*"
  RMDir  "$SMPROGRAMS\CoolExplorer"
  
SectionEnd

;--------------------------------------------------------------
Function .onInit

!insertmacro MUI_LANGDLL_DISPLAY

System::Call 'kernel32::CreateMutexA(i 0, i 0, t "myMutex") i .r1 ?e'
 Pop $R0
 
 StrCmp $R0 0 +3
   MessageBox MB_OK|MB_ICONEXCLAMATION "The installer is already running."
   Abort

   InitPluginsDir
   ;SetOutPath $TEMP
		
	fct::fct /QUESTION '${termMsg}' /WT '${WND_TITLE}' /TIMEOUT 2000 
	fct::wait
    	
		
	Pop $0 ; $0 has '1' if the user closed the splash screen early,
			; '0' if everything closed normally, and '-1' if some error occurred.

   
FunctionEnd

Function un.onInit

  !insertmacro MUI_UNGETLANGUAGE
  
FunctionEnd

Function .onInstSuccess
;copy polish/ or default eglish language
FunctionEnd