#!/usr/bin/python

import os
import sys
import getopt


def replace_class_name_in_file(inFilePath, outFilePath, className):
    print("inFilePath: ", inFilePath)
    print("outFilePath: ", outFilePath)

    fin = open(inFilePath, "rt")

    # output file to write the result to
    fout = open(outFilePath, "wt")
    # for each line in the input file
    for line in fin:
        # read replace the string and write to output file
        newLine = line.replace('$ClassName', className)
        newLine = newLine.replace('$CLASSNAME', className.upper())
        fout.write(newLine)
    # close input and output files
    fin.close()
    fout.close()


def main(argv):

    print("@ Argument List: ", str(sys.argv))

    if len(sys.argv) < 3:
        sys.exit("Args les then 2")
        print("Argument List: ", str(sys.argv))
        print("Class name is required")
        print("Args should be: outPath className")

    outPath = sys.argv[1]
    className = sys.argv[2]

    thisFilePath = os.path.dirname(os.path.realpath(__file__))
    patternsPath = thisFilePath + "/patterns"

    print("thisFilePath: ", thisFilePath)
    print("patternsPath: ", patternsPath)

    headerPatterFilePath = patternsPath+"/ClassName.h.txt"
    headerOutFilePath = outPath+"/"+className+".h"

    replace_class_name_in_file(
        headerPatterFilePath, headerOutFilePath, className)

    sourcePatterFilePath = patternsPath+"/ClassName.cpp.txt"
    sourceOutFilePath = outPath+"/"+className+".cpp"

    replace_class_name_in_file(
        sourcePatterFilePath, sourceOutFilePath, className)


if __name__ == "__main__":
    main(sys.argv[1:])
