# Don't bother with implicit checks
.SUFFIXES:

.NOTPARALLEL:

include $(SMING_HOME)/util.mk

all: $(AVAILABLE_SOCS)

.PHONY: $(AVAILABLE_SOCS)
$(AVAILABLE_SOCS):
	$(Q) $(MAKE) --no-print-directory test SMING_SOC=$@ STRICT=1

.PHONY: test
test:
	$(info )
	$(info )
	$(info ===================== )
	$(info Building $@ for $(SMING_SOC))
	$(info ===================== )
	$(info )
	$(Q) $(MAKE) -C ../../test -r -R --no-print-directory \
		$(if $(filter host,$(SMING_SOC)),execute)

clean:
	$(Q) rm -rf test/out
