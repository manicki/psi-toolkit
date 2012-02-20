# najwiekszy test

advx = advx-adverb*
	{ Block := false },
	setscore -3.76120011569356

advx = advx-adverb--advx*
	{ Block := false },
	setscore -2.55722731136763

advx = advx-adverb--ap*
	{ Block := false },
	setscore -4.85981240436167

advx = advx-adverb--conj*
	{ Block := false },
	setscore -3.47351804324178

advx = advx-adverb--np*
	{ Block := false },
	setscore -2.91390225530636

advx = advx-adverb--part*
	{ Block := false },
	setscore -3.76120011569356

advx = advx-adverb--pp*
	{ Block := false },
	setscore -2.78037086268184

advx = advx-adverb--simpx*
	{ Block := false },
	setscore -4.85981240436167

advx = advx-advx--advx*
	{ Block := false },
	setscore -2.4619171315633

advx = advx-advx--ap*
	{ Block := false },
	setscore -3.76120011569356

advx = advx-advx--conj*
	{ Block := false },
	setscore -3.76120011569356

advx = advx-advx--fx*
	{ Block := false },
	setscore -4.85981240436167

advx = advx-advx--np*
	{ Block := false },
	setscore -2.22075507474641

advx = advx-advx--pp*
	{ Block := false },
	setscore -1.86408013080768

advx = advx-advx--r-simpx*
	{ Block := false },
	setscore -4.16666522380173

advx = advx-advx--simpx*
	{ Block := false },
	setscore -2.37490575457367

advx = advx-ap--advx*
	{ Block := false },
	setscore -4.85981240436167

advx = advx-conj--adverb*
	{ Block := false },
	setscore -4.85981240436167

advx = advx-conj--advx*
	{ Block := false },
	setscore -4.16666522380173

advx = advx-exclamation*
	{ Block := false },
	setscore -4.85981240436167

advx = advx-exclamation--advx*
	{ Block := false },
	setscore -4.85981240436167

advx = advx-np--simpx*
	{ Block := false },
	setscore -4.85981240436167

advx = advx-num*
	{ Block := false },
	setscore -4.85981240436167

advx = advx-part*
	{ Block := false },
	setscore -4.85981240436167

advx = advx-part--advx*
	{ Block := false },
	setscore -4.16666522380173

advx = advx-part--dm*
	{ Block := false },
	setscore -4.85981240436167

advx = advx-part--np*
	{ Block := false },
	setscore -4.85981240436167

advx = advx-pp--pp*
	{ Block := false },
	setscore -4.85981240436167

advx = advx-simpx--advx*
	{ Block := false },
	setscore -4.85981240436167

advx = advx-trunc--adverb*
	{ Block := false },
	setscore -4.16666522380173

advx-adverb = adverb* %ADVX[adverb]%
	Block := 1,
	T == deadj,
	setscore -0.470003629245736

advx-adverb = adverb* %ADVX[adverb]%
	Block := 1,
	T == other,
	setscore -1.04982212449868

advx-adverb = adverb* %ADVX[adverb]%
	Block := 1,
	T == rel,
	setscore -3.68887945411394

advx-adverb--advx = advx advx-adverb* %advx-adverb[advx+]%
	setscore -0.893817876022096

advx-adverb--advx = advx advx-adverb--advx* %advx-adverb--advx[advx+]%
	setscore -1.70474809223843

advx-adverb--advx = advx advx-adverb--ap* %advx-adverb--ap[advx+]%
	setscore -3.09104245335832

advx-adverb--advx = advx advx-adverb--np* %advx-adverb--np[advx+]%
	setscore -1.70474809223843

advx-adverb--advx = advx advx-adverb--part* %advx-adverb--part[advx+]%
	setscore -3.09104245335832

advx-adverb--advx = advx advx-adverb--pp* %advx-adverb--pp[advx+]%
	setscore -2.39789527279837

advx-adverb--ap = ap advx-adverb* %advx-adverb[ap+]%
	setscore 0

advx-adverb--conj = conj advx-adverb* %advx-adverb[conj+]%
	setscore -0.916290731874155

advx-adverb--conj = conj advx-adverb--advx* %advx-adverb--advx[conj+]%
	setscore -1.6094379124341

advx-adverb--conj = conj advx-adverb--pp* %advx-adverb--pp[conj+]%
	setscore -1.6094379124341

advx-adverb--np = np advx-adverb* %advx-adverb[np+]%
	setscore -0.559615787935423

advx-adverb--np = np advx-adverb--advx* %advx-adverb--advx[np+]%
	setscore -1.94591014905531

advx-adverb--np = np advx-adverb--np* %advx-adverb--np[np+]%
	setscore -1.94591014905531

advx-adverb--np = np advx-adverb--part* %advx-adverb--part[np+]%
	setscore -2.63905732961526

advx-adverb--np = np advx-adverb--pp* %advx-adverb--pp[np+]%
	setscore -2.63905732961526

advx-adverb--part = part advx-adverb* %advx-adverb[part+]%
	setscore -0.22314355131421

advx-adverb--part = part advx-adverb--advx* %advx-adverb--advx[part+]%
	setscore -1.6094379124341

advx-adverb--pp = pp advx-adverb* %advx-adverb[pp:compl1+]%
	setscore -0.693147180559945

advx-adverb--pp = pp advx-adverb--advx* %advx-adverb--advx[pp:compl1+]%
	setscore -1.67397643357167

advx-adverb--pp = pp advx-adverb--np* %advx-adverb--np[pp:compl1+]%
	setscore -2.77258872223978

advx-adverb--pp = pp advx-adverb--pp* %advx-adverb--pp[pp:compl1+]%
	setscore -1.67397643357167

advx-adverb--simpx = simpx advx-adverb* %advx-adverb[simpx+]%
	setscore 0

advx-advx = advx* %ADVX[advx:app]%
	Block := 1,
	setscore -3.16406758837321

advx-advx = advx* %ADVX[advx:konj]%
	Block := 1,
	setscore -1.171637423683

advx-advx = advx* %ADVX[advx]%
	Block := 1,
	setscore -0.43403848055222

advx-advx--advx = advx advx-advx* %advx-advx[advx+]%
	setscore -0.737598943130779

advx-advx--advx = advx advx-advx--comma* %advx-advx--comma[advx+]%
	setscore -3.13549421592915

advx-advx--advx = advx advx-advx--np* %advx-advx--np[advx+]%
	setscore -3.13549421592915

advx-advx--advx = advx advx-advx--pp* %advx-advx--pp[advx+]%
	setscore -3.13549421592915

advx-advx--advx = advx-advx* advx %advx-advx[+advx:konj]%
	setscore -2.4423470353692

advx-advx--advx = advx-advx* advx %advx-advx[+advx]%
	setscore -3.13549421592915

advx-advx--advx = advx-advx--comma* advx %advx-advx--comma[+advx:konj]%
	setscore -2.4423470353692

advx-advx--advx = advx-advx--comma* advx %advx-advx--comma[+advx]%
	setscore -3.13549421592915

advx-advx--advx = advx-advx--conj* advx %advx-advx--conj[+advx:konj]%
	setscore -2.4423470353692

advx-advx--advx = advx-advx--pp* advx %advx-advx--pp[+advx]%
	setscore -3.13549421592915

advx-advx--ap = advx-advx--comma* ap %advx-advx--comma[+ap:konj]%
	setscore -1.09861228866811

advx-advx--ap = advx-advx--conj* ap %advx-advx--conj[+ap:konj]%
	setscore -0.405465108108164

advx-advx--comma = advx-advx* comma %advx-advx[+comma]%
	setscore -0.322773392263051

advx-advx--comma = advx-advx--advx* comma %advx-advx--advx[+comma]%
	setscore -2.67414864942653

advx-advx--comma = advx-advx--conj* comma %advx-advx--conj[+comma]%
	setscore -2.67414864942653

advx-advx--comma = advx-advx--np* comma %advx-advx--np[+comma]%
	setscore -3.36729582998647

advx-advx--comma = advx-advx--pp* comma %advx-advx--pp[+comma]%
	setscore -2.67414864942653

advx-advx--comma = comma advx-advx* %advx-advx[comma+]%
	setscore -3.36729582998647

advx-advx--conj = advx-advx* conj %advx-advx[+conj]%
	setscore -1.38629436111989

advx-advx--conj = advx-advx--advx* conj %advx-advx--advx[+conj]%
	setscore -2.99573227355399

advx-advx--conj = advx-advx--comma* conj %advx-advx--comma[+conj]%
	setscore -1.6094379124341

advx-advx--conj = advx-advx--pp* conj %advx-advx--pp[+conj]%
	setscore -2.99573227355399

advx-advx--conj = advx-advx--simpx* conj %advx-advx--simpx[+conj]%
	setscore -2.99573227355399

advx-advx--conj = conj advx-advx* %advx-advx[conj+]%
	setscore -1.6094379124341

advx-advx--conj = conj advx-advx--advx* %advx-advx--advx[conj+]%
	setscore -1.89711998488588

advx-advx--conj = conj advx-advx--np* %advx-advx--np[conj+]%
	setscore -2.99573227355399

advx-advx--fx = fx advx-advx* %advx-advx[fx+]%
	setscore 0

advx-advx--np = advx-advx* np %advx-advx[+np:possessive]%
	setscore -3.04452243772342

advx-advx--np = advx-advx* np %advx-advx[+np]%
	setscore -3.04452243772342

advx-advx--np = advx-advx--advx* np %advx-advx--advx[+np]%
	setscore -3.04452243772342

advx-advx--np = advx-advx--comma* np %advx-advx--comma[+np:app]%
	setscore -3.04452243772342

advx-advx--np = advx-advx--comma* np %advx-advx--comma[+np:konj]%
	setscore -1.65822807660353

advx-advx--np = advx-advx--comma* np %advx-advx--comma[+np]%
	setscore -3.04452243772342

advx-advx--np = advx-advx--conj* np %advx-advx--conj[+np:konj]%
	setscore -3.04452243772342

advx-advx--np = advx-advx--conj* np %advx-advx--conj[+np]%
	setscore -3.04452243772342

advx-advx--np = advx-advx--pp* np %advx-advx--pp[+np]%
	setscore -2.35137525716348

advx-advx--np = np advx-advx* %advx-advx[np+]%
	setscore -1.65822807660353

advx-advx--pp = advx-advx* pp %advx-advx[+pp:compl1]%
	setscore -2.33537491581704

advx-advx--pp = advx-advx* pp %advx-advx[+pp:konj]%
	setscore -3.43398720448515

advx-advx--pp = advx-advx--advx* pp %advx-advx--advx[+pp:compl1]%
	setscore -3.43398720448515

advx-advx--pp = advx-advx--comma* pp %advx-advx--comma[+pp:app]%
	setscore -3.43398720448515

advx-advx--pp = advx-advx--comma* pp %advx-advx--comma[+pp:compl1]%
	setscore -2.7408400239252

advx-advx--pp = advx-advx--comma* pp %advx-advx--comma[+pp:konj]%
	setscore -2.33537491581704

advx-advx--pp = advx-advx--conj* pp %advx-advx--conj[+pp:compl1]%
	setscore -2.7408400239252

advx-advx--pp = advx-advx--conj* pp %advx-advx--conj[+pp:konj]%
	setscore -2.04769284336526

advx-advx--pp = advx-advx--np* pp %advx-advx--np[+pp:compl1]%
	setscore -3.43398720448515

advx-advx--pp = advx-advx--pp* pp %advx-advx--pp[+pp:compl1]%
	setscore -2.7408400239252

advx-advx--pp = advx-advx--simpx* pp %advx-advx--simpx[+pp:compl1]%
	setscore -3.43398720448515

advx-advx--pp = pp advx-advx* %advx-advx[pp:compl1+]%
	setscore -1.82454929205105

advx-advx--pp = pp advx-advx--advx* %advx-advx--advx[pp:compl1+]%
	setscore -2.33537491581704

advx-advx--pp = pp advx-advx--pp* %advx-advx--pp[pp:compl1+]%
	setscore -3.43398720448515

advx-advx--r-simpx = advx-advx--comma* r-simpx %advx-advx--comma[+r-simpx]%
	setscore 0

advx-advx--simpx = advx-advx* simpx %advx-advx[+simpx:konj]%
	setscore -2.70805020110221

advx-advx--simpx = advx-advx* simpx %advx-advx[+simpx]%
	setscore -2.01490302054226

advx-advx--simpx = advx-advx--advx* simpx %advx-advx--advx[+simpx]%
	setscore -2.70805020110221

advx-advx--simpx = advx-advx--comma* simpx %advx-advx--comma[+simpx:konj]%
	setscore -2.01490302054226

advx-advx--simpx = advx-advx--comma* simpx %advx-advx--comma[+simpx]%
	setscore -1.32175583998232

advx-advx--simpx = advx-advx--conj* simpx %advx-advx--conj[+simpx:konj]%
	setscore -1.6094379124341

advx-advx--simpx = simpx advx-advx* %advx-advx[simpx+]%
	setscore -2.70805020110221

advx-ap = ap* %ADVX[ap:konj]%
	Block := 1,
	setscore -0.693147180559945

advx-ap = ap* %ADVX[ap]%
	Block := 1,
	setscore -0.693147180559945

advx-ap--advx = advx-ap--conj* advx %advx-ap--conj[+advx:konj]%
	setscore -0.693147180559945

advx-ap--conj = advx-ap* conj %advx-ap[+conj]%
	setscore 0

advx-conj = conj* %ADVX[conj]%
	Block := 1,
	setscore 0

advx-conj--adverb = advx-conj--advx* adverb %advx-conj--advx[+adverb:konj]%
	setscore 0

advx-conj--advx = advx-conj* advx %advx-conj[+advx:konj]%
	setscore -0.693147180559945

advx-conj--advx = advx-conj--conj* advx %advx-conj--conj[+advx:konj]%
	setscore -1.38629436111989

advx-conj--advx = advx-conj--conj* advx %advx-conj--conj[+advx]%
	setscore -1.38629436111989

advx-conj--conj = advx-conj--advx* conj %advx-conj--advx[+conj]%
	setscore -0.693147180559945

advx-conj--conj = advx-conj--trunc* conj %advx-conj--trunc[+conj]%
	setscore -0.693147180559945

advx-conj--trunc = advx-conj* trunc %advx-conj[+trunc:konj]%
	setscore 0

advx-exclamation = exclamation* %ADVX[exclamation]%
	Block := 1,
	setscore 0

advx-exclamation--advx = advx advx-exclamation* %advx-exclamation[advx+]%
	setscore 0

advx-np = np* %ADVX[np]%
	Block := 1,
	setscore 0

advx-np--comma = advx-np* comma %advx-np[+comma]%
	setscore 0

advx-np--simpx = advx-np--comma* simpx %advx-np--comma[+simpx]%
	setscore 0

advx-num = num* %ADVX[num]%
	Block := 1,
	setscore 0

advx-part = part* %ADVX[part]%
	Block := 1,
	setscore -1.6094379124341

advx-part = part* %ADVX[part]%
	Block := 1,
	T == neg,
	setscore -0.22314355131421

advx-part--advx = advx advx-part* %advx-part[advx+]%
	setscore -0.693147180559945

advx-part--advx = advx advx-part--advx* %advx-part--advx[advx+]%
	setscore -1.79175946922806

advx-part--advx = advx-part* advx %advx-part[+advx]%
	setscore -1.79175946922806

advx-part--advx = advx-part--conj* advx %advx-part--conj[+advx]%
	setscore -1.79175946922806

advx-part--ap = advx-part--advx* ap %advx-part--advx[+ap:konj]%
	setscore 0

advx-part--comma = advx-part--ap* comma %advx-part--ap[+comma]%
	setscore -0.693147180559945

advx-part--comma = comma advx-part--advx* %advx-part--advx[comma+]%
	setscore -0.693147180559945

advx-part--conj = advx-part--comma* conj %advx-part--comma[+conj]%
	setscore 0

advx-part--dm = dm advx-part--comma* %advx-part--comma[dm+]%
	setscore 0

advx-part--np = advx-part--advx* np %advx-part--advx[+np:konj]%
	setscore 0

advx-pp = pp* %ADVX[pp:konj]%
	Block := 1,
	setscore 0

advx-pp--conj = advx-pp* conj %advx-pp[+conj]%
	setscore 0

advx-pp--pp = advx-pp--conj* pp %advx-pp--conj[+pp:konj]%
	setscore 0

advx-simpx = simpx* %ADVX[simpx:konj]%
	Block := 1,
	setscore 0

advx-simpx--advx = advx-simpx--comma* advx %advx-simpx--comma[+advx:konj]%
	setscore 0

advx-simpx--comma = advx-simpx* comma %advx-simpx[+comma]%
	setscore 0

advx-trunc = trunc* %ADVX[trunc:konj]%
	Block := 1,
	setscore 0

advx-trunc--adverb = advx-trunc--comma* adverb %advx-trunc--comma[+adverb:konj]%
	setscore -0.693147180559945

advx-trunc--adverb = advx-trunc--conj* adverb %advx-trunc--conj[+adverb:konj]%
	setscore -0.693147180559945

advx-trunc--comma = advx-trunc--trunc* comma %advx-trunc--trunc[+comma]%
	setscore 0

advx-trunc--conj = advx-trunc* conj %advx-trunc[+conj]%
	setscore 0

advx-trunc--trunc = advx-trunc--conj* trunc %advx-trunc--conj[+trunc:konj]%
	setscore 0

ap = ap-adjective*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-adjective--advx*
	{ Block := false },
	setscore -2.43944427571124

ap = ap-adjective--ap*
	{ Block := false },
	setscore -4.45434729625351

ap = ap-adjective--conj*
	{ Block := false },
	setscore -3.3557350075854

ap = ap-adjective--exclamation*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-adjective--np*
	{ Block := false },
	setscore -2.84490938381941

ap = ap-adjective--part*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-adjective--pp*
	{ Block := false },
	setscore -2.66258782702545

ap = ap-adjective--prn*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-adjective--pron*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-adverb*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-advx*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-advx--adjective*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-advx--advx*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-advx--ap*
	{ Block := false },
	setscore -3.53805656437935

ap = ap-advx--pp*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-ap--advx*
	{ Block := false },
	setscore -3.06805293513362

ap = ap-ap--ap*
	{ Block := false },
	setscore -2.50843714719819

ap = ap-ap--conj*
	{ Block := false },
	setscore -4.45434729625351

ap = ap-ap--fx*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-ap--np*
	{ Block := false },
	setscore -2.66258782702545

ap = ap-ap--pp*
	{ Block := false },
	setscore -2.66258782702545

ap = ap-ap--r-simpx*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-ap--simpx*
	{ Block := false },
	setscore -3.76120011569356

ap = ap-article*
	{ Block := false },
	setscore -4.45434729625351

ap = ap-conj--ap*
	{ Block := false },
	setscore -4.45434729625351

ap = ap-conj--np*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-conj--pp*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-np--advx*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-np--pp*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-num*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-num--advx*
	{ Block := false },
	setscore -4.45434729625351

ap = ap-num--ap*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-num--num*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-num--pp*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-num--prep*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-part--adjective*
	{ Block := false },
	setscore -3.53805656437935

ap = ap-part--advx*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-pp*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-pp--advx*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-pp--ap*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-prn*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-pron*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-pron--ap*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-pron--pp*
	{ Block := false },
	setscore -4.45434729625351

ap = ap-trunc--adjective*
	{ Block := false },
	setscore -4.45434729625351

ap = ap-trunc--ap*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-trunc--num*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-unknown*
	{ Block := false },
	setscore -5.14749447681345

ap = ap-unknown--advx*
	{ Block := false },
	setscore -5.14749447681345

ap-adjective = adjective* %AP[adjective]%
	Block := 1,
	setscore 0

ap-adjective--advx = advx ap-adjective* %ap-adjective[advx+]%
	setscore -1.09861228866811

ap-adjective--advx = advx ap-adjective--advx* %ap-adjective--advx[advx+]%
	setscore -1.65822807660353

ap-adjective--advx = advx ap-adjective--ap* %ap-adjective--ap[advx+]%
	setscore -3.04452243772342

ap-adjective--advx = advx ap-adjective--conj* %ap-adjective--conj[advx+]%
	setscore -3.04452243772342

ap-adjective--advx = advx ap-adjective--np* %ap-adjective--np[advx+]%
	setscore -2.35137525716348

ap-adjective--advx = advx ap-adjective--part* %ap-adjective--part[advx+]%
	setscore -3.73766961828337

ap-adjective--advx = advx ap-adjective--pp* %ap-adjective--pp[advx+]%
	setscore -1.94591014905531

ap-adjective--advx = advx ap-adjective--vp* %ap-adjective--vp[advx+]%
	setscore -3.73766961828337

ap-adjective--ap = ap ap-adjective* %ap-adjective[ap+]%
	setscore -0.693147180559945

ap-adjective--ap = ap ap-adjective--advx* %ap-adjective--advx[ap+]%
	setscore -1.38629436111989

ap-adjective--ap = ap ap-adjective--pp* %ap-adjective--pp[ap+]%
	setscore -1.38629436111989

ap-adjective--comma = comma ap-adjective* %ap-adjective[comma+]%
	setscore -0.405465108108164

ap-adjective--comma = comma ap-adjective--advx* %ap-adjective--advx[comma+]%
	setscore -1.09861228866811

ap-adjective--conj = conj ap-adjective* %ap-adjective[conj+]%
	setscore -1.29928298413026

ap-adjective--conj = conj ap-adjective--advx* %ap-adjective--advx[conj+]%
	setscore -1.70474809223843

ap-adjective--conj = conj ap-adjective--comma* %ap-adjective--comma[conj+]%
	setscore -2.39789527279837

ap-adjective--conj = conj ap-adjective--np* %ap-adjective--np[conj+]%
	setscore -2.39789527279837

ap-adjective--conj = conj ap-adjective--pp* %ap-adjective--pp[conj+]%
	setscore -2.39789527279837

ap-adjective--exclamation = exclamation ap-adjective* %ap-adjective[exclamation+]%
	setscore 0

ap-adjective--np = np ap-adjective* %ap-adjective[np+]%
	setscore -0.93826963859293

ap-adjective--np = np ap-adjective* %ap-adjective[np:possessive+]%
	setscore -3.13549421592915

ap-adjective--np = np ap-adjective--advx* %ap-adjective--advx[np+]%
	setscore -2.03688192726104

ap-adjective--np = np ap-adjective--np* %ap-adjective--np[np+]%
	setscore -3.13549421592915

ap-adjective--np = np ap-adjective--np* %ap-adjective--np[np:possessive+]%
	setscore -3.13549421592915

ap-adjective--np = np ap-adjective--pp* %ap-adjective--pp[np+]%
	setscore -1.52605630349505

ap-adjective--part = part ap-adjective* %ap-adjective[part+]%
	setscore 0

ap-adjective--pp = pp ap-adjective* %ap-adjective[pp:compl1+]%
	setscore -0.980829253011726

ap-adjective--pp = pp ap-adjective--advx* %ap-adjective--advx[pp:compl1+]%
	setscore -2.07944154167984

ap-adjective--pp = pp ap-adjective--comma* %ap-adjective--comma[pp:compl1+]%
	setscore -2.77258872223978

ap-adjective--pp = pp ap-adjective--np* %ap-adjective--np[pp:compl1+]%
	setscore -2.36712361413162

ap-adjective--pp = pp ap-adjective--part* %ap-adjective--part[pp:compl1+]%
	setscore -3.46573590279973

ap-adjective--pp = pp ap-adjective--pp* %ap-adjective--pp[pp:compl1+]%
	setscore -1.85629799036563

ap-adjective--prn = prn ap-adjective* %ap-adjective[prn+]%
	setscore 0

ap-adjective--pron = pron ap-adjective* %ap-adjective[pron+]%
	setscore 0

ap-adjective--vp = vp ap-adjective* %ap-adjective[vp+]%
	setscore 0

ap-adverb = adverb* %AP[adverb]%
	Block := 1,
	setscore 0

ap-advx = advx* %AP[advx:konj]%
	Block := 1,
	setscore -1.50407739677627

ap-advx = advx* %AP[advx]%
	Block := 1,
	setscore -0.251314428280906

ap-advx--adjective = ap-advx--conj* adjective %ap-advx--conj[+adjective:konj]%
	setscore 0

ap-advx--advx = ap-advx* advx %ap-advx[+advx]%
	setscore -0.693147180559945

ap-advx--advx = ap-advx--conj* advx %ap-advx--conj[+advx]%
	setscore -0.693147180559945

ap-advx--ap = ap-advx* ap %ap-advx[+ap:konj]%
	O := ap.O,
	setscore -1.09861228866811

ap-advx--ap = ap-advx--advx* ap %ap-advx--advx[+ap:konj]%
	O := ap.O,
	setscore -1.50407739677627

ap-advx--ap = ap-advx--advx* ap %ap-advx--advx[+ap:konj]%
	O == ap.O,
	setscore -2.19722457733622

ap-advx--ap = ap-advx--conj* ap %ap-advx--conj[+ap:konj]%
	O == ap.O,
	setscore -1.50407739677627

ap-advx--ap = ap-advx--conj* ap %ap-advx--conj[+ap:konj]%
	setscore -2.19722457733622

ap-advx--comma = ap-advx--ap* comma %ap-advx--ap[+comma]%
	setscore 0

ap-advx--conj = ap-advx* conj %ap-advx[+conj]%
	setscore -1.25276296849537

ap-advx--conj = ap-advx--comma* conj %ap-advx--comma[+conj]%
	setscore -0.559615787935423

ap-advx--conj = ap-advx--trunc* conj %ap-advx--trunc[+conj]%
	setscore -1.94591014905531

ap-advx--pp = ap-advx--conj* pp %ap-advx--conj[+pp:konj]%
	setscore 0

ap-advx--trunc = ap-advx* trunc %ap-advx[+trunc:konj]%
	setscore 0

ap-ap = ap* %AP[ap:konj]%
	Block := 1,
	setscore -0.662375521893192

ap-ap = ap* %AP[ap]%
	Block := 1,
	setscore -0.724895878874526

ap-ap--advx = advx ap-ap* %ap-ap[advx+]%
	setscore -1.09861228866811

ap-ap--advx = advx ap-ap--advx* %ap-ap--advx[advx+]%
	setscore -2.70805020110221

ap-ap--advx = ap-ap* advx %ap-ap[+advx]%
	setscore -2.01490302054226

ap-ap--advx = ap-ap--advx* advx %ap-ap--advx[+advx]%
	setscore -2.70805020110221

ap-ap--advx = ap-ap--comma* advx %ap-ap--comma[+advx]%
	setscore -2.70805020110221

ap-ap--advx = ap-ap--conj* advx %ap-ap--conj[+advx:konj]%
	setscore -1.6094379124341

ap-ap--advx = ap-ap--conj* advx %ap-ap--conj[+advx]%
	setscore -2.70805020110221

ap-ap--ap = ap ap-ap* %ap-ap[ap+]%
	setscore -3.09104245335832

ap-ap--ap = ap-ap* ap %ap-ap[+ap:konj]%
	O == ap.O,
	setscore -2.39789527279837

ap-ap--ap = ap-ap--comma* ap %ap-ap--comma[+ap:konj]%
	O == ap.O,
	setscore -1.48160454092422

ap-ap--ap = ap-ap--comma* ap %ap-ap--comma[+ap:konj]%
	setscore -3.09104245335832

ap-ap--ap = ap-ap--conj* ap %ap-ap--conj[+ap:konj]%
	O == ap.O,
	setscore -1.145132304303

ap-ap--ap = ap-ap--conj* ap %ap-ap--conj[+ap:konj]%
	setscore -1.99243016469021

ap-ap--comma = ap-ap* comma %ap-ap[+comma]%
	setscore -0.200670695462151

ap-ap--comma = ap-ap--advx* comma %ap-ap--advx[+comma]%
	setscore -3.09104245335832

ap-ap--comma = ap-ap--conj* comma %ap-ap--conj[+comma]%
	setscore -2.39789527279837

ap-ap--comma = ap-ap--pp* comma %ap-ap--pp[+comma]%
	setscore -3.09104245335832

ap-ap--conj = ap-ap* conj %ap-ap[+conj]%
	setscore -0.869037847023609

ap-ap--conj = ap-ap--advx* conj %ap-ap--advx[+conj]%
	setscore -3.43398720448515

ap-ap--conj = ap-ap--ap* conj %ap-ap--ap[+conj]%
	setscore -2.33537491581704

ap-ap--conj = ap-ap--comma* conj %ap-ap--comma[+conj]%
	setscore -2.04769284336526

ap-ap--conj = ap-ap--pp* conj %ap-ap--pp[+conj]%
	setscore -3.43398720448515

ap-ap--conj = conj ap-ap* %ap-ap[conj+]%
	setscore -2.33537491581704

ap-ap--conj = conj ap-ap--advx* %ap-ap--advx[conj+]%
	setscore -3.43398720448515

ap-ap--dm = ap-ap--comma* dm %ap-ap--comma[+dm]%
	setscore 0

ap-ap--fx = ap-ap* fx %ap-ap[+fx]%
	setscore 0

ap-ap--np = ap-ap* np %ap-ap[+np:konj]%
	setscore -2.63905732961526

ap-ap--np = ap-ap* np %ap-ap[+np:possessive]%
	setscore -2.63905732961526

ap-ap--np = ap-ap* np %ap-ap[+np]%
	setscore -2.63905732961526

ap-ap--np = ap-ap--comma* np %ap-ap--comma[+np:konj]%
	setscore -1.94591014905531

ap-ap--np = ap-ap--comma* np %ap-ap--comma[+np]%
	setscore -2.63905732961526

ap-ap--np = ap-ap--conj* np %ap-ap--conj[+np:konj]%
	setscore -1.94591014905531

ap-ap--np = ap-ap--conj* np %ap-ap--conj[+np]%
	setscore -1.54044504094715

ap-ap--np = ap-ap--pp* np %ap-ap--pp[+np]%
	setscore -2.63905732961526

ap-ap--np = np ap-ap* %ap-ap[np+]%
	setscore -2.63905732961526

ap-ap--pp = ap-ap* pp %ap-ap[+pp:compl1]%
	setscore -2.70805020110221

ap-ap--pp = ap-ap--advx* pp %ap-ap--advx[+pp:compl1]%
	setscore -2.70805020110221

ap-ap--pp = ap-ap--comma* pp %ap-ap--comma[+pp:compl1]%
	setscore -2.70805020110221

ap-ap--pp = ap-ap--comma* pp %ap-ap--comma[+pp:konj]%
	setscore -1.6094379124341

ap-ap--pp = ap-ap--conj* pp %ap-ap--conj[+pp:compl1]%
	setscore -2.70805020110221

ap-ap--pp = ap-ap--conj* pp %ap-ap--conj[+pp:konj]%
	setscore -1.09861228866811

ap-ap--pp = pp ap-ap* %ap-ap[pp:compl1+]%
	setscore -2.01490302054226

ap-ap--r-simpx = ap-ap* r-simpx %ap-ap[+r-simpx]%
	setscore 0

ap-ap--simpx = ap-ap* simpx %ap-ap[+simpx]%
	setscore -1.38629436111989

ap-ap--simpx = ap-ap--comma* simpx %ap-ap--comma[+simpx:konj]%
	setscore -1.38629436111989

ap-ap--simpx = ap-ap--comma* simpx %ap-ap--comma[+simpx]%
	setscore -0.693147180559945

ap-article = article* %AP[article]%
	Block := 1,
	setscore 0

ap-conj = conj* %AP[conj]%
	Block := 1,
	setscore 0

ap-conj--advx = ap-conj--conj* advx %ap-conj--conj[+advx]%
	setscore 0

ap-conj--ap = ap-conj* ap %ap-conj[+ap:konj]%
	O := ap.O,
	setscore -0.22314355131421

ap-conj--ap = ap-conj--conj* ap %ap-conj--conj[+ap:konj]%
	O == ap.O,
	setscore -1.6094379124341

ap-conj--conj = ap-conj--ap* conj %ap-conj--ap[+conj]%
	setscore 0

ap-conj--np = ap-conj--conj* np %ap-conj--conj[+np:konj]%
	setscore 0

ap-conj--pp = ap-conj--advx* pp %ap-conj--advx[+pp:konj]%
	setscore 0

ap-np = np* %AP[np]%
	Block := 1,
	setscore 0

ap-np--advx = ap-np* advx %ap-np[+advx]%
	setscore 0

ap-np--pp = ap-np* pp %ap-np[+pp:compl1]%
	setscore 0

ap-num = num* %AP[num]%
	Block := 1,
	setscore 0

ap-num--advx = advx ap-num* %ap-num[advx+]%
	setscore -0.693147180559945

ap-num--advx = advx ap-num--conj* %ap-num--conj[advx+]%
	setscore -0.693147180559945

ap-num--ap = ap ap-num* %ap-num[ap+]%
	setscore 0

ap-num--conj = conj ap-num* %ap-num[conj+]%
	setscore 0

ap-num--num = ap-num* num %ap-num[+num]%
	setscore 0

ap-num--pp = pp ap-num* %ap-num[pp:compl1+]%
	setscore 0

ap-num--prep = prep ap-num* %ap-num[prep+]%
	setscore 0

ap-part = part* %AP[part]%
	Block := 1,
	setscore 0

ap-part--adjective = ap-part* adjective %ap-part[+adjective]%
	setscore -1.6094379124341

ap-part--adjective = ap-part--advx* adjective %ap-part--advx[+adjective]%
	setscore -0.916290731874155

ap-part--adjective = ap-part--pp* adjective %ap-part--pp[+adjective]%
	setscore -0.916290731874155

ap-part--advx = advx ap-part* %ap-part[advx+]%
	setscore -0.510825623765991

ap-part--advx = advx ap-part--advx* %ap-part--advx[advx+]%
	setscore -1.6094379124341

ap-part--advx = ap-part* advx %ap-part[+advx]%
	setscore -1.6094379124341

ap-part--pp = pp ap-part* %ap-part[pp:compl1+]%
	setscore -1.09861228866811

ap-part--pp = pp ap-part--advx* %ap-part--advx[pp:compl1+]%
	setscore -1.09861228866811

ap-part--pp = pp ap-part--pp* %ap-part--pp[pp:compl1+]%
	setscore -1.09861228866811

ap-pp = pp* %AP[pp:compl1]%
	Block := 1,
	setscore -1.09861228866811

ap-pp = pp* %AP[pp:konj]%
	Block := 1,
	setscore -1.09861228866811

ap-pp = pp* %AP[pp]%
	Block := 1,
	setscore -1.09861228866811

ap-pp--advx = ap-pp* advx %ap-pp[+advx]%
	setscore 0

ap-pp--ap = ap-pp--conj* ap %ap-pp--conj[+ap:konj]%
	setscore 0

ap-pp--comma = ap-pp* comma %ap-pp[+comma]%
	setscore 0

ap-pp--conj = ap-pp--comma* conj %ap-pp--comma[+conj]%
	setscore 0

ap-prn = prn* %AP[prn]%
	Block := 1,
	setscore 0

ap-pron = pron* %AP[pron]%
	Block := 1,
	setscore 0

ap-pron--advx = advx ap-pron* %ap-pron[advx+]%
	setscore 0

ap-pron--ap = ap ap-pron* %ap-pron[ap+]%
	setscore 0

ap-pron--pp = pp ap-pron* %ap-pron[pp:compl1+]%
	setscore -0.693147180559945

ap-pron--pp = pp ap-pron--advx* %ap-pron--advx[pp:compl1+]%
	setscore -0.693147180559945

ap-trunc = trunc* %AP[trunc:konj]%
	Block := 1,
	setscore 0

ap-trunc--adjective = ap-trunc* adjective %ap-trunc[+adjective:konj]%
	setscore -0.693147180559945

ap-trunc--adjective = ap-trunc--conj* adjective %ap-trunc--conj[+adjective:konj]%
	setscore -0.693147180559945

ap-trunc--ap = ap-trunc--comma* ap %ap-trunc--comma[+ap:konj]%
	setscore 0

ap-trunc--comma = ap-trunc* comma %ap-trunc[+comma]%
	setscore 0

ap-trunc--conj = ap-trunc* conj %ap-trunc[+conj]%
	setscore 0

ap-trunc--num = ap-trunc--conj* num %ap-trunc--conj[+num:konj]%
	setscore 0

ap-unknown = unknown* %AP[unknown]%
	Block := 1,
	setscore 0

ap-unknown--advx = advx ap-unknown* %ap-unknown[advx+]%
	setscore 0

c = c-advx*
	setscore -1.57553636075842

c = c-ap*
	setscore -2.26868354131836

c = c-conj*
	setscore -2.67414864942653

c = c-conj--conj*
	setscore -3.36729582998647

c = c-np*
	setscore -1.57553636075842

c = c-pp*
	setscore -1.06471073699243

c = c-simpx*
	setscore -3.36729582998647

c-advx = advx* %C[advx:mod]%
	Block := 1,
	setscore -1.79175946922806

c-advx = advx* %C[advx:oadjp]%
	Block := 1,
	setscore -1.79175946922806

c-advx = advx* %C[advx:oadvp]%
	Block := 1,
	setscore -1.79175946922806

c-advx = advx* %C[advx:pred-mod]%
	Block := 1,
	setscore -1.79175946922806

c-advx = advx* %C[advx:pred]%
	Block := 1,
	PRED := true,
	setscore -1.79175946922806

c-advx = advx* %C[advx:v-mod]%
	Block := 1,
	setscore -1.79175946922806

c-ap = ap* %C[ap:pred-mod]%
	Block := 1,
	setscore -1.09861228866811

c-ap = ap* %C[ap:pred]%
	Block := 1,
	PRED := true,
	setscore -1.09861228866811

c-ap = ap* %C[ap:v-mod]%
	Block := 1,
	setscore -1.09861228866811

c-conj = conj* %C[conj]%
	Block := 1,
	CONJ := 1,
	setscore 0

c-conj--conj = c-conj* conj %c-conj[+conj]%
	setscore 0

c-np = np* %C[np:compl1]%
	Block := 1,
	setscore -1.79175946922806

c-np = np* %C[np:compl2]%
	Block := 1,
	setscore -1.79175946922806

c-np = np* %C[np:oa]%
	Block := 1,
	C == acc,
	OA := true,
	OA_C := C,
	setscore -1.79175946922806

c-np = np* %C[np:on-mod]%
	Block := 1,
	setscore -1.79175946922806

c-np = np* %C[np:pred]%
	Block := 1,
	PRED := true,
	setscore -1.79175946922806

c-np = np* %C[np:subject]%
	Block := 1,
	setscore -1.79175946922806

c-pp = pp* %C[pp:compl1]%
	Block := 1,
	setscore -2.30258509299405

c-pp = pp* %C[pp:compl2]%
	Block := 1,
	setscore -2.30258509299405

c-pp = pp* %C[pp:fopp]%
	Block := 1,
	setscore -2.30258509299405

c-pp = pp* %C[pp:mod]%
	Block := 1,
	setscore -2.30258509299405

c-pp = pp* %C[pp:oa-mod]%
	Block := 1,
	setscore -2.30258509299405

c-pp = pp* %C[pp:on-mod]%
	Block := 1,
	setscore -2.30258509299405

c-pp = pp* %C[pp:opp-mod]%
	Block := 1,
	setscore -2.30258509299405

c-pp = pp* %C[pp:pred-mod]%
	Block := 1,
	setscore -2.30258509299405

c-pp = pp* %C[pp:pred]%
	Block := 1,
	PRED := true,
	setscore -2.30258509299405

c-pp = pp* %C[pp:v-mod]%
	Block := 1,
	setscore -2.30258509299405

c-simpx = simpx* %C[simpx:subject]%
	Block := 1,
	setscore 0

dm = dm-advx*
	setscore -3.17805383034795

dm = dm-advx--advx*
	setscore -3.17805383034795

dm = dm-advx--exclamation*
	setscore -2.484906649788

dm = dm-conj*
	setscore -3.17805383034795

dm = dm-conj--advx*
	setscore -3.17805383034795

dm = dm-conj--exclamation*
	setscore -2.484906649788

dm = dm-conj--np*
	setscore -3.17805383034795

dm = dm-exclamation*
	setscore -3.17805383034795

dm = dm-exclamation--advx*
	setscore -3.17805383034795

dm = dm-exclamation--conj*
	setscore -3.17805383034795

dm = dm-exclamation--exclamation*
	setscore -3.17805383034795

dm = dm-exclamation--np*
	setscore -3.17805383034795

dm = dm-exclamation--pp*
	setscore -3.17805383034795

dm = dm-exclamation--unknown*
	setscore -2.484906649788

dm = dm-np*
	setscore -3.17805383034795

dm = dm-np--exclamation*
	setscore -3.17805383034795

dm = dm-np--np*
	setscore -3.17805383034795

dm = dm-pp*
	setscore -3.17805383034795

dm = dm-unknown*
	setscore -3.17805383034795

dm = dm-unknown--unknown*
	setscore -3.17805383034795

dm = dm-vp*
	setscore -3.17805383034795

dm-advx = advx* %DM[advx]%
	Block := 1,
	setscore 0

dm-advx--advx = dm-advx* advx %dm-advx[+advx]%
	setscore 0

dm-advx--exclamation = dm-advx* exclamation %dm-advx[+exclamation]%
	setscore -0.693147180559945

dm-advx--exclamation = dm-advx--advx* exclamation %dm-advx--advx[+exclamation]%
	setscore -0.693147180559945

dm-conj = conj* %DM[conj]%
	Block := 1,
	setscore 0

dm-conj--advx = dm-conj* advx %dm-conj[+advx]%
	setscore 0

dm-conj--exclamation = dm-conj* exclamation %dm-conj[+exclamation]%
	setscore -0.693147180559945

dm-conj--exclamation = dm-conj--pp* exclamation %dm-conj--pp[+exclamation]%
	setscore -0.693147180559945

dm-conj--np = dm-conj--advx* np %dm-conj--advx[+np]%
	setscore 0

dm-conj--pp = dm-conj* pp %dm-conj[+pp:compl1]%
	setscore 0

dm-exclamation = exclamation* %DM[exclamation]%
	Block := 1,
	setscore 0

dm-exclamation--advx = dm-exclamation* advx %dm-exclamation[+advx]%
	setscore 0

dm-exclamation--comma = dm-exclamation* comma %dm-exclamation[+comma]%
	setscore -0.405465108108164

dm-exclamation--comma = dm-exclamation--unknown* comma %dm-exclamation--unknown[+comma]%
	setscore -1.09861228866811

dm-exclamation--conj = dm-exclamation* conj %dm-exclamation[+conj]%
	setscore 0

dm-exclamation--exclamation = dm-exclamation* exclamation %dm-exclamation[+exclamation]%
	setscore 0

dm-exclamation--np = dm-exclamation* np %dm-exclamation[+np]%
	setscore 0

dm-exclamation--pp = dm-exclamation* pp %dm-exclamation[+pp:compl1]%
	setscore 0

dm-exclamation--unknown = dm-exclamation--comma* unknown %dm-exclamation--comma[+unknown]%
	setscore 0

dm-np = np* %DM[np]%
	Block := 1,
	setscore 0

dm-np--comma = dm-np* comma %dm-np[+comma]%
	setscore 0

dm-np--exclamation = dm-np* exclamation %dm-np[+exclamation]%
	setscore 0

dm-np--np = dm-np--comma* np %dm-np--comma[+np]%
	setscore 0

dm-pp = pp* %DM[pp:compl1]%
	Block := 1,
	setscore 0

dm-unknown = unknown* %DM[unknown]%
	Block := 1,
	setscore 0

dm-unknown--unknown = dm-unknown* unknown %dm-unknown[+unknown]%
	setscore 0

dm-vp = vp* %DM[vp]%
	Block := 1,
	setscore 0

dp = dp-dp--dp*
	{ Block := false },
	setscore -1.79175946922806

dp = dp-pron*
	{ Block := false },
	setscore -1.79175946922806

dp = dp-pron--advx*
	{ Block := false },
	setscore -1.09861228866811

dp = dp-pron--np*
	{ Block := false },
	setscore -1.79175946922806

dp = dp-pron--part*
	{ Block := false },
	setscore -1.79175946922806

dp-dp = dp* %DP[dp:konj]%
	Block := 1,
	setscore 0

dp-dp--conj = dp-dp* conj %dp-dp[+conj]%
	setscore 0

dp-dp--dp = dp-dp--conj* dp %dp-dp--conj[+dp:konj]%
	setscore 0

dp-pron = pron* %DP[pron]%
	Block := 1,
	setscore 0

dp-pron--advx = advx dp-pron* %dp-pron[advx+]%
	setscore -0.693147180559945

dp-pron--advx = advx dp-pron--part* %dp-pron--part[advx+]%
	setscore -0.693147180559945

dp-pron--np = np dp-pron* %dp-pron[np+]%
	setscore 0

dp-pron--part = part dp-pron* %dp-pron[part+]%
	setscore 0

fkonj = fkonj-c--mf*
	setscore -5.28826703069454

fkonj = fkonj-c--vc*
	C_ON = false,
	MF_OA = true,
	MF_ON = true,
	setscore -5.28826703069454

fkonj = fkonj-c--vc*
	C_ON = false,
	MF_ON = true,
	setscore -5.28826703069454

fkonj = fkonj-c--vc*
	MF_OA = true,
	setscore -5.28826703069454

fkonj = fkonj-lk*
	setscore -5.28826703069454

fkonj = fkonj-lk--fkoord*
	MF_OA = true,
	setscore -5.28826703069454

fkonj = fkonj-lk--fkoord*
	setscore -5.28826703069454

fkonj = fkonj-lk--mf*
	MF_OA = true,
	setscore -3.67882911826043

fkonj = fkonj-lk--mf*
	MF_OA = true,
	MF_OD = true,
	setscore -4.59511985013459

fkonj = fkonj-lk--mf*
	MF_OA = true,
	MF_OD = true,
	MF_ON = true,
	setscore -4.59511985013459

fkonj = fkonj-lk--mf*
	MF_OA = true,
	MF_OG = true,
	setscore -5.28826703069454

fkonj = fkonj-lk--mf*
	MF_OA = true,
	MF_ON = true,
	setscore -4.18965474202643

fkonj = fkonj-lk--mf*
	MF_OA = true,
	MF_ON = true,
	VF_ON = false,
	setscore -5.28826703069454

fkonj = fkonj-lk--mf*
	MF_OD = true,
	setscore -4.18965474202643

fkonj = fkonj-lk--mf*
	MF_OD = true,
	MF_ON = true,
	setscore -5.28826703069454

fkonj = fkonj-lk--mf*
	MF_OG = true,
	setscore -5.28826703069454

fkonj = fkonj-lk--mf*
	MF_ON = false,
	VF_ON = true,
	setscore -5.28826703069454

fkonj = fkonj-lk--mf*
	MF_ON = true,
	setscore -4.18965474202643

fkonj = fkonj-lk--mf*
	MF_ON = true,
	VF_ON = false,
	setscore -5.28826703069454

fkonj = fkonj-lk--mf*
	setscore -3.67882911826043

fkonj = fkonj-lk--nf*
	MF_OA = true,
	setscore -2.80336038090653

fkonj = fkonj-lk--nf*
	MF_OA = true,
	MF_OD = true,
	setscore -4.59511985013459

fkonj = fkonj-lk--nf*
	MF_OA = true,
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	setscore -5.28826703069454

fkonj = fkonj-lk--nf*
	MF_OA = true,
	MF_ON = true,
	NF_ON = false,
	setscore -4.59511985013459

fkonj = fkonj-lk--nf*
	MF_OD = true,
	setscore -3.90197266957464

fkonj = fkonj-lk--nf*
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	setscore -5.28826703069454

fkonj = fkonj-lk--nf*
	MF_ON = true,
	NF_ON = false,
	setscore -4.59511985013459

fkonj = fkonj-lk--nf*
	setscore -2.24374459297111

fkonj = fkonj-lk--vc*
	MF_OA = true,
	setscore -3.90197266957464

fkonj = fkonj-lk--vc*
	MF_OA = true,
	MF_OD = true,
	setscore -5.28826703069454

fkonj = fkonj-lk--vc*
	MF_OA = true,
	MF_OD = true,
	MF_ON = true,
	setscore -5.28826703069454

fkonj = fkonj-lk--vc*
	MF_OA = true,
	MF_OG = true,
	setscore -5.28826703069454

fkonj = fkonj-lk--vc*
	MF_OA = true,
	MF_OG = true,
	MF_ON = true,
	setscore -5.28826703069454

fkonj = fkonj-lk--vc*
	MF_OA = true,
	MF_ON = false,
	VF_ON = true,
	setscore -5.28826703069454

fkonj = fkonj-lk--vc*
	MF_OA = true,
	MF_ON = true,
	setscore -4.18965474202643

fkonj = fkonj-lk--vc*
	MF_OD = true,
	setscore -5.28826703069454

fkonj = fkonj-lk--vc*
	MF_OD = true,
	MF_ON = true,
	setscore -5.28826703069454

fkonj = fkonj-lk--vc*
	MF_ON = true,
	setscore -4.18965474202643

fkonj = fkonj-lk--vc*
	MF_ON = true,
	VF_OA = true,
	VF_ON = false,
	setscore -5.28826703069454

fkonj = fkonj-lk--vc*
	setscore -3.09104245335832

fkonj = fkonj-mf*
	setscore -5.28826703069454

fkonj = fkonj-mf--nf*
	MF_OA = true,
	setscore -3.67882911826043

fkonj = fkonj-mf--nf*
	MF_OA = true,
	MF_OD = true,
	setscore -4.59511985013459

fkonj = fkonj-mf--nf*
	MF_OA = true,
	MF_OG = true,
	setscore -4.59511985013459

fkonj = fkonj-mf--nf*
	MF_OA = true,
	MF_ON = true,
	NF_ON = false,
	setscore -4.59511985013459

fkonj = fkonj-mf--nf*
	MF_OD = true,
	setscore -4.18965474202643

fkonj = fkonj-mf--nf*
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	setscore -5.28826703069454

fkonj = fkonj-mf--nf*
	MF_ON = true,
	NF_ON = false,
	setscore -3.34235688163922

fkonj = fkonj-mf--nf*
	setscore -2.98568193770049

fkonj = fkonj-mf--simpx*
	MF_OA = true,
	setscore -5.28826703069454

fkonj = fkonj-mf--vc*
	MF_OA = true,
	setscore -3.67882911826043

fkonj = fkonj-mf--vc*
	MF_OA = true,
	MF_OD = true,
	setscore -4.18965474202643

fkonj = fkonj-mf--vc*
	MF_OA = true,
	MF_OD = true,
	MF_ON = true,
	setscore -4.59511985013459

fkonj = fkonj-mf--vc*
	MF_OA = true,
	MF_OG = true,
	setscore -5.28826703069454

fkonj = fkonj-mf--vc*
	MF_OA = true,
	MF_ON = true,
	setscore -3.67882911826043

fkonj = fkonj-mf--vc*
	MF_OD = true,
	setscore -4.18965474202643

fkonj = fkonj-mf--vc*
	MF_OD = true,
	MF_ON = true,
	setscore -4.18965474202643

fkonj = fkonj-mf--vc*
	MF_OG = true,
	setscore -5.28826703069454

fkonj = fkonj-mf--vc*
	MF_OG = true,
	MF_ON = true,
	setscore -5.28826703069454

fkonj = fkonj-mf--vc*
	MF_ON = true,
	setscore -3.49650756146648

fkonj = fkonj-mf--vc*
	setscore -3.49650756146648

fkonj = fkonj-vc*
	setscore -5.28826703069454

fkonj = fkonj-vc--mf*
	MF_OA = true,
	setscore -5.28826703069454

fkonj = fkonj-vc--nf*
	setscore -4.59511985013459

fkonj = fkonj-vf--mf*
	setscore -5.28826703069454

fkonj = fkonj-vf--vc*
	VF_ON = true,
	setscore -5.28826703069454

fkonj-c = c* %FKONJ[c]%
	Block := 1,
	setscore -0.693147180559945

fkonj-c = c* %FKONJ[c]%
	Block := 1,
	ON = false,
	setscore -0.693147180559945

fkonj-c--mf = fkonj-c* mf %fkonj-c[+mf]%
	MF_OA := mf.OA,
	setscore -1.38629436111989

fkonj-c--mf = fkonj-c* mf %fkonj-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	setscore -1.38629436111989

fkonj-c--mf = fkonj-c* mf %fkonj-c[+mf]%
	MF_ON := mf.ON,
	setscore -1.38629436111989

fkonj-c--mf = fkonj-c* mf %fkonj-c[+mf]%
	setscore -1.38629436111989

fkonj-c--vc = fkonj-c--mf* vc %fkonj-c--mf[+vc]%
	setscore 0

fkonj-lk = lk* %FKONJ[lk]%
	Block := 1,
	setscore 0

fkonj-lk--comma = fkonj-lk* comma %fkonj-lk[+comma]%
	setscore -1.67397643357167

fkonj-lk--comma = fkonj-lk--mf* comma %fkonj-lk--mf[+comma]%
	setscore -0.980829253011726

fkonj-lk--comma = fkonj-lk--vc* comma %fkonj-lk--vc[+comma]%
	setscore -0.826678573184468

fkonj-lk--conj = fkonj-lk* conj %fkonj-lk[+conj]%
	setscore 0

fkonj-lk--fkoord = fkonj-lk* fkoord %fkonj-lk[+fkoord]%
	setscore -0.693147180559945

fkonj-lk--fkoord = fkonj-lk--mf* fkoord %fkonj-lk--mf[+fkoord]%
	setscore -0.693147180559945

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
	MF_OA := mf.OA,
	setscore -2.484906649788

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	setscore -3.17805383034795

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	setscore -3.46573590279973

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	mf.OPP = true,
	setscore -4.56434819146784

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	mf.OPP = true,
	setscore -4.56434819146784

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
	MF_OA := mf.OA,
	MF_OG := mf.OG,
	setscore -3.87120101090789

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
	MF_OA := mf.OA,
	MF_OG := mf.OG,
	MF_ON := mf.ON,
	setscore -4.56434819146784

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	setscore -3.17805383034795

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	mf.OPP = true,
	setscore -3.87120101090789

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	mf.PRED = true,
	setscore -3.87120101090789

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
	MF_OA := mf.OA,
	mf.OPP = true,
	setscore -3.17805383034795

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
	MF_OA := mf.OA,
	mf.PRED = true,
	setscore -2.95491027903374

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
	MF_OD := mf.OD,
	setscore -2.95491027903374

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	setscore -3.46573590279973

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
	MF_OD := mf.OD,
	mf.OPP = true,
	setscore -4.56434819146784

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
	MF_OD := mf.OD,
	mf.PRED = true,
	setscore -3.87120101090789

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
	MF_OG := mf.OG,
	mf.PRED = true,
	setscore -4.56434819146784

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
	MF_ON := mf.ON,
	setscore -3.17805383034795

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
	MF_ON := mf.ON,
	mf.OPP = true,
	setscore -3.46573590279973

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
	MF_ON := mf.ON,
	mf.PRED = true,
	setscore -3.87120101090789

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
	mf.OPP = true,
	setscore -2.77258872223978

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
	mf.OPP = true,
	mf.PRED = true,
	setscore -4.56434819146784

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
	mf.PRED = true,
	setscore -2.77258872223978

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
	setscore -2.26176309847379

fkonj-lk--mf = fkonj-lk--comma* mf %fkonj-lk--comma[+mf]%
	MF_OA := mf.OA,
	setscore -3.87120101090789

fkonj-lk--mf = fkonj-lk--comma* mf %fkonj-lk--comma[+mf]%
	MF_OA := mf.OA,
	mf.OPP = true,
	setscore -4.56434819146784

fkonj-lk--mf = fkonj-lk--comma* mf %fkonj-lk--comma[+mf]%
	setscore -3.87120101090789

fkonj-lk--mf = fkonj-lk--conj* mf %fkonj-lk--conj[+mf]%
	setscore -4.56434819146784

fkonj-lk--mf = fkonj-lk--vf* mf %fkonj-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	setscore -4.56434819146784

fkonj-lk--mf = fkonj-lk--vf* mf %fkonj-lk--vf[+mf]%
	MF_OA := mf.OA,
	mf.ON = false,
	setscore -4.56434819146784

fkonj-lk--mf = fkonj-lk--vf* mf %fkonj-lk--vf[+mf]%
	MF_ON := mf.ON,
	setscore -3.87120101090789

fkonj-lk--mf = fkonj-lk--vf* mf %fkonj-lk--vf[+mf]%
	mf.ON = false,
	setscore -4.56434819146784

fkonj-lk--nf = fkonj-lk* nf %fkonj-lk[+nf]%
	setscore -3.8286413964891

fkonj-lk--nf = fkonj-lk--comma* nf %fkonj-lk--comma[+nf]%
	nf.ON = false,
	setscore -2.4423470353692

fkonj-lk--nf = fkonj-lk--comma* nf %fkonj-lk--comma[+nf]%
	setscore -0.784118958765672

fkonj-lk--nf = fkonj-lk--mf* nf %fkonj-lk--mf[+nf]%
	nf.ON = false,
	setscore -3.13549421592915

fkonj-lk--nf = fkonj-lk--mf* nf %fkonj-lk--mf[+nf]%
	nf.OPP = true,
	setscore -3.8286413964891

fkonj-lk--nf = fkonj-lk--mf* nf %fkonj-lk--mf[+nf]%
	setscore -2.21920348405499

fkonj-lk--nf = fkonj-lk--vc* nf %fkonj-lk--vc[+nf]%
	nf.OPP = true,
	setscore -3.13549421592915

fkonj-lk--nf = fkonj-lk--vc* nf %fkonj-lk--vc[+nf]%
	nf.PRED = true,
	setscore -3.13549421592915

fkonj-lk--nf = fkonj-lk--vc* nf %fkonj-lk--vc[+nf]%
	setscore -2.21920348405499

fkonj-lk--pp = fkonj-lk* pp %fkonj-lk[+pp:fopp]%
	setscore 0

fkonj-lk--vc = fkonj-lk* vc %fkonj-lk[+vc]%
	setscore -2.32238772029023

fkonj-lk--vc = fkonj-lk--comma* vc %fkonj-lk--comma[+vc]%
	setscore -3.23867845216438

fkonj-lk--vc = fkonj-lk--mf* vc %fkonj-lk--mf[+vc]%
	setscore -0.242946178610389

fkonj-lk--vc = fkonj-lk--pp* vc %fkonj-lk--pp[+vc]%
	setscore -3.93182563272433

fkonj-lk--vf = vf fkonj-lk* %fkonj-lk[vf+]%
	VF_OA := vf.OA,
	vf.ON = false,
	setscore -1.6094379124341

fkonj-lk--vf = vf fkonj-lk* %fkonj-lk[vf+]%
	VF_ON := vf.ON,
	setscore -0.916290731874155

fkonj-lk--vf = vf fkonj-lk* %fkonj-lk[vf+]%
	vf.ON = false,
	setscore -0.916290731874155

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	setscore -1.87180217690159

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	MF_OA := OA,
	setscore -2.15948424935337

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	MF_OA := OA,
	MF_OD := OD,
	setscore -3.25809653802148

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	MF_OA := OA,
	MF_OD := OD,
	MF_ON := ON,
	setscore -4.35670882668959

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	MF_OA := OA,
	MF_OD := OD,
	MF_ON := ON,
	PRED = true,
	setscore -4.35670882668959

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	MF_OA := OA,
	MF_OD := OD,
	OPP = true,
	setscore -4.35670882668959

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	MF_OA := OA,
	MF_OD := OD,
	PRED = true,
	setscore -4.35670882668959

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	MF_OA := OA,
	MF_OG := OG,
	setscore -3.25809653802148

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	MF_OA := OA,
	MF_ON := ON,
	setscore -2.56494935746154

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	MF_OA := OA,
	MF_ON := ON,
	OPP = true,
	setscore -4.35670882668959

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	MF_OA := OA,
	MF_ON := ON,
	OPP = true,
	PRED = true,
	setscore -4.35670882668959

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	MF_OA := OA,
	MF_ON := ON,
	PRED = true,
	setscore -4.35670882668959

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	MF_OA := OA,
	OPP = true,
	setscore -3.66356164612965

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	MF_OA := OA,
	OPP = true,
	PRED = true,
	setscore -4.35670882668959

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	MF_OA := OA,
	PRED = true,
	setscore -3.66356164612965

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	MF_OD := OD,
	setscore -3.66356164612965

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	MF_OD := OD,
	MF_ON := ON,
	setscore -3.25809653802148

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	MF_OD := OD,
	MF_ON := ON,
	PRED = true,
	setscore -4.35670882668959

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	MF_OD := OD,
	OPP = true,
	setscore -3.66356164612965

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	MF_OD := OD,
	PRED = true,
	setscore -3.66356164612965

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	MF_OG := OG,
	setscore -4.35670882668959

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	MF_OG := OG,
	MF_ON := ON,
	setscore -4.35670882668959

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	MF_ON := ON,
	setscore -2.56494935746154

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	MF_ON := ON,
	OPP = true,
	setscore -3.25809653802148

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	MF_ON := ON,
	OPP = true,
	PRED = true,
	setscore -4.35670882668959

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	MF_ON := ON,
	PRED = true,
	setscore -2.9704144655697

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	OPP = true,
	setscore -3.25809653802148

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	OPP = true,
	PRED = true,
	setscore -4.35670882668959

fkonj-mf = mf* %FKONJ[mf]%
	Block := 1,
	PRED = true,
	setscore -3.25809653802148

fkonj-mf--comma = fkonj-mf* comma %fkonj-mf[+comma]%
	setscore -1.42711635564015

fkonj-mf--comma = fkonj-mf--vc* comma %fkonj-mf--vc[+comma]%
	setscore -0.27443684570176

fkonj-mf--nf = fkonj-mf* nf %fkonj-mf[+nf]%
	setscore -3.61091791264422

fkonj-mf--nf = fkonj-mf--comma* nf %fkonj-mf--comma[+nf]%
	nf.ON = false,
	setscore -1.81915844341617

fkonj-mf--nf = fkonj-mf--comma* nf %fkonj-mf--comma[+nf]%
	nf.OPP = true,
	setscore -3.61091791264422

fkonj-mf--nf = fkonj-mf--comma* nf %fkonj-mf--comma[+nf]%
	setscore -1.12601126285622

fkonj-mf--nf = fkonj-mf--vc* nf %fkonj-mf--vc[+nf]%
	nf.ON = false,
	setscore -2.22462355152433

fkonj-mf--nf = fkonj-mf--vc* nf %fkonj-mf--vc[+nf]%
	nf.OPP = true,
	setscore -3.61091791264422

fkonj-mf--nf = fkonj-mf--vc* nf %fkonj-mf--vc[+nf]%
	nf.PRED = true,
	setscore -3.61091791264422

fkonj-mf--nf = fkonj-mf--vc* nf %fkonj-mf--vc[+nf]%
	setscore -2.00148000021012

fkonj-mf--pp = fkonj-mf* pp %fkonj-mf[+pp:v-mod]%
	setscore 0

fkonj-mf--simpx = fkonj-mf--comma* simpx %fkonj-mf--comma[+simpx]%
	setscore 0

fkonj-mf--vc = fkonj-mf* vc %fkonj-mf[+vc]%
	setscore -0.174353387144778

fkonj-mf--vc = fkonj-mf--comma* vc %fkonj-mf--comma[+vc]%
	setscore -2.70805020110221

fkonj-mf--vc = fkonj-mf--pp* vc %fkonj-mf--pp[+vc]%
	setscore -4.31748811353631

fkonj-mf--vc = fkonj-mf--vce* vc %fkonj-mf--vce[+vc]%
	setscore -4.31748811353631

fkonj-mf--vce = fkonj-mf* vce %fkonj-mf[+vce]%
	setscore 0

fkonj-vc = vc* %FKONJ[vc]%
	Block := 1,
	setscore 0

fkonj-vc--comma = fkonj-vc* comma %fkonj-vc[+comma]%
	setscore 0

fkonj-vc--mf = fkonj-vc* mf %fkonj-vc[+mf]%
	MF_OA := mf.OA,
	setscore 0

fkonj-vc--nf = fkonj-vc* nf %fkonj-vc[+nf]%
	setscore -0.693147180559945

fkonj-vc--nf = fkonj-vc--comma* nf %fkonj-vc--comma[+nf]%
	setscore -0.693147180559945

fkonj-vf = vf* %FKONJ[vf]%
	Block := 1,
	setscore -0.693147180559945

fkonj-vf = vf* %FKONJ[vf]%
	Block := 1,
	VF_ON := ON,
	setscore -0.693147180559945

fkonj-vf--mf = fkonj-vf* mf %fkonj-vf[+mf]%
	setscore 0

fkonj-vf--vc = fkonj-vf* vc %fkonj-vf[+vc]%
	setscore 0

fkoord = fkoord-advx--fkonj*
	setscore -4.18965474202643

fkoord = fkoord-c--c*
	setscore -4.18965474202643

fkoord = fkoord-conj--fkonj*
	setscore -2.80336038090653

fkoord = fkoord-conj--mf*
	setscore -4.18965474202643

fkoord = fkoord-conj--vc*
	setscore -4.18965474202643

fkoord = fkoord-fkonj*
	setscore -4.18965474202643

fkoord = fkoord-fkonj--conj*
	setscore -4.18965474202643

fkoord = fkoord-fkonj--fkonj*
	setscore -1.99243016469021

fkoord = fkoord-fkonj--fkoord*
	setscore -3.09104245335832

fkoord = fkoord-fkonj--mf*
	setscore -2.58021682959233

fkoord = fkoord-fkonj--vc*
	setscore -2.80336038090653

fkoord = fkoord-fkoord--fkonj*
	setscore -3.09104245335832

fkoord = fkoord-fkoord--nf*
	setscore -4.18965474202643

fkoord = fkoord-lk--fkonj*
	setscore -2.24374459297111

fkoord = fkoord-lk--fkoord*
	setscore -3.49650756146648

fkoord = fkoord-lk--lk*
	setscore -3.49650756146648

fkoord = fkoord-mf--fkonj*
	setscore -2.39789527279837

fkoord = fkoord-mf--mf*
	setscore -2.80336038090653

fkoord = fkoord-vc--fkonj*
	setscore -2.80336038090653

fkoord = fkoord-vc--vc*
	setscore -4.18965474202643

fkoord-advx = advx* %FKOORD[advx]%
	Block := 1,
	setscore 0

fkoord-advx--advx = fkoord-advx* advx %fkoord-advx[+advx]%
	setscore 0

fkoord-advx--comma = fkoord-advx--fkonj* comma %fkoord-advx--fkonj[+comma]%
	setscore 0

fkoord-advx--conj = fkoord-advx--comma* conj %fkoord-advx--comma[+conj]%
	setscore 0

fkoord-advx--fkonj = fkoord-advx--advx* fkonj %fkoord-advx--advx[+fkonj:konj]%
	setscore -0.693147180559945

fkoord-advx--fkonj = fkoord-advx--conj* fkonj %fkoord-advx--conj[+fkonj:konj]%
	setscore -0.693147180559945

fkoord-c = c* %FKOORD[c:konj]%
	Block := 1,
	setscore 0

fkoord-c--c = fkoord-c--conj* c %fkoord-c--conj[+c:konj]%
	setscore 0

fkoord-c--conj = fkoord-c* conj %fkoord-c[+conj]%
	setscore 0

fkoord-conj = conj* %FKOORD[conj]%
	Block := 1,
	setscore 0

fkoord-conj--advx = fkoord-conj--conj* advx %fkoord-conj--conj[+advx]%
	setscore 0

fkoord-conj--comma = fkoord-conj--fkonj* comma %fkoord-conj--fkonj[+comma]%
	setscore -0.693147180559945

fkoord-conj--comma = fkoord-conj--vc* comma %fkoord-conj--vc[+comma]%
	setscore -0.693147180559945

fkoord-conj--conj = fkoord-conj--comma* conj %fkoord-conj--comma[+conj]%
	setscore -1.09861228866811

fkoord-conj--conj = fkoord-conj--fkonj* conj %fkoord-conj--fkonj[+conj]%
	setscore -1.09861228866811

fkoord-conj--conj = fkoord-conj--vc* conj %fkoord-conj--vc[+conj]%
	setscore -1.09861228866811

fkoord-conj--fkonj = fkoord-conj* fkonj %fkoord-conj[+fkonj:konj]%
	setscore -0.847297860387204

fkoord-conj--fkonj = fkoord-conj--advx* fkonj %fkoord-conj--advx[+fkonj:konj]%
	setscore -1.94591014905531

fkoord-conj--fkonj = fkoord-conj--conj* fkonj %fkoord-conj--conj[+fkonj:konj]%
	setscore -0.847297860387204

fkoord-conj--mf = fkoord-conj--conj* mf %fkoord-conj--conj[+mf:konj]%
	setscore 0

fkoord-conj--vc = fkoord-conj* vc %fkoord-conj[+vc:konj]%
	setscore -0.287682072451781

fkoord-conj--vc = fkoord-conj--conj* vc %fkoord-conj--conj[+vc:konj]%
	setscore -1.38629436111989

fkoord-fkonj = fkonj* %FKOORD[fkonj:konj]%
	Block := 1,
	setscore -0.0408219945202552

fkoord-fkonj = fkonj* %FKOORD[fkonj]%
	Block := 1,
	setscore -3.2188758248682

fkoord-fkonj--advx = fkoord-fkonj--advx* advx %fkoord-fkonj--advx[+advx]%
	setscore -0.693147180559945

fkoord-fkonj--advx = fkoord-fkonj--comma* advx %fkoord-fkonj--comma[+advx]%
	setscore -0.693147180559945

fkoord-fkonj--comma = fkoord-fkonj* comma %fkoord-fkonj[+comma]%
	setscore -0.0741079721537218

fkoord-fkonj--comma = fkoord-fkonj--vc* comma %fkoord-fkonj--vc[+comma]%
	setscore -2.63905732961526

fkoord-fkonj--conj = fkoord-fkonj* conj %fkoord-fkonj[+conj]%
	setscore -1.20397280432594

fkoord-fkonj--conj = fkoord-fkonj--comma* conj %fkoord-fkonj--comma[+conj]%
	setscore -1.38629436111989

fkoord-fkonj--conj = fkoord-fkonj--fkonj* conj %fkoord-fkonj--fkonj[+conj]%
	setscore -1.89711998488588

fkoord-fkonj--conj = fkoord-fkonj--mf* conj %fkoord-fkonj--mf[+conj]%
	setscore -2.99573227355399

fkoord-fkonj--conj = fkoord-fkonj--vc* conj %fkoord-fkonj--vc[+conj]%
	setscore -1.89711998488588

fkoord-fkonj--fkonj = fkoord-fkonj* fkonj %fkoord-fkonj[+fkonj:konj]%
	setscore -1.94591014905531

fkoord-fkonj--fkonj = fkoord-fkonj--advx* fkonj %fkoord-fkonj--advx[+fkonj:konj]%
	setscore -2.63905732961526

fkoord-fkonj--fkonj = fkoord-fkonj--comma* fkonj %fkoord-fkonj--comma[+fkonj:konj]%
	setscore -1.94591014905531

fkoord-fkonj--fkonj = fkoord-fkonj--conj* fkonj %fkoord-fkonj--conj[+fkonj:konj]%
	setscore -0.559615787935423

fkoord-fkonj--fkoord = fkoord-fkonj--comma* fkoord %fkoord-fkonj--comma[+fkoord:konj]%
	setscore -1.09861228866811

fkoord-fkonj--fkoord = fkoord-fkonj--conj* fkoord %fkoord-fkonj--conj[+fkoord:konj]%
	setscore -0.405465108108164

fkoord-fkonj--mf = fkoord-fkonj* mf %fkoord-fkonj[+mf:konj]%
	setscore -1.94591014905531

fkoord-fkonj--mf = fkoord-fkonj--comma* mf %fkoord-fkonj--comma[+mf:konj]%
	setscore -1.25276296849537

fkoord-fkonj--mf = fkoord-fkonj--conj* mf %fkoord-fkonj--conj[+mf:konj]%
	setscore -0.847297860387204

fkoord-fkonj--mf = fkoord-fkonj--mf* mf %fkoord-fkonj--mf[+mf:konj]%
	setscore -1.94591014905531

fkoord-fkonj--vc = fkoord-fkonj--comma* vc %fkoord-fkonj--comma[+vc:konj]%
	setscore -0.980829253011726

fkoord-fkonj--vc = fkoord-fkonj--conj* vc %fkoord-fkonj--conj[+vc:konj]%
	setscore -0.470003629245736

fkoord-fkoord = fkoord* %FKOORD[fkoord:konj]%
	Block := 1,
	setscore 0

fkoord-fkoord--comma = fkoord-fkoord* comma %fkoord-fkoord[+comma]%
	setscore 0

fkoord-fkoord--conj = fkoord-fkoord--comma* conj %fkoord-fkoord--comma[+conj]%
	setscore -0.693147180559945

fkoord-fkoord--conj = fkoord-fkoord--fkonj* conj %fkoord-fkoord--fkonj[+conj]%
	setscore -0.693147180559945

fkoord-fkoord--fkonj = fkoord-fkoord--comma* fkonj %fkoord-fkoord--comma[+fkonj:konj]%
	setscore -0.693147180559945

fkoord-fkoord--fkonj = fkoord-fkoord--conj* fkonj %fkoord-fkoord--conj[+fkonj:konj]%
	setscore -0.693147180559945

fkoord-fkoord--nf = fkoord-fkoord--comma* nf %fkoord-fkoord--comma[+nf:konj]%
	setscore 0

fkoord-lk = lk* %FKOORD[lk:konj]%
	Block := 1,
	setscore 0

fkoord-lk--comma = comma fkoord-lk* %fkoord-lk[comma+]%
	setscore -1.38629436111989

fkoord-lk--comma = fkoord-lk* comma %fkoord-lk[+comma]%
	setscore -0.693147180559945

fkoord-lk--comma = fkoord-lk--fkonj* comma %fkoord-lk--fkonj[+comma]%
	setscore -1.38629436111989

fkoord-lk--conj = conj fkoord-lk* %fkoord-lk[conj+]%
	setscore -1.6094379124341

fkoord-lk--conj = fkoord-lk* conj %fkoord-lk[+conj]%
	setscore -0.916290731874155

fkoord-lk--conj = fkoord-lk--fkonj* conj %fkoord-lk--fkonj[+conj]%
	setscore -1.6094379124341

fkoord-lk--fkonj = fkonj fkoord-lk--comma* %fkoord-lk--comma[fkonj:konj+]%
	setscore -1.6094379124341

fkoord-lk--fkonj = fkonj fkoord-lk--conj* %fkoord-lk--conj[fkonj:konj+]%
	setscore -2.30258509299405

fkoord-lk--fkonj = fkoord-lk--comma* fkonj %fkoord-lk--comma[+fkonj:konj]%
	setscore -0.916290731874155

fkoord-lk--fkonj = fkoord-lk--conj* fkonj %fkoord-lk--conj[+fkonj:konj]%
	setscore -1.20397280432594

fkoord-lk--fkoord = fkoord-lk* fkoord %fkoord-lk[+fkoord:konj]%
	setscore -0.693147180559945

fkoord-lk--fkoord = fkoord-lk--comma* fkoord %fkoord-lk--comma[+fkoord:konj]%
	setscore -0.693147180559945

fkoord-lk--lk = fkoord-lk--comma* lk %fkoord-lk--comma[+lk:konj]%
	setscore -0.693147180559945

fkoord-lk--lk = fkoord-lk--conj* lk %fkoord-lk--conj[+lk:konj]%
	setscore -0.693147180559945

fkoord-mf = mf* %FKOORD[mf:konj]%
	Block := 1,
	setscore 0

fkoord-mf--comma = fkoord-mf* comma %fkoord-mf[+comma]%
	setscore 0

fkoord-mf--conj = fkoord-mf* conj %fkoord-mf[+conj]%
	setscore -0.847297860387204

fkoord-mf--conj = fkoord-mf--comma* conj %fkoord-mf--comma[+conj]%
	setscore -1.25276296849537

fkoord-mf--conj = fkoord-mf--mf* conj %fkoord-mf--mf[+conj]%
	setscore -1.94591014905531

fkoord-mf--conj = fkoord-mf--vc* conj %fkoord-mf--vc[+conj]%
	setscore -1.94591014905531

fkoord-mf--fkonj = fkoord-mf* fkonj %fkoord-mf[+fkonj:konj]%
	setscore -1.94591014905531

fkoord-mf--fkonj = fkoord-mf--comma* fkonj %fkoord-mf--comma[+fkonj:konj]%
	setscore -1.94591014905531

fkoord-mf--fkonj = fkoord-mf--conj* fkonj %fkoord-mf--conj[+fkonj:konj]%
	setscore -0.559615787935423

fkoord-mf--mf = fkoord-mf* mf %fkoord-mf[+mf:konj]%
	setscore -1.6094379124341

fkoord-mf--mf = fkoord-mf--comma* mf %fkoord-mf--comma[+mf:konj]%
	setscore -0.916290731874155

fkoord-mf--mf = fkoord-mf--conj* mf %fkoord-mf--conj[+mf:konj]%
	setscore -0.916290731874155

fkoord-mf--vc = fkoord-mf--comma* vc %fkoord-mf--comma[+vc:konj]%
	setscore 0

fkoord-vc = vc* %FKOORD[vc:konj]%
	Block := 1,
	setscore 0

fkoord-vc--comma = fkoord-vc* comma %fkoord-vc[+comma]%
	setscore 0

fkoord-vc--conj = fkoord-vc* conj %fkoord-vc[+conj]%
	setscore -0.693147180559945

fkoord-vc--conj = fkoord-vc--comma* conj %fkoord-vc--comma[+conj]%
	setscore -1.38629436111989

fkoord-vc--conj = fkoord-vc--vc* conj %fkoord-vc--vc[+conj]%
	setscore -1.38629436111989

fkoord-vc--fkonj = fkoord-vc--comma* fkonj %fkoord-vc--comma[+fkonj:konj]%
	setscore -1.38629436111989

fkoord-vc--fkonj = fkoord-vc--conj* fkonj %fkoord-vc--conj[+fkonj:konj]%
	setscore -0.287682072451781

fkoord-vc--vc = fkoord-vc--comma* vc %fkoord-vc--comma[+vc:konj]%
	setscore -0.693147180559945

fkoord-vc--vc = fkoord-vc--conj* vc %fkoord-vc--conj[+vc:konj]%
	setscore -0.693147180559945

fx = fx-advx--pp*
	{ Block := false },
	setscore -3.78418963391826

fx = fx-fx--advx*
	{ Block := false },
	setscore -3.09104245335832

fx = fx-fx--ap*
	{ Block := false },
	setscore -3.78418963391826

fx = fx-fx--conj*
	{ Block := false },
	setscore -3.78418963391826

fx = fx-fx--fx*
	{ Block := false },
	setscore -2.68557734525015

fx = fx-fx--np*
	{ Block := false },
	setscore -2.17475172148416

fx = fx-fx--pp*
	{ Block := false },
	setscore -2.17475172148416

fx = fx-fx--simpx*
	{ Block := false },
	setscore -3.78418963391826

fx = fx-np--np*
	{ Block := false },
	setscore -3.09104245335832

fx = fx-np--unknown*
	{ Block := false },
	setscore -2.17475172148416

fx = fx-prn--prn*
	{ Block := false },
	setscore -3.78418963391826

fx = fx-unknown*
	{ Block := false },
	setscore -3.78418963391826

fx = fx-unknown--advx*
	{ Block := false },
	setscore -3.09104245335832

fx = fx-unknown--np*
	{ Block := false },
	setscore -2.68557734525015

fx = fx-unknown--unknown*
	{ Block := false },
	setscore -2.17475172148416

fx-advx = advx* %FX[advx]%
	Block := 1,
	setscore 0

fx-advx--pp = fx-advx* pp %fx-advx[+pp:compl1]%
	setscore 0

fx-fx = fx* %FX[fx:app]%
	Block := 1,
	setscore -1.89711998488588

fx-fx = fx* %FX[fx:konj]%
	Block := 1,
	setscore -1.38629436111989

fx-fx = fx* %FX[fx]%
	Block := 1,
	setscore -0.510825623765991

fx-fx--advx = advx fx-fx* %fx-fx[advx+]%
	setscore -1.09861228866811

fx-fx--advx = fx-fx* advx %fx-fx[+advx]%
	setscore -1.09861228866811

fx-fx--advx = fx-fx--comma* advx %fx-fx--comma[+advx]%
	setscore -1.09861228866811

fx-fx--ap = ap fx-fx* %fx-fx[ap+]%
	setscore 0

fx-fx--comma = fx-fx* comma %fx-fx[+comma]%
	setscore 0

fx-fx--conj = conj fx-fx* %fx-fx[conj+]%
	setscore -1.79175946922806

fx-fx--conj = conj fx-fx--pp* %fx-fx--pp[conj+]%
	setscore -1.79175946922806

fx-fx--conj = fx-fx* conj %fx-fx[+conj]%
	setscore -0.693147180559945

fx-fx--fx = fx-fx* fx %fx-fx[+fx:konj]%
	setscore -1.09861228866811

fx-fx--fx = fx-fx--comma* fx %fx-fx--comma[+fx:konj]%
	setscore -1.09861228866811

fx-fx--fx = fx-fx--conj* fx %fx-fx--conj[+fx:konj]%
	setscore -1.09861228866811

fx-fx--np = fx-fx* np %fx-fx[+np:app]%
	setscore -1.6094379124341

fx-fx--np = fx-fx* np %fx-fx[+np:possessive]%
	setscore -1.6094379124341

fx-fx--np = fx-fx* np %fx-fx[+np]%
	setscore -1.6094379124341

fx-fx--np = fx-fx--comma* np %fx-fx--comma[+np:app]%
	setscore -1.6094379124341

fx-fx--np = fx-fx--conj* np %fx-fx--conj[+np:konj]%
	setscore -1.6094379124341

fx-fx--pp = fx-fx* pp %fx-fx[+pp:compl1]%
	setscore -2.19722457733622

fx-fx--pp = fx-fx--advx* pp %fx-fx--advx[+pp:compl1]%
	setscore -2.19722457733622

fx-fx--pp = fx-fx--conj* pp %fx-fx--conj[+pp:compl1]%
	setscore -2.19722457733622

fx-fx--pp = fx-fx--conj* pp %fx-fx--conj[+pp:konj]%
	setscore -2.19722457733622

fx-fx--pp = pp fx-fx* %fx-fx[pp:compl1+]%
	setscore -2.19722457733622

fx-fx--pp = pp fx-fx--pp* %fx-fx--pp[pp:compl1+]%
	setscore -2.19722457733622

fx-fx--simpx = fx-fx* simpx %fx-fx[+simpx:app]%
	setscore 0

fx-np = np* %FX[np:possessive]%
	Block := 1,
	setscore -0.847297860387204

fx-np = np* %FX[np]%
	Block := 1,
	setscore -0.559615787935423

fx-np--comma = fx-np* comma %fx-np[+comma]%
	setscore 0

fx-np--np = fx-np--unknown* np %fx-np--unknown[+np]%
	setscore 0

fx-np--unknown = fx-np* unknown %fx-np[+unknown]%
	setscore -0.606135803570316

fx-np--unknown = fx-np--comma* unknown %fx-np--comma[+unknown]%
	setscore -2.39789527279837

fx-np--unknown = fx-np--unknown* unknown %fx-np--unknown[+unknown]%
	setscore -1.01160091167848

fx-prn = prn* %FX[prn]%
	Block := 1,
	setscore 0

fx-prn--prn = fx-prn* prn %fx-prn[+prn]%
	setscore 0

fx-unknown = unknown* %FX[unknown]%
	Block := 1,
	setscore 0

fx-unknown--advx = advx fx-unknown* %fx-unknown[advx+]%
	setscore -0.693147180559945

fx-unknown--advx = fx-unknown--unknown* advx %fx-unknown--unknown[+advx]%
	setscore -0.693147180559945

fx-unknown--comma = fx-unknown* comma %fx-unknown[+comma]%
	setscore 0

fx-unknown--conj = fx-unknown* conj %fx-unknown[+conj]%
	setscore -0.693147180559945

fx-unknown--np = fx-unknown* np %fx-unknown[+np]%
	setscore -0.510825623765991

fx-unknown--np = fx-unknown--unknown* np %fx-unknown--unknown[+np]%
	setscore -0.916290731874155

fx-unknown--prn = fx-unknown--comma* prn %fx-unknown--comma[+prn]%
	setscore 0

fx-unknown--unknown = fx-unknown* unknown %fx-unknown[+unknown]%
	setscore -1.38629436111989

fx-unknown--unknown = fx-unknown--comma* unknown %fx-unknown--comma[+unknown]%
	setscore -1.38629436111989

fx-unknown--unknown = fx-unknown--conj* unknown %fx-unknown--conj[+unknown]%
	setscore -2.07944154167984

fx-unknown--unknown = fx-unknown--np* unknown %fx-unknown--np[+unknown]%
	setscore -1.38629436111989

fx-unknown--unknown = fx-unknown--prn* unknown %fx-unknown--prn[+unknown]%
	setscore -2.07944154167984

koord = koord-conj*
	{ Block := false },
	setscore 0

koord-conj = conj* %KOORD[conj]%
	Block := 1,
	setscore 0

lk = lk-fx*
	setscore -0.693147180559945

lk = lk-vp*
	setscore -0.693147180559945

lk-fx = fx* %LK[fx]%
	Block := 1,
	setscore 0

lk-vp = vp* %LK[vp]%
	Block := 1,
	setscore 0

lv = lv-advx*
	{ Block := false },
	setscore -2.12026353620009

lv = lv-ap*
	{ Block := false },
	setscore -1.83258146374831

lv = lv-fx*
	{ Block := false },
	setscore -2.12026353620009

lv = lv-np*
	{ Block := false },
	setscore -1.6094379124341

lv = lv-pp*
	{ Block := false },
	setscore -2.12026353620009

lv = lv-simpx*
	{ Block := false },
	setscore -1.27296567581289

lv-advx = advx* %LV[advx:mod-mod]%
	Block := 1,
	setscore -1.09861228866811

lv-advx = advx* %LV[advx:mod]%
	Block := 1,
	setscore -1.09861228866811

lv-advx = advx* %LV[advx:opp-mod]%
	Block := 1,
	setscore -1.09861228866811

lv-ap = ap* %LV[ap:mod-mod]%
	Block := 1,
	setscore -1.38629436111989

lv-ap = ap* %LV[ap:oa-mod]%
	Block := 1,
	setscore -1.38629436111989

lv-ap = ap* %LV[ap:on-mod]%
	Block := 1,
	setscore -1.38629436111989

lv-ap = ap* %LV[ap:v-mod]%
	Block := 1,
	setscore -1.38629436111989

lv-fx = fx* %LV[fx:oa-mod]%
	Block := 1,
	setscore -1.09861228866811

lv-fx = fx* %LV[fx:on-mod]%
	Block := 1,
	setscore -1.09861228866811

lv-fx = fx* %LV[fx:pred-mod]%
	Block := 1,
	setscore -1.09861228866811

lv-np = np* %LV[np:mod-mod]%
	Block := 1,
	setscore -1.6094379124341

lv-np = np* %LV[np:mod]%
	Block := 1,
	setscore -1.6094379124341

lv-np = np* %LV[np:oa-mod]%
	Block := 1,
	setscore -1.6094379124341

lv-np = np* %LV[np:on-mod]%
	Block := 1,
	setscore -1.6094379124341

lv-np = np* %LV[np:pred-mod]%
	Block := 1,
	setscore -1.6094379124341

lv-pp = pp* %LV[pp:mod-mod]%
	Block := 1,
	setscore -1.09861228866811

lv-pp = pp* %LV[pp:oadvp-mo]%
	Block := 1,
	setscore -1.09861228866811

lv-pp = pp* %LV[pp:on-mod]%
	Block := 1,
	setscore -1.09861228866811

lv-simpx = simpx* %LV[simpx:fopp-mod]%
	Block := 1,
	setscore -1.94591014905531

lv-simpx = simpx* %LV[simpx:mod-mod]%
	Block := 1,
	setscore -1.94591014905531

lv-simpx = simpx* %LV[simpx:mod]%
	Block := 1,
	setscore -1.94591014905531

lv-simpx = simpx* %LV[simpx:oa-mod]%
	Block := 1,
	setscore -1.94591014905531

lv-simpx = simpx* %LV[simpx:od-mod]%
	Block := 1,
	setscore -1.94591014905531

lv-simpx = simpx* %LV[simpx:on-mod]%
	Block := 1,
	setscore -1.94591014905531

lv-simpx = simpx* %LV[simpx:opp-mod]%
	Block := 1,
	setscore -1.94591014905531

mf = mf-advx*
	setscore -6.08026004589111

mf = mf-advx--advx*
	setscore -4.074926476365

mf = mf-advx--ap*
	setscore -3.63172104027399

mf = mf-advx--dm*
	setscore -8.02617019494643

mf = mf-advx--fx*
	setscore -6.63987583382654

mf = mf-advx--np*
	setscore -3.07034313734517

mf = mf-advx--pp*
	setscore -2.54970664301492

mf = mf-advx--simpx*
	setscore -6.23441072571837

mf = mf-ap*
	setscore -6.92755790627832

mf = mf-ap--advx*
	setscore -5.94672865326659

mf = mf-ap--ap*
	setscore -5.72358510195238

mf = mf-ap--fx*
	setscore -8.02617019494643

mf = mf-ap--np*
	setscore -5.13579843705026

mf = mf-ap--pp*
	setscore -4.52966263347995

mf = mf-dm*
	setscore -8.02617019494643

mf = mf-fx*
	setscore -6.92755790627832

mf = mf-fx--advx*
	setscore -6.92755790627832

mf = mf-fx--ap*
	setscore -6.41673228251233

mf = mf-fx--np*
	setscore -6.08026004589111

mf = mf-fx--pp*
	setscore -6.92755790627832

mf = mf-np*
	setscore -5.25358147270665

mf = mf-np--advx*
	setscore -2.44267388616473

mf = mf-np--ap*
	setscore -2.40576932922928

mf = mf-np--conj*
	setscore -7.33302301438648

mf = mf-np--fx*
	setscore -5.82894561761021

mf = mf-np--np*
	setscore -2.15970213801313

mf = mf-np--pp*
	setscore -1.44830883722538

mf = mf-np--simpx*
	setscore -5.62827492214806

mf = mf-pp*
	setscore -5.54126354515843

mf = mf-pp--advx*
	setscore -3.76349031790511

mf = mf-pp--ap*
	setscore -3.52636052461616

mf = mf-pp--fx*
	setscore -7.33302301438648

mf = mf-pp--np*
	setscore -2.7583120358831

mf = mf-pp--pp*
	setscore -2.87867571813297

mf = mf-pp--simpx*
	setscore -8.02617019494643

mf = mf-simpx*
	setscore -6.92755790627832

mf = mf-simpx--advx*
	setscore -6.92755790627832

mf = mf-simpx--ap*
	setscore -6.23441072571837

mf = mf-simpx--np*
	setscore -6.08026004589111

mf = mf-simpx--pp*
	setscore -6.41673228251233

mf-advx = advx* %MF[advx:mod]%
	Block := 1,
	setscore -0.62004752712034

mf-advx = advx* %MF[advx:mod]%
	Block := 1,
	OA := true,
	setscore -3.77540994626622

mf-advx = advx* %MF[advx:mod]%
	Block := 1,
	OPP := true,
	setscore -5.64721212316781

mf-advx = advx* %MF[advx:mod]%
	Block := 1,
	PRED := true,
	setscore -1.75539182505718

mf-advx = advx* %MF[advx:oadjp]%
	Block := 1,
	setscore -4.95406494260786

mf-advx = advx* %MF[advx:oadvp]%
	Block := 1,
	setscore -5.64721212316781

mf-advx = advx* %MF[advx:od]%
	Block := 1,
	setscore -6.34035930372775

mf-advx = advx* %MF[advx:on-mod]%
	Block := 1,
	setscore -6.34035930372775

mf-advx = advx* %MF[advx:on-mod]%
	Block := 1,
	PRED := true,
	setscore -6.34035930372775

mf-advx = advx* %MF[advx:pred-mod]%
	Block := 1,
	setscore -6.34035930372775

mf-advx = advx* %MF[advx:pred]%
	Block := 1,
	PRED := true,
	setscore -5.24174701505964

mf-advx = advx* %MF[advx:v-mod]%
	Block := 1,
	setscore -1.69596840458638

mf-advx = advx* %MF[advx:v-mod]%
	Block := 1,
	OA := true,
	setscore -5.24174701505964

mf-advx = advx* %MF[advx:v-mod]%
	Block := 1,
	OPP := true,
	setscore -6.34035930372775

mf-advx = advx* %MF[advx:v-mod]%
	Block := 1,
	PRED := true,
	setscore -3.04452243772342

mf-advx = advx* %MF[advx]%
	Block := 1,
	PRED := true,
	setscore -6.34035930372775

mf-advx--advx = mf-advx* advx %mf-advx[+advx:mod]%
	setscore -0.850067945261019

mf-advx--advx = mf-advx* advx %mf-advx[+advx:oadjp]%
	setscore -4.38618464482255

mf-advx--advx = mf-advx* advx %mf-advx[+advx:oadvp]%
	setscore -4.79164975293071

mf-advx--advx = mf-advx* advx %mf-advx[+advx:pred]%
	setscore -4.09850257237076

mf-advx--advx = mf-advx* advx %mf-advx[+advx:v-mod]%
	setscore -2.54035795432421

mf-advx--advx = mf-advx--ap* advx %mf-advx--ap[+advx:mod]%
	setscore -4.09850257237076

mf-advx--advx = mf-advx--ap* advx %mf-advx--ap[+advx:oadvp]%
	setscore -5.48479693349065

mf-advx--advx = mf-advx--comma* advx %mf-advx--comma[+advx:mod]%
	setscore -3.40535539181082

mf-advx--advx = mf-advx--comma* advx %mf-advx--comma[+advx:v-mod]%
	setscore -4.79164975293071

mf-advx--advx = mf-advx--np* advx %mf-advx--np[+advx:mod]%
	setscore -1.90127799503455

mf-advx--advx = mf-advx--np* advx %mf-advx--np[+advx:oadjp]%
	setscore -5.48479693349065

mf-advx--advx = mf-advx--np* advx %mf-advx--np[+advx:oadvp]%
	setscore -5.48479693349065

mf-advx--advx = mf-advx--np* advx %mf-advx--np[+advx:pred]%
	setscore -4.38618464482255

mf-advx--advx = mf-advx--np* advx %mf-advx--np[+advx:v-mod]%
	setscore -3.6930374642626

mf-advx--advx = mf-advx--pp* advx %mf-advx--pp[+advx:mod]%
	setscore -2.05080972900551

mf-advx--advx = mf-advx--pp* advx %mf-advx--pp[+advx:oadjp]%
	setscore -5.48479693349065

mf-advx--advx = mf-advx--pp* advx %mf-advx--pp[+advx:oadvp]%
	setscore -5.48479693349065

mf-advx--advx = mf-advx--pp* advx %mf-advx--pp[+advx:pred]%
	setscore -4.79164975293071

mf-advx--advx = mf-advx--pp* advx %mf-advx--pp[+advx:v-mod]%
	setscore -3.40535539181082

mf-advx--ap = mf-advx* ap %mf-advx[+ap:pred]%
	setscore -2.98856378407538

mf-advx--ap = mf-advx* ap %mf-advx[+ap:v-mod]%
	setscore -1.79897971720154

mf-advx--ap = mf-advx--advx* ap %mf-advx--advx[+ap:pred]%
	setscore -2.53657866033232

mf-advx--ap = mf-advx--advx* ap %mf-advx--advx[+ap:v-mod]%
	setscore -2.16188521089091

mf-advx--ap = mf-advx--ap* ap %mf-advx--ap[+ap:pred]%
	setscore -4.93447393313069

mf-advx--ap = mf-advx--comma* ap %mf-advx--comma[+ap:pred]%
	setscore -3.5481795720108

mf-advx--ap = mf-advx--comma* ap %mf-advx--comma[+ap:v-mod]%
	setscore -3.5481795720108

mf-advx--ap = mf-advx--fx* ap %mf-advx--fx[+ap:pred]%
	setscore -4.93447393313069

mf-advx--ap = mf-advx--np* ap %mf-advx--np[+ap:pred]%
	setscore -2.04410217523453

mf-advx--ap = mf-advx--np* ap %mf-advx--np[+ap:v-mod]%
	setscore -1.84343147977238

mf-advx--ap = mf-advx--pp* ap %mf-advx--pp[+ap:pred]%
	setscore -2.04410217523453

mf-advx--ap = mf-advx--pp* ap %mf-advx--pp[+ap:v-mod]%
	setscore -2.63188884013665

mf-advx--comma = mf-advx* comma %mf-advx[+comma]%
	setscore -1.15923691048454

mf-advx--comma = mf-advx--advx* comma %mf-advx--advx[+comma]%
	setscore -2.83321334405622

mf-advx--comma = mf-advx--np* comma %mf-advx--np[+comma]%
	setscore -0.84078317936601

mf-advx--comma = mf-advx--pp* comma %mf-advx--pp[+comma]%
	setscore -1.62924053973028

mf-advx--conj = mf-advx--np* conj %mf-advx--np[+conj]%
	setscore 0

mf-advx--dm = mf-advx--np* dm %mf-advx--np[+dm:compl1]%
	setscore 0

mf-advx--fx = mf-advx* fx %mf-advx[+fx:mod]%
	setscore -1.38629436111989

mf-advx--fx = mf-advx* fx %mf-advx[+fx:oa]%
	setscore -2.07944154167984

mf-advx--fx = mf-advx* fx %mf-advx[+fx:pred]%
	setscore -2.07944154167984

mf-advx--fx = mf-advx* fx %mf-advx[+fx:subject]%
	setscore -2.07944154167984

mf-advx--fx = mf-advx* fx %mf-advx[+fx:v-mod]%
	setscore -2.07944154167984

mf-advx--fx = mf-advx--advx* fx %mf-advx--advx[+fx:compl1]%
	setscore -2.07944154167984

mf-advx--np = mf-advx* np %mf-advx[+np:compl1]%
	setscore -2.51595127146996

mf-advx--np = mf-advx* np %mf-advx[+np:compl2]%
	setscore -3.82428409112014

mf-advx--np = mf-advx* np %mf-advx[+np:mod]%
	setscore -5.02825689544608

mf-advx--np = mf-advx* np %mf-advx[+np:oa]%
	OA_C := np.C,
	setscore -4.33510971488613

mf-advx--np = mf-advx* np %mf-advx[+np:oa]%
	OA_C := np.C,
	np.C == dat,
	setscore -6.12686918411419

mf-advx--np = mf-advx* np %mf-advx[+np:oa]%
	OA_C := np.C,
	np.C == gen,
	setscore -6.12686918411419

mf-advx--np = mf-advx* np %mf-advx[+np:oa]%
	OA_C := np.C,
	np.C == nom,
	setscore -6.12686918411419

mf-advx--np = mf-advx* np %mf-advx[+np:oa]%
	setscore -6.12686918411419

mf-advx--np = mf-advx* np %mf-advx[+np:od]%
	OD := true,
	OD_C := np.C,
	np.C == dat,
	setscore -6.12686918411419

mf-advx--np = mf-advx* np %mf-advx[+np:on-mod]%
	setscore -3.3542804618744

mf-advx--np = mf-advx* np %mf-advx[+np:pred]%
	setscore -4.33510971488613

mf-advx--np = mf-advx* np %mf-advx[+np:subject]%
	setscore -1.40837031281909

mf-advx--np = mf-advx* np %mf-advx[+np:v-mod]%
	setscore -3.48781185449893

mf-advx--np = mf-advx--advx* np %mf-advx--advx[+np:compl1]%
	setscore -3.23649742621802

mf-advx--np = mf-advx--advx* np %mf-advx--advx[+np:compl2]%
	setscore -5.43372200355424

mf-advx--np = mf-advx--advx* np %mf-advx--advx[+np:oa-mod]%
	setscore -6.12686918411419

mf-advx--np = mf-advx--advx* np %mf-advx--advx[+np:oa]%
	OA_C := np.C,
	setscore -5.43372200355424

mf-advx--np = mf-advx--advx* np %mf-advx--advx[+np:on-mod]%
	setscore -6.12686918411419

mf-advx--np = mf-advx--advx* np %mf-advx--advx[+np:pred]%
	setscore -4.04742764243435

mf-advx--np = mf-advx--advx* np %mf-advx--advx[+np:subject]%
	setscore -2.51595127146996

mf-advx--np = mf-advx--advx* np %mf-advx--advx[+np:v-mod]%
	setscore -4.18095903505887

mf-advx--np = mf-advx--ap* np %mf-advx--ap[+np:compl1]%
	setscore -4.04742764243435

mf-advx--np = mf-advx--ap* np %mf-advx--ap[+np:pred]%
	setscore -5.43372200355424

mf-advx--np = mf-advx--ap* np %mf-advx--ap[+np:subject]%
	setscore -4.51743127168008

mf-advx--np = mf-advx--comma* np %mf-advx--comma[+np:compl1]%
	setscore -4.18095903505887

mf-advx--np = mf-advx--comma* np %mf-advx--comma[+np:pred]%
	setscore -5.02825689544608

mf-advx--np = mf-advx--comma* np %mf-advx--comma[+np:subject]%
	setscore -5.02825689544608

mf-advx--np = mf-advx--np* np %mf-advx--np[+np:compl1]%
	setscore -2.66113328131446

mf-advx--np = mf-advx--np* np %mf-advx--np[+np:mod-mod]%
	setscore -6.12686918411419

mf-advx--np = mf-advx--np* np %mf-advx--np[+np:mod]%
	setscore -6.12686918411419

mf-advx--np = mf-advx--np* np %mf-advx--np[+np:oa]%
	OA_C := np.C,
	setscore -6.12686918411419

mf-advx--np = mf-advx--np* np %mf-advx--np[+np:oa]%
	setscore -6.12686918411419

mf-advx--np = mf-advx--np* np %mf-advx--np[+np:pred]%
	setscore -4.51743127168008

mf-advx--np = mf-advx--np* np %mf-advx--np[+np:subject]%
	setscore -4.33510971488613

mf-advx--np = mf-advx--np* np %mf-advx--np[+np:v-mod]%
	setscore -6.12686918411419

mf-advx--np = mf-advx--pp* np %mf-advx--pp[+np:compl1]%
	setscore -2.57152112262477

mf-advx--np = mf-advx--pp* np %mf-advx--pp[+np:compl2]%
	setscore -4.74057482299429

mf-advx--np = mf-advx--pp* np %mf-advx--pp[+np:mod-mod]%
	setscore -6.12686918411419

mf-advx--np = mf-advx--pp* np %mf-advx--pp[+np:mod]%
	setscore -6.12686918411419

mf-advx--np = mf-advx--pp* np %mf-advx--pp[+np:oa]%
	OA := true,
	OA_C := np.C,
	np.C == acc,
	setscore -6.12686918411419

mf-advx--np = mf-advx--pp* np %mf-advx--pp[+np:oa]%
	OA_C := np.C,
	setscore -6.12686918411419

mf-advx--np = mf-advx--pp* np %mf-advx--pp[+np:oa]%
	OA_C := np.C,
	np.C == nom,
	setscore -6.12686918411419

mf-advx--np = mf-advx--pp* np %mf-advx--pp[+np:on-mod]%
	setscore -6.12686918411419

mf-advx--np = mf-advx--pp* np %mf-advx--pp[+np:pred]%
	setscore -4.18095903505887

mf-advx--np = mf-advx--pp* np %mf-advx--pp[+np:subject]%
	setscore -2.94881535376624

mf-advx--np = mf-advx--pp* np %mf-advx--pp[+np:v-mod]%
	setscore -5.02825689544608

mf-advx--pp = mf-advx* pp %mf-advx[+pp:compl1]%
	setscore -3.97656152656572

mf-advx--pp = mf-advx* pp %mf-advx[+pp:compl2]%
	setscore -4.78749174278205

mf-advx--pp = mf-advx* pp %mf-advx[+pp:fopp]%
	setscore -3.12926366617851

mf-advx--pp = mf-advx* pp %mf-advx[+pp:mod]%
	setscore -3.17805383034795

mf-advx--pp = mf-advx* pp %mf-advx[+pp:oa-mod]%
	setscore -5.48063892334199

mf-advx--pp = mf-advx* pp %mf-advx[+pp:on-mod]%
	setscore -6.17378610390194

mf-advx--pp = mf-advx* pp %mf-advx[+pp:opp]%
	OPP_C := pp.C,
	OPP_Przyim := pp.Przyim,
	setscore -5.48063892334199

mf-advx--pp = mf-advx* pp %mf-advx[+pp:pred]%
	setscore -4.78749174278205

mf-advx--pp = mf-advx* pp %mf-advx[+pp:v-mod]%
	setscore -1.74296930505862

mf-advx--pp = mf-advx--advx* pp %mf-advx--advx[+pp:compl1]%
	setscore -3.68887945411394

mf-advx--pp = mf-advx--advx* pp %mf-advx--advx[+pp:compl2]%
	setscore -5.07517381523383

mf-advx--pp = mf-advx--advx* pp %mf-advx--advx[+pp:fopp]%
	setscore -3.68887945411394

mf-advx--pp = mf-advx--advx* pp %mf-advx--advx[+pp:mod]%
	setscore -4.22787595484662

mf-advx--pp = mf-advx--advx* pp %mf-advx--advx[+pp:on-mod]%
	setscore -6.17378610390194

mf-advx--pp = mf-advx--advx* pp %mf-advx--advx[+pp:opp]%
	OPP_C := pp.C,
	OPP_Przyim := pp.Przyim,
	setscore -6.17378610390194

mf-advx--pp = mf-advx--advx* pp %mf-advx--advx[+pp:pred]%
	setscore -5.07517381523383

mf-advx--pp = mf-advx--advx* pp %mf-advx--advx[+pp:v-mod]%
	setscore -2.64742557928578

mf-advx--pp = mf-advx--ap* pp %mf-advx--ap[+pp:compl1]%
	setscore -4.0943445622221

mf-advx--pp = mf-advx--ap* pp %mf-advx--ap[+pp:compl2]%
	setscore -4.78749174278205

mf-advx--pp = mf-advx--ap* pp %mf-advx--ap[+pp:fopp]%
	setscore -3.87120101090789

mf-advx--pp = mf-advx--ap* pp %mf-advx--ap[+pp:mod]%
	setscore -5.07517381523383

mf-advx--pp = mf-advx--ap* pp %mf-advx--ap[+pp:pred]%
	setscore -6.17378610390194

mf-advx--pp = mf-advx--ap* pp %mf-advx--ap[+pp:v-mod]%
	setscore -4.56434819146784

mf-advx--pp = mf-advx--comma* pp %mf-advx--comma[+pp:compl2]%
	setscore -6.17378610390194

mf-advx--pp = mf-advx--comma* pp %mf-advx--comma[+pp:fopp]%
	setscore -5.48063892334199

mf-advx--pp = mf-advx--comma* pp %mf-advx--comma[+pp:mod]%
	setscore -6.17378610390194

mf-advx--pp = mf-advx--comma* pp %mf-advx--comma[+pp:pred]%
	setscore -6.17378610390194

mf-advx--pp = mf-advx--comma* pp %mf-advx--comma[+pp:v-mod]%
	setscore -3.77589083110357

mf-advx--pp = mf-advx--fx* pp %mf-advx--fx[+pp:fopp]%
	setscore -6.17378610390194

mf-advx--pp = mf-advx--fx* pp %mf-advx--fx[+pp:v-mod]%
	setscore -6.17378610390194

mf-advx--pp = mf-advx--np* pp %mf-advx--np[+pp:compl1]%
	setscore -3.46573590279973

mf-advx--pp = mf-advx--np* pp %mf-advx--np[+pp:compl2]%
	setscore -3.77589083110357

mf-advx--pp = mf-advx--np* pp %mf-advx--np[+pp:fopp]%
	setscore -2.70805020110221

mf-advx--pp = mf-advx--np* pp %mf-advx--np[+pp:mod-mod]%
	setscore -6.17378610390194

mf-advx--pp = mf-advx--np* pp %mf-advx--np[+pp:mod]%
	setscore -3.34057275984572

mf-advx--pp = mf-advx--np* pp %mf-advx--np[+pp:pred]%
	setscore -4.38202663467388

mf-advx--pp = mf-advx--np* pp %mf-advx--np[+pp:v-mod]%
	setscore -2.22254238532051

mf-advx--pp = mf-advx--pp* pp %mf-advx--pp[+pp:compl1]%
	setscore -3.77589083110357

mf-advx--pp = mf-advx--pp* pp %mf-advx--pp[+pp:compl2]%
	setscore -6.17378610390194

mf-advx--pp = mf-advx--pp* pp %mf-advx--pp[+pp:fopp]%
	setscore -3.77589083110357

mf-advx--pp = mf-advx--pp* pp %mf-advx--pp[+pp:mod]%
	setscore -3.6088367464404

mf-advx--pp = mf-advx--pp* pp %mf-advx--pp[+pp:pred]%
	setscore -4.78749174278205

mf-advx--pp = mf-advx--pp* pp %mf-advx--pp[+pp:v-mod]%
	setscore -2.95491027903374

mf-advx--simpx = mf-advx--advx* simpx %mf-advx--advx[+simpx:os]%
	setscore -2.19722457733622

mf-advx--simpx = mf-advx--comma* simpx %mf-advx--comma[+simpx:mod]%
	setscore -1.50407739677627

mf-advx--simpx = mf-advx--comma* simpx %mf-advx--comma[+simpx:os]%
	setscore -2.19722457733622

mf-advx--simpx = mf-advx--comma* simpx %mf-advx--comma[+simpx:subject]%
	setscore -2.19722457733622

mf-advx--simpx = mf-advx--np* simpx %mf-advx--np[+simpx:os]%
	setscore -2.19722457733622

mf-ap = ap* %MF[ap:mod]%
	Block := 1,
	setscore -4.30406509320417

mf-ap = ap* %MF[ap:on-mod]%
	Block := 1,
	setscore -4.30406509320417

mf-ap = ap* %MF[ap:pred]%
	Block := 1,
	PRED := true,
	setscore -2.00148000021012

mf-ap = ap* %MF[ap:v-mod]%
	Block := 1,
	setscore -0.352821374622742

mf-ap = ap* %MF[ap:v-mod]%
	Block := 1,
	PRED := true,
	setscore -2.00148000021012

mf-ap--advx = mf-ap* advx %mf-ap[+advx:mod]%
	setscore -0.773189888233482

mf-ap--advx = mf-ap* advx %mf-ap[+advx:oadvp]%
	setscore -2.56494935746154

mf-ap--advx = mf-ap* advx %mf-ap[+advx:v-mod]%
	setscore -3.25809653802148

mf-ap--advx = mf-ap* advx %mf-ap[+advx]%
	setscore -3.25809653802148

mf-ap--advx = mf-ap--advx* advx %mf-ap--advx[+advx:mod]%
	setscore -2.56494935746154

mf-ap--advx = mf-ap--advx* advx %mf-ap--advx[+advx:v-mod]%
	setscore -3.25809653802148

mf-ap--advx = mf-ap--comma* advx %mf-ap--comma[+advx:mod]%
	setscore -3.25809653802148

mf-ap--advx = mf-ap--np* advx %mf-ap--np[+advx:mod]%
	setscore -2.56494935746154

mf-ap--advx = mf-ap--np* advx %mf-ap--np[+advx:v-mod]%
	setscore -3.25809653802148

mf-ap--advx = mf-ap--pp* advx %mf-ap--pp[+advx:mod]%
	setscore -2.56494935746154

mf-ap--ap = mf-ap* ap %mf-ap[+ap:pred]%
	setscore -2.484906649788

mf-ap--ap = mf-ap* ap %mf-ap[+ap:v-mod]%
	setscore -1.38629436111989

mf-ap--ap = mf-ap--advx* ap %mf-ap--advx[+ap:pred]%
	setscore -2.484906649788

mf-ap--ap = mf-ap--advx* ap %mf-ap--advx[+ap:v-mod]%
	setscore -1.79175946922806

mf-ap--ap = mf-ap--comma* ap %mf-ap--comma[+ap:pred]%
	setscore -2.484906649788

mf-ap--ap = mf-ap--np* ap %mf-ap--np[+ap:pred]%
	setscore -2.484906649788

mf-ap--ap = mf-ap--np* ap %mf-ap--np[+ap:v-mod]%
	setscore -1.79175946922806

mf-ap--ap = mf-ap--pp* ap %mf-ap--pp[+ap:pred]%
	setscore -2.484906649788

mf-ap--comma = mf-ap* comma %mf-ap[+comma]%
	setscore -1.09861228866811

mf-ap--comma = mf-ap--advx* comma %mf-ap--advx[+comma]%
	setscore -1.79175946922806

mf-ap--comma = mf-ap--np* comma %mf-ap--np[+comma]%
	setscore -1.09861228866811

mf-ap--comma = mf-ap--pp* comma %mf-ap--pp[+comma]%
	setscore -1.79175946922806

mf-ap--fx = mf-ap--pp* fx %mf-ap--pp[+fx:pred]%
	setscore 0

mf-ap--np = mf-ap* np %mf-ap[+np:compl1]%
	setscore -1.58923520511658

mf-ap--np = mf-ap* np %mf-ap[+np:pred]%
	setscore -3.19867311755068

mf-ap--np = mf-ap* np %mf-ap[+np:subject]%
	setscore -1.32687094064909

mf-ap--np = mf-ap* np %mf-ap[+np:v-mod]%
	setscore -3.89182029811063

mf-ap--np = mf-ap--advx* np %mf-ap--advx[+np:compl1]%
	setscore -2.50552593699074

mf-ap--np = mf-ap--advx* np %mf-ap--advx[+np:subject]%
	setscore -3.89182029811063

mf-ap--np = mf-ap--ap* np %mf-ap--ap[+np:compl1]%
	setscore -3.89182029811063

mf-ap--np = mf-ap--comma* np %mf-ap--comma[+np:compl1]%
	setscore -3.19867311755068

mf-ap--np = mf-ap--comma* np %mf-ap--comma[+np:pred]%
	setscore -3.89182029811063

mf-ap--np = mf-ap--np* np %mf-ap--np[+np:compl1]%
	setscore -2.28238238567653

mf-ap--np = mf-ap--np* np %mf-ap--np[+np:mod-mod]%
	setscore -3.19867311755068

mf-ap--np = mf-ap--np* np %mf-ap--np[+np:pred]%
	setscore -3.89182029811063

mf-ap--np = mf-ap--np* np %mf-ap--np[+np:subject]%
	setscore -3.89182029811063

mf-ap--np = mf-ap--pp* np %mf-ap--pp[+np:compl1]%
	setscore -2.79320800944252

mf-ap--np = mf-ap--pp* np %mf-ap--pp[+np:subject]%
	setscore -3.19867311755068

mf-ap--pp = mf-ap* pp %mf-ap[+pp:compl1]%
	setscore -3.19867311755068

mf-ap--pp = mf-ap* pp %mf-ap[+pp:compl2]%
	setscore -3.89182029811063

mf-ap--pp = mf-ap* pp %mf-ap[+pp:fopp]%
	setscore -2.28238238567653

mf-ap--pp = mf-ap* pp %mf-ap[+pp:mod]%
	setscore -3.19867311755068

mf-ap--pp = mf-ap* pp %mf-ap[+pp:pred]%
	setscore -3.89182029811063

mf-ap--pp = mf-ap* pp %mf-ap[+pp:v-mod]%
	setscore -1.49392502531226

mf-ap--pp = mf-ap--advx* pp %mf-ap--advx[+pp:fopp]%
	setscore -2.79320800944252

mf-ap--pp = mf-ap--advx* pp %mf-ap--advx[+pp:v-mod]%
	setscore -3.19867311755068

mf-ap--pp = mf-ap--ap* pp %mf-ap--ap[+pp:v-mod]%
	setscore -3.89182029811063

mf-ap--pp = mf-ap--comma* pp %mf-ap--comma[+pp:v-mod]%
	setscore -3.89182029811063

mf-ap--pp = mf-ap--np* pp %mf-ap--np[+pp:compl1]%
	setscore -3.89182029811063

mf-ap--pp = mf-ap--np* pp %mf-ap--np[+pp:compl2]%
	setscore -3.89182029811063

mf-ap--pp = mf-ap--np* pp %mf-ap--np[+pp:fopp]%
	setscore -2.79320800944252

mf-ap--pp = mf-ap--np* pp %mf-ap--np[+pp:mod]%
	setscore -2.79320800944252

mf-ap--pp = mf-ap--np* pp %mf-ap--np[+pp:v-mod]%
	setscore -2.10006082888257

mf-ap--pp = mf-ap--pp* pp %mf-ap--pp[+pp:compl1]%
	setscore -3.89182029811063

mf-ap--pp = mf-ap--pp* pp %mf-ap--pp[+pp:mod]%
	setscore -3.19867311755068

mf-ap--pp = mf-ap--pp* pp %mf-ap--pp[+pp:oa-mod]%
	setscore -3.89182029811063

mf-ap--pp = mf-ap--pp* pp %mf-ap--pp[+pp:v-mod]%
	setscore -3.19867311755068

mf-dm = dm* %MF[dm:compl1]%
	Block := 1,
	setscore 0

mf-fx = fx* %MF[fx:mod]%
	Block := 1,
	setscore -1.94591014905531

mf-fx = fx* %MF[fx:mod]%
	Block := 1,
	OA := true,
	setscore -3.04452243772342

mf-fx = fx* %MF[fx:mod]%
	Block := 1,
	PRED := true,
	setscore -1.94591014905531

mf-fx = fx* %MF[fx:oa]%
	Block := 1,
	setscore -2.35137525716348

mf-fx = fx* %MF[fx:pred]%
	Block := 1,
	setscore -3.04452243772342

mf-fx = fx* %MF[fx:subject]%
	Block := 1,
	setscore -1.25276296849537

mf-fx = fx* %MF[fx:subject]%
	Block := 1,
	PRED := true,
	setscore -1.94591014905531

mf-fx = fx* %MF[fx:v-mod]%
	Block := 1,
	setscore -3.04452243772342

mf-fx = fx* %MF[fx:v-mod]%
	Block := 1,
	PRED := true,
	setscore -3.04452243772342

mf-fx--advx = mf-fx* advx %mf-fx[+advx:mod]%
	setscore -0.318453731118535

mf-fx--advx = mf-fx--advx* advx %mf-fx--advx[+advx:mod]%
	setscore -1.70474809223843

mf-fx--advx = mf-fx--np* advx %mf-fx--np[+advx:v-mod]%
	setscore -2.39789527279837

mf-fx--ap = mf-fx* ap %mf-fx[+ap:pred]%
	setscore -1.6094379124341

mf-fx--ap = mf-fx--advx* ap %mf-fx--advx[+ap:pred]%
	setscore -1.6094379124341

mf-fx--ap = mf-fx--advx* ap %mf-fx--advx[+ap:v-mod]%
	setscore -1.6094379124341

mf-fx--ap = mf-fx--pp* ap %mf-fx--pp[+ap:pred]%
	setscore -0.916290731874155

mf-fx--np = mf-fx* np %mf-fx[+np:compl1]%
	setscore -1.38629436111989

mf-fx--np = mf-fx* np %mf-fx[+np:oa]%
	OA_C := np.C,
	setscore -2.07944154167984

mf-fx--np = mf-fx* np %mf-fx[+np:pred]%
	setscore -1.38629436111989

mf-fx--np = mf-fx* np %mf-fx[+np:subject]%
	setscore -2.07944154167984

mf-fx--np = mf-fx--advx* np %mf-fx--advx[+np:pred]%
	setscore -2.07944154167984

mf-fx--np = mf-fx--pp* np %mf-fx--pp[+np:compl1]%
	setscore -2.07944154167984

mf-fx--pp = mf-fx* pp %mf-fx[+pp:fopp]%
	setscore -1.79175946922806

mf-fx--pp = mf-fx* pp %mf-fx[+pp:v-mod]%
	setscore -1.09861228866811

mf-fx--pp = mf-fx--advx* pp %mf-fx--advx[+pp:compl1]%
	setscore -1.79175946922806

mf-fx--pp = mf-fx--advx* pp %mf-fx--advx[+pp:fopp]%
	setscore -1.79175946922806

mf-fx--pp = mf-fx--advx* pp %mf-fx--advx[+pp:v-mod]%
	setscore -1.79175946922806

mf-np = np* %MF[np:app]%
	Block := 1,
	setscore -7.47193207824512

mf-np = np* %MF[np:compl1]%
	Block := 1,
	setscore -1.44124681798386

mf-np = np* %MF[np:compl1]%
	Block := 1,
	OA := true,
	setscore -7.47193207824512

mf-np = np* %MF[np:compl1]%
	Block := 1,
	OG := true,
	setscore -7.47193207824512

mf-np = np* %MF[np:compl1]%
	Block := 1,
	OPP := true,
	setscore -5.68017260901707

mf-np = np* %MF[np:compl1]%
	Block := 1,
	PRED := true,
	setscore -2.86676189225703

mf-np = np* %MF[np:compl2]%
	Block := 1,
	setscore -3.12812665639144

mf-np = np* %MF[np:compl2]%
	Block := 1,
	OA := true,
	setscore -6.37331978957701

mf-np = np* %MF[np:compl2]%
	Block := 1,
	OA := true,
	PRED := true,
	setscore -7.47193207824512

mf-np = np* %MF[np:compl2]%
	Block := 1,
	OPP := true,
	setscore -5.39249053656529

mf-np = np* %MF[np:compl2]%
	Block := 1,
	PRED := true,
	setscore -5.52602192918981

mf-np = np* %MF[np:mod-mod]%
	Block := 1,
	setscore -7.47193207824512

mf-np = np* %MF[np:mod]%
	Block := 1,
	setscore -4.63871873418891

mf-np = np* %MF[np:mod]%
	Block := 1,
	PRED := true,
	setscore -6.37331978957701

mf-np = np* %MF[np:oa-mod]%
	Block := 1,
	setscore -7.47193207824512

mf-np = np* %MF[np:oa]%
	Block := 1,
	C == acc,
	OA := true,
	OA_C := C,
	setscore -7.47193207824512

mf-np = np* %MF[np:oa]%
	Block := 1,
	C == dat,
	OA := true,
	OA_C := C,
	setscore -6.77878489768518

mf-np = np* %MF[np:oa]%
	Block := 1,
	C == dat,
	OA_C := C,
	setscore -7.47193207824512

mf-np = np* %MF[np:oa]%
	Block := 1,
	C == nom,
	OA := true,
	OA_C := C,
	setscore -6.77878489768518

mf-np = np* %MF[np:oa]%
	Block := 1,
	OA := true,
	setscore -5.68017260901707

mf-np = np* %MF[np:oa]%
	Block := 1,
	OA := true,
	OA_C := C,
	setscore -5.2747075009089

mf-np = np* %MF[np:oa]%
	Block := 1,
	OA := true,
	OA_C := C,
	PRED := true,
	setscore -6.77878489768518

mf-np = np* %MF[np:od-mod]%
	Block := 1,
	setscore -7.47193207824512

mf-np = np* %MF[np:od]%
	Block := 1,
	C == acc,
	OD := true,
	OD_C := C,
	setscore -7.47193207824512

mf-np = np* %MF[np:od]%
	Block := 1,
	C == dat,
	OD := true,
	OD_C := C,
	setscore -6.77878489768518

mf-np = np* %MF[np:od]%
	Block := 1,
	OD := true,
	setscore -6.77878489768518

mf-np = np* %MF[np:od]%
	Block := 1,
	OD := true,
	OD_C := C,
	setscore -7.47193207824512

mf-np = np* %MF[np:od]%
	Block := 1,
	OD := true,
	PRED := true,
	setscore -6.77878489768518

mf-np = np* %MF[np:on-mod]%
	Block := 1,
	setscore -4.13972756806992

mf-np = np* %MF[np:on-mod]%
	Block := 1,
	PRED := true,
	setscore -4.29387824789718

mf-np = np* %MF[np:os-mod]%
	Block := 1,
	setscore -4.33643786231597

mf-np = np* %MF[np:os-mod]%
	Block := 1,
	PRED := true,
	setscore -5.68017260901707

mf-np = np* %MF[np:pred-mod]%
	Block := 1,
	setscore -6.37331978957701

mf-np = np* %MF[np:pred-mod]%
	Block := 1,
	PRED := true,
	setscore -7.47193207824512

mf-np = np* %MF[np:pred]%
	Block := 1,
	PRED := true,
	setscore -4.07073469658297

mf-np = np* %MF[np:subject]%
	Block := 1,
	setscore -0.961673737721973

mf-np = np* %MF[np:subject]%
	Block := 1,
	OA := true,
	setscore -4.21383554022364

mf-np = np* %MF[np:subject]%
	Block := 1,
	OADJP := true,
	setscore -7.47193207824512

mf-np = np* %MF[np:subject]%
	Block := 1,
	OD := true,
	setscore -6.77878489768518

mf-np = np* %MF[np:subject]%
	Block := 1,
	OG := true,
	setscore -7.47193207824512

mf-np = np* %MF[np:subject]%
	Block := 1,
	OPP := true,
	setscore -6.08563771712523

mf-np = np* %MF[np:subject]%
	Block := 1,
	PRED := true,
	setscore -2.19893251968138

mf-np = np* %MF[np:v-mod]%
	Block := 1,
	setscore -3.48294803168085

mf-np = np* %MF[np:v-mod]%
	Block := 1,
	PRED := true,
	setscore -4.98702542845712

mf-np = np* %MF[np]%
	Block := 1,
	setscore -7.47193207824512

mf-np--advx = mf-np* advx %mf-np[+advx:mod-mod]%
	setscore -7.21890970761906

mf-np--advx = mf-np* advx %mf-np[+advx:mod]%
	setscore -1.29198368164865

mf-np--advx = mf-np* advx %mf-np[+advx:oadjp]%
	setscore -6.12029741895095

mf-np--advx = mf-np* advx %mf-np[+advx:oadvp]%
	setscore -5.83261534649917

mf-np--advx = mf-np* advx %mf-np[+advx:od]%
	setscore -7.21890970761906

mf-np--advx = mf-np* advx %mf-np[+advx:pred]%
	setscore -5.83261534649917

mf-np--advx = mf-np* advx %mf-np[+advx:v-mod]%
	setscore -2.43978621450753

mf-np--advx = mf-np--advx* advx %mf-np--advx[+advx:mod]%
	setscore -1.85293369259721

mf-np--advx = mf-np--advx* advx %mf-np--advx[+advx:oa-mod]%
	setscore -7.21890970761906

mf-np--advx = mf-np--advx* advx %mf-np--advx[+advx:oadjp]%
	setscore -5.13946816593922

mf-np--advx = mf-np--advx* advx %mf-np--advx[+advx:oadvp-mo]%
	setscore -7.21890970761906

mf-np--advx = mf-np--advx* advx %mf-np--advx[+advx:oadvp]%
	setscore -5.13946816593922

mf-np--advx = mf-np--advx* advx %mf-np--advx[+advx:pred]%
	setscore -5.27299955856375

mf-np--advx = mf-np--advx* advx %mf-np--advx[+advx:v-mod]%
	setscore -3.6925491830029

mf-np--advx = mf-np--ap* advx %mf-np--ap[+advx:mod]%
	setscore -4.51085950651685

mf-np--advx = mf-np--ap* advx %mf-np--ap[+advx:oadjp]%
	setscore -6.52576252705911

mf-np--advx = mf-np--ap* advx %mf-np--ap[+advx:oadvp]%
	setscore -6.12029741895095

mf-np--advx = mf-np--ap* advx %mf-np--ap[+advx:pred]%
	setscore -6.52576252705911

mf-np--advx = mf-np--ap* advx %mf-np--ap[+advx:v-mod]%
	setscore -5.60947179518496

mf-np--advx = mf-np--comma* advx %mf-np--comma[+advx:mod]%
	setscore -3.14137226371334

mf-np--advx = mf-np--comma* advx %mf-np--comma[+advx:oadjp]%
	setscore -7.21890970761906

mf-np--advx = mf-np--comma* advx %mf-np--comma[+advx:oadvp]%
	setscore -7.21890970761906

mf-np--advx = mf-np--comma* advx %mf-np--comma[+advx:v-mod]%
	setscore -5.427150238391

mf-np--advx = mf-np--conj* advx %mf-np--conj[+advx:mod]%
	setscore -7.21890970761906

mf-np--advx = mf-np--fx* advx %mf-np--fx[+advx:mod]%
	setscore -6.52576252705911

mf-np--advx = mf-np--np* advx %mf-np--np[+advx:mod]%
	setscore -2.32106990766815

mf-np--advx = mf-np--np* advx %mf-np--np[+advx:oadjp]%
	setscore -5.83261534649917

mf-np--advx = mf-np--np* advx %mf-np--np[+advx:oadvp]%
	setscore -6.52576252705911

mf-np--advx = mf-np--np* advx %mf-np--np[+advx:pred]%
	setscore -5.60947179518496

mf-np--advx = mf-np--np* advx %mf-np--np[+advx:v-mod]%
	setscore -3.92307284161473

mf-np--advx = mf-np--pp* advx %mf-np--pp[+advx:compl1]%
	setscore -7.21890970761906

mf-np--advx = mf-np--pp* advx %mf-np--pp[+advx:mod]%
	setscore -1.81173793615894

mf-np--advx = mf-np--pp* advx %mf-np--pp[+advx:oadjp]%
	setscore -5.27299955856375

mf-np--advx = mf-np--pp* advx %mf-np--pp[+advx:oadvp]%
	setscore -5.13946816593922

mf-np--advx = mf-np--pp* advx %mf-np--pp[+advx:pred]%
	setscore -5.02168513028284

mf-np--advx = mf-np--pp* advx %mf-np--pp[+advx:v-mod]%
	setscore -3.6925491830029

mf-np--advx = mf-np--simpx* advx %mf-np--simpx[+advx:mod]%
	setscore -7.21890970761906

mf-np--ap = mf-np* ap %mf-np[+ap:oadjp]%
	setscore -6.20050917404269

mf-np--ap = mf-np* ap %mf-np[+ap:pred-mod]%
	setscore -6.20050917404269

mf-np--ap = mf-np* ap %mf-np[+ap:pred]%
	setscore -3.42792045180291

mf-np--ap = mf-np* ap %mf-np[+ap:v-mod]%
	setscore -2.12297173013697

mf-np--ap = mf-np--advx* ap %mf-np--advx[+ap:pred]%
	setscore -2.08963530986938

mf-np--ap = mf-np--advx* ap %mf-np--advx[+ap:v-mod]%
	setscore -1.67872059699365

mf-np--ap = mf-np--ap* ap %mf-np--ap[+ap:pred]%
	setscore -4.8142148129228

mf-np--ap = mf-np--ap* ap %mf-np--ap[+ap:v-mod]%
	setscore -5.10189688537458

mf-np--ap = mf-np--comma* ap %mf-np--comma[+ap:mod-mod]%
	setscore -6.20050917404269

mf-np--ap = mf-np--comma* ap %mf-np--comma[+ap:pred]%
	setscore -4.00328459670647

mf-np--ap = mf-np--comma* ap %mf-np--comma[+ap:v-mod]%
	setscore -3.10946672068437

mf-np--ap = mf-np--np* ap %mf-np--np[+ap:on-mod]%
	setscore -6.20050917404269

mf-np--ap = mf-np--np* ap %mf-np--np[+ap:pred]%
	setscore -3.02245534369474

mf-np--ap = mf-np--np* ap %mf-np--np[+ap:v-mod]%
	setscore -2.3293081631348

mf-np--ap = mf-np--pp* ap %mf-np--pp[+ap:pred]%
	setscore -2.01085443201626

mf-np--ap = mf-np--pp* ap %mf-np--pp[+ap:v-mod]%
	setscore -2.15745790620814

mf-np--ap = mf-np--simpx* ap %mf-np--simpx[+ap:pred]%
	setscore -6.20050917404269

mf-np--comma = mf-np* comma %mf-np[+comma]%
	setscore -0.66542263254509

mf-np--comma = mf-np--advx* comma %mf-np--advx[+comma]%
	setscore -2.62708113856854

mf-np--comma = mf-np--ap* comma %mf-np--ap[+comma]%
	setscore -3.57154274740939

mf-np--comma = mf-np--np* comma %mf-np--np[+comma]%
	setscore -2.1852483862895

mf-np--comma = mf-np--pp* comma %mf-np--pp[+comma]%
	setscore -1.47440162863016

mf-np--comma = mf-np--simpx* comma %mf-np--simpx[+comma]%
	setscore -3.11955762366634

mf-np--conj = mf-np* conj %mf-np[+conj]%
	setscore -0.559615787935423

mf-np--conj = mf-np--np* conj %mf-np--np[+conj]%
	setscore -1.94591014905531

mf-np--conj = mf-np--pp* conj %mf-np--pp[+conj]%
	setscore -1.25276296849537

mf-np--fx = mf-np* fx %mf-np[+fx:mod]%
	setscore -1.84582669049833

mf-np--fx = mf-np* fx %mf-np[+fx:pred]%
	setscore -2.2512917986065

mf-np--fx = mf-np* fx %mf-np[+fx:v-mod]%
	setscore -2.94443897916644

mf-np--fx = mf-np--advx* fx %mf-np--advx[+fx:mod]%
	setscore -2.2512917986065

mf-np--fx = mf-np--advx* fx %mf-np--advx[+fx:v-mod]%
	setscore -2.94443897916644

mf-np--fx = mf-np--comma* fx %mf-np--comma[+fx:v-mod]%
	setscore -2.94443897916644

mf-np--fx = mf-np--np* fx %mf-np--np[+fx:oa]%
	setscore -2.94443897916644

mf-np--fx = mf-np--np* fx %mf-np--np[+fx:v-mod]%
	setscore -2.2512917986065

mf-np--fx = mf-np--pp* fx %mf-np--pp[+fx:compl1]%
	setscore -2.94443897916644

mf-np--fx = mf-np--pp* fx %mf-np--pp[+fx:mod]%
	setscore -2.94443897916644

mf-np--fx = mf-np--pp* fx %mf-np--pp[+fx:pred]%
	setscore -2.94443897916644

mf-np--np = mf-np* np %mf-np[+np:app]%
	C == np.C,
	Num == np.Num,
	P == np.P,
	setscore -6.89365635460264

mf-np--np = mf-np* np %mf-np[+np:compl1]%
	setscore -1.72887038067912

mf-np--np = mf-np* np %mf-np[+np:compl2]%
	setscore -3.36729582998647

mf-np--np = mf-np* np %mf-np[+np:mod-mod]%
	setscore -6.89365635460264

mf-np--np = mf-np* np %mf-np[+np:mod]%
	setscore -4.49576108180426

mf-np--np = mf-np* np %mf-np[+np:oa-mod]%
	setscore -6.20050917404269

mf-np--np = mf-np* np %mf-np[+np:oa]%
	OA := true,
	OA_C := np.C,
	np.C == acc,
	setscore -6.89365635460264

mf-np--np = mf-np* np %mf-np[+np:oa]%
	OA := true,
	np.C == acc,
	setscore -6.89365635460264

mf-np--np = mf-np* np %mf-np[+np:oa]%
	OA_C := np.C,
	setscore -5.28421844216853

mf-np--np = mf-np* np %mf-np[+np:oa]%
	OA_C := np.C,
	np.C == dat,
	setscore -6.89365635460264

mf-np--np = mf-np* np %mf-np[+np:oa]%
	OA_C := np.C,
	np.C == nom,
	setscore -6.89365635460264

mf-np--np = mf-np* np %mf-np[+np:oa]%
	setscore -6.20050917404269

mf-np--np = mf-np* np %mf-np[+np:od]%
	OD_C := np.C,
	np.C == acc,
	setscore -6.89365635460264

mf-np--np = mf-np* np %mf-np[+np:od]%
	setscore -6.89365635460264

mf-np--np = mf-np* np %mf-np[+np:og]%
	OG_C := np.C,
	np.C == dat,
	setscore -6.89365635460264

mf-np--np = mf-np* np %mf-np[+np:on-mod]%
	setscore -5.28421844216853

mf-np--np = mf-np* np %mf-np[+np:os-mod]%
	setscore -4.94774620554732

mf-np--np = mf-np* np %mf-np[+np:pred-mod]%
	setscore -6.89365635460264

mf-np--np = mf-np* np %mf-np[+np:pred]%
	setscore -3.89792408104864

mf-np--np = mf-np* np %mf-np[+np:subject]%
	setscore -2.08147199923022

mf-np--np = mf-np* np %mf-np[+np:v-mod]%
	setscore -3.45966915011749

mf-np--np = mf-np--advx* np %mf-np--advx[+np:compl1]%
	setscore -2.35036157233263

mf-np--np = mf-np--advx* np %mf-np--advx[+np:compl2]%
	setscore -4.40874970481464

mf-np--np = mf-np--advx* np %mf-np--advx[+np:mod]%
	setscore -5.50736199348274

mf-np--np = mf-np--advx* np %mf-np--advx[+np:oa-mod]%
	setscore -6.89365635460264

mf-np--np = mf-np--advx* np %mf-np--advx[+np:oa]%
	OA := true,
	OA_C := np.C,
	np.C == acc,
	setscore -6.89365635460264

mf-np--np = mf-np--advx* np %mf-np--advx[+np:oa]%
	OA_C := np.C,
	setscore -4.94774620554732

mf-np--np = mf-np--advx* np %mf-np--advx[+np:oa]%
	OA_C := np.C,
	np.C == gen,
	setscore -6.89365635460264

mf-np--np = mf-np--advx* np %mf-np--advx[+np:oa]%
	OA_C := np.C,
	np.C == nom,
	setscore -6.89365635460264

mf-np--np = mf-np--advx* np %mf-np--advx[+np:oa]%
	setscore -6.89365635460264

mf-np--np = mf-np--advx* np %mf-np--advx[+np:od]%
	OD := true,
	OD_C := np.C,
	np.C == dat,
	setscore -6.89365635460264

mf-np--np = mf-np--advx* np %mf-np--advx[+np:on-mod]%
	setscore -4.94774620554732

mf-np--np = mf-np--advx* np %mf-np--advx[+np:pred-mod]%
	setscore -6.89365635460264

mf-np--np = mf-np--advx* np %mf-np--advx[+np:pred]%
	setscore -3.52636052461616

mf-np--np = mf-np--advx* np %mf-np--advx[+np:subject]%
	setscore -2.85060508676809

mf-np--np = mf-np--advx* np %mf-np--advx[+np:v-mod]%
	setscore -4.00328459670647

mf-np--np = mf-np--ap* np %mf-np--ap[+np:compl1]%
	setscore -3.89792408104864

mf-np--np = mf-np--ap* np %mf-np--ap[+np:compl2]%
	setscore -5.79504406593453

mf-np--np = mf-np--ap* np %mf-np--ap[+np:oa]%
	OA_C := np.C,
	setscore -6.20050917404269

mf-np--np = mf-np--ap* np %mf-np--ap[+np:pred]%
	setscore -5.50736199348274

mf-np--np = mf-np--ap* np %mf-np--ap[+np:subject]%
	setscore -4.94774620554732

mf-np--np = mf-np--comma* np %mf-np--comma[+np:compl1]%
	setscore -3.56145184442743

mf-np--np = mf-np--comma* np %mf-np--comma[+np:compl2]%
	setscore -6.20050917404269

mf-np--np = mf-np--comma* np %mf-np--comma[+np:mod]%
	setscore -6.89365635460264

mf-np--np = mf-np--comma* np %mf-np--comma[+np:oa]%
	OA_C := np.C,
	np.C == nom,
	setscore -6.89365635460264

mf-np--np = mf-np--comma* np %mf-np--comma[+np:oa]%
	setscore -6.89365635460264

mf-np--np = mf-np--comma* np %mf-np--comma[+np:on-mod]%
	setscore -6.89365635460264

mf-np--np = mf-np--comma* np %mf-np--comma[+np:os-mod]%
	setscore -6.20050917404269

mf-np--np = mf-np--comma* np %mf-np--comma[+np:pred]%
	setscore -5.50736199348274

mf-np--np = mf-np--comma* np %mf-np--comma[+np:subject]%
	setscore -4.8142148129228

mf-np--np = mf-np--comma* np %mf-np--comma[+np:v-mod]%
	setscore -5.79504406593453

mf-np--np = mf-np--conj* np %mf-np--conj[+np:compl1]%
	setscore -6.20050917404269

mf-np--np = mf-np--fx* np %mf-np--fx[+np:compl1]%
	setscore -6.89365635460264

mf-np--np = mf-np--fx* np %mf-np--fx[+np:pred]%
	setscore -6.20050917404269

mf-np--np = mf-np--pp* np %mf-np--pp[+np:compl1]%
	setscore -2.258927366373

mf-np--np = mf-np--pp* np %mf-np--pp[+np:compl2]%
	setscore -4.94774620554732

mf-np--np = mf-np--pp* np %mf-np--pp[+np:mod]%
	setscore -5.50736199348274

mf-np--np = mf-np--pp* np %mf-np--pp[+np:oa-mod]%
	setscore -6.89365635460264

mf-np--np = mf-np--pp* np %mf-np--pp[+np:oa]%
	OA_C := np.C,
	setscore -4.94774620554732

mf-np--np = mf-np--pp* np %mf-np--pp[+np:og]%
	OG_C := np.C,
	np.C == dat,
	setscore -6.89365635460264

mf-np--np = mf-np--pp* np %mf-np--pp[+np:on-mod]%
	setscore -5.79504406593453

mf-np--np = mf-np--pp* np %mf-np--pp[+np:pred]%
	setscore -3.71560252425469

mf-np--np = mf-np--pp* np %mf-np--pp[+np:subject]%
	setscore -3.42792045180291

mf-np--np = mf-np--pp* np %mf-np--pp[+np:v-mod]%
	setscore -4.8142148129228

mf-np--np = mf-np--simpx* np %mf-np--simpx[+np:compl1]%
	setscore -6.89365635460264

mf-np--pp = mf-np* pp %mf-np[+pp:compl1]%
	setscore -4.38618464482255

mf-np--pp = mf-np* pp %mf-np[+pp:compl2]%
	setscore -4.48626810337953

mf-np--pp = mf-np* pp %mf-np[+pp:fopp]%
	setscore -3.37026407199955

mf-np--pp = mf-np* pp %mf-np[+pp:foppk]%
	setscore -7.43070708254597

mf-np--pp = mf-np* pp %mf-np[+pp:mod-mod]%
	setscore -5.63894761331791

mf-np--pp = mf-np* pp %mf-np[+pp:mod]%
	setscore -2.61852272717355

mf-np--pp = mf-np* pp %mf-np[+pp:oa-mod]%
	setscore -4.94580043275797

mf-np--pp = mf-np* pp %mf-np[+pp:on-mod]%
	setscore -5.63894761331791

mf-np--pp = mf-np* pp %mf-np[+pp:opp]%
	OPP_C := pp.C,
	OPP_Przyim := pp.Przyim,
	setscore -6.33209479387786

mf-np--pp = mf-np* pp %mf-np[+pp:oppk]%
	setscore -7.43070708254597

mf-np--pp = mf-np* pp %mf-np[+pp:pred-mod]%
	setscore -7.43070708254597

mf-np--pp = mf-np* pp %mf-np[+pp:pred]%
	setscore -5.82126917011187

mf-np--pp = mf-np* pp %mf-np[+pp:v-mod]%
	setscore -1.83972010203511

mf-np--pp = mf-np* pp %mf-np[+pp:v-modk]%
	setscore -7.43070708254597

mf-np--pp = mf-np--advx* pp %mf-np--advx[+pp:compl1]%
	setscore -3.99671987806082

mf-np--pp = mf-np--advx* pp %mf-np--advx[+pp:compl2]%
	setscore -3.64651744862771

mf-np--pp = mf-np--advx* pp %mf-np--advx[+pp:fopp]%
	setscore -3.19660057794871

mf-np--pp = mf-np--advx* pp %mf-np--advx[+pp:mod]%
	setscore -3.81978916990174

mf-np--pp = mf-np--advx* pp %mf-np--advx[+pp:oa-mod]%
	setscore -6.33209479387786

mf-np--pp = mf-np--advx* pp %mf-np--advx[+pp:on-mod]%
	setscore -7.43070708254597

mf-np--pp = mf-np--advx* pp %mf-np--advx[+pp:opp]%
	OPP_C := pp.C,
	OPP_Przyim := pp.Przyim,
	setscore -6.73755990198602

mf-np--pp = mf-np--advx* pp %mf-np--advx[+pp:pred-mod]%
	setscore -7.43070708254597

mf-np--pp = mf-np--advx* pp %mf-np--advx[+pp:pred]%
	setscore -4.94580043275797

mf-np--pp = mf-np--advx* pp %mf-np--advx[+pp:predk]%
	setscore -7.43070708254597

mf-np--pp = mf-np--advx* pp %mf-np--advx[+pp:v-mod]%
	setscore -2.31271327012921

mf-np--pp = mf-np--ap* pp %mf-np--ap[+pp:compl1]%
	setscore -5.0328118097476

mf-np--pp = mf-np--ap* pp %mf-np--ap[+pp:compl2]%
	setscore -4.29521286661682

mf-np--pp = mf-np--ap* pp %mf-np--ap[+pp:fopp]%
	setscore -4.02950970088381

mf-np--pp = mf-np--ap* pp %mf-np--ap[+pp:mod]%
	setscore -6.04441272142608

mf-np--pp = mf-np--ap* pp %mf-np--ap[+pp:oa-mod]%
	setscore -6.73755990198602

mf-np--pp = mf-np--ap* pp %mf-np--ap[+pp:oadjp]%
	setscore -7.43070708254597

mf-np--pp = mf-np--ap* pp %mf-np--ap[+pp:opp]%
	OPP_C := pp.C,
	OPP_Przyim := pp.Przyim,
	setscore -7.43070708254597

mf-np--pp = mf-np--ap* pp %mf-np--ap[+pp:oppk]%
	setscore -7.43070708254597

mf-np--pp = mf-np--ap* pp %mf-np--ap[+pp:pred]%
	setscore -6.33209479387786

mf-np--pp = mf-np--ap* pp %mf-np--ap[+pp:v-mod]%
	setscore -4.09850257237076

mf-np--pp = mf-np--comma* pp %mf-np--comma[+pp:compl1]%
	setscore -6.33209479387786

mf-np--pp = mf-np--comma* pp %mf-np--comma[+pp:compl2]%
	setscore -5.63894761331791

mf-np--pp = mf-np--comma* pp %mf-np--comma[+pp:fopp]%
	setscore -4.79164975293071

mf-np--pp = mf-np--comma* pp %mf-np--comma[+pp:mod]%
	setscore -5.12812198955192

mf-np--pp = mf-np--comma* pp %mf-np--comma[+pp:pred]%
	setscore -6.73755990198602

mf-np--pp = mf-np--comma* pp %mf-np--comma[+pp:v-mod]%
	setscore -3.55950607163808

mf-np--pp = mf-np--conj* pp %mf-np--conj[+pp:fopp]%
	setscore -7.43070708254597

mf-np--pp = mf-np--conj* pp %mf-np--conj[+pp:v-mod]%
	setscore -7.43070708254597

mf-np--pp = mf-np--fx* pp %mf-np--fx[+pp:compl1]%
	setscore -7.43070708254597

mf-np--pp = mf-np--fx* pp %mf-np--fx[+pp:fopp]%
	setscore -7.43070708254597

mf-np--pp = mf-np--np* pp %mf-np--np[+pp:compl1]%
	setscore -4.65811836030619

mf-np--pp = mf-np--np* pp %mf-np--np[+pp:compl2]%
	setscore -3.71713501584166

mf-np--pp = mf-np--np* pp %mf-np--np[+pp:fopp]%
	setscore -3.53888678443534

mf-np--pp = mf-np--np* pp %mf-np--np[+pp:mod-mod]%
	setscore -7.43070708254597

mf-np--pp = mf-np--np* pp %mf-np--np[+pp:mod]%
	setscore -3.40535539181082

mf-np--pp = mf-np--np* pp %mf-np--np[+pp:oa-mod]%
	setscore -7.43070708254597

mf-np--pp = mf-np--np* pp %mf-np--np[+pp:opp]%
	OPP_C := pp.C,
	OPP_Przyim := pp.Przyim,
	setscore -5.12812198955192

mf-np--pp = mf-np--np* pp %mf-np--np[+pp:pred]%
	setscore -5.63894761331791

mf-np--pp = mf-np--np* pp %mf-np--np[+pp:v-mod]%
	setscore -2.61852272717355

mf-np--pp = mf-np--pp* pp %mf-np--pp[+pp:compl1]%
	setscore -4.33966462918765

mf-np--pp = mf-np--pp* pp %mf-np--pp[+pp:compl2]%
	setscore -4.17261054452449

mf-np--pp = mf-np--pp* pp %mf-np--pp[+pp:fopp]%
	setscore -3.76714543641632

mf-np--pp = mf-np--pp* pp %mf-np--pp[+pp:foppk]%
	setscore -7.43070708254597

mf-np--pp = mf-np--pp* pp %mf-np--pp[+pp:mod-mod]%
	setscore -6.73755990198602

mf-np--pp = mf-np--pp* pp %mf-np--pp[+pp:mod]%
	setscore -3.79312092281958

mf-np--pp = mf-np--pp* pp %mf-np--pp[+pp:oa-mod]%
	setscore -5.82126917011187

mf-np--pp = mf-np--pp* pp %mf-np--pp[+pp:on-mod]%
	setscore -7.43070708254597

mf-np--pp = mf-np--pp* pp %mf-np--pp[+pp:opp]%
	OPP_C := pp.C,
	OPP_Przyim := pp.Przyim,
	setscore -7.43070708254597

mf-np--pp = mf-np--pp* pp %mf-np--pp[+pp:pred-mod]%
	setscore -7.43070708254597

mf-np--pp = mf-np--pp* pp %mf-np--pp[+pp:pred]%
	setscore -4.72265688144376

mf-np--pp = mf-np--pp* pp %mf-np--pp[+pp:v-mod]%
	setscore -2.79597809431633

mf-np--pp = mf-np--pp* pp %mf-np--pp[+pp:v-modk]%
	setscore -7.43070708254597

mf-np--pp = mf-np--simpx* pp %mf-np--simpx[+pp:compl2]%
	setscore -7.43070708254597

mf-np--pp = mf-np--simpx* pp %mf-np--simpx[+pp:v-mod]%
	setscore -7.43070708254597

mf-np--simpx = mf-np* simpx %mf-np[+simpx:mod]%
	setscore -2.30258509299405

mf-np--simpx = mf-np* simpx %mf-np[+simpx:os]%
	setscore -2.30258509299405

mf-np--simpx = mf-np--advx* simpx %mf-np--advx[+simpx:mod]%
	setscore -2.70805020110221

mf-np--simpx = mf-np--comma* simpx %mf-np--comma[+simpx:mod]%
	setscore -0.836248024200619

mf-np--simpx = mf-np--comma* simpx %mf-np--comma[+simpx:os]%
	setscore -3.40119738166216

mf-np--simpx = mf-np--comma* simpx %mf-np--comma[+simpx:v-mod]%
	setscore -3.40119738166216

mf-np--simpx = mf-np--np* simpx %mf-np--np[+simpx:mod]%
	setscore -3.40119738166216

mf-np--simpx = mf-np--pp* simpx %mf-np--pp[+simpx:os]%
	setscore -2.30258509299405

mf-pp = pp* %MF[pp:compl1]%
	Block := 1,
	setscore -3.36901848329792

mf-pp = pp* %MF[pp:compl1]%
	Block := 1,
	PRED := true,
	setscore -4.57299128762386

mf-pp = pp* %MF[pp:compl2]%
	Block := 1,
	setscore -4.57299128762386

mf-pp = pp* %MF[pp:compl2]%
	Block := 1,
	OA := true,
	setscore -5.2661384681838

mf-pp = pp* %MF[pp:compl2]%
	Block := 1,
	PRED := true,
	setscore -6.36475075685191

mf-pp = pp* %MF[pp:fopp-mod]%
	Block := 1,
	setscore -6.36475075685191

mf-pp = pp* %MF[pp:fopp]%
	Block := 1,
	setscore -2.72716459712553

mf-pp = pp* %MF[pp:fopp]%
	Block := 1,
	OA := true,
	setscore -5.2661384681838

mf-pp = pp* %MF[pp:fopp]%
	Block := 1,
	PRED := true,
	setscore -4.06216566385787

mf-pp = pp* %MF[pp:mod-mod]%
	Block := 1,
	setscore -5.2661384681838

mf-pp = pp* %MF[pp:mod]%
	Block := 1,
	setscore -1.71079040669439

mf-pp = pp* %MF[pp:mod]%
	Block := 1,
	OA := true,
	setscore -4.57299128762386

mf-pp = pp* %MF[pp:mod]%
	Block := 1,
	OPP := true,
	setscore -5.67160357629197

mf-pp = pp* %MF[pp:mod]%
	Block := 1,
	PRED := true,
	setscore -2.51460315514185

mf-pp = pp* %MF[pp:oa-mod]%
	Block := 1,
	setscore -4.4188406077966

mf-pp = pp* %MF[pp:oa-mod]%
	Block := 1,
	OA := true,
	setscore -6.36475075685191

mf-pp = pp* %MF[pp:oa]%
	Block := 1,
	setscore -6.36475075685191

mf-pp = pp* %MF[pp:oadjp]%
	Block := 1,
	setscore -6.36475075685191

mf-pp = pp* %MF[pp:on-mod]%
	Block := 1,
	setscore -3.79980139939037

mf-pp = pp* %MF[pp:opp]%
	Block := 1,
	OPP := true,
	setscore -6.36475075685191

mf-pp = pp* %MF[pp:opp]%
	Block := 1,
	OPP := true,
	OPP_C := C,
	OPP_Przyim := Przyim,
	setscore -6.36475075685191

mf-pp = pp* %MF[pp:pred-mod]%
	Block := 1,
	PRED := true,
	setscore -5.67160357629197

mf-pp = pp* %MF[pp:pred]%
	Block := 1,
	PRED := true,
	setscore -4.97845639573202

mf-pp = pp* %MF[pp:subject]%
	Block := 1,
	setscore -6.36475075685191

mf-pp = pp* %MF[pp:v-mod]%
	Block := 1,
	setscore -0.97112321049955

mf-pp = pp* %MF[pp:v-mod]%
	Block := 1,
	OA := true,
	setscore -4.16752617951569

mf-pp = pp* %MF[pp:v-mod]%
	Block := 1,
	OD := true,
	setscore -6.36475075685191

mf-pp = pp* %MF[pp:v-mod]%
	Block := 1,
	PRED := true,
	setscore -2.1452430516758

mf-pp--advx = mf-pp* advx %mf-pp[+advx:mod]%
	setscore -0.912510009034249

mf-pp--advx = mf-pp* advx %mf-pp[+advx:oadjp]%
	setscore -4.88280192258637

mf-pp--advx = mf-pp* advx %mf-pp[+advx:oadvp]%
	setscore -5.98141421125448

mf-pp--advx = mf-pp* advx %mf-pp[+advx:opp-mod]%
	setscore -5.98141421125448

mf-pp--advx = mf-pp* advx %mf-pp[+advx:pred]%
	setscore -5.98141421125448

mf-pp--advx = mf-pp* advx %mf-pp[+advx:v-mod]%
	setscore -2.76253838638628

mf-pp--advx = mf-pp--advx* advx %mf-pp--advx[+advx:mod]%
	setscore -1.82253112789481

mf-pp--advx = mf-pp--advx* advx %mf-pp--advx[+advx:oadjp]%
	setscore -5.28826703069454

mf-pp--advx = mf-pp--advx* advx %mf-pp--advx[+advx:oadvp]%
	setscore -4.37197629882038

mf-pp--advx = mf-pp--advx* advx %mf-pp--advx[+advx:pred]%
	setscore -4.88280192258637

mf-pp--advx = mf-pp--advx* advx %mf-pp--advx[+advx:v-mod]%
	setscore -4.03550406219917

mf-pp--advx = mf-pp--ap* advx %mf-pp--ap[+advx:mod]%
	setscore -5.28826703069454

mf-pp--advx = mf-pp--comma* advx %mf-pp--comma[+advx:mod]%
	setscore -3.34235688163922

mf-pp--advx = mf-pp--comma* advx %mf-pp--comma[+advx:oadjp]%
	setscore -5.98141421125448

mf-pp--advx = mf-pp--comma* advx %mf-pp--comma[+advx:v-mod]%
	setscore -5.28826703069454

mf-pp--advx = mf-pp--np* advx %mf-pp--np[+advx:mod]%
	setscore -2.15277281476539

mf-pp--advx = mf-pp--np* advx %mf-pp--np[+advx:oadjp]%
	setscore -5.28826703069454

mf-pp--advx = mf-pp--np* advx %mf-pp--np[+advx:pred]%
	setscore -4.59511985013459

mf-pp--advx = mf-pp--np* advx %mf-pp--np[+advx:v-mod]%
	setscore -3.67882911826043

mf-pp--advx = mf-pp--pp* advx %mf-pp--pp[+advx:mod]%
	setscore -2.68557734525015

mf-pp--advx = mf-pp--pp* advx %mf-pp--pp[+advx:oadvp]%
	setscore -5.98141421125448

mf-pp--advx = mf-pp--pp* advx %mf-pp--pp[+advx:pred]%
	setscore -5.28826703069454

mf-pp--advx = mf-pp--pp* advx %mf-pp--pp[+advx:v-mod]%
	setscore -4.59511985013459

mf-pp--ap = mf-pp* ap %mf-pp[+ap:pred]%
	setscore -3.14988295338125

mf-pp--ap = mf-pp* ap %mf-pp[+ap:v-mod]%
	setscore -1.85059996925099

mf-pp--ap = mf-pp--advx* ap %mf-pp--advx[+ap:pred]%
	setscore -2.16905370036952

mf-pp--ap = mf-pp--advx* ap %mf-pp--advx[+ap:v-mod]%
	setscore -1.85059996925099

mf-pp--ap = mf-pp--ap* ap %mf-pp--ap[+ap:pred]%
	setscore -4.9416424226093

mf-pp--ap = mf-pp--ap* ap %mf-pp--ap[+ap:v-mod]%
	setscore -4.9416424226093

mf-pp--ap = mf-pp--comma* ap %mf-pp--comma[+ap:on-mod]%
	setscore -4.9416424226093

mf-pp--ap = mf-pp--comma* ap %mf-pp--comma[+ap:pred]%
	setscore -4.9416424226093

mf-pp--ap = mf-pp--comma* ap %mf-pp--comma[+ap:v-mod]%
	setscore -3.84303013394119

mf-pp--ap = mf-pp--np* ap %mf-pp--np[+ap:pred]%
	setscore -2.23359222150709

mf-pp--ap = mf-pp--np* ap %mf-pp--np[+ap:v-mod]%
	setscore -2.30258509299405

mf-pp--ap = mf-pp--pp* ap %mf-pp--pp[+ap:pred]%
	setscore -1.89711998488588

mf-pp--ap = mf-pp--pp* ap %mf-pp--pp[+ap:v-mod]%
	setscore -2.63905732961526

mf-pp--comma = mf-pp* comma %mf-pp[+comma]%
	setscore -0.281851152140988

mf-pp--comma = mf-pp--ap* comma %mf-pp--ap[+comma]%
	setscore -4.04305126783455

mf-pp--comma = mf-pp--np* comma %mf-pp--np[+comma]%
	setscore -1.96360972615471

mf-pp--comma = mf-pp--pp* comma %mf-pp--pp[+comma]%
	setscore -2.43361335540045

mf-pp--fx = mf-pp* fx %mf-pp[+fx:pred]%
	setscore -1.6094379124341

mf-pp--fx = mf-pp--np* fx %mf-pp--np[+fx:compl2]%
	setscore -1.6094379124341

mf-pp--np = mf-pp* np %mf-pp[+np:compl1]%
	setscore -2.25450895955827

mf-pp--np = mf-pp* np %mf-pp[+np:compl2]%
	setscore -4.35456978844084

mf-pp--np = mf-pp* np %mf-pp[+np:mod]%
	setscore -4.5368913452348

mf-pp--np = mf-pp* np %mf-pp[+np:oa]%
	OA := true,
	OA_C := np.C,
	np.C == acc,
	setscore -6.1463292576689

mf-pp--np = mf-pp* np %mf-pp[+np:oa]%
	OA_C := np.C,
	setscore -4.35456978844084

mf-pp--np = mf-pp* np %mf-pp[+np:oa]%
	OA_C := np.C,
	np.C == nom,
	setscore -6.1463292576689

mf-pp--np = mf-pp* np %mf-pp[+np:oa]%
	setscore -5.04771696900079

mf-pp--np = mf-pp* np %mf-pp[+np:od]%
	OD_C := np.C,
	np.C == acc,
	setscore -6.1463292576689

mf-pp--np = mf-pp* np %mf-pp[+np:on-mod]%
	setscore -4.35456978844084

mf-pp--np = mf-pp* np %mf-pp[+np:os-mod]%
	setscore -6.1463292576689

mf-pp--np = mf-pp* np %mf-pp[+np:pred]%
	setscore -3.74843398487053

mf-pp--np = mf-pp* np %mf-pp[+np:subject]%
	setscore -1.76430262299502

mf-pp--np = mf-pp* np %mf-pp[+np:v-mod]%
	setscore -3.58137990020736

mf-pp--np = mf-pp--advx* np %mf-pp--advx[+np:compl1]%
	setscore -2.40865963938553

mf-pp--np = mf-pp--advx* np %mf-pp--advx[+np:compl2]%
	setscore -5.04771696900079

mf-pp--np = mf-pp--advx* np %mf-pp--advx[+np:oa]%
	OA_C := np.C,
	setscore -3.94910468033268

mf-pp--np = mf-pp--advx* np %mf-pp--advx[+np:oa]%
	OA_C := np.C,
	np.C == nom,
	setscore -6.1463292576689

mf-pp--np = mf-pp--advx* np %mf-pp--advx[+np:oa]%
	setscore -6.1463292576689

mf-pp--np = mf-pp--advx* np %mf-pp--advx[+np:pred]%
	setscore -3.6614226078809

mf-pp--np = mf-pp--advx* np %mf-pp--advx[+np:subject]%
	setscore -2.21450362494457

mf-pp--np = mf-pp--advx* np %mf-pp--advx[+np:v-mod]%
	setscore -4.20041910861358

mf-pp--np = mf-pp--ap* np %mf-pp--ap[+np:compl1]%
	setscore -3.94910468033268

mf-pp--np = mf-pp--ap* np %mf-pp--ap[+np:pred]%
	setscore -5.04771696900079

mf-pp--np = mf-pp--ap* np %mf-pp--ap[+np:subject]%
	setscore -4.20041910861358

mf-pp--np = mf-pp--comma* np %mf-pp--comma[+np:compl1]%
	setscore -3.84374416467485

mf-pp--np = mf-pp--comma* np %mf-pp--comma[+np:pred]%
	setscore -5.45318207710895

mf-pp--np = mf-pp--comma* np %mf-pp--comma[+np:subject]%
	setscore -3.74843398487053

mf-pp--np = mf-pp--comma* np %mf-pp--comma[+np:v-mod]%
	setscore -6.1463292576689

mf-pp--np = mf-pp--np* np %mf-pp--np[+np:compl1]%
	setscore -2.53541134502467

mf-pp--np = mf-pp--np* np %mf-pp--np[+np:compl2]%
	setscore -6.1463292576689

mf-pp--np = mf-pp--np* np %mf-pp--np[+np:mod]%
	setscore -6.1463292576689

mf-pp--np = mf-pp--np* np %mf-pp--np[+np:pred]%
	setscore -3.94910468033268

mf-pp--np = mf-pp--np* np %mf-pp--np[+np:subject]%
	setscore -3.74843398487053

mf-pp--np = mf-pp--np* np %mf-pp--np[+np:v-mod]%
	setscore -5.45318207710895

mf-pp--np = mf-pp--pp* np %mf-pp--pp[+np:compl1]%
	setscore -3.58137990020736

mf-pp--np = mf-pp--pp* np %mf-pp--pp[+np:compl2]%
	setscore -6.1463292576689

mf-pp--np = mf-pp--pp* np %mf-pp--pp[+np:mod]%
	setscore -6.1463292576689

mf-pp--np = mf-pp--pp* np %mf-pp--pp[+np:oa]%
	OA_C := np.C,
	setscore -6.1463292576689

mf-pp--np = mf-pp--pp* np %mf-pp--pp[+np:on-mod]%
	setscore -6.1463292576689

mf-pp--np = mf-pp--pp* np %mf-pp--pp[+np:pred]%
	setscore -4.76003489654901

mf-pp--np = mf-pp--pp* np %mf-pp--pp[+np:subject]%
	setscore -3.50727192805364

mf-pp--pp = mf-pp* pp %mf-pp[+pp:compl1]%
	setscore -3.88156379794344

mf-pp--pp = mf-pp* pp %mf-pp[+pp:compl2]%
	setscore -5.67332326717149

mf-pp--pp = mf-pp* pp %mf-pp[+pp:fopp]%
	setscore -3.18841661738349

mf-pp--pp = mf-pp* pp %mf-pp[+pp:mod]%
	setscore -2.62880082944807

mf-pp--pp = mf-pp* pp %mf-pp[+pp:oa-mod]%
	setscore -4.57471097850338

mf-pp--pp = mf-pp* pp %mf-pp[+pp:on-mod]%
	setscore -4.57471097850338

mf-pp--pp = mf-pp* pp %mf-pp[+pp:opp]%
	OPP_C := pp.C,
	OPP_Przyim := pp.Przyim,
	setscore -5.67332326717149

mf-pp--pp = mf-pp* pp %mf-pp[+pp:os]%
	setscore -5.67332326717149

mf-pp--pp = mf-pp* pp %mf-pp[+pp:pred]%
	setscore -4.98017608661155

mf-pp--pp = mf-pp* pp %mf-pp[+pp:v-mod]%
	setscore -1.86666077740117

mf-pp--pp = mf-pp--advx* pp %mf-pp--advx[+pp:compl1]%
	setscore -3.59388172549166

mf-pp--pp = mf-pp--advx* pp %mf-pp--advx[+pp:compl2]%
	setscore -4.57471097850338

mf-pp--pp = mf-pp--advx* pp %mf-pp--advx[+pp:fopp]%
	setscore -2.78295150927533

mf-pp--pp = mf-pp--advx* pp %mf-pp--advx[+pp:mod]%
	setscore -4.06388535473739

mf-pp--pp = mf-pp--advx* pp %mf-pp--advx[+pp:pred]%
	setscore -4.06388535473739

mf-pp--pp = mf-pp--advx* pp %mf-pp--advx[+pp:v-mod]%
	setscore -2.41522672915001

mf-pp--pp = mf-pp--ap* pp %mf-pp--ap[+pp:compl1]%
	setscore -3.88156379794344

mf-pp--pp = mf-pp--ap* pp %mf-pp--ap[+pp:compl2]%
	setscore -4.98017608661155

mf-pp--pp = mf-pp--ap* pp %mf-pp--ap[+pp:fopp]%
	setscore -4.2870289060516

mf-pp--pp = mf-pp--ap* pp %mf-pp--ap[+pp:v-mod]%
	setscore -3.88156379794344

mf-pp--pp = mf-pp--comma* pp %mf-pp--comma[+pp:compl1]%
	setscore -4.57471097850338

mf-pp--pp = mf-pp--comma* pp %mf-pp--comma[+pp:fopp]%
	setscore -5.67332326717149

mf-pp--pp = mf-pp--comma* pp %mf-pp--comma[+pp:mod]%
	setscore -4.98017608661155

mf-pp--pp = mf-pp--comma* pp %mf-pp--comma[+pp:v-mod]%
	setscore -4.06388535473739

mf-pp--pp = mf-pp--np* pp %mf-pp--np[+pp:compl1]%
	setscore -3.72741311811618

mf-pp--pp = mf-pp--np* pp %mf-pp--np[+pp:compl2]%
	setscore -2.96527306606928

mf-pp--pp = mf-pp--np* pp %mf-pp--np[+pp:fopp]%
	setscore -2.84010992311528

mf-pp--pp = mf-pp--np* pp %mf-pp--np[+pp:mod-mod]%
	setscore -5.67332326717149

mf-pp--pp = mf-pp--np* pp %mf-pp--np[+pp:mod]%
	setscore -3.03426593755623

mf-pp--pp = mf-pp--np* pp %mf-pp--np[+pp:opp]%
	OPP_C := pp.C,
	OPP_Przyim := pp.Przyim,
	setscore -5.67332326717149

mf-pp--pp = mf-pp--np* pp %mf-pp--np[+pp:pred]%
	setscore -4.06388535473739

mf-pp--pp = mf-pp--np* pp %mf-pp--np[+pp:v-mod]%
	setscore -2.23933606268635

mf-pp--pp = mf-pp--np* pp %mf-pp--np[+pp:v-modk]%
	setscore -4.98017608661155

mf-pp--simpx = mf-pp* simpx %mf-pp[+simpx:os]%
	setscore -0.693147180559945

mf-simpx = simpx* %MF[simpx:mod]%
	Block := 1,
	setscore -0.980829253011726

mf-simpx = simpx* %MF[simpx:mod]%
	Block := 1,
	PRED := true,
	setscore -1.56861591791385

mf-simpx = simpx* %MF[simpx:os]%
	Block := 1,
	setscore -3.17805383034795

mf-simpx = simpx* %MF[simpx:os]%
	Block := 1,
	PRED := true,
	setscore -2.07944154167984

mf-simpx = simpx* %MF[simpx:pred]%
	Block := 1,
	PRED := true,
	setscore -3.17805383034795

mf-simpx = simpx* %MF[simpx:subject]%
	Block := 1,
	setscore -2.07944154167984

mf-simpx = simpx* %MF[simpx:subject]%
	Block := 1,
	PRED := true,
	setscore -3.17805383034795

mf-simpx = simpx* %MF[simpx:v-mod]%
	Block := 1,
	setscore -3.17805383034795

mf-simpx--advx = mf-simpx* advx %mf-simpx[+advx:pred]%
	setscore -1.94591014905531

mf-simpx--advx = mf-simpx--advx* advx %mf-simpx--advx[+advx:mod]%
	setscore -1.25276296849537

mf-simpx--advx = mf-simpx--comma* advx %mf-simpx--comma[+advx:mod]%
	setscore -0.847297860387204

mf-simpx--advx = mf-simpx--np* advx %mf-simpx--np[+advx:mod]%
	setscore -1.94591014905531

mf-simpx--ap = mf-simpx* ap %mf-simpx[+ap:pred]%
	setscore -1.38629436111989

mf-simpx--ap = mf-simpx--comma* ap %mf-simpx--comma[+ap:pred]%
	setscore -2.07944154167984

mf-simpx--ap = mf-simpx--comma* ap %mf-simpx--comma[+ap:v-mod]%
	setscore -2.07944154167984

mf-simpx--ap = mf-simpx--np* ap %mf-simpx--np[+ap:pred]%
	setscore -2.07944154167984

mf-simpx--ap = mf-simpx--np* ap %mf-simpx--np[+ap:v-mod]%
	setscore -2.07944154167984

mf-simpx--ap = mf-simpx--pp* ap %mf-simpx--pp[+ap:pred]%
	setscore -1.38629436111989

mf-simpx--comma = mf-simpx* comma %mf-simpx[+comma]%
	setscore 0

mf-simpx--np = mf-simpx* np %mf-simpx[+np:compl1]%
	setscore -1.94591014905531

mf-simpx--np = mf-simpx* np %mf-simpx[+np:subject]%
	setscore -2.63905732961526

mf-simpx--np = mf-simpx--advx* np %mf-simpx--advx[+np:pred]%
	setscore -2.63905732961526

mf-simpx--np = mf-simpx--advx* np %mf-simpx--advx[+np:subject]%
	setscore -2.63905732961526

mf-simpx--np = mf-simpx--ap* np %mf-simpx--ap[+np:compl2]%
	setscore -2.63905732961526

mf-simpx--np = mf-simpx--comma* np %mf-simpx--comma[+np:compl1]%
	setscore -1.94591014905531

mf-simpx--np = mf-simpx--comma* np %mf-simpx--comma[+np:compl2]%
	setscore -2.63905732961526

mf-simpx--np = mf-simpx--comma* np %mf-simpx--comma[+np:pred]%
	setscore -2.63905732961526

mf-simpx--np = mf-simpx--comma* np %mf-simpx--comma[+np:subject]%
	setscore -2.63905732961526

mf-simpx--np = mf-simpx--np* np %mf-simpx--np[+np:compl1]%
	setscore -1.54044504094715

mf-simpx--pp = mf-simpx* pp %mf-simpx[+pp:mod]%
	setscore -1.94591014905531

mf-simpx--pp = mf-simpx* pp %mf-simpx[+pp:v-mod]%
	setscore -1.94591014905531

mf-simpx--pp = mf-simpx--ap* pp %mf-simpx--ap[+pp:fopp]%
	setscore -1.94591014905531

mf-simpx--pp = mf-simpx--comma* pp %mf-simpx--comma[+pp:fopp]%
	setscore -1.94591014905531

mf-simpx--pp = mf-simpx--comma* pp %mf-simpx--comma[+pp:mod]%
	setscore -1.94591014905531

mf-simpx--pp = mf-simpx--comma* pp %mf-simpx--comma[+pp:v-mod]%
	setscore -1.94591014905531

mf-simpx--pp = mf-simpx--np* pp %mf-simpx--np[+pp:compl2]%
	setscore -1.94591014905531

mfe = mfe-ap*
	{ Block := false },
	setscore 0

mfe-ap = ap* %MFE[ap:pred]%
	Block := 1,
	setscore 0

nf = nf-advx*
	setscore -3.36297616884196

nf = nf-advx--advx*
	setscore -5.44241771052179

nf = nf-advx--ap*
	setscore -4.74927052996185

nf = nf-advx--fx*
	setscore -5.44241771052179

nf = nf-advx--np*
	setscore -2.87746835306026

nf = nf-advx--pp*
	setscore -2.87746835306026

nf = nf-advx--simpx*
	setscore -4.0561233494019

nf = nf-ap*
	setscore -3.24519313318557

nf = nf-ap--np*
	setscore -4.74927052996185

nf = nf-ap--pp*
	setscore -4.74927052996185

nf = nf-ap--simpx*
	setscore -4.74927052996185

nf = nf-fkonj*
	setscore -5.44241771052179

nf = nf-fx*
	setscore -4.0561233494019

nf = nf-np*
	setscore -2.39789527279837

nf = nf-np--advx*
	setscore -4.34380542185368

nf = nf-np--np*
	setscore -5.44241771052179

nf = nf-np--pp*
	setscore -3.83297979808769

nf = nf-np--r-simpx*
	setscore -5.44241771052179

nf = nf-np--simpx*
	setscore -3.83297979808769

nf = nf-p-simpx*
	setscore -5.44241771052179

nf = nf-pp*
	setscore -2.35137525716348

nf = nf-pp--advx*
	setscore -4.0561233494019

nf = nf-pp--ap*
	setscore -4.0561233494019

nf = nf-pp--np*
	setscore -3.83297979808769

nf = nf-pp--pp*
	setscore -3.04452243772342

nf = nf-pp--r-simpx*
	setscore -5.44241771052179

nf = nf-pp--simpx*
	setscore -3.49650756146648

nf = nf-r-simpx*
	setscore -2.95751106073379

nf = nf-r-simpx--advx*
	setscore -5.44241771052179

nf = nf-r-simpx--np*
	setscore -3.13983261752775

nf = nf-r-simpx--pp*
	setscore -4.74927052996185

nf = nf-r-simpx--r-simpx*
	setscore -4.74927052996185

nf = nf-r-simpx--simpx*
	setscore -3.36297616884196

nf = nf-simpx*
	setscore -2.4466854369678

nf = nf-simpx--advx*
	setscore -4.74927052996185

nf = nf-simpx--ap*
	setscore -4.74927052996185

nf = nf-simpx--np*
	setscore -3.65065824129374

nf = nf-simpx--pp*
	setscore -4.34380542185368

nf = nf-simpx--r-simpx*
	setscore -5.44241771052179

nf = nf-simpx--simpx*
	setscore -4.0561233494019

nf = nf-vp*
	setscore -5.44241771052179

nf-advx = advx* %NF[advx:mod-mod]%
	Block := 1,
	setscore -3.04452243772342

nf-advx = advx* %NF[advx:mod]%
	Block := 1,
	setscore -0.303682413798222

nf-advx = advx* %NF[advx:mod]%
	Block := 1,
	PRED := true,
	setscore -3.73766961828337

nf-advx = advx* %NF[advx:oa-mod]%
	Block := 1,
	setscore -3.73766961828337

nf-advx = advx* %NF[advx:oa-modk]%
	Block := 1,
	setscore -3.73766961828337

nf-advx = advx* %NF[advx:oadjp]%
	Block := 1,
	setscore -3.73766961828337

nf-advx = advx* %NF[advx:on-mod]%
	Block := 1,
	setscore -3.73766961828337

nf-advx = advx* %NF[advx:pred-mod]%
	Block := 1,
	setscore -3.73766961828337

nf-advx = advx* %NF[advx:v-mod]%
	Block := 1,
	setscore -2.63905732961526

nf-advx--advx = nf-advx* advx %nf-advx[+advx:mod]%
	setscore 0

nf-advx--ap = nf-advx* ap %nf-advx[+ap:v-mod]%
	setscore -0.693147180559945

nf-advx--ap = nf-advx--advx* ap %nf-advx--advx[+ap:v-mod]%
	setscore -0.693147180559945

nf-advx--comma = nf-advx* comma %nf-advx[+comma]%
	setscore -0.405465108108164

nf-advx--comma = nf-advx--advx* comma %nf-advx--advx[+comma]%
	setscore -1.09861228866811

nf-advx--fx = nf-advx* fx %nf-advx[+fx:subject]%
	setscore 0

nf-advx--np = nf-advx* np %nf-advx[+np:mod]%
	setscore -2.56494935746154

nf-advx--np = nf-advx* np %nf-advx[+np:oa-mod]%
	setscore -2.56494935746154

nf-advx--np = nf-advx* np %nf-advx[+np:oak]%
	setscore -2.56494935746154

nf-advx--np = nf-advx* np %nf-advx[+np:on-mod]%
	setscore -1.87180217690159

nf-advx--np = nf-advx* np %nf-advx[+np:onk]%
	setscore -2.56494935746154

nf-advx--np = nf-advx* np %nf-advx[+np:pred-mod]%
	setscore -2.56494935746154

nf-advx--np = nf-advx* np %nf-advx[+np:pred]%
	setscore -2.56494935746154

nf-advx--np = nf-advx* np %nf-advx[+np:v-mod]%
	setscore -2.56494935746154

nf-advx--np = nf-advx--advx* np %nf-advx--advx[+np:oak]%
	setscore -2.56494935746154

nf-advx--np = nf-advx--advx* np %nf-advx--advx[+np:onk]%
	setscore -2.56494935746154

nf-advx--np = nf-advx--advx* np %nf-advx--advx[+np:subject]%
	setscore -2.56494935746154

nf-advx--np = nf-advx--pp* np %nf-advx--pp[+np:oa-mod]%
	setscore -2.56494935746154

nf-advx--pp = nf-advx* pp %nf-advx[+pp:compl2]%
	setscore -2.63905732961526

nf-advx--pp = nf-advx* pp %nf-advx[+pp:fopp]%
	setscore -2.63905732961526

nf-advx--pp = nf-advx* pp %nf-advx[+pp:mod-mod]%
	setscore -2.63905732961526

nf-advx--pp = nf-advx* pp %nf-advx[+pp:mod]%
	setscore -1.94591014905531

nf-advx--pp = nf-advx* pp %nf-advx[+pp:oa-mod]%
	setscore -2.63905732961526

nf-advx--pp = nf-advx* pp %nf-advx[+pp:on-mod]%
	setscore -2.63905732961526

nf-advx--pp = nf-advx* pp %nf-advx[+pp:v-mod]%
	setscore -1.94591014905531

nf-advx--pp = nf-advx--advx* pp %nf-advx--advx[+pp:foppk]%
	setscore -2.63905732961526

nf-advx--pp = nf-advx--advx* pp %nf-advx--advx[+pp:mod]%
	setscore -2.63905732961526

nf-advx--pp = nf-advx--advx* pp %nf-advx--advx[+pp:oa-mod]%
	setscore -2.63905732961526

nf-advx--pp = nf-advx--advx* pp %nf-advx--advx[+pp:v-mod]%
	setscore -2.63905732961526

nf-advx--pp = nf-advx--advx* pp %nf-advx--advx[+pp:v-modk]%
	setscore -2.63905732961526

nf-advx--simpx = nf-advx* simpx %nf-advx[+simpx:mod]%
	setscore -1.38629436111989

nf-advx--simpx = nf-advx--comma* simpx %nf-advx--comma[+simpx:mod]%
	setscore -0.693147180559945

nf-advx--simpx = nf-advx--comma* simpx %nf-advx--comma[+simpx:onk]%
	setscore -1.38629436111989

nf-ap = ap* %NF[ap:mod-mod]%
	Block := 1,
	setscore -2.70805020110221

nf-ap = ap* %NF[ap:oa-mod]%
	Block := 1,
	setscore -2.01490302054226

nf-ap = ap* %NF[ap:on-mod]%
	Block := 1,
	setscore -2.70805020110221

nf-ap = ap* %NF[ap:onk]%
	Block := 1,
	setscore -2.70805020110221

nf-ap = ap* %NF[ap:pred-mod]%
	Block := 1,
	setscore -2.70805020110221

nf-ap = ap* %NF[ap:pred]%
	Block := 1,
	PRED := true,
	setscore -2.70805020110221

nf-ap = ap* %NF[ap:predk]%
	Block := 1,
	setscore -2.70805020110221

nf-ap = ap* %NF[ap:v-mod]%
	Block := 1,
	setscore -1.09861228866811

nf-ap = ap* %NF[ap:v-mod]%
	Block := 1,
	PRED := true,
	setscore -2.70805020110221

nf-ap = ap* %NF[ap:v-modk]%
	Block := 1,
	setscore -2.70805020110221

nf-ap--comma = nf-ap* comma %nf-ap[+comma]%
	setscore 0

nf-ap--np = nf-ap* np %nf-ap[+np:oa-mod]%
	setscore -0.693147180559945

nf-ap--np = nf-ap--comma* np %nf-ap--comma[+np:pred]%
	setscore -0.693147180559945

nf-ap--pp = nf-ap* pp %nf-ap[+pp:compl1]%
	setscore -0.693147180559945

nf-ap--pp = nf-ap* pp %nf-ap[+pp:v-mod]%
	setscore -0.693147180559945

nf-ap--simpx = nf-ap--comma* simpx %nf-ap--comma[+simpx:mod]%
	setscore 0

nf-fkonj = fkonj* %NF[fkonj:os]%
	Block := 1,
	setscore 0

nf-fx = fx* %NF[fx:mod]%
	Block := 1,
	setscore -1.38629436111989

nf-fx = fx* %NF[fx:oa-mod]%
	Block := 1,
	setscore -1.38629436111989

nf-fx = fx* %NF[fx:on-mod]%
	Block := 1,
	setscore -1.38629436111989

nf-fx = fx* %NF[fx:os]%
	Block := 1,
	setscore -1.38629436111989

nf-np = np* %NF[np:compl1]%
	Block := 1,
	setscore -3.58351893845611

nf-np = np* %NF[np:foppk]%
	Block := 1,
	setscore -3.58351893845611

nf-np = np* %NF[np:mod-mod]%
	Block := 1,
	setscore -2.484906649788

nf-np = np* %NF[np:mod]%
	Block := 1,
	setscore -3.58351893845611

nf-np = np* %NF[np:modk]%
	Block := 1,
	setscore -3.58351893845611

nf-np = np* %NF[np:oa-mod]%
	Block := 1,
	setscore -2.484906649788

nf-np = np* %NF[np:oa]%
	Block := 1,
	C == nom,
	OA := true,
	OA_C := C,
	setscore -3.58351893845611

nf-np = np* %NF[np:oadjp-mo]%
	Block := 1,
	setscore -3.58351893845611

nf-np = np* %NF[np:oak]%
	Block := 1,
	setscore -3.58351893845611

nf-np = np* %NF[np:od-mod]%
	Block := 1,
	setscore -3.58351893845611

nf-np = np* %NF[np:odk]%
	Block := 1,
	setscore -3.58351893845611

nf-np = np* %NF[np:og-mod]%
	Block := 1,
	setscore -3.58351893845611

nf-np = np* %NF[np:on-mod]%
	Block := 1,
	setscore -1.79175946922806

nf-np = np* %NF[np:onk]%
	Block := 1,
	setscore -2.89037175789617

nf-np = np* %NF[np:opp-mod]%
	Block := 1,
	setscore -3.58351893845611

nf-np = np* %NF[np:pred-mod]%
	Block := 1,
	setscore -3.58351893845611

nf-np = np* %NF[np:pred]%
	Block := 1,
	PRED := true,
	setscore -2.89037175789617

nf-np = np* %NF[np:predk]%
	Block := 1,
	setscore -3.58351893845611

nf-np = np* %NF[np:subject]%
	Block := 1,
	setscore -3.58351893845611

nf-np = np* %NF[np:v-mod]%
	Block := 1,
	setscore -1.97408102602201

nf-np = np* %NF[np:v-modk]%
	Block := 1,
	setscore -3.58351893845611

nf-np--advx = nf-np* advx %nf-np[+advx:mod]%
	setscore -0.287682072451781

nf-np--advx = nf-np--advx* advx %nf-np--advx[+advx:mod]%
	setscore -1.38629436111989

nf-np--comma = nf-np* comma %nf-np[+comma]%
	setscore 0

nf-np--np = nf-np* np %nf-np[+np:v-mod]%
	setscore 0

nf-np--pp = nf-np* pp %nf-np[+pp:fopp]%
	setscore -1.6094379124341

nf-np--pp = nf-np* pp %nf-np[+pp:mod-mod]%
	setscore -1.6094379124341

nf-np--pp = nf-np* pp %nf-np[+pp:mod]%
	setscore -0.916290731874155

nf-np--pp = nf-np* pp %nf-np[+pp:v-mod]%
	setscore -1.6094379124341

nf-np--r-simpx = nf-np--comma* r-simpx %nf-np--comma[+r-simpx:on-mod]%
	setscore 0

nf-np--simpx = nf-np--comma* simpx %nf-np--comma[+simpx:mod]%
	setscore -0.22314355131421

nf-np--simpx = nf-np--comma* simpx %nf-np--comma[+simpx:os]%
	setscore -1.6094379124341

nf-p-simpx = p-simpx* %NF[p-simpx:os]%
	Block := 1,
	setscore 0

nf-pp = pp* %NF[pp:compl1]%
	Block := 1,
	setscore -2.89037175789617

nf-pp = pp* %NF[pp:compl2]%
	Block := 1,
	setscore -3.98898404656427

nf-pp = pp* %NF[pp:fopp-mod]%
	Block := 1,
	setscore -3.29583686600433

nf-pp = pp* %NF[pp:fopp]%
	Block := 1,
	setscore -2.60268968544438

nf-pp = pp* %NF[pp:foppk]%
	Block := 1,
	setscore -3.98898404656427

nf-pp = pp* %NF[pp:mod-mod]%
	Block := 1,
	setscore -2.60268968544438

nf-pp = pp* %NF[pp:mod]%
	Block := 1,
	setscore -1.5910887737659

nf-pp = pp* %NF[pp:modk]%
	Block := 1,
	setscore -3.98898404656427

nf-pp = pp* %NF[pp:oa-mod]%
	Block := 1,
	setscore -3.29583686600433

nf-pp = pp* %NF[pp:oadjp-mo]%
	Block := 1,
	setscore -3.98898404656427

nf-pp = pp* %NF[pp:oadjp]%
	Block := 1,
	setscore -3.98898404656427

nf-pp = pp* %NF[pp:oadvpk]%
	Block := 1,
	setscore -3.98898404656427

nf-pp = pp* %NF[pp:oak]%
	Block := 1,
	setscore -3.98898404656427

nf-pp = pp* %NF[pp:od-mod]%
	Block := 1,
	setscore -3.98898404656427

nf-pp = pp* %NF[pp:on-mod]%
	Block := 1,
	setscore -3.98898404656427

nf-pp = pp* %NF[pp:on-modk]%
	Block := 1,
	setscore -3.98898404656427

nf-pp = pp* %NF[pp:opp-mod]%
	Block := 1,
	setscore -3.98898404656427

nf-pp = pp* %NF[pp:oppk]%
	Block := 1,
	setscore -3.98898404656427

nf-pp = pp* %NF[pp:pred-mod]%
	Block := 1,
	setscore -2.89037175789617

nf-pp = pp* %NF[pp:predk]%
	Block := 1,
	setscore -3.98898404656427

nf-pp = pp* %NF[pp:v-mod]%
	Block := 1,
	setscore -1.68639895357023

nf-pp = pp* %NF[pp:v-modk]%
	Block := 1,
	setscore -3.29583686600433

nf-pp--advx = nf-pp* advx %nf-pp[+advx:mod]%
	setscore -0.980829253011726

nf-pp--advx = nf-pp* advx %nf-pp[+advx:v-mod]%
	setscore -2.07944154167984

nf-pp--advx = nf-pp--advx* advx %nf-pp--advx[+advx:mod]%
	setscore -2.07944154167984

nf-pp--advx = nf-pp--comma* advx %nf-pp--comma[+advx:mod]%
	setscore -1.38629436111989

nf-pp--advx = nf-pp--pp* advx %nf-pp--pp[+advx:mod]%
	setscore -2.07944154167984

nf-pp--ap = nf-pp--advx* ap %nf-pp--advx[+ap:predk]%
	setscore -1.38629436111989

nf-pp--ap = nf-pp--advx* ap %nf-pp--advx[+ap:v-mod]%
	setscore -1.38629436111989

nf-pp--ap = nf-pp--comma* ap %nf-pp--comma[+ap:pred-mod]%
	setscore -1.38629436111989

nf-pp--ap = nf-pp--comma* ap %nf-pp--comma[+ap:predk]%
	setscore -1.38629436111989

nf-pp--comma = nf-pp* comma %nf-pp[+comma]%
	setscore 0

nf-pp--np = nf-pp* np %nf-pp[+np:mod]%
	setscore -1.6094379124341

nf-pp--np = nf-pp* np %nf-pp[+np:oa-mod]%
	setscore -1.6094379124341

nf-pp--np = nf-pp* np %nf-pp[+np:on-mod]%
	setscore -1.6094379124341

nf-pp--np = nf-pp--comma* np %nf-pp--comma[+np:mod-mod]%
	setscore -1.6094379124341

nf-pp--np = nf-pp--comma* np %nf-pp--comma[+np:predk]%
	setscore -1.6094379124341

nf-pp--pp = nf-pp* pp %nf-pp[+pp:mod]%
	setscore -1.38629436111989

nf-pp--pp = nf-pp* pp %nf-pp[+pp:oa-mod]%
	setscore -2.484906649788

nf-pp--pp = nf-pp* pp %nf-pp[+pp:v-mod]%
	setscore -1.09861228866811

nf-pp--pp = nf-pp--advx* pp %nf-pp--advx[+pp:v-mod]%
	setscore -2.484906649788

nf-pp--pp = nf-pp--comma* pp %nf-pp--comma[+pp:fopp]%
	setscore -2.484906649788

nf-pp--pp = nf-pp--comma* pp %nf-pp--comma[+pp:oa-mod]%
	setscore -2.484906649788

nf-pp--pp = nf-pp--comma* pp %nf-pp--comma[+pp:v-mod]%
	setscore -2.484906649788

nf-pp--r-simpx = nf-pp--comma* r-simpx %nf-pp--comma[+r-simpx:on-mod]%
	setscore 0

nf-pp--simpx = nf-pp* simpx %nf-pp[+simpx:mod]%
	setscore -1.94591014905531

nf-pp--simpx = nf-pp--comma* simpx %nf-pp--comma[+simpx:mod]%
	setscore -0.336472236621213

nf-pp--simpx = nf-pp--comma* simpx %nf-pp--comma[+simpx:os]%
	setscore -1.94591014905531

nf-r-simpx = r-simpx* %NF[r-simpx:fopp-mod]%
	Block := 1,
	setscore -2.86220088092947

nf-r-simpx = r-simpx* %NF[r-simpx:konj]%
	Block := 1,
	setscore -3.55534806148941

nf-r-simpx = r-simpx* %NF[r-simpx:mod-mod]%
	Block := 1,
	setscore -3.55534806148941

nf-r-simpx = r-simpx* %NF[r-simpx:mod]%
	Block := 1,
	setscore -1.6094379124341

nf-r-simpx = r-simpx* %NF[r-simpx:oa-mod]%
	Block := 1,
	setscore -1.6094379124341

nf-r-simpx = r-simpx* %NF[r-simpx:od-mod]%
	Block := 1,
	setscore -2.4567357728213

nf-r-simpx = r-simpx* %NF[r-simpx:og-mod]%
	Block := 1,
	setscore -3.55534806148941

nf-r-simpx = r-simpx* %NF[r-simpx:on-mod]%
	Block := 1,
	setscore -2.16905370036952

nf-r-simpx = r-simpx* %NF[r-simpx:opp-mod]%
	Block := 1,
	setscore -3.55534806148941

nf-r-simpx = r-simpx* %NF[r-simpx:os]%
	Block := 1,
	setscore -3.55534806148941

nf-r-simpx = r-simpx* %NF[r-simpx:pred-mod]%
	Block := 1,
	setscore -1.94591014905531

nf-r-simpx = r-simpx* %NF[r-simpx:pred]%
	Block := 1,
	setscore -3.55534806148941

nf-r-simpx = r-simpx* %NF[r-simpx:subject]%
	Block := 1,
	setscore -3.55534806148941

nf-r-simpx--advx = nf-r-simpx* advx %nf-r-simpx[+advx:mod]%
	setscore 0

nf-r-simpx--comma = nf-r-simpx* comma %nf-r-simpx[+comma]%
	setscore 0

nf-r-simpx--np = nf-r-simpx* np %nf-r-simpx[+np:mod]%
	setscore -2.30258509299405

nf-r-simpx--np = nf-r-simpx* np %nf-r-simpx[+np:oa-mod]%
	setscore -2.30258509299405

nf-r-simpx--np = nf-r-simpx* np %nf-r-simpx[+np:onk]%
	setscore -2.30258509299405

nf-r-simpx--np = nf-r-simpx* np %nf-r-simpx[+np:pred-mod]%
	setscore -2.30258509299405

nf-r-simpx--np = nf-r-simpx--comma* np %nf-r-simpx--comma[+np:mod]%
	setscore -2.30258509299405

nf-r-simpx--np = nf-r-simpx--comma* np %nf-r-simpx--comma[+np:oa-mod]%
	setscore -2.30258509299405

nf-r-simpx--np = nf-r-simpx--comma* np %nf-r-simpx--comma[+np:oak]%
	setscore -2.30258509299405

nf-r-simpx--np = nf-r-simpx--comma* np %nf-r-simpx--comma[+np:on-mod]%
	setscore -2.30258509299405

nf-r-simpx--np = nf-r-simpx--comma* np %nf-r-simpx--comma[+np:onk]%
	setscore -2.30258509299405

nf-r-simpx--np = nf-r-simpx--pp* np %nf-r-simpx--pp[+np:oa-mod]%
	setscore -2.30258509299405

nf-r-simpx--pp = nf-r-simpx--comma* pp %nf-r-simpx--comma[+pp:mod]%
	setscore -0.405465108108164

nf-r-simpx--pp = nf-r-simpx--comma* pp %nf-r-simpx--comma[+pp:pred-mod]%
	setscore -1.09861228866811

nf-r-simpx--r-simpx = nf-r-simpx--comma* r-simpx %nf-r-simpx--comma[+r-simpx:mod]%
	setscore -0.693147180559945

nf-r-simpx--r-simpx = nf-r-simpx--comma* r-simpx %nf-r-simpx--comma[+r-simpx:oa-mod]%
	setscore -0.693147180559945

nf-r-simpx--simpx = nf-r-simpx* simpx %nf-r-simpx[+simpx:mod]%
	setscore -2.07944154167984

nf-r-simpx--simpx = nf-r-simpx* simpx %nf-r-simpx[+simpx:oa-mod]%
	setscore -2.07944154167984

nf-r-simpx--simpx = nf-r-simpx* simpx %nf-r-simpx[+simpx:os]%
	setscore -2.07944154167984

nf-r-simpx--simpx = nf-r-simpx--comma* simpx %nf-r-simpx--comma[+simpx:mod]%
	setscore -0.980829253011726

nf-r-simpx--simpx = nf-r-simpx--comma* simpx %nf-r-simpx--comma[+simpx:os]%
	setscore -2.07944154167984

nf-r-simpx--simpx = nf-r-simpx--comma* simpx %nf-r-simpx--comma[+simpx:subject]%
	setscore -2.07944154167984

nf-simpx = simpx* %NF[simpx:fopp-mod]%
	Block := 1,
	setscore -3.63758615972639

nf-simpx = simpx* %NF[simpx:foppk]%
	Block := 1,
	setscore -3.63758615972639

nf-simpx = simpx* %NF[simpx:mod-mod]%
	Block := 1,
	setscore -3.63758615972639

nf-simpx = simpx* %NF[simpx:mod]%
	Block := 1,
	setscore -2.2512917986065

nf-simpx = simpx* %NF[simpx:modk]%
	Block := 1,
	setscore -3.63758615972639

nf-simpx = simpx* %NF[simpx:oa-mod]%
	Block := 1,
	setscore -2.2512917986065

nf-simpx = simpx* %NF[simpx:oadjp-mo]%
	Block := 1,
	setscore -3.63758615972639

nf-simpx = simpx* %NF[simpx:oadvp-mo]%
	Block := 1,
	setscore -3.63758615972639

nf-simpx = simpx* %NF[simpx:od-mod]%
	Block := 1,
	setscore -3.63758615972639

nf-simpx = simpx* %NF[simpx:og-mod]%
	Block := 1,
	setscore -3.63758615972639

nf-simpx = simpx* %NF[simpx:on-mod]%
	Block := 1,
	setscore -3.63758615972639

nf-simpx = simpx* %NF[simpx:opp-mod]%
	Block := 1,
	setscore -2.94443897916644

nf-simpx = simpx* %NF[simpx:opp]%
	Block := 1,
	setscore -3.63758615972639

nf-simpx = simpx* %NF[simpx:os-mod]%
	Block := 1,
	setscore -3.63758615972639

nf-simpx = simpx* %NF[simpx:os]%
	Block := 1,
	setscore -2.53897387105828

nf-simpx = simpx* %NF[simpx:ov]%
	Block := 1,
	setscore -3.63758615972639

nf-simpx = simpx* %NF[simpx:pred-mod]%
	Block := 1,
	setscore -3.63758615972639

nf-simpx = simpx* %NF[simpx:pred]%
	Block := 1,
	PRED := true,
	setscore -3.63758615972639

nf-simpx = simpx* %NF[simpx:predk]%
	Block := 1,
	setscore -3.63758615972639

nf-simpx = simpx* %NF[simpx:subject]%
	Block := 1,
	setscore -1.33500106673234

nf-simpx--advx = nf-simpx--comma* advx %nf-simpx--comma[+advx:mod]%
	setscore 0

nf-simpx--ap = nf-simpx--comma* ap %nf-simpx--comma[+ap:modk]%
	setscore -0.693147180559945

nf-simpx--ap = nf-simpx--comma* ap %nf-simpx--comma[+ap:predk]%
	setscore -0.693147180559945

nf-simpx--comma = nf-simpx* comma %nf-simpx[+comma]%
	setscore 0

nf-simpx--np = nf-simpx* np %nf-simpx[+np:onk]%
	setscore -1.79175946922806

nf-simpx--np = nf-simpx* np %nf-simpx[+np:predk]%
	setscore -1.79175946922806

nf-simpx--np = nf-simpx--comma* np %nf-simpx--comma[+np:oa-mod]%
	setscore -1.79175946922806

nf-simpx--np = nf-simpx--comma* np %nf-simpx--comma[+np:oak]%
	setscore -1.79175946922806

nf-simpx--np = nf-simpx--comma* np %nf-simpx--comma[+np:on-mod]%
	setscore -1.79175946922806

nf-simpx--np = nf-simpx--comma* np %nf-simpx--comma[+np:pred-mod]%
	setscore -1.79175946922806

nf-simpx--pp = nf-simpx* pp %nf-simpx[+pp:pred-mod]%
	setscore -1.09861228866811

nf-simpx--pp = nf-simpx--comma* pp %nf-simpx--comma[+pp:mod]%
	setscore -1.09861228866811

nf-simpx--pp = nf-simpx--comma* pp %nf-simpx--comma[+pp:oppk]%
	setscore -1.09861228866811

nf-simpx--r-simpx = nf-simpx--comma* r-simpx %nf-simpx--comma[+r-simpx:mod]%
	setscore 0

nf-simpx--simpx = nf-simpx* simpx %nf-simpx[+simpx:mod]%
	setscore -1.38629436111989

nf-simpx--simpx = nf-simpx--comma* simpx %nf-simpx--comma[+simpx:mod]%
	setscore -0.693147180559945

nf-simpx--simpx = nf-simpx--comma* simpx %nf-simpx--comma[+simpx:pred-mod]%
	setscore -1.38629436111989

nf-vp = vp* %NF[vp]%
	Block := 1,
	setscore 0

np = np-advx*
	{ Block := false },
	setscore -6.93342302573071

np = np-advx--ap*
	{ Block := false },
	setscore -5.54712866461082

np = np-advx--dp*
	{ Block := false },
	setscore -6.24027584517077

np = np-advx--noun*
	{ Block := false },
	setscore -6.93342302573071

np = np-advx--np*
	{ Block := false },
	setscore -4.29436569611546

np = np-advx--pron*
	{ Block := false },
	setscore -6.93342302573071

np = np-advx--simpx*
	{ Block := false },
	setscore -6.93342302573071

np = np-advx--trunc*
	{ Block := false },
	setscore -6.93342302573071

np = np-ap*
	{ Block := false },
	setscore -6.93342302573071

np = np-ap--ap*
	{ Block := false },
	setscore -6.93342302573071

np = np-ap--np*
	{ Block := false },
	setscore -6.93342302573071

np = np-ap--pp*
	{ Block := false },
	setscore -6.24027584517077

np = np-ap--trunc*
	{ Block := false },
	setscore -6.93342302573071

np = np-article*
	{ Block := false },
	setscore -6.93342302573071

np = np-article--advx*
	{ Block := false },
	setscore -6.93342302573071

np = np-article--ap*
	{ Block := false },
	setscore -5.8348107370626

np = np-article--noun*
	{ Block := false },
	setscore -6.24027584517077

np = np-article--np*
	{ Block := false },
	setscore -6.93342302573071

np = np-article--pp*
	{ Block := false },
	setscore -6.93342302573071

np = np-article--prn*
	{ Block := false },
	setscore -6.93342302573071

np = np-article--trunc*
	{ Block := false },
	setscore -6.24027584517077

np = np-conj--advx*
	{ Block := false },
	setscore -6.93342302573071

np = np-conj--ap*
	{ Block := false },
	setscore -5.54712866461082

np = np-conj--fkonj*
	{ Block := false },
	setscore -6.93342302573071

np = np-conj--fx*
	{ Block := false },
	setscore -6.93342302573071

np = np-conj--np*
	{ Block := false },
	setscore -4.36847366826918

np = np-conj--pp*
	{ Block := false },
	setscore -6.93342302573071

np = np-conj--simpx*
	{ Block := false },
	setscore -6.24027584517077

np = np-conj--trunc*
	{ Block := false },
	setscore -6.93342302573071

np = np-fx--advx*
	{ Block := false },
	setscore -5.8348107370626

np = np-fx--ap*
	{ Block := false },
	setscore -6.24027584517077

np = np-fx--article*
	{ Block := false },
	setscore -5.8348107370626

np = np-fx--conj*
	{ Block := false },
	setscore -5.8348107370626

np = np-fx--fx*
	{ Block := false },
	setscore -6.93342302573071

np = np-fx--np*
	{ Block := false },
	setscore -4.63083793273667

np = np-fx--pp*
	{ Block := false },
	setscore -6.93342302573071

np = np-fx--pron*
	{ Block := false },
	setscore -6.24027584517077

np = np-fx--r-simpx*
	{ Block := false },
	setscore -6.93342302573071

np = np-fx--simpx*
	{ Block := false },
	setscore -6.24027584517077

np = np-noun*
	{ Block := false },
	setscore -6.24027584517077

np = np-noun--advx*
	{ Block := false },
	setscore -3.21985095902641

np = np-noun--ap*
	{ Block := false },
	setscore -3.93769075217672

np = np-noun--article*
	{ Block := false },
	setscore -2.89037175789617

np = np-noun--conj*
	{ Block := false },
	setscore -3.49943582124557

np = np-noun--dm*
	{ Block := false },
	setscore -6.93342302573071

np = np-noun--dp*
	{ Block := false },
	setscore -5.54712866461082

np = np-noun--fx*
	{ Block := false },
	setscore -6.24027584517077

np = np-noun--np*
	{ Block := false },
	setscore -3.98898404656427

np = np-noun--num*
	{ Block := false },
	setscore -6.24027584517077

np = np-noun--part*
	{ Block := false },
	setscore -6.93342302573071

np = np-noun--pp*
	{ Block := false },
	setscore -4.44851637594271

np = np-noun--prep*
	{ Block := false },
	setscore -6.93342302573071

np = np-noun--prn*
	{ Block := false },
	setscore -6.93342302573071

np = np-noun--pron*
	{ Block := false },
	setscore -2.69931652113346

np = np-np*
	{ Block := false },
	setscore -6.93342302573071

np = np-np--advx*
	{ Block := false },
	setscore -3.40706250111455

np = np-np--ap*
	{ Block := false },
	setscore -4.63083793273667

np = np-np--article*
	{ Block := false },
	setscore -3.98898404656427

np = np-np--conj*
	{ Block := false },
	setscore -4.16083430349093

np = np-np--dp*
	{ Block := false },
	setscore -6.93342302573071

np = np-np--fx*
	{ Block := false },
	setscore -4.53552775293234

np = np-np--noun*
	{ Block := false },
	setscore -6.93342302573071

np = np-np--np*
	{ Block := false },
	setscore -2.2329426599383

np = np-np--pp*
	{ Block := false },
	setscore -2.94443897916644

np = np-np--prn*
	{ Block := false },
	setscore -5.8348107370626

np = np-np--pron*
	{ Block := false },
	setscore -4.04305126783455

np = np-np--r-simpx*
	{ Block := false },
	setscore -4.29436569611546

np = np-np--simpx*
	{ Block := false },
	setscore -3.93769075217672

np = np-np--unknown*
	{ Block := false },
	setscore -6.93342302573071

np = np-np--vp*
	{ Block := false },
	setscore -6.24027584517077

np = np-num*
	{ Block := false },
	setscore -6.93342302573071

np = np-num--advx*
	{ Block := false },
	setscore -5.54712866461082

np = np-num--ap*
	{ Block := false },
	setscore -6.93342302573071

np = np-num--article*
	{ Block := false },
	setscore -6.24027584517077

np = np-num--conj*
	{ Block := false },
	setscore -6.24027584517077

np = np-num--np*
	{ Block := false },
	setscore -6.93342302573071

np = np-num--num*
	{ Block := false },
	setscore -6.93342302573071

np = np-num--pp*
	{ Block := false },
	setscore -6.93342302573071

np = np-num--pron*
	{ Block := false },
	setscore -6.24027584517077

np = np-part--np*
	{ Block := false },
	setscore -6.24027584517077

np = np-pp*
	{ Block := false },
	setscore -6.93342302573071

np = np-pp--ap*
	{ Block := false },
	setscore -6.24027584517077

np = np-pp--np*
	{ Block := false },
	setscore -6.24027584517077

np = np-pp--pp*
	{ Block := false },
	setscore -6.93342302573071

np = np-pp--pron*
	{ Block := false },
	setscore -6.24027584517077

np = np-pp--r-simpx*
	{ Block := false },
	setscore -6.93342302573071

np = np-prn*
	{ Block := false },
	setscore -6.24027584517077

np = np-prn--advx*
	{ Block := false },
	setscore -5.14166355650266

np = np-prn--ap*
	{ Block := false },
	setscore -5.8348107370626

np = np-prn--article*
	{ Block := false },
	setscore -4.9875128766754

np = np-prn--conj*
	{ Block := false },
	setscore -5.54712866461082

np = np-prn--dp*
	{ Block := false },
	setscore -6.24027584517077

np = np-prn--fx*
	{ Block := false },
	setscore -6.93342302573071

np = np-prn--noun*
	{ Block := false },
	setscore -6.93342302573071

np = np-prn--np*
	{ Block := false },
	setscore -5.8348107370626

np = np-prn--pp*
	{ Block := false },
	setscore -5.8348107370626

np = np-prn--prn*
	{ Block := false },
	setscore -5.54712866461082

np = np-prn--pron*
	{ Block := false },
	setscore -4.9875128766754

np = np-pron*
	{ Block := false },
	setscore -4.7361984483945

np = np-pron--advx*
	{ Block := false },
	setscore -4.36847366826918

np = np-pron--ap*
	{ Block := false },
	setscore -6.93342302573071

np = np-pron--article*
	{ Block := false },
	setscore -5.32398511329661

np = np-pron--conj*
	{ Block := false },
	setscore -5.14166355650266

np = np-pron--noun*
	{ Block := false },
	setscore -6.93342302573071

np = np-pron--part*
	{ Block := false },
	setscore -6.93342302573071

np = np-pron--pp*
	{ Block := false },
	setscore -6.24027584517077

np = np-pron--pron*
	{ Block := false },
	setscore -6.24027584517077

np = np-simpx--advx*
	{ Block := false },
	setscore -6.93342302573071

np = np-simpx--np*
	{ Block := false },
	setscore -6.93342302573071

np = np-simpx--pp*
	{ Block := false },
	setscore -6.93342302573071

np = np-trunc*
	{ Block := false },
	setscore -6.93342302573071

np = np-trunc--noun*
	{ Block := false },
	setscore -4.9875128766754

np = np-trunc--np*
	{ Block := false },
	setscore -5.54712866461082

np = np-trunc--prn*
	{ Block := false },
	setscore -6.93342302573071

np = np-unknown*
	{ Block := false },
	setscore -6.93342302573071

np = np-unknown--advx*
	{ Block := false },
	setscore -6.24027584517077

np = np-unknown--ap*
	{ Block := false },
	setscore -6.93342302573071

np = np-unknown--article*
	{ Block := false },
	setscore -5.54712866461082

np = np-unknown--conj*
	{ Block := false },
	setscore -6.93342302573071

np = np-unknown--pron*
	{ Block := false },
	setscore -6.24027584517077

np-adjective = adjective* %NP[adjective]%
	Block := 1,
	setscore 0

np-adjective--article = article np-adjective--ap* %np-adjective--ap[article+]%
	setscore 0

np-advx = advx* %NP[advx]%
	Block := 1,
	setscore -0.7339691750802

np-advx = advx* %NP[advx]%
	Block := 1,
	Num := any,
	P := any,
	R := any,
	setscore -0.653926467406664

np-advx--advx = np-advx* advx %np-advx[+advx]%
	setscore -0.559615787935423

np-advx--advx = np-advx--conj* advx %np-advx--conj[+advx]%
	setscore -0.847297860387204

np-advx--ap = np-advx* ap %np-advx[+ap]%
	setscore -1.38629436111989

np-advx--ap = np-advx--advx* ap %np-advx--advx[+ap]%
	setscore -1.38629436111989

np-advx--ap = np-advx--conj* ap %np-advx--conj[+ap:konj]%
	setscore -1.38629436111989

np-advx--ap = np-advx--conj* ap %np-advx--conj[+ap]%
	setscore -1.38629436111989

np-advx--comma = np-advx--np* comma %np-advx--np[+comma]%
	setscore -0.154150679827258

np-advx--comma = np-advx--pp* comma %np-advx--pp[+comma]%
	setscore -1.94591014905531

np-advx--conj = np-advx* conj %np-advx[+conj]%
	conj.T == comp,
	setscore -2.484906649788

np-advx--conj = np-advx--comma* conj %np-advx--comma[+conj]%
	conj.T == coord,
	setscore -0.693147180559945

np-advx--conj = np-advx--np* conj %np-advx--np[+conj]%
	conj.T == comp,
	setscore -2.484906649788

np-advx--conj = np-advx--np* conj %np-advx--np[+conj]%
	conj.T == coord,
	setscore -1.38629436111989

np-advx--dp = np-advx* dp %np-advx[+dp]%
	setscore -0.693147180559945

np-advx--dp = np-advx--advx* dp %np-advx--advx[+dp]%
	setscore -0.693147180559945

np-advx--noun = np-advx* noun %np-advx[+noun]%
	setscore 0

np-advx--np = np-advx* np %np-advx[+np:app]%
	C := np.C,
	Num := np.Num,
	P := np.P,
	R := np.R,
	setscore -3.2188758248682

np-advx--np = np-advx* np %np-advx[+np:konj]%
	C := np.C,
	setscore -1.27296567581289

np-advx--np = np-advx* np %np-advx[+np:konj]%
	setscore -2.52572864430826

np-advx--np = np-advx* np %np-advx[+np]%
	setscore -3.2188758248682

np-advx--np = np-advx--advx* np %np-advx--advx[+np:konj]%
	C := np.C,
	setscore -2.52572864430826

np-advx--np = np-advx--advx* np %np-advx--advx[+np:konj]%
	C == np.C,
	setscore -2.52572864430826

np-advx--np = np-advx--advx* np %np-advx--advx[+np:konj]%
	setscore -3.2188758248682

np-advx--np = np-advx--comma* np %np-advx--comma[+np:app]%
	C == np.C,
	Num == np.Num,
	P == np.P,
	setscore -3.2188758248682

np-advx--np = np-advx--conj* np %np-advx--conj[+np:konj]%
	C == np.C,
	setscore -2.12026353620009

np-advx--np = np-advx--conj* np %np-advx--conj[+np:konj]%
	setscore -2.12026353620009

np-advx--np = np-advx--pp* np %np-advx--pp[+np:konj]%
	C := np.C,
	setscore -3.2188758248682

np-advx--pp = np-advx* pp %np-advx[+pp:compl1]%
	setscore -0.693147180559945

np-advx--pp = np-advx* pp %np-advx[+pp:konj]%
	setscore -0.693147180559945

np-advx--pron = np-advx* pron %np-advx[+pron]%
	setscore 0

np-advx--simpx = np-advx--conj* simpx %np-advx--conj[+simpx:konj]%
	setscore 0

np-advx--trunc = np-advx* trunc %np-advx[+trunc]%
	setscore 0

np-ap = ap* %NP[ap:konj]%
	Block := 1,
	setscore -1.25276296849537

np-ap = ap* %NP[ap]%
	Block := 1,
	setscore -0.336472236621213

np-ap--ap = np-ap* ap %np-ap[+ap]%
	setscore 0

np-ap--comma = np-ap* comma %np-ap[+comma]%
	setscore 0

np-ap--conj = np-ap* conj %np-ap[+conj]%
	conj.T == coord,
	setscore -0.693147180559945

np-ap--conj = np-ap--comma* conj %np-ap--comma[+conj]%
	conj.T == coord,
	setscore -0.693147180559945

np-ap--np = np-ap* np %np-ap[+np]%
	setscore -0.693147180559945

np-ap--pp = np-ap* pp %np-ap[+pp:compl1]%
	setscore -0.693147180559945

np-ap--pp = np-ap--conj* pp %np-ap--conj[+pp:konj]%
	setscore -0.693147180559945

np-ap--trunc = np-ap* trunc %np-ap[+trunc]%
	setscore 0

np-article = article* %NP[article]%
	Block := 1,
	setscore -0.0800427076735364

np-article = article* %NP[article]%
	Block := 1,
	Proper := true,
	setscore -2.56494935746154

np-article--advx = np-article* advx %np-article[+advx]%
	setscore 0

np-article--ap = np-article* ap %np-article[+ap]%
	setscore -0.559615787935423

np-article--ap = np-article--advx* ap %np-article--advx[+ap]%
	setscore -1.94591014905531

np-article--ap = np-article--ap* ap %np-article--ap[+ap]%
	setscore -1.94591014905531

np-article--noun = np-article* noun %np-article[+noun]%
	setscore -0.693147180559945

np-article--noun = np-article--ap* noun %np-article--ap[+noun]%
	setscore -0.693147180559945

np-article--np = np-article* np %np-article[+np]%
	setscore 0

np-article--pp = np-article* pp %np-article[+pp:compl1]%
	setscore 0

np-article--prn = np-article* prn %np-article[+prn]%
	C := prn.C,
	Num := prn.Num,
	P := prn.P,
	R := prn.R,
	setscore 0

np-article--trunc = np-article* trunc %np-article[+trunc]%
	setscore -0.693147180559945

np-article--trunc = np-article--ap* trunc %np-article--ap[+trunc]%
	setscore -0.693147180559945

np-conj = conj* %NP[conj]%
	Block := 1,
	Num := any,
	P := any,
	R := any,
	T == coord,
	setscore -0.367724780125317

np-conj = conj* %NP[conj]%
	Block := 1,
	T == comp,
	setscore -1.31218638896617

np-conj = conj* %NP[conj]%
	Block := 1,
	T == coord,
	setscore -3.25809653802148

np-conj--advx = np-conj* advx %np-conj[+advx]%
	setscore -0.847297860387204

np-conj--advx = np-conj--advx* advx %np-conj--advx[+advx]%
	setscore -1.94591014905531

np-conj--advx = np-conj--comma* advx %np-conj--comma[+advx]%
	setscore -2.63905732961526

np-conj--advx = np-conj--conj* advx %np-conj--conj[+advx]%
	setscore -1.02961941718116

np-conj--ap = np-conj* ap %np-conj[+ap]%
	setscore -1.79175946922806

np-conj--ap = np-conj--ap* ap %np-conj--ap[+ap]%
	setscore -1.79175946922806

np-conj--ap = np-conj--article* ap %np-conj--article[+ap]%
	setscore -0.693147180559945

np-conj--ap = np-conj--pron* ap %np-conj--pron[+ap]%
	setscore -1.79175946922806

np-conj--article = np-conj* article %np-conj[+article]%
	setscore -0.405465108108164

np-conj--comma = np-conj--np* comma %np-conj--np[+comma]%
	setscore 0

np-conj--conj = np-conj--comma* conj %np-conj--comma[+conj]%
	conj.T == coord,
	setscore -1.70474809223843

np-conj--conj = np-conj--np* conj %np-conj--np[+conj]%
	conj.T == coord,
	setscore -0.200670695462151

np-conj--fkonj = np-conj--conj* fkonj %np-conj--conj[+fkonj:konj]%
	setscore 0

np-conj--fx = np-conj--advx* fx %np-conj--advx[+fx:konj]%
	setscore 0

np-conj--np = np-conj* np %np-conj[+np:konj]%
	C := np.C,
	setscore -0.955511445027436

np-conj--np = np-conj* np %np-conj[+np:konj]%
	setscore -2.15948424935337

np-conj--np = np-conj--advx* np %np-conj--advx[+np:konj]%
	C := np.C,
	setscore -2.15948424935337

np-conj--np = np-conj--advx* np %np-conj--advx[+np:konj]%
	C == np.C,
	setscore -2.15948424935337

np-conj--np = np-conj--advx* np %np-conj--advx[+np:konj]%
	setscore -2.15948424935337

np-conj--np = np-conj--advx* np %np-conj--advx[+np]%
	setscore -3.25809653802148

np-conj--np = np-conj--conj* np %np-conj--conj[+np:konj]%
	C == np.C,
	setscore -3.25809653802148

np-conj--np = np-conj--conj* np %np-conj--conj[+np:konj]%
	setscore -3.25809653802148

np-conj--pp = np-conj--conj* pp %np-conj--conj[+pp:konj]%
	setscore 0

np-conj--pron = np-conj* pron %np-conj[+pron]%
	setscore 0

np-conj--simpx = np-conj--conj* simpx %np-conj--conj[+simpx:konj]%
	setscore 0

np-conj--trunc = np-conj* trunc %np-conj[+trunc]%
	setscore 0

np-fx = fx* %NP[fx:app]%
	Block := 1,
	setscore -2.05412373369555

np-fx = fx* %NP[fx:konj]%
	Block := 1,
	setscore -2.27726728500976

np-fx = fx* %NP[fx]%
	Block := 1,
	setscore -0.262364264467491

np-fx--advx = advx np-fx--advx* %np-fx--advx[advx+]%
	setscore -1.25276296849537

np-fx--advx = advx np-fx--article* %np-fx--article[advx+]%
	setscore -1.94591014905531

np-fx--advx = advx np-fx--conj* %np-fx--conj[advx+]%
	setscore -1.94591014905531

np-fx--ap = ap np-fx* %np-fx[ap+]%
	setscore -0.606135803570316

np-fx--ap = np-fx* ap %np-fx[+ap:app]%
	setscore -2.39789527279837

np-fx--article = article np-fx* %np-fx[article+]%
	setscore -1.25276296849537

np-fx--article = article np-fx--ap* %np-fx--ap[article+]%
	setscore -1.25276296849537

np-fx--comma = np-fx* comma %np-fx[+comma]%
	setscore 0

np-fx--conj = conj np-fx* %np-fx[conj+]%
	conj.T == comp,
	setscore -1.20397280432594

np-fx--conj = conj np-fx--ap* %np-fx--ap[conj+]%
	conj.T == comp,
	setscore -2.30258509299405

np-fx--conj = conj np-fx--np* %np-fx--np[conj+]%
	conj.T == comp,
	setscore -2.30258509299405

np-fx--conj = np-fx* conj %np-fx[+conj]%
	conj.T == coord,
	setscore -1.6094379124341

np-fx--fx = np-fx--conj* fx %np-fx--conj[+fx:konj]%
	setscore -0.693147180559945

np-fx--np = np np-fx* %np-fx[np:possessive+]%
	setscore -1.87180217690159

np-fx--np = np np-fx--ap* %np-fx--ap[np:possessive+]%
	setscore -2.56494935746154

np-fx--np = np-fx* np %np-fx[+np:app]%
	setscore -2.56494935746154

np-fx--np = np-fx* np %np-fx[+np]%
	setscore -2.56494935746154

np-fx--np = np-fx--comma* np %np-fx--comma[+np:app]%
	setscore -2.56494935746154

np-fx--np = np-fx--comma* np %np-fx--comma[+np:konj]%
	setscore -2.56494935746154

np-fx--np = np-fx--comma* np %np-fx--comma[+np]%
	setscore -2.56494935746154

np-fx--np = np-fx--conj* np %np-fx--conj[+np:konj]%
	setscore -1.87180217690159

np-fx--np = np-fx--conj* np %np-fx--conj[+np:possessive]%
	setscore -1.87180217690159

np-fx--pp = np-fx* pp %np-fx[+pp:compl1]%
	setscore 0

np-fx--pron = pron np-fx* %np-fx[pron+]%
	setscore -1.09861228866811

np-fx--pron = pron np-fx--ap* %np-fx--ap[pron+]%
	setscore -1.09861228866811

np-fx--r-simpx = np-fx* r-simpx %np-fx[+r-simpx]%
	setscore 0

np-fx--simpx = np-fx* simpx %np-fx[+simpx:app]%
	setscore -0.693147180559945

np-fx--simpx = np-fx* simpx %np-fx[+simpx]%
	setscore -0.693147180559945

np-noun = noun* %NP[noun:konj]%
	Block := 1,
	Num := any,
	P := any,
	R := any,
	setscore -5.80513496891649

np-noun = noun* %NP[noun]%
	Block := 1,
	setscore -0.00604231445596259

np-noun = noun* %NP[noun]%
	Block := 1,
	Proper := true,
	setscore -5.80513496891649

np-noun--advx = advx np-noun* %np-noun[advx+]%
	setscore -2.21920348405499

np-noun--advx = advx np-noun--advx* %np-noun--advx[advx+]%
	setscore -1.9568392195875

np-noun--advx = advx np-noun--ap* %np-noun--ap[advx+]%
	setscore -1.5773495978826

np-noun--advx = advx np-noun--article* %np-noun--article[advx+]%
	setscore -1.68857523299282

np-noun--advx = advx np-noun--conj* %np-noun--conj[advx+]%
	setscore -2.12389330425067

np-noun--advx = advx np-noun--np* %np-noun--np[advx+]%
	setscore -2.91235066461494

np-noun--advx = advx np-noun--pp* %np-noun--pp[advx+]%
	setscore -4.52178857704904

np-noun--advx = advx np-noun--pron* %np-noun--pron[advx+]%
	setscore -2.03688192726104

np-noun--ap = ap np-noun* %np-noun[ap+]%
	C :== ap.C,
	Num :== ap.Num,
	R :== ap.R,
	setscore -5.57594910314632

np-noun--ap = ap np-noun* %np-noun[ap+]%
	C :== ap.C,
	Num :== ap.Num,
	R :== ap.R,
	ap.O == attr,
	setscore -0.655968177318191

np-noun--ap = ap np-noun* %np-noun[ap+]%
	C :== ap.C,
	Num :== ap.Num,
	ap.O == attr,
	setscore -4.18965474202643

np-noun--ap = ap np-noun* %np-noun[ap+]%
	C :== ap.C,
	R :== ap.R,
	ap.O == attr,
	setscore -3.78418963391826

np-noun--ap = ap np-noun* %np-noun[ap+]%
	C :== ap.C,
	ap.O == attr,
	setscore -5.57594910314632

np-noun--ap = ap np-noun* %np-noun[ap+]%
	Num :== ap.Num,
	setscore -5.57594910314632

np-noun--ap = ap np-noun* %np-noun[ap+]%
	Num :== ap.Num,
	R :== ap.R,
	ap.O == attr,
	setscore -4.47733681447821

np-noun--ap = ap np-noun* %np-noun[ap+]%
	R :== ap.R,
	ap.O == attr,
	setscore -5.57594910314632

np-noun--ap = ap np-noun* %np-noun[ap+]%
	ap.O == attr,
	setscore -5.57594910314632

np-noun--ap = ap np-noun* %np-noun[ap+]%
	setscore -3.78418963391826

np-noun--ap = ap np-noun--advx* %np-noun--advx[ap+]%
	C :== ap.C,
	Num :== ap.Num,
	R :== ap.R,
	ap.O == attr,
	setscore -4.18965474202643

np-noun--ap = ap np-noun--ap* %np-noun--ap[ap+]%
	C :== ap.C,
	Num :== ap.Num,
	R :== ap.R,
	ap.O == attr,
	setscore -2.11021320034659

np-noun--ap = ap np-noun--ap* %np-noun--ap[ap+]%
	C :== ap.C,
	Num :== ap.Num,
	ap.O == attr,
	setscore -4.18965474202643

np-noun--ap = ap np-noun--ap* %np-noun--ap[ap+]%
	C :== ap.C,
	R :== ap.R,
	ap.O == attr,
	setscore -4.18965474202643

np-noun--ap = ap np-noun--ap* %np-noun--ap[ap+]%
	Num :== ap.Num,
	R :== ap.R,
	ap.O == attr,
	setscore -5.57594910314632

np-noun--ap = ap np-noun--ap* %np-noun--ap[ap+]%
	setscore -4.88280192258637

np-noun--ap = ap np-noun--comma* %np-noun--comma[ap+]%
	C :== ap.C,
	Num :== ap.Num,
	R :== ap.R,
	ap.O == attr,
	setscore -4.88280192258637

np-noun--ap = ap np-noun--comma* %np-noun--comma[ap+]%
	Num :== ap.Num,
	R :== ap.R,
	ap.O == attr,
	setscore -5.57594910314632

np-noun--article = article np-noun* %np-noun[article+]%
	C :== article.C,
	Num :== article.Num,
	setscore -4.69134788222914

np-noun--article = article np-noun* %np-noun[article+]%
	C :== article.C,
	Num :== article.Num,
	R :== article.R,
	setscore -2.12639852476761

np-noun--article = article np-noun* %np-noun[article+]%
	C :== article.C,
	R :== article.R,
	setscore -4.69134788222914

np-noun--article = article np-noun* %np-noun[article+]%
	setscore -4.69134788222914

np-noun--article = article np-noun--advx* %np-noun--advx[article+]%
	C :== article.C,
	Num :== article.Num,
	R :== article.R,
	setscore -2.74543773317383

np-noun--article = article np-noun--advx* %np-noun--advx[article+]%
	C :== article.C,
	R :== article.R,
	setscore -4.69134788222914

np-noun--article = article np-noun--ap* %np-noun--ap[article+]%
	C :== article.C,
	setscore -3.59273559356103

np-noun--article = article np-noun--ap* %np-noun--ap[article+]%
	C :== article.C,
	Num :== article.Num,
	setscore -2.74543773317383

np-noun--article = article np-noun--ap* %np-noun--ap[article+]%
	C :== article.C,
	Num :== article.Num,
	R :== article.R,
	setscore -1.13599982073973

np-noun--article = article np-noun--ap* %np-noun--ap[article+]%
	C :== article.C,
	R :== article.R,
	setscore -2.89958841300109

np-noun--article = article np-noun--ap* %np-noun--ap[article+]%
	Num :== article.Num,
	setscore -4.69134788222914

np-noun--article = article np-noun--ap* %np-noun--ap[article+]%
	Num :== article.Num,
	R :== article.R,
	setscore -3.9982007016692

np-noun--article = article np-noun--ap* %np-noun--ap[article+]%
	R :== article.R,
	setscore -4.69134788222914

np-noun--article = article np-noun--ap* %np-noun--ap[article+]%
	setscore -3.59273559356103

np-noun--article = article np-noun--comma* %np-noun--comma[article+]%
	C :== article.C,
	Num :== article.Num,
	R :== article.R,
	setscore -3.59273559356103

np-noun--article = article np-noun--fx* %np-noun--fx[article+]%
	C :== article.C,
	Num :== article.Num,
	R :== article.R,
	setscore -4.69134788222914

np-noun--article = article np-noun--np* %np-noun--np[article+]%
	C :== article.C,
	Num :== article.Num,
	R :== article.R,
	setscore -3.59273559356103

np-noun--article = article np-noun--pp* %np-noun--pp[article+]%
	C :== article.C,
	Num :== article.Num,
	R :== article.R,
	setscore -4.69134788222914

np-noun--comma = comma np-noun* %np-noun[comma+]%
	setscore -1.16315080980568

np-noun--comma = comma np-noun--advx* %np-noun--advx[comma+]%
	setscore -2.77258872223978

np-noun--comma = comma np-noun--ap* %np-noun--ap[comma+]%
	setscore -0.980829253011726

np-noun--comma = comma np-noun--dm* %np-noun--dm[comma+]%
	setscore -2.77258872223978

np-noun--comma = comma np-noun--pron* %np-noun--pron[comma+]%
	setscore -2.77258872223978

np-noun--comma = comma np-noun--simpx* %np-noun--simpx[comma+]%
	setscore -2.77258872223978

np-noun--comma = np-noun* comma %np-noun[+comma]%
	setscore -2.77258872223978

np-noun--conj = conj np-noun* %np-noun[conj+]%
	conj.T == comp,
	setscore -3.27714473299218

np-noun--conj = conj np-noun* %np-noun[conj+]%
	conj.T == coord,
	setscore -3.97029191355212

np-noun--conj = conj np-noun--advx* %np-noun--advx[conj+]%
	conj.T == comp,
	setscore -2.17853244432407

np-noun--conj = conj np-noun--advx* %np-noun--advx[conj+]%
	conj.T == coord,
	setscore -3.97029191355212

np-noun--conj = conj np-noun--ap* %np-noun--ap[conj+]%
	conj.T == comp,
	setscore -1.66770682055808

np-noun--conj = conj np-noun--article* %np-noun--article[conj+]%
	conj.T == comp,
	setscore -1.66770682055808

np-noun--conj = conj np-noun--article* %np-noun--article[conj+]%
	conj.T == coord,
	setscore -3.27714473299218

np-noun--conj = conj np-noun--dp* %np-noun--dp[conj+]%
	conj.T == comp,
	setscore -3.97029191355212

np-noun--conj = conj np-noun--np* %np-noun--np[conj+]%
	conj.T == comp,
	setscore -3.97029191355212

np-noun--conj = conj np-noun--pp* %np-noun--pp[conj+]%
	conj.T == comp,
	setscore -2.87167962488401

np-noun--conj = conj np-noun--pron* %np-noun--pron[conj+]%
	conj.T == comp,
	setscore -1.66770682055808

np-noun--dm = dm np-noun--comma* %np-noun--comma[dm+]%
	setscore 0

np-noun--dp = dp np-noun* %np-noun[dp+]%
	setscore -1.09861228866811

np-noun--dp = dp np-noun--ap* %np-noun--ap[dp+]%
	setscore -1.79175946922806

np-noun--dp = dp np-noun--pron* %np-noun--pron[dp+]%
	setscore -1.79175946922806

np-noun--fx = fx np-noun* %np-noun[fx+]%
	setscore -0.693147180559945

np-noun--np = np np-noun* %np-noun[np+]%
	np.C == acc,
	setscore -3.02042488614436

np-noun--np = np np-noun* %np-noun[np+]%
	np.C == dat,
	setscore -1.92181259747625

np-noun--np = np np-noun* %np-noun[np+]%
	np.C == dat,
	np.Proper == true,
	setscore -3.71357206670431

np-noun--np = np np-noun* %np-noun[np+]%
	np.C == nom,
	setscore -2.32727770558442

np-noun--np = np np-noun* %np-noun[np+]%
	np.C == nom,
	np.Proper == true,
	setscore -3.02042488614436

np-noun--np = np np-noun* %np-noun[np+]%
	np.Cardinal == true,
	setscore -3.71357206670431

np-noun--np = np np-noun* %np-noun[np+]%
	setscore -3.71357206670431

np-noun--np = np np-noun* %np-noun[np:possessive+]%
	setscore -2.6149597780362

np-noun--np = np np-noun--advx* %np-noun--advx[np+]%
	np.C == nom,
	setscore -3.71357206670431

np-noun--np = np np-noun--ap* %np-noun--ap[np+]%
	np.C == nom,
	setscore -3.71357206670431

np-noun--np = np np-noun--ap* %np-noun--ap[np:possessive+]%
	setscore -1.92181259747625

np-noun--np = np np-noun--np* %np-noun--np[np+]%
	np.C == acc,
	setscore -3.71357206670431

np-noun--np = np np-noun--np* %np-noun--np[np+]%
	np.C == dat,
	setscore -3.71357206670431

np-noun--np = np np-noun--np* %np-noun--np[np:possessive+]%
	setscore -3.71357206670431

np-noun--np = np-noun* np %np-noun[+np]%
	np.Cardinal == true,
	setscore -3.71357206670431

np-noun--np = np-noun--comma* np %np-noun--comma[+np]%
	np.C == nom,
	setscore -3.71357206670431

np-noun--num = num np-noun* %np-noun[num+]%
	setscore -0.693147180559945

np-noun--num = num np-noun--ap* %np-noun--ap[num+]%
	setscore -0.693147180559945

np-noun--part = part np-noun* %np-noun[part+]%
	setscore 0

np-noun--pp = pp np-noun* %np-noun[pp:compl1+]%
	setscore -1.73460105538811

np-noun--pp = pp np-noun--advx* %np-noun--advx[pp:compl1+]%
	setscore -1.04145387482816

np-noun--pp = pp np-noun--ap* %np-noun--ap[pp:compl1+]%
	setscore -2.83321334405622

np-noun--pp = pp np-noun--article* %np-noun--article[pp:compl1+]%
	setscore -1.73460105538811

np-noun--pp = pp np-noun--conj* %np-noun--conj[pp:compl1+]%
	setscore -2.83321334405622

np-noun--pp = pp np-noun--np* %np-noun--np[pp:compl1+]%
	setscore -2.83321334405622

np-noun--pp = pp np-noun--pron* %np-noun--pron[pp:compl1+]%
	setscore -2.14006616349627

np-noun--prep = prep np-noun* %np-noun[prep+]%
	setscore 0

np-noun--prn = prn np-noun* %np-noun[prn+]%
	C := prn.C,
	Num := prn.Num,
	P := prn.P,
	R := prn.R,
	setscore 0

np-noun--pron = pron np-noun* %np-noun[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	setscore -4.78749174278205

np-noun--pron = pron np-noun* %np-noun[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == demons,
	setscore -3.17805383034795

np-noun--pron = pron np-noun* %np-noun[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == indef,
	setscore -2.59026716544583

np-noun--pron = pron np-noun* %np-noun[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == poss,
	setscore -3.17805383034795

np-noun--pron = pron np-noun* %np-noun[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == rel,
	setscore -4.78749174278205

np-noun--pron = pron np-noun* %np-noun[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == sub,
	pron.Typ == indef,
	setscore -4.78749174278205

np-noun--pron = pron np-noun* %np-noun[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	pron.O == attr,
	pron.Typ == demons,
	setscore -4.78749174278205

np-noun--pron = pron np-noun* %np-noun[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	pron.O == attr,
	pron.Typ == poss,
	setscore -4.78749174278205

np-noun--pron = pron np-noun* %np-noun[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	pron.O == attr,
	pron.Typ == rel,
	setscore -4.78749174278205

np-noun--pron = pron np-noun* %np-noun[pron+]%
	C :== pron.C,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == demons,
	setscore -4.78749174278205

np-noun--pron = pron np-noun* %np-noun[pron+]%
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == demons,
	setscore -4.0943445622221

np-noun--pron = pron np-noun* %np-noun[pron+]%
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == poss,
	setscore -4.78749174278205

np-noun--pron = pron np-noun* %np-noun[pron+]%
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == rel,
	setscore -4.78749174278205

np-noun--pron = pron np-noun* %np-noun[pron+]%
	Num :== pron.Num,
	pron.O == attr,
	pron.Typ == demons,
	setscore -4.78749174278205

np-noun--pron = pron np-noun* %np-noun[pron+]%
	Num :== pron.Num,
	pron.O == attr,
	pron.Typ == rel,
	setscore -4.78749174278205

np-noun--pron = pron np-noun* %np-noun[pron+]%
	R :== pron.R,
	pron.O == attr,
	pron.Typ == demons,
	setscore -4.78749174278205

np-noun--pron = pron np-noun* %np-noun[pron+]%
	R :== pron.R,
	pron.O == attr,
	pron.Typ == rel,
	setscore -4.0943445622221

np-noun--pron = pron np-noun* %np-noun[pron+]%
	pron.O == attr,
	pron.Typ == demons,
	setscore -4.78749174278205

np-noun--pron = pron np-noun* %np-noun[pron+]%
	pron.O == attr,
	pron.Typ == rel,
	setscore -4.78749174278205

np-noun--pron = pron np-noun--advx* %np-noun--advx[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == demons,
	setscore -4.78749174278205

np-noun--pron = pron np-noun--advx* %np-noun--advx[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == poss,
	setscore -4.78749174278205

np-noun--pron = pron np-noun--ap* %np-noun--ap[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	setscore -4.78749174278205

np-noun--pron = pron np-noun--ap* %np-noun--ap[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == demons,
	setscore -2.99573227355399

np-noun--pron = pron np-noun--ap* %np-noun--ap[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == indef,
	setscore -2.59026716544583

np-noun--pron = pron np-noun--ap* %np-noun--ap[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == poss,
	setscore -2.38959646998368

np-noun--pron = pron np-noun--ap* %np-noun--ap[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	pron.O == attr,
	pron.Typ == demons,
	setscore -4.78749174278205

np-noun--pron = pron np-noun--ap* %np-noun--ap[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	pron.O == attr,
	pron.Typ == poss,
	setscore -4.0943445622221

np-noun--pron = pron np-noun--ap* %np-noun--ap[pron+]%
	C :== pron.C,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == demons,
	setscore -4.78749174278205

np-noun--pron = pron np-noun--ap* %np-noun--ap[pron+]%
	C :== pron.C,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == poss,
	setscore -4.78749174278205

np-noun--pron = pron np-noun--ap* %np-noun--ap[pron+]%
	C :== pron.C,
	pron.O == attr,
	pron.Typ == indef,
	setscore -4.78749174278205

np-noun--pron = pron np-noun--ap* %np-noun--ap[pron+]%
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == demons,
	setscore -4.0943445622221

np-noun--pron = pron np-noun--ap* %np-noun--ap[pron+]%
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == indef,
	setscore -4.78749174278205

np-noun--pron = pron np-noun--ap* %np-noun--ap[pron+]%
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == poss,
	setscore -4.78749174278205

np-noun--pron = pron np-noun--ap* %np-noun--ap[pron+]%
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == rel,
	setscore -4.78749174278205

np-noun--pron = pron np-noun--ap* %np-noun--ap[pron+]%
	Num :== pron.Num,
	pron.O == attr,
	pron.Typ == demons,
	setscore -4.78749174278205

np-noun--pron = pron np-noun--ap* %np-noun--ap[pron+]%
	Num :== pron.Num,
	pron.O == attr,
	pron.Typ == rel,
	setscore -4.78749174278205

np-noun--pron = pron np-noun--ap* %np-noun--ap[pron+]%
	R :== pron.R,
	pron.O == attr,
	pron.Typ == demons,
	setscore -4.78749174278205

np-noun--pron = pron np-noun--ap* %np-noun--ap[pron+]%
	R :== pron.R,
	pron.O == attr,
	pron.Typ == rel,
	setscore -4.78749174278205

np-noun--pron = pron np-noun--ap* %np-noun--ap[pron+]%
	pron.O == attr,
	pron.Typ == demons,
	setscore -4.0943445622221

np-noun--pron = pron np-noun--ap* %np-noun--ap[pron+]%
	pron.O == attr,
	pron.Typ == rel,
	setscore -4.78749174278205

np-noun--pron = pron np-noun--article* %np-noun--article[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == indef,
	setscore -3.40119738166216

np-noun--pron = pron np-noun--comma* %np-noun--comma[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == demons,
	setscore -4.0943445622221

np-noun--pron = pron np-noun--comma* %np-noun--comma[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == indef,
	setscore -4.0943445622221

np-noun--pron = pron np-noun--comma* %np-noun--comma[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == poss,
	setscore -4.0943445622221

np-noun--pron = pron np-noun--pron* %np-noun--pron[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == demons,
	setscore -4.0943445622221

np-noun--pron = pron np-noun--pron* %np-noun--pron[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == indef,
	setscore -2.99573227355399

np-noun--simpx = simpx np-noun* %np-noun[simpx+]%
	setscore -1.09861228866811

np-noun--simpx = simpx np-noun--comma* %np-noun--comma[simpx+]%
	setscore -1.09861228866811

np-np = np* %NP[np:app]%
	Block := 1,
	setscore -2.18262577791507

np-np = np* %NP[np:konj]%
	Block := 1,
	Num := any,
	P := any,
	R := any,
	setscore -1.82161243237774

np-np = np* %NP[np:possessive]%
	Block := 1,
	setscore -6.01126717440416

np-np = np* %NP[np:possessive]%
	Block := 1,
	Proper := true,
	setscore -6.01126717440416

np-np = np* %NP[np]%
	Block := 1,
	setscore -0.341386251423642

np-np = np* %NP[np]%
	Block := 1,
	Num := any,
	P := any,
	R := any,
	setscore -5.31811999384422

np-np = np* %NP[np]%
	Block := 1,
	Proper := true,
	setscore -5.31811999384422

np-np--advx = advx np-np* %np-np[advx+]%
	setscore -1.05812092731337

np-np--advx = advx np-np--advx* %np-np--advx[advx+]%
	setscore -2.39789527279837

np-np--advx = advx np-np--ap* %np-np--ap[advx+]%
	setscore -3.40949618447685

np-np--advx = advx np-np--article* %np-np--article[advx+]%
	setscore -4.1026433650368

np-np--advx = advx np-np--comma* %np-np--comma[advx+]%
	setscore -4.1026433650368

np-np--advx = advx np-np--conj* %np-np--conj[advx+]%
	setscore -3.00403107636869

np-np--advx = advx np-np--pp* %np-np--pp[advx+]%
	setscore -4.1026433650368

np-np--advx = advx np-np--simpx* %np-np--simpx[advx+]%
	setscore -4.79579054559674

np-np--advx = np-np* advx %np-np[+advx:app]%
	setscore -4.79579054559674

np-np--advx = np-np* advx %np-np[+advx:konj]%
	setscore -4.79579054559674

np-np--advx = np-np* advx %np-np[+advx]%
	setscore -3.00403107636869

np-np--advx = np-np--advx* advx %np-np--advx[+advx]%
	setscore -2.39789527279837

np-np--advx = np-np--comma* advx %np-np--comma[+advx:konj]%
	setscore -4.79579054559674

np-np--advx = np-np--comma* advx %np-np--comma[+advx]%
	setscore -3.40949618447685

np-np--advx = np-np--conj* advx %np-np--conj[+advx:konj]%
	setscore -4.1026433650368

np-np--advx = np-np--conj* advx %np-np--conj[+advx]%
	setscore -2.59856596826052

np-np--advx = np-np--fx* advx %np-np--fx[+advx]%
	setscore -4.79579054559674

np-np--advx = np-np--np* advx %np-np--np[+advx:konj]%
	setscore -4.1026433650368

np-np--advx = np-np--pp* advx %np-np--pp[+advx]%
	setscore -3.18635263316264

np-np--ap = ap np-np* %np-np[ap+]%
	C :== ap.C,
	Num :== ap.Num,
	R :== ap.R,
	ap.O == attr,
	setscore -0.796331416795176

np-np--ap = ap np-np* %np-np[ap+]%
	C :== ap.C,
	Num :== ap.Num,
	ap.O == attr,
	setscore -3.23867845216438

np-np--ap = ap np-np* %np-np[ap+]%
	C :== ap.C,
	R :== ap.R,
	ap.O == attr,
	setscore -3.93182563272433

np-np--ap = ap np-np* %np-np[ap+]%
	ap.O == attr,
	setscore -3.93182563272433

np-np--ap = ap np-np* %np-np[ap+]%
	setscore -3.23867845216438

np-np--ap = ap np-np--ap* %np-np--ap[ap+]%
	C :== ap.C,
	Num :== ap.Num,
	R :== ap.R,
	ap.O == attr,
	setscore -2.54553127160444

np-np--ap = ap np-np--ap* %np-np--ap[ap+]%
	Num :== ap.Num,
	R :== ap.R,
	ap.O == attr,
	setscore -3.93182563272433

np-np--ap = np-np* ap %np-np[+ap:app]%
	setscore -3.93182563272433

np-np--ap = np-np* ap %np-np[+ap:konj]%
	setscore -3.93182563272433

np-np--ap = np-np--comma* ap %np-np--comma[+ap:konj]%
	setscore -3.23867845216438

np-np--ap = np-np--conj* ap %np-np--conj[+ap:konj]%
	setscore -2.83321334405622

np-np--article = article np-np* %np-np[article+]%
	C :== article.C,
	setscore -3.66356164612965

np-np--article = article np-np* %np-np[article+]%
	C :== article.C,
	Num :== article.Num,
	setscore -3.66356164612965

np-np--article = article np-np* %np-np[article+]%
	C :== article.C,
	Num :== article.Num,
	R :== article.R,
	setscore -1.71765149707433

np-np--article = article np-np* %np-np[article+]%
	C :== article.C,
	R :== article.R,
	setscore -2.9704144655697

np-np--article = article np-np* %np-np[article+]%
	Num :== article.Num,
	setscore -3.66356164612965

np-np--article = article np-np* %np-np[article+]%
	setscore -3.66356164612965

np-np--article = article np-np--advx* %np-np--advx[article+]%
	C :== article.C,
	Num :== article.Num,
	R :== article.R,
	setscore -2.9704144655697

np-np--article = article np-np--ap* %np-np--ap[article+]%
	C :== article.C,
	Num :== article.Num,
	setscore -2.56494935746154

np-np--article = article np-np--ap* %np-np--ap[article+]%
	C :== article.C,
	Num :== article.Num,
	R :== article.R,
	setscore -1.3609765531356

np-np--article = article np-np--ap* %np-np--ap[article+]%
	C :== article.C,
	R :== article.R,
	setscore -3.66356164612965

np-np--article = article np-np--ap* %np-np--ap[article+]%
	setscore -3.66356164612965

np-np--article = article np-np--np* %np-np--np[article+]%
	C :== article.C,
	Num :== article.Num,
	R :== article.R,
	setscore -2.27726728500976

np-np--comma = comma np-np* %np-np[comma+]%
	setscore -2.56494935746154

np-np--comma = comma np-np--advx* %np-np--advx[comma+]%
	setscore -4.51085950651685

np-np--comma = comma np-np--pp* %np-np--pp[comma+]%
	setscore -4.51085950651685

np-np--comma = np-np* comma %np-np[+comma]%
	setscore -0.540567592964728

np-np--comma = np-np--advx* comma %np-np--advx[+comma]%
	setscore -2.56494935746154

np-np--comma = np-np--conj* comma %np-np--conj[+comma]%
	setscore -2.7191000372888

np-np--comma = np-np--fx* comma %np-np--fx[+comma]%
	setscore -3.41224721784874

np-np--comma = np-np--np* comma %np-np--np[+comma]%
	setscore -3.12456514539696

np-np--comma = np-np--pp* comma %np-np--pp[+comma]%
	setscore -2.56494935746154

np-np--comma = np-np--simpx* comma %np-np--simpx[+comma]%
	setscore -3.8177123259569

np-np--conj = conj np-np* %np-np[conj+]%
	conj.T == comp,
	setscore -1.52605630349505

np-np--conj = conj np-np* %np-np[conj+]%
	conj.T == coord,
	setscore -2.54770755102703

np-np--conj = conj np-np--advx* %np-np--advx[conj+]%
	conj.T == comp,
	setscore -3.35863776724336

np-np--conj = conj np-np--advx* %np-np--advx[conj+]%
	conj.T == coord,
	setscore -3.64631983969514

np-np--conj = conj np-np--ap* %np-np--ap[conj+]%
	conj.T == comp,
	setscore -3.35863776724336

np-np--conj = conj np-np--article* %np-np--article[conj+]%
	conj.T == comp,
	setscore -4.0517849478033

np-np--conj = conj np-np--np* %np-np--np[conj+]%
	conj.T == comp,
	setscore -4.0517849478033

np-np--conj = conj np-np--np* %np-np--np[conj+]%
	conj.T == coord,
	setscore -4.74493212836325

np-np--conj = conj np-np--pp* %np-np--pp[conj+]%
	conj.T == comp,
	setscore -3.64631983969514

np-np--conj = conj np-np--pp* %np-np--pp[conj+]%
	conj.T == coord,
	setscore -4.0517849478033

np-np--conj = np-np* conj %np-np[+conj]%
	conj.T == comp,
	setscore -4.0517849478033

np-np--conj = np-np* conj %np-np[+conj]%
	conj.T == coord,
	setscore -1.70040969063983

np-np--conj = np-np--advx* conj %np-np--advx[+conj]%
	conj.T == coord,
	setscore -4.0517849478033

np-np--conj = np-np--comma* conj %np-np--comma[+conj]%
	conj.T == coord,
	setscore -2.4423470353692

np-np--conj = np-np--fx* conj %np-np--fx[+conj]%
	conj.T == coord,
	setscore -4.74493212836325

np-np--conj = np-np--np* conj %np-np--np[+conj]%
	conj.T == coord,
	setscore -2.9531726591352

np-np--conj = np-np--simpx* conj %np-np--simpx[+conj]%
	conj.T == coord,
	setscore -4.74493212836325

np-np--dp = dp np-np* %np-np[dp+]%
	setscore 0

np-np--fx = fx np-np* %np-np[fx+]%
	setscore -1.74919985480926

np-np--fx = fx np-np--conj* %np-np--conj[fx+]%
	setscore -3.13549421592915

np-np--fx = np-np* fx %np-np[+fx:app]%
	setscore -2.4423470353692

np-np--fx = np-np* fx %np-np[+fx:konj]%
	setscore -3.13549421592915

np-np--fx = np-np* fx %np-np[+fx]%
	setscore -3.13549421592915

np-np--fx = np-np--advx* fx %np-np--advx[+fx]%
	setscore -3.13549421592915

np-np--fx = np-np--comma* fx %np-np--comma[+fx:app]%
	setscore -3.13549421592915

np-np--fx = np-np--comma* fx %np-np--comma[+fx:konj]%
	setscore -2.03688192726104

np-np--fx = np-np--comma* fx %np-np--comma[+fx]%
	setscore -3.13549421592915

np-np--fx = np-np--conj* fx %np-np--conj[+fx:konj]%
	setscore -2.4423470353692

np-np--fx = np-np--conj* fx %np-np--conj[+fx]%
	setscore -3.13549421592915

np-np--noun = np-np* noun %np-np[+noun]%
	setscore 0

np-np--np = np np-np* %np-np[np+]%
	np.C == acc,
	setscore -5.20948615284142

np-np--np = np np-np* %np-np[np+]%
	np.C == acc,
	np.Proper == true,
	setscore -5.20948615284142

np-np--np = np np-np* %np-np[np+]%
	np.C == dat,
	setscore -4.11087386417331

np-np--np = np np-np* %np-np[np+]%
	np.C == dat,
	np.Proper == true,
	setscore -5.20948615284142

np-np--np = np np-np* %np-np[np+]%
	np.C == nom,
	setscore -3.82319179172153

np-np--np = np np-np* %np-np[np+]%
	np.C == nom,
	np.Proper == true,
	setscore -5.20948615284142

np-np--np = np np-np* %np-np[np+]%
	np.Cardinal == true,
	setscore -5.20948615284142

np-np--np = np np-np* %np-np[np+]%
	setscore -5.20948615284142

np-np--np = np np-np* %np-np[np:possessive+]%
	setscore -3.60004824040732

np-np--np = np np-np--advx* %np-np--advx[np+]%
	np.C == dat,
	setscore -5.20948615284142

np-np--np = np np-np--comma* %np-np--comma[np+]%
	np.C == nom,
	setscore -5.20948615284142

np-np--np = np np-np--comma* %np-np--comma[np:possessive+]%
	setscore -5.20948615284142

np-np--np = np np-np--conj* %np-np--conj[np+]%
	np.C == acc,
	setscore -5.20948615284142

np-np--np = np np-np--conj* %np-np--conj[np:konj+]%
	C := np.C,
	setscore -5.20948615284142

np-np--np = np-np* np %np-np[+np:app]%
	C == np.C,
	Num == np.Num,
	setscore -5.20948615284142

np-np--np = np-np* np %np-np[+np:app]%
	C == np.C,
	Num == np.Num,
	P == np.P,
	setscore -5.20948615284142

np-np--np = np-np* np %np-np[+np:app]%
	C == np.C,
	P == np.P,
	setscore -5.20948615284142

np-np--np = np-np* np %np-np[+np:app]%
	Num == np.Num,
	P == np.P,
	setscore -5.20948615284142

np-np--np = np-np* np %np-np[+np:app]%
	P == np.P,
	setscore -5.20948615284142

np-np--np = np-np* np %np-np[+np:app]%
	setscore -4.11087386417331

np-np--np = np-np* np %np-np[+np:konj]%
	C := np.C,
	setscore -5.20948615284142

np-np--np = np-np* np %np-np[+np:konj]%
	C == np.C,
	setscore -5.20948615284142

np-np--np = np-np* np %np-np[+np:konj]%
	setscore -4.51633897228148

np-np--np = np-np* np %np-np[+np:possessive]%
	setscore -4.11087386417331

np-np--np = np-np* np %np-np[+np]%
	np.C == acc,
	setscore -5.20948615284142

np-np--np = np-np* np %np-np[+np]%
	np.C == acc,
	np.Proper == true,
	setscore -5.20948615284142

np-np--np = np-np* np %np-np[+np]%
	np.C == dat,
	setscore -5.20948615284142

np-np--np = np-np* np %np-np[+np]%
	np.C == dat,
	np.Proper == true,
	setscore -5.20948615284142

np-np--np = np-np* np %np-np[+np]%
	np.C == nom,
	setscore -5.20948615284142

np-np--np = np-np* np %np-np[+np]%
	np.C == nom,
	np.Proper == true,
	setscore -5.20948615284142

np-np--np = np-np* np %np-np[+np]%
	np.Cardinal == true,
	setscore -5.20948615284142

np-np--np = np-np* np %np-np[+np]%
	setscore -4.51633897228148

np-np--np = np-np--advx* np %np-np--advx[+np:konj]%
	C == np.C,
	setscore -3.41772668361337

np-np--np = np-np--advx* np %np-np--advx[+np:possessive]%
	setscore -4.11087386417331

np-np--np = np-np--advx* np %np-np--advx[+np]%
	np.C == acc,
	setscore -5.20948615284142

np-np--np = np-np--advx* np %np-np--advx[+np]%
	np.C == dat,
	setscore -5.20948615284142

np-np--np = np-np--advx* np %np-np--advx[+np]%
	np.C == nom,
	setscore -4.51633897228148

np-np--np = np-np--advx* np %np-np--advx[+np]%
	np.Cardinal == true,
	setscore -5.20948615284142

np-np--np = np-np--ap* np %np-np--ap[+np:konj]%
	C == np.C,
	setscore -5.20948615284142

np-np--np = np-np--article* np %np-np--article[+np:possessive]%
	setscore -5.20948615284142

np-np--np = np-np--comma* np %np-np--comma[+np:app]%
	C == np.C,
	Num == np.Num,
	setscore -5.20948615284142

np-np--np = np-np--comma* np %np-np--comma[+np:app]%
	C == np.C,
	Num == np.Num,
	P == np.P,
	setscore -5.20948615284142

np-np--np = np-np--comma* np %np-np--comma[+np:app]%
	C == np.C,
	P == np.P,
	setscore -5.20948615284142

np-np--np = np-np--comma* np %np-np--comma[+np:app]%
	Num == np.Num,
	P == np.P,
	setscore -5.20948615284142

np-np--np = np-np--comma* np %np-np--comma[+np:app]%
	setscore -4.51633897228148

np-np--np = np-np--comma* np %np-np--comma[+np:konj]%
	C == np.C,
	setscore -2.81159088004305

np-np--np = np-np--comma* np %np-np--comma[+np:konj]%
	setscore -3.82319179172153

np-np--np = np-np--comma* np %np-np--comma[+np:possessive]%
	setscore -5.20948615284142

np-np--np = np-np--comma* np %np-np--comma[+np]%
	np.C == acc,
	setscore -4.51633897228148

np-np--np = np-np--comma* np %np-np--comma[+np]%
	np.C == dat,
	setscore -5.20948615284142

np-np--np = np-np--comma* np %np-np--comma[+np]%
	np.C == dat,
	np.Proper == true,
	setscore -5.20948615284142

np-np--np = np-np--comma* np %np-np--comma[+np]%
	np.C == nom,
	setscore -3.60004824040732

np-np--np = np-np--comma* np %np-np--comma[+np]%
	np.C == nom,
	np.Proper == true,
	setscore -4.51633897228148

np-np--np = np-np--comma* np %np-np--comma[+np]%
	np.Cardinal == true,
	setscore -5.20948615284142

np-np--np = np-np--comma* np %np-np--comma[+np]%
	setscore -5.20948615284142

np-np--np = np-np--conj* np %np-np--conj[+np:konj]%
	C == np.C,
	setscore -3.26357600378611

np-np--np = np-np--conj* np %np-np--conj[+np:konj]%
	setscore -3.26357600378611

np-np--np = np-np--conj* np %np-np--conj[+np:possessive]%
	setscore -3.60004824040732

np-np--np = np-np--conj* np %np-np--conj[+np]%
	np.C == acc,
	setscore -5.20948615284142

np-np--np = np-np--conj* np %np-np--conj[+np]%
	np.C == dat,
	setscore -4.51633897228148

np-np--np = np-np--conj* np %np-np--conj[+np]%
	np.C == nom,
	setscore -4.51633897228148

np-np--np = np-np--conj* np %np-np--conj[+np]%
	np.Cardinal == true,
	setscore -5.20948615284142

np-np--np = np-np--conj* np %np-np--conj[+np]%
	setscore -4.51633897228148

np-np--np = np-np--pp* np %np-np--pp[+np:possessive]%
	setscore -3.60004824040732

np-np--np = np-np--pp* np %np-np--pp[+np]%
	np.C == acc,
	setscore -5.20948615284142

np-np--np = np-np--pp* np %np-np--pp[+np]%
	np.C == nom,
	setscore -4.51633897228148

np-np--np = np-np--pp* np %np-np--pp[+np]%
	np.Cardinal == true,
	setscore -5.20948615284142

np-np--pp = np-np* pp %np-np[+pp:app]%
	setscore -4.59511985013459

np-np--pp = np-np* pp %np-np[+pp:compl1]%
	setscore -2.51567830845475

np-np--pp = np-np* pp %np-np[+pp:konj]%
	setscore -4.59511985013459

np-np--pp = np-np--advx* pp %np-np--advx[+pp:compl1]%
	setscore -2.19722457733622

np-np--pp = np-np--ap* pp %np-np--ap[+pp:compl1]%
	setscore -4.59511985013459

np-np--pp = np-np--article* pp %np-np--article[+pp:compl1]%
	setscore -4.59511985013459

np-np--pp = np-np--comma* pp %np-np--comma[+pp:compl1]%
	setscore -3.2088254890147

np-np--pp = np-np--comma* pp %np-np--comma[+pp:konj]%
	setscore -3.90197266957464

np-np--pp = np-np--conj* pp %np-np--conj[+pp:compl1]%
	setscore -2.39789527279837

np-np--pp = np-np--conj* pp %np-np--conj[+pp:konj]%
	setscore -3.49650756146648

np-np--pp = np-np--fx* pp %np-np--fx[+pp:compl1]%
	setscore -4.59511985013459

np-np--pp = np-np--np* pp %np-np--np[+pp:compl1]%
	setscore -2.98568193770049

np-np--pp = np-np--pp* pp %np-np--pp[+pp:compl1]%
	setscore -2.64920970107928

np-np--pp = pp np-np* %np-np[pp:compl1+]%
	setscore -1.82253112789481

np-np--pp = pp np-np--advx* %np-np--advx[pp:compl1+]%
	setscore -2.39789527279837

np-np--pp = pp np-np--article* %np-np--article[pp:compl1+]%
	setscore -3.90197266957464

np-np--pp = pp np-np--comma* %np-np--comma[pp:compl1+]%
	setscore -3.49650756146648

np-np--pp = pp np-np--conj* %np-np--conj[pp:compl1+]%
	setscore -3.90197266957464

np-np--pp = pp np-np--pp* %np-np--pp[pp:compl1+]%
	setscore -4.59511985013459

np-np--pp = pp np-np--pron* %np-np--pron[pp:compl1+]%
	setscore -4.59511985013459

np-np--prn = np-np* prn %np-np[+prn]%
	C := prn.C,
	Num := prn.Num,
	P := prn.P,
	R := prn.R,
	setscore -0.510825623765991

np-np--prn = np-np--prn* prn %np-np--prn[+prn]%
	setscore -0.916290731874155

np-np--pron = np-np* pron %np-np[+pron]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == poss,
	setscore -3.25809653802148

np-np--pron = pron np-np* %np-np[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	setscore -3.25809653802148

np-np--pron = pron np-np* %np-np[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == demons,
	setscore -2.56494935746154

np-np--pron = pron np-np* %np-np[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == indef,
	setscore -3.25809653802148

np-np--pron = pron np-np* %np-np[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == poss,
	setscore -2.56494935746154

np-np--pron = pron np-np* %np-np[pron+]%
	C :== pron.C,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == poss,
	setscore -3.25809653802148

np-np--pron = pron np-np* %np-np[pron+]%
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == rel,
	setscore -3.25809653802148

np-np--pron = pron np-np* %np-np[pron+]%
	Num :== pron.Num,
	pron.O == attr,
	pron.Typ == demons,
	setscore -3.25809653802148

np-np--pron = pron np-np--ap* %np-np--ap[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	setscore -3.25809653802148

np-np--pron = pron np-np--ap* %np-np--ap[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == demons,
	setscore -3.25809653802148

np-np--pron = pron np-np--ap* %np-np--ap[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == indef,
	setscore -3.25809653802148

np-np--pron = pron np-np--ap* %np-np--ap[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == poss,
	setscore -2.15948424935337

np-np--pron = pron np-np--article* %np-np--article[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == indef,
	setscore -2.56494935746154

np-np--pron = pron np-np--article* %np-np--article[pron+]%
	pron.O == attr,
	pron.Typ == indef,
	setscore -3.25809653802148

np-np--pron = pron np-np--fx* %np-np--fx[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == poss,
	setscore -3.25809653802148

np-np--pron = pron np-np--pron* %np-np--pron[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == indef,
	setscore -2.56494935746154

np-np--r-simpx = np-np* r-simpx %np-np[+r-simpx]%
	setscore -2.77258872223978

np-np--r-simpx = np-np--advx* r-simpx %np-np--advx[+r-simpx]%
	setscore -2.77258872223978

np-np--r-simpx = np-np--comma* r-simpx %np-np--comma[+r-simpx]%
	setscore -0.575364144903562

np-np--r-simpx = np-np--conj* r-simpx %np-np--conj[+r-simpx]%
	setscore -2.07944154167984

np-np--simpx = np-np* simpx %np-np[+simpx:app]%
	setscore -3.40119738166216

np-np--simpx = np-np* simpx %np-np[+simpx:konj]%
	setscore -3.40119738166216

np-np--simpx = np-np* simpx %np-np[+simpx]%
	setscore -3.40119738166216

np-np--simpx = np-np--comma* simpx %np-np--comma[+simpx:app]%
	setscore -3.40119738166216

np-np--simpx = np-np--comma* simpx %np-np--comma[+simpx:konj]%
	setscore -2.30258509299405

np-np--simpx = np-np--comma* simpx %np-np--comma[+simpx]%
	setscore -1.79175946922806

np-np--simpx = np-np--conj* simpx %np-np--conj[+simpx:konj]%
	setscore -1.45528723260684

np-np--simpx = np-np--np* simpx %np-np--np[+simpx]%
	setscore -3.40119738166216

np-np--simpx = simpx np-np* %np-np[simpx+]%
	setscore -2.70805020110221

np-np--simpx = simpx np-np--comma* %np-np--comma[simpx+]%
	setscore -2.70805020110221

np-np--unknown = np-np--comma* unknown %np-np--comma[+unknown]%
	setscore -0.693147180559945

np-np--unknown = np-np--unknown* unknown %np-np--unknown[+unknown]%
	setscore -0.693147180559945

np-np--vp = np-np* vp %np-np[+vp:app]%
	setscore -1.09861228866811

np-np--vp = np-np--conj* vp %np-np--conj[+vp:konj]%
	setscore -1.09861228866811

np-num = num* %NP[num]%
	Block := 1,
	setscore -2.01490302054226

np-num = num* %NP[num]%
	Block := 1,
	Cardinal := true,
	setscore -0.143100843640673

np-num--advx = advx np-num* %np-num[advx+]%
	setscore -0.693147180559945

np-num--advx = advx np-num--advx* %np-num--advx[advx+]%
	setscore -1.79175946922806

np-num--advx = advx np-num--article* %np-num--article[advx+]%
	setscore -1.79175946922806

np-num--advx = advx np-num--pron* %np-num--pron[advx+]%
	setscore -1.79175946922806

np-num--ap = ap np-num* %np-num[ap+]%
	setscore 0

np-num--article = article np-num* %np-num[article+]%
	setscore -0.405465108108164

np-num--article = article np-num--ap* %np-num--ap[article+]%
	setscore -1.09861228866811

np-num--conj = conj np-num* %np-num[conj+]%
	conj.T == comp,
	setscore -0.693147180559945

np-num--conj = conj np-num--advx* %np-num--advx[conj+]%
	conj.T == comp,
	setscore -0.693147180559945

np-num--noun = np-num* noun %np-num[+noun]%
	setscore 0

np-num--np = np np-num* %np-num[np+]%
	setscore 0

np-num--num = np-num* num %np-num[+num]%
	setscore 0

np-num--pp = pp np-num* %np-num[pp:compl1+]%
	setscore 0

np-num--pron = np-num--noun* pron %np-num--noun[+pron]%
	setscore -1.09861228866811

np-num--pron = pron np-num* %np-num[pron+]%
	setscore -0.405465108108164

np-part = part* %NP[part]%
	Block := 1,
	Num := any,
	P := any,
	R := any,
	setscore 0

np-part--advx = np-part* advx %np-part[+advx]%
	setscore 0

np-part--comma = np-part--np* comma %np-part--np[+comma]%
	setscore 0

np-part--conj = np-part--comma* conj %np-part--comma[+conj]%
	conj.T == coord,
	setscore 0

np-part--np = np-part* np %np-part[+np:konj]%
	C := np.C,
	setscore -1.09861228866811

np-part--np = np-part--advx* np %np-part--advx[+np:konj]%
	C := np.C,
	setscore -1.09861228866811

np-part--np = np-part--pp* np %np-part--pp[+np:konj]%
	C == np.C,
	setscore -1.09861228866811

np-part--pp = np-part--conj* pp %np-part--conj[+pp:compl1]%
	setscore 0

np-pp = pp* %NP[pp:app]%
	Block := 1,
	setscore -2.19722457733622

np-pp = pp* %NP[pp:compl1]%
	Block := 1,
	setscore -0.587786664902119

np-pp = pp* %NP[pp]%
	Block := 1,
	setscore -1.09861228866811

np-pp--ap = np-pp* ap %np-pp[+ap]%
	setscore -0.693147180559945

np-pp--ap = np-pp--article* ap %np-pp--article[+ap]%
	setscore -0.693147180559945

np-pp--article = np-pp* article %np-pp[+article]%
	setscore 0

np-pp--comma = np-pp* comma %np-pp[+comma]%
	setscore 0

np-pp--np = np-pp* np %np-pp[+np:app]%
	setscore -0.693147180559945

np-pp--np = np-pp* np %np-pp[+np]%
	setscore -0.693147180559945

np-pp--pp = np-pp* pp %np-pp[+pp:compl1]%
	setscore 0

np-pp--pron = np-pp* pron %np-pp[+pron]%
	setscore -0.693147180559945

np-pp--pron = pron np-pp* %np-pp[pron+]%
	setscore -0.693147180559945

np-pp--r-simpx = np-pp--comma* r-simpx %np-pp--comma[+r-simpx]%
	setscore 0

np-prn = prn* %NP[prn:konj]%
	Block := 1,
	setscore -4.07753744390572

np-prn = prn* %NP[prn]%
	Block := 1,
	setscore -0.185717145795093

np-prn = prn* %NP[prn]%
	Block := 1,
	Proper := true,
	setscore -1.8803128665695

np-prn--advx = advx np-prn* %np-prn[advx+]%
	setscore -1.09861228866811

np-prn--advx = advx np-prn--advx* %np-prn--advx[advx+]%
	setscore -2.19722457733622

np-prn--advx = advx np-prn--article* %np-prn--article[advx+]%
	setscore -1.50407739677627

np-prn--advx = advx np-prn--pron* %np-prn--pron[advx+]%
	setscore -2.19722457733622

np-prn--ap = ap np-prn* %np-prn[ap+]%
	C :== ap.C,
	Num :== ap.Num,
	R :== ap.R,
	ap.O == attr,
	setscore -0.538996500732687

np-prn--ap = ap np-prn* %np-prn[ap+]%
	C :== ap.C,
	ap.O == attr,
	setscore -3.17805383034795

np-prn--ap = ap np-prn--ap* %np-prn--ap[ap+]%
	C :== ap.C,
	Num :== ap.Num,
	R :== ap.R,
	ap.O == attr,
	setscore -2.07944154167984

np-prn--article = article np-prn* %np-prn[article+]%
	C :== article.C,
	Num :== article.Num,
	setscore -2.83321334405622

np-prn--article = article np-prn* %np-prn[article+]%
	C :== article.C,
	Num :== article.Num,
	R :== article.R,
	setscore -1.44691898293633

np-prn--article = article np-prn--ap* %np-prn--ap[article+]%
	C :== article.C,
	Num :== article.Num,
	setscore -2.83321334405622

np-prn--article = article np-prn--ap* %np-prn--ap[article+]%
	C :== article.C,
	Num :== article.Num,
	R :== article.R,
	setscore -1.04145387482816

np-prn--article = article np-prn--comma* %np-prn--comma[article+]%
	C :== article.C,
	Num :== article.Num,
	R :== article.R,
	setscore -2.83321334405622

np-prn--comma = comma np-prn--ap* %np-prn--ap[comma+]%
	setscore -0.693147180559945

np-prn--comma = np-prn* comma %np-prn[+comma]%
	setscore -0.693147180559945

np-prn--conj = conj np-prn* %np-prn[conj+]%
	conj.T == comp,
	setscore -1.94591014905531

np-prn--conj = conj np-prn--advx* %np-prn--advx[conj+]%
	conj.T == comp,
	setscore -1.94591014905531

np-prn--conj = conj np-prn--article* %np-prn--article[conj+]%
	conj.T == comp,
	setscore -1.25276296849537

np-prn--conj = np-prn* conj %np-prn[+conj]%
	conj.T == coord,
	setscore -1.94591014905531

np-prn--dp = dp np-prn* %np-prn[dp+]%
	setscore -0.693147180559945

np-prn--dp = dp np-prn--ap* %np-prn--ap[dp+]%
	setscore -0.693147180559945

np-prn--fx = fx np-prn* %np-prn[fx+]%
	setscore 0

np-prn--noun = np-prn--conj* noun %np-prn--conj[+noun:konj]%
	setscore 0

np-prn--np = np np-prn* %np-prn[np+]%
	setscore -1.94591014905531

np-prn--np = np np-prn* %np-prn[np:possessive+]%
	setscore -1.94591014905531

np-prn--np = np-prn* np %np-prn[+np]%
	setscore -1.94591014905531

np-prn--pp = np-prn* pp %np-prn[+pp:compl1]%
	setscore -1.09861228866811

np-prn--pp = np-prn--prn* pp %np-prn--prn[+pp:compl1]%
	setscore -1.09861228866811

np-prn--pp = pp np-prn* %np-prn[pp:compl1+]%
	setscore -1.09861228866811

np-prn--prn = np-prn* prn %np-prn[+prn]%
	setscore -1.09861228866811

np-prn--prn = np-prn--comma* prn %np-prn--comma[+prn]%
	setscore -1.79175946922806

np-prn--prn = np-prn--simpx* prn %np-prn--simpx[+prn]%
	setscore -1.79175946922806

np-prn--pron = pron np-prn* %np-prn[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == demons,
	setscore -2.39789527279837

np-prn--pron = pron np-prn* %np-prn[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == indef,
	setscore -2.39789527279837

np-prn--pron = pron np-prn* %np-prn[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == poss,
	setscore -1.70474809223843

np-prn--pron = pron np-prn--ap* %np-prn--ap[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == demons,
	setscore -2.39789527279837

np-prn--pron = pron np-prn--ap* %np-prn--ap[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == indef,
	setscore -2.39789527279837

np-prn--pron = pron np-prn--ap* %np-prn--ap[pron+]%
	C :== pron.C,
	Num :== pron.Num,
	R :== pron.R,
	pron.O == attr,
	pron.Typ == poss,
	setscore -2.39789527279837

np-prn--simpx = np-prn* simpx %np-prn[+simpx]%
	setscore 0

np-pron = pron* %NP[pron]%
	Block := 1,
	setscore -2.35137525716348

np-pron = pron* %NP[pron]%
	Block := 1,
	O == attr,
	Typ == indef,
	setscore -3.04452243772342

np-pron = pron* %NP[pron]%
	Block := 1,
	O == sub,
	setscore -2.63905732961526

np-pron = pron* %NP[pron]%
	Block := 1,
	O == sub,
	Typ == demons,
	setscore -1.94591014905531

np-pron = pron* %NP[pron]%
	Block := 1,
	O == sub,
	Typ == indef,
	setscore -0.847297860387204

np-pron = pron* %NP[pron]%
	Block := 1,
	O == sub,
	Typ == pers,
	setscore -2.35137525716348

np-pron = pron* %NP[pron]%
	Block := 1,
	O == sub,
	Typ == poss,
	setscore -2.63905732961526

np-pron = pron* %NP[pron]%
	Block := 1,
	O == sub,
	Typ == rel,
	setscore -3.73766961828337

np-pron = pron* %NP[pron]%
	Block := 1,
	Typ == refl,
	setscore -3.73766961828337

np-pron--advx = advx np-pron* %np-pron[advx+]%
	setscore -0.587786664902119

np-pron--advx = advx np-pron--advx* %np-pron--advx[advx+]%
	setscore -2.19722457733622

np-pron--advx = advx np-pron--conj* %np-pron--conj[advx+]%
	setscore -1.79175946922806

np-pron--advx = advx np-pron--part* %np-pron--part[advx+]%
	setscore -2.89037175789617

np-pron--advx = np-pron* advx %np-pron[+advx]%
	setscore -2.89037175789617

np-pron--ap = ap np-pron* %np-pron[ap+]%
	setscore -0.405465108108164

np-pron--ap = np-pron* ap %np-pron[+ap]%
	setscore -1.09861228866811

np-pron--article = article np-pron* %np-pron[article+]%
	setscore -0.336472236621213

np-pron--article = article np-pron--ap* %np-pron--ap[article+]%
	setscore -1.25276296849537

np-pron--conj = conj np-pron* %np-pron[conj+]%
	conj.T == comp,
	setscore -0.810930216216329

np-pron--conj = conj np-pron--advx* %np-pron--advx[conj+]%
	conj.T == comp,
	setscore -1.50407739677627

np-pron--conj = conj np-pron--article* %np-pron--article[conj+]%
	conj.T == comp,
	setscore -2.19722457733622

np-pron--conj = conj np-pron--pron* %np-pron--pron[conj+]%
	conj.T == comp,
	setscore -1.50407739677627

np-pron--noun = np-pron* noun %np-pron[+noun:possessive]%
	setscore 0

np-pron--part = part np-pron* %np-pron[part+]%
	setscore 0

np-pron--pp = pp np-pron* %np-pron[pp:compl1+]%
	setscore -1.09861228866811

np-pron--pp = pp np-pron--article* %np-pron--article[pp:compl1+]%
	setscore -1.09861228866811

np-pron--pron = pron np-pron* %np-pron[pron+]%
	setscore 0

np-simpx = simpx* %NP[simpx:app]%
	Block := 1,
	setscore -2.07944154167984

np-simpx = simpx* %NP[simpx]%
	Block := 1,
	setscore -0.133531392624523

np-simpx--article = article np-simpx--ap* %np-simpx--ap[article+]%
	setscore -0.693147180559945

np-simpx--np = np-simpx* np %np-simpx[+np:app]%
	setscore -0.693147180559945

np-simpx--pron = pron np-simpx--ap* %np-simpx--ap[pron+]%
	setscore 0

np-trunc = trunc* %NP[trunc:konj]%
	Block := 1,
	setscore -0.0800427076735364

np-trunc = trunc* %NP[trunc]%
	Block := 1,
	setscore -2.56494935746154

np-trunc--advx = np-trunc--conj* advx %np-trunc--conj[+advx]%
	setscore 0

np-trunc--comma = np-trunc* comma %np-trunc[+comma]%
	setscore -0.133531392624523

np-trunc--comma = np-trunc--trunc* comma %np-trunc--trunc[+comma]%
	setscore -2.07944154167984

np-trunc--conj = np-trunc* conj %np-trunc[+conj]%
	conj.T == comp,
	setscore -2.19722457733622

np-trunc--conj = np-trunc* conj %np-trunc[+conj]%
	conj.T == coord,
	setscore -1.09861228866811

np-trunc--conj = np-trunc--comma* conj %np-trunc--comma[+conj]%
	conj.T == coord,
	setscore -1.50407739677627

np-trunc--conj = np-trunc--np* conj %np-trunc--np[+conj]%
	conj.T == coord,
	setscore -2.19722457733622

np-trunc--conj = np-trunc--trunc* conj %np-trunc--trunc[+conj]%
	conj.T == coord,
	setscore -1.50407739677627

np-trunc--noun = np-trunc* noun %np-trunc[+noun:konj]%
	setscore -1.94591014905531

np-trunc--noun = np-trunc--advx* noun %np-trunc--advx[+noun:konj]%
	setscore -1.94591014905531

np-trunc--noun = np-trunc--comma* noun %np-trunc--comma[+noun:konj]%
	setscore -1.94591014905531

np-trunc--noun = np-trunc--conj* noun %np-trunc--conj[+noun:konj]%
	setscore -0.559615787935423

np-trunc--np = np-trunc--comma* np %np-trunc--comma[+np:konj]%
	setscore -0.916290731874155

np-trunc--np = np-trunc--conj* np %np-trunc--conj[+np:konj]%
	setscore -0.510825623765991

np-trunc--prn = np-trunc--conj* prn %np-trunc--conj[+prn:konj]%
	setscore 0

np-trunc--trunc = np-trunc--comma* trunc %np-trunc--comma[+trunc:konj]%
	setscore 0

np-unknown = unknown* %NP[unknown]%
	Block := 1,
	setscore 0

np-unknown--advx = advx np-unknown--np* %np-unknown--np[advx+]%
	setscore -0.693147180559945

np-unknown--advx = advx np-unknown--pron* %np-unknown--pron[advx+]%
	setscore -0.693147180559945

np-unknown--ap = ap np-unknown* %np-unknown[ap+]%
	setscore -0.470003629245736

np-unknown--ap = ap np-unknown--ap* %np-unknown--ap[ap+]%
	setscore -2.07944154167984

np-unknown--article = article np-unknown* %np-unknown[article+]%
	setscore -1.79175946922806

np-unknown--article = article np-unknown--ap* %np-unknown--ap[article+]%
	setscore -0.693147180559945

np-unknown--article = article np-unknown--comma* %np-unknown--comma[article+]%
	setscore -1.79175946922806

np-unknown--comma = comma np-unknown--ap* %np-unknown--ap[comma+]%
	setscore 0

np-unknown--conj = conj np-unknown* %np-unknown[conj+]%
	conj.T == comp,
	setscore 0

np-unknown--np = np np-unknown* %np-unknown[np+]%
	setscore -0.693147180559945

np-unknown--pron = pron np-unknown* %np-unknown[pron+]%
	setscore -0.405465108108164

np-unknown--pron = pron np-unknown--ap* %np-unknown--ap[pron+]%
	setscore -1.09861228866811

p-simpx = p-simpx-simpx--conj*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -1.79175946922806

p-simpx = p-simpx-simpx--simpx*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -1.09861228866811

p-simpx-simpx = simpx* %P-SIMPX[simpx:konj]%
	Block := 1,
	setscore -0.182321556793955

p-simpx-simpx = simpx* %P-SIMPX[simpx]%
	Block := 1,
	setscore -1.79175946922806

p-simpx-simpx--comma = p-simpx-simpx* comma %p-simpx-simpx[+comma]%
	setscore 0

p-simpx-simpx--conj = p-simpx-simpx* conj %p-simpx-simpx[+conj]%
	setscore -1.09861228866811

p-simpx-simpx--conj = p-simpx-simpx--comma* conj %p-simpx-simpx--comma[+conj]%
	setscore -0.693147180559945

p-simpx-simpx--simpx = p-simpx-simpx--conj* simpx %p-simpx-simpx--conj[+simpx:konj]%
	setscore -0.510825623765991

parord = parord-conj*
	{ Block := false },
	setscore 0

parord-conj = conj* %PARORD[conj]%
	Block := 1,
	setscore 0

pp = pp-adverb*
	{ Block := false },
	setscore -4.60517018598809

pp = pp-adverb--advx*
	{ Block := false },
	setscore -4.19970507787993

pp = pp-adverb--np*
	{ Block := false },
	setscore -5.29831736654804

pp = pp-adverb--pp*
	{ Block := false },
	setscore -5.29831736654804

pp = pp-advx--advx*
	{ Block := false },
	setscore -4.60517018598809

pp = pp-advx--conj*
	{ Block := false },
	setscore -5.29831736654804

pp = pp-advx--np*
	{ Block := false },
	setscore -5.29831736654804

pp = pp-advx--pp*
	{ Block := false },
	setscore -3.10109278921182

pp = pp-advx--prep*
	{ Block := false },
	setscore -4.60517018598809

pp = pp-advx--prepart*
	{ Block := false },
	setscore -5.29831736654804

pp = pp-advx--simpx*
	{ Block := false },
	setscore -5.29831736654804

pp = pp-ap--advx*
	{ Block := false },
	setscore -4.60517018598809

pp = pp-ap--prep*
	{ Block := false },
	setscore -4.60517018598809

pp = pp-conj--np*
	{ Block := false },
	setscore -5.29831736654804

pp = pp-conj--pp*
	{ Block := false },
	setscore -3.50655789731998

pp = pp-fx--advx*
	{ Block := false },
	setscore -4.60517018598809

pp = pp-fx--conj*
	{ Block := false },
	setscore -5.29831736654804

pp = pp-fx--prep*
	{ Block := false },
	setscore -5.29831736654804

pp = pp-fx--prepart*
	{ Block := false },
	setscore -5.29831736654804

pp = pp-noun--prepart*
	{ Block := false },
	setscore -5.29831736654804

pp = pp-np*
	{ Block := false },
	setscore -5.29831736654804

pp = pp-np--advx*
	{ Block := false },
	setscore -2.90042209374967

pp = pp-np--conj*
	{ Block := false },
	setscore -3.2188758248682

pp = pp-np--fx*
	{ Block := false },
	setscore -5.29831736654804

pp = pp-np--np*
	{ Block := false },
	setscore -2.81341071676004

pp = pp-np--pp*
	{ Block := false },
	setscore -3.50655789731998

pp = pp-np--prep*
	{ Block := false },
	setscore -3.35240721749272

pp = pp-np--prepart*
	{ Block := false },
	setscore -3.91202300542815

pp = pp-np--simpx*
	{ Block := false },
	setscore -5.29831736654804

pp = pp-part--pp*
	{ Block := false },
	setscore -5.29831736654804

pp = pp-pp--advx*
	{ Block := false },
	setscore -2.40794560865187

pp = pp-pp--ap*
	{ Block := false },
	setscore -4.60517018598809

pp = pp-pp--conj*
	{ Block := false },
	setscore -4.19970507787993

pp = pp-pp--fx*
	{ Block := false },
	setscore -5.29831736654804

pp = pp-pp--np*
	{ Block := false },
	setscore -2.7333680090865

pp = pp-pp--pp*
	{ Block := false },
	setscore -2.59026716544583

pp = pp-pp--prep*
	{ Block := false },
	setscore -4.60517018598809

pp = pp-pp--r-simpx*
	{ Block := false },
	setscore -5.29831736654804

pp = pp-pp--simpx*
	{ Block := false },
	setscore -2.99573227355399

pp = pp-prep*
	{ Block := false },
	setscore -5.29831736654804

pp = pp-prep--np*
	{ Block := false },
	setscore -5.29831736654804

pp = pp-prepart--np*
	{ Block := false },
	setscore -5.29831736654804

pp = pp-simpx--prep*
	{ Block := false },
	setscore -4.60517018598809

pp = pp-simpx--prepart*
	{ Block := false },
	setscore -5.29831736654804

pp-adverb = adverb* %PP[adverb]%
	Block := 1,
	T == pron,
	setscore -0.154150679827258

pp-adverb = adverb* %PP[adverb]%
	Block := 1,
	T == rel,
	setscore -1.94591014905531

pp-adverb--advx = advx pp-adverb* %pp-adverb[advx+]%
	setscore -0.693147180559945

pp-adverb--advx = advx pp-adverb--advx* %pp-adverb--advx[advx+]%
	setscore -1.38629436111989

pp-adverb--advx = advx pp-adverb--pp* %pp-adverb--pp[advx+]%
	setscore -1.38629436111989

pp-adverb--np = np pp-adverb* %pp-adverb[np+]%
	setscore 0

pp-adverb--pp = pp pp-adverb* %pp-adverb[pp:compl1+]%
	setscore 0

pp-advx = advx* %PP[advx]%
	Block := 1,
	setscore 0

pp-advx--advx = advx pp-advx* %pp-advx[advx+]%
	setscore -2.07944154167984

pp-advx--advx = advx pp-advx--prep* %pp-advx--prep[advx+]%
	setscore -2.07944154167984

pp-advx--advx = pp-advx* advx %pp-advx[+advx]%
	setscore -0.980829253011726

pp-advx--advx = pp-advx--conj* advx %pp-advx--conj[+advx]%
	setscore -0.980829253011726

pp-advx--comma = pp-advx--pp* comma %pp-advx--pp[+comma]%
	setscore 0

pp-advx--conj = conj pp-advx--prep* %pp-advx--prep[conj+]%
	setscore -2.30258509299405

pp-advx--conj = pp-advx--comma* conj %pp-advx--comma[+conj]%
	setscore -0.510825623765991

pp-advx--conj = pp-advx--pp* conj %pp-advx--pp[+conj]%
	setscore -1.20397280432594

pp-advx--np = pp-advx--conj* np %pp-advx--conj[+np:konj]%
	setscore 0

pp-advx--pp = pp pp-advx--advx* %pp-advx--advx[pp:compl1+]%
	setscore -2.99573227355399

pp-advx--pp = pp-advx* pp %pp-advx[+pp:compl1]%
	setscore -2.99573227355399

pp-advx--pp = pp-advx* pp %pp-advx[+pp:konj]%
	setscore -0.916290731874155

pp-advx--pp = pp-advx--advx* pp %pp-advx--advx[+pp:konj]%
	setscore -1.38629436111989

pp-advx--pp = pp-advx--conj* pp %pp-advx--conj[+pp:konj]%
	setscore -1.89711998488588

pp-advx--prep = pp-advx--prep* prep %pp-advx--prep[+prep]%
	setscore -1.79175946922806

pp-advx--prep = prep pp-advx* %pp-advx[prep+]%
	Przyim := prep.Core,
	setscore -0.405465108108164

pp-advx--prepart = prepart pp-advx* %pp-advx[prepart+]%
	Przyim := prepart.Core,
	setscore 0

pp-advx--simpx = pp-advx--conj* simpx %pp-advx--conj[+simpx:konj]%
	setscore 0

pp-ap = ap* %PP[ap]%
	Block := 1,
	setscore -1.94591014905531

pp-ap = ap* %PP[ap]%
	Block := 1,
	O == attr,
	setscore -1.94591014905531

pp-ap = ap* %PP[ap]%
	Block := 1,
	O == pred,
	setscore -0.336472236621213

pp-ap--advx = advx pp-ap--prep* %pp-ap--prep[advx+]%
	setscore -0.693147180559945

pp-ap--advx = pp-ap* advx %pp-ap[+advx]%
	setscore -0.693147180559945

pp-ap--pp = pp pp-ap--prep* %pp-ap--prep[pp:compl1+]%
	setscore 0

pp-ap--prep = prep pp-ap* %pp-ap[prep+]%
	Przyim := prep.Core,
	setscore -0.693147180559945

pp-conj = conj* %PP[conj]%
	Block := 1,
	setscore 0

pp-conj--advx = pp-conj* advx %pp-conj[+advx]%
	setscore -1.09861228866811

pp-conj--advx = pp-conj--conj* advx %pp-conj--conj[+advx]%
	setscore -0.405465108108164

pp-conj--comma = pp-conj--pp* comma %pp-conj--pp[+comma]%
	setscore 0

pp-conj--conj = pp-conj--comma* conj %pp-conj--comma[+conj]%
	setscore -0.693147180559945

pp-conj--conj = pp-conj--pp* conj %pp-conj--pp[+conj]%
	setscore -0.693147180559945

pp-conj--np = pp-conj* np %pp-conj[+np:konj]%
	setscore 0

pp-conj--pp = pp-conj* pp %pp-conj[+pp:konj]%
	setscore -0.693147180559945

pp-conj--pp = pp-conj--advx* pp %pp-conj--advx[+pp:konj]%
	setscore -1.20397280432594

pp-conj--pp = pp-conj--conj* pp %pp-conj--conj[+pp:konj]%
	setscore -1.6094379124341

pp-fx = fx* %PP[fx]%
	Block := 1,
	setscore 0

pp-fx--advx = advx pp-fx--prep* %pp-fx--prep[advx+]%
	setscore -0.405465108108164

pp-fx--advx = advx pp-fx--prepart* %pp-fx--prepart[advx+]%
	setscore -1.09861228866811

pp-fx--conj = conj pp-fx--advx* %pp-fx--advx[conj+]%
	setscore -0.693147180559945

pp-fx--conj = conj pp-fx--prep* %pp-fx--prep[conj+]%
	setscore -0.693147180559945

pp-fx--prep = prep pp-fx* %pp-fx[prep+]%
	Przyim := prep.Core,
	setscore -0.847297860387204

pp-fx--prepart = prepart pp-fx* %pp-fx[prepart+]%
	Przyim := prepart.Core,
	setscore -0.405465108108164

pp-noun = noun* %PP[noun]%
	Block := 1,
	setscore 0

pp-noun--prepart = prepart pp-noun* %pp-noun[prepart+]%
	Przyim := prepart.Core,
	setscore 0

pp-np = np* %PP[np]%
	Block := 1,
	setscore 0

pp-np--advx = advx pp-np* %pp-np[advx+]%
	setscore -3.3322045101752

pp-np--advx = advx pp-np--advx* %pp-np--advx[advx+]%
	setscore -1.94591014905531

pp-np--advx = advx pp-np--comma* %pp-np--comma[advx+]%
	setscore -2.63905732961526

pp-np--advx = advx pp-np--np* %pp-np--np[advx+]%
	setscore -2.63905732961526

pp-np--advx = advx pp-np--prep* %pp-np--prep[advx+]%
	setscore -0.847297860387204

pp-np--advx = advx pp-np--prepart* %pp-np--prepart[advx+]%
	setscore -1.94591014905531

pp-np--comma = comma pp-np* %pp-np[comma+]%
	setscore -1.38629436111989

pp-np--comma = comma pp-np--prep* %pp-np--prep[comma+]%
	setscore -1.38629436111989

pp-np--comma = comma pp-np--prepart* %pp-np--prepart[comma+]%
	setscore -0.693147180559945

pp-np--conj = conj pp-np* %pp-np[conj+]%
	setscore -2.30258509299405

pp-np--conj = conj pp-np--advx* %pp-np--advx[conj+]%
	setscore -0.693147180559945

pp-np--conj = conj pp-np--pp* %pp-np--pp[conj+]%
	setscore -2.30258509299405

pp-np--conj = conj pp-np--prep* %pp-np--prep[conj+]%
	setscore -1.6094379124341

pp-np--conj = conj pp-np--prepart* %pp-np--prepart[conj+]%
	setscore -2.30258509299405

pp-np--fx = fx pp-np--prep* %pp-np--prep[fx+]%
	setscore 0

pp-np--np = np pp-np--advx* %pp-np--advx[np+]%
	np.C == acc,
	setscore -2.70805020110221

pp-np--np = np pp-np--comma* %pp-np--comma[np+]%
	np.C == nom,
	setscore -2.70805020110221

pp-np--np = np pp-np--conj* %pp-np--conj[np+]%
	np.Cardinal == true,
	setscore -2.70805020110221


pp-np--np = np pp-np--prep* %pp-np--prep[np+]%
	np.C == acc,
	setscore -1.32175583998232

pp-np--np = np pp-np--prep* %pp-np--prep[np+]%
	np.C == nom,
	setscore -2.01490302054226


pp-np--np = np pp-np--prepart* %pp-np--prepart[np+]%
	np.C == acc,
	setscore -2.70805020110221

pp-np--pp = pp pp-np* %pp-np[pp:compl1+]%
	setscore -2.19722457733622

pp-np--pp = pp pp-np--advx* %pp-np--advx[pp:compl1+]%
	setscore -1.50407739677627

pp-np--pp = pp pp-np--prep* %pp-np--prep[pp:compl1+]%
	setscore -0.810930216216329

pp-np--pp = pp pp-np--prepart* %pp-np--prepart[pp:compl1+]%
	setscore -2.19722457733622

pp-np--prep = pp-np* prep %pp-np[+prep]%
	C :== prep.Dop1,
	Przyim := prep.Core,
	setscore -3.17805383034795

pp-np--prep = pp-np--advx* prep %pp-np--advx[+prep]%
	setscore -3.87120101090789

pp-np--prep = pp-np--prep* prep %pp-np--prep[+prep]%
	setscore -3.17805383034795

pp-np--prep = pp-np--prepart* prep %pp-np--prepart[+prep]%
	setscore -3.87120101090789

pp-np--prep = prep pp-np* %pp-np[prep+]%
	C :== prep.Dop1,
	Dop1 := prep.Dop1,
	Przyim := prep.Core,
	setscore -3.17805383034795

pp-np--prep = prep pp-np* %pp-np[prep+]%
	C :== prep.Dop1,
	Przyim := prep.Core,
	setscore -0.826678573184468

pp-np--prep = prep pp-np* %pp-np[prep+]%
	Dop1 := prep.Dop1,
	Przyim := prep.Core,
	setscore -3.87120101090789

pp-np--prep = prep pp-np* %pp-np[prep+]%
	Przyim := prep.Core,
	setscore -1.79175946922806

pp-np--prepart = pp-np* prepart %pp-np[+prepart]%
	C :== prepart.Dop1,
	Przyim := prepart.Core,
	setscore -2.94443897916644

pp-np--prepart = prepart pp-np* %pp-np[prepart+]%
	C :== prepart.Dop1,
	Dop1 := prepart.Dop1,
	Przyim := prepart.Core,
	setscore -2.94443897916644

pp-np--prepart = prepart pp-np* %pp-np[prepart+]%
	C :== prepart.Dop1,
	Przyim := prepart.Core,
	setscore -0.379489621704904

pp-np--prepart = prepart pp-np* %pp-np[prepart+]%
	Przyim := prepart.Core,
	setscore -2.94443897916644

pp-np--prepart = prepart pp-np--comma* %pp-np--comma[prepart+]%
	C :== prepart.Dop1,
	Przyim := prepart.Core,
	setscore -2.94443897916644

pp-np--simpx = simpx pp-np--prepart* %pp-np--prepart[simpx+]%
	setscore 0

pp-part = part* %PP[part]%
	Block := 1,
	setscore 0

pp-part--comma = pp-part--pp* comma %pp-part--pp[+comma]%
	setscore 0

pp-part--conj = pp-part--comma* conj %pp-part--comma[+conj]%
	setscore 0

pp-part--pp = pp-part* pp %pp-part[+pp:konj]%
	setscore -0.693147180559945

pp-part--pp = pp-part--conj* pp %pp-part--conj[+pp:konj]%
	setscore -0.693147180559945

pp-pp = pp* %PP[pp:app]%
	Block := 1,
	setscore -2.87638551592142

pp-pp = pp* %PP[pp:konj]%
	Block := 1,
	setscore -0.966843011036986

pp-pp = pp* %PP[pp]%
	Block := 1,
	setscore -0.573800422927379

pp-pp--advx = advx pp-pp* %pp-pp[advx+]%
	setscore -1.19392246847243

pp-pp--advx = advx pp-pp--advx* %pp-pp--advx[advx+]%
	setscore -2.39789527279837

pp-pp--advx = advx pp-pp--comma* %pp-pp--comma[advx+]%
	setscore -3.49650756146648

pp-pp--advx = advx pp-pp--pp* %pp-pp--pp[advx+]%
	setscore -3.49650756146648

pp-pp--advx = advx pp-pp--prep* %pp-pp--prep[advx+]%
	setscore -3.49650756146648

pp-pp--advx = pp-pp* advx %pp-pp[+advx:app]%
	setscore -3.49650756146648

pp-pp--advx = pp-pp* advx %pp-pp[+advx:konj]%
	setscore -3.49650756146648

pp-pp--advx = pp-pp* advx %pp-pp[+advx]%
	setscore -2.80336038090653

pp-pp--advx = pp-pp--advx* advx %pp-pp--advx[+advx]%
	setscore -2.39789527279837

pp-pp--advx = pp-pp--comma* advx %pp-pp--comma[+advx]%
	setscore -2.80336038090653

pp-pp--advx = pp-pp--conj* advx %pp-pp--conj[+advx:konj]%
	setscore -1.88706964903238

pp-pp--advx = pp-pp--conj* advx %pp-pp--conj[+advx]%
	setscore -2.80336038090653

pp-pp--advx = pp-pp--pp* advx %pp-pp--pp[+advx:konj]%
	setscore -3.49650756146648

pp-pp--ap = pp-pp--comma* ap %pp-pp--comma[+ap:konj]%
	setscore -0.405465108108164

pp-pp--ap = pp-pp--conj* ap %pp-pp--conj[+ap:konj]%
	setscore -1.09861228866811

pp-pp--comma = comma pp-pp* %pp-pp[comma+]%
	setscore -3.43398720448515

pp-pp--comma = pp-pp* comma %pp-pp[+comma]%
	setscore -0.389464766761723

pp-pp--comma = pp-pp--advx* comma %pp-pp--advx[+comma]%
	setscore -3.43398720448515

pp-pp--comma = pp-pp--ap* comma %pp-pp--ap[+comma]%
	setscore -3.43398720448515

pp-pp--comma = pp-pp--conj* comma %pp-pp--conj[+comma]%
	setscore -3.43398720448515

pp-pp--comma = pp-pp--np* comma %pp-pp--np[+comma]%
	setscore -2.7408400239252

pp-pp--comma = pp-pp--pp* comma %pp-pp--pp[+comma]%
	setscore -2.33537491581704

pp-pp--comma = pp-pp--simpx* comma %pp-pp--simpx[+comma]%
	setscore -3.43398720448515

pp-pp--conj = conj pp-pp* %pp-pp[conj+]%
	setscore -1.70474809223843

pp-pp--conj = conj pp-pp--advx* %pp-pp--advx[conj+]%
	setscore -2.39789527279837

pp-pp--conj = pp-pp* conj %pp-pp[+conj]%
	setscore -1.145132304303

pp-pp--conj = pp-pp--comma* conj %pp-pp--comma[+conj]%
	setscore -1.70474809223843

pp-pp--conj = pp-pp--pp* conj %pp-pp--pp[+conj]%
	setscore -1.99243016469021

pp-pp--fx = pp-pp* fx %pp-pp[+fx:app]%
	setscore 0

pp-pp--np = np pp-pp* %pp-pp[np+]%
	setscore -2.77258872223978

pp-pp--np = np pp-pp--prep* %pp-pp--prep[np+]%
	setscore -2.77258872223978

pp-pp--np = pp-pp* np %pp-pp[+np:konj]%
	setscore -2.77258872223978

pp-pp--np = pp-pp* np %pp-pp[+np:possessive]%
	setscore -2.77258872223978

pp-pp--np = pp-pp* np %pp-pp[+np]%
	setscore -2.77258872223978

pp-pp--np = pp-pp--advx* np %pp-pp--advx[+np]%
	setscore -2.77258872223978

pp-pp--np = pp-pp--comma* np %pp-pp--comma[+np:konj]%
	setscore -1.16315080980568

pp-pp--np = pp-pp--comma* np %pp-pp--comma[+np]%
	setscore -2.77258872223978

pp-pp--np = pp-pp--conj* np %pp-pp--conj[+np:konj]%
	setscore -2.07944154167984

pp-pp--np = pp-pp--conj* np %pp-pp--conj[+np]%
	setscore -2.77258872223978

pp-pp--pp = pp pp-pp* %pp-pp[pp:compl1+]%
	setscore -2.52572864430826

pp-pp--pp = pp pp-pp--advx* %pp-pp--advx[pp:compl1+]%
	setscore -2.52572864430826

pp-pp--pp = pp-pp* pp %pp-pp[+pp:app]%
	setscore -3.2188758248682

pp-pp--pp = pp-pp* pp %pp-pp[+pp:compl1]%
	setscore -3.2188758248682

pp-pp--pp = pp-pp* pp %pp-pp[+pp:konj]%
	setscore -2.52572864430826

pp-pp--pp = pp-pp--advx* pp %pp-pp--advx[+pp:compl1]%
	setscore -3.2188758248682

pp-pp--pp = pp-pp--advx* pp %pp-pp--advx[+pp:konj]%
	setscore -2.52572864430826

pp-pp--pp = pp-pp--comma* pp %pp-pp--comma[+pp:app]%
	setscore -3.2188758248682

pp-pp--pp = pp-pp--comma* pp %pp-pp--comma[+pp:compl1]%
	setscore -3.2188758248682

pp-pp--pp = pp-pp--comma* pp %pp-pp--comma[+pp:konj]%
	setscore -1.42711635564015

pp-pp--pp = pp-pp--conj* pp %pp-pp--conj[+pp:compl1]%
	setscore -3.2188758248682

pp-pp--pp = pp-pp--conj* pp %pp-pp--conj[+pp:konj]%
	setscore -1.83258146374831

pp-pp--prep = prep pp-pp* %pp-pp[prep+]%
	Przyim := prep.Core,
	setscore -1.6094379124341

pp-pp--prep = prep pp-pp* %pp-pp[prep+]%
	setscore -0.510825623765991

pp-pp--r-simpx = pp-pp--comma* r-simpx %pp-pp--comma[+r-simpx]%
	setscore 0

pp-pp--simpx = pp-pp* simpx %pp-pp[+simpx:konj]%
	setscore -2.39789527279837

pp-pp--simpx = pp-pp* simpx %pp-pp[+simpx]%
	setscore -2.39789527279837

pp-pp--simpx = pp-pp--comma* simpx %pp-pp--comma[+simpx:konj]%
	setscore -1.29928298413026

pp-pp--simpx = pp-pp--comma* simpx %pp-pp--comma[+simpx]%
	setscore -1.01160091167848

pp-pp--simpx = pp-pp--conj* simpx %pp-pp--conj[+simpx:konj]%
	setscore -2.39789527279837

pp-pp--simpx = simpx pp-pp* %pp-pp[simpx+]%
	setscore -2.39789527279837

pp-prep = prep* %PP[prep]%
	Block := 1,
	Przyim := Core,
	setscore 0


pp-prepart = prepart* %PP[prepart]%
	Block := 1,
	Przyim := Core,
	setscore 0


pp-simpx = simpx* %PP[simpx]%
	Block := 1,
	setscore 0

pp-simpx--comma = comma pp-simpx* %pp-simpx[comma+]%
	setscore 0

pp-simpx--conj = conj pp-simpx--prep* %pp-simpx--prep[conj+]%
	setscore 0

pp-simpx--prep = prep pp-simpx* %pp-simpx[prep+]%
	Przyim := prep.Core,
	setscore -1.38629436111989

pp-simpx--prep = prep pp-simpx--comma* %pp-simpx--comma[prep+]%
	Przyim := prep.Core,
	setscore -1.38629436111989

pp-simpx--prepart = prepart pp-simpx* %pp-simpx[prepart+]%
	Przyim := prepart.Core,
	setscore 0

pp-vp = vp* %PP[vp]%
	Block := 1,
	setscore 0

r-simpx = r-simpx-c--fkoord*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -4.79579054559674

r-simpx = r-simpx-c--mf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -5.48893772615669

r-simpx = r-simpx-c--mf*
	C_ON = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -4.79579054559674

r-simpx = r-simpx-c--nf*
	C_OA = true,
	C_ON = false,
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	setscore -5.48893772615669

r-simpx = r-simpx-c--nf*
	C_OA = true,
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -4.39032543748858

r-simpx = r-simpx-c--nf*
	C_OA = true,
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	setscore -3.87949981372259

r-simpx = r-simpx-c--nf*
	C_OD = true,
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -5.48893772615669

r-simpx = r-simpx-c--nf*
	C_OD = true,
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	setscore -4.79579054559674

r-simpx = r-simpx-c--nf*
	C_ON = false,
	MF_OA = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	setscore -3.69717825692863

r-simpx = r-simpx-c--nf*
	C_ON = false,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -4.79579054559674

r-simpx = r-simpx-c--nf*
	C_ON = false,
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	setscore -4.1026433650368

r-simpx = r-simpx-c--nf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -3.54302757710137

r-simpx = r-simpx-c--nf*
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	setscore -2.84988039654143

r-simpx = r-simpx-c--nf*
	C_ON = true,
	MF_OA = true,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -4.79579054559674

r-simpx = r-simpx-c--nf*
	C_ON = true,
	MF_OA = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -3.00403107636869

r-simpx = r-simpx-c--nf*
	C_ON = true,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -4.1026433650368

r-simpx = r-simpx-c--nf*
	C_ON = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -2.65572438210047

r-simpx = r-simpx-c--vc*
	C_OA = true,
	C_ON = false,
	MF_OA = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	setscore -5.48893772615669

r-simpx = r-simpx-c--vc*
	C_OA = true,
	C_ON = false,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -5.48893772615669

r-simpx = r-simpx-c--vc*
	C_OA = true,
	C_ON = false,
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	setscore -4.1026433650368

r-simpx = r-simpx-c--vc*
	C_OA = true,
	C_ON = false,
	MF_OG = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	setscore -5.48893772615669

r-simpx = r-simpx-c--vc*
	C_OA = true,
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -4.79579054559674

r-simpx = r-simpx-c--vc*
	C_OA = true,
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	setscore -3.09104245335832

r-simpx = r-simpx-c--vc*
	C_OD = true,
	C_ON = false,
	MF_OA = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -5.48893772615669

r-simpx = r-simpx-c--vc*
	C_OD = true,
	C_ON = false,
	MF_OA = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	setscore -4.39032543748858

r-simpx = r-simpx-c--vc*
	C_OD = true,
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	setscore -3.69717825692863

r-simpx = r-simpx-c--vc*
	C_ON = false,
	MF_OA = true,
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	setscore -4.39032543748858

r-simpx = r-simpx-c--vc*
	C_ON = false,
	MF_OA = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -4.79579054559674

r-simpx = r-simpx-c--vc*
	C_ON = false,
	MF_OA = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	setscore -2.78088752505448

r-simpx = r-simpx-c--vc*
	C_ON = false,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -5.48893772615669

r-simpx = r-simpx-c--vc*
	C_ON = false,
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	setscore -4.1026433650368

r-simpx = r-simpx-c--vc*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -4.79579054559674

r-simpx = r-simpx-c--vc*
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	setscore -2.65572438210047

r-simpx = r-simpx-c--vc*
	C_ON = true,
	MF_OA = true,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -3.40949618447685

r-simpx = r-simpx-c--vc*
	C_ON = true,
	MF_OA = true,
	MF_OG = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -5.48893772615669

r-simpx = r-simpx-c--vc*
	C_ON = true,
	MF_OA = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -2.65572438210047

r-simpx = r-simpx-c--vc*
	C_ON = true,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -3.54302757710137

r-simpx = r-simpx-c--vc*
	C_ON = true,
	MF_OG = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -4.39032543748858

r-simpx = r-simpx-c--vc*
	C_ON = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -2.31088389580874

r-simpx = r-simpx-fkoord--vc*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -5.48893772615669

r-simpx = r-simpx-koord--vc*
	C_ON = false,
	MF_OA = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	setscore -5.48893772615669

r-simpx = r-simpx-nf--vc*
	C_ON = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -5.48893772615669

r-simpx = r-simpx-r-simpx--advx*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -5.48893772615669

r-simpx = r-simpx-r-simpx--r-simpx*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -4.39032543748858

r-simpx = r-simpx-r-simpx--simpx*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	setscore -5.48893772615669

r-simpx = r-simpx-vc--mf*
	C_OD = true,
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	setscore -5.48893772615669

r-simpx-c = c* %R-SIMPX[c]%
	Block := 1,
	setscore -2.67414864942653

r-simpx-c = c* %R-SIMPX[c]%
	Block := 1,
	C_OA := OA,
	setscore -3.65497790243825

r-simpx-c = c* %R-SIMPX[c]%
	Block := 1,
	C_OA := OA,
	ON = false,
	setscore -2.31124315573716

r-simpx-c = c* %R-SIMPX[c]%
	Block := 1,
	C_OD := OD,
	setscore -4.75359019110636

r-simpx-c = c* %R-SIMPX[c]%
	Block := 1,
	C_OD := OD,
	ON = false,
	setscore -3.04884209886794

r-simpx-c = c* %R-SIMPX[c]%
	Block := 1,
	C_ON := ON,
	setscore -0.76460614454209

r-simpx-c = c* %R-SIMPX[c]%
	Block := 1,
	ON = false,
	setscore -1.51491173894198

r-simpx-c = c* %R-SIMPX[c]%
	Block := 1,
	ON = false,
	OPP = true,
	setscore -3.14415227867226

r-simpx-c = c* %R-SIMPX[c]%
	Block := 1,
	ON = false,
	PRED = true,
	setscore -4.3481250829982

r-simpx-c = c* %R-SIMPX[c]%
	Block := 1,
	OPP = true,
	setscore -4.75359019110636

r-simpx-c--comma = r-simpx-c* comma %r-simpx-c[+comma]%
	setscore -2.61006979274201

r-simpx-c--comma = r-simpx-c--fkoord* comma %r-simpx-c--fkoord[+comma]%
	setscore -4.21950770517611

r-simpx-c--comma = r-simpx-c--mf* comma %r-simpx-c--mf[+comma]%
	setscore -1.91692261218206

r-simpx-c--comma = r-simpx-c--vc* comma %r-simpx-c--vc[+comma]%
	setscore -0.268263986594679

r-simpx-c--fkoord = r-simpx-c* fkoord %r-simpx-c[+fkoord]%
	setscore -0.251314428280906

r-simpx-c--fkoord = r-simpx-c--mf* fkoord %r-simpx-c--mf[+fkoord]%
	setscore -2.19722457733622

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	ON_Num := mf.ON_Num,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	mf.ON = false,
	setscore -3.57887855889961

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	mf.ON = false,
	mf.OPP = true,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	mf.ON = false,
	mf.PRED = true,
	setscore -4.67749084756772

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_OG := mf.OG,
	OA_C := mf.OA_C,
	OG_C := mf.OG_C,
	mf.ON = false,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	setscore -3.29119648644783

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	setscore -3.57887855889961

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	mf.OPP = true,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	mf.PRED = true,
	setscore -3.98434366700777

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	ON_P := mf.ON_P,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	ON_P := mf.ON_P,
	mf.PRED = true,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	mf.PRED = true,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	mf.PRED = true,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	setscore -4.67749084756772

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -3.76120011569356

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	mf.PRED = true,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -4.27202573945955

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.ON = false,
	setscore -2.66258782702545

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.ON = false,
	mf.OPP = true,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.ON = false,
	mf.PRED = true,
	setscore -3.76120011569356

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.PRED = true,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OA := mf.OA,
	mf.ON = false,
	setscore -4.67749084756772

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OD_C := mf.OD_C,
	setscore -3.76120011569356

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OD_C := mf.OD_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	setscore -3.98434366700777

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OD_C := mf.OD_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -4.67749084756772

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OD_C := mf.OD_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	mf.OPP = true,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OD_C := mf.OD_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	mf.PRED = true,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	setscore -4.27202573945955

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -4.27202573945955

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	mf.ON = false,
	setscore -3.57887855889961

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	mf.ON = false,
	mf.OPP = true,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	mf.ON = false,
	mf.PRED = true,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	mf.PRED = true,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OG := mf.OG,
	MF_ON := mf.ON,
	OG_C := mf.OG_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OG := mf.OG,
	OG_C := mf.OG_C,
	mf.ON = false,
	setscore -4.67749084756772

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_OG := mf.OG,
	OG_C := mf.OG_C,
	mf.ON = false,
	mf.PRED = true,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_ON := mf.ON,
	setscore -3.06805293513362

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	setscore -2.7315806985124

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -3.76120011569356

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	mf.PRED = true,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -4.67749084756772

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	mf.OPP = true,
	setscore -4.27202573945955

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	mf.PRED = true,
	setscore -3.06805293513362

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_ON := mf.ON,
	ON_P := mf.ON_P,
	setscore -4.67749084756772

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_ON := mf.ON,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -4.67749084756772

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	MF_ON := mf.ON,
	mf.PRED = true,
	setscore -3.98434366700777

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -3.57887855889961

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	mf.PRED = true,
	setscore -4.67749084756772

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	mf.PRED = true,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	mf.ON = false,
	setscore -2.97274275532929

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	mf.ON = false,
	mf.OPP = true,
	setscore -4.27202573945955

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	mf.ON = false,
	mf.PRED = true,
	setscore -3.42472787907235

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	mf.PRED = true,
	setscore -4.67749084756772

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
	setscore -3.29119648644783

r-simpx-c--mf = r-simpx-c--comma* mf %r-simpx-c--comma[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	mf.ON = false,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c--comma* mf %r-simpx-c--comma[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	mf.ON = false,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c--comma* mf %r-simpx-c--comma[+mf]%
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	mf.PRED = true,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c--comma* mf %r-simpx-c--comma[+mf]%
	mf.ON = false,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c--comma* mf %r-simpx-c--comma[+mf]%
	mf.ON = false,
	mf.PRED = true,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c--vc* mf %r-simpx-c--vc[+mf]%
	mf.ON = false,
	setscore -5.37063802812766

r-simpx-c--mf = r-simpx-c--vc* mf %r-simpx-c--vc[+mf]%
	mf.ON = false,
	mf.PRED = true,
	setscore -5.37063802812766

r-simpx-c--mfe = r-simpx-c--vce* mfe %r-simpx-c--vce[+mfe]%
	setscore 0

r-simpx-c--nf = r-simpx-c--comma* nf %r-simpx-c--comma[+nf]%
	nf.ON = false,
	setscore -0.765467842139571

r-simpx-c--nf = r-simpx-c--comma* nf %r-simpx-c--comma[+nf]%
	nf.ON = false,
	nf.PRED = true,
	setscore -3.76120011569356

r-simpx-c--nf = r-simpx-c--comma* nf %r-simpx-c--comma[+nf]%
	setscore -2.05645202345514

r-simpx-c--nf = r-simpx-c--fkoord* nf %r-simpx-c--fkoord[+nf]%
	setscore -4.45434729625351

r-simpx-c--nf = r-simpx-c--vc* nf %r-simpx-c--vc[+nf]%
	nf.ON = false,
	setscore -1.27629346590556

r-simpx-c--nf = r-simpx-c--vc* nf %r-simpx-c--vc[+nf]%
	nf.ON = false,
	nf.OPP = true,
	setscore -4.45434729625351

r-simpx-c--nf = r-simpx-c--vc* nf %r-simpx-c--vc[+nf]%
	nf.ON = false,
	nf.PRED = true,
	setscore -4.45434729625351

r-simpx-c--nf = r-simpx-c--vc* nf %r-simpx-c--vc[+nf]%
	setscore -4.45434729625351

r-simpx-c--vc = r-simpx-c* vc %r-simpx-c[+vc]%
	vc.Dop1 == OA_C,
	vc.Przyim1 == zero,
	setscore -4.72738781871234

r-simpx-c--vc = r-simpx-c* vc %r-simpx-c[+vc]%
	vc.Dop1 == OPP_C,
	vc.Przyim1 == OPP_Przyim,
	setscore -5.42053499927229

r-simpx-c--vc = r-simpx-c* vc %r-simpx-c[+vc]%
	vc.Num == ON_Num,
	vc.P == ON_P,
	setscore -4.0342406381524

r-simpx-c--vc = r-simpx-c* vc %r-simpx-c[+vc]%
	setscore -4.0342406381524

r-simpx-c--vc = r-simpx-c--comma* vc %r-simpx-c--comma[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OD_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -5.42053499927229

r-simpx-c--vc = r-simpx-c--comma* vc %r-simpx-c--comma[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OD_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -5.42053499927229

r-simpx-c--vc = r-simpx-c--comma* vc %r-simpx-c--comma[+vc]%
	vc.Dop1 == OA_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	setscore -4.72738781871234

r-simpx-c--vc = r-simpx-c--comma* vc %r-simpx-c--comma[+vc]%
	vc.Dop1 == OA_C,
	vc.Przyim1 == zero,
	setscore -5.42053499927229

r-simpx-c--vc = r-simpx-c--comma* vc %r-simpx-c--comma[+vc]%
	vc.Dop1 == OD_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	setscore -5.42053499927229

r-simpx-c--vc = r-simpx-c--comma* vc %r-simpx-c--comma[+vc]%
	vc.Num == ON_Num,
	vc.P == ON_P,
	setscore -4.0342406381524

r-simpx-c--vc = r-simpx-c--fkoord* vc %r-simpx-c--fkoord[+vc]%
	vc.Num == ON_Num,
	vc.P == ON_P,
	setscore -4.72738781871234

r-simpx-c--vc = r-simpx-c--fkoord* vc %r-simpx-c--fkoord[+vc]%
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	setscore -5.42053499927229

r-simpx-c--vc = r-simpx-c--fkoord* vc %r-simpx-c--fkoord[+vc]%
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -5.42053499927229

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OD_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -3.11794990627824

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OD_C,
	vc.Num == ON_Num,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -5.42053499927229

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OD_C,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -5.42053499927229

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OD_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -3.34109345759245

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OG_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -4.72738781871234

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OPP_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -3.11794990627824

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OPP_C,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -5.42053499927229

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OPP_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -3.62877553004423

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	setscore -2.28504078334314

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -4.32192271060418

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	setscore -4.32192271060418

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -5.42053499927229

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Przyim1 == zero,
	setscore -2.53016324137612

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Dop1 == OD_C,
	vc.Dop2 == OPP_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -3.81109708683819

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Dop1 == OD_C,
	vc.Dop2 == OPP_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -5.42053499927229

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Dop1 == OD_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	setscore -2.85558564181075

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Dop1 == OD_C,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	setscore -5.42053499927229

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Dop1 == OD_C,
	vc.Przyim1 == zero,
	setscore -3.11794990627824

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Dop1 == OG_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	setscore -4.72738781871234

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Dop1 == OG_C,
	vc.Przyim1 == zero,
	setscore -5.42053499927229

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Dop1 == OPP_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == OPP_Przyim,
	setscore -3.11794990627824

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Dop1 == OPP_C,
	vc.P == ON_P,
	vc.Przyim1 == OPP_Przyim,
	setscore -5.42053499927229

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Dop1 == OPP_C,
	vc.Przyim1 == OPP_Przyim,
	setscore -3.47462485021697

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Dop2 == OD_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -5.42053499927229

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Num == ON_Num,
	vc.P == ON_P,
	setscore -2.1624384612508

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == OPP_Przyim,
	setscore -4.32192271060418

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	setscore -5.42053499927229

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.P == ON_P,
	setscore -4.0342406381524

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	vc.Przyim1 == zero,
	setscore -5.42053499927229

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
	setscore -2.78147766965703

r-simpx-c--vc = r-simpx-c--mfe* vc %r-simpx-c--mfe[+vc]%
	vc.Dop1 == OA_C,
	vc.Przyim1 == zero,
	setscore -5.42053499927229

r-simpx-c--vc = r-simpx-c--vce* vc %r-simpx-c--vce[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OD_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -5.42053499927229

r-simpx-c--vc = r-simpx-c--vce* vc %r-simpx-c--vce[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OPP_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -5.42053499927229

r-simpx-c--vc = r-simpx-c--vce* vc %r-simpx-c--vce[+vc]%
	vc.Dop1 == OA_C,
	vc.Przyim1 == zero,
	setscore -4.32192271060418

r-simpx-c--vc = r-simpx-c--vce* vc %r-simpx-c--vce[+vc]%
	setscore -5.42053499927229

r-simpx-c--vce = r-simpx-c--mf* vce %r-simpx-c--mf[+vce]%
	setscore 0

r-simpx-fkoord = fkoord* %R-SIMPX[fkoord]%
	Block := 1,
	setscore 0

r-simpx-fkoord--vc = r-simpx-fkoord* vc %r-simpx-fkoord[+vc]%
	setscore 0

r-simpx-koord = koord* %R-SIMPX[koord]%
	Block := 1,
	setscore 0

r-simpx-koord--c = r-simpx-koord* c %r-simpx-koord[+c]%
	c.ON = false,
	setscore 0

r-simpx-koord--mf = r-simpx-koord--c* mf %r-simpx-koord--c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	mf.PRED = true,
	setscore 0

r-simpx-koord--vc = r-simpx-koord--mf* vc %r-simpx-koord--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	setscore 0

r-simpx-nf = nf* %R-SIMPX[nf]%
	Block := 1,
	ON = false,
	setscore 0

r-simpx-nf--c = r-simpx-nf* c %r-simpx-nf[+c]%
	C_ON := c.ON,
	ON_Num := c.ON_Num,
	ON_P := c.ON_P,
	setscore 0

r-simpx-nf--mf = r-simpx-nf--c* mf %r-simpx-nf--c[+mf]%
	mf.ON = false,
	mf.PRED = true,
	setscore 0

r-simpx-nf--vc = r-simpx-nf--mf* vc %r-simpx-nf--mf[+vc]%
	vc.Num == ON_Num,
	vc.P == ON_P,
	setscore 0

r-simpx-r-simpx = r-simpx* %R-SIMPX[r-simpx:konj]%
	Block := 1,
	setscore 0

r-simpx-r-simpx--advx = r-simpx-r-simpx* advx %r-simpx-r-simpx[+advx:konj]%
	setscore 0

r-simpx-r-simpx--comma = r-simpx-r-simpx* comma %r-simpx-r-simpx[+comma]%
	setscore 0

r-simpx-r-simpx--conj = r-simpx-r-simpx* conj %r-simpx-r-simpx[+conj]%
	setscore -0.693147180559945

r-simpx-r-simpx--conj = r-simpx-r-simpx--comma* conj %r-simpx-r-simpx--comma[+conj]%
	setscore -1.38629436111989

r-simpx-r-simpx--r-simpx = r-simpx-r-simpx--comma* r-simpx %r-simpx-r-simpx--comma[+r-simpx:konj]%
	setscore -1.38629436111989

r-simpx-r-simpx--r-simpx = r-simpx-r-simpx--conj* r-simpx %r-simpx-r-simpx--conj[+r-simpx:konj]%
	setscore -0.287682072451781

r-simpx-r-simpx--simpx = r-simpx-r-simpx--conj* simpx %r-simpx-r-simpx--conj[+simpx:konj]%
	setscore 0

r-simpx-vc = vc* %R-SIMPX[vc]%
	Block := 1,
	Przyim1 == zero,
	setscore 0

r-simpx-vc--c = r-simpx-vc* c %r-simpx-vc[+c]%
	C_OD := c.OD,
	Dop1 == c.OD_C,
	c.ON = false,
	setscore 0

r-simpx-vc--mf = r-simpx-vc--c* mf %r-simpx-vc--c[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore 0

simpx = simpx-advx--simpx*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-c*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-c--fkoord*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.48119420757924

simpx = simpx-c--mf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-c--mf*
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-c--nf*
	C_OA = true,
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.25805065626503

simpx = simpx-c--nf*
	C_ON = false,
	MF_OA = true,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.76887628003102

simpx = simpx-c--nf*
	C_ON = false,
	MF_OA = true,
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.07572909947107

simpx = simpx-c--nf*
	C_ON = false,
	MF_OA = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.07572909947107

simpx = simpx-c--nf*
	C_ON = false,
	MF_OA = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -4.64861274383093

simpx = simpx-c--nf*
	C_ON = false,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.48119420757924

simpx = simpx-c--nf*
	C_ON = false,
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.07572909947107

simpx = simpx-c--nf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -5.03427522464291

simpx = simpx-c--nf*
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -4.12981895041576

simpx = simpx-c--nf*
	C_ON = true,
	MF_OA = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.25805065626503

simpx = simpx-c--nf*
	C_ON = true,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.48119420757924

simpx = simpx-c--nf*
	C_ON = true,
	MF_OG = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-c--nf*
	C_ON = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -5.67026399136291

simpx = simpx-c--vc*
	C_OA = true,
	C_ON = false,
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.76887628003102

simpx = simpx-c--vc*
	C_OA = true,
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -5.92157841964382

simpx = simpx-c--vc*
	C_OD = true,
	C_ON = false,
	MF_OA = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-c--vc*
	C_OD = true,
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-c--vc*
	C_ON = false,
	MF_OA = true,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.76887628003102

simpx = simpx-c--vc*
	C_ON = false,
	MF_OA = true,
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -5.56490347570508

simpx = simpx-c--vc*
	C_ON = false,
	MF_OA = true,
	MF_OG = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-c--vc*
	C_ON = false,
	MF_OA = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -5.67026399136291

simpx = simpx-c--vc*
	C_ON = false,
	MF_OA = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -4.50019273871265

simpx = simpx-c--vc*
	C_ON = false,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.48119420757924

simpx = simpx-c--vc*
	C_ON = false,
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -5.46959329590076

simpx = simpx-c--vc*
	C_ON = false,
	MF_OG = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-c--vc*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -5.46959329590076

simpx = simpx-c--vc*
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -3.9162448501177

simpx = simpx-c--vc*
	C_ON = true,
	MF_OA = true,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-c--vc*
	C_ON = true,
	MF_OA = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -5.67026399136291

simpx = simpx-c--vc*
	C_ON = true,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.48119420757924

simpx = simpx-c--vc*
	C_ON = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -5.30253921123759

simpx = simpx-conj--simpx*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.48119420757924

simpx = simpx-fkoord--mf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-fkoord--nf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.25805065626503

simpx = simpx-fkoord--vc*
	C_ON = false,
	MF_OA = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-fkoord--vc*
	C_ON = false,
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-fkoord--vc*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-fkoord--vf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-koord--fkoord*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.07572909947107

simpx = simpx-koord--mf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-koord--nf*
	C_ON = false,
	MF_OA = true,
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-koord--nf*
	C_ON = false,
	MF_OA = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.48119420757924

simpx = simpx-koord--nf*
	C_ON = false,
	MF_OA = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-koord--nf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_OA = true,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-koord--nf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.48119420757924

simpx = simpx-koord--nf*
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.76887628003102

simpx = simpx-koord--vc*
	C_ON = false,
	MF_OA = true,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-koord--vc*
	C_ON = false,
	MF_OA = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.76887628003102

simpx = simpx-koord--vc*
	C_ON = false,
	MF_OA = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.76887628003102

simpx = simpx-koord--vc*
	C_ON = false,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-koord--vc*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.25805065626503

simpx = simpx-koord--vc*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-koord--vc*
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.76887628003102

simpx = simpx-lk*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--comma*
	C_ON = false,
	MF_OA = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--comma*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--fkoord*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_OA = true,
	VF_ON = false,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-lk--fkoord*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_OD = true,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--fkoord*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -5.38258191891113

simpx = simpx-lk--fkoord*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -5.09489984645935

simpx = simpx-lk--fkoord*
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.76887628003102

simpx = simpx-lk--koord*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-lk--lv*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--mf*
	C_ON = false,
	MF_OA = true,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.48119420757924

simpx = simpx-lk--mf*
	C_ON = false,
	MF_OA = true,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -5.67026399136291

simpx = simpx-lk--mf*
	C_ON = false,
	MF_OA = true,
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -5.38258191891113

simpx = simpx-lk--mf*
	C_ON = false,
	MF_OA = true,
	MF_OG = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--mf*
	C_ON = false,
	MF_OA = true,
	MF_OG = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-lk--mf*
	C_ON = false,
	MF_OA = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -5.15943836759692

simpx = simpx-lk--mf*
	C_ON = false,
	MF_OA = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -4.03884717221003

simpx = simpx-lk--mf*
	C_ON = false,
	MF_OA = true,
	MF_ON = true,
	NF_ON = false,
	VF_OD = true,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--mf*
	C_ON = false,
	MF_OA = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -3.80704555815271

simpx = simpx-lk--mf*
	C_ON = false,
	MF_OD = true,
	MF_OG = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--mf*
	C_ON = false,
	MF_OD = true,
	MF_OG = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--mf*
	C_ON = false,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -5.78804702701929

simpx = simpx-lk--mf*
	C_ON = false,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -5.38258191891113

simpx = simpx-lk--mf*
	C_ON = false,
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	VF_OA = true,
	VF_ON = false,
	{ Block := false },
	setscore -6.76887628003102

simpx = simpx-lk--mf*
	C_ON = false,
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -4.82296613097571

simpx = simpx-lk--mf*
	C_ON = false,
	MF_OG = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--mf*
	C_ON = false,
	MF_OG = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-lk--mf*
	C_ON = false,
	MF_OG = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--mf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_OA = true,
	VF_ON = false,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-lk--mf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_OD = true,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--mf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -4.73199435276998

simpx = simpx-lk--mf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -3.84213687796398

simpx = simpx-lk--mf*
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_OA = true,
	VF_ON = false,
	{ Block := false },
	setscore -4.53528405852393

simpx = simpx-lk--mf*
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_OD = true,
	VF_ON = false,
	{ Block := false },
	setscore -5.92157841964382

simpx = simpx-lk--mf*
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -3.37885219896699

simpx = simpx-lk--nf*
	C_ON = false,
	MF_OA = true,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.07572909947107

simpx = simpx-lk--nf*
	C_ON = false,
	MF_OA = true,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -4.87175629514514

simpx = simpx-lk--nf*
	C_ON = false,
	MF_OA = true,
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -5.09489984645935

simpx = simpx-lk--nf*
	C_ON = false,
	MF_OA = true,
	MF_OG = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-lk--nf*
	C_ON = false,
	MF_OA = true,
	MF_OG = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--nf*
	C_ON = false,
	MF_OA = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -4.60939203067765

simpx = simpx-lk--nf*
	C_ON = false,
	MF_OA = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -3.40158045004455

simpx = simpx-lk--nf*
	C_ON = false,
	MF_OA = true,
	MF_ON = true,
	NF_ON = false,
	VF_OA = true,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--nf*
	C_ON = false,
	MF_OA = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -3.29277759019575

simpx = simpx-lk--nf*
	C_ON = false,
	MF_OD = true,
	MF_OG = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--nf*
	C_ON = false,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -4.82296613097571

simpx = simpx-lk--nf*
	C_ON = false,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -4.46629118703697

simpx = simpx-lk--nf*
	C_ON = false,
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	VF_OA = true,
	VF_ON = false,
	{ Block := false },
	setscore -5.67026399136291

simpx = simpx-lk--nf*
	C_ON = false,
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -4.12981895041576

simpx = simpx-lk--nf*
	C_ON = false,
	MF_OG = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--nf*
	C_ON = false,
	MF_OG = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--nf*
	C_ON = false,
	MF_ON = false,
	NF_OA = true,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--nf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_OA = true,
	VF_ON = false,
	{ Block := false },
	setscore -6.48119420757924

simpx = simpx-lk--nf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_OD = true,
	VF_ON = false,
	{ Block := false },
	setscore -5.30253921123759

simpx = simpx-lk--nf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -3.28252109002856

simpx = simpx-lk--nf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -2.6913388361253

simpx = simpx-lk--nf*
	C_ON = false,
	MF_ON = false,
	NF_ON = true,
	VF_ON = false,
	{ Block := false },
	setscore -6.25805065626503

simpx = simpx-lk--nf*
	C_ON = false,
	MF_ON = true,
	NF_OA = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--nf*
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_OA = true,
	VF_ON = false,
	{ Block := false },
	setscore -4.34112804408297

simpx = simpx-lk--nf*
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_OD = true,
	VF_ON = false,
	{ Block := false },
	setscore -5.30253921123759

simpx = simpx-lk--nf*
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_OG = true,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--nf*
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -2.6471327436208

simpx = simpx-lk--parord*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--simpx*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--vc*
	C_ON = false,
	MF_OA = true,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.25805065626503

simpx = simpx-lk--vc*
	C_ON = false,
	MF_OA = true,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -5.15943836759692

simpx = simpx-lk--vc*
	C_ON = false,
	MF_OA = true,
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -4.68943473835118

simpx = simpx-lk--vc*
	C_ON = false,
	MF_OA = true,
	MF_OG = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--vc*
	C_ON = false,
	MF_OA = true,
	MF_OG = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--vc*
	C_ON = false,
	MF_OA = true,
	MF_OG = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--vc*
	C_ON = false,
	MF_OA = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -4.92304958953269

simpx = simpx-lk--vc*
	C_ON = false,
	MF_OA = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -3.84213687796398

simpx = simpx-lk--vc*
	C_ON = false,
	MF_OA = true,
	MF_ON = true,
	NF_ON = false,
	VF_OA = true,
	VF_ON = false,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-lk--vc*
	C_ON = false,
	MF_OA = true,
	MF_ON = true,
	NF_ON = false,
	VF_OD = true,
	VF_ON = false,
	{ Block := false },
	setscore -6.07572909947107

simpx = simpx-lk--vc*
	C_ON = false,
	MF_OA = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -3.42483731220881

simpx = simpx-lk--vc*
	C_ON = false,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -5.67026399136291

simpx = simpx-lk--vc*
	C_ON = false,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -4.82296613097571

simpx = simpx-lk--vc*
	C_ON = false,
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	VF_OA = true,
	VF_ON = false,
	{ Block := false },
	setscore -5.56490347570508

simpx = simpx-lk--vc*
	C_ON = false,
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -4.5716517026948

simpx = simpx-lk--vc*
	C_ON = false,
	MF_OG = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--vc*
	C_ON = false,
	MF_OG = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--vc*
	C_ON = false,
	MF_OG = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-lk--vc*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_OA = true,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--vc*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_OD = true,
	VF_ON = false,
	{ Block := false },
	setscore -6.07572909947107

simpx = simpx-lk--vc*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -4.03884717221003

simpx = simpx-lk--vc*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -3.32419378642913

simpx = simpx-lk--vc*
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_OA = true,
	VF_ON = false,
	{ Block := false },
	setscore -4.31214050720972

simpx = simpx-lk--vc*
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_OD = true,
	VF_ON = false,
	{ Block := false },
	setscore -5.46959329590076

simpx = simpx-lk--vc*
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_OG = true,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--vc*
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -3.05530421332671

simpx = simpx-lk--vf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_OA = true,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--vf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lk--vf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -6.25805065626503

simpx = simpx-lk--vf*
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lv--fkoord*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-lv--mf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lv--nf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-lv--vc*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-mf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-mf--fkoord*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-mf--mf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-mf--nf*
	C_ON = false,
	MF_OA = true,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.48119420757924

simpx = simpx-mf--nf*
	C_ON = false,
	MF_OA = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -5.56490347570508

simpx = simpx-mf--nf*
	C_ON = false,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.76887628003102

simpx = simpx-mf--nf*
	C_ON = false,
	MF_ON = false,
	NF_OA = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-mf--nf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -5.30253921123759

simpx = simpx-mf--nf*
	C_ON = false,
	MF_ON = false,
	NF_ON = true,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-mf--nf*
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-mf--vc*
	C_OA = true,
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-mf--vc*
	C_ON = false,
	MF_OA = true,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.76887628003102

simpx = simpx-mf--vc*
	C_ON = false,
	MF_OA = true,
	MF_OD = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-mf--vc*
	C_ON = false,
	MF_OA = true,
	MF_OG = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-mf--vc*
	C_ON = false,
	MF_OA = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -5.56490347570508

simpx = simpx-mf--vc*
	C_ON = false,
	MF_OA = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-mf--vc*
	C_ON = false,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.76887628003102

simpx = simpx-mf--vc*
	C_ON = false,
	MF_OG = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-mf--vc*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -5.92157841964382

simpx = simpx-mf--vc*
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.48119420757924

simpx = simpx-nf--comma*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-p-simpx--simpx*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.48119420757924

simpx = simpx-parord--fkoord*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-parord--vc*
	C_ON = false,
	MF_OA = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-simpx--advx*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-simpx--ap*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-simpx--comma*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-simpx--conj*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.76887628003102

simpx = simpx-simpx--np*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.25805065626503

simpx = simpx-simpx--p-simpx*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-simpx--pp*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.76887628003102

simpx = simpx-simpx--simpx*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -5.67026399136291

simpx = simpx-vc*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-vc--mf*
	C_ON = false,
	MF_OA = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-vc--nf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.48119420757924

simpx = simpx-vf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-vf--fkoord*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-vf--mf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-vf--nf*
	C_ON = false,
	MF_OA = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-vf--nf*
	C_ON = false,
	MF_OA = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-vf--nf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_OA = true,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-vf--nf*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -5.92157841964382

simpx = simpx-vf--vc*
	C_ON = false,
	MF_OA = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-vf--vc*
	C_ON = false,
	MF_OA = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -7.17434138813918

simpx = simpx-vf--vc*
	C_ON = false,
	MF_OA = true,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-vf--vc*
	C_ON = false,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-vf--vc*
	C_ON = false,
	MF_OD = true,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-vf--vc*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_OA = true,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx = simpx-vf--vc*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -6.76887628003102

simpx = simpx-vf--vc*
	C_ON = false,
	MF_ON = false,
	NF_ON = false,
	VF_ON = true,
	{ Block := false },
	setscore -6.07572909947107

simpx = simpx-vf--vc*
	C_ON = false,
	MF_ON = true,
	NF_ON = false,
	VF_ON = false,
	{ Block := false },
	setscore -7.86748856869913

simpx-advx = advx* %SIMPX[advx]%
	Block := 1,
	setscore 0

simpx-advx--advx = simpx-advx* advx %simpx-advx[+advx]%
	setscore 0

simpx-advx--comma = simpx-advx--simpx* comma %simpx-advx--simpx[+comma]%
	setscore 0

simpx-advx--conj = simpx-advx--comma* conj %simpx-advx--comma[+conj]%
	setscore 0

simpx-advx--simpx = simpx-advx* simpx %simpx-advx[+simpx:konj]%
	setscore -1.38629436111989

simpx-advx--simpx = simpx-advx--advx* simpx %simpx-advx--advx[+simpx:konj]%
	setscore -1.38629436111989

simpx-advx--simpx = simpx-advx--conj* simpx %simpx-advx--conj[+simpx:konj]%
	setscore -0.693147180559945

simpx-c = c* %SIMPX[c]%
	Block := 1,
	setscore -3.32922388203707

simpx-c = c* %SIMPX[c]%
	Block := 1,
	CONJ == 1,
	setscore -1.8067973465926

simpx-c = c* %SIMPX[c]%
	Block := 1,
	CONJ == 1,
	ON = false,
	setscore -0.79025001097879

simpx-c = c* %SIMPX[c]%
	Block := 1,
	CONJ == 1,
	ON = false,
	T == sub,
	setscore -5.81413053182507

simpx-c = c* %SIMPX[c]%
	Block := 1,
	C_OA := OA,
	ON = false,
	setscore -3.10608033072286

simpx-c = c* %SIMPX[c]%
	Block := 1,
	C_OD := OD,
	ON = false,
	setscore -4.71551824315696

simpx-c = c* %SIMPX[c]%
	Block := 1,
	C_ON := ON,
	setscore -1.98548913533597

simpx-c = c* %SIMPX[c]%
	Block := 1,
	ON = false,
	setscore -2.31762297035859

simpx-c = c* %SIMPX[c]%
	Block := 1,
	ON = false,
	OPP = true,
	setscore -3.61690595448885

simpx-c = c* %SIMPX[c]%
	Block := 1,
	ON = false,
	PRED = true,
	setscore -3.86822038276975

simpx-c = c* %SIMPX[c]%
	Block := 1,
	OPP = true,
	setscore -5.81413053182507

simpx-c = c* %SIMPX[c]%
	Block := 1,
	PRED = true,
	setscore -5.81413053182507

simpx-c--comma = simpx-c* comma %simpx-c[+comma]%
	setscore -2.5032557884562

simpx-c--comma = simpx-c--fkoord* comma %simpx-c--fkoord[+comma]%
	setscore -3.60186807712431

simpx-c--comma = simpx-c--mf* comma %simpx-c--mf[+comma]%
	setscore -1.75604138662598

simpx-c--comma = simpx-c--vc* comma %simpx-c--vc[+comma]%
	setscore -0.331032513325395

simpx-c--fkoord = simpx-c* fkoord %simpx-c[+fkoord]%
	setscore -0.538996500732687

simpx-c--fkoord = simpx-c--comma* fkoord %simpx-c--comma[+fkoord]%
	setscore -2.484906649788

simpx-c--fkoord = simpx-c--mf* fkoord %simpx-c--mf[+fkoord]%
	setscore -1.38629436111989

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	setscore -4.36309862478836

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	setscore -3.9576335166802

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	mf.OPP = true,
	setscore -5.05624580534831

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	mf.PRED = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	ON_Num := mf.ON_Num,
	mf.PRED = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	mf.OPP = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OD_C := mf.OD_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	setscore -4.13995507347415

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	mf.ON = false,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	mf.OPP = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	mf.PRED = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_OG := mf.OG,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OG_C := mf.OG_C,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_OG := mf.OG,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OG_C := mf.OG_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	mf.PRED = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	setscore -3.04134278480604

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	ON_Num := mf.ON_Num,
	setscore -4.65078069724014

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	setscore -3.26448633612025

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -4.13995507347415

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	mf.PRED = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	mf.OPP = true,
	setscore -5.05624580534831

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	mf.PRED = true,
	setscore -4.36309862478836

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	ON_Num := mf.ON_Num,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	ON_Num := mf.ON_Num,
	mf.PRED = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	ON_P := mf.ON_P,
	setscore -5.05624580534831

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	ON_P := mf.ON_P,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -4.65078069724014

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	mf.PRED = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	mf.OPP = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	mf.PRED = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	setscore -3.9576335166802

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -4.65078069724014

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.05624580534831

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.ON = false,
	setscore -3.9576335166802

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.ON = false,
	mf.OPP = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.ON = false,
	mf.PRED = true,
	setscore -4.65078069724014

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.OPP = true,
	setscore -5.05624580534831

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.PRED = true,
	setscore -5.05624580534831

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OD_C := mf.OD_C,
	setscore -4.65078069724014

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OD_C := mf.OD_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	setscore -3.80348283685294

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OD_C := mf.OD_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -4.65078069724014

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OD_C := mf.OD_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	mf.OPP = true,
	setscore -5.05624580534831

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OD_C := mf.OD_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	mf.PRED = true,
	setscore -4.65078069724014

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OD_C := mf.OD_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OD_C := mf.OD_C,
	mf.PRED = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	setscore -4.65078069724014

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.05624580534831

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	mf.ON = false,
	setscore -4.36309862478836

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	mf.ON = false,
	mf.PRED = true,
	setscore -4.65078069724014

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	mf.PRED = true,
	setscore -5.05624580534831

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OG := mf.OG,
	MF_ON := mf.ON,
	OG_C := mf.OG_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OG := mf.OG,
	MF_ON := mf.ON,
	OG_C := mf.OG_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	mf.PRED = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_OG := mf.OG,
	OG_C := mf.OG_C,
	mf.ON = false,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_ON := mf.ON,
	setscore -2.97680426366847

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	setscore -4.65078069724014

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	setscore -2.65835053254994

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -3.55216840857203

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	mf.PRED = true,
	setscore -5.05624580534831

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.05624580534831

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	mf.OADJP = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	mf.OPP = true,
	setscore -4.36309862478836

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	mf.OPP = true,
	mf.PRED = true,
	setscore -4.65078069724014

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	mf.PRED = true,
	setscore -3.35149771310988

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.05624580534831

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	mf.PRED = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_ON := mf.ON,
	ON_P := mf.ON_P,
	setscore -5.05624580534831

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_ON := mf.ON,
	ON_P := mf.ON_P,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_ON := mf.ON,
	ON_P := mf.ON_P,
	mf.OPP = true,
	setscore -5.05624580534831

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_ON := mf.ON,
	ON_P := mf.ON_P,
	mf.PRED = true,
	setscore -5.05624580534831

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_ON := mf.ON,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -4.13995507347415

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_ON := mf.ON,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	mf.PRED = true,
	setscore -4.36309862478836

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_ON := mf.ON,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_ON := mf.ON,
	mf.OPP = true,
	setscore -4.65078069724014

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	MF_ON := mf.ON,
	mf.PRED = true,
	setscore -3.9576335166802

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -4.36309862478836

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	mf.PRED = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	mf.PRED = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	mf.ON = false,
	setscore -4.13995507347415

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	mf.ON = false,
	mf.OPP = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	mf.ON = false,
	mf.OPP = true,
	mf.PRED = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	mf.ON = false,
	mf.PRED = true,
	setscore -4.13995507347415

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	mf.OPP = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	mf.PRED = true,
	setscore -4.36309862478836

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
	setscore -3.18444362844672

simpx-c--mf = simpx-c--comma* mf %simpx-c--comma[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	ON_P := mf.ON_P,
	setscore -5.74939298590825

simpx-c--mf = simpx-c--comma* mf %simpx-c--comma[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c--comma* mf %simpx-c--comma[+mf]%
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	setscore -4.65078069724014

simpx-c--mf = simpx-c--comma* mf %simpx-c--comma[+mf]%
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	mf.PRED = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c--comma* mf %simpx-c--comma[+mf]%
	MF_ON := mf.ON,
	mf.PRED = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c--comma* mf %simpx-c--comma[+mf]%
	setscore -5.74939298590825

simpx-c--mf = simpx-c--vc* mf %simpx-c--vc[+mf]%
	Dop1 == mf.OPP_C,
	MF_ON := mf.ON,
	Przyim1 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.74939298590825

simpx-c--mf = simpx-c--vc* mf %simpx-c--vc[+mf]%
	MF_ON := mf.ON,
	setscore -5.74939298590825

simpx-c--mf = simpx-c--vc* mf %simpx-c--vc[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -5.74939298590825

simpx-c--nf = simpx-c--comma* nf %simpx-c--comma[+nf]%
	nf.ON = false,
	setscore -0.821625505333077

simpx-c--nf = simpx-c--comma* nf %simpx-c--comma[+nf]%
	nf.ON = false,
	nf.PRED = true,
	setscore -4.94875989037817

simpx-c--nf = simpx-c--comma* nf %simpx-c--comma[+nf]%
	setscore -1.85771743701985

simpx-c--nf = simpx-c--fkoord* nf %simpx-c--fkoord[+nf]%
	setscore -4.94875989037817

simpx-c--nf = simpx-c--mf* nf %simpx-c--mf[+nf]%
	setscore -4.94875989037817

simpx-c--nf = simpx-c--vc* nf %simpx-c--vc[+nf]%
	nf.ON = false,
	setscore -1.33784197773394

simpx-c--nf = simpx-c--vc* nf %simpx-c--vc[+nf]%
	nf.ON = false,
	nf.OPP = true,
	setscore -4.25561270981822

simpx-c--nf = simpx-c--vc* nf %simpx-c--vc[+nf]%
	nf.ON = false,
	nf.PRED = true,
	setscore -4.94875989037817

simpx-c--nf = simpx-c--vc* nf %simpx-c--vc[+nf]%
	setscore -3.15700042115011

simpx-c--vc = simpx-c* vc %simpx-c[+vc]%
	Dop1 := vc.Dop1,
	Przyim1 := vc.Przyim1,
	setscore -5.76519110278484

simpx-c--vc = simpx-c* vc %simpx-c[+vc]%
	Num := vc.Num,
	P := vc.P,
	setscore -5.76519110278484

simpx-c--vc = simpx-c* vc %simpx-c[+vc]%
	vc.Dop1 == OPP_C,
	vc.Przyim1 == OPP_Przyim,
	setscore -5.76519110278484

simpx-c--vc = simpx-c* vc %simpx-c[+vc]%
	vc.Num == ON_Num,
	vc.P == ON_P,
	setscore -4.66657881411674

simpx-c--vc = simpx-c* vc %simpx-c[+vc]%
	setscore -3.81928095372953

simpx-c--vc = simpx-c--comma* vc %simpx-c--comma[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OD_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -5.76519110278484

simpx-c--vc = simpx-c--comma* vc %simpx-c--comma[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OPP_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -5.76519110278484

simpx-c--vc = simpx-c--comma* vc %simpx-c--comma[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OPP_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -5.76519110278484

simpx-c--vc = simpx-c--comma* vc %simpx-c--comma[+vc]%
	vc.Dop1 == OA_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	setscore -5.76519110278484

simpx-c--vc = simpx-c--comma* vc %simpx-c--comma[+vc]%
	vc.Dop1 == OA_C,
	vc.Przyim1 == zero,
	setscore -4.37889674166495

simpx-c--vc = simpx-c--comma* vc %simpx-c--comma[+vc]%
	vc.Dop1 == OD_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	setscore -5.76519110278484

simpx-c--vc = simpx-c--comma* vc %simpx-c--comma[+vc]%
	vc.Dop1 == OPP_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == OPP_Przyim,
	setscore -5.76519110278484

simpx-c--vc = simpx-c--comma* vc %simpx-c--comma[+vc]%
	vc.Num == ON_Num,
	vc.P == ON_P,
	setscore -4.66657881411674

simpx-c--vc = simpx-c--comma* vc %simpx-c--comma[+vc]%
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == OPP_Przyim,
	setscore -5.76519110278484

simpx-c--vc = simpx-c--comma* vc %simpx-c--comma[+vc]%
	vc.P == ON_P,
	setscore -5.76519110278484

simpx-c--vc = simpx-c--comma* vc %simpx-c--comma[+vc]%
	setscore -5.76519110278484

simpx-c--vc = simpx-c--fkoord* vc %simpx-c--fkoord[+vc]%
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	setscore -5.76519110278484

simpx-c--vc = simpx-c--fkoord* vc %simpx-c--fkoord[+vc]%
	vc.Przyim1 == zero,
	setscore -5.76519110278484

simpx-c--vc = simpx-c--fkoord* vc %simpx-c--fkoord[+vc]%
	setscore -5.76519110278484

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OD_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -3.36729582998647

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OD_C,
	vc.Num == ON_Num,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -5.76519110278484

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OD_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -3.12613377316959

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OG_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -5.76519110278484

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OG_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -5.76519110278484

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OPP_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -3.4626060097908

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OPP_C,
	vc.Num == ON_Num,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -5.76519110278484

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OPP_C,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -5.76519110278484

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OPP_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -3.68574956110501

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	setscore -2.46935423678052

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -5.76519110278484

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Num == ON_Num,
	vc.Przyim1 == zero,
	setscore -4.37889674166495

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	setscore -4.66657881411674

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Przyim1 == zero,
	setscore -2.46935423678052

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -5.0720439222249

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop1 == OD_C,
	vc.Dop2 == OPP_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -4.37889674166495

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop1 == OD_C,
	vc.Dop2 == OPP_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -4.37889674166495

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop1 == OD_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	setscore -3.05714090168263

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop1 == OD_C,
	vc.Przyim1 == zero,
	setscore -3.36729582998647

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop1 == OG_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	setscore -4.66657881411674

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop1 == OPP_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == OPP_Przyim,
	setscore -3.28028445299684

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop1 == OPP_C,
	vc.Num == ON_Num,
	vc.Przyim1 == OPP_Przyim,
	setscore -5.0720439222249

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop1 == OPP_C,
	vc.P == ON_P,
	vc.Przyim1 == OPP_Przyim,
	setscore -5.76519110278484

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop1 == OPP_C,
	vc.Przyim1 == OPP_Przyim,
	setscore -3.36729582998647

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop2 == OD_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -5.76519110278484

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Dop2 == OPP_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -5.0720439222249

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Num == ON_Num,
	setscore -4.66657881411674

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Num == ON_Num,
	vc.P == ON_P,
	setscore -2.02752148450148

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == OPP_Przyim,
	setscore -5.76519110278484

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	setscore -5.0720439222249

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.P == ON_P,
	setscore -3.97343163355679

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Przyim1 == OPP_Przyim,
	setscore -5.76519110278484

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	vc.Przyim1 == zero,
	setscore -5.76519110278484

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
	setscore -2.23883057816868

simpx-c--vc = simpx-c--vce* vc %simpx-c--vce[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OD_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -5.76519110278484

simpx-c--vc = simpx-c--vce* vc %simpx-c--vce[+vc]%
	vc.Dop1 == OA_C,
	vc.Przyim1 == zero,
	setscore -5.0720439222249

simpx-c--vc = simpx-c--vce* vc %simpx-c--vce[+vc]%
	vc.Dop1 == OPP_C,
	vc.Przyim1 == OPP_Przyim,
	setscore -5.76519110278484

simpx-c--vc = simpx-c--vce* vc %simpx-c--vce[+vc]%
	setscore -5.0720439222249

simpx-c--vce = simpx-c--mf* vce %simpx-c--mf[+vce]%
	setscore 0

simpx-conj = conj* %SIMPX[conj]%
	Block := 1,
	setscore 0

simpx-conj--comma = simpx-conj--simpx* comma %simpx-conj--simpx[+comma]%
	setscore 0

simpx-conj--conj = simpx-conj--comma* conj %simpx-conj--comma[+conj]%
	setscore -0.693147180559945

simpx-conj--conj = simpx-conj--simpx* conj %simpx-conj--simpx[+conj]%
	setscore -0.693147180559945

simpx-conj--simpx = simpx-conj* simpx %simpx-conj[+simpx:konj]%
	setscore -0.847297860387204

simpx-conj--simpx = simpx-conj* simpx %simpx-conj[+simpx]%
	setscore -1.94591014905531

simpx-conj--simpx = simpx-conj--conj* simpx %simpx-conj--conj[+simpx:konj]%
	setscore -1.25276296849537

simpx-fkoord = fkoord* %SIMPX[fkoord]%
	Block := 1,
	setscore 0

simpx-fkoord--comma = simpx-fkoord* comma %simpx-fkoord[+comma]%
	setscore -0.693147180559945

simpx-fkoord--comma = simpx-fkoord--vc* comma %simpx-fkoord--vc[+comma]%
	setscore -0.693147180559945

simpx-fkoord--mf = simpx-fkoord* mf %simpx-fkoord[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	setscore -1.09861228866811

simpx-fkoord--mf = simpx-fkoord* mf %simpx-fkoord[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OD_C := mf.OD_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	setscore -1.09861228866811

simpx-fkoord--mf = simpx-fkoord--comma* mf %simpx-fkoord--comma[+mf]%
	setscore -1.09861228866811

simpx-fkoord--nf = simpx-fkoord* nf %simpx-fkoord[+nf]%
	setscore -1.6094379124341

simpx-fkoord--nf = simpx-fkoord--comma* nf %simpx-fkoord--comma[+nf]%
	setscore -0.510825623765991

simpx-fkoord--nf = simpx-fkoord--vc* nf %simpx-fkoord--vc[+nf]%
	setscore -1.6094379124341

simpx-fkoord--vc = simpx-fkoord* vc %simpx-fkoord[+vc]%
	vc.Przyim1 == zero,
	setscore -1.25276296849537

simpx-fkoord--vc = simpx-fkoord* vc %simpx-fkoord[+vc]%
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -1.94591014905531

simpx-fkoord--vc = simpx-fkoord* vc %simpx-fkoord[+vc]%
	setscore -1.25276296849537

simpx-fkoord--vc = simpx-fkoord--mf* vc %simpx-fkoord--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Przyim1 == zero,
	setscore -1.94591014905531

simpx-fkoord--vc = simpx-fkoord--mf* vc %simpx-fkoord--mf[+vc]%
	vc.Dop1 == OD_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	setscore -1.94591014905531

simpx-fkoord--vf = simpx-fkoord* vf %simpx-fkoord[+vf]%
	setscore 0

simpx-koord = koord* %SIMPX[koord]%
	Block := 1,
	setscore 0

simpx-koord--c = simpx-koord* c %simpx-koord[+c]%
	c.CONJ == 1,
	setscore -1.09861228866811

simpx-koord--c = simpx-koord* c %simpx-koord[+c]%
	c.CONJ == 1,
	c.ON = false,
	setscore -0.510825623765991

simpx-koord--c = simpx-koord* c %simpx-koord[+c]%
	c.ON = false,
	setscore -2.70805020110221

simpx-koord--comma = simpx-koord--fkoord* comma %simpx-koord--fkoord[+comma]%
	setscore -2.484906649788

simpx-koord--comma = simpx-koord--lv* comma %simpx-koord--lv[+comma]%
	setscore -2.484906649788

simpx-koord--comma = simpx-koord--vc* comma %simpx-koord--vc[+comma]%
	setscore -0.287682072451781

simpx-koord--comma = simpx-koord--vf* comma %simpx-koord--vf[+comma]%
	setscore -2.484906649788

simpx-koord--fkoord = simpx-koord* fkoord %simpx-koord[+fkoord]%
	setscore -2.19722457733622

simpx-koord--fkoord = simpx-koord--c* fkoord %simpx-koord--c[+fkoord]%
	setscore -2.19722457733622

simpx-koord--fkoord = simpx-koord--comma* fkoord %simpx-koord--comma[+fkoord]%
	setscore -2.19722457733622

simpx-koord--fkoord = simpx-koord--vf* fkoord %simpx-koord--vf[+fkoord]%
	setscore -0.587786664902119

simpx-koord--lv = simpx-koord* lv %simpx-koord[+lv]%
	setscore 0

simpx-koord--mf = simpx-koord* mf %simpx-koord[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	setscore -3.3322045101752

simpx-koord--mf = simpx-koord* mf %simpx-koord[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	setscore -2.63905732961526

simpx-koord--mf = simpx-koord* mf %simpx-koord[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -2.63905732961526

simpx-koord--mf = simpx-koord* mf %simpx-koord[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.PRED = true,
	setscore -3.3322045101752

simpx-koord--mf = simpx-koord* mf %simpx-koord[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	setscore -3.3322045101752

simpx-koord--mf = simpx-koord* mf %simpx-koord[+mf]%
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	setscore -3.3322045101752

simpx-koord--mf = simpx-koord* mf %simpx-koord[+mf]%
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -3.3322045101752

simpx-koord--mf = simpx-koord* mf %simpx-koord[+mf]%
	mf.PRED = true,
	setscore -3.3322045101752

simpx-koord--mf = simpx-koord* mf %simpx-koord[+mf]%
	setscore -2.63905732961526

simpx-koord--mf = simpx-koord--c* mf %simpx-koord--c[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	setscore -3.3322045101752

simpx-koord--mf = simpx-koord--c* mf %simpx-koord--c[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	setscore -3.3322045101752

simpx-koord--mf = simpx-koord--c* mf %simpx-koord--c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	setscore -2.63905732961526

simpx-koord--mf = simpx-koord--c* mf %simpx-koord--c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -3.3322045101752

simpx-koord--mf = simpx-koord--c* mf %simpx-koord--c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	setscore -3.3322045101752

simpx-koord--mf = simpx-koord--c* mf %simpx-koord--c[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	setscore -3.3322045101752

simpx-koord--mf = simpx-koord--c* mf %simpx-koord--c[+mf]%
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	setscore -2.23359222150709

simpx-koord--mf = simpx-koord--c* mf %simpx-koord--c[+mf]%
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -3.3322045101752

simpx-koord--mf = simpx-koord--c* mf %simpx-koord--c[+mf]%
	MF_ON := mf.ON,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	mf.PRED = true,
	setscore -3.3322045101752

simpx-koord--mf = simpx-koord--c* mf %simpx-koord--c[+mf]%
	setscore -2.63905732961526

simpx-koord--mf = simpx-koord--vf* mf %simpx-koord--vf[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	setscore -3.3322045101752

simpx-koord--mf = simpx-koord--vf* mf %simpx-koord--vf[+mf]%
	setscore -3.3322045101752

simpx-koord--nf = simpx-koord--comma* nf %simpx-koord--comma[+nf]%
	nf.ON = false,
	setscore -1.54044504094715

simpx-koord--nf = simpx-koord--comma* nf %simpx-koord--comma[+nf]%
	nf.ON = false,
	nf.PRED = true,
	setscore -2.63905732961526

simpx-koord--nf = simpx-koord--comma* nf %simpx-koord--comma[+nf]%
	setscore -0.847297860387204

simpx-koord--nf = simpx-koord--vc* nf %simpx-koord--vc[+nf]%
	nf.ON = false,
	nf.OPP = true,
	setscore -2.63905732961526

simpx-koord--nf = simpx-koord--vc* nf %simpx-koord--vc[+nf]%
	setscore -1.54044504094715

simpx-koord--vc = simpx-koord* vc %simpx-koord[+vc]%
	setscore -3.43398720448515

simpx-koord--vc = simpx-koord--fkoord* vc %simpx-koord--fkoord[+vc]%
	setscore -3.43398720448515

simpx-koord--vc = simpx-koord--mf* vc %simpx-koord--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OD_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -2.33537491581704

simpx-koord--vc = simpx-koord--mf* vc %simpx-koord--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OPP_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -3.43398720448515

simpx-koord--vc = simpx-koord--mf* vc %simpx-koord--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OPP_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -2.7408400239252

simpx-koord--vc = simpx-koord--mf* vc %simpx-koord--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	setscore -2.7408400239252

simpx-koord--vc = simpx-koord--mf* vc %simpx-koord--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Przyim1 == zero,
	setscore -1.82454929205105

simpx-koord--vc = simpx-koord--mf* vc %simpx-koord--mf[+vc]%
	vc.Dop1 == OD_C,
	vc.Przyim1 == zero,
	setscore -3.43398720448515

simpx-koord--vc = simpx-koord--mf* vc %simpx-koord--mf[+vc]%
	vc.Dop1 == OPP_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == OPP_Przyim,
	setscore -3.43398720448515

simpx-koord--vc = simpx-koord--mf* vc %simpx-koord--mf[+vc]%
	vc.Dop1 == OPP_C,
	vc.Przyim1 == OPP_Przyim,
	setscore -3.43398720448515

simpx-koord--vc = simpx-koord--mf* vc %simpx-koord--mf[+vc]%
	vc.Num == ON_Num,
	vc.P == ON_P,
	setscore -1.82454929205105

simpx-koord--vc = simpx-koord--mf* vc %simpx-koord--mf[+vc]%
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	setscore -3.43398720448515

simpx-koord--vc = simpx-koord--mf* vc %simpx-koord--mf[+vc]%
	setscore -1.82454929205105

simpx-koord--vc = simpx-koord--vf* vc %simpx-koord--vf[+vc]%
	vc.Dop1 == OA_C,
	vc.Przyim1 == zero,
	setscore -3.43398720448515

simpx-koord--vc = simpx-koord--vf* vc %simpx-koord--vf[+vc]%
	setscore -3.43398720448515

simpx-koord--vf = simpx-koord* vf %simpx-koord[+vf]%
	OA_C := vf.OA_C,
	VF_OA := vf.OA,
	setscore -2.39789527279837

simpx-koord--vf = simpx-koord* vf %simpx-koord[+vf]%
	VF_ON := vf.ON,
	setscore -2.39789527279837

simpx-koord--vf = simpx-koord* vf %simpx-koord[+vf]%
	setscore -0.606135803570316

simpx-koord--vf = simpx-koord--comma* vf %simpx-koord--comma[+vf]%
	setscore -2.39789527279837

simpx-lk = lk* %SIMPX[lk]%
	Block := 1,
	setscore -0.371318967951381

simpx-lk = lk* %SIMPX[lk]%
	Block := 1,
	Przyim1 == zero,
	setscore -1.32739794988395

simpx-lk = lk* %SIMPX[lk]%
	Block := 1,
	Przyim1 == zero,
	Przyim2 == zero,
	setscore -3.10087537427455

simpx-lk--c = c simpx-lk* %simpx-lk[c+]%
	c.CONJ == 1,
	setscore -2.03688192726104

simpx-lk--c = c simpx-lk* %simpx-lk[c+]%
	c.CONJ == 1,
	c.ON = false,
	setscore -0.139761942375159

simpx-lk--comma = comma simpx-lk* %simpx-lk[comma+]%
	setscore -1.02392139606652

simpx-lk--comma = comma simpx-lk--vf* %simpx-lk--vf[comma+]%
	setscore -2.55681923137829

simpx-lk--comma = simpx-lk* comma %simpx-lk[+comma]%
	setscore -6.41754894241888

simpx-lk--comma = simpx-lk--fkoord* comma %simpx-lk--fkoord[+comma]%
	setscore -5.03125458129899

simpx-lk--comma = simpx-lk--koord* comma %simpx-lk--koord[+comma]%
	setscore -4.91347154564261

simpx-lk--comma = simpx-lk--mf* comma %simpx-lk--mf[+comma]%
	setscore -1.45820694271018

simpx-lk--comma = simpx-lk--nf* comma %simpx-lk--nf[+comma]%
	setscore -7.11069612297883

simpx-lk--comma = simpx-lk--parord* comma %simpx-lk--parord[+comma]%
	setscore -7.11069612297883

simpx-lk--comma = simpx-lk--vc* comma %simpx-lk--vc[+comma]%
	setscore -1.2642573479211

simpx-lk--comma = simpx-lk--vf* comma %simpx-lk--vf[+comma]%
	setscore -3.55534806148941

simpx-lk--conj = conj simpx-lk* %simpx-lk[conj+]%
	setscore -1.38629436111989

simpx-lk--conj = conj simpx-lk--vf* %simpx-lk--vf[conj+]%
	setscore -0.693147180559945

simpx-lk--conj = simpx-lk--comma* conj %simpx-lk--comma[+conj]%
	setscore -1.38629436111989

simpx-lk--fkoord = simpx-lk* fkoord %simpx-lk[+fkoord]%
	setscore -3.38439026334577

simpx-lk--fkoord = simpx-lk--c* fkoord %simpx-lk--c[+fkoord]%
	setscore -3.38439026334577

simpx-lk--fkoord = simpx-lk--comma* fkoord %simpx-lk--comma[+fkoord]%
	setscore -4.07753744390572

simpx-lk--fkoord = simpx-lk--koord* fkoord %simpx-lk--koord[+fkoord]%
	setscore -2.28577797467766

simpx-lk--fkoord = simpx-lk--lv* fkoord %simpx-lk--lv[+fkoord]%
	setscore -3.38439026334577

simpx-lk--fkoord = simpx-lk--mf* fkoord %simpx-lk--mf[+fkoord]%
	setscore -2.69124308278583

simpx-lk--fkoord = simpx-lk--parord* fkoord %simpx-lk--parord[+fkoord]%
	setscore -2.97892515523761

simpx-lk--fkoord = simpx-lk--vf* fkoord %simpx-lk--vf[+fkoord]%
	setscore -0.522189382416306

simpx-lk--koord = koord simpx-lk* %simpx-lk[koord+]%
	setscore -2.50843714719819

simpx-lk--koord = koord simpx-lk--comma* %simpx-lk--comma[koord+]%
	setscore -5.8406416573734

simpx-lk--koord = koord simpx-lk--lv* %simpx-lk--lv[koord+]%
	setscore -3.13259145627119

simpx-lk--koord = koord simpx-lk--vf* %simpx-lk--vf[koord+]%
	setscore -0.177681177237452

simpx-lk--lv = lv simpx-lk--comma* %simpx-lk--comma[lv+]%
	setscore -0.369360103466048

simpx-lk--lv = lv simpx-lk--vf* %simpx-lk--vf[lv+]%
	setscore -1.19908281903174

simpx-lk--mf = mf simpx-lk* %simpx-lk[mf+]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	setscore -7.53743003658651

simpx-lk--mf = mf simpx-lk* %simpx-lk[mf+]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OA_C := mf.OA_C,
	P == mf.ON_P,
	setscore -7.53743003658651

simpx-lk--mf = mf simpx-lk* %simpx-lk[mf+]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -6.4388177479184

simpx-lk--mf = mf simpx-lk* %simpx-lk[mf+]%
	mf.ON = false,
	mf.PRED = true,
	setscore -6.84428285602656

simpx-lk--mf = mf simpx-lk--vf* %simpx-lk--vf[mf+]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	mf.ON = false,
	setscore -7.53743003658651

simpx-lk--mf = mf simpx-lk--vf* %simpx-lk--vf[mf+]%
	Dop1 == mf.OPP_C,
	Przyim1 == mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = mf simpx-lk--vf* %simpx-lk--vf[mf+]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OA_C := mf.OA_C,
	P == mf.ON_P,
	setscore -7.53743003658651

simpx-lk--mf = mf simpx-lk--vf* %simpx-lk--vf[mf+]%
	MF_ON := mf.ON,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = mf simpx-lk--vf* %simpx-lk--vf[mf+]%
	mf.ON = false,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OD_C,
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	setscore -6.15113567546662

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OD_C,
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OD_C,
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OD_C,
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OD_C,
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OPP_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OPP_C,
	MF_OA := mf.OA,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	setscore -5.92799212415241

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -5.92799212415241

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OD_C,
	Dop2 == mf.OPP_C,
	MF_OD := mf.OD,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	setscore -6.15113567546662

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OPP_C,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	Przyim1 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OPP_C,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	Przyim1 == mf.OPP_Przyim,
	mf.OPP = true,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop1 == mf.OPP_C,
	Przyim1 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	Dop2 == mf.OD_C,
	MF_OD := mf.OD,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_OA := mf.OA,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	P == mf.ON_P,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OA_C := mf.OA_C,
	P == mf.ON_P,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.PRED = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OD_C := mf.OD_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OD_C := mf.OD_C,
	P == mf.ON_P,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OD_C := mf.OD_C,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OD_C := mf.OD_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_OG := mf.OG,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OG_C := mf.OG_C,
	P == mf.ON_P,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_ON := mf.ON,
	setscore -5.92799212415241

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -5.45798849490667

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.PRED = true,
	setscore -5.74567056735845

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_ON := mf.ON,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_ON := mf.ON,
	P == mf.ON_P,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	MF_ON := mf.ON,
	mf.PRED = true,
	setscore -6.15113567546662

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	mf.ON = false,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	mf.PRED = true,
	setscore -5.92799212415241

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
	setscore -5.5915198875312

simpx-lk--mf = simpx-lk--c* mf %simpx-lk--c[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--c* mf %simpx-lk--c[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--c* mf %simpx-lk--c[+mf]%
	Dop1 == mf.OPP_C,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	Przyim1 == mf.OPP_Przyim,
	mf.OPP = true,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--c* mf %simpx-lk--c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OA_C := mf.OA_C,
	P == mf.ON_P,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--c* mf %simpx-lk--c[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--c* mf %simpx-lk--c[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OD_C := mf.OD_C,
	P == mf.ON_P,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--c* mf %simpx-lk--c[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--c* mf %simpx-lk--c[+mf]%
	MF_ON := mf.ON,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--c* mf %simpx-lk--c[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -5.92799212415241

simpx-lk--mf = simpx-lk--c* mf %simpx-lk--c[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.PRED = true,
	setscore -6.15113567546662

simpx-lk--mf = simpx-lk--comma* mf %simpx-lk--comma[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--comma* mf %simpx-lk--comma[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	mf.ON = false,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--comma* mf %simpx-lk--comma[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	mf.ON = false,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--comma* mf %simpx-lk--comma[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	mf.ON = false,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--comma* mf %simpx-lk--comma[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.ON = false,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--comma* mf %simpx-lk--comma[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.ON = false,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--comma* mf %simpx-lk--comma[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	mf.ON = false,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--comma* mf %simpx-lk--comma[+mf]%
	MF_ON := mf.ON,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--comma* mf %simpx-lk--comma[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -6.15113567546662

simpx-lk--mf = simpx-lk--comma* mf %simpx-lk--comma[+mf]%
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--comma* mf %simpx-lk--comma[+mf]%
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--comma* mf %simpx-lk--comma[+mf]%
	mf.ON = false,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--comma* mf %simpx-lk--comma[+mf]%
	mf.ON = false,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--comma* mf %simpx-lk--comma[+mf]%
	mf.ON = false,
	mf.PRED = true,
	setscore -5.92799212415241

simpx-lk--mf = simpx-lk--comma* mf %simpx-lk--comma[+mf]%
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--comma* mf %simpx-lk--comma[+mf]%
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--conj* mf %simpx-lk--conj[+mf]%
	Dop1 == mf.OPP_C,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	Przyim1 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--conj* mf %simpx-lk--conj[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OD_C := mf.OD_C,
	P == mf.ON_P,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--conj* mf %simpx-lk--conj[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OD_C,
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -5.74567056735845

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OD_C,
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	mf.ON = false,
	setscore -5.92799212415241

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OD_C,
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	mf.ON = false,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OG_C,
	MF_OA := mf.OA,
	MF_OG := mf.OG,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OPP_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -6.15113567546662

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OPP_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OPP_C,
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	Przyim2 == mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OPP_C,
	MF_OA := mf.OA,
	Przyim2 == mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	setscore -5.92799212415241

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -4.89837270697125

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.PRED = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	mf.ON = false,
	setscore -5.23484494359246

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	mf.ON = false,
	mf.OPP = true,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	mf.ON = false,
	mf.PRED = true,
	setscore -6.15113567546662

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OD_C,
	Dop2 == mf.OPP_C,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OD_C,
	Dop2 == mf.OPP_C,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -5.92799212415241

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.PRED = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	mf.ON = false,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	mf.ON = false,
	mf.PRED = true,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	mf.PRED = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OG_C,
	MF_OG := mf.OG,
	mf.ON = false,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OPP_C,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	Przyim1 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.74567056735845

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OPP_C,
	MF_ON := mf.ON,
	Przyim1 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OPP_C,
	Przyim1 == mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -5.92799212415241

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop1 == mf.OPP_C,
	Przyim1 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop2 == mf.OD_C,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	Dop2 == mf.OPP_C,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_OA := mf.OA,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	P == mf.ON_P,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	mf.ON = false,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OA_C := mf.OA_C,
	P == mf.ON_P,
	setscore -5.34020545925029

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	setscore -5.5915198875312

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.ON = false,
	setscore -5.34020545925029

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.ON = false,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OD_C := mf.OD_C,
	P == mf.ON_P,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OD_C := mf.OD_C,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OD_C := mf.OD_C,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OD_C := mf.OD_C,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	mf.ON = false,
	setscore -6.15113567546662

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	mf.ON = false,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_ON := mf.ON,
	setscore -4.54169776303252

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -6.15113567546662

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -3.87386839045686

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.PRED = true,
	setscore -5.05252338679851

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_ON := mf.ON,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_ON := mf.ON,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_ON := mf.ON,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	MF_ON := mf.ON,
	mf.PRED = true,
	setscore -6.15113567546662

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -5.92799212415241

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	mf.ON = false,
	setscore -4.40193582065736

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	mf.ON = false,
	mf.OPP = true,
	setscore -6.15113567546662

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	mf.ON = false,
	mf.PRED = true,
	setscore -5.13953476378814

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	mf.PRED = true,
	setscore -5.34020545925029

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
	setscore -5.05252338679851

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OD_C,
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OPP_C,
	MF_OA := mf.OA,
	Przyim2 == mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -5.5915198875312

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.PRED = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	mf.ON = false,
	setscore -5.92799212415241

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	Dop1 == mf.OD_C,
	Dop2 == mf.OG_C,
	MF_OD := mf.OD,
	MF_OG := mf.OG,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	Dop1 == mf.OD_C,
	Dop2 == mf.OPP_C,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	mf.ON = false,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	Dop1 == mf.OPP_C,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	Przyim1 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	Dop1 == mf.OPP_C,
	MF_ON := mf.ON,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	Przyim1 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	Dop1 == mf.OPP_C,
	Przyim1 == mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	Dop1 == mf.OPP_C,
	Przyim1 == mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	Dop2 == mf.OPP_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	Dop2 == mf.OPP_C,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OA_C := mf.OA_C,
	P == mf.ON_P,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OA_C := mf.OA_C,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.ON = false,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OD_C := mf.OD_C,
	P == mf.ON_P,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OD_C := mf.OD_C,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	mf.ON = false,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	MF_ON := mf.ON,
	setscore -5.34020545925029

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -4.49290759886309

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.PRED = true,
	setscore -5.74567056735845

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	MF_ON := mf.ON,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	MF_ON := mf.ON,
	P == mf.ON_P,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	MF_ON := mf.ON,
	mf.PRED = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	mf.ON = false,
	setscore -5.5915198875312

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	mf.ON = false,
	mf.PRED = true,
	setscore -5.34020545925029

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--nf* mf %simpx-lk--nf[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--nf* mf %simpx-lk--nf[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--nf* mf %simpx-lk--nf[+mf]%
	mf.ON = false,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OD_C,
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OD_C,
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	mf.ON = false,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OPP_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OPP_C,
	MF_OA := mf.OA,
	Przyim2 == mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -5.92799212415241

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	mf.ON = false,
	setscore -5.92799212415241

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	mf.ON = false,
	mf.PRED = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	Dop1 == mf.OD_C,
	Dop2 == mf.OPP_C,
	MF_OD := mf.OD,
	Przyim2 == mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	Dop1 == mf.OPP_C,
	Przyim1 == mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	Dop2 == mf.OD_C,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OD_C := mf.OD_C,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	P == mf.ON_P,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	mf.ON = false,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OA_C := mf.OA_C,
	P == mf.ON_P,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.ON = false,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.ON = false,
	mf.PRED = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	mf.ON = false,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	MF_ON := mf.ON,
	setscore -6.15113567546662

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -4.8293798354843

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.PRED = true,
	setscore -6.15113567546662

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	MF_ON := mf.ON,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	mf.ON = false,
	setscore -5.23484494359246

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	mf.ON = false,
	mf.OPP = true,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	mf.ON = false,
	mf.PRED = true,
	setscore -5.74567056735845

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	mf.PRED = true,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
	setscore -6.15113567546662

simpx-lk--mf = simpx-lk--vc* mf %simpx-lk--vc[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	mf.ON = false,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OD_C,
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	setscore -5.74567056735845

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OD_C,
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -5.13953476378814

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OD_C,
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OD_C,
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OD_C,
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OD_C,
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OD_C,
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	mf.ON = false,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OD_C,
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	mf.ON = false,
	setscore -5.13953476378814

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OD_C,
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	mf.ON = false,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OD_C,
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	mf.ON = false,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OG_C,
	MF_OA := mf.OA,
	MF_OG := mf.OG,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OG_C,
	MF_OA := mf.OA,
	MF_OG := mf.OG,
	mf.ON = false,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OPP_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.45798849490667

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OPP_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OPP_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	P == mf.ON_P,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OPP_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OPP_C,
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	Przyim2 == mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OPP_C,
	MF_OA := mf.OA,
	Przyim2 == mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -5.23484494359246

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OPP_C,
	MF_OA := mf.OA,
	Przyim2 == mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	mf.PRED = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	Dop2 == mf.OPP_C,
	MF_OA := mf.OA,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	setscore -5.5915198875312

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	setscore -5.45798849490667

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -4.01106951197035

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.OPP = true,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.PRED = true,
	setscore -5.45798849490667

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	setscore -5.92799212415241

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	P == mf.ON_P,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	P == mf.ON_P,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.ON = false,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.ON = false,
	mf.PRED = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	Przyim2 == mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	mf.ON = false,
	setscore -4.17013420660003

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	mf.ON = false,
	mf.OPP = true,
	setscore -5.92799212415241

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	mf.ON = false,
	mf.OPP = true,
	mf.PRED = true,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	mf.ON = false,
	mf.PRED = true,
	setscore -5.13953476378814

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	mf.PRED = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OD_C,
	Dop2 == mf.OG_C,
	MF_OD := mf.OD,
	MF_OG := mf.OG,
	mf.ON = false,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OD_C,
	Dop2 == mf.OPP_C,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.92799212415241

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OD_C,
	Dop2 == mf.OPP_C,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	mf.PRED = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OD_C,
	Dop2 == mf.OPP_C,
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OD_C,
	Dop2 == mf.OPP_C,
	MF_OD := mf.OD,
	Przyim2 == mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -5.92799212415241

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	setscore -5.74567056735845

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -4.70421669253029

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.OPP = true,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.PRED = true,
	setscore -5.74567056735845

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OD_C := mf.OD_C,
	setscore -6.15113567546662

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	mf.ON = false,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	mf.ON = false,
	setscore -5.23484494359246

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	mf.ON = false,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	mf.ON = false,
	mf.PRED = true,
	setscore -5.5915198875312

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OD_C,
	MF_OD := mf.OD,
	mf.PRED = true,
	setscore -5.92799212415241

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OG_C,
	MF_OG := mf.OG,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OG_C,
	MF_OG := mf.OG,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OG_C,
	MF_OG := mf.OG,
	mf.ON = false,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OPP_C,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	Przyim1 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.23484494359246

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OPP_C,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	Przyim1 == mf.OPP_Przyim,
	mf.OPP = true,
	mf.PRED = true,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OPP_C,
	MF_ON := mf.ON,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	Przyim1 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -6.15113567546662

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OPP_C,
	MF_ON := mf.ON,
	Przyim1 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OPP_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	Przyim1 == mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -6.15113567546662

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OPP_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	Przyim1 == mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OPP_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	Przyim1 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OPP_C,
	Przyim1 == mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -4.8293798354843

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OPP_C,
	Przyim1 == mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	mf.PRED = true,
	setscore -6.15113567546662

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop1 == mf.OPP_C,
	Przyim1 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.5915198875312

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop2 == mf.OD_C,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop2 == mf.OD_C,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -5.74567056735845

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop2 == mf.OD_C,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop2 == mf.OD_C,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OD_C := mf.OD_C,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop2 == mf.OPP_C,
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop2 == mf.OPP_C,
	MF_OA := mf.OA,
	Przyim2 == mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop2 == mf.OPP_C,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.45798849490667

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop2 == mf.OPP_C,
	MF_ON := mf.ON,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop2 == mf.OPP_C,
	MF_ON := mf.ON,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop2 == mf.OPP_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Dop2 == mf.OPP_C,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	P == mf.ON_P,
	setscore -5.74567056735845

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	P == mf.ON_P,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	mf.ON = false,
	setscore -5.23484494359246

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	mf.ON = false,
	mf.OPP = true,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_OD := mf.OD,
	OA_C := mf.OA_C,
	OD_C := mf.OD_C,
	mf.ON = false,
	mf.PRED = true,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_OG := mf.OG,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OG_C := mf.OG_C,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_OG := mf.OG,
	OA_C := mf.OA_C,
	OG_C := mf.OG_C,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_OG := mf.OG,
	OA_C := mf.OA_C,
	OG_C := mf.OG_C,
	mf.ON = false,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	setscore -6.15113567546662

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OA_C := mf.OA_C,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -6.15113567546662

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	P == mf.ON_P,
	mf.OPP = true,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OA_C := mf.OA_C,
	P == mf.ON_P,
	setscore -4.64705827869034

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OA_C := mf.OA_C,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OA_C := mf.OA_C,
	P == mf.ON_P,
	mf.PRED = true,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -5.45798849490667

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	setscore -4.70421669253029

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -6.15113567546662

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	P == mf.ON_P,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	mf.PRED = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	setscore -5.92799212415241

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -5.13953476378814

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.ON = false,
	setscore -4.44638758322819

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.ON = false,
	mf.OPP = true,
	setscore -5.92799212415241

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.ON = false,
	mf.PRED = true,
	setscore -5.23484494359246

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OA := mf.OA,
	mf.ON = false,
	setscore -5.74567056735845

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OD := mf.OD,
	MF_OG := mf.OG,
	OD_C := mf.OD_C,
	OG_C := mf.OG_C,
	mf.ON = false,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OD_C := mf.OD_C,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OD_C := mf.OD_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OD_C := mf.OD_C,
	P == mf.ON_P,
	setscore -5.34020545925029

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OD_C := mf.OD_C,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OD_C := mf.OD_C,
	P == mf.ON_P,
	mf.PRED = true,
	setscore -6.15113567546662

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OD_C := mf.OD_C,
	setscore -5.74567056735845

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OD := mf.OD,
	MF_ON := mf.ON,
	OD_C := mf.OD_C,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	setscore -5.5915198875312

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	mf.ON = false,
	setscore -5.23484494359246

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	mf.ON = false,
	mf.PRED = true,
	setscore -5.92799212415241

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	mf.PRED = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OD := mf.OD,
	mf.ON = false,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OD := mf.OD,
	mf.ON = false,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OG := mf.OG,
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OG_C := mf.OG_C,
	P == mf.ON_P,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OG := mf.OG,
	OG_C := mf.OG_C,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_OG := mf.OG,
	OG_C := mf.OG_C,
	mf.ON = false,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_ON := mf.ON,
	setscore -3.54844599002223

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -4.97248067912497

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	P == mf.ON_P,
	mf.OPP = true,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	OPP_Przyim := mf.OPP_Przyim,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	setscore -2.91245722330224

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	Przyim1 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -6.4388177479184

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	Przyim2 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.OPP = true,
	setscore -4.89837270697125

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.OPP = true,
	mf.PRED = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	P == mf.ON_P,
	mf.PRED = true,
	setscore -4.07169413378678

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_ON := mf.ON,
	Num == mf.ON_Num,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_ON := mf.ON,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.34020545925029

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_ON := mf.ON,
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	mf.PRED = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_ON := mf.ON,
	OPP_Przyim := mf.OPP_Przyim,
	Przyim1 == mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_ON := mf.ON,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_ON := mf.ON,
	P == mf.ON_P,
	setscore -4.97248067912497

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_ON := mf.ON,
	P == mf.ON_P,
	mf.PRED = true,
	setscore -6.15113567546662

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_ON := mf.ON,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	MF_ON := mf.ON,
	mf.PRED = true,
	setscore -4.97248067912497

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -4.97248067912497

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	mf.PRED = true,
	setscore -7.53743003658651

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.OPP = true,
	setscore -5.5915198875312

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	Przyim1 == mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -6.15113567546662

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	mf.ON = false,
	setscore -3.4265561724132

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	mf.ON = false,
	mf.OPP = true,
	setscore -4.70421669253029

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	mf.ON = false,
	mf.OPP = true,
	mf.PRED = true,
	setscore -6.84428285602656

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	mf.ON = false,
	mf.PRED = true,
	setscore -4.01106951197035

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	mf.OPP = true,
	setscore -5.5915198875312

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	mf.PRED = true,
	setscore -4.76484131434673

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
	setscore -3.87386839045686

simpx-lk--nf = nf simpx-lk* %simpx-lk[nf+]%
	nf.ON = false,
	setscore -6.1463292576689

simpx-lk--nf = nf simpx-lk--comma* %simpx-lk--comma[nf+]%
	nf.ON = false,
	setscore -6.83947643822884

simpx-lk--nf = nf simpx-lk--comma* %simpx-lk--comma[nf+]%
	setscore -6.83947643822884

simpx-lk--nf = nf simpx-lk--vf* %simpx-lk--vf[nf+]%
	nf.ON = false,
	setscore -6.83947643822884

simpx-lk--nf = simpx-lk* nf %simpx-lk[+nf]%
	setscore -6.83947643822884

simpx-lk--nf = simpx-lk--comma* nf %simpx-lk--comma[+nf]%
	NF_ON := nf.ON,
	setscore -6.1463292576689

simpx-lk--nf = simpx-lk--comma* nf %simpx-lk--comma[+nf]%
	nf.ON = false,
	setscore -0.671959947340501

simpx-lk--nf = simpx-lk--comma* nf %simpx-lk--comma[+nf]%
	nf.ON = false,
	nf.PRED = true,
	setscore -3.6614226078809

simpx-lk--nf = simpx-lk--comma* nf %simpx-lk--comma[+nf]%
	nf.PRED = true,
	setscore -6.83947643822884

simpx-lk--nf = simpx-lk--comma* nf %simpx-lk--comma[+nf]%
	setscore -2.01919487262381

simpx-lk--nf = simpx-lk--fkoord* nf %simpx-lk--fkoord[+nf]%
	nf.ON = false,
	setscore -6.1463292576689

simpx-lk--nf = simpx-lk--fkoord* nf %simpx-lk--fkoord[+nf]%
	setscore -6.1463292576689

simpx-lk--nf = simpx-lk--koord* nf %simpx-lk--koord[+nf]%
	nf.ON = false,
	setscore -6.83947643822884

simpx-lk--nf = simpx-lk--mf* nf %simpx-lk--mf[+nf]%
	NF_OA := nf.OA,
	nf.ON = false,
	setscore -6.83947643822884

simpx-lk--nf = simpx-lk--mf* nf %simpx-lk--mf[+nf]%
	nf.ON = false,
	setscore -2.54901699708045

simpx-lk--nf = simpx-lk--mf* nf %simpx-lk--mf[+nf]%
	nf.ON = false,
	nf.OPP = true,
	setscore -6.1463292576689

simpx-lk--nf = simpx-lk--mf* nf %simpx-lk--mf[+nf]%
	nf.ON = false,
	nf.PRED = true,
	setscore -5.74086414956073

simpx-lk--nf = simpx-lk--mf* nf %simpx-lk--mf[+nf]%
	setscore -4.27452708076731

simpx-lk--nf = simpx-lk--parord* nf %simpx-lk--parord[+nf]%
	nf.ON = false,
	setscore -6.83947643822884

simpx-lk--nf = simpx-lk--parord* nf %simpx-lk--parord[+nf]%
	setscore -6.1463292576689

simpx-lk--nf = simpx-lk--vc* nf %simpx-lk--vc[+nf]%
	NF_OA := nf.OA,
	nf.ON = false,
	setscore -6.83947643822884

simpx-lk--nf = simpx-lk--vc* nf %simpx-lk--vc[+nf]%
	NF_ON := nf.ON,
	setscore -6.1463292576689

simpx-lk--nf = simpx-lk--vc* nf %simpx-lk--vc[+nf]%
	nf.ON = false,
	setscore -2.0436858926321

simpx-lk--nf = simpx-lk--vc* nf %simpx-lk--vc[+nf]%
	nf.ON = false,
	nf.OPP = true,
	setscore -4.76003489654901

simpx-lk--nf = simpx-lk--vc* nf %simpx-lk--vc[+nf]%
	nf.ON = false,
	nf.PRED = true,
	setscore -4.64225186089262

simpx-lk--nf = simpx-lk--vc* nf %simpx-lk--vc[+nf]%
	nf.PRED = true,
	setscore -6.83947643822884

simpx-lk--nf = simpx-lk--vc* nf %simpx-lk--vc[+nf]%
	setscore -3.94910468033268

simpx-lk--nf = simpx-lk--vf* nf %simpx-lk--vf[+nf]%
	NF_ON := nf.ON,
	setscore -6.83947643822884

simpx-lk--nf = simpx-lk--vf* nf %simpx-lk--vf[+nf]%
	nf.ON = false,
	setscore -6.1463292576689

simpx-lk--nf = simpx-lk--vf* nf %simpx-lk--vf[+nf]%
	nf.ON = false,
	nf.PRED = true,
	setscore -6.83947643822884

simpx-lk--nf = simpx-lk--vf* nf %simpx-lk--vf[+nf]%
	setscore -5.74086414956073

simpx-lk--parord = parord simpx-lk* %simpx-lk[parord+]%
	setscore -4.58496747867057

simpx-lk--parord = parord simpx-lk--lv* %simpx-lk--lv[parord+]%
	setscore -3.48635519000246

simpx-lk--parord = parord simpx-lk--vf* %simpx-lk--vf[parord+]%
	setscore -0.0851578083403068

simpx-lk--simpx = simpx-lk--conj* simpx %simpx-lk--conj[+simpx:konj]%
	setscore 0

simpx-lk--vc = simpx-lk* vc %simpx-lk[+vc]%
	Dop3 == vc.Typ,
	setscore -5.66901848221986

simpx-lk--vc = simpx-lk* vc %simpx-lk[+vc]%
	setscore -7.05531284333975

simpx-lk--vc = simpx-lk--comma* vc %simpx-lk--comma[+vc]%
	Dop3 == vc.Typ,
	setscore -4.75272775034571

simpx-lk--vc = simpx-lk--comma* vc %simpx-lk--comma[+vc]%
	Dop3 == vc.Typ,
	vc.Dop1 == OA_C,
	vc.Dop2 == OD_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -7.05531284333975

simpx-lk--vc = simpx-lk--comma* vc %simpx-lk--comma[+vc]%
	Dop3 == vc.Typ,
	vc.Dop1 == OA_C,
	vc.Dop2 == OPP_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -7.05531284333975

simpx-lk--vc = simpx-lk--comma* vc %simpx-lk--comma[+vc]%
	Dop3 == vc.Typ,
	vc.Dop1 == OA_C,
	vc.Przyim1 == zero,
	setscore -5.2635533741117

simpx-lk--vc = simpx-lk--comma* vc %simpx-lk--comma[+vc]%
	Dop3 == vc.Typ,
	vc.Dop1 == OD_C,
	vc.Przyim1 == zero,
	setscore -6.36216566277981

simpx-lk--vc = simpx-lk--comma* vc %simpx-lk--comma[+vc]%
	Dop3 == vc.Typ,
	vc.Dop1 == OPP_C,
	vc.Przyim1 == OPP_Przyim,
	setscore -5.44587493090565

simpx-lk--vc = simpx-lk--comma* vc %simpx-lk--comma[+vc]%
	vc.Dop1 == OPP_C,
	vc.Przyim1 == OPP_Przyim,
	setscore -7.05531284333975

simpx-lk--vc = simpx-lk--comma* vc %simpx-lk--comma[+vc]%
	setscore -5.2635533741117

simpx-lk--vc = simpx-lk--fkoord* vc %simpx-lk--fkoord[+vc]%
	Dop3 == vc.Typ,
	setscore -5.66901848221986

simpx-lk--vc = simpx-lk--fkoord* vc %simpx-lk--fkoord[+vc]%
	Dop3 == vc.Typ,
	vc.Przyim1 == zero,
	setscore -6.36216566277981

simpx-lk--vc = simpx-lk--fkoord* vc %simpx-lk--fkoord[+vc]%
	setscore -5.95670055467164

simpx-lk--vc = simpx-lk--koord* vc %simpx-lk--koord[+vc]%
	Dop3 == vc.Typ,
	setscore -5.66901848221986

simpx-lk--vc = simpx-lk--koord* vc %simpx-lk--koord[+vc]%
	setscore -5.66901848221986

simpx-lk--vc = simpx-lk--lv* vc %simpx-lk--lv[+vc]%
	Dop3 == vc.Typ,
	setscore -6.36216566277981

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
	Dop3 == vc.Typ,
	setscore -1.72259405007438

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
	Dop3 == vc.Typ,
	vc.Dop1 == OA_C,
	vc.Dop2 == OD_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -3.0122615755052

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
	Dop3 == vc.Typ,
	vc.Dop1 == OA_C,
	vc.Dop2 == OG_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -5.66901848221986

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
	Dop3 == vc.Typ,
	vc.Dop1 == OA_C,
	vc.Dop2 == OPP_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -3.22667144685066

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
	Dop3 == vc.Typ,
	vc.Dop1 == OA_C,
	vc.Przyim1 == zero,
	setscore -1.867927037499

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
	Dop3 == vc.Typ,
	vc.Dop1 == OA_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -6.36216566277981

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
	Dop3 == vc.Typ,
	vc.Dop1 == OD_C,
	vc.Dop2 == OG_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -7.05531284333975

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
	Dop3 == vc.Typ,
	vc.Dop1 == OD_C,
	vc.Dop2 == OPP_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -4.65741757054138

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
	Dop3 == vc.Typ,
	vc.Dop1 == OD_C,
	vc.Przyim1 == zero,
	setscore -2.69860401665016

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
	Dop3 == vc.Typ,
	vc.Dop1 == OG_C,
	vc.Przyim1 == zero,
	setscore -5.44587493090565

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
	Dop3 == vc.Typ,
	vc.Dop1 == OPP_C,
	vc.Przyim1 == OPP_Przyim,
	setscore -2.79263296629844

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
	Dop3 == vc.Typ,
	vc.Dop2 == OPP_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -7.05531284333975

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
	Dop3 == vc.Typ,
	vc.Przyim1 == OPP_Przyim,
	setscore -5.10940269428444

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
	Dop3 == vc.Typ,
	vc.Przyim1 == zero,
	setscore -5.10940269428444

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OD_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -4.97587130165992

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OPP_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -5.10940269428444

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Przyim1 == zero,
	setscore -4.05958056978576

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
	vc.Dop1 == OD_C,
	vc.Dop2 == OPP_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -5.95670055467164

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
	vc.Dop1 == OD_C,
	vc.Przyim1 == zero,
	setscore -4.49036348587822

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
	vc.Dop1 == OPP_C,
	vc.Przyim1 == OPP_Przyim,
	setscore -4.41625551372449

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
	vc.Przyim1 == OPP_Przyim,
	setscore -7.05531284333975

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
	vc.Przyim1 == zero,
	setscore -7.05531284333975

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
	setscore -1.40986594569651

simpx-lk--vc = simpx-lk--parord* vc %simpx-lk--parord[+vc]%
	Dop3 == vc.Typ,
	setscore -7.05531284333975

simpx-lk--vc = simpx-lk--vf* vc %simpx-lk--vf[+vc]%
	Dop3 == vc.Typ,
	setscore -4.05958056978576

simpx-lk--vc = simpx-lk--vf* vc %simpx-lk--vf[+vc]%
	Dop3 == vc.Typ,
	vc.Dop1 == OA_C,
	vc.Przyim1 == zero,
	setscore -7.05531284333975

simpx-lk--vc = simpx-lk--vf* vc %simpx-lk--vf[+vc]%
	Dop3 == vc.Typ,
	vc.Dop1 == OD_C,
	vc.Przyim1 == zero,
	setscore -5.95670055467164

simpx-lk--vc = simpx-lk--vf* vc %simpx-lk--vf[+vc]%
	Dop3 == vc.Typ,
	vc.Dop1 == OPP_C,
	vc.Przyim1 == OPP_Przyim,
	setscore -7.05531284333975

simpx-lk--vc = simpx-lk--vf* vc %simpx-lk--vf[+vc]%
	Dop3 == vc.Typ,
	vc.Przyim1 == zero,
	setscore -7.05531284333975

simpx-lk--vc = simpx-lk--vf* vc %simpx-lk--vf[+vc]%
	vc.Dop1 == OD_C,
	vc.Przyim1 == zero,
	setscore -7.05531284333975

simpx-lk--vc = simpx-lk--vf* vc %simpx-lk--vf[+vc]%
	vc.Dop1 == OPP_C,
	vc.Przyim1 == OPP_Przyim,
	setscore -6.36216566277981

simpx-lk--vc = simpx-lk--vf* vc %simpx-lk--vf[+vc]%
	setscore -4.05958056978576

simpx-lk--vc = vc simpx-lk--vf* %simpx-lk--vf[vc+]%
	Dop1 := vc.Dop1,
	Dop3 == vc.Typ,
	vc.Przyim1 == zero,
	setscore -7.05531284333975

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	Dop1 == vf.OA_C,
	OA_C := vf.OA_C,
	VF_OA := vf.OA,
	vf.ON = false,
	setscore -5.57486626600748

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	Dop1 == vf.OA_C,
	VF_OA := vf.OA,
	setscore -5.72901694583474

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	Dop1 == vf.OA_C,
	VF_OA := vf.OA,
	vf.ON = false,
	setscore -3.51344322983033

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	Dop1 == vf.OD_C,
	OD_C := vf.OD_C,
	VF_OD := vf.OD,
	setscore -5.9113385026287

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	Dop1 == vf.OD_C,
	OD_C := vf.OD_C,
	VF_OD := vf.OD,
	vf.ON = false,
	setscore -6.82762923450285

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	Dop1 == vf.OD_C,
	VF_OD := vf.OD,
	setscore -5.32355183772658

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	Dop1 == vf.OD_C,
	VF_OD := vf.OD,
	vf.ON = false,
	setscore -4.95582705760126

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	Dop1 == vf.OG_C,
	VF_OG := vf.OG,
	vf.ON = false,
	setscore -7.5207764150628

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	Dop1 == vf.OPP_C,
	OPP_C := vf.OPP_C,
	OPP_Przyim := vf.OPP_Przyim,
	Przyim1 == vf.OPP_Przyim,
	vf.ON = false,
	vf.OPP = true,
	setscore -7.5207764150628

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	Dop1 == vf.OPP_C,
	OPP_C := vf.OPP_C,
	OPP_Przyim := vf.OPP_Przyim,
	Przyim1 == vf.OPP_Przyim,
	vf.OPP = true,
	setscore -6.42216412639469

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	Dop1 == vf.OPP_C,
	Przyim1 == vf.OPP_Przyim,
	vf.ON = false,
	vf.OPP = true,
	setscore -4.81272621396059

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	Dop1 == vf.OPP_C,
	Przyim1 == vf.OPP_Przyim,
	vf.OPP = true,
	setscore -5.9113385026287

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	Dop2 == vf.OD_C,
	VF_OD := vf.OD,
	vf.ON = false,
	setscore -6.13448205394291

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	Dop2 == vf.OPP_C,
	Przyim2 == vf.OPP_Przyim,
	vf.ON = false,
	vf.OPP = true,
	setscore -5.32355183772658

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	Num == vf.ON_Num,
	P == vf.ON_P,
	VF_ON := vf.ON,
	setscore -1.6543083581295

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	Num == vf.ON_Num,
	VF_ON := vf.ON,
	setscore -5.57486626600748

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	OA_C := vf.OA_C,
	VF_OA := vf.OA,
	setscore -7.5207764150628

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	OA_C := vf.OA_C,
	VF_OA := vf.OA,
	vf.ON = false,
	setscore -3.88319025533641

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	OD_C := vf.OD_C,
	VF_OD := vf.OD,
	setscore -5.57486626600748

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	OD_C := vf.OD_C,
	VF_OD := vf.OD,
	vf.ON = false,
	setscore -4.95582705760126

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	OG_C := vf.OG_C,
	VF_OG := vf.OG,
	vf.ON = false,
	setscore -7.5207764150628

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	OPP_C := vf.OPP_C,
	OPP_Przyim := vf.OPP_Przyim,
	vf.ON = false,
	vf.OPP = true,
	setscore -5.12288114226443

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	OPP_C := vf.OPP_C,
	OPP_Przyim := vf.OPP_Przyim,
	vf.OPP = true,
	setscore -5.9113385026287

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	P == vf.ON_P,
	VF_ON := vf.ON,
	setscore -4.95582705760126

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	Przyim1 == vf.OPP_Przyim,
	vf.ON = false,
	vf.OPP = true,
	setscore -6.82762923450285

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	VF_OA := vf.OA,
	setscore -7.5207764150628

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	VF_OA := vf.OA,
	vf.ON = false,
	setscore -5.57486626600748

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	VF_ON := vf.ON,
	setscore -2.48382381264917

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	vf.OADJP = true,
	vf.ON = false,
	setscore -6.42216412639469

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	vf.ON = false,
	setscore -1.54442550576486

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	vf.ON = false,
	vf.OPP = true,
	setscore -4.22493954905847

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	vf.ON = false,
	vf.PRED = true,
	setscore -3.58895078233847

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	vf.OPP = true,
	setscore -6.82762923450285

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	vf.PRED = true,
	setscore -4.95582705760126

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
	setscore -2.85733732095073

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
	Dop1 == vf.OA_C,
	VF_OA := vf.OA,
	setscore -7.5207764150628

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
	Dop1 == vf.OA_C,
	VF_OA := vf.OA,
	vf.ON = false,
	setscore -5.44133487338296

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
	Dop1 == vf.OD_C,
	VF_OD := vf.OD,
	vf.ON = false,
	setscore -6.42216412639469

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
	Dop1 == vf.OPP_C,
	Przyim1 == vf.OPP_Przyim,
	vf.ON = false,
	vf.OPP = true,
	setscore -5.9113385026287

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
	Dop2 == vf.OD_C,
	VF_OD := vf.OD,
	vf.ON = false,
	setscore -7.5207764150628

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
	Dop2 == vf.OPP_C,
	OPP_C := vf.OPP_C,
	OPP_Przyim := vf.OPP_Przyim,
	Przyim2 == vf.OPP_Przyim,
	vf.ON = false,
	vf.OPP = true,
	setscore -7.5207764150628

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
	Dop2 == vf.OPP_C,
	Przyim2 == vf.OPP_Przyim,
	vf.ON = false,
	vf.OPP = true,
	setscore -6.42216412639469

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
	Num == vf.ON_Num,
	P == vf.ON_P,
	VF_ON := vf.ON,
	setscore -2.86681606490527

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
	Num == vf.ON_Num,
	VF_ON := vf.ON,
	setscore -7.5207764150628

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
	OA_C := vf.OA_C,
	VF_OA := vf.OA,
	setscore -7.5207764150628

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
	OA_C := vf.OA_C,
	VF_OA := vf.OA,
	vf.ON = false,
	setscore -5.57486626600748

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
	OD_C := vf.OD_C,
	VF_OD := vf.OD,
	setscore -7.5207764150628

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
	OD_C := vf.OD_C,
	VF_OD := vf.OD,
	vf.ON = false,
	setscore -6.42216412639469

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
	OPP_C := vf.OPP_C,
	OPP_Przyim := vf.OPP_Przyim,
	vf.ON = false,
	vf.OPP = true,
	setscore -6.82762923450285

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
	P == vf.ON_P,
	VF_ON := vf.ON,
	setscore -5.12288114226443

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
	VF_OA := vf.OA,
	vf.ON = false,
	setscore -7.5207764150628

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
	VF_ON := vf.ON,
	setscore -3.88319025533641

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
	vf.ON = false,
	setscore -2.49033849367036

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
	vf.ON = false,
	vf.OPP = true,
	setscore -6.82762923450285

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
	vf.ON = false,
	vf.PRED = true,
	setscore -6.42216412639469

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
	vf.OPP = true,
	setscore -7.5207764150628

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
	setscore -3.06642911880929

simpx-lk--vf = vf simpx-lk--koord* %simpx-lk--koord[vf+]%
	vf.ON = false,
	setscore -7.5207764150628

simpx-lk--vf = vf simpx-lk--mf* %simpx-lk--mf[vf+]%
	Num == vf.ON_Num,
	P == vf.ON_P,
	VF_ON := vf.ON,
	setscore -6.82762923450285

simpx-lk--vf = vf simpx-lk--mf* %simpx-lk--mf[vf+]%
	vf.ON = false,
	setscore -6.82762923450285

simpx-lv = lv* %SIMPX[lv]%
	Block := 1,
	setscore 0

simpx-lv--comma = simpx-lv* comma %simpx-lv[+comma]%
	setscore -0.22314355131421

simpx-lv--comma = simpx-lv--mf* comma %simpx-lv--mf[+comma]%
	setscore -1.6094379124341

simpx-lv--fkoord = simpx-lv--vf* fkoord %simpx-lv--vf[+fkoord]%
	setscore 0

simpx-lv--mf = simpx-lv--vf* mf %simpx-lv--vf[+mf]%
	setscore 0

simpx-lv--nf = simpx-lv--comma* nf %simpx-lv--comma[+nf]%
	setscore 0

simpx-lv--vc = simpx-lv--vf* vc %simpx-lv--vf[+vc]%
	setscore 0

simpx-lv--vf = simpx-lv* vf %simpx-lv[+vf]%
	setscore -1.6094379124341

simpx-lv--vf = simpx-lv--comma* vf %simpx-lv--comma[+vf]%
	setscore -0.22314355131421

simpx-mf = mf* %SIMPX[mf:konj]%
	Block := 1,
	setscore -4.33073334028633

simpx-mf = mf* %SIMPX[mf]%
	Block := 1,
	setscore -1.62268313918412

simpx-mf = mf* %SIMPX[mf]%
	Block := 1,
	MF_OA := OA,
	setscore -2.02814824729229

simpx-mf = mf* %SIMPX[mf]%
	Block := 1,
	MF_OA := OA,
	MF_OD := OD,
	setscore -2.94443897916644

simpx-mf = mf* %SIMPX[mf]%
	Block := 1,
	MF_OA := OA,
	MF_OD := OD,
	MF_ON := ON,
	setscore -4.33073334028633

simpx-mf = mf* %SIMPX[mf]%
	Block := 1,
	MF_OA := OA,
	MF_OD := OD,
	OPP = true,
	setscore -3.23212105161822

simpx-mf = mf* %SIMPX[mf]%
	Block := 1,
	MF_OA := OA,
	MF_OD := OD,
	PRED = true,
	setscore -4.33073334028633

simpx-mf = mf* %SIMPX[mf]%
	Block := 1,
	MF_OA := OA,
	MF_OG := OG,
	setscore -4.33073334028633

simpx-mf = mf* %SIMPX[mf]%
	Block := 1,
	MF_OA := OA,
	MF_ON := ON,
	setscore -4.33073334028633

simpx-mf = mf* %SIMPX[mf]%
	Block := 1,
	MF_OA := OA,
	MF_ON := ON,
	OPP = true,
	setscore -4.33073334028633

simpx-mf = mf* %SIMPX[mf]%
	Block := 1,
	MF_OA := OA,
	OPP = true,
	setscore -2.38482319123102

simpx-mf = mf* %SIMPX[mf]%
	Block := 1,
	MF_OA := OA,
	OPP = true,
	PRED = true,
	setscore -3.63758615972639

simpx-mf = mf* %SIMPX[mf]%
	Block := 1,
	MF_OA := OA,
	PRED = true,
	setscore -3.23212105161822

simpx-mf = mf* %SIMPX[mf]%
	Block := 1,
	MF_OD := OD,
	setscore -3.23212105161822

simpx-mf = mf* %SIMPX[mf]%
	Block := 1,
	MF_OD := OD,
	OPP = true,
	setscore -3.63758615972639

simpx-mf = mf* %SIMPX[mf]%
	Block := 1,
	MF_OD := OD,
	PRED = true,
	setscore -4.33073334028633

simpx-mf = mf* %SIMPX[mf]%
	Block := 1,
	MF_OG := OG,
	setscore -4.33073334028633

simpx-mf = mf* %SIMPX[mf]%
	Block := 1,
	MF_ON := ON,
	setscore -2.72129542785223

simpx-mf = mf* %SIMPX[mf]%
	Block := 1,
	MF_ON := ON,
	OPP = true,
	setscore -4.33073334028633

simpx-mf = mf* %SIMPX[mf]%
	Block := 1,
	MF_ON := ON,
	PRED = true,
	setscore -4.33073334028633

simpx-mf = mf* %SIMPX[mf]%
	Block := 1,
	ON = false,
	setscore -4.33073334028633

simpx-mf = mf* %SIMPX[mf]%
	Block := 1,
	OPP = true,
	setscore -2.38482319123102

simpx-mf = mf* %SIMPX[mf]%
	Block := 1,
	OPP = true,
	PRED = true,
	setscore -4.33073334028633

simpx-mf = mf* %SIMPX[mf]%
	Block := 1,
	PRED = true,
	setscore -3.23212105161822

simpx-mf--c = simpx-mf* c %simpx-mf[+c]%
	C_OA := c.OA,
	OA_C := c.OA_C,
	c.ON = false,
	setscore -0.693147180559945

simpx-mf--c = simpx-mf* c %simpx-mf[+c]%
	c.CONJ == 1,
	c.ON = false,
	setscore -0.693147180559945

simpx-mf--comma = simpx-mf* comma %simpx-mf[+comma]%
	setscore -1.34373474670109

simpx-mf--comma = simpx-mf--vc* comma %simpx-mf--vc[+comma]%
	setscore -0.302280871872934

simpx-mf--fkoord = simpx-mf* fkoord %simpx-mf[+fkoord]%
	setscore 0

simpx-mf--mf = simpx-mf--comma* mf %simpx-mf--comma[+mf:konj]%
	setscore 0

simpx-mf--nf = simpx-mf* nf %simpx-mf[+nf]%
	setscore -3.61091791264422

simpx-mf--nf = simpx-mf--comma* nf %simpx-mf--comma[+nf]%
	NF_OA := nf.OA,
	setscore -3.61091791264422

simpx-mf--nf = simpx-mf--comma* nf %simpx-mf--comma[+nf]%
	nf.ON = false,
	nf.PRED = true,
	setscore -3.61091791264422

simpx-mf--nf = simpx-mf--comma* nf %simpx-mf--comma[+nf]%
	setscore -0.838329190404443

simpx-mf--nf = simpx-mf--vc* nf %simpx-mf--vc[+nf]%
	NF_OA := nf.OA,
	setscore -3.61091791264422

simpx-mf--nf = simpx-mf--vc* nf %simpx-mf--vc[+nf]%
	NF_ON := nf.ON,
	setscore -3.61091791264422

simpx-mf--nf = simpx-mf--vc* nf %simpx-mf--vc[+nf]%
	nf.ON = false,
	setscore -3.61091791264422

simpx-mf--nf = simpx-mf--vc* nf %simpx-mf--vc[+nf]%
	nf.OPP = true,
	setscore -3.61091791264422

simpx-mf--nf = simpx-mf--vc* nf %simpx-mf--vc[+nf]%
	nf.PRED = true,
	setscore -3.61091791264422

simpx-mf--nf = simpx-mf--vc* nf %simpx-mf--vc[+nf]%
	setscore -1.21302263984585

simpx-mf--vc = simpx-mf* vc %simpx-mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OD_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -4.26267987704132

simpx-mf--vc = simpx-mf* vc %simpx-mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OD_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -2.18323833536148

simpx-mf--vc = simpx-mf* vc %simpx-mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OG_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == zero,
	setscore -4.26267987704132

simpx-mf--vc = simpx-mf* vc %simpx-mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OPP_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -4.26267987704132

simpx-mf--vc = simpx-mf* vc %simpx-mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Dop2 == OPP_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -2.65324196460722

simpx-mf--vc = simpx-mf* vc %simpx-mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Przyim1 == zero,
	setscore -2.18323833536148

simpx-mf--vc = simpx-mf* vc %simpx-mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -4.26267987704132

simpx-mf--vc = simpx-mf* vc %simpx-mf[+vc]%
	vc.Dop1 == OD_C,
	vc.Dop2 == OPP_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -3.56953269648137

simpx-mf--vc = simpx-mf* vc %simpx-mf[+vc]%
	vc.Dop1 == OD_C,
	vc.Przyim1 == zero,
	setscore -2.87638551592142

simpx-mf--vc = simpx-mf* vc %simpx-mf[+vc]%
	vc.Dop1 == OG_C,
	vc.Przyim1 == zero,
	setscore -4.26267987704132

simpx-mf--vc = simpx-mf* vc %simpx-mf[+vc]%
	vc.Dop1 == OPP_C,
	vc.Przyim1 == OPP_Przyim,
	setscore -2.47092040781326

simpx-mf--vc = simpx-mf* vc %simpx-mf[+vc]%
	vc.Dop2 == OPP_C,
	vc.Przyim1 == zero,
	vc.Przyim2 == OPP_Przyim,
	setscore -4.26267987704132

simpx-mf--vc = simpx-mf* vc %simpx-mf[+vc]%
	vc.Num == ON_Num,
	vc.P == ON_P,
	setscore -3.56953269648137

simpx-mf--vc = simpx-mf* vc %simpx-mf[+vc]%
	vc.Przyim1 == zero,
	setscore -2.65324196460722

simpx-mf--vc = simpx-mf* vc %simpx-mf[+vc]%
	setscore -1.49009115480153

simpx-mf--vc = simpx-mf--c* vc %simpx-mf--c[+vc]%
	vc.Dop1 == OA_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	setscore -4.26267987704132

simpx-mf--vc = simpx-mf--c* vc %simpx-mf--c[+vc]%
	vc.Dop1 == OA_C,
	vc.Przyim1 == zero,
	setscore -4.26267987704132

simpx-mf--vc = simpx-mf--comma* vc %simpx-mf--comma[+vc]%
	vc.Dop1 == OA_C,
	vc.Przyim1 == zero,
	setscore -3.56953269648137

simpx-mf--vc = simpx-mf--comma* vc %simpx-mf--comma[+vc]%
	setscore -3.56953269648137

simpx-nf = nf* %SIMPX[nf]%
	Block := 1,
	setscore 0

simpx-nf--comma = simpx-nf--vc* comma %simpx-nf--vc[+comma]%
	setscore 0

simpx-nf--mf = simpx-nf* mf %simpx-nf[+mf]%
	setscore 0

simpx-nf--vc = simpx-nf--mf* vc %simpx-nf--mf[+vc]%
	setscore 0

simpx-p-simpx = p-simpx* %SIMPX[p-simpx:konj]%
	Block := 1,
	setscore 0

simpx-p-simpx--comma = simpx-p-simpx* comma %simpx-p-simpx[+comma]%
	setscore 0

simpx-p-simpx--conj = simpx-p-simpx* conj %simpx-p-simpx[+conj]%
	setscore -0.693147180559945

simpx-p-simpx--conj = simpx-p-simpx--comma* conj %simpx-p-simpx--comma[+conj]%
	setscore -0.693147180559945

simpx-p-simpx--simpx = simpx-p-simpx* simpx %simpx-p-simpx[+simpx:konj]%
	setscore -1.38629436111989

simpx-p-simpx--simpx = simpx-p-simpx--comma* simpx %simpx-p-simpx--comma[+simpx:konj]%
	setscore -1.38629436111989

simpx-p-simpx--simpx = simpx-p-simpx--conj* simpx %simpx-p-simpx--conj[+simpx:konj]%
	setscore -0.693147180559945

simpx-parord = parord* %SIMPX[parord]%
	Block := 1,
	setscore 0

simpx-parord--fkoord = simpx-parord* fkoord %simpx-parord[+fkoord]%
	setscore -0.693147180559945

simpx-parord--fkoord = simpx-parord--vf* fkoord %simpx-parord--vf[+fkoord]%
	setscore -0.693147180559945

simpx-parord--mf = simpx-parord* mf %simpx-parord[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	setscore 0

simpx-parord--vc = simpx-parord--mf* vc %simpx-parord--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	setscore 0

simpx-parord--vf = simpx-parord* vf %simpx-parord[+vf]%
	setscore 0

simpx-simpx = simpx* %SIMPX[simpx:konj]%
	Block := 1,
	setscore -0.307484699747961

simpx-simpx = simpx* %SIMPX[simpx]%
	Block := 1,
	setscore -1.32913594727994

simpx-simpx--advx = simpx-simpx--advx* advx %simpx-simpx--advx[+advx:konj]%
	setscore -1.09861228866811

simpx-simpx--advx = simpx-simpx--conj* advx %simpx-simpx--conj[+advx]%
	setscore -1.09861228866811

simpx-simpx--advx = simpx-simpx--simpx* advx %simpx-simpx--simpx[+advx:konj]%
	setscore -1.09861228866811

simpx-simpx--ap = simpx-simpx--conj* ap %simpx-simpx--conj[+ap:konj]%
	setscore 0

simpx-simpx--comma = simpx-simpx* comma %simpx-simpx[+comma]%
	setscore -0.251314428280906

simpx-simpx--comma = simpx-simpx--p-simpx* comma %simpx-simpx--p-simpx[+comma]%
	setscore -2.89037175789617

simpx-simpx--comma = simpx-simpx--simpx* comma %simpx-simpx--simpx[+comma]%
	setscore -1.79175946922806

simpx-simpx--conj = conj simpx-simpx* %simpx-simpx[conj+]%
	setscore -2.94443897916644

simpx-simpx--conj = simpx-simpx* conj %simpx-simpx[+conj]%
	setscore -1.15267950993839

simpx-simpx--conj = simpx-simpx--comma* conj %simpx-simpx--comma[+conj]%
	setscore -0.998528830111127

simpx-simpx--conj = simpx-simpx--np* conj %simpx-simpx--np[+conj]%
	setscore -2.94443897916644

simpx-simpx--conj = simpx-simpx--simpx* conj %simpx-simpx--simpx[+conj]%
	setscore -2.2512917986065

simpx-simpx--np = simpx-simpx* np %simpx-simpx[+np]%
	setscore -1.94591014905531

simpx-simpx--np = simpx-simpx--comma* np %simpx-simpx--comma[+np:konj]%
	setscore -1.25276296849537

simpx-simpx--np = simpx-simpx--comma* np %simpx-simpx--comma[+np]%
	setscore -1.94591014905531

simpx-simpx--np = simpx-simpx--conj* np %simpx-simpx--conj[+np:konj]%
	setscore -1.25276296849537

simpx-simpx--p-simpx = simpx-simpx* p-simpx %simpx-simpx[+p-simpx:konj]%
	setscore -1.09861228866811

simpx-simpx--p-simpx = simpx-simpx--conj* p-simpx %simpx-simpx--conj[+p-simpx:konj]%
	setscore -0.405465108108164

simpx-simpx--pp = pp simpx-simpx* %simpx-simpx[pp:compl1+]%
	setscore -1.38629436111989

simpx-simpx--pp = simpx-simpx--comma* pp %simpx-simpx--comma[+pp:konj]%
	setscore -1.38629436111989

simpx-simpx--pp = simpx-simpx--conj* pp %simpx-simpx--conj[+pp:konj]%
	setscore -1.38629436111989

simpx-simpx--simpx = simpx-simpx* simpx %simpx-simpx[+simpx:konj]%
	setscore -1.6094379124341

simpx-simpx--simpx = simpx-simpx* simpx %simpx-simpx[+simpx]%
	setscore -2.99573227355399

simpx-simpx--simpx = simpx-simpx--advx* simpx %simpx-simpx--advx[+simpx:konj]%
	setscore -2.99573227355399

simpx-simpx--simpx = simpx-simpx--comma* simpx %simpx-simpx--comma[+simpx:konj]%
	setscore -1.38629436111989

simpx-simpx--simpx = simpx-simpx--comma* simpx %simpx-simpx--comma[+simpx]%
	setscore -2.99573227355399

simpx-simpx--simpx = simpx-simpx--conj* simpx %simpx-simpx--conj[+simpx:konj]%
	setscore -1.6094379124341

simpx-vc = vc* %SIMPX[vc]%
	Block := 1,
	setscore -0.336472236621213

simpx-vc = vc* %SIMPX[vc]%
	Block := 1,
	Przyim1 == zero,
	setscore -1.25276296849537

simpx-vc--comma = simpx-vc* comma %simpx-vc[+comma]%
	setscore 0

simpx-vc--mf = simpx-vc* mf %simpx-vc[+mf]%
	Dop1 == mf.OA_C,
	MF_OA := mf.OA,
	setscore 0

simpx-vc--nf = simpx-vc* nf %simpx-vc[+nf]%
	nf.OPP = true,
	setscore -1.6094379124341

simpx-vc--nf = simpx-vc* nf %simpx-vc[+nf]%
	nf.PRED = true,
	setscore -1.6094379124341

simpx-vc--nf = simpx-vc* nf %simpx-vc[+nf]%
	setscore -1.6094379124341

simpx-vc--nf = simpx-vc--comma* nf %simpx-vc--comma[+nf]%
	setscore -1.6094379124341

simpx-vf = vf* %SIMPX[vf]%
	Block := 1,
	setscore -0.53062825106217

simpx-vf = vf* %SIMPX[vf]%
	Block := 1,
	ON = false,
	setscore -2.42774823594805

simpx-vf = vf* %SIMPX[vf]%
	Block := 1,
	VF_OA := OA,
	setscore -2.83321334405622

simpx-vf = vf* %SIMPX[vf]%
	Block := 1,
	VF_ON := ON,
	setscore -1.32913594727994

simpx-vf--comma = simpx-vf* comma %simpx-vf[+comma]%
	setscore -0.693147180559945

simpx-vf--comma = simpx-vf--fkoord* comma %simpx-vf--fkoord[+comma]%
	setscore -1.79175946922806

simpx-vf--comma = simpx-vf--mf* comma %simpx-vf--mf[+comma]%
	setscore -2.484906649788

simpx-vf--comma = simpx-vf--vc* comma %simpx-vf--vc[+comma]%
	setscore -1.38629436111989

simpx-vf--fkoord = simpx-vf* fkoord %simpx-vf[+fkoord]%
	setscore -0.693147180559945

simpx-vf--fkoord = simpx-vf--comma* fkoord %simpx-vf--comma[+fkoord]%
	setscore -0.693147180559945

simpx-vf--mf = simpx-vf* mf %simpx-vf[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	setscore -2.94443897916644

simpx-vf--mf = simpx-vf* mf %simpx-vf[+mf]%
	MF_OA := mf.OA,
	MF_ON := mf.ON,
	OA_C := mf.OA_C,
	ON_Num := mf.ON_Num,
	ON_P := mf.ON_P,
	setscore -2.94443897916644

simpx-vf--mf = simpx-vf* mf %simpx-vf[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	setscore -2.2512917986065

simpx-vf--mf = simpx-vf* mf %simpx-vf[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.ON = false,
	setscore -2.94443897916644

simpx-vf--mf = simpx-vf* mf %simpx-vf[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.PRED = true,
	setscore -2.94443897916644

simpx-vf--mf = simpx-vf* mf %simpx-vf[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	setscore -2.94443897916644

simpx-vf--mf = simpx-vf* mf %simpx-vf[+mf]%
	MF_OD := mf.OD,
	OD_C := mf.OD_C,
	mf.ON = false,
	setscore -2.94443897916644

simpx-vf--mf = simpx-vf* mf %simpx-vf[+mf]%
	MF_ON := mf.ON,
	setscore -2.94443897916644

simpx-vf--mf = simpx-vf* mf %simpx-vf[+mf]%
	OPP_C := mf.OPP_C,
	OPP_Przyim := mf.OPP_Przyim,
	mf.ON = false,
	mf.OPP = true,
	setscore -2.94443897916644

simpx-vf--mf = simpx-vf* mf %simpx-vf[+mf]%
	mf.ON = false,
	setscore -2.2512917986065

simpx-vf--mf = simpx-vf* mf %simpx-vf[+mf]%
	mf.ON = false,
	mf.PRED = true,
	setscore -2.94443897916644

simpx-vf--mf = simpx-vf* mf %simpx-vf[+mf]%
	setscore -1.84582669049833

simpx-vf--mf = simpx-vf--comma* mf %simpx-vf--comma[+mf]%
	MF_OA := mf.OA,
	OA_C := mf.OA_C,
	mf.ON = false,
	setscore -2.94443897916644

simpx-vf--mf = simpx-vf--comma* mf %simpx-vf--comma[+mf]%
	setscore -2.94443897916644

simpx-vf--nf = simpx-vf* nf %simpx-vf[+nf]%
	setscore -2.30258509299405

simpx-vf--nf = simpx-vf--comma* nf %simpx-vf--comma[+nf]%
	nf.ON = false,
	setscore -2.30258509299405

simpx-vf--nf = simpx-vf--comma* nf %simpx-vf--comma[+nf]%
	setscore -0.510825623765991

simpx-vf--nf = simpx-vf--fkoord* nf %simpx-vf--fkoord[+nf]%
	setscore -2.30258509299405

simpx-vf--nf = simpx-vf--vc* nf %simpx-vf--vc[+nf]%
	setscore -2.30258509299405

simpx-vf--vc = simpx-vf* vc %simpx-vf[+vc]%
	vc.Dop1 == OA_C,
	vc.Przyim1 == zero,
	setscore -2.39789527279837

simpx-vf--vc = simpx-vf* vc %simpx-vf[+vc]%
	vc.Num == ON_Num,
	vc.P == ON_P,
	setscore -3.09104245335832

simpx-vf--vc = simpx-vf* vc %simpx-vf[+vc]%
	setscore -1.99243016469021

simpx-vf--vc = simpx-vf--fkoord* vc %simpx-vf--fkoord[+vc]%
	setscore -3.09104245335832

simpx-vf--vc = simpx-vf--mf* vc %simpx-vf--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Num == ON_Num,
	vc.P == ON_P,
	vc.Przyim1 == zero,
	setscore -3.09104245335832

simpx-vf--vc = simpx-vf--mf* vc %simpx-vf--mf[+vc]%
	vc.Dop1 == OA_C,
	vc.Przyim1 == zero,
	setscore -1.29928298413026

simpx-vf--vc = simpx-vf--mf* vc %simpx-vf--mf[+vc]%
	vc.Dop1 == OD_C,
	vc.Przyim1 == zero,
	setscore -2.39789527279837

simpx-vf--vc = simpx-vf--mf* vc %simpx-vf--mf[+vc]%
	vc.Dop1 == OPP_C,
	vc.Przyim1 == OPP_Przyim,
	setscore -3.09104245335832

simpx-vf--vc = simpx-vf--mf* vc %simpx-vf--mf[+vc]%
	vc.Num == ON_Num,
	vc.P == ON_P,
	setscore -3.09104245335832

simpx-vf--vc = simpx-vf--mf* vc %simpx-vf--mf[+vc]%
	setscore -1.70474809223843

vc = vc-part*
	setscore -3.66356164612965

vc = vc-vp*
	setscore -0.405465108108164

vc = vc-vp--vp*
	setscore -1.58412010444981

vc-part = part* %VC[part:vpt]%
	Block := 1,
	T == prefix,
	VPT := Core,
	setscore 0

vc-vp = vp* %VC[vp:compl3]%
	Block := 1,
	setscore -3.63758615972639

vc-vp = vp* %VC[vp:compl3]%
	Block := 1,
	Typ == indic_pres,
	setscore -3.63758615972639

vc-vp = vp* %VC[vp:compl3]%
	Block := 1,
	Typ == inf,
	setscore -2.94443897916644

vc-vp = vp* %VC[vp:compl3]%
	Block := 1,
	Typ == part,
	setscore -3.63758615972639

vc-vp = vp* %VC[vp:compl3]%
	Block := 1,
	Typ == zuinf,
	setscore -3.63758615972639

vc-vp = vp* %VC[vp:ov]%
	Block := 1,
	setscore -2.94443897916644

vc-vp = vp* %VC[vp:ov]%
	Block := 1,
	Typ == indic_past,
	setscore -3.63758615972639

vc-vp = vp* %VC[vp:ov]%
	Block := 1,
	Typ == inf,
	setscore -2.2512917986065

vc-vp = vp* %VC[vp:ov]%
	Block := 1,
	Typ == part,
	setscore -2.53897387105828

vc-vp = vp* %VC[vp:ov]%
	Block := 1,
	Typ == zuinf,
	setscore -2.53897387105828

vc-vp = vp* %VC[vp]%
	Block := 1,
	setscore -1.15267950993839

vc-vp = vp* %VC[vp]%
	Block := 1,
	Typ == cond_past,
	setscore -3.63758615972639

vc-vp = vp* %VC[vp]%
	Block := 1,
	Typ == cond_pres,
	setscore -3.63758615972639

vc-vp = vp* %VC[vp]%
	Block := 1,
	Typ == indic_past,
	setscore -3.63758615972639

vc-vp = vp* %VC[vp]%
	Block := 1,
	Typ == indic_pres,
	setscore -3.63758615972639

vc-vp = vp* %VC[vp]%
	Block := 1,
	Typ == inf,
	setscore -3.63758615972639

vc-vp = vp* %VC[vp]%
	Block := 1,
	Typ == part,
	setscore -3.63758615972639

vc-vp = vp* %VC[vp]%
	Block := 1,
	Typ == zuinf,
	setscore -3.63758615972639

vc-vp--vp = vc-vp* vp %vc-vp[+vp:compl3]%
	vp.Typ == inf,
	setscore -2.484906649788

vc-vp--vp = vc-vp* vp %vc-vp[+vp:compl3]%
	vp.Typ == part,
	setscore -2.484906649788

vc-vp--vp = vc-vp* vp %vc-vp[+vp:compl3]%
	setscore -2.484906649788

vc-vp--vp = vp vc-vp* %vc-vp[vp:compl3+]%
	vp.Typ == indic_pres,
	setscore -2.484906649788

vc-vp--vp = vp vc-vp* %vc-vp[vp:compl3+]%
	vp.Typ == inf,
	setscore -2.484906649788

vc-vp--vp = vp vc-vp* %vc-vp[vp:compl3+]%
	vp.Typ == part,
	setscore -2.484906649788

vc-vp--vp = vp vc-vp* %vc-vp[vp:compl3+]%
	vp.Typ == zuinf,
	setscore -2.484906649788

vc-vp--vp = vp vc-vp* %vc-vp[vp:compl3+]%
	setscore -2.484906649788

vce = vce-vp*
	{ Block := false },
	setscore 0

vce-vp = vp* %VCE[vp]%
	Block := 1,
	setscore 0

vf = vf-advx*
	setscore -2.28819635554195

vf = vf-ap*
	setscore -2.4423470353692

vf = vf-fkonj*
	setscore -4.23410650459726

vf = vf-fkoord*
	setscore -4.23410650459726

vf = vf-fx*
	setscore -2.28819635554195

vf = vf-mf*
	setscore -4.23410650459726

vf = vf-np*
	setscore -1.23837423104327

vf = vf-p-simpx*
	setscore -4.23410650459726

vf = vf-pp*
	setscore -1.595049174982

vf = vf-simpx*
	setscore -1.93152141160321

vf = vf-vc*
	setscore -4.23410650459726

vf-advx = advx* %VF[advx:mod-mod]%
	Block := 1,
	setscore -1.94591014905531

vf-advx = advx* %VF[advx:mod]%
	Block := 1,
	setscore -1.94591014905531

vf-advx = advx* %VF[advx:oadjp]%
	Block := 1,
	setscore -1.94591014905531

vf-advx = advx* %VF[advx:oadvp]%
	Block := 1,
	setscore -1.94591014905531

vf-advx = advx* %VF[advx:pred-mod]%
	Block := 1,
	setscore -1.94591014905531

vf-advx = advx* %VF[advx:pred]%
	Block := 1,
	PRED := true,
	setscore -1.94591014905531

vf-advx = advx* %VF[advx:v-mod]%
	Block := 1,
	setscore -1.94591014905531

vf-ap = ap* %VF[ap:mod]%
	Block := 1,
	setscore -1.79175946922806

vf-ap = ap* %VF[ap:oadjp]%
	Block := 1,
	OADJP := true,
	T == rel,
	setscore -1.79175946922806

vf-ap = ap* %VF[ap:on-mod]%
	Block := 1,
	setscore -1.79175946922806

vf-ap = ap* %VF[ap:pred]%
	Block := 1,
	PRED := true,
	setscore -1.79175946922806

vf-ap = ap* %VF[ap:subject]%
	Block := 1,
	setscore -1.79175946922806

vf-ap = ap* %VF[ap:v-mod]%
	Block := 1,
	setscore -1.79175946922806

vf-fkonj = fkonj* %VF[fkonj]%
	Block := 1,
	setscore 0

vf-fkoord = fkoord* %VF[fkoord]%
	Block := 1,
	setscore 0

vf-fx = fx* %VF[fx:compl1]%
	Block := 1,
	setscore -1.94591014905531

vf-fx = fx* %VF[fx:mod]%
	Block := 1,
	setscore -1.94591014905531

vf-fx = fx* %VF[fx:oa]%
	Block := 1,
	setscore -1.94591014905531

vf-fx = fx* %VF[fx:os]%
	Block := 1,
	setscore -1.94591014905531

vf-fx = fx* %VF[fx:pred]%
	Block := 1,
	setscore -1.94591014905531

vf-fx = fx* %VF[fx:subject]%
	Block := 1,
	setscore -1.94591014905531

vf-fx = fx* %VF[fx:v-mod]%
	Block := 1,
	setscore -1.94591014905531

vf-mf = mf* %VF[mf]%
	Block := 1,
	setscore 0

vf-np = np* %VF[np:compl1]%
	Block := 1,
	setscore -2.99573227355399

vf-np = np* %VF[np:compl2]%
	Block := 1,
	setscore -2.99573227355399

vf-np = np* %VF[np:es]%
	Block := 1,
	ES := true,
	setscore -2.99573227355399

vf-np = np* %VF[np:mod]%
	Block := 1,
	setscore -2.99573227355399

vf-np = np* %VF[np:oa-mod]%
	Block := 1,
	setscore -2.99573227355399

vf-np = np* %VF[np:oa]%
	Block := 1,
	C == acc,
	OA := true,
	OA_C := C,
	setscore -2.99573227355399

vf-np = np* %VF[np:oa]%
	Block := 1,
	C == dat,
	OA := true,
	OA_C := C,
	setscore -2.99573227355399

vf-np = np* %VF[np:oa]%
	Block := 1,
	C == nom,
	OA := true,
	OA_C := C,
	setscore -2.99573227355399

vf-np = np* %VF[np:oa]%
	Block := 1,
	OA := true,
	setscore -2.99573227355399

vf-np = np* %VF[np:oa]%
	Block := 1,
	OA := true,
	OA_C := C,
	setscore -2.99573227355399

vf-np = np* %VF[np:od-mod]%
	Block := 1,
	setscore -2.99573227355399

vf-np = np* %VF[np:od]%
	Block := 1,
	C == dat,
	OD := true,
	OD_C := C,
	setscore -2.99573227355399

vf-np = np* %VF[np:od]%
	Block := 1,
	C == nom,
	OD := true,
	OD_C := C,
	setscore -2.99573227355399

vf-np = np* %VF[np:on-mod]%
	Block := 1,
	setscore -2.99573227355399

vf-np = np* %VF[np:os]%
	Block := 1,
	setscore -2.99573227355399

vf-np = np* %VF[np:pred-mod]%
	Block := 1,
	setscore -2.99573227355399

vf-np = np* %VF[np:pred]%
	Block := 1,
	PRED := true,
	setscore -2.99573227355399

vf-np = np* %VF[np:subject]%
	Block := 1,
	setscore -2.99573227355399

vf-np = np* %VF[np:v-mod]%
	Block := 1,
	setscore -2.99573227355399

vf-np = np* %VF[np]%
	Block := 1,
	setscore -2.99573227355399

vf-p-simpx = p-simpx* %VF[p-simpx:os]%
	Block := 1,
	setscore 0

vf-pp = pp* %VF[pp:compl1]%
	Block := 1,
	setscore -2.63905732961526

vf-pp = pp* %VF[pp:compl2]%
	Block := 1,
	setscore -2.63905732961526

vf-pp = pp* %VF[pp:fopp]%
	Block := 1,
	setscore -2.63905732961526

vf-pp = pp* %VF[pp:mod-mod]%
	Block := 1,
	setscore -2.63905732961526

vf-pp = pp* %VF[pp:mod]%
	Block := 1,
	setscore -2.63905732961526

vf-pp = pp* %VF[pp:oa-mod]%
	Block := 1,
	setscore -2.63905732961526

vf-pp = pp* %VF[pp:on-mod]%
	Block := 1,
	setscore -2.63905732961526

vf-pp = pp* %VF[pp:opp-mod]%
	Block := 1,
	setscore -2.63905732961526

vf-pp = pp* %VF[pp:opp]%
	Block := 1,
	OPP := true,
	OPP_C := C,
	OPP_Przyim := Przyim,
	setscore -2.63905732961526

vf-pp = pp* %VF[pp:os-mod]%
	Block := 1,
	setscore -2.63905732961526

vf-pp = pp* %VF[pp:pred-mod]%
	Block := 1,
	setscore -2.63905732961526

vf-pp = pp* %VF[pp:pred]%
	Block := 1,
	PRED := true,
	setscore -2.63905732961526

vf-pp = pp* %VF[pp:subject]%
	Block := 1,
	setscore -2.63905732961526

vf-pp = pp* %VF[pp:v-mod]%
	Block := 1,
	setscore -2.63905732961526

vf-simpx = simpx* %VF[simpx:fopp-mod]%
	Block := 1,
	setscore -2.30258509299405

vf-simpx = simpx* %VF[simpx:mod-mod]%
	Block := 1,
	setscore -2.30258509299405

vf-simpx = simpx* %VF[simpx:mod]%
	Block := 1,
	setscore -2.30258509299405

vf-simpx = simpx* %VF[simpx:oa-mod]%
	Block := 1,
	setscore -2.30258509299405

vf-simpx = simpx* %VF[simpx:on-mod]%
	Block := 1,
	setscore -2.30258509299405

vf-simpx = simpx* %VF[simpx:os]%
	Block := 1,
	setscore -2.30258509299405

vf-simpx = simpx* %VF[simpx:pred]%
	Block := 1,
	PRED := true,
	setscore -2.30258509299405

vf-simpx = simpx* %VF[simpx:subject]%
	Block := 1,
	setscore -2.30258509299405

vf-simpx = simpx* %VF[simpx:v-mod]%
	Block := 1,
	setscore -2.30258509299405

vf-simpx = simpx* %VF[simpx]%
	Block := 1,
	setscore -2.30258509299405

vf-vc = vc* %VF[vc]%
	Block := 1,
	setscore 0

vp = vp-conj--vp*
	{ Block := false },
	setscore -2.99573227355399

vp = vp-part*
	{ Block := false },
	setscore -2.99573227355399

vp = vp-part--verb*
	{ Block := false },
	setscore -1.20397280432594

vp = vp-trunc--verb*
	{ Block := false },
	setscore -2.30258509299405

vp = vp-verb*
	{ Block := false },
	setscore -2.99573227355399

vp = vp-verb--part*
	{ Block := false },
	setscore -2.99573227355399

vp = vp-vp--vp*
	{ Block := false },
	setscore -1.6094379124341

vp-advx = advx* %VP[advx]%
	Block := 1,
	setscore 0

vp-advx--comma = vp-advx--vp* comma %vp-advx--vp[+comma]%
	setscore 0

vp-advx--conj = vp-advx--comma* conj %vp-advx--comma[+conj]%
	setscore 0

vp-advx--vp = vp-advx* vp %vp-advx[+vp:konj]%
	setscore -0.693147180559945

vp-conj = conj* %VP[conj]%
	Block := 1,
	setscore 0

vp-conj--conj = vp-conj--vp* conj %vp-conj--vp[+conj]%
	setscore 0

vp-conj--vp = vp-conj* vp %vp-conj[+vp:konj]%
	setscore -0.693147180559945

vp-conj--vp = vp-conj--conj* vp %vp-conj--conj[+vp:konj]%
	setscore -0.693147180559945

vp-part = part* %VP[part]%
	Block := 1,
	T == zu,
	To := 1,
	setscore 0

vp-part--verb = vp-part* verb %vp-part[+verb]%
	Dop1 := verb.Dop1,
	Dop2 := verb.Dop2,
	Dop3 := verb.Dop3,
	Przyim1 := verb.Przyim1,
	Przyim2 := verb.Przyim2,
	Przyim3 := verb.Przyim3,
	Typ := verb.Typ,
	setscore -1.94591014905531

vp-part--verb = vp-part* verb %vp-part[+verb]%
	Dop1 := verb.Dop1,
	Dop2 := verb.Dop2,
	Przyim1 := verb.Przyim1,
	Przyim2 := verb.Przyim2,
	Typ := verb.Typ,
	setscore -1.94591014905531

vp-part--verb = vp-part* verb %vp-part[+verb]%
	Dop1 := verb.Dop1,
	Przyim1 := verb.Przyim1,
	Typ := verb.Typ,
	setscore -1.94591014905531

vp-part--verb = vp-part* verb %vp-part[+verb]%
	Dop3 := verb.Dop3,
	Typ := verb.Typ,
	setscore -1.94591014905531

vp-part--verb = vp-part* verb %vp-part[+verb]%
	Typ := verb.Typ,
	setscore -1.94591014905531

vp-trunc = trunc* %VP[trunc:konj]%
	Block := 1,
	setscore 0

vp-trunc--comma = vp-trunc* comma %vp-trunc[+comma]%
	setscore 0

vp-trunc--conj = vp-trunc* conj %vp-trunc[+conj]%
	setscore -0.693147180559945

vp-trunc--conj = vp-trunc--trunc* conj %vp-trunc--trunc[+conj]%
	setscore -0.693147180559945

vp-trunc--trunc = vp-trunc--comma* trunc %vp-trunc--comma[+trunc:konj]%
	setscore 0

vp-trunc--verb = vp-trunc--conj* verb %vp-trunc--conj[+verb:konj]%
	setscore 0

vp-verb = verb* %VP[verb]%
	Block := 1,
	setscore 0

vp-verb--part = part vp-verb* %vp-verb[part+]%
	setscore 0

vp-vp = vp* %VP[vp:konj]%
	Block := 1,
	setscore -0.22314355131421

vp-vp = vp* %VP[vp]%
	Block := 1,
	setscore -1.6094379124341

vp-vp--comma = vp-vp* comma %vp-vp[+comma]%
	setscore 0

vp-vp--conj = vp-vp* conj %vp-vp[+conj]%
	setscore -0.693147180559945

vp-vp--vp = vp-vp* vp %vp-vp[+vp:konj]%
	setscore -1.6094379124341

vp-vp--vp = vp-vp* vp %vp-vp[+vp]%
	setscore -1.6094379124341

vp-vp--vp = vp-vp--comma* vp %vp-vp--comma[+vp:konj]%
	setscore -1.6094379124341

vp-vp--vp = vp-vp--conj* vp %vp-vp--conj[+vp:konj]%
	setscore -0.916290731874155
