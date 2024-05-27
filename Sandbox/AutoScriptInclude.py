import os
from typing import List, Tuple, Set
from dataclasses import dataclass

@dataclass
class Include:
    className: str
    filepath: str

def parseFile(filepath: str) -> List[Include]:
    includes: List[Include] = []
    if filepath[-2:] != ".h":
        return includes

    with open(filepath,'r') as file:
        for line in file.readlines():
            if "DB_SCRIPT" not in line:
                continue
            words = line.split()
            if words[0] == "DB_SCRIPT":
                includes.append(Include(words[2],filepath))
    return includes

def generateIncludeFile(includes: List[Include]) -> None:
    headerFile = "ScriptInclude.auto.h"
    sourceFile = "ScriptInclude.auto.cpp"


    with open(headerFile, "w") as file:
        file.write("#pragma once\n#include <Daybreak.h>\n\n")
        includeFiles: Set[str] = {include.filepath for include in includes}
        for filepath in includeFiles:
            file.write(f'#include "{filepath}"\n')
        file.write("\nnamespace Daybreak::Auto\n{")
        file.write("\n\tvoid AutoScriptInclude();\n}")

    with open(sourceFile, "w") as file:
        file.write('#include "ScriptInclude.auto.h"\n\n')
        file.write('void Daybreak::Auto::AutoScriptInclude()\n{\n')

        for include in includes:
            file.write(f'\tDB_REGISTER_SCRIPT({include.className});\n')

        file.write('}')
    return

def main() -> None:
    os.chdir("Sandbox/src")

    scriptDir: str = "scripts"
    includes: List[Include] = []
    paths = os.walk(scriptDir)
    for dirName, subFolders, files in paths:
        for filename in files:
            includes += parseFile(os.path.join(dirName,filename))

    generateIncludeFile(includes)
    return

if __name__ == '__main__':
    main()