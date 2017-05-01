import argparse
import glob
import os
import subprocess


cmdline_parser = argparse.ArgumentParser()

cmdline_parser.add_argument("path", help="pass the desired working dir here"
" (relative or absolute)")

args = cmdline_parser.parse_args()
 
os.chdir(args.path)

folder_content = glob.glob("bin_*")
if len(folder_content) != 1 or not os.path.isdir(folder_content[0]):
    raise RuntimeError("didn't find a unique bin_* subdir")

os.rename(folder_content[0], "bin")

os.chdir("_clean")
subprocess.call("ReleaseFolderCleaner.exe")
os.chdir("..")
