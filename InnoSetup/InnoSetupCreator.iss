; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "MiniserverUpdater"
#define MyAppVersion "0.1.0"
#define MyAppPublisher "Musat"
#define MyAppExeName "Qt_MiniserverUpdater.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{8259765F-8364-4436-8A49-3FD08C22125D}
AppName={#MyAppName}
AppVersion={#MyAppVersion} 
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={autopf}\MiniserverUpdater 
DisableProgramGroupPage=yes
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
OutputDir=C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\InnoSetup
OutputBaseFilename=MiniserverUpdater_{#MyAppVersion}
SetupIconFile=C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\Qt_MiniserverUpdater.ico
Compression=lzma
SolidCompression=yes
WizardStyle=modern
;DisableDirPage=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\Changelog.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\EmptyProject.Loxone"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\Qt6Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\Qt6Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\Qt6Network.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\Qt6Svg.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\Qt6Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\D3Dcompiler_47.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\libgcc_s_seh-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\msvcp140.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\msvcp140_1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\msvcp140_2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\opengl32sw.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\vcruntime140.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\vcruntime140_1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\resources\*"; DestDir: "{app}\resources"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\generic\*"; DestDir: "{app}\generic"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\iconengines\*"; DestDir: "{app}\iconengines"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\imageformats\*"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\networkinformation\*"; DestDir: "{app}\networkinformation"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\platforms\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\styles\*"; DestDir: "{app}\styles"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Users\musatbe\source\repos\Qt_MiniserverUpdater\x64\Release\tls\*"; DestDir: "{app}\tls"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

