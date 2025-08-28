#!/usr/bin/python

import os
import re

import targets
import uml_io
import log_debug

from typing import Pattern
from typing import AnyStr


def rx(p: str, printPattern=False):
    if printPattern:
        print(f">>>>>>>>>>>>>>>>>>>> re.compile({p})")
    return re.compile(p)


class AvailableClassLibrary:
    def __init__(self):
        self.classesList = [str()]
        self.targetClassList = [targets.TClass()]

        self.targetClassList.clear()
        self.classesList.clear()

    def add_new_class(self, className: str):
        if self.contains_class(className):
            return False

        self.classesList.append(className)

    def contains_class(self, className: str):
        return className in str(self.classesList)

    def add_new_target_class(self, t: targets.TClass):
        # Check already exists
        for tc in self.targetClassList:
            if t.name == tc.name:
                print(
                    f">>>>>>>>>>>>>>>>>>>>>>>!!!!!!!!!!!!! ERROR: Target class already exists: {tc.name}")
                return False

        self.targetClassList.append(t)


globalClassLibrary = AvailableClassLibrary()


class ObjectParser(log_debug.LogDebug):
    def __init__(self):
        super().__init__()
        self.isActivated = False
        self.DebugLogs = False

    def processLine(self, line: str):
        print("Not Implemented")


class CommentsParser(ObjectParser):
    def __init__(self):
        super().__init__()
        self.commentBlockStartPattern = rx(r"^[ ]*\/\*")
        self.commentBlockEndPattern = rx(r"\*\/")
        self.lineCommentPattern = rx(r"^[ ]*\/\/")
        self.result = ""

    def processLine(self, line: str):
        self.result = ""
        if not self.isActivated and re.search(self.commentBlockStartPattern, line):
            # print(f">>> CommentsParser: Found Block start: activated: {self.isActivated}, line:{line}")
            self.result = "block comment start"
            self.isActivated = True
            return True
        if self.isActivated and re.search(self.commentBlockEndPattern, line):
            # print(f">>> CommentsParser: Found Block end: activated: {self.isActivated}, line:{line}")
            self.result = "block comment end  "
            self.isActivated = False
            return True
        elif re.search(self.lineCommentPattern, line):
            self.result = "single line comment"
            # print(f">>> CommentsParser: Found line comment: activated: {self.isActivated}, line:{line}")
            return True
        elif self.isActivated:
            self.result = "block comment      "

        return False


class AcessSpecifierParser(ObjectParser):
    def __init__(self):
        super().__init__()
        self.accessSpecifier = "+"  # defualt is 'public:'

        self.public = rx("public:")
        self.protected = rx("protected:")
        self.private = rx("private:")

    def processLine(self, line: str):
        if re.search(self.public, line):
            self.accessSpecifier = "+"
        elif re.search(self.protected, line):
            self.accessSpecifier = "#"
        elif re.search(self.private, line):
            self.accessSpecifier = "-"


class ClassConnectionsParser(ObjectParser):
    def __init__(self):
        super().__init__()
        self.isDerivedFrom = rx(": (public|private|protected) ([\w:<>]+)")
        self.compositionPattern = rx("^[ ]*([\w:]+)")
        self.aggregationPatterns = []
        self.DebugLogs = True

        # unique_ptr or shared_ptr
        self.aggregationPatterns.append(
            rx("(?<=unique_ptr<|shared_ptr<)[ ]*([\w:]+)"))
        # Pointer to the object: A* a; A const * const a;  etc.
        self.aggregationPatterns.append(rx("^[ ]*([\w:]+).*\*"))

    def log_new_connection(self, connectionTitle: str, tConnection: targets.TClassConnections):
        titleStr = f"New {connectionTitle} found"
        h1Size = 100
        titleLen = len(titleStr)
        marginsSize = 2
        h1SizeWithoutTitle = h1Size - titleLen - marginsSize
        halfSize = int(h1SizeWithoutTitle / 2)
        halfTitleStr = "*"*halfSize
        marginStr = " " * marginsSize
        fullTitle = halfTitleStr + marginStr + titleStr + marginStr + halfTitleStr
        print(fullTitle)
        print(
            f"C++: {tConnection.entireLine}\nUML: {tConnection.name}")
        print("*" * len(fullTitle))

    def check_derived(self, line: str, tclass: targets.TClass):
        match = re.search(self.isDerivedFrom, line)
        if match:
            className = tclass.name
            derivedFrom = match.group(2)

            # StressIndexLimitsWarning --|> "template<typename T> BoundsHelper"
            connectionStr = f"{derivedFrom} <|-- {className} : derived"
            tConnection = targets.TClassConnections(
                connectionStr, line, derivedFrom)

            tclass.addConnection(tConnection)

            self.log_new_connection("derived class", tConnection)

    def check_check_aggregation(self, line: str, tclass: targets.TClass):
        for aggregation in self.aggregationPatterns:
            match = re.search(aggregation, line)
            if match:
                className = tclass.name
                aggregatesClass = match.group(1)

                # UserDataController -* UserData
                connectionStr = f"{aggregatesClass} o-- {className} : aggregation"
                tConnection = targets.TClassConnections(
                    connectionStr, line, aggregatesClass)

                tclass.addConnection(tConnection)
                self.log_new_connection("aggregation", tConnection)

                return True
        return False

    def check_check_composition(self, line: str, tclass: targets.TClass):
        match = re.search(self.compositionPattern, line)
        if match:
            className = tclass.name
            compositsClass = match.group(1)

            # UserDataController -- UserData
            connectionStr = f"{compositsClass} *-- {className} : composition"
            tConnection = targets.TClassConnections(
                connectionStr, line, compositsClass)

            tclass.addConnection(tConnection)
            self.log_new_connection("composition", tConnection)

            return True
        return False

    def check_class_member_connections(self, line: str, tclass: targets.TClass):

        if "signal<" in line:
            self.log_debug(f"Found a signal in line: {line}")
            return False

        # Check aggregation
        if not self.check_check_aggregation(line, tclass):
            # If not aggregation check if it is a composition
            return self.check_check_composition(line, tclass)

        return True

    def processLine(self, line: str, tclass: targets.TClass):
        self.check_derived(line, tclass)


class ClassMemberParser(ObjectParser):
    def __init__(self):
        super().__init__()
        self.DebugLogs = True
        # Old version. Miss some captures
        # self.member = rx("^[ ]*[\w:<>]+ ([a-zA-Z0-9_])+[a-zA-Z0-9:_{}]*;")

        # rx: '(?!\().*(?<!\))' - match any line without () to filter functions
        # rx: ' ([\w]+)' - space(captured variable name)
        # rx: '[= {}\w]+;")' after variable name could be an intialization a={false} or a 1} ending with ';'
        # self.member = rx("^(?!\().*(?<!\)) ([\w]+)[= {}\w]+;")
        self.member = rx(
            "^(?!\().*(?<!\)) ([\w]+)[= {}\w]*;|^(?!\().*( [a-zA-Z_]+)[ ]*[{]+(?<!\))")

        self.accessSpecifierParser = AcessSpecifierParser()
        self.memberConnectionParser = ClassConnectionsParser()

    def check_is_class_member(self, line: str, tclass: targets.TClass):
        match = re.search(self.member, line)
        if match:
            name = match.group(1)
            enireLine = line

            print("check_is_class_member: {str(match.groups())}")

            if not name:
                name = match.group(2)
                rmPattern = rx(r"{.*")
                enireLine = re.sub(rmPattern, "", enireLine)

            if not name:
                name = match.group(3)

            # Re-check if it is not a function to filter something like:
            # ConnectedDevices(ConnectedDevices&&) = default;
            # which matches as: 'default' var of type 'ConnectedDevices'
            isSignal = "signal" in enireLine
            if not isSignal:
                isFunction = "(" in enireLine
                if isFunction:
                    return False

            self.log_debug(
                f"Found a new class member: class: {tclass.name} member:{name}, line: {line}")

            member = targets.TClassMember(name, enireLine)
            member.accessSpecifier = self.accessSpecifierParser.accessSpecifier

            tclass.addMember(member)
            return True
        return False

    def processLine(self, line: str, tclass: targets.TClass):
        self.log_debug(f"{tclass.name} processing member line: {line}")
        # Get access specifier: public, protected, private
        self.accessSpecifierParser.processLine(line)

        # Check if it's a class member
        if self.check_is_class_member(line, tclass):
            # Check composition\aggregation for the class member
            self.memberConnectionParser.check_class_member_connections(
                line, tclass)


class ClassMethodObjectParser(ObjectParser):
    def __init__(self):
        super().__init__()
        self.startPattern = rx(
            "[a-zA-Z0-9 \*&]+ ([_~a-zA-Z0-9]+)\(")
        self.endPattern = rx(
            "\) *\w*;|[ <>&\*->\w:]+;$")
        self.commaInTheEnd = rx(";")
        self.functionBodyStart = rx(" *{")
        self.functionBodyEnd = rx("^[ \t]+}(?!;)")
        self.completed = False
        self.opened = False
        self.accessSpecifierParser = AcessSpecifierParser()
        self.DebugLogs = True

    def process(self, line: str, tclass: targets.TClass):
        self.log_debug(f"{tclass.name} processing method line: {line}")
        # Get access specifier: public, protected, private
        self.accessSpecifierParser.processLine(line)

        if self.isActivated:
            # End function body
            if self.opened:
                # End function
                if re.search(self.functionBodyEnd, line):
                    self.opened = False
                    self.isActivated = False
                    self.completed = False
                    return True
            # Start function body
            elif re.search(self.functionBodyStart, line):
                self.opened = True
                return True
            elif re.search(self.endPattern, line):
                self.opened = False
                self.isActivated = False
                self.completed = False
                return True
        else:
            # Find next function
            match = re.search(self.startPattern, line)
            if(match):
                funcName = match.group(1)
                enireLine = line

                self.log_debug(f"Found a new function: {funcName}")
                method = targets.TClassMethod(funcName, enireLine)
                # Set access specifier for the method: +, #, -
                method.accessSpecifier = self.accessSpecifierParser.accessSpecifier

                tclass.addMethod(method)

                # Comma in the end, so this is declaration and completed function
                if re.search(self.commaInTheEnd, line):
                    self.completed = True
                else:
                    # No comma in the end, wait for body start
                    self.isActivated = True


class ClassObjectParser(ObjectParser):
    def __init__(self):
        super().__init__()
        self.methodParser = ClassMethodObjectParser()
        self.targetClass = targets.TClass
        self.startPattern = rx(
            # "^[\s]*(class|struct)\s([a-zA-Z0-9]+)[\s:<>]+{")
            # "^[\s]*(class|struct)\s([a-zA-Z0-9]+)[\s:<>a-zA-Z]+{")
            # "^[\s]*(class|struct)\s([a-zA-Z0-9]+)[\s:<>a-zA-Z]+{|^class [\w]+$")  # v3
            "^[\s]*(class|struct)\s(?P<name>[a-zA-Z0-9]+)[\s:<>a-zA-Z]+{|^[\s]*(class|struct) (?P<name1>[\w]+) [\w: ,]+$")  # v4
        self.simpleStartPattern = rx("^[\s]*(class|struct)\s(?P<name>[a-zA-Z0-9]+)")
        self.endPattern = rx("^[ ]*};")
        self.targetClassList = [targets.TClass()]
        self.memberParser = ClassMemberParser()
        self.inheritanceParser = ClassConnectionsParser()
        self.commentsParser = CommentsParser()
        self.DebugLogs = True

        self.targetClassList.clear()

    def skip_comments(self, line: str):
        commentFound = self.commentsParser.processLine(line)

        if self.commentsParser.isActivated or commentFound:
            print(
                f"Comments parser {self.commentsParser.result}: isActivated: {self.commentsParser.isActivated }, line: {line}", end="")
            return True
        return False

    def processLine(self, line: str):

        # print(f"ClassObjectParser processing line: {line}")

        # Don't process comments
        if self.skip_comments(line):
            return

        # Remove comments in the line
        commentInTheLine = rx(r"\/\*.*\*\/|//.*")
        line = re.sub(commentInTheLine, "", line)

        if self.isActivated:
            # Check for class end
            if re.search(self.endPattern, line):
                self.on_class_end()
            else:
                # Parse functions
                isFunction = self.methodParser.process(line, self.targetClass)

                if self.methodParser.isActivated or isFunction:
                    self.log_debug(
                        f"{self.targetClass} method is activated. Exiting...")
                    return

                # Parse class members
                self.memberParser.processLine(line, self.targetClass)

                # Parse Composition/Agregartion
                self.inheritanceParser.check_derived(line, self.targetClass)
        else:
            # Check for class start
            self.log_debug(f">>> Searching for a new class in line: {line}")
            match = re.search(self.startPattern, line)

            # Do simple check
            if not match:
                print(f"not matched: {line}. Try simple pattern")
                match = re.search(self.simpleStartPattern, line)
                # If line ends with ';' discard as a new class
                if match:
                    print(f"Found simple match: {line}")
                    if re.search(rx(";$"), line):
                        print("Disarding simple match, because line ends with ';'")
                        match = None

            if match:
                print(f"New class matched: {line}")

                className = match.group('name')

                if not className:
                    className = match.group('name1')

                if not className:
                    print(f"ERROR: class<{self.targetClass.name}> name is empty")
                    return

                self.log_debug(f"Found new class: {className}")

                self.on_class_begin(className, line)

    def on_class_begin(self, className: str, line: str):
        self.isActivated = True
        self.targetClass = targets.TClass(className, line)

        # Check derived
        self.inheritanceParser.check_derived(line, self.targetClass)

        globalClassLibrary.add_new_class(className)

    def on_class_end(self):
        self.isActivated = False

        self.targetClassList.append(self.targetClass)
        globalClassLibrary.add_new_target_class(self.targetClass)

        self.targetClass = None

    def parser_write(self, inpOut: uml_io.IO):

        # Write all classes
        for c in self.targetClassList:
            self.log_debug(f"Writing out: {c.name}", True)
            c.done_write(inpOut)
