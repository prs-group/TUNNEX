#!/usr/bin/env python3
# vim:tabstop=4:softtabstop=4:shiftwidth=4

import re 
import sys
from tempfile import NamedTemporaryFile
from optparse import OptionParser
from pathlib import Path

def generateOptionParser():
    parser = OptionParser()
    parser.add_option("-n", "--namespace", dest="userNamespace",
            default="::", nargs=1, action="store",
            help="Only add declspec to declarations in namespace")

    parser.add_option("-m", "--declspec-macro", dest="dclspecMacro",
            default="DECLSPEC_MACRO", nargs=1, action="store",
            help="The marco used for declspec")

    parser.add_option("-d", "--dry-run", dest="dry_run", action="store_true",
            help="Do not change file. Prints modified line to std out")

    parser.add_option("-v", "--verbose",action="store_true",
            help="More output to std error")

    return parser


namespaceRegex=re.compile("namespace\s*([\w_]+)")
classRegex=re.compile("class (\w+)")
functionRegex=re.compile("(\w+)\s+([\w_]+)\s*\(\s*[\w\s_:&,]+\);")

def main():

    parser=generateOptionParser()
    (options, args)=parser.parse_args()

    fileNames=args

    
    for fileName in fileNames:
        outFileTemporary=NamedTemporaryFile(delete=False)

        with open(fileName) as file:
            currentNamespace="::"
            openCurlyBrackets=0
            lineNumber=0
            for line in file:
                newLine=line
                namespaceMatch=namespaceRegex.search(line)

                if options.dclspecMacro in line:
                    outFileTemporary.write(newLine.encode('UTF-8'))
                    continue

                if namespaceMatch:
                    currentNamespace=namespaceMatch.group(1)
                    lineNumber+=1
                    outFileTemporary.write(newLine.encode('UTF-8'))
                    continue

                classMatch=classRegex.search(line)

                if classMatch and currentNamespace == options.userNamespace:
                    newLine=line.replace("class", "class " + options.dclspecMacro + " ")
                    if options.dry_run:
                        print("At Line {}: {}".format(lineNumber,newLine))
                        continue

                functionMatch=functionRegex.search(line)
                
                if functionMatch and currentNamespace == options.userNamespace:
                    newLine=options.dclspecMacro + " " + line
                    if options.dry_run:
                        print("At Line {}: {}".format(lineNumber,newLine))
                        continue

                if '{' in line:
                    openCurlyBrackets+=1

                if '}' in line:
                    openCurlyBrackets-=1

                if openCurlyBrackets == 0:
                    currentNamespace="::"

                outFileTemporary.write(newLine.encode('utf-8'))
                lineNumber+=1


        if not options.dry_run:
            oldFile=Path(fileName)
            oldFileName=str(fileName)
            newFile=Path(outFileTemporary.name)
            oldFile.rename(fileName + ".bak")
            newFile.rename(oldFileName)



if __name__ == "__main__":
    main()

