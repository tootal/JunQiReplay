$FILE_DIR = Split-Path -Parent ${MyInvocation}.MyCommand.Definition
$WORK_DIR = Split-Path -Parent ${FILE_DIR}
Push-Location -Path ${WORK_DIR}
$QT_PATH = 'C:\Qt\6.2.2\msvc2019_64'
$BUILD_DIR = 'build/vs'
cmake -DCMAKE_PREFIX_PATH="${QT_PATH}" -B "${BUILD_DIR}" -G "Visual Studio 16 2019" -T host=x64 -A x64
cmake --open "${BUILD_DIR}"
Pop-Location
