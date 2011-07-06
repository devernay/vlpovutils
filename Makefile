# This is a -*- Makefile -*- for the vlpov Library
# Warning: this file contains tabs which cannot be converted to spaces

# TOP_SRCDIR must be relative
TOP_SRCDIR = ../..
SRCDIR = .

include $(TOP_SRCDIR)/config/default.mk
-include $(TOP_SRCDIR)/config/local.mk
-include $(TOP_SRCDIR)/config/local-$(CONFIG).mk


CPPFLAGS= \
	$(BOOST_CPPFLAGS) \
	-I$(SRCDIR)/$(TOP_SRCDIR) -I$(SRCDIR)

LIBS = \
	$(BOOST_LDFLAGS) \
	$(BOOST_LIB_MATH_C99)

#CFLAGS_EXTRA=$(CFLAGS_OPENMP)
CXXFLAGS=$(CFLAGS_OPT) $(CFLAGS_EXTRA)
CFLAGS=$(CFLAGS_OPT) $(CFLAGS_EXTRA)
LDFLAGS=$(CFLAGS_OPT) $(CFLAGS_EXTRA) $(LDFLAGS_EXTRA)

COMPILE.c=$(CC) -c $(CFLAGS) $(CPPFLAGS)
COMPILE.cpp=$(CXX) -c $(CXXFLAGS) $(CPPFLAGS)

# for gcov profiling add:
#-fprofile-arcs -ftest-coverage


PROGRAMS = vlpov_motionfield vlpov_motionfield2 vlpov_project

LIBRARY1 = libvlpov.a
LIBRARIES = $(LIBRARY1)

all: $(LIBRARIES) $(PROGRAMS)

lib: $(LIBRARIES)

vlpov_SOURCES_CPP = \
	look2motion.cpp \
	motionfield.cpp \
	occlusionmap.cpp \
	povread.cpp \
	vec2camera.cpp \
	vec2motion.cpp

vlpov_motionfield_SOURCES_CPP = \
	vlpov_motionfield.cpp

vlpov_motionfield2_SOURCES_CPP = \
	vlpov_motionfield2.cpp

vlpov_project_SOURCES_CPP = \
	vlpov_motionfield2.cpp

vlpov_HEADERS = \
	vlpov.hpp

vlpov_LIBS =

vlpov_OBJS =  $(vlpov_SOURCES_CPP:.cpp=.o)
vlpov_motionfield_OBJS =  $(vlpov_motionfield_SOURCES_CPP:.cpp=.o)
vlpov_motionfield2_OBJS =  $(vlpov_motionfield2_SOURCES_CPP:.cpp=.o)
vlpov_project_OBJS =  $(vlpov_project_SOURCES_CPP:.cpp=.o)

SRCS_CPP = \
	$(vlpov_SOURCES_CPP) \
	$(vlpov_motionfield_SOURCES_CPP) \
	$(vlpov_motionfield2_SOURCES_CPP) \
	$(vlpov_project_SOURCES_CPP) \

HEADERS = \
	$(surf_HEADERS)

vlpov_motionfield: $(vlpov_motionfield_OBJS) $(LIBRARY1)
	$(LD) -o $@ $^ $(LDFLAGS) $(vlpov_LIBS) $(LDADD)

vlpov_motionfield2: $(vlpov_motionfield2_OBJS) $(LIBRARY1)
	$(LD) -o $@ $^ $(LDFLAGS) $(vlpov_LIBS) $(LDADD)

vlpov_project: $(vlpov_project_OBJS) $(LIBRARY1)
	$(LD) -o $@ $^ $(LDFLAGS) $(vlpov_LIBS) $(LDADD)

$(LIBRARY1): $(vlpov_OBJS)
	$(AR) rvu $@ $^
	$(RANLIB) $@

.SUFFIXES: .o .cpp

## gcc-only version:
%.o : %.cpp
	$(COMPILE.cpp) -MD -o $@ $<
	@cp $*.d $*.P; \
	    sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	        -e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; \
	    rm -f $*.d

## general version:
#MAKEDEPEND = gcc -M $(CPPFLAGS) -o $*.d $<
# %.o : %.cpp
# 	@$(MAKEDEPEND); \
# 	    cp $*.d $*.P; \
# 	    sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
# 		-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; \
# 	    rm -f $*.d
# 	$(COMPILE.cpp) -o $@ $<

.PHONY: clean distclean
clean:
	-rm -rf $(PROGRAMS) $(LIBRARIES) *.o  *~ *.dSYM

distclean: clean
	-rm -f $(SRCS_CPP:.cpp=.P)

count:
	 wc -l $(SRCS_CPP) $(HEADERS)

-include $(SRCS_CPP:.cpp=.P)
