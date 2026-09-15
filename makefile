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

all: makefile $(OUTDIR)/type1_py2k2.c $(OUTDIR)/type1_cave.c $(OUTDIR)/type1_puzzli2.c $(OUTDIR)/type1_puzzli2s.c $(OUTDIR)/type3_demonfront.c $(OUTDIR)/type1_oldsplus.c

################################################################################
# Oriental Legend Super

OLDSPSRC = startup.s main.c oldsplus.c
OLDSPOBJ = $(patsubst %.s,$(OBJDIR)/%.o, $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(OLDSPSRC))))
OLDSPDEP = $(OLDSPOBJ:%.o=%.d)

$(OUTDIR)/type1_oldsplus.elf: $(OLDSPOBJ) $(LINKSCRIPT) | $(OUTDIR)
	$(LN) $(LFLAGS) $(OLDSPOBJ) -o $@

################################################################################
# Demon Front

DMNFRSRC = startup_demonfront.s
DMNFROBJ = $(patsubst %.s,$(OBJDIR)/%.o, $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(DMNFRSRC))))
DMNFRDEP = $(DMNFROBJ:%.o=%.d)

$(OUTDIR)/type3_demonfront.elf: $(DMNFROBJ) $(LINKSCRIPT) | $(OUTDIR)
	$(LN) $(LFLAGS) $(DMNFROBJ) -o $@

################################################################################
# Puzzli2

PZLI2SRC = startup.s main.c puzzli2.c
PZLI2OBJ = $(patsubst %.s,$(OBJDIR)/%.o, $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(PZLI2SRC))))
PZLI2DEP = $(PZLI2OBJ:%.o=%.d)

$(OUTDIR)/type1_puzzli2.elf: $(PZLI2OBJ) $(LINKSCRIPT) | $(OUTDIR)
	$(LN) $(LFLAGS) $(PZLI2OBJ) -o $@

################################################################################
# Puzzli2 Super

PZLI2SSRC = startup.s main.c puzzli2s.c
PZLI2SOBJ = $(patsubst %.s,$(OBJDIR)/%.o, $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(PZLI2SSRC))))
PZLI2SDEP = $(PZLI2SOBJ:%.o=%.d)

$(OUTDIR)/type1_puzzli2s.elf: $(PZLI2SOBJ) $(LINKSCRIPT) | $(OUTDIR)
	$(LN) $(LFLAGS) $(PZLI2SOBJ) -o $@

################################################################################
# Photo Y2K2

PY2K2SRC = startup.s main.c py2k2.c
PY2K2OBJ = $(patsubst %.s,$(OBJDIR)/%.o, $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(PY2K2SRC))))
PY2K2DEP = $(PY2K2OBJ:%.o=%.d)

$(OUTDIR)/type1_py2k2.elf: $(PY2K2OBJ) $(LINKSCRIPT) | $(OUTDIR)
	$(LN) $(LFLAGS) $(PY2K2OBJ) -o $@

################################################################################
# Cave games (ddp3, ket and espgal)

CAVESRC = startup.s main.c cave.c
CAVEOBJ = $(patsubst %.s,$(OBJDIR)/%.o, $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(CAVESRC))))
CAVEDEP = $(CAVEOBJ:%.o=%.d)

$(OUTDIR)/type1_cave.elf: $(CAVEOBJ) $(LINKSCRIPT) | $(OUTDIR)
	$(LN) $(LFLAGS) $(CAVEOBJ) -o $@

################################################################################

-include $(OLDSPDEP)
-include $(DMNFRDEP)
-include $(PZLI2DEP)
-include $(PZLI2SDEP)
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
