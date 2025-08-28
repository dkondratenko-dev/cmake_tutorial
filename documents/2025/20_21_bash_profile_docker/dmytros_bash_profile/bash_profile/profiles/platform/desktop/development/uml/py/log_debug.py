#!/usr/bin/python

import os


class LogDebug:
    def __init__(self):
        self.DebugLogs = False

    def log_debug(self, msg: str, forcePrint=False):
        if self.DebugLogs or forcePrint:
            print(f"DEBUG: {msg}")
