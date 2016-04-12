#!/usr/bin/python
#-*- coding: utf-8 -*-

import sys
import os
import commands
from subprocess import call

argc = len(sys.argv)

if argc < 6:
	print "Program wymaga 5 argumentów"
	exit()

#if argc == 1:
#	print "Podaj nazwę programu"
#	exit()

#if argc == 2:
#	os.environ["OMP_NUM_THREADS"] = sys.argv[2]
#elif 'OMP_NUM_THREADS' in os.environ:
#	del os.environ["OMP_NUM_THREADS"]

#print commands.getoutput("gcc -fopenmp {0}.c -o {0} && ./{0}".format(sys.argv[1]))


os.environ["OMP_NUM_THREADS"] = sys.argv[1]
print commands.getoutput("gcc -fopenmp {0}.c -o {0} && ./{0} {1} {2} {3}".format(sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5]))



