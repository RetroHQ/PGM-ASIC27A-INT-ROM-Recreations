BINDIR = bin
OBJDIR = obj
OUTDIR = out

AS = arm-none-eabi-as
CC = arm-none-eabi-gcc
LN = arm-none-eabi-gcc
OC = arm-none-eabi-objcopy
XXD = xxd

LINKSCRIPT = type1.ld

CFLAGS = -march=armv4t -mcpu=arm7tdmi -ffunction-sections -fdata-sections -Wreturn-type -Wno-multichar -O3 -fno-tree-loop-distribute-patterns
SFLAGS = -march=armv4t -mcpu=arm7tdmi
LFLAGS = -march=armv4t -mcpu=arm7tdmi -nostdlib -nostartfiles -Wl,-T,$(LINKSCRIPT),--gc-sections,-Map=$@.map

################################################################################

all: makefile $(OUTDIR)/type1_py2k2.c $(OUTDIR)/type1_cave.c $(OUTDIR)/type1_puzzli2.c

################################################################################

PZLI2SRC = startup.s main.c puzzli2.c
PZLI2OBJ = $(patsubst %.s,$(OBJDIR)/%.o, $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(PZLI2SRC))))
PZLI2DEP = $(PZLI2OBJ:%.o=%.d)

$(OUTDIR)/type1_puzzli2.elf: $(PZLI2OBJ) $(LINKSCRIPT) | $(OUTDIR)
	$(LN) $(LFLAGS) $(PZLI2OBJ) -o $@

################################################################################

PY2K2SRC = startup.s main.c py2k2.c
PY2K2OBJ = $(patsubst %.s,$(OBJDIR)/%.o, $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(PY2K2SRC))))
PY2K2DEP = $(PY2K2OBJ:%.o=%.d)

$(OUTDIR)/type1_py2k2.elf: $(PY2K2OBJ) $(LINKSCRIPT) | $(OUTDIR)
	$(LN) $(LFLAGS) $(PY2K2OBJ) -o $@

################################################################################

CAVESRC = startup.s main.c cave.c
CAVEOBJ = $(patsubst %.s,$(OBJDIR)/%.o, $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(CAVESRC))))
CAVEDEP = $(CAVEOBJ:%.o=%.d)

$(OUTDIR)/type1_cave.elf: $(CAVEOBJ) $(LINKSCRIPT) | $(OUTDIR)
	$(LN) $(LFLAGS) $(CAVEOBJ) -o $@

################################################################################

-include $(PZLI2DEP)
-include $(PY2K2DEP)
-include $(CAVEDEP)

$(OUTDIR)/%.c: $(OUTDIR)/%.bin | $(OUTDIR)
	$(XXD) -i -c 16 $< > $@

$(OUTDIR)/%.bin: $(OUTDIR)/%.elf | $(OUTDIR)
	$(OC) -I elf32-littlearm $< -O binary $@

$(OBJDIR)/%.o: %.c makefile | $(OBJDIR)
	$(CC) $(CFLAGS) -MMD -c -o $@ $<

$(OBJDIR)/%.o: %.s makefile | $(OBJDIR)
	$(AS) $(SFLAGS) $< -o $@

$(BINDIR):
	mkdir $(BINDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

$(OUTDIR):
	mkdir $(OUTDIR)

################################################################################

clean:
ifneq (,$(wildcard $(OBJDIR)))
	rm -f $(OBJDIR)/*
	rmdir $(OBJDIR)
endif
ifneq (,$(wildcard $(OUTDIR)))
	rm -f $(OUTDIR)/*
	rmdir $(OUTDIR)
endif
ifneq (,$(wildcard $(BINDIR)))
	rm -f $(BINDIR)/*
	rmdir $(BINDIR)
endif
