# default header
import os
env = Environment(
	CXX = "clang++",
	CXXFLAGS = '-std=c++11',
	CCFLAGS = ['-O3'],
	CPPPATH = [
	],
	LIBPATH = [
	])
env['ENV']['TERM'] = os.environ['TERM']

# project specific code
env.Program(
	'jee',
	source = [
		'main.cpp',
		],
	LIBS = [
		])
