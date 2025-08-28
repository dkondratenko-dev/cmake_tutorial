#!/usr/bin/python


import os
import sys
import getopt
import re

import targets
import umL_states
import uml_io


class SingleFileConvertor:
    def __init__(self):
        self.stages = [umL_states.ProcessState()]

        self.make_stages()

    def make_stages(self):
        self.stages.clear()
        self.stages.append(umL_states.AddHeader())
        self.stages.append(umL_states.ConvertSingleFileState())
        self.stages.append(umL_states.AddFooter())

    def process(self, io: uml_io.IO):

        print(f"Processing ... {io.inFilePath}")

        for stage in self.stages:
            stage.execute(io)


class DirectoryConvertor:
    def __init__(self, dirPath: str, outFilePath: str):
        self.dirPath = dirPath
        self.outFilePath = outFilePath

    def process(self, io: uml_io.Writable):

        io.open_write()
        umL_states.AddHeader().execute(io)
        io.close_write()

        umL_states.DirectoryConvertState(
            self.dirPath, self.outFilePath).execute(None)

        io.open_write()
        umL_states.AddFooter().execute(io)
        io.close_write()


def output_file_name(inputFileName: str):
    return inputFileName + "_class.pu"


def convert_cpp_to_uml_file(inputFilePath: str):

    outputFilePath = output_file_name(inputFilePath)

    print("inputFilePath: ", inputFilePath)
    print("outputFilePath: ", outputFilePath)

    convertor = SingleFileConvertor()

    io = uml_io.IO(inputFilePath, outputFilePath)
    convertor.process(io)


def convert_cpp_to_uml_dir(dirPath: str):

    dirName = os.path.basename(os.path.normpath(dirPath))

    outputFilePath = dirName + "_class_diagram_all.pu"

    io = uml_io.Writable(outputFilePath, True)
    dirConvertor = DirectoryConvertor(dirPath, outputFilePath)
    dirConvertor.process(io)


def main(argv):

    print("@ Argument List: ", str(sys.argv))

    if len(sys.argv) < 2:
        sys.exit("Args les then 1")
        print("Argument List: ", str(sys.argv))
        print("Input file name is required")
        print("Args should be: inputFileName")

    path = sys.argv[1]

    if os.path.isdir(path):
        convert_cpp_to_uml_dir(path)
    else:
        convert_cpp_to_uml_file(path)


if __name__ == "__main__":
    main(sys.argv[1:])
