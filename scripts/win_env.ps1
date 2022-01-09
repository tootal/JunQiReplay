$FILE_DIR = Split-Path -Parent ${MyInvocation}.MyCommand.Definition
$WORK_DIR = Split-Path -Parent ${FILE_DIR}
Push-Location -Path ${WORK_DIR}
echo 'Setting up environment...'

$QT_BASEDIR = 'C:\Qt'
$QT_KIT = 'msvc2019_64'
$QT_VERSION = '6.2.2'

$QT_PATH = "${QT_BASEDIR}\${QT_VERSION}\${QT_KIT}"
$Qt6_DIR = "${QT_PATH}\lib\cmake\Qt6"

echo "Detect QT_PATH: ${QT_PATH}"
$env:PATH = "${QT_PATH}\bin;${env:PATH}"
qmake --version

$VS_BASEDIR = 'C:\Program Files (x86)\Microsoft Visual Studio'
$VS_VERSION = '2019'
$VS_BASEDIR = "${VS_BASEDIR}\${VS_VERSION}"
$VS_TYPE = 'Enterprise'
if (-Not (Test-Path "${VS_BASEDIR}\${VS_TYPE}")) {
    $VS_TYPE = 'Community'
}
$VS_BASEDIR = "${VS_BASEDIR}\${VS_TYPE}"
echo "Detect VS_BASEDIR: ${VS_BASEDIR}"
$env:VCINSTALLDIR = "${VS_BASEDIR}\VC"
# call "%VCINSTALLDIR%\Auxiliary\Build\vcvarsall.bat" x64
Pop-Location