################################################################################
## OCS Inventory NG
## Copyleft 2008 OCS Inventory NG Team
## Web : http://www.ocsinventory-ng.org
##
## This code is open source and may be copied and modified as long as the source
## code is always made freely available.
## Please refer to the General Public Licence http://www.gnu.org/ or Licence.txt
################################################################################

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "OCS Inventory NG Agent Deployment Tool"
!define PRODUCT_VERSION "2.3.0.0"
!define PRODUCT_PUBLISHER "OCS Inventory NG"
!define PRODUCT_WEB_SITE "http://www.ocsinventory-ng.org"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\ocsinventory-deploy-tool.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"

SetCompressor bzip2

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "install-ocs.ico"
!define MUI_UNICON "uninstall-ocs.ico"
!define MUI_HEADERIMAGE_BITMAP "OCS-NG-48.bmp" ; optional

!define MUI_HEADERIMAGE
!define MUI_WELCOMEPAGE_TITLE_3LINES
; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
;!define MUI_LICENSEPAGE_CHECKBOX
!insertmacro MUI_PAGE_LICENSE "LICENSE.txt"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Start menu page
var ICONS_GROUP
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "OCS Inventory NG\Agent Deployment Tool"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${PRODUCT_STARTMENU_REGVAL}"
!insertmacro MUI_PAGE_STARTMENU Application $ICONS_GROUP
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\ocsinventory-deploy-tool.exe"
!define MUI_FINISHPAGE_RUN_TEXT "Start ${PRODUCT_NAME}"
!define MUI_FINISHPAGE_TITLE_3LINES
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French"

; Reserve files
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

; MUI end ------

  VIProductVersion "${PRODUCT_VERSION}"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "${PRODUCT_NAME}"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "Comments" "Help pushing OCS Inventory NG Agent setup under Windows and Unix computers."
  VIAddVersionKey /LANG=${LANG_ENGLISH} "CompanyName" "${PRODUCT_PUBLISHER}"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalTrademarks" "${PRODUCT_NAME} is a part of OCS Inventory NG product. Distributed under GNU GPL Licence."
  VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" "${PRODUCT_PUBLISHER} ${PRODUCT_WEB_SITE}"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "${PRODUCT_NAME}"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" "${PRODUCT_VERSION}"

BRANDINGTEXT "OCS Inventory NG"
Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "OCS-NG-Agent-Deployment-Tool-Setup.exe"
InstallDir "$PROGRAMFILES\OCS Inventory NG Agent Deployment Tool"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails hide
ShowUnInstDetails hide

#####################################################################
# This function display spash screen
#####################################################################
Function .onInit
    ; Initializing plugins dir and extracting custom option pages
	InitPluginsDir
	File /oname=$PLUGINSDIR\splash.bmp "banner-ocs.bmp"
	advsplash::show 1000 500 1000 -1 $PLUGINSDIR\splash
FunctionEnd


Section "Main Files" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  ; MSVC 12 CRT redist
  ; File "C:\Windows\System32\msvcm120.dll"
  File "C:\Windows\System32\msvcp120.dll"
  File "C:\Windows\System32\msvcr120.dll"
  ; MSVC 12 MFC redist
  File "C:\Windows\System32\mfc120.dll"
  File "C:\Windows\System32\mfc120u.dll"
  File "C:\Windows\System32\mfcm120.dll"
  File "C:\Windows\System32\mfcm120u.dll"
  ; ocsinventory-deploy-tool
  File "..\Release\ocsinventory-deploy-tool.exe"
  File "modules.conf"
  File "setup.sh"
  File "LICENSE.txt"
  File "PuTTY_LICENCE.txt"

; Shortcuts
  ; Add icon group to All Users
  SetShellVarContext all
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  CreateDirectory "$SMPROGRAMS\$ICONS_GROUP"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\OCS Inventory NG Agent Deployment Tool.lnk" "$INSTDIR\ocsinventory-deploy-tool.exe"
  CreateShortCut "$DESKTOP\OCS Inventory NG Agent Deployment Tool.lnk" "$INSTDIR\ocsinventory-deploy-tool.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section -AdditionalIcons
  ; Add icon group to All Users
  SetShellVarContext all
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  WriteIniStr "$INSTDIR\${PRODUCT_PUBLISHER}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\OCS Inventory NG Website.lnk" "$INSTDIR\${PRODUCT_PUBLISHER}.url"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk" "$INSTDIR\uninst.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\ocsinventory-deploy-tool.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\ocsinventory-deploy-tool.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully uninstalled from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Do you really want to uninstall $(^Name) ?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  !insertmacro MUI_STARTMENU_GETFOLDER "Application" $ICONS_GROUP
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\setup.sh"
  Delete "$INSTDIR\modules.conf"
  Delete "$INSTDIR\ocsinventory-deploy-tool.exe"
  ; MSVC 12 CRT redist
  Delete "$INSTDIR\msvcm120.dll"
  Delete "$INSTDIR\msvcp120.dll"
  Delete "$INSTDIR\msvcr120.dll"
  ; MSVC 12 MFC redist
  Delete "$INSTDIR\mfc120.dll"
  Delete "$INSTDIR\mfc120u.dll"
  Delete "$INSTDIR\mfcm120.dll"
  Delete "$INSTDIR\mfcm120u.dll"

  ; Remove icon group to All Users
  SetShellVarContext all
  Delete "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\OCS Inventory NG Website.lnk"
  Delete "$DESKTOP\OCS Inventory NG Agent Deployment Tool.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\OCS Inventory NG Agent Deployment Tool.lnk"

  RMDir "$SMPROGRAMS\$ICONS_GROUP"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd