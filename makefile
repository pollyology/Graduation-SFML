# For use of SFML projects in Windows or Linux
SRC_DIR := ./code
OBJ_DIR := .

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Detect OS
ifeq ($(OS),Windows_NT)
	# --- Windows Settings ---
	TARGET := Graduation.exe
	LDFLAGS := -LC:/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
	CXXFLAGS := -g -Wall -fpermissive -std=c++17 -IC:/SFML/include
	RUN_CMD := $(TARGET)
	CLEAN_CMD := del /Q $(TARGET) *.o
else
	# --- Linux Settings ---
	TARGET := game.out
	LDFLAGS := -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
	CXXFLAGS := -g -Wall -fpermissive -std=c++17 -I/opt/homebrew/include
	RUN_CMD := ./$(TARGET)
	CLEAN_CMD := rm -f $(TARGET) *.o
endif

$(TARGET): $(OBJ_FILES)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: $(TARGET)
	$(RUN_CMD)

clean:
	$(CLEAN_CMD)
