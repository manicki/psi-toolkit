# test prostego zestawu regul binarnych i unarnych

frz0 = rzeczownik*

frz1 = frz0*

frz1 = frz0* frz
	frz.P == dop

frz2 = frz1*

frz2 = fprz frz2*
	P :== fprz.P,
	R :== fprz.R,
	L :== fprz.L,
	setscore 1

frz = frz2*


fprz0 = przymiotnik*

fprz1 = fprz0*

fprz1 = fps fprz0*
	fps.Typ = przym

fprz = fprz1*

fps = przyslowek*

