# test specjalnego symbolu __PREHOOK__

__PREHOOK__ = __PREHOOK__ __PREHOOK__
	Len := __PREHOOK__[0].Len + __PREHOOK__[1].Len,
	(__PREHOOK__[1].A ==> (C := 2)),
	{ Beg := __PREHOOK__[0].Beg },
	{ End := __PREHOOK__[1].End }

__PREHOOK__ = __PREHOOK__
	Len := __PREHOOK__.Len,
	{ A := __PREHOOK__.A },
	{ Beg := __PREHOOK__.Beg },
	{ End := __PREHOOK__.End }


cx = c

x = b* cx
	C = 2,
	Len = 2,
	Beg,
	End,
	D = 11

y = b v* d
	Len = 3,
	End = false,
	Beg,
	Z = 10






