CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = game

SRCS = main.cpp \
       GameController.cpp \
       GameGrid.cpp \
       GridCell.cpp \
       GameActor.cpp \
       PlayerCharacter.cpp \
       EnemyCharacter.cpp \
       EnemyStructure.cpp \
       SpellCard.cpp \
       DirectDamageSpell.cpp \
       AreaDamageSpell.cpp \
       SpellHand.cpp

OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run
