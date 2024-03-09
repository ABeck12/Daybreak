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
    match sys.argv[1]:
        case 'clean': CleanProject()
        case 'vscode': GenerateVsCode()
        case 'vscode': GenerateVsCode()
        case _: print('Unknown argument!')
    return

if __name__ == '__main__':
    main()