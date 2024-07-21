#.DEFAULT_GOAL := compile

compile:
	g++ -o main main.cpp \
		pangenomeGraph.cpp datasetParser.cpp util.cpp path.cpp segment.cpp \
		karpRabin.cpp \
		-g -O3
