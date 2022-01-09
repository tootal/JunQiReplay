$FILE_DIR = Split-Path -Parent ${MyInvocation}.MyCommand.Definition
$WORK_DIR = Split-Path -Parent ${FILE_DIR}
Push-Location -Path ${WORK_DIR}

Invoke-Expression -Command 'scripts\win_env.ps1'

$BUILD_DIR = 'build/vs_build'
cmake -DCMAKE_PREFIX_PATH="${env:QT6_DIR}" -B "${BUILD_DIR}" -G "Visual Studio 16 2019" -T host=x64 -A x64
if (${LASTEXITCODE} -eq 0) {
    # cmake --open "${BUILD_DIR}"
}
Pop-Location
