# Configuration.
B ?= debug
_BUILD_DIR ?= ./_build

# Error checking.
override __empty :=
override __space := $(__empty) $(__empty)
override B := $(strip $(B))

ifeq (,$(filter debug profile release,$(B)))
$(error Unknown B=$(B). Should be one of: B=debug, B=profile, or B=release)

else ifeq ($(__space),$(findstring $(__space),$(B)))
$(error Cannot specify multiple space-separated options for B; B=$(B) given)

endif

# Project-related defines.
override TARGET_NAME := containerof_interface

INC_DIRS := ./inc

BIN_DIRS := ./bin
BIN_SRCS := $(shell find $(BIN_DIRS) -name '*.c')
BIN_OBJS := $(BIN_SRCS:%.c=$(_BUILD_DIR)/$(B)/%.o)

LIB_DIRS := ./lib
LIB_SRCS := $(shell find $(LIB_DIRS) -name '*.c')
LIB_OBJS := $(LIB_SRCS:%.c=$(_BUILD_DIR)/$(B)/%.o)

OBJS := $(BIN_OBJS) $(LIB_OBJS)
DEPS := $(OBJS:.o=.d)

LDLIBS := -lm
CPPFLAGS := -MMD -MP
CFLAGS := --std=c11 -Wall -Wextra -Wpedantic
override CFLAGS += $(addprefix -I,$(INC_DIRS))

$(_BUILD_DIR)/$(TARGET_NAME): $(_BUILD_DIR)/$(B)/$(TARGET_NAME)
	cp -RP $< $(@D)

$(_BUILD_DIR)/debug/$(TARGET_NAME): override OPTFLAGS := -fdebug-prefix-map=..=$$(readlink -f ..) -O0 -g3
$(_BUILD_DIR)/debug/$(TARGET_NAME): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(OPTFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(_BUILD_DIR)/profile/$(TARGET_NAME): override OPTFLAGS := -Ofast -flto -g3
$(_BUILD_DIR)/profile/$(TARGET_NAME): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(OPTFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(_BUILD_DIR)/release/$(TARGET_NAME): override OPTFLAGS := -Ofast -flto
$(_BUILD_DIR)/release/$(TARGET_NAME): override LDFLAGS += -s
$(_BUILD_DIR)/release/$(TARGET_NAME): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(OPTFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJS): $(_BUILD_DIR)/$(B)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $(OPTFLAGS) $< -o $@

.PHONY: clean
clean:
	@rm -rf $(_BUILD_DIR)
	@rm -rf $(shell find . -name '*.o')
	@rm -rf $(shell find . -name '*.d')
	@rm -rf $(shell find . -name '*.gch')

-include $(DEPS)
