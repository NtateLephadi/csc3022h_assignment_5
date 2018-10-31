TEST = test
TEST_BIN = test/bin/audio_test
TEST_AUDIO = test/audio
TEST_SOURCE = src/audio.cpp src/audio_test.cpp
SOURCE = src/audio.cpp src/audioops.cpp
BIN = $(SOURCE:.cpp=.o)
PICTURES = audio
EXECUTABLE = audioops
CC = g++
FLAGS = --std=c++11
WARNING = -w

# link files and send to bin
default: $(BIN)
	$(CC) $(SOURCE) -o bin/$(EXECUTABLE) $(FLAGS) $(WARNING)

.cpp.o:
	$(CC) -c $< -o bin/audio.o $(FLAGS) $(WARNING)

# link all test files and send to test/bin
test: $(TEST_SOURCE)
	$(CC) $(TEST_SOURCE) -o $(TEST_BIN) $(FLAGS) $(WARNING)

# run audioops with no command line entries
run: default
	cd ./bin && ./$(EXECUTABLE) $(WARNING)

# run all tests
run_test: test
	cd ./test/bin && ./audio_test

# remove all .o and .exe files
clean:
	rm -f bin/audio.o bin/$(EXECUTABLE) $(TEST_BIN)
