# Project configuration.
TARGET_NAME := containerof_interface
INC_DIRS := ./src/inc
BIN_DIRS := ./src/bin
LIB_DIRS := ./src/lib

# Build-profile configuration.
B ?= debug
override BUILD_PROFILES := debug profile release
_BUILD_DIR ?= ./_build

# Build-profile error checking.
override B := $(strip $(B))

ifeq (,$(filter $(BUILD_PROFILES),$(B)))
$(error Unknown B="$(B)"; should be one of: { $(patsubst %,"%",$(BUILD_PROFILES)) })
endif

override __empty :=
override __space := $(__empty) $(__empty)

ifeq ($(__space),$(findstring $(__space),$(B)))
$(error Cannot specify multiple space-separated options for B; B="$(B)" given)
endif

# C-specific configuration.
ifeq (release,$(B))
OPTFLAGS := -Ofast -flto
LDFLAGS += -s
else ifeq (profile,$(B))
OPTFLAGS := -Ofast -flto -g3
else # debug
OPTFLAGS := -fdebug-prefix-map=..=$$(readlink -f ..) -O0 -g3
endif

LDLIBS := -lm
CPPFLAGS := -MMD -MP
override CPPFLAGS += -D=_GNU_SOURCE
CFLAGS := --std=c11 -Wall -Wextra -Wpedantic
override CFLAGS += $(addprefix -I,$(INC_DIRS))

# Static configuration.
override FINAL_TARGET := $(_BUILD_DIR)/$(TARGET_NAME)
override BUILD_TARGET := $(_BUILD_DIR)/$(B)/$(TARGET_NAME)

override BIN_SRCS := $(shell find $(BIN_DIRS) -name '*.c')
override BIN_OBJS := $(BIN_SRCS:%.c=$(_BUILD_DIR)/$(B)/%.o)

override LIB_SRCS := $(shell find $(LIB_DIRS) -name '*.c')
override LIB_OBJS := $(LIB_SRCS:%.c=$(_BUILD_DIR)/$(B)/%.o)

override OBJS := $(BIN_OBJS) $(LIB_OBJS)
override DEPS := $(OBJS:.o=.d)

# Targets.
.PHONY: $(FINAL_TARGET)
$(FINAL_TARGET): $(BUILD_TARGET)
	@mkdir -p $(@D)
	cp $< $@

$(BUILD_TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(OPTFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJS): $(_BUILD_DIR)/$(B)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $(OPTFLAGS) $< -o $@

.PHONY: run
run: $(FINAL_TARGET)
	$(info Running "$(TARGET_NAME)" benchmark...)
	@stdbuf -o0 $(FINAL_TARGET) | head -n 5

.PHONY: clean
clean:
	@rm -rf $(_BUILD_DIR)
	@rm -rf $(shell find . -name '*.o')
	@rm -rf $(shell find . -name '*.d')
	@rm -rf $(shell find . -name '*.gch')

-include $(DEPS)
