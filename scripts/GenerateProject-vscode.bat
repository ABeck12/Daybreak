call vendor\premake\premake5.exe vscode
call vendor\premake\premake5.exe export-compile-commands
copy compile_commands\release.json .\compile_commands.json
mkdir .vscode
copy Tasks\.vscode\tasks.json .\.vscode\tasks.json
@REM PAUSE