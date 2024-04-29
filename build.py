import os
import platform
import sys


def CleanProject():
    system = platform.system()
    if system == 'Windows':
        os.system(r'.\scripts\CleanProject.bat')


def GenerateVsCode():
    system = platform.system()
    if system == 'Windows':
        os.system(r'.\scripts\GenerateProject-vscode.bat')


def GenerateVS():
    system = platform.system()
    if system == 'Windows':
        os.system(r'.\scripts\GenerateProject-VS.bat')


def Compile(flag):
    match flag:
        case '-r': os.system(r'"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" C:\dev\Daybreak\Daybreak.sln -p:Configuration=Release -maxcpucount')
        case '-d': os.system(r'"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" C:\dev\Daybreak\Daybreak.sln -p:Configuration=Debug -maxcpucount')


def Run(flag):
    os.chdir("Sandbox")
    match flag:
        case '-d': os.startfile(os.path.abspath("../bin/Debug-windows-x86_64/Sandbox/Sandbox.exe"))
        case '-r': os.startfile(os.path.abspath("../bin/Release-windows-x86_64/Sandbox/Sandbox.exe"))


def main():
    if len(sys.argv) == 1:
        print('Provide one argument! Arguments are "vs","vscode","clean","compile","run"')
        return
    
    try:
        compileFlag = sys.argv[2]
    except IndexError:
        compileFlag = '-r'

    match sys.argv[1]:
        case 'clean': CleanProject()
        case 'vscode': GenerateVsCode()
        case 'vs': GenerateVS()
        case 'compile': Compile(compileFlag)
        case 'run': Run(compileFlag)
        case _: print('Unknown argument! Arguments are "vs","vscode","clean","compile","run"')
    return


if __name__ == '__main__':
    main()
