#
# SmartERP NSIS installer script
#
# This installer creator needs to be run with:
# makensis erp.nsi
#

#--------------------------------
# Include Modern UI

    !include "MUI2.nsh"

#--------------------------------
# General

    # Program version
    !define RENTMANAGER_VERSION "1.0.0"

    # VIProductVersion requires version in x.x.x.x format
    !define RENTMANAGER_VIPRODUCTVERSION "1.0.0.0"

    # Installer name and filename
    Name "Rent Manager"
    Caption "Rent Manager ${RENTMANAGER_VERSION} Setup"
    OutFile "Rent Manager-${RENTMANAGER_VERSION}.exe"

    # Icon to use for the installer
    !define MUI_ICON "rentman.ico"

    # Default installation folder
    InstallDir "$PROGRAMFILES\RentManager"

    # Get installation folder from registry if available
    InstallDirRegKey HKCU "Software\RentManager" ""

    # Request application privileges
    RequestExecutionLevel admin

    #Disable compression
    SetCompress auto

#--------------------------------
# Version information

    VIProductVersion "${RENTMANAGER_VIPRODUCTVERSION}"
    VIAddVersionKey "ProductName" "RentManager"
    VIAddVersionKey "FileDescription" "Rent Management System"
    VIAddVersionKey "FileVersion" "${RENTMANAGER_VERSION}"
    VIAddVersionKey "LegalCopyright" "GPL v.3"
    VIAddVersionKey "ProductVersion" "${RENTMANAGER_VERSION}"

#--------------------------------
# Settings

    # Show a warn on aborting installation
    !define MUI_ABORTWARNING

    # Defines the target start menu folder
    !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU"
    !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\RentManager"
    !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"

#--------------------------------
# Variables

    Var StartMenuFolder

#--------------------------------
# Pages

    # Installer pages
    !insertmacro MUI_PAGE_LICENSE "licence.txt"
    !insertmacro MUI_PAGE_DIRECTORY
    !insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
    !insertmacro MUI_PAGE_INSTFILES

    # Uninstaller pages
    !insertmacro MUI_UNPAGE_CONFIRM
    !insertmacro MUI_UNPAGE_INSTFILES

# Languages

    !insertmacro MUI_LANGUAGE "English"

#--------------------------------
# Default installer section

Section
    SetShellVarContext all

    # Installation path
    SetOutPath "$INSTDIR"

    # Delete any already installed DLLs to avoid buildup of various
    # versions of the same library when upgrading
    Delete "$INSTDIR\*.dll"

    # Files to include in installer
    #File /r build\imageformats
    File /r build\platforms
    File /r build\printsupport
    File /r build\sqldrivers
    File build\*.dll
    File build\rentmanager.exe
    File licence.txt
    File rentman.ico

    # Store installation folder in registry
    WriteRegStr HKCU "Software/RentManager" "" $INSTDIR

# Create shortcuts
    !insertMacro MUI_STARTMENU_WRITE_BEGIN Application
	CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
	CreateShortcut "$SMPROGRAMS\$StartMenuFolder\RentManager.lnk" \
		"$INSTDIR\RentManager.exe" "" "$INSTDIR\rentman.ico"
	CreateShortcut "$DESKTOP\RentManager.lnk" \
		"$INSTDIR\RentManager.exe" "" "$INSTDIR\rentman.ico"
	CreateShortcut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" \
		"$INSTDIR\Uninstall.exe"
    !insertMacro MUI_STARTMENU_WRITE_END

    # Create the uninstaller
    WriteUninstaller "$INSTDIR\Uninstall.exe"

    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\RentManager" \
        "DisplayName" "RentManager"
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\RentManager" \
        "DisplayIcon" "$INSTDIR\rentman.ico"
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\RentManager" \
        "UninstallString" "$INSTDIR\Uninstall.exe"
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\RentManager" \
        "DisplayVersion" ${RENTMANAGER_VERSION}

SectionEnd
#--------------------------------
# Uninstaller section

Section "Uninstall"
    SetShellVarContext all

    # Delete installed files
    Delete "$INSTDIR\*.*"
    RMDir /r "$INSTDIR\imageformats"
    RMDir /r "$INSTDIR\platforms"
    RMDir /r "$INSTDIR\translations"
    RMDir /r "$INSTDIR\printsupport"
    RMDir /r "$INSTDIR\sqldrivers"
    RMDir "$INSTDIR"

    # Remove shortcuts
    !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
    Delete "$SMPROGRAMS\$StartMenuFolder\RentManager.lnk"
    Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall RentManager.lnk"
    RMDir "$SMPROGRAMS\$StartMenuFolder"
    Delete "$DESKTOP\RentManager.lnk"

    # Remove registry entries
    DeleteRegKey /ifempty HKCU "Software\RentManager"
    DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\megvel"

SectionEnd



