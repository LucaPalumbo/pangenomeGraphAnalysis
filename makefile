#.DEFAULT_GOAL := compile

compile:
	time \
	g++ -o main main.cpp \
		pangenomeGraph.cpp datasetParser.cpp util.cpp path.cpp segment.cpp \
		karpRabin.cpp \
		primeGenerator.cpp \
		-g -O3

fast:
	time \
	g++ -o main main.cpp \
		pangenomeGraph.cpp datasetParser.cpp util.cpp path.cpp segment.cpp \
		karpRabin.cpp \
		primeGenerator.cpp \