#This makefile makes all the main book code with CppUTest test harness

#Set this to @ to keep the makefile quiet
SILENCE = @

#---- Outputs ----#
COMPONENT_NAME = BookCode_CppUTest

#--- Inputs ----#
CPPUTEST = cpputest-3.4
CPPUTEST_HOME = ./$(CPPUTEST)
CPPUTEST_USE_EXTENSIONS = Y
CPP_PLATFORM = Gcc
PROJECT_HOME_DIR = .
CPPUTEST_LIB_DIR = ./lib

#SRC_DIRS = \
#	src/HomeAutomation \
#	src/util\
#	src/LedDriver \
#	src/dvr\
#	src/IO \
#	src/MyOS \
#	src/MyOS/posix \
#	src/zune \

SRC_FILES =\
	src/IO/Flash.c\
#SRC_DIRS:=$(shell find ./ -name "*.c" -a ! -name "IO.c")

#TEST_SRC_DIRS = \
#	.\
#	mocks\
#	mocks\
#	tests/LedDriver\
#	tests/stdio\
#	tests/util\
#	tests/IO\
#	tests/zune\
#	tests/HomeAutomation\
#	tests/dvr\
#	tests\
#	tests/MyOS\

TEST_SRC_FILES = \
	mocks/IO/MockIO.cpp\
	mocks/FakeMicroTime.c\
	tests/IO/FlashTest.cpp\
	tests/AllTests.cpp\
	
	
INCLUDE_DIRS =\
  .\
  $(CPPUTEST)/include\
  include\
  include/IO\
  mocks\
  include/util\
  include/HomeAutomation\
  include/LedDriver\
  include/MyOS\
  include/dvr\
  include/zune\

MOCKS_SRC_DIRS = \
	mocks\

#�R���p�C���Ώۂ���͂���
#SRC_DIRS=$(shell find ./ -name "*.c" -a ! -name "IO.c")

#����̃f�B���N�g���ȉ����r���h�ΏۊO�ɂ�����
#SRCS=$(shell C:/cygwin/bin/find.exe . -name "*.cc" -print -o -path "./x86_64/RHEL5/1.0/batch" -prune)

CPPUTEST_WARNINGFLAGS = -Wall -Wswitch-default -Werror 
#CPPUTEST_CFLAGS = -std=c89 
CPPUTEST_CFLAGS += -Wall -Wstrict-prototypes -pedantic
LD_LIBRARIES = -lpthread
	
  
include $(CPPUTEST_HOME)/build/MakefileWorker.mk



