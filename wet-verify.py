import shutil
import re
import sys
import os

# input:
# [0] system reserved :(
# [1] path to zip file

files_to_include = ["wet2util.h", "main24a2.cpp"]
# all files in files_to_include MUST be in the same directory as the .py file

okh = [
    "cstdio", "iostream", "istream", "ostream",
    "assert.h", "ctype.h", "errno.h", "fenv.h", "float.h", "inttypes.h",
    "limits.h", "math.h", "cassert", "cerrno", "exception", "stacktrace",
    "stdexcept", "system_error", "stdarg.h", "stddef.h", "stdint.h",
    "stdio.h", "stdlib.h", "string.h", "time.h", "uchar.h", "wchar.h",
    "wctype.h", "ccomplex", "complex.h", "ctgmath", "tgmath.h", "cstdlib",
    "exception", "cassert", "cmath", "cfloat", "cinttypes", "climits",
    "cstdint", "limits", "stdfloat"
]

stlh = [
    "string", "iterator", "generator", "ranges", "valarray", "array",
    "deque", "flat_map", "flat_set", "forward_list", "list", "map",
    "mdspan", "queue", "set", "span", "stack", "unordered_map",
    "unordered_set", "vector", "algorithm", "execution"
]

deniedh = [
    "coroutine", "fstream", "sstream", "streambuf", "strstream",
    "syncstream", "spanstream", "regex", "barrier", "condition_variable",
    "future", "latch", "mutex", "semaphore", "shared_mutex", "stop_token",
    "thread", "setjmp.h", "signal.h"
]

def zip_extract(zzip, zipdir):
    cmd = f"unzip {zzip} -d {zipdir} >/dev/null"
    r = os.system(cmd)
    return r == 0

def test_stl(path):
    err = 0
    with open(path, 'r') as fin:
        for line in fin:
            match = re.search(r'#\s*include\s*<([^>]*)>', line)
            if match:
                inc = match.group(1).strip()
                if inc.lower() in okh:
                    continue
                if inc.lower() in stlh or inc.lower() in deniedh:
                    err += 1
                    print(f"Error: found {'STL' if inc.lower() in stlh else 'denied'} header in {path}: include <{inc}>")
                else:
                    print(f"Note: {path}: include <{inc}>")
    return err

def verify_files(dir, content):
    codefiles = 0
    submissiontxt = 0
    drypdf = 0
    other = 0
    
    for filename in os.listdir(dir):
        path = os.path.join(dir, filename)
        if filename in files_to_include:
            print(f"Error: file {filename} must not be submitted")
            return 1
        
        _, ext = os.path.splitext(filename)
        if ext.lower() in [".h", ".hpp"]:
            content.hfiles.append(path)
            codefiles += 1
        elif ext.lower() == ".cpp":
            content.codefiles.append(path)
            codefiles += 1
        elif filename == "submissions.txt":
            submissiontxt += 1
        elif filename.lower() == "dry.pdf":
            drypdf += 1
        else:
            other += 1

    if codefiles == 0:
        print("Error: no code files! Shouldn't have a directory in the top level of zip")
        return 1
    if submissiontxt == 0:
        print("Error: missing file: submissions.txt")
        return 1
    if drypdf == 0:
        print("Error: missing file: dry.pdf")
        return 1

    return 0

def verify_stl(content):
    errors = 0
    for path in content.codefiles:
        errors += test_stl(path)
    for path in content.hfiles:
        errors += test_stl(path)
    return errors

def verify_compilation(content):
    cd = os.getcwd()
    os.chdir(content.dir)
    r = os.system("g++ -std=c++11 -DNDEBUG -Wall *.cpp 2>/dev/null")
    os.chdir(cd)
    if r == 0:
        return 0
    print("Error: g++ compiler error")
    return 1 # compiler error

class Expando:
    pass

def main_verify():
    if len(sys.argv) < 2:
        print(f"wrong arguments: argc={len(sys.argv)}")
        exit(1)
    
    temproot = "TEMP_DIRECTORY_FOR_VERIFICATION/"
    if os.path.isdir(temproot):
        shutil.rmtree(temproot)
    os.mkdir(temproot)
    
    zipdir = os.path.join(temproot, "zip")
    os.mkdir(zipdir)

    zip_path = sys.argv[1]

    if not os.path.exists(zip_path):
        print(f"input zip file does not exist: {zip_path}")
        exit(1)

    if not os.path.isfile(zip_path):
        print(f"input path is not a file: {zip_path}")
        exit(1)
    
    if not zip_extract(zip_path, zipdir):
        print(f"Failed to extract zip file: {zip_path}")
        exit(1)

    nerr = 0

    content = Expando()
    content.dir = zipdir
    content.codefiles = []
    content.hfiles = []
    
    mydir = os.path.dirname(os.path.realpath(__file__))
    
    nerr += verify_files(zipdir, content)
    
    for file in files_to_include:
        arg1 = os.path.join(mydir, file)
        arg2 = os.path.join(zipdir, file)
        shutil.copy(arg1, arg2)
        
    nerr += verify_stl(content)
    nerr += verify_compilation(content)

    shutil.rmtree(temproot)

    if nerr == 0:
        print("ok")
    else:
        print(f"{nerr} errors found")

if __name__ == "__main__":
    main_verify()
