Name "Project Crimson Alpha 1.2.0"

OutFile "crimson_alpha_1_2_0_win_setup_SLCOPUB.exe"

InstallDir "$APPDATA\HarpNet Studios\Project Crimson Alpha"

InstallDirRegKey HKLM "Software\CrimsonAlpha" "Install_Dir"

SetCompressor /SOLID lzma
XPStyle on

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

Section "Project Crimson Alpha (required)"

  SectionIn RO

  SetOutPath $INSTDIR

  File /r /x src /x home /x .vs /x bin_unix /x res /x scripts "..\..\*.*"

  WriteRegStr HKLM "SOFTWARE\CrimsonAlpha" "Install_Dir" "$INSTDIR"

  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CrimsonAlpha" "DisplayName" "Project Crimson Alpha"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CrimsonAlpha" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CrimsonAlpha" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CrimsonAlpha" "NoRepair" 1
  WriteUninstaller "uninstall.exe"

  IfFileExists "$DOCUMENTS\My Games\Project Crimson Alpha\config.cfg" ConfigFound NoConfig
  ConfigFound:
     Delete "$DOCUMENTS\My Games\Project Crimson Alpha\old-config.cfg"
     Rename "$DOCUMENTS\My Games\Project Crimson Alpha\config.cfg" "$DOCUMENTS\My Games\Project Crimson Alpha\old-config.cfg"
  NoConfig:

SectionEnd

Section "Desktop Shortcut"

    CreateShortCut "$DESKTOP\Project Crimson Alpha.lnk" "$INSTDIR\crimson.bat" "" "$INSTDIR\bin64\cardboard-MGW.exe" 0 SW_SHOWMINIMIZED

SectionEnd

Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\Project Crimson Alpha"

  SetOutPath "$INSTDIR"

  CreateShortCut "$INSTDIR\Project Crimson Alpha.lnk"                "$INSTDIR\crimson.bat" "" "$INSTDIR\bin64\cardboard-MGW.exe" 0 SW_SHOWMINIMIZED
  CreateShortCut "$SMPROGRAMS\Project Crimson Alpha\Project Crimson Alpha.lnk" "$INSTDIR\crimson.bat" "" "$INSTDIR\bin64\cardboard-MGW.exe" 0 SW_SHOWMINIMIZED
  CreateShortCut "$SMPROGRAMS\Project Crimson Alpha\Uninstall.lnk"   "$INSTDIR\uninstall.exe"   "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\Project Crimson Alpha\README.lnk"      "$INSTDIR\README.html"     "" "$INSTDIR\README.html" 0

SectionEnd

Section "Uninstall"

  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CrimsonAlpha"
  DeleteRegKey HKLM SOFTWARE\CrimsonAlpha

  RMDir /r "$SMPROGRAMS\Project Crimson Alpha"
  RMDir /r "$INSTDIR"

SectionEnd

Function .onInit
   InitPluginsDir
   ;Get the skin file to use
   File /oname=$PLUGINSDIR\VCLStyle.vsf "MetroBlackRed.vsf"
   ;Load the skin using the LoadVCLStyle function
   NSISVCLStyles::LoadVCLStyle $PLUGINSDIR\VCLStyle.vsf
FunctionEnd
