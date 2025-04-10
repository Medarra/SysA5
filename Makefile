
.PHONY: all clean

all:
	$(MAKE) -C Common
	$(MAKE) -C "DC"
	$(MAKE) -C "DP-1"
	$(MAKE) -C "DP-2"

clean:
	$(MAKE) -C Common clean
	$(MAKE) -C "DC" clean
	$(MAKE) -C "DP-1" clean
	$(MAKE) -C "DP-2" clean