$FILE_DIR = Split-Path -Parent ${MyInvocation}.MyCommand.Definition
$WORK_DIR = Split-Path -Parent ${FILE_DIR}
Push-Location -Path ${WORK_DIR}
echo 'Setting up environment...'

if (-Not (Test-Path env:QT6_DIR)) {
    $env:QT6_DIR = 'C:\Qt\6.2.2\msvc2019_64'
    $env:PATH = "${env:QT6_DIR}\bin;${env:PATH}"
}

echo "Detect Qt6_DIR: ${env:Qt6_DIR}"
qmake --version

$VS_BASEDIR = 'C:\Program Files (x86)\Microsoft Visual Studio\2019'
$VS_TYPE = 'Enterprise'
if (-Not (Test-Path "${VS_BASEDIR}\${VS_TYPE}")) {
    $VS_TYPE = 'Community'
}
$VS_BASEDIR = "${VS_BASEDIR}\${VS_TYPE}"
echo "Detect VS_BASEDIR: ${VS_BASEDIR}"
$env:VCINSTALLDIR = "${VS_BASEDIR}\VC"
# call "%VCINSTALLDIR%\Auxiliary\Build\vcvarsall.bat" x64
Pop-Location
