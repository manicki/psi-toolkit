# maly zestaw

ap = ap-adjective*
	{ Block := false },
	setscore 0

ap-adjective = adjective* %AP[adjective]%
	Block := 1,
	setscore 0

lk = lk-vp*
	setscore 0

lk-vp = vp* %LK[vp]%
	Block := 1,
	setscore 0

mf = mf-np--np*
	setscore 0

mf-np = np* %MF[np:compl1]%
	Block := 1,
	C == dat,
	OD := true,
	OD_C := C,
	setscore 0

mf-np--np = mf-np* np %mf-np[+np:subject]%
	ON := true,
	ON_Num := np.Num,
	ON_P := np.P,
	np.C == nom,
	setscore 0

np = np-noun*
	{ Block := false },
	setscore -1.38629436111989

np = np-noun--ap*
	{ Block := false },
	setscore -1.38629436111989

np = np-noun--article*
	{ Block := false },
	setscore -1.38629436111989

np = np-np--pp*
	{ Block := false },
	setscore -1.38629436111989

np-noun = noun* %NP[noun]%
	Block := 1,
	setscore 0

np-noun--ap = ap np-noun* %np-noun[ap+]%
	C :== ap.C,
	Num :== ap.Num,
	R :== ap.R,
	ap.O == attr,
	setscore -0.405465108108164

np-noun--ap = ap np-noun--ap* %np-noun--ap[ap+]%
	C :== ap.C,
	Num :== ap.Num,
	R :== ap.R,
	ap.O == attr,
	setscore -1.09861228866811

np-noun--article = article np-noun--ap* %np-noun--ap[article+]%
	C :== article.C,
	Num :== article.Num,
	R :== article.R,
	setscore 0

np-np = np* %NP[np]%
	Block := 1,
	setscore 0

np-np--pp = np-np* pp %np-np[+pp:compl1]%
	setscore 0

pp = pp-np--prep*
	{ Block := false },
	setscore 0

pp-np = np* %PP[np]%
	Block := 1,
	setscore 0

pp-np--prep = prep pp-np* %pp-np[prep+]%
	C :== prep.Dop1,
	Przyim := prep.Core,
	setscore 0

simpx = simpx-lk--vc*
	C_ON = false,
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore 0

simpx-lk = lk* %SIMPX[lk]%
	Block := 1,
	setscore 0

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OD_C := mf.OD_C,
	P == mf.ON_P,
	mf.OPP = false,
	setscore 0

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
	Dop3 == vc.Typ,
	vc.Dop1 == OD_C,
	vc.Przyim1 == zero,
	setscore 0

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	vf.ON = false,
	vf.OPP = false,
	setscore 0

vc = vc-vp*
	setscore 0

vc-vp = vp* %VC[vp:compl3]%
	Block := 1,
	Typ == part,
	setscore 0

vf = vf-pp*
	setscore 0

vf-pp = pp* %VF[pp:v-mod]%
	Block := 1,
	setscore 0

vp = vp-verb*
	{ Block := false },
	setscore 0

vp-verb = verb* %VP[verb]%
	Block := 1,
	setscore 0

