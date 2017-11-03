CXXFLAGS=-Iinterface/ `root-config --cflags`
SO_FLAGS=-shared -fPIC
LD_FLAGS=`root-config --ldflags`
ROOT_LIBS=`root-config --glibs`

default:libPlotter.so

PlotterDict.cxx:interface/Plotter.h interface/LinkDef.h
	rootcint -f $@ -c $(CXXFLAGS) -p $^

libPlotter.so: src/Plotter.cxx interface/Plotter.h PlotterDict.cxx 
	$(CXX) $(SO_FLAGS)  $(CXXFLAGS) $^ -o $@ $(LD_FLAGS) $(ROOT_LIBS)

clean:
	rm -rf libPlotter.so  PlotterDict.cxx  PlotterDict_rdict.pcm
