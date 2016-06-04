CC       := gcc
CC_FLAGS := -Wall -O3 -g3
LIBS     := -lGL -lGLU -lglut

GLEM_OBJ := obj/server.o
SAMPLE_OBJ := obj/example.o obj/glcd.o

all: dirs glem sample

obj/%.o: src/%.c
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" -c "$<"

glem: $(GLEM_OBJ)
	@$(CC) $(CC_FLAGS) -o bin/$@.elf $^ $(LIBS)

sample: $(SAMPLE_OBJ)
	@$(CC) $(CC_FLAGS) -o bin/$@.elf $^ $(LIBS)

dirs:
	@mkdir -p bin
	@mkdir -p obj

clean:
	@rm -rf bin/* obj/*

.PHONY: clean glem sample

