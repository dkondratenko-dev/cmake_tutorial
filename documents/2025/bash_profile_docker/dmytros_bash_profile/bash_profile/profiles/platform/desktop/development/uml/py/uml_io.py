#!/usr/bin/python

import os
# import sys
# import getopt
# import re


class Writable:
    def __init__(self, outFilePath, append=False):
        self.fout = None
        self.outFilePath = outFilePath
        self.opened = False
        self.append = append

        self.opened = True
        if self.append:
            self.fout = open(outFilePath, "a+")
        else:
            self.fout = open(outFilePath, "wt")

        # self.open()

    def __del__(self):
        self.close_write()

    def open_write(self):
        if self.opened:
            return True

        self.opened = True
        if self.append:
            self.fout = open(self.outFilePath, "a+")
        else:
            self.fout = open(self.outFilePath, "wt")

        return True

    def close_write(self):
        if not self.opened:
            return False

        self.fout.close()
        self.opened = False
        return True

    def write(self, line: str):
        self.fout.write(line)


class Readable:
    def __init__(self, inFilePath):
        self.fin = open(inFilePath, "rt")
        self.inFilePath = inFilePath
        self.opened = True

    def close_read(self):
        if self.opened:
            self.fin.close()

    def open_read(self):
        if self.opened:
            self.close_read()

        self.fin = open(self.inFilePath, "rt")
        return True

    def __del__(self):
        self.close_read()


class IO(Readable, Writable):
    def __init__(self, inFilePath, outFilePath, append=False):
        Readable.__init__(self, inFilePath)
        Writable.__init__(self, outFilePath, append)

    def open(self):
        self.open_read()
        self.open_write()

    def close(self):
        self.close_read()
        self.close_write()
