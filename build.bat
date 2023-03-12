chcp 65001

set PROJECT_DIR=%~dp0
set BUILD_DIR=%PROJECT_DIR%\build
if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

cmake -S %PROJECT_DIR% -B %BUILD_DIR% ^
    -G "Visual Studio 15 2017" -A x64
cmake --build %BUILD_DIR% --config Debug

%BUILD_DIR%\Debug\glkit_app.exe