; -- FreeEMS-Loader.iss --


[Setup]
AppName=FreeEMS-Loader
AppVerName=FreeEMS-Loader v. ?.?.?
DefaultDirName={pf}\FreeEMS-Loader
DefaultGroupName=FreeEMS-Loader
UninstallDisplayIcon={app}\FreeEMS-Loader.exe
Compression=lzma
SolidCompression=yes
; Page 0: Initial setup. Disable with DisableStartupPrompt
DisableStartupPrompt=Yes
; Page 3: License Agreement.  Shown if LicenseFile is set.
; Users may proceed to the next page only if the option
; "I accept the agreement" is selected.
LicenseFile=.\LICENSE
; Page 5: Information.  Shown if InfoBeforeFile is set. Name the file
; to make it easy to recognise
;InfoBeforeFile=.\WIN_NOTES.txt

[Files]
Source: "z:\target\boost\lib\libboost_system-mt.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "z:\target\qt\bin\libgcc_s_dw2-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "z:\target\qt\bin\QtCore4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "z:\target\qt\bin\QtGui4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "z:\target\qt\bin\mingwm10.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "z:\home\dave\git\freeems-loader\src\debug\*.exe"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\FreeEMS Loader"; Filename: "{app}\FreeEMS_Loader.exe"; WorkingDir: "{app}"
Name: "{userdesktop}\FreeEMS Loader"; Filename: "{app}\FreeEMS_Loader.exe"; WorkingDir: "{app}"
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\FreeEMS Loader"; Filename: "{app}\FreeEMS_Loader.exe"; WorkingDir: "{app}"

