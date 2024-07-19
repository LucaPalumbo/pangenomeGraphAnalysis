#.DEFAULT_GOAL := compile

compile:
	g++ -o main main.cpp \
		gfaGraph.cpp datasetParser.cpp util.cpp path.cpp segment.cpp \
		karpRabin.cpp \
		-O3
