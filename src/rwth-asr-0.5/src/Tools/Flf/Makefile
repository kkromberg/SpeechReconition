#!gmake

TOPDIR		= ../../..

include $(TOPDIR)/Makefile.cfg

# -----------------------------------------------------------------------------

SUBDIRS 	=
TARGETS		= flf-tool$(exe)

FLF_TOOL_O	= $(OBJDIR)/FlfTool.o \
		  ../../Flf/libSprintFlf.$(a) \
		  ../../Flf/FlfCore/libSprintFlfCore.$(a) \
		  ../../Speech/libSprintSpeech.$(a) \
		  ../../Search/libSprintSearch.$(a) \
		  ../../Lattice/libSprintLattice.$(a) \
		  ../../Lm/libSprintLm.$(a) \
		  ../../Am/libSprintAm.$(a) \
		  ../../Mm/libSprintMm.$(a) \
		  ../../Mc/libSprintMc.$(a) \
		  ../../Legacy/libSprintLegacy.$(a) \
		  ../../Bliss/libSprintBliss.$(a) \
		  ../../Audio/libSprintAudio.$(a) \
		  ../../Core/libSprintCore.$(a) \
		  ../../Fsa/libSprintFsa.$(a) \
                  ../../Teaching/libSprintTeaching.$(a)

ifndef MODULE_SEARCH_SIGNLANGUAGE
FLF_TOOL_O += ../../Signal/libSprintSignal.$(a)
FLF_TOOL_O += ../../Flow/libSprintFlow.$(a)
FLF_TOOL_O += ../../Math/libSprintMath.$(a)
FLF_TOOL_O += ../../Math/Lapack/libSprintMathLapack.$(a)
endif

ifdef MODULE_CART
FLF_TOOL_O += ../../Cart/libSprintCart.$(a)
endif

# -----------------------------------------------------------------------------

build: $(TARGETS)

install:
	$(INSTALL) $(TARGETS) $(INSTALL_TARGET)

flf-tool$(exe): $(FLF_TOOL_O)
	$(LD) $(LD_START_GROUP) $^ $(LD_END_GROUP) -o $@ $(LDFLAGS)

include $(TOPDIR)/Rules.make

sinclude $(patsubst %.o,%.d,$(filter %.o,$(FLF_TOOL_O)))
