#!/usr/bin/python

import os
# import sys
# import getopt
import re

# from io import IO

import uml_io
# from uml_io import IO
import log_debug

from log_debug import LogDebug


def tab():
    return "    "


class TObject(LogDebug):
    def __init__(self, name="", entireLine=""):
        super().__init__()
        self.name = name.strip()
        self.entireLine = entireLine.strip()
        self.accessSpecifier = "+"

        self.log_debug(f"New TObject created: <{name}>")

    def write(self, io: uml_io.IO):
        pass

    def write_msg(self, io: uml_io.IO, msg: str):
        io.write(msg + "\n")


class TClassMethod(TObject):
    def __init__(self, name="", entireLine=""):
        super().__init__(name, entireLine)

    def write(self, io: uml_io.IO):
        fName = self.entireLine

        # Remove ';' at the end
        fName = re.sub(';.*$', '', fName)
        # If function is ending with '(' replace with '(...)'
        fName = re.sub('\($', '(...)', fName)

        self.write_msg(io, tab() + f'{{method}}{self.accessSpecifier} {fName}')


class TClassMember(TObject):
    def __init__(self, name="", entireLine=""):
        super().__init__(name, entireLine)

    def write(self, io: uml_io.IO):
        mName = self.entireLine
        mName = re.sub(';.*$', '', mName)

        self.write_msg(io, tab() + f'{{field}}{self.accessSpecifier} {mName}')


class TClassConnections(TObject):
    def __init__(self, name="", entireLine="", otherClassName=""):
        super().__init__(name, entireLine)
        self.otherClassName = otherClassName.strip()

    def write(self, io: uml_io.IO):
        mName = self.entireLine.strip()
        mName = re.sub(';.*$', '', mName)

        self.write_msg(io, tab() + f'{{field}}{self.accessSpecifier} {mName}')


class TClass(TObject):
    def __init__(self, name="", entireLine=""):
        super().__init__(name, entireLine)
        self.methods = [TClassMethod()]
        self.members = [TClassMember()]
        self.connections = [TClassConnections()]
        self.allObject = [TObject()]
        self.DebugLogs = True

        self.methods.clear()
        self.members.clear()
        self.connections.clear()
        self.allObject.clear()

    def addMethod(self, obj: TClassMethod):
        self.methods.append(obj)
        self.allObject.append(obj)

    def addMember(self, obj: TClassMember):
        print(
            f">> Adding new class member: Class name: {self.name}, Member: {obj.name}, Entire line: {obj.entireLine}")
        self.members.append(obj)
        self.allObject.append(obj)

    def addConnection(self, obj: TClassConnections()):
        self.connections.append(obj)

    def print_header(self, io, header: str):
        self.write_msg(io, "")
        self.write_msg(io, header)
        self.write_msg(io, "")

    def writeObjectForAccessType(self, io, header: str, accessType: str, objects:  [TObject]):

        headerPrinted = False
        if not header:
            headerPrinted = True

        for obj in objects:

            self.log_debug(
                    f"Class: {self.name}, Checking Obj to write -> name: {obj.name}, entire line: {obj.entireLine}, access: {obj.accessSpecifier}, passed access: {accessType}")

            if obj.accessSpecifier == accessType:
                if not headerPrinted:
                    self.print_header(io, header)
                    headerPrinted = True
                self.log_debug(
                    f"Class: {self.name}, Writing Obj name: {obj.name}, entire line: {obj.entireLine}, access: {obj.accessSpecifier}, passed access: {accessType}")
                obj.write(io)

    def done_write(self, io):
        self.write_msg(io, f"class {self.name}")
        self.write_msg(io, "{")

        self.log_debug(
            f"Writing methods for class: {self.name} objects count: {len(self.methods)}")

        if len(self.methods) > 0:
            self.write_msg(io, "..Methods..")

            self.writeObjectForAccessType(io, "..Public..", "+", self.methods)
            self.writeObjectForAccessType(
                io, "..Protected..", "#", self.methods)
            self.writeObjectForAccessType(io, "..Private..", "-", self.methods)

        self.log_debug(
            f"Writing members for class: {self.name} objects count: {len(self.members)}")

        if len(self.members) > 0:
            self.write_msg(io, "\n..Fields..")

            self.writeObjectForAccessType(io, "..Public..", "+", self.members)
            self.writeObjectForAccessType(
                io, "..Protected..", "#", self.members)
            self.writeObjectForAccessType(io, "..Private..", "-", self.members)

        self.write_msg(io, "}")
        self.write_msg(io, "")
