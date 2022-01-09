$FILE_DIR = Split-Path -Parent ${MyInvocation}.MyCommand.Definition
$WORK_DIR = Split-Path -Parent ${FILE_DIR}
Push-Location -Path ${WORK_DIR}

Invoke-Expression -Command 'scripts\win_env.ps1'

$BUILD_DIR = 'build\win_build'
$DEPLOY_DIR = 'build\win_deploy'
$DEPLOY_NAME = 'JunQiReplay'
$DEPLOY_PATH = "${DEPLOY_DIR}\${DEPLOY_NAME}"

echo 'Removing cache...'
if (Test-Path ${DEPLOY_DIR}) {
    Remove-Item ${DEPLOY_DIR} -Recurse
}
echo 'Building...'
cmake -B "${BUILD_DIR}"
cmake --build "${BUILD_DIR}" --config Release

echo 'Deploying...'
New-Item ${DEPLOY_PATH} -Type Directory
Copy-Item "${BUILD_DIR}\Release\${DEPLOY_NAME}.exe" ${DEPLOY_PATH}
windeployqt "${DEPLOY_PATH}\${DEPLOY_NAME}.exe" --release --verbose 0

echo 'Compressing...'
Push-Location -Path ${DEPLOY_DIR}
7z a "${DEPLOY_NAME}.7z" ${DEPLOY_NAME} -mx9
Pop-Location

echo 'Deploy successful!'
Invoke-Item ${DEPLOY_DIR}

Pop-Location
