#!/usr/bin/python

import os

import parsers
import uml_io

import log_debug

from parsers import globalClassLibrary


def output_file_name(inputFileName: str):
    return inputFileName + "_class.pu"


class ProcessState:
    def __init__(self):
        # log_debug.LogDebug.__init__(self)
        # self.DebugLogs = True
        pass

    def log_debug(self, msg: str, forcePrint=False):
        # if self.DebugLogs or forcePrint:
        print(msg)

    def _type(self):
        return self.__class__.__name__

    def execute(self, io):
        # if self.DebugLogs:
        self.log_debug(f"Executing stage for: {self._type()}")
        # else:
        # print("Not implemented")

    def write(self, io: uml_io.Writable, msg: str):
        io.write(msg + "\n")


class AddHeader(ProcessState):
    def __init__(self):
        super().__init__()

    def execute(self, io):
        super().execute(io)
        self.write(io, "@startuml")


class AddFooter(ProcessState):
    def __init__(self):
        super().__init__()

    def execute(self, io):
        super().execute(io)
        self.write(io, "@enduml")


class ConvertSingleFileState(ProcessState):
    # def __init__(self):
    #     super().__init__()

    def execute(self, io: uml_io.IO):
        super().execute(io)
        parser = parsers.ClassObjectParser()

        for line in io.fin:
            parser.processLine(line)

        parser.parser_write(io)


class DirectoryConvertState(ProcessState):
    def __init__(self):
        super().__init__()

    def __init__(self, dirPath: str, outFilePath: str):
        self.dirPath = dirPath
        self.outFilePath = outFilePath

    def execute(self, unused_io):
        super().execute(unused_io)

        # Parse directory, find classes in headers and add them to
        # main class diagram : outFilePath
        # Also writeSeparateFile can be enabled to store an idiviadul diagram for
        # each processed file

        writeSeparateFile = False

        singleFileConvertor = ConvertSingleFileState()

        mainDiagramIO = uml_io.IO(self.outFilePath, self.outFilePath, True)

        for path, subdirs, files in os.walk(self.dirPath):
            for name in files:
                inputFilePath = os.path.join(path, name)
                if inputFilePath.lower().endswith(('.h', '.hpp',)):

                    print("Processing file: " + inputFilePath + " ...")

                    #  Process single file and append to all class diagram

                    mainDiagramIO.close_read()
                    mainDiagramIO.inFilePath = inputFilePath
                    mainDiagramIO.open_read()
                    ConvertSingleFileState().execute(mainDiagramIO)

                    # Process and write out a single file

                    if writeSeparateFile:
                        outputSingleFilePath = output_file_name(inputFilePath)
                        ioSingleFile = uml_io.IO(
                            inputFilePath, outputSingleFilePath)
                        singleFileConvertor.execute(ioSingleFile)

        # Add connection to the diagram
        connectionsStr = ""
        for tclass in globalClassLibrary.targetClassList:
            connectionAdded = False
            for connection in tclass.connections:
                # Check is connection other class name is in globalClassLibrary
                # So we skip inheritance\aggregation\composition for unknown types
                if globalClassLibrary.contains_class(connection.otherClassName):
                    # mainDiagramIO.write(connection.name)
                    connectionsStr += connection.name + "\n"
                    connectionAdded = True

            # Add new line after each class connections
            # mainDiagramIO.write("\n")
            if connectionAdded:
                connectionsStr += "\n"

        # Write connections
        if len(connectionsStr) > 0:
            mainDiagramIO.write(connectionsStr)
            mainDiagramIO.close()
