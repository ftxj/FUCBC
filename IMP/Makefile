CC			:= g++
LD			:= g++

MODULES		:= data_structure tfAPI tfnode optimization
SRC_DIR		:= $(addprefix src/,$(MODULES))
BUILD_DIR	:= $(addprefix build/,$(MODULES))

SRC			:= $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJ			:= $(patsubst src/%.cpp,build/%.o,$(SRC))
INCLUDES	:= -I ./include/

vpath %.cpp $(SRC_DIR)

define make-goal
$1/%.o: %.cpp
	echo 1
	$(CC) $(INCLUDES) -c $$< -o $$@
endef

.PHONY: all checkdirs clean

all: checkdirs build/test.exe

build/test.exe: $(OBJ)
	$(LD) $^ -o $@

checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(BUILD_DIR)

$(foreach bdir, $(BUILD_DIR), $(eval $(call make-goal, $(bdir))))