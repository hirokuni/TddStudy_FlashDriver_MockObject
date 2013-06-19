# this is set up to by default to make the top level and test with CppUTest
#all: codeUnity

CPPUTEST = cpputest-3.4

all: codeCppUTest

#clean: cleanCodeUnity
clean: cleanCodeCppUTest

codeCppUTest: CppUTest CppUTestExt
	make -i -f MakefileCppUTest.mk 

cleanCodeCppUTest:
#	make -i -f MakefileCppUTest.mk clean
	rm lib/*.a
	rm -rf objs

everything: CppUTest CppUTestExt
	make -i -C CppUTest all extensions
	make -i -C t0
	make -i -C t1
	make -i -C t2
	make -i -C t3
	make -i -C SandBox
	make -i -f MakefileCppUTest.mk 
	make -i -f MakefileUnity.mk

cleaneverything:
	make -i -C t0 clean
	make -i -C t1 clean
	make -i -C t2 clean
	make -i -C t3 clean
	make -i -C SandBox clean
	make -i -f MakefileCppUTest.mk clean
	make -i -f MakefileUnity.mk clean
	make -i -C CppUTest

#-- if CppUTestExt gives you any problems, t1 is really the only project depending on it.
t1: CppUTestExt

CppUTest: $(CPPUTEST)/lib/libCppUTest.a

CppUTestExt: $(CPPUTEST)/lib/libCppUTestExt.a

$(CPPUTEST)/lib/libCppUTest.a:
	pwd
	make -i -C CppUTest

$(CPPUTEST)/lib/libCppUTestExt.a:
	make -i -C CppUTest extensions
