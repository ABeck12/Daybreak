import os, platform, sys

def CleanProject():
    system = platform.system()
    if system == 'Windows':
        os.system('.\scripts\CleanProject.bat')

def GenerateVsCode():
    system = platform.system()
    if system == 'Windows':
       os.system('.\scripts\GenerateProject-vscode.bat')
        

def GenerateVS():
    system = platform.system()
    if system == 'Windows':
        os.system('.\scripts\GenerateProject-VS.bat')

def main():
    if len(sys.argv) == 1:
        print('Provide one argument! Arguments are "vs","vscode","clean"')
        return
    match sys.argv[1]:
        case 'clean': CleanProject()
        case 'vscode': GenerateVsCode()
        case 'vs': GenerateVS()
        case _: print('Unknown argument! Arguments are "vs","vscode","clean"')
    return

if __name__ == '__main__':
    main()