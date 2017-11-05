VPATH       = src:test
INC_DIR     = inc
BUILD_DIR   = build
BIN_DIR     = bin
3P_DIR      = 3p
CMOCKA_INC_DIR = 3p/cmocka-1.1.1/include/

MKDIR_P = mkdir -p
RM_F    = rm -f
MV_F    = mv -f

OUTPUT_NAME = project1

TEST_OUTPUT_NAME = test_project1

SRCS  = main.c \
        light.c \
		temp.c \
		log.c \
		msg.c \

TEST_SRCS = temp.c \
			light.c \
			log.c \
			msg.c \
			test_light_conv.c \
			test_temp_conv.c \
			test_light_rw.c \
			test_temp_rw.c \
			test_main.c

OBJS := $(SRCS:.c=.o)

TEST_OBJS := $(TEST_SRCS:.c=.o)

CFLAGS = -std=gnu99 -g -O0 -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -I$(INC_DIR) -I$(CMOCKA_INC_DIR)

LDFLAGS = -lrt -lmraa -pthread -lm

TESTFLAGS = -lcmocka

CC = gcc

# Targets
##############################################################################

all: build

$(BIN_DIR)/$(OUTPUT_NAME): $(addprefix $(BUILD_DIR)/, $(OBJS))
	@$(MKDIR_P) $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c
	@$(MKDIR_P) $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/$(TEST_OUTPUT_NAME): $(addprefix $(BUILD_DIR)/, $(TEST_OBJS))
	    @$(MKDIR_P) $(BIN_DIR)
		$(CC) $(CFLAGS) $(TESTFLAGS) -o $@ $^ $(LDFLAGS)

# Remaps an individual object file to the correct folder
.PHONY: %.o
%.o: $(BUILD_DIR)/%.o
	@echo Output will be in build folder

# Build all files and link
.PHONY: build
build: $(BIN_DIR)/$(OUTPUT_NAME)

# Build test suite and execute
.PHONY: test
test:  $(BIN_DIR)/$(TEST_OUTPUT_NAME)
	$(BIN_DIR)/$(TEST_OUTPUT_NAME)

# Deletes build files, leaves executables
.PHONY: clean
clean:
	@$(RM_F) $(BUILD_DIR)/*
	@$(RM_F) $(BIN_DIR)/*
	@echo Successfully cleaned.

