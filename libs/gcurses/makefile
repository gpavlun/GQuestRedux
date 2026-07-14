CC := gcc
AR := ar

# ───────────────────────────────────────────────────────
# Terminal Colors
# ───────────────────────────────────────────────────────

GREEN  := \033[0;32m
BLUE   := \033[0;34m
RED    := \033[0;31m
CYAN   := \033[0;36m
RESET  := \033[0m

# ───────────────────────────────────────────────────────
# Compiler Options
# ───────────────────────────────────────────────────────

WARN ?= 0

ifeq ($(WARN),1)
    WARNFLAGS := -Wall -Wextra
else
    WARNFLAGS :=
endif

CFLAGS := $(WARNFLAGS) -g \
          -Iinclude \
          -I../objects/include \
          -MMD -MP

# ───────────────────────────────────────────────────────
# Directories
# ───────────────────────────────────────────────────────

SRC_DIR := src
BUILD_DIR := build
LIB_DIR := lib

TARGET := $(LIB_DIR)/libgcurses.a

DEMO := demo
DEMO_SRC := demo.c

LDFLAGS := \
	-L$(LIB_DIR) \
	-L../objects/lib

LDLIBS := \
	-lgcurses \
	-lobjects

# ───────────────────────────────────────────────────────
# Sources
# ───────────────────────────────────────────────────────

SRCS := $(shell find $(SRC_DIR) -name "*.c")

OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

DEPS := $(OBJS:.o=.d)

# ───────────────────────────────────────────────────────
# Default Targets
# ───────────────────────────────────────────────────────

.PHONY: all demo clean objects

all: $(TARGET)

# ───────────────────────────────────────────────────────
# External Libraries
# ───────────────────────────────────────────────────────

objects:
	@echo "$(CYAN)[MAKE]$(RESET) objects"
	@$(MAKE) -C ../objects

# ───────────────────────────────────────────────────────
# Static Library
# ───────────────────────────────────────────────────────

$(TARGET): objects $(OBJS)
	@mkdir -p $(LIB_DIR)
	@echo "$(CYAN)[AR]$(RESET) $@"
	@$(AR) rcs $@ $(OBJS)

# ───────────────────────────────────────────────────────
# Demo
# ───────────────────────────────────────────────────────

demo: $(TARGET)
	@echo ""
	@echo "$(CYAN)┌──────────────────────────────────────┐$(RESET)"
	@echo "$(CYAN)│ Building Demo                        │$(RESET)"
	@echo "$(CYAN)└──────────────────────────────────────┘$(RESET)"
	@echo "$(GREEN)[LD]$(RESET) $(DEMO)"
	@$(CC) \
		$(CFLAGS) \
		$(DEMO_SRC) \
		$(LDFLAGS) \
		$(LDLIBS) \
		-o $(DEMO)

# ───────────────────────────────────────────────────────
# Compilation
# ───────────────────────────────────────────────────────

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "$(BLUE)[CC]$(RESET) $<"
	@$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

# ───────────────────────────────────────────────────────
# Utility Targets
# ───────────────────────────────────────────────────────

clean:
	@echo "$(RED)[CLEAN]$(RESET) Removing library build artifacts..."
	@rm -rf $(BUILD_DIR) $(LIB_DIR) $(DEMO)