SRCROOT:=	../..
.include "$(SRCROOT)/pttbbs.mk"

SRCS:=	test.fbs

.for SRC in $(SRCS)
$(SRC).h: $(SRC)
	flatc -c $(SRC) --filename-suffix .fbs

HDRS+=	$(SRC).h
.endfor

install:

all: $(HDRS)

clean:
	@rm -f $(HDRS)
