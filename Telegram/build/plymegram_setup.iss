#define MyAppName "Plymegram Desktop"
#define MyAppShortName "Plymegram"
#define MyAppPublisher "Plymegram"
#define MyAppURL "https://t.me/Plymegram"
#define MyAppExeName "Plymegram.exe"
#define MyAppId "{{B7F8D3C2-4A6E-4D91-9E2B-7C5A1F0D8E34}"
#define CurrentYear GetDateTimeString('yyyy','','')

[Setup]
AppId={#MyAppId}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppCopyright={#MyAppPublisher} 2026-{#CurrentYear}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
OutputDir={#ReleasePath}
OutputBaseFilename=Plymegram-Setup-{#MyAppVersion}
SetupIconFile={#SourcePath}..\Resources\art\icon256.ico
UninstallDisplayName={#MyAppName}
UninstallDisplayIcon={app}\{#MyAppExeName}
Compression=lzma2
SolidCompression=yes
LZMA2DictionarySize=64
WizardStyle=modern
WizardImageFile={#SourcePath}wizard-image.bmp
WizardSmallImageFile={#SourcePath}wizard-small-image.bmp
PrivilegesRequired=lowest
DisableDirPage=auto
DisableProgramGroupPage=yes
CloseApplications=force
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
MinVersion=10.0

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "ru"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "ua"; MessagesFile: "compiler:Languages\Ukrainian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"

[Files]
Source: "{#ReleasePath}\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#ReleasePath}\x64\TelegramWidgetHelper.exe"; DestDir: "{app}\x64"; Flags: ignoreversion; SkipIfNotExists: yes
Source: "{#ReleasePath}\x64\d3d\d3dcompiler_47.dll"; DestDir: "{app}\x64\d3d"; Flags: ignoreversion; SkipIfNotExists: yes

[Icons]
Name: "{group}\{#MyAppShortName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppShortName}}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\{#MyAppShortName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppShortName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: filesandordirs; Name: "{app}\tdata"
Type: filesandordirs; Name: "{app}\tcache"
Type: dirifempty; Name: "{app}"
