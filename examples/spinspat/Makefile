NAME=spinspat

ifeq ($(shell uname -s),Darwin)
    OS:=Darwin
else
    OS:=Linux
endif

all: $(NAME)

########################################################

DEPS = spatosc-0.1 liblo

CXXFLAGS = -g -I/usr/include/boost `pkg-config --cflags $(DEPS)` `python-config --cflags`
LDFLAGS = -lSPIN -lboost_python `pkg-config --libs $(DEPS)` `python-config --libs`

ifeq ($(shell uname -s),Darwin)
  CXXFLAGS += -I/opt/local/include
  #CXXFLAGS += -DDARWIN -D__USE_OSX_AGL_IMPLEMENTATION__
  LDFLAGS += -lpthread \
          -lOpenThreads -losg -losgGA -losgSim -losgDB -losgIntrospection \
          -losgManipulator -losgText -losgUtil -losgFX -losgParticle -losgViewer
  LDFLAGS += -L/usr/local/lib -L/opt/local/lib
else
  LDFLAGS += -lc -lm -ldl -lGL -lX11 -lpthread \
          -lOpenThreads -losg -losgGA -losgSim -losgDB -losgIntrospection \
          -losgManipulator -losgText -losgUtil -losgFX -losgParticle -losgViewer
  LDFLAGS += -L/usr/local/lib/osgPlugins \
          -L/usr/local/lib64\
          -L/usr/local/lib64/osgPlugins \
          -L/usr/X11R6/lib 
endif


###################################################################
# TARGETS:

%.o: %.cpp %.h
	@echo "--------------- compiling $@ ---------------" 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): spinspat.o
	@echo "\n--------------- linking $@: ---------------" 
	$(CXX) -o $@ $^ $(LDFLAGS)

########################################################
install:
	-cp $(NAME) /usr/local/bin


########################################################
clean:
	-rm -f $(NAME) *.o
