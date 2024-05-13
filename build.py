import os
import sys
import shutil


def CleanProject() -> None:
    os.system(r'vendor\premake\premake5.exe clean')


def GenerateVsCode() -> None:
    os.system(r'vendor\premake\premake5.exe vscode')
    os.system(r'vendor\premake\premake5.exe export-compile-commands')
    shutil.copy(r'compile_commands\release.json',r'.\compile_commands.json')
    shutil.rmtree('compile_commands')
    if not os.path.exists(".vscode"):
        os.mkdir(".vscode")
    shutil.copy(r'Tasks\.vscode\tasks.json', r'.\.vscode\tasks.json')


def GenerateVS() -> None:
    os.system(r'vendor\premake\premake5.exe vs2022')
    os.system(r'vendor\premake\premake5.exe export-compile-commands')
    shutil.copy(r'compile_commands\release.json',r'.\compile_commands.json')
    shutil.rmtree('compile_commands')


def GenerateMake() -> None:
    os.system(r'vendor\premake\premake5.exe gmake2')
    os.system(r'vendor\premake\premake5.exe export-compile-commands')
    shutil.copy(r'compile_commands\release.json',r'.\compile_commands.json')
 

def Compile(flag: str) -> None:
    match flag:
        case '-r': os.system(r'"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" Daybreak.sln -p:Configuration=Release -maxcpucount')
        case '-d': os.system(r'"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" Daybreak.sln -p:Configuration=Debug -maxcpucount')


def Run(flag: str) -> None:
    os.chdir("Sandbox")
    match flag:
        case '-d': os.startfile(os.path.abspath("../bin/Debug-windows-x86_64/Sandbox/Sandbox.exe"))
        case '-r': os.startfile(os.path.abspath("../bin/Release-windows-x86_64/Sandbox/Sandbox.exe"))


def main() -> None:
    if len(sys.argv) == 1:
        print('Provide one argument! Arguments are "vs","vscode","make","clean","compile","run"')
        return
    
    try:
        compileFlag = sys.argv[2]
        if compileFlag not in ['-r', '-d']:
            print(f"Unknown compile or run flag {compileFlag}! Please use '-r' for relase or '-d' for debug modes")
            return
    except IndexError:
        compileFlag = '-r'

    match sys.argv[1]:
        case 'clean': CleanProject()
        case 'vscode': GenerateVsCode()
        case 'vs': GenerateVS()
        case 'make': GenerateMake()
        case 'compile': Compile(compileFlag)
        case 'run': Run(compileFlag)
        case _: print('Unknown argument! Arguments are "vs","vscode","make,"clean","compile","run"')
    return


if __name__ == '__main__':
    main()
