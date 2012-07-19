# sredni zestaw
# rules_5.g from old Gobio parser

advx = advx-adverb*
    { Block := false },
    setscore -2.15948424935337

advx = advx-adverb--advx*
    { Block := false },
    setscore -1.87180217690159

advx = advx-adverb--ap*
    { Block := false },
    setscore -3.25809653802148

advx = advx-adverb--np*
    { Block := false },
    setscore -2.15948424935337

advx = advx-adverb--part*
    { Block := false },
    setscore -3.25809653802148

advx = advx-adverb--pp*
    { Block := false },
    setscore -3.25809653802148

advx = advx-advx--advx*
    { Block := false },
    setscore -2.56494935746154

advx = advx-advx--ap*
    { Block := false },
    setscore -3.25809653802148

advx = advx-advx--np*
    { Block := false },
    setscore -2.56494935746154

advx = advx-advx--pp*
    { Block := false },
    setscore -2.15948424935337

advx = advx-advx--r-simpx*
    { Block := false },
    setscore -3.25809653802148

advx = advx-advx--simpx*
    { Block := false },
    setscore -2.56494935746154

advx = advx-part*
    { Block := false },
    setscore -3.25809653802148

advx = advx-part--advx*
    { Block := false },
    setscore -3.25809653802148

advx-adverb = adverb* %ADVX[adverb]%
    Block := 1,
    T == deadj,
    setscore -0.619039208406224

advx-adverb = adverb* %ADVX[adverb]%
    Block := 1,
    T == other,
    setscore -0.955511445027436

advx-adverb = adverb* %ADVX[adverb]%
    Block := 1,
    T == rel,
    setscore -2.56494935746154

advx-adverb--advx = advx advx-adverb* %advx-adverb[advx+]%
    setscore -0.405465108108164

advx-adverb--advx = advx advx-adverb--advx* %advx-adverb--advx[advx+]%
    setscore -1.09861228866811

advx-adverb--ap = ap advx-adverb* %advx-adverb[ap+]%
    setscore 0

advx-adverb--np = np advx-adverb* %advx-adverb[np+]%
    setscore -0.405465108108164

advx-adverb--np = np advx-adverb--pp* %advx-adverb--pp[np+]%
    setscore -1.09861228866811

advx-adverb--part = part advx-adverb* %advx-adverb[part+]%
    setscore 0

advx-adverb--pp = pp advx-adverb* %advx-adverb[pp:compl1+]%
    setscore 0

advx-advx = advx* %ADVX[advx:konj]%
    Block := 1,
    setscore -1.01160091167848

advx-advx = advx* %ADVX[advx]%
    Block := 1,
    setscore -0.451985123743057

advx-advx--advx = advx-advx* advx %advx-advx[+advx]%
    setscore -1.09861228866811

advx-advx--advx = advx-advx--comma* advx %advx-advx--comma[+advx:konj]%
    setscore -1.09861228866811

advx-advx--advx = advx-advx--conj* advx %advx-advx--conj[+advx:konj]%
    setscore -1.09861228866811

advx-advx--ap = advx-advx--conj* ap %advx-advx--conj[+ap:konj]%
    setscore 0

advx-advx--comma = advx-advx* comma %advx-advx[+comma]%
    setscore 0

advx-advx--conj = advx-advx* conj %advx-advx[+conj]%
    setscore -0.287682072451781

advx-advx--conj = advx-advx--advx* conj %advx-advx--advx[+conj]%
    setscore -1.38629436111989

advx-advx--np = advx-advx* np %advx-advx[+np:possessive]%
    setscore -0.693147180559945

advx-advx--np = advx-advx* np %advx-advx[+np]%
    setscore -0.693147180559945

advx-advx--pp = advx-advx* pp %advx-advx[+pp:compl1]%
    setscore -1.09861228866811

advx-advx--pp = advx-advx--conj* pp %advx-advx--conj[+pp:konj]%
    setscore -0.405465108108164

advx-advx--r-simpx = advx-advx--comma* r-simpx %advx-advx--comma[+r-simpx]%
    setscore 0

advx-advx--simpx = advx-advx* simpx %advx-advx[+simpx]%
    setscore -0.693147180559945

advx-advx--simpx = advx-advx--comma* simpx %advx-advx--comma[+simpx]%
    setscore -0.693147180559945

advx-part = part* %ADVX[part]%
    Block := 1,
    T == neg,
    setscore 0

advx-part--advx = advx advx-part* %advx-part[advx+]%
    setscore 0

ap = ap-adjective*
    { Block := false },
    setscore -3.04452243772342

ap = ap-adjective--advx*
    { Block := false },
    setscore -1.94591014905531

ap = ap-adjective--np*
    { Block := false },
    setscore -3.04452243772342

ap = ap-adjective--pp*
    { Block := false },
    setscore -2.35137525716348

ap = ap-ap--advx*
    { Block := false },
    setscore -3.04452243772342

ap = ap-ap--ap*
    { Block := false },
    setscore -1.25276296849537

ap = ap-ap--pp*
    { Block := false },
    setscore -1.94591014905531

ap = ap-ap--simpx*
    { Block := false },
    setscore -3.04452243772342

ap = ap-conj--ap*
    { Block := false },
    setscore -3.04452243772342

ap = ap-num*
    { Block := false },
    setscore -3.04452243772342

ap = ap-pron*
    { Block := false },
    setscore -3.04452243772342

ap-adjective = adjective* %AP[adjective]%
    Block := 1,
    setscore 0

ap-adjective--advx = advx ap-adjective* %ap-adjective[advx+]%
    setscore -0.510825623765991

ap-adjective--advx = advx ap-adjective--advx* %ap-adjective--advx[advx+]%
    setscore -1.6094379124341

ap-adjective--advx = advx ap-adjective--pp* %ap-adjective--pp[advx+]%
    setscore -1.6094379124341

ap-adjective--np = np ap-adjective* %ap-adjective[np+]%
    setscore 0

ap-adjective--pp = pp ap-adjective* %ap-adjective[pp:compl1+]%
    setscore -0.405465108108164

ap-adjective--pp = pp ap-adjective--advx* %ap-adjective--advx[pp:compl1+]%
    setscore -1.09861228866811

ap-ap = ap* %AP[ap:konj]%
    Block := 1,
    setscore -0.451985123743057

ap-ap = ap* %AP[ap]%
    Block := 1,
    setscore -1.01160091167848

ap-ap--advx = advx ap-ap* %ap-ap[advx+]%
    setscore 0

ap-ap--ap = ap-ap* ap %ap-ap[+ap:konj]%
    O == ap.O,
    setscore -1.79175946922806

ap-ap--ap = ap-ap--comma* ap %ap-ap--comma[+ap:konj]%
    O == ap.O,
    setscore -1.09861228866811

ap-ap--ap = ap-ap--conj* ap %ap-ap--conj[+ap:konj]%
    O == ap.O,
    setscore -0.693147180559945

ap-ap--comma = ap-ap* comma %ap-ap[+comma]%
    setscore 0

ap-ap--conj = ap-ap* conj %ap-ap[+conj]%
    setscore -0.287682072451781

ap-ap--conj = ap-ap--comma* conj %ap-ap--comma[+conj]%
    setscore -1.38629436111989

ap-ap--pp = ap-ap* pp %ap-ap[+pp:compl1]%
    setscore -1.09861228866811

ap-ap--pp = ap-ap--advx* pp %ap-ap--advx[+pp:compl1]%
    setscore -1.09861228866811

ap-ap--pp = ap-ap--conj* pp %ap-ap--conj[+pp:konj]%
    setscore -1.09861228866811

ap-ap--simpx = ap-ap* simpx %ap-ap[+simpx]%
    setscore 0

ap-conj = conj* %AP[conj]%
    Block := 1,
    setscore 0

ap-conj--ap = ap-conj* ap %ap-conj[+ap:konj]%
    O := ap.O,
    setscore -0.693147180559945

ap-conj--ap = ap-conj--conj* ap %ap-conj--conj[+ap:konj]%
    O == ap.O,
    setscore -0.693147180559945

ap-conj--conj = ap-conj--ap* conj %ap-conj--ap[+conj]%
    setscore 0

ap-num = num* %AP[num]%
    Block := 1,
    setscore 0

ap-pron = pron* %AP[pron]%
    Block := 1,
    setscore 0

c = c-advx*
    setscore -1.17865499634165

c = c-ap*
    setscore -1.87180217690159

c = c-conj*
    setscore -2.56494935746154

c = c-np*
    setscore -1.46633706879343

c = c-pp*
    setscore -1.46633706879343

c-advx = advx* %C[advx:mod]%
    Block := 1,
    setscore -1.38629436111989

c-advx = advx* %C[advx:oadvp]%
    Block := 1,
    setscore -1.38629436111989

c-advx = advx* %C[advx:pred]%
    Block := 1,
    PRED := true,
    setscore -1.38629436111989

c-advx = advx* %C[advx:v-mod]%
    Block := 1,
    setscore -1.38629436111989

c-ap = ap* %C[ap:pred]%
    Block := 1,
    PRED := true,
    setscore -0.693147180559945

c-ap = ap* %C[ap:v-mod]%
    Block := 1,
    setscore -0.693147180559945

c-conj = conj* %C[conj]%
    Block := 1,
    CONJ := 1,
    setscore 0

c-np = np* %C[np:compl1]%
    Block := 1,
    C == acc,
    OA := true,
    OA_C := C,
    setscore -1.09861228866811

c-np = np* %C[np:oa]%
    Block := 1,
    C == acc,
    OA := true,
    OA_C := C,
    setscore -1.09861228866811

c-np = np* %C[np:subject]%
    Block := 1,
    C == nom,
    ON := true,
    ON_Num := Num,
    ON_P := P,
    setscore -1.09861228866811

c-pp = pp* %C[pp:compl1]%
    Block := 1,
    OPP := true,
    setscore -1.09861228866811

c-pp = pp* %C[pp:compl1]%
    Block := 1,
    OPP := true,
    OPP_C := C,
    OPP_Przyim := Przyim,
    setscore -1.09861228866811

c-pp = pp* %C[pp:v-mod]%
    Block := 1,
    setscore -1.09861228866811

dm = dm-advx*
    setscore -1.6094379124341

dm = dm-conj*
    setscore -1.6094379124341

dm = dm-exclamation*
    setscore -1.6094379124341

dm = dm-np*
    setscore -1.6094379124341

dm = dm-pp*
    setscore -1.6094379124341

dm-advx = advx* %DM[advx]%
    Block := 1,
    setscore 0

dm-conj = conj* %DM[conj]%
    Block := 1,
    setscore 0

dm-exclamation = exclamation* %DM[exclamation]%
    Block := 1,
    setscore 0

dm-np = np* %DM[np]%
    Block := 1,
    setscore 0

dm-pp = pp* %DM[pp:compl1]%
    Block := 1,
    setscore 0

dp = dp-pron--np*
    { Block := false },
    setscore -0.693147180559945

dp = dp-pron--part*
    { Block := false },
    setscore -0.693147180559945

dp-pron = pron* %DP[pron]%
    Block := 1,
    setscore 0

dp-pron--np = np dp-pron* %dp-pron[np+]%
    setscore 0

dp-pron--part = part dp-pron* %dp-pron[part+]%
    setscore 0

fkonj = fkonj-lk--mf*
    MF_OA = true,
    setscore -2.94443897916644

fkonj = fkonj-lk--mf*
    MF_OA = true,
    MF_OD = true,
    setscore -3.63758615972639

fkonj = fkonj-lk--mf*
    MF_ON = true,
    setscore -2.53897387105828

fkonj = fkonj-lk--mf*
    setscore -2.53897387105828

fkonj = fkonj-lk--nf*
    MF_OA = true,
    setscore -3.63758615972639

fkonj = fkonj-lk--nf*
    setscore -2.02814824729229

fkonj = fkonj-lk--vc*
    MF_OA = true,
    setscore -2.94443897916644

fkonj = fkonj-lk--vc*
    MF_OA = true,
    MF_OD = true,
    setscore -3.63758615972639

fkonj = fkonj-lk--vc*
    setscore -2.53897387105828

fkonj = fkonj-mf--nf*
    MF_OA = true,
    setscore -3.63758615972639

fkonj = fkonj-mf--nf*
    MF_OA = true,
    MF_OD = true,
    setscore -3.63758615972639

fkonj = fkonj-mf--vc*
    MF_OA = true,
    setscore -2.53897387105828

fkonj = fkonj-mf--vc*
    MF_OA = true,
    MF_OD = true,
    setscore -3.63758615972639

fkonj = fkonj-mf--vc*
    MF_OA = true,
    MF_ON = true,
    setscore -2.53897387105828

fkonj = fkonj-mf--vc*
    MF_ON = true,
    setscore -2.53897387105828

fkonj = fkonj-mf--vc*
    setscore -2.53897387105828

fkonj = fkonj-vc--nf*
    setscore -2.94443897916644

fkonj-lk = lk* %FKONJ[lk]%
    Block := 1,
    setscore 0

fkonj-lk--comma = fkonj-lk* comma %fkonj-lk[+comma]%
    setscore -1.6094379124341

fkonj-lk--comma = fkonj-lk--mf* comma %fkonj-lk--mf[+comma]%
    setscore -0.22314355131421

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
    MF_OA := mf.OA,
    setscore -2.2512917986065

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
    MF_OA := mf.OA,
    MF_OD := mf.OD,
    setscore -2.94443897916644

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
    MF_OA := mf.OA,
    MF_OD := mf.OD,
    mf.OPP = false,
    setscore -2.94443897916644

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
    MF_OA := mf.OA,
    mf.OPP = false,
    setscore -2.94443897916644

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
    MF_OA := mf.OA,
    mf.OPP = false,
    mf.PRED = true,
    setscore -2.94443897916644

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
    MF_OA := mf.OA,
    mf.OPP = true,
    setscore -2.94443897916644

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
    MF_ON := mf.ON,
    setscore -2.94443897916644

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
    MF_ON := mf.ON,
    mf.OPP = true,
    setscore -2.94443897916644

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
    MF_ON := mf.ON,
    mf.PRED = true,
    setscore -2.94443897916644

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
    mf.OPP = false,
    setscore -2.94443897916644

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
    mf.OPP = false,
    mf.PRED = true,
    setscore -2.94443897916644

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
    mf.OPP = true,
    setscore -1.84582669049833

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
    mf.OPP = true,
    mf.PRED = true,
    setscore -2.94443897916644

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
    mf.PRED = true,
    setscore -2.94443897916644

fkonj-lk--mf = fkonj-lk* mf %fkonj-lk[+mf]%
    setscore -2.2512917986065

fkonj-lk--nf = fkonj-lk--comma* nf %fkonj-lk--comma[+nf]%
    nf.OPP = false,
    setscore -1.09861228866811

fkonj-lk--nf = fkonj-lk--comma* nf %fkonj-lk--comma[+nf]%
    setscore -0.693147180559945

fkonj-lk--nf = fkonj-lk--vc* nf %fkonj-lk--vc[+nf]%
    nf.OPP = false,
    setscore -1.79175946922806

fkonj-lk--vc = fkonj-lk* vc %fkonj-lk[+vc]%
    setscore -1.94591014905531

fkonj-lk--vc = fkonj-lk--mf* vc %fkonj-lk--mf[+vc]%
    setscore -0.154150679827258

fkonj-mf = mf* %FKONJ[mf]%
    Block := 1,
    MF_OA := OA,
    MF_OD := OD,
    OPP = false,
    setscore -2.01490302054226

fkonj-mf = mf* %FKONJ[mf]%
    Block := 1,
    MF_OA := OA,
    MF_ON := ON,
    OPP = false,
    setscore -2.70805020110221

fkonj-mf = mf* %FKONJ[mf]%
    Block := 1,
    MF_OA := OA,
    MF_ON := ON,
    OPP = false,
    PRED = true,
    setscore -2.70805020110221

fkonj-mf = mf* %FKONJ[mf]%
    Block := 1,
    MF_OA := OA,
    MF_ON := ON,
    OPP = true,
    setscore -2.70805020110221

fkonj-mf = mf* %FKONJ[mf]%
    Block := 1,
    MF_OA := OA,
    OPP = false,
    setscore -2.01490302054226

fkonj-mf = mf* %FKONJ[mf]%
    Block := 1,
    MF_OA := OA,
    OPP = false,
    PRED = true,
    setscore -2.70805020110221

fkonj-mf = mf* %FKONJ[mf]%
    Block := 1,
    MF_OA := OA,
    OPP = true,
    setscore -2.70805020110221

fkonj-mf = mf* %FKONJ[mf]%
    Block := 1,
    MF_ON := ON,
    OPP = false,
    setscore -2.70805020110221

fkonj-mf = mf* %FKONJ[mf]%
    Block := 1,
    MF_ON := ON,
    OPP = false,
    PRED = true,
    setscore -2.70805020110221

fkonj-mf = mf* %FKONJ[mf]%
    Block := 1,
    MF_ON := ON,
    OPP = true,
    setscore -2.70805020110221

fkonj-mf = mf* %FKONJ[mf]%
    Block := 1,
    OPP = false,
    setscore -2.70805020110221

fkonj-mf = mf* %FKONJ[mf]%
    Block := 1,
    OPP = true,
    setscore -2.70805020110221

fkonj-mf = mf* %FKONJ[mf]%
    Block := 1,
    OPP = true,
    PRED = true,
    setscore -2.70805020110221

fkonj-mf--comma = fkonj-mf--vc* comma %fkonj-mf--vc[+comma]%
    setscore 0

fkonj-mf--nf = fkonj-mf--comma* nf %fkonj-mf--comma[+nf]%
    nf.OPP = false,
    setscore 0

fkonj-mf--vc = fkonj-mf* vc %fkonj-mf[+vc]%
    setscore 0

fkonj-vc = vc* %FKONJ[vc]%
    Block := 1,
    setscore 0

fkonj-vc--comma = fkonj-vc* comma %fkonj-vc[+comma]%
    setscore 0

fkonj-vc--nf = fkonj-vc* nf %fkonj-vc[+nf]%
    setscore -0.693147180559945

fkonj-vc--nf = fkonj-vc--comma* nf %fkonj-vc--comma[+nf]%
    setscore -0.693147180559945

fkoord = fkoord-fkonj--fkonj*
    setscore -0.510825623765991

fkoord = fkoord-mf--mf*
    setscore -0.916290731874155

fkoord-fkonj = fkonj* %FKOORD[fkonj:konj]%
    Block := 1,
    setscore 0

fkoord-fkonj--comma = fkoord-fkonj* comma %fkoord-fkonj[+comma]%
    setscore 0

fkoord-fkonj--conj = fkoord-fkonj* conj %fkoord-fkonj[+conj]%
    setscore -0.693147180559945

fkoord-fkonj--conj = fkoord-fkonj--comma* conj %fkoord-fkonj--comma[+conj]%
    setscore -0.693147180559945

fkoord-fkonj--fkonj = fkoord-fkonj--comma* fkonj %fkoord-fkonj--comma[+fkonj:konj]%
    setscore -1.09861228866811

fkoord-fkonj--fkonj = fkoord-fkonj--conj* fkonj %fkoord-fkonj--conj[+fkonj:konj]%
    setscore -0.405465108108164

fkoord-mf = mf* %FKOORD[mf:konj]%
    Block := 1,
    setscore 0

fkoord-mf--comma = fkoord-mf* comma %fkoord-mf[+comma]%
    setscore 0

fkoord-mf--conj = fkoord-mf* conj %fkoord-mf[+conj]%
    setscore -0.693147180559945

fkoord-mf--conj = fkoord-mf--comma* conj %fkoord-mf--comma[+conj]%
    setscore -0.693147180559945

fkoord-mf--mf = fkoord-mf--conj* mf %fkoord-mf--conj[+mf:konj]%
    setscore 0

fx = fx-fx--pp*
    { Block := false },
    setscore -1.6094379124341

fx = fx-np--unknown*
    { Block := false },
    setscore -1.6094379124341

fx = fx-unknown*
    { Block := false },
    setscore -1.6094379124341

fx = fx-unknown--np*
    { Block := false },
    setscore -1.6094379124341

fx = fx-unknown--unknown*
    { Block := false },
    setscore -1.6094379124341

fx-fx = fx* %FX[fx]%
    Block := 1,
    setscore 0

fx-fx--pp = fx-fx* pp %fx-fx[+pp:compl1]%
    setscore 0

fx-np = np* %FX[np]%
    Block := 1,
    setscore 0

fx-np--unknown = fx-np* unknown %fx-np[+unknown]%
    setscore 0

fx-unknown = unknown* %FX[unknown]%
    Block := 1,
    setscore 0

fx-unknown--np = fx-unknown--unknown* np %fx-unknown--unknown[+np]%
    setscore 0

fx-unknown--unknown = fx-unknown* unknown %fx-unknown[+unknown]%
    setscore 0

koord = koord-conj*
    { Block := false },
    setscore 0

koord-conj = conj* %KOORD[conj]%
    Block := 1,
    setscore 0

lk = lk-vp*
    setscore 0

lk-vp = vp* %LK[vp]%
    Block := 1,
    setscore 0

lv = lv-np*
    { Block := false },
    setscore -0.693147180559945

lv = lv-simpx*
    { Block := false },
    setscore -0.693147180559945

lv-np = np* %LV[np:oa-mod]%
    Block := 1,
    setscore 0

lv-simpx = simpx* %LV[simpx:mod-mod]%
    Block := 1,
    setscore 0

mf = mf-advx*
    setscore -4.46206934234742

mf = mf-advx--advx*
    setscore -4.86753445045558

mf = mf-advx--ap*
    setscore -3.48124008933569

mf = mf-advx--np*
    setscore -3.16278635821716

mf = mf-advx--pp*
    setscore -2.5161591932921

mf = mf-ap*
    setscore -4.86753445045558

mf = mf-ap--advx*
    setscore -5.56068163101553

mf = mf-ap--np*
    setscore -5.56068163101553

mf = mf-ap--pp*
    setscore -3.95124371858143

mf = mf-np*
    setscore -3.48124008933569

mf = mf-np--advx*
    setscore -2.42518741508638

mf = mf-np--ap*
    setscore -2.30258509299405

mf = mf-np--np*
    setscore -2.06417406954905

mf = mf-np--pp*
    setscore -1.34117392583942

mf = mf-np--simpx*
    setscore -4.46206934234742

mf = mf-pp*
    setscore -4.17438726989564

mf = mf-pp--advx*
    setscore -5.56068163101553

mf = mf-pp--ap*
    setscore -3.61477148196021

mf = mf-pp--np*
    setscore -2.38262780066758

mf = mf-pp--pp*
    setscore -3.36345705367931

mf-advx = advx* %MF[advx:mod]%
    Block := 1,
    setscore -0.973449145714104

mf-advx = advx* %MF[advx:mod]%
    Block := 1,
    OPP := true,
    setscore -1.72722094809048

mf-advx = advx* %MF[advx:mod]%
    Block := 1,
    OPP := true,
    PRED := true,
    setscore -3.80666248977032

mf-advx = advx* %MF[advx:mod]%
    Block := 1,
    PRED := true,
    setscore -2.01490302054226

mf-advx = advx* %MF[advx:pred]%
    Block := 1,
    PRED := true,
    setscore -3.80666248977032

mf-advx = advx* %MF[advx:v-mod]%
    Block := 1,
    setscore -2.01490302054226

mf-advx = advx* %MF[advx:v-mod]%
    Block := 1,
    OA := true,
    OPP := true,
    setscore -3.80666248977032

mf-advx = advx* %MF[advx:v-mod]%
    Block := 1,
    OPP := true,
    setscore -2.42036812865043

mf-advx = advx* %MF[advx:v-mod]%
    Block := 1,
    PRED := true,
    setscore -3.80666248977032

mf-advx--advx = mf-advx* advx %mf-advx[+advx:mod]%
    setscore -0.22314355131421

mf-advx--advx = mf-advx--np* advx %mf-advx--np[+advx:mod]%
    setscore -2.70805020110221

mf-advx--advx = mf-advx--pp* advx %mf-advx--pp[+advx:mod]%
    setscore -2.70805020110221

mf-advx--advx = mf-advx--pp* advx %mf-advx--pp[+advx:v-mod]%
    setscore -2.70805020110221

mf-advx--ap = mf-advx* ap %mf-advx[+ap:pred]%
    setscore -2.39789527279837

mf-advx--ap = mf-advx* ap %mf-advx[+ap:v-mod]%
    setscore -1.01160091167848

mf-advx--ap = mf-advx--advx* ap %mf-advx--advx[+ap:pred]%
    setscore -2.39789527279837

mf-advx--ap = mf-advx--ap* ap %mf-advx--ap[+ap:pred]%
    setscore -2.39789527279837

mf-advx--ap = mf-advx--np* ap %mf-advx--np[+ap:v-mod]%
    setscore -1.70474809223843

mf-advx--ap = mf-advx--pp* ap %mf-advx--pp[+ap:pred]%
    setscore -2.39789527279837

mf-advx--ap = mf-advx--pp* ap %mf-advx--pp[+ap:v-mod]%
    setscore -2.39789527279837

mf-advx--np = mf-advx* np %mf-advx[+np:compl1]%
    OA := true,
    OA_C := np.C,
    np.C == acc,
    setscore -1.7227665977411

mf-advx--np = mf-advx* np %mf-advx[+np:compl1]%
    OD := true,
    OD_C := np.C,
    np.C == dat,
    setscore -3.3322045101752

mf-advx--np = mf-advx* np %mf-advx[+np:compl2]%
    OA_C := np.C,
    setscore -3.3322045101752

mf-advx--np = mf-advx* np %mf-advx[+np:on-mod]%
    setscore -3.3322045101752

mf-advx--np = mf-advx* np %mf-advx[+np:pred]%
    setscore -3.3322045101752

mf-advx--np = mf-advx* np %mf-advx[+np:subject]%
    ON := true,
    ON_Num := np.Num,
    ON_P := np.P,
    np.C == nom,
    setscore -1.54044504094715

mf-advx--np = mf-advx* np %mf-advx[+np:v-mod]%
    setscore -3.3322045101752

mf-advx--np = mf-advx--advx* np %mf-advx--advx[+np:compl1]%
    OA := true,
    OA_C := np.C,
    np.C == acc,
    setscore -2.63905732961526

mf-advx--np = mf-advx--advx* np %mf-advx--advx[+np:pred]%
    setscore -3.3322045101752

mf-advx--np = mf-advx--advx* np %mf-advx--advx[+np:subject]%
    ON := true,
    ON_Num := np.Num,
    ON_P := np.P,
    np.C == nom,
    setscore -2.23359222150709

mf-advx--np = mf-advx--ap* np %mf-advx--ap[+np:compl1]%
    OA := true,
    OA_C := np.C,
    np.C == acc,
    setscore -3.3322045101752

mf-advx--np = mf-advx--np* np %mf-advx--np[+np:compl1]%
    OA := true,
    OA_C := np.C,
    np.C == acc,
    setscore -2.63905732961526

mf-advx--np = mf-advx--pp* np %mf-advx--pp[+np:pred]%
    setscore -2.63905732961526

mf-advx--np = mf-advx--pp* np %mf-advx--pp[+np:subject]%
    ON := true,
    ON_Num := np.Num,
    ON_P := np.P,
    np.C == nom,
    setscore -3.3322045101752

mf-advx--pp = mf-advx* pp %mf-advx[+pp:compl1]%
    OPP_C := pp.C,
    OPP_Przyim := pp.Przyim,
    setscore -2.01490302054226

mf-advx--pp = mf-advx* pp %mf-advx[+pp:mod]%
    setscore -3.40119738166216

mf-advx--pp = mf-advx* pp %mf-advx[+pp:v-mod]%
    setscore -2.01490302054226

mf-advx--pp = mf-advx--advx* pp %mf-advx--advx[+pp:compl1]%
    OPP_C := pp.C,
    OPP_Przyim := pp.Przyim,
    setscore -2.70805020110221

mf-advx--pp = mf-advx--advx* pp %mf-advx--advx[+pp:mod]%
    setscore -3.40119738166216

mf-advx--pp = mf-advx--advx* pp %mf-advx--advx[+pp:v-mod]%
    setscore -2.30258509299405

mf-advx--pp = mf-advx--ap* pp %mf-advx--ap[+pp:compl1]%
    OPP_C := pp.C,
    OPP_Przyim := pp.Przyim,
    setscore -3.40119738166216

mf-advx--pp = mf-advx--np* pp %mf-advx--np[+pp:compl1]%
    OPP_C := pp.C,
    OPP_Przyim := pp.Przyim,
    setscore -2.30258509299405

mf-advx--pp = mf-advx--np* pp %mf-advx--np[+pp:compl2]%
    OPP_C := pp.C,
    OPP_Przyim := pp.Przyim,
    setscore -3.40119738166216

mf-advx--pp = mf-advx--np* pp %mf-advx--np[+pp:compl2]%
    OPP_Przyim := pp.Przyim,
    setscore -3.40119738166216

mf-advx--pp = mf-advx--np* pp %mf-advx--np[+pp:compl2]%
    setscore -3.40119738166216

mf-advx--pp = mf-advx--np* pp %mf-advx--np[+pp:mod]%
    setscore -3.40119738166216

mf-advx--pp = mf-advx--np* pp %mf-advx--np[+pp:v-mod]%
    setscore -1.79175946922806

mf-advx--pp = mf-advx--pp* pp %mf-advx--pp[+pp:compl1]%
    OPP_C := pp.C,
    OPP_Przyim := pp.Przyim,
    setscore -3.40119738166216

mf-advx--pp = mf-advx--pp* pp %mf-advx--pp[+pp:v-mod]%
    setscore -3.40119738166216

mf-ap = ap* %MF[ap:pred]%
    Block := 1,
    OPP := true,
    PRED := true,
    setscore -2.19722457733622

mf-ap = ap* %MF[ap:pred]%
    Block := 1,
    PRED := true,
    setscore -2.19722457733622

mf-ap = ap* %MF[ap:v-mod]%
    Block := 1,
    setscore -0.587786664902119

mf-ap = ap* %MF[ap:v-mod]%
    Block := 1,
    OPP := true,
    setscore -1.50407739677627

mf-ap--advx = mf-ap* advx %mf-ap[+advx:mod]%
    setscore -0.693147180559945

mf-ap--advx = mf-ap--np* advx %mf-ap--np[+advx:mod]%
    setscore -1.38629436111989

mf-ap--advx = mf-ap--np* advx %mf-ap--np[+advx:v-mod]%
    setscore -1.38629436111989

mf-ap--np = mf-ap* np %mf-ap[+np:subject]%
    ON := true,
    ON_Num := np.Num,
    ON_P := np.P,
    np.C == nom,
    setscore -0.405465108108164

mf-ap--np = mf-ap--advx* np %mf-ap--advx[+np:compl1]%
    OA := true,
    OA_C := np.C,
    np.C == acc,
    setscore -1.09861228866811

mf-ap--pp = mf-ap* pp %mf-ap[+pp:compl1]%
    OPP_C := pp.C,
    OPP_Przyim := pp.Przyim,
    setscore -1.6094379124341

mf-ap--pp = mf-ap* pp %mf-ap[+pp:compl1]%
    setscore -1.6094379124341

mf-ap--pp = mf-ap* pp %mf-ap[+pp:v-mod]%
    setscore -1.6094379124341

mf-ap--pp = mf-ap--advx* pp %mf-ap--advx[+pp:compl1]%
    OPP_C := pp.C,
    OPP_Przyim := pp.Przyim,
    setscore -1.6094379124341

mf-ap--pp = mf-ap--advx* pp %mf-ap--advx[+pp:v-mod]%
    setscore -1.6094379124341

mf-np = np* %MF[np:compl1]%
    Block := 1,
    C == acc,
    OA := true,
    OA_C := C,
    setscore -1.90335053463652

mf-np = np* %MF[np:compl1]%
    Block := 1,
    C == acc,
    OA := true,
    OA_C := C,
    OPP := true,
    setscore -2.77881927199042

mf-np = np* %MF[np:compl1]%
    Block := 1,
    C == acc,
    OA := true,
    OA_C := C,
    OPP := true,
    PRED := true,
    setscore -3.98279207631635

mf-np = np* %MF[np:compl1]%
    Block := 1,
    C == acc,
    OA := true,
    OA_C := C,
    PRED := true,
    setscore -3.98279207631635

mf-np = np* %MF[np:compl1]%
    Block := 1,
    C == dat,
    OD := true,
    OD_C := C,
    setscore -3.13549421592915

mf-np = np* %MF[np:compl1]%
    Block := 1,
    C == dat,
    OD := true,
    OD_C := C,
    ON := true,
    OPP := true,
    setscore -5.08140436498446

mf-np = np* %MF[np:compl1]%
    Block := 1,
    C == dat,
    OD := true,
    OD_C := C,
    OPP := true,
    setscore -5.08140436498446

mf-np = np* %MF[np:compl1]%
    Block := 1,
    C == dat,
    OD := true,
    OD_C := C,
    PRED := true,
    setscore -3.98279207631635

mf-np = np* %MF[np:compl1]%
    Block := 1,
    OA := true,
    OA_C := C,
    setscore -3.69511000386457

mf-np = np* %MF[np:compl2]%
    Block := 1,
    C == dat,
    OD := true,
    OD_C := C,
    setscore -3.98279207631635

mf-np = np* %MF[np:compl2]%
    Block := 1,
    C == dat,
    OD := true,
    OD_C := C,
    OPP := true,
    setscore -5.08140436498446

mf-np = np* %MF[np:oa]%
    Block := 1,
    OA := true,
    OA_C := C,
    setscore -4.38825718442452

mf-np = np* %MF[np:oa]%
    Block := 1,
    OA := true,
    OA_C := C,
    OPP := true,
    setscore -5.08140436498446

mf-np = np* %MF[np:on-mod]%
    Block := 1,
    setscore -4.38825718442452

mf-np = np* %MF[np:on-mod]%
    Block := 1,
    PRED := true,
    setscore -4.38825718442452

mf-np = np* %MF[np:os-mod]%
    Block := 1,
    OPP := true,
    setscore -5.08140436498446

mf-np = np* %MF[np:os-mod]%
    Block := 1,
    PRED := true,
    setscore -5.08140436498446

mf-np = np* %MF[np:pred]%
    Block := 1,
    ON := true,
    PRED := true,
    setscore -5.08140436498446

mf-np = np* %MF[np:pred]%
    Block := 1,
    PRED := true,
    setscore -4.38825718442452

mf-np = np* %MF[np:subject]%
    Block := 1,
    C == nom,
    OA := true,
    ON := true,
    ON_Num := Num,
    ON_P := P,
    setscore -3.98279207631635

mf-np = np* %MF[np:subject]%
    Block := 1,
    C == nom,
    ON := true,
    ON_Num := Num,
    ON_P := P,
    setscore -1.3202042492909

mf-np = np* %MF[np:subject]%
    Block := 1,
    C == nom,
    ON := true,
    ON_Num := Num,
    ON_P := P,
    OPP := true,
    setscore -1.99036191162615

mf-np = np* %MF[np:subject]%
    Block := 1,
    C == nom,
    ON := true,
    ON_Num := Num,
    ON_P := P,
    OPP := true,
    PRED := true,
    setscore -5.08140436498446

mf-np = np* %MF[np:subject]%
    Block := 1,
    C == nom,
    ON := true,
    ON_Num := Num,
    ON_P := P,
    PRED := true,
    setscore -2.1910326070883

mf-np = np* %MF[np:subject]%
    Block := 1,
    ON := true,
    ON_Num := Num,
    ON_P := P,
    setscore -5.08140436498446

mf-np = np* %MF[np:subject]%
    Block := 1,
    ON := true,
    ON_Num := Num,
    ON_P := P,
    PRED := true,
    setscore -5.08140436498446

mf-np--advx = mf-np* advx %mf-np[+advx:mod]%
    setscore -0.93826963859293

mf-np--advx = mf-np* advx %mf-np[+advx:oadvp]%
    setscore -4.52178857704904

mf-np--advx = mf-np* advx %mf-np[+advx:pred]%
    setscore -4.52178857704904

mf-np--advx = mf-np* advx %mf-np[+advx:v-mod]%
    setscore -2.4423470353692

mf-np--advx = mf-np--advx* advx %mf-np--advx[+advx:mod]%
    setscore -1.88273124743378

mf-np--advx = mf-np--advx* advx %mf-np--advx[+advx:oadjp]%
    setscore -4.52178857704904

mf-np--advx = mf-np--comma* advx %mf-np--comma[+advx:mod]%
    setscore -3.8286413964891

mf-np--advx = mf-np--np* advx %mf-np--np[+advx:mod]%
    setscore -2.21920348405499

mf-np--advx = mf-np--np* advx %mf-np--np[+advx:oadjp]%
    setscore -4.52178857704904

mf-np--advx = mf-np--np* advx %mf-np--np[+advx:v-mod]%
    setscore -4.52178857704904

mf-np--advx = mf-np--pp* advx %mf-np--pp[+advx:mod]%
    setscore -1.81373837594683

mf-np--advx = mf-np--pp* advx %mf-np--pp[+advx:oadvp]%
    setscore -4.52178857704904

mf-np--advx = mf-np--pp* advx %mf-np--pp[+advx:v-mod]%
    setscore -4.52178857704904

mf-np--ap = mf-np* ap %mf-np[+ap:pred]%
    setscore -2.16905370036952

mf-np--ap = mf-np* ap %mf-np[+ap:v-mod]%
    setscore -1.76358859226136

mf-np--ap = mf-np--advx* ap %mf-np--advx[+ap:pred]%
    setscore -1.94591014905531

mf-np--ap = mf-np--advx* ap %mf-np--advx[+ap:v-mod]%
    setscore -1.94591014905531

mf-np--ap = mf-np--comma* ap %mf-np--comma[+ap:pred]%
    setscore -3.55534806148941

mf-np--ap = mf-np--np* ap %mf-np--np[+ap:pred]%
    setscore -3.55534806148941

mf-np--ap = mf-np--np* ap %mf-np--np[+ap:v-mod]%
    setscore -2.86220088092947

mf-np--ap = mf-np--pp* ap %mf-np--pp[+ap:pred]%
    setscore -1.76358859226136

mf-np--ap = mf-np--pp* ap %mf-np--pp[+ap:v-mod]%
    setscore -1.94591014905531

mf-np--comma = mf-np* comma %mf-np[+comma]%
    setscore -0.336472236621213

mf-np--comma = mf-np--np* comma %mf-np--np[+comma]%
    setscore -1.25276296849537

mf-np--np = mf-np* np %mf-np[+np:compl1]%
    OA := true,
    OA_C := np.C,
    np.C == acc,
    setscore -1.46151778235748

mf-np--np = mf-np* np %mf-np[+np:compl1]%
    OA_C := np.C,
    setscore -4.23410650459726

mf-np--np = mf-np* np %mf-np[+np:compl1]%
    OD := true,
    OD_C := np.C,
    np.C == dat,
    setscore -3.54095932403731

mf-np--np = mf-np* np %mf-np[+np:compl2]%
    OD := true,
    OD_C := np.C,
    np.C == dat,
    setscore -4.23410650459726

mf-np--np = mf-np* np %mf-np[+np:os-mod]%
    setscore -4.23410650459726

mf-np--np = mf-np* np %mf-np[+np:pred]%
    setscore -3.13549421592915

mf-np--np = mf-np* np %mf-np[+np:subject]%
    ON := true,
    ON_Num := np.Num,
    ON_P := np.P,
    np.C == nom,
    setscore -1.52605630349505

mf-np--np = mf-np* np %mf-np[+np:subject]%
    ON_Num := np.Num,
    ON_P := np.P,
    setscore -4.23410650459726

mf-np--np = mf-np* np %mf-np[+np:subject]%
    setscore -4.23410650459726

mf-np--np = mf-np--advx* np %mf-np--advx[+np:compl1]%
    OA := true,
    OA_C := np.C,
    np.C == acc,
    setscore -2.28819635554195

mf-np--np = mf-np--advx* np %mf-np--advx[+np:compl2]%
    OA_C := np.C,
    setscore -4.23410650459726

mf-np--np = mf-np--advx* np %mf-np--advx[+np:pred]%
    setscore -2.4423470353692

mf-np--np = mf-np--advx* np %mf-np--advx[+np:subject]%
    ON := true,
    ON_Num := np.Num,
    ON_P := np.P,
    np.C == nom,
    setscore -3.13549421592915

mf-np--np = mf-np--advx* np %mf-np--advx[+np:v-mod]%
    setscore -4.23410650459726

mf-np--np = mf-np--ap* np %mf-np--ap[+np:compl1]%
    OA := true,
    OA_C := np.C,
    np.C == acc,
    setscore -4.23410650459726

mf-np--np = mf-np--ap* np %mf-np--ap[+np:compl2]%
    OD := true,
    OD_C := np.C,
    np.C == dat,
    setscore -4.23410650459726

mf-np--np = mf-np--pp* np %mf-np--pp[+np:compl1]%
    OA := true,
    OA_C := np.C,
    np.C == acc,
    setscore -2.62466859216316

mf-np--np = mf-np--pp* np %mf-np--pp[+np:oa]%
    OA_C := np.C,
    setscore -4.23410650459726

mf-np--np = mf-np--pp* np %mf-np--pp[+np:pred]%
    setscore -4.23410650459726

mf-np--np = mf-np--pp* np %mf-np--pp[+np:subject]%
    ON := true,
    ON_Num := np.Num,
    ON_P := np.P,
    np.C == nom,
    setscore -4.23410650459726

mf-np--pp = mf-np* pp %mf-np[+pp:compl1]%
    OPP_C := pp.C,
    OPP_Przyim := pp.Przyim,
    setscore -4.11087386417331

mf-np--pp = mf-np* pp %mf-np[+pp:compl1]%
    OPP_Przyim := pp.Przyim,
    setscore -4.80402104473326

mf-np--pp = mf-np* pp %mf-np[+pp:compl2]%
    OPP_C := pp.C,
    OPP_Przyim := pp.Przyim,
    setscore -3.19458313229916

mf-np--pp = mf-np* pp %mf-np[+pp:compl2]%
    setscore -4.80402104473326

mf-np--pp = mf-np* pp %mf-np[+pp:mod]%
    setscore -2.31911439494526

mf-np--pp = mf-np* pp %mf-np[+pp:opp]%
    OPP_C := pp.C,
    OPP_Przyim := pp.Przyim,
    setscore -4.80402104473326

mf-np--pp = mf-np* pp %mf-np[+pp:pred]%
    setscore -4.11087386417331

mf-np--pp = mf-np* pp %mf-np[+pp:v-mod]%
    setscore -1.54592450671177

mf-np--pp = mf-np--advx* pp %mf-np--advx[+pp:compl1]%
    OPP_C := pp.C,
    OPP_Przyim := pp.Przyim,
    setscore -3.41772668361337

mf-np--pp = mf-np--advx* pp %mf-np--advx[+pp:compl2]%
    OPP_C := pp.C,
    OPP_Przyim := pp.Przyim,
    setscore -2.85811089567794

mf-np--pp = mf-np--advx* pp %mf-np--advx[+pp:oa-mod]%
    setscore -4.80402104473326

mf-np--pp = mf-np--advx* pp %mf-np--advx[+pp:pred]%
    setscore -4.80402104473326

mf-np--pp = mf-np--advx* pp %mf-np--advx[+pp:v-mod]%
    setscore -2.23907168727172

mf-np--pp = mf-np--ap* pp %mf-np--ap[+pp:compl1]%
    OPP_C := pp.C,
    OPP_Przyim := pp.Przyim,
    setscore -3.70540875606515

mf-np--pp = mf-np--ap* pp %mf-np--ap[+pp:compl2]%
    OPP_C := pp.C,
    OPP_Przyim := pp.Przyim,
    setscore -4.80402104473326

mf-np--pp = mf-np--ap* pp %mf-np--ap[+pp:v-mod]%
    setscore -3.70540875606515

mf-np--pp = mf-np--comma* pp %mf-np--comma[+pp:compl2]%
    OPP_C := pp.C,
    OPP_Przyim := pp.Przyim,
    setscore -4.80402104473326

mf-np--pp = mf-np--comma* pp %mf-np--comma[+pp:v-mod]%
    setscore -4.80402104473326

mf-np--pp = mf-np--np* pp %mf-np--np[+pp:compl1]%
    OPP_C := pp.C,
    OPP_Przyim := pp.Przyim,
    setscore -4.80402104473326

mf-np--pp = mf-np--np* pp %mf-np--np[+pp:compl2]%
    OPP_C := pp.C,
    OPP_Przyim := pp.Przyim,
    setscore -3.41772668361337

mf-np--pp = mf-np--np* pp %mf-np--np[+pp:compl2]%
    OPP_Przyim := pp.Przyim,
    setscore -4.80402104473326

mf-np--pp = mf-np--np* pp %mf-np--np[+pp:compl2]%
    setscore -4.11087386417331

mf-np--pp = mf-np--np* pp %mf-np--np[+pp:mod]%
    setscore -4.11087386417331

mf-np--pp = mf-np--np* pp %mf-np--np[+pp:v-mod]%
    setscore -2.60679646739704

mf-np--pp = mf-np--pp* pp %mf-np--pp[+pp:compl1]%
    OPP_C := pp.C,
    OPP_Przyim := pp.Przyim,
    setscore -4.11087386417331

mf-np--pp = mf-np--pp* pp %mf-np--pp[+pp:compl2]%
    OPP_C := pp.C,
    OPP_Przyim := pp.Przyim,
    setscore -3.70540875606515

mf-np--pp = mf-np--pp* pp %mf-np--pp[+pp:compl2]%
    setscore -4.80402104473326

mf-np--pp = mf-np--pp* pp %mf-np--pp[+pp:mod]%
    setscore -3.70540875606515

mf-np--pp = mf-np--pp* pp %mf-np--pp[+pp:opp]%
    OPP_C := pp.C,
    OPP_Przyim := pp.Przyim,
    setscore -4.80402104473326

mf-np--pp = mf-np--pp* pp %mf-np--pp[+pp:pred]%
    setscore -4.80402104473326

mf-np--pp = mf-np--pp* pp %mf-np--pp[+pp:v-mod]%
    setscore -2.85811089567794

mf-np--simpx = mf-np* simpx %mf-np[+simpx:os]%
    setscore -1.09861228866811

mf-np--simpx = mf-np--comma* simpx %mf-np--comma[+simpx:os]%
    setscore -1.09861228866811

mf-np--simpx = mf-np--comma* simpx %mf-np--comma[+simpx:v-mod]%
    setscore -1.09861228866811

mf-pp = pp* %MF[pp:compl1]%
    Block := 1,
    OPP := true,
    setscore -3.11351530921037

mf-pp = pp* %MF[pp:compl1]%
    Block := 1,
    OPP := true,
    OPP_C := C,
    OPP_Przyim := Przyim,
    setscore -2.19722457733622

mf-pp = pp* %MF[pp:compl1]%
    Block := 1,
    OPP := true,
    OPP_C := C,
    OPP_Przyim := Przyim,
    PRED := true,
    setscore -3.80666248977032

mf-pp = pp* %MF[pp:compl1]%
    Block := 1,
    OPP := true,
    PRED := true,
    setscore -3.80666248977032

mf-pp = pp* %MF[pp:compl2]%
    Block := 1,
    OA := true,
    OPP := true,
    OPP_C := C,
    OPP_Przyim := Przyim,
    setscore -3.80666248977032

mf-pp = pp* %MF[pp:mod]%
    Block := 1,
    setscore -1.86075234071501

mf-pp = pp* %MF[pp:mod]%
    Block := 1,
    PRED := true,
    setscore -2.19722457733622

mf-pp = pp* %MF[pp:v-mod]%
    Block := 1,
    setscore -1.16760516015506

mf-pp = pp* %MF[pp:v-mod]%
    Block := 1,
    ON := true,
    setscore -3.80666248977032

mf-pp = pp* %MF[pp:v-mod]%
    Block := 1,
    OPP := true,
    setscore -3.11351530921037

mf-pp = pp* %MF[pp:v-mod]%
    Block := 1,
    PRED := true,
    setscore -2.01490302054226

mf-pp--advx = mf-pp* advx %mf-pp[+advx:mod]%
    setscore -0.496436886313891

mf-pp--advx = mf-pp* advx %mf-pp[+advx:v-mod]%
    setscore -2.4423470353692

mf-pp--advx = mf-pp--advx* advx %mf-pp--advx[+advx:mod]%
    setscore -1.74919985480926

mf-pp--advx = mf-pp--np* advx %mf-pp--np[+advx:mod]%
    setscore -3.13549421592915

mf-pp--advx = mf-pp--pp* advx %mf-pp--pp[+advx:mod]%
    setscore -2.4423470353692

mf-pp--ap = mf-pp* ap %mf-pp[+ap:pred]%
    setscore -0.847297860387204

mf-pp--ap = mf-pp* ap %mf-pp[+ap:v-mod]%
    setscore -1.94591014905531

mf-pp--ap = mf-pp--advx* ap %mf-pp--advx[+ap:pred]%
    setscore -1.25276296849537

mf-pp--ap = mf-pp--advx* ap %mf-pp--advx[+ap:v-mod]%
    setscore -1.94591014905531

mf-pp--np = mf-pp* np %mf-pp[+np:compl1]%
    OA := true,
    OA_C := np.C,
    np.C == acc,
    setscore -2.36712361413162

mf-pp--np = mf-pp* np %mf-pp[+np:compl1]%
    OA_C := np.C,
    setscore -3.46573590279973

mf-pp--np = mf-pp* np %mf-pp[+np:compl1]%
    OD := true,
    OD_C := np.C,
    np.C == dat,
    setscore -3.46573590279973

mf-pp--np = mf-pp* np %mf-pp[+np:pred]%
    setscore -2.36712361413162

mf-pp--np = mf-pp* np %mf-pp[+np:subject]%
    ON := true,
    ON_Num := np.Num,
    ON_P := np.P,
    np.C == nom,
    setscore -2.07944154167984

mf-pp--np = mf-pp--advx* np %mf-pp--advx[+np:compl1]%
    OA := true,
    OA_C := np.C,
    np.C == acc,
    setscore -2.77258872223978

mf-pp--np = mf-pp--advx* np %mf-pp--advx[+np:compl2]%
    OD := true,
    OD_C := np.C,
    np.C == dat,
    setscore -3.46573590279973

mf-pp--np = mf-pp--advx* np %mf-pp--advx[+np:pred]%
    setscore -2.36712361413162

mf-pp--np = mf-pp--advx* np %mf-pp--advx[+np:subject]%
    ON := true,
    ON_Num := np.Num,
    ON_P := np.P,
    np.C == nom,
    setscore -2.07944154167984

mf-pp--np = mf-pp--np* np %mf-pp--np[+np:compl1]%
    OA := true,
    OA_C := np.C,
    np.C == acc,
    setscore -1.85629799036563

mf-pp--np = mf-pp--np* np %mf-pp--np[+np:subject]%
    ON_Num := np.Num,
    ON_P := np.P,
    setscore -3.46573590279973

mf-pp--np = mf-pp--pp* np %mf-pp--pp[+np:compl1]%
    OA := true,
    OA_C := np.C,
    np.C == acc,
    setscore -2.77258872223978

mf-pp--np = mf-pp--pp* np %mf-pp--pp[+np:pred]%
    setscore -3.46573590279973

mf-pp--np = mf-pp--pp* np %mf-pp--pp[+np:subject]%
    ON := true,
    ON_Num := np.Num,
    ON_P := np.P,
    np.C == nom,
    setscore -3.46573590279973

mf-pp--pp = mf-pp* pp %mf-pp[+pp:compl1]%
    OPP_C := pp.C,
    OPP_Przyim := pp.Przyim,
    setscore -2.70805020110221

mf-pp--pp = mf-pp* pp %mf-pp[+pp:mod]%
    setscore -2.01490302054226

mf-pp--pp = mf-pp* pp %mf-pp[+pp:v-mod]%
    setscore -0.916290731874155

mf-pp--pp = mf-pp--advx* pp %mf-pp--advx[+pp:compl1]%
    OPP_C := pp.C,
    OPP_Przyim := pp.Przyim,
    setscore -2.70805020110221

mf-pp--pp = mf-pp--advx* pp %mf-pp--advx[+pp:pred]%
    setscore -2.70805020110221

mf-pp--pp = mf-pp--advx* pp %mf-pp--advx[+pp:v-mod]%
    setscore -1.6094379124341

mf-pp--pp = mf-pp--np* pp %mf-pp--np[+pp:v-mod]%
    setscore -2.70805020110221

nf = nf-advx*
    setscore -3.52636052461616

nf = nf-ap*
    setscore -3.52636052461616

nf = nf-fkonj*
    setscore -3.52636052461616

nf = nf-np*
    setscore -1.44691898293633

nf = nf-np--simpx*
    setscore -3.52636052461616

nf = nf-pp*
    setscore -1.73460105538811

nf = nf-pp--r-simpx*
    setscore -3.52636052461616

nf = nf-r-simpx*
    setscore -1.91692261218206

nf = nf-r-simpx--advx*
    setscore -3.52636052461616

nf = nf-simpx*
    setscore -1.32913594727994

nf-advx = advx* %NF[advx:mod]%
    Block := 1,
    setscore 0

nf-ap = ap* %NF[ap:on-mod]%
    Block := 1,
    setscore 0

nf-fkonj = fkonj* %NF[fkonj:os]%
    Block := 1,
    setscore 0

nf-np = np* %NF[np:mod-mod]%
    Block := 1,
    setscore -2.19722457733622

nf-np = np* %NF[np:mod]%
    Block := 1,
    setscore -2.19722457733622

nf-np = np* %NF[np:oa-mod]%
    Block := 1,
    setscore -2.19722457733622

nf-np = np* %NF[np:oak]%
    Block := 1,
    setscore -2.19722457733622

nf-np = np* %NF[np:on-mod]%
    Block := 1,
    setscore -1.50407739677627

nf-np = np* %NF[np:onk]%
    Block := 1,
    setscore -2.19722457733622

nf-np = np* %NF[np:pred-mod]%
    Block := 1,
    setscore -2.19722457733622

nf-np = np* %NF[np:subject]%
    Block := 1,
    C == nom,
    ON := true,
    ON_Num := Num,
    ON_P := P,
    setscore -2.19722457733622

nf-np--comma = nf-np* comma %nf-np[+comma]%
    setscore 0

nf-np--simpx = nf-np--comma* simpx %nf-np--comma[+simpx:mod]%
    setscore 0

nf-pp = pp* %NF[pp:compl1]%
    Block := 1,
    OPP := true,
    OPP_C := C,
    OPP_Przyim := Przyim,
    setscore -1.94591014905531

nf-pp = pp* %NF[pp:compl2]%
    Block := 1,
    OPP := true,
    OPP_C := C,
    OPP_Przyim := Przyim,
    setscore -1.94591014905531

nf-pp = pp* %NF[pp:mod-mod]%
    Block := 1,
    setscore -1.94591014905531

nf-pp = pp* %NF[pp:mod]%
    Block := 1,
    setscore -1.94591014905531

nf-pp = pp* %NF[pp:oa-mod]%
    Block := 1,
    setscore -1.94591014905531

nf-pp = pp* %NF[pp:pred-mod]%
    Block := 1,
    setscore -1.94591014905531

nf-pp = pp* %NF[pp:v-mod]%
    Block := 1,
    setscore -1.94591014905531

nf-pp--comma = nf-pp* comma %nf-pp[+comma]%
    setscore 0

nf-pp--r-simpx = nf-pp--comma* r-simpx %nf-pp--comma[+r-simpx:on-mod]%
    setscore 0

nf-r-simpx = r-simpx* %NF[r-simpx:mod-mod]%
    Block := 1,
    setscore -1.79175946922806

nf-r-simpx = r-simpx* %NF[r-simpx:mod]%
    Block := 1,
    setscore -1.79175946922806

nf-r-simpx = r-simpx* %NF[r-simpx:oa-mod]%
    Block := 1,
    setscore -1.79175946922806

nf-r-simpx = r-simpx* %NF[r-simpx:on-mod]%
    Block := 1,
    setscore -1.79175946922806

nf-r-simpx = r-simpx* %NF[r-simpx:pred-mod]%
    Block := 1,
    setscore -1.09861228866811

nf-r-simpx--advx = nf-r-simpx* advx %nf-r-simpx[+advx:mod]%
    setscore 0

nf-simpx = simpx* %NF[simpx:fopp-mod]%
    Block := 1,
    setscore -2.19722457733622

nf-simpx = simpx* %NF[simpx:mod-mod]%
    Block := 1,
    setscore -2.19722457733622

nf-simpx = simpx* %NF[simpx:mod]%
    Block := 1,
    setscore -2.19722457733622

nf-simpx = simpx* %NF[simpx:oa-mod]%
    Block := 1,
    setscore -2.19722457733622

nf-simpx = simpx* %NF[simpx:on-mod]%
    Block := 1,
    setscore -2.19722457733622

nf-simpx = simpx* %NF[simpx:opp-mod]%
    Block := 1,
    setscore -2.19722457733622

nf-simpx = simpx* %NF[simpx:os]%
    Block := 1,
    setscore -2.19722457733622

nf-simpx = simpx* %NF[simpx:pred-mod]%
    Block := 1,
    setscore -2.19722457733622

nf-simpx = simpx* %NF[simpx:subject]%
    Block := 1,
    setscore -2.19722457733622

np = np-advx*
    { Block := false },
    setscore -4.89783979995091

np = np-advx--np*
    { Block := false },
    setscore -4.89783979995091

np = np-ap*
    { Block := false },
    setscore -4.89783979995091

np = np-article--ap*
    { Block := false },
    setscore -4.89783979995091

np = np-conj--ap*
    { Block := false },
    setscore -4.20469261939097

np = np-conj--np*
    { Block := false },
    setscore -4.89783979995091

np = np-fx--article*
    { Block := false },
    setscore -4.89783979995091

np = np-fx--np*
    { Block := false },
    setscore -4.89783979995091

np = np-fx--r-simpx*
    { Block := false },
    setscore -4.89783979995091

np = np-fx--simpx*
    { Block := false },
    setscore -4.89783979995091

np = np-noun*
    { Block := false },
    setscore -4.89783979995091

np = np-noun--advx*
    { Block := false },
    setscore -3.28840188751681

np = np-noun--ap*
    { Block := false },
    setscore -3.7992275112828

np = np-noun--article*
    { Block := false },
    setscore -3.28840188751681

np = np-noun--conj*
    { Block := false },
    setscore -3.51154543883102

np = np-noun--dp*
    { Block := false },
    setscore -4.89783979995091

np = np-noun--fx*
    { Block := false },
    setscore -4.89783979995091

np = np-noun--np*
    { Block := false },
    setscore -3.51154543883102

np = np-noun--pron*
    { Block := false },
    setscore -2.1897895988487

np = np-np--advx*
    { Block := false },
    setscore -3.28840188751681

np = np-np--ap*
    { Block := false },
    setscore -4.20469261939097

np = np-np--article*
    { Block := false },
    setscore -4.20469261939097

np = np-np--conj*
    { Block := false },
    setscore -4.89783979995091

np = np-np--dp*
    { Block := false },
    setscore -4.89783979995091

np = np-np--fx*
    { Block := false },
    setscore -4.20469261939097

np = np-np--np*
    { Block := false },
    setscore -1.56563528977571

np = np-np--pp*
    { Block := false },
    setscore -2.9519296508956

np = np-np--pron*
    { Block := false },
    setscore -4.89783979995091

np = np-np--r-simpx*
    { Block := false },
    setscore -4.20469261939097

np = np-np--simpx*
    { Block := false },
    setscore -4.20469261939097

np = np-num*
    { Block := false },
    setscore -4.89783979995091

np = np-num--article*
    { Block := false },
    setscore -4.89783979995091

np = np-pp*
    { Block := false },
    setscore -4.89783979995091

np = np-pp--r-simpx*
    { Block := false },
    setscore -4.89783979995091

np = np-prn*
    { Block := false },
    setscore -4.89783979995091

np = np-prn--advx*
    { Block := false },
    setscore -4.20469261939097

np = np-prn--article*
    { Block := false },
    setscore -4.20469261939097

np = np-prn--conj*
    { Block := false },
    setscore -4.89783979995091

np = np-prn--pp*
    { Block := false },
    setscore -4.89783979995091

np = np-prn--prn*
    { Block := false },
    setscore -4.89783979995091

np = np-prn--pron*
    { Block := false },
    setscore -4.89783979995091

np = np-pron*
    { Block := false },
    setscore -2.9519296508956

np = np-pron--advx*
    { Block := false },
    setscore -3.7992275112828

np = np-pron--ap*
    { Block := false },
    setscore -4.89783979995091

np = np-pron--article*
    { Block := false },
    setscore -4.20469261939097

np = np-pron--pron*
    { Block := false },
    setscore -4.89783979995091

np = np-trunc--noun*
    { Block := false },
    setscore -4.89783979995091

np = np-unknown*
    { Block := false },
    setscore -4.89783979995091

np = np-unknown--conj*
    { Block := false },
    setscore -4.89783979995091

np = np-unknown--pron*
    { Block := false },
    setscore -4.89783979995091

np-advx = advx* %NP[advx]%
    Block := 1,
    setscore -0.693147180559945

np-advx = advx* %NP[advx]%
    Block := 1,
    Num := any,
    P := any,
    R := any,
    setscore -0.693147180559945

np-advx--np = np-advx* np %np-advx[+np:konj]%
    C := np.C,
    setscore 0

np-ap = ap* %NP[ap]%
    Block := 1,
    setscore 0

np-article = article* %NP[article]%
    Block := 1,
    setscore 0

np-article--ap = np-article* ap %np-article[+ap]%
    setscore 0

np-conj = conj* %NP[conj]%
    Block := 1,
    Num := any,
    P := any,
    R := any,
    T == coord,
    setscore -1.09861228866811

np-conj = conj* %NP[conj]%
    Block := 1,
    T == comp,
    setscore -0.405465108108164

np-conj--ap = np-conj--article* ap %np-conj--article[+ap]%
    setscore -0.693147180559945

np-conj--ap = np-conj--pron* ap %np-conj--pron[+ap]%
    setscore -0.693147180559945

np-conj--article = np-conj* article %np-conj[+article]%
    setscore 0

np-conj--np = np-conj* np %np-conj[+np:konj]%
    C := np.C,
    setscore 0

np-conj--pron = np-conj* pron %np-conj[+pron]%
    setscore 0

np-fx = fx* %NP[fx:app]%
    Block := 1,
    setscore -0.693147180559945

np-fx = fx* %NP[fx]%
    Block := 1,
    setscore -0.693147180559945

np-fx--ap = ap np-fx* %np-fx[ap+]%
    setscore 0

np-fx--article = article np-fx--ap* %np-fx--ap[article+]%
    setscore 0

np-fx--np = np-fx* np %np-fx[+np:app]%
    setscore 0

np-fx--r-simpx = np-fx* r-simpx %np-fx[+r-simpx]%
    setscore 0

np-fx--simpx = np-fx* simpx %np-fx[+simpx:app]%
    setscore 0

np-noun = noun* %NP[noun]%
    Block := 1,
    setscore 0

np-noun--advx = advx np-noun* %np-noun[advx+]%
    setscore -1.79175946922806

np-noun--advx = advx np-noun--ap* %np-noun--ap[advx+]%
    setscore -1.09861228866811

np-noun--advx = advx np-noun--article* %np-noun--article[advx+]%
    setscore -1.79175946922806

np-noun--advx = advx np-noun--conj* %np-noun--conj[advx+]%
    setscore -1.79175946922806

np-noun--advx = advx np-noun--pron* %np-noun--pron[advx+]%
    setscore -1.79175946922806

np-noun--ap = ap np-noun* %np-noun[ap+]%
    C :== ap.C,
    Num :== ap.Num,
    R :== ap.R,
    ap.O == attr,
    setscore -0.233614851181505

np-noun--ap = ap np-noun--ap* %np-noun--ap[ap+]%
    C :== ap.C,
    Num :== ap.Num,
    R :== ap.R,
    ap.O == attr,
    setscore -1.79175946922806

np-noun--ap = ap np-noun--ap* %np-noun--ap[ap+]%
    C :== ap.C,
    R :== ap.R,
    ap.O == attr,
    setscore -3.17805383034795

np-noun--article = article np-noun* %np-noun[article+]%
    C :== article.C,
    Num :== article.Num,
    R :== article.R,
    setscore -1.09861228866811

np-noun--article = article np-noun--advx* %np-noun--advx[article+]%
    C :== article.C,
    Num :== article.Num,
    R :== article.R,
    setscore -2.19722457733622

np-noun--article = article np-noun--ap* %np-noun--ap[article+]%
    C :== article.C,
    setscore -2.19722457733622

np-noun--article = article np-noun--ap* %np-noun--ap[article+]%
    C :== article.C,
    Num :== article.Num,
    R :== article.R,
    setscore -0.810930216216329

np-noun--conj = conj np-noun* %np-noun[conj+]%
    conj.T == comp,
    setscore -1.6094379124341

np-noun--conj = conj np-noun--ap* %np-noun--ap[conj+]%
    conj.T == comp,
    setscore -0.916290731874155

np-noun--conj = conj np-noun--pron* %np-noun--pron[conj+]%
    conj.T == comp,
    setscore -0.916290731874155

np-noun--dp = dp np-noun* %np-noun[dp+]%
    setscore 0

np-noun--fx = fx np-noun* %np-noun[fx+]%
    setscore 0

np-noun--np = np np-noun* %np-noun[np+]%
    np.C == nom,
    np.Proper == true,
    setscore -1.38629436111989

np-noun--np = np np-noun* %np-noun[np:possessive+]%
    np.C == gen,
    setscore -1.38629436111989

np-noun--np = np np-noun* %np-noun[np:possessive+]%
    np.C == gen,
    np.Proper == true,
    setscore -1.38629436111989

np-noun--np = np np-noun--ap* %np-noun--ap[np:possessive+]%
    np.C == gen,
    setscore -1.38629436111989

np-noun--pron = pron np-noun* %np-noun[pron+]%
    C :== pron.C,
    Num :== pron.Num,
    R :== pron.R,
    pron.O == attr,
    setscore -2.89037175789617

np-noun--pron = pron np-noun* %np-noun[pron+]%
    C :== pron.C,
    Num :== pron.Num,
    R :== pron.R,
    pron.O == attr,
    pron.Typ == demons,
    setscore -2.19722457733622

np-noun--pron = pron np-noun* %np-noun[pron+]%
    C :== pron.C,
    Num :== pron.Num,
    R :== pron.R,
    pron.O == attr,
    pron.Typ == indef,
    setscore -2.19722457733622

np-noun--pron = pron np-noun* %np-noun[pron+]%
    C :== pron.C,
    Num :== pron.Num,
    R :== pron.R,
    pron.O == attr,
    pron.Typ == poss,
    setscore -2.89037175789617

np-noun--pron = pron np-noun* %np-noun[pron+]%
    Num :== pron.Num,
    pron.O == attr,
    pron.Typ == rel,
    setscore -2.89037175789617

np-noun--pron = pron np-noun* %np-noun[pron+]%
    R :== pron.R,
    pron.O == attr,
    pron.Typ == rel,
    setscore -2.89037175789617

np-noun--pron = pron np-noun* %np-noun[pron+]%
    pron.O == attr,
    pron.Typ == rel,
    setscore -2.89037175789617

np-noun--pron = pron np-noun--ap* %np-noun--ap[pron+]%
    C :== pron.C,
    Num :== pron.Num,
    R :== pron.R,
    pron.O == attr,
    pron.Typ == demons,
    setscore -2.89037175789617

np-noun--pron = pron np-noun--ap* %np-noun--ap[pron+]%
    C :== pron.C,
    Num :== pron.Num,
    R :== pron.R,
    pron.O == attr,
    pron.Typ == indef,
    setscore -2.19722457733622

np-noun--pron = pron np-noun--ap* %np-noun--ap[pron+]%
    C :== pron.C,
    Num :== pron.Num,
    R :== pron.R,
    pron.O == attr,
    pron.Typ == poss,
    setscore -2.19722457733622

np-noun--pron = pron np-noun--ap* %np-noun--ap[pron+]%
    Num :== pron.Num,
    pron.O == attr,
    pron.Typ == rel,
    setscore -2.89037175789617

np-noun--pron = pron np-noun--article* %np-noun--article[pron+]%
    C :== pron.C,
    Num :== pron.Num,
    R :== pron.R,
    pron.O == attr,
    pron.Typ == indef,
    setscore -1.79175946922806

np-np = np* %NP[np:app]%
    Block := 1,
    setscore -1.66770682055808

np-np = np* %NP[np:konj]%
    Block := 1,
    Num := any,
    P := any,
    R := any,
    setscore -1.66770682055808

np-np = np* %NP[np]%
    Block := 1,
    setscore -0.473784352085642

np-np--advx = advx np-np* %np-np[advx+]%
    setscore -0.810930216216329

np-np--advx = np-np* advx %np-np[+advx]%
    setscore -1.50407739677627

np-np--advx = np-np--comma* advx %np-np--comma[+advx]%
    setscore -2.19722457733622

np-np--advx = np-np--conj* advx %np-np--conj[+advx]%
    setscore -2.19722457733622

np-np--advx = np-np--np* advx %np-np--np[+advx:konj]%
    setscore -2.19722457733622

np-np--ap = ap np-np* %np-np[ap+]%
    C :== ap.C,
    Num :== ap.Num,
    R :== ap.R,
    ap.O == attr,
    setscore -0.405465108108164

np-np--ap = np-np--conj* ap %np-np--conj[+ap:konj]%
    setscore -1.09861228866811

np-np--article = article np-np* %np-np[article+]%
    C :== article.C,
    Num :== article.Num,
    R :== article.R,
    setscore -0.693147180559945

np-np--article = article np-np--ap* %np-np--ap[article+]%
    C :== article.C,
    Num :== article.Num,
    R :== article.R,
    setscore -0.693147180559945

np-np--comma = np-np* comma %np-np[+comma]%
    setscore 0

np-np--conj = conj np-np* %np-np[conj+]%
    conj.T == comp,
    setscore -1.20397280432594

np-np--conj = conj np-np* %np-np[conj+]%
    conj.T == coord,
    setscore -1.6094379124341

np-np--conj = np-np* conj %np-np[+conj]%
    conj.T == coord,
    setscore -0.916290731874155

np-np--conj = np-np--comma* conj %np-np--comma[+conj]%
    conj.T == coord,
    setscore -2.30258509299405

np-np--dp = dp np-np* %np-np[dp+]%
    setscore 0

np-np--fx = fx np-np* %np-np[fx+]%
    setscore -0.693147180559945

np-np--fx = np-np* fx %np-np[+fx:app]%
    setscore -0.693147180559945

np-np--np = np np-np* %np-np[np+]%
    np.C == nom,
    setscore -3.36729582998647

np-np--np = np-np* np %np-np[+np:app]%
    C == np.C,
    Num == np.Num,
    setscore -3.36729582998647

np-np--np = np-np* np %np-np[+np:app]%
    C == np.C,
    Num == np.Num,
    P == np.P,
    setscore -3.36729582998647

np-np--np = np-np* np %np-np[+np:app]%
    Num == np.Num,
    P == np.P,
    setscore -3.36729582998647

np-np--np = np-np* np %np-np[+np:app]%
    P == np.P,
    setscore -3.36729582998647

np-np--np = np-np* np %np-np[+np:app]%
    setscore -3.36729582998647

np-np--np = np-np* np %np-np[+np:konj]%
    C == np.C,
    setscore -3.36729582998647

np-np--np = np-np* np %np-np[+np:konj]%
    setscore -3.36729582998647

np-np--np = np-np* np %np-np[+np:possessive]%
    np.C == gen,
    setscore -3.36729582998647

np-np--np = np-np* np %np-np[+np:possessive]%
    np.C == gen,
    np.Proper == true,
    setscore -3.36729582998647

np-np--np = np-np* np %np-np[+np]%
    np.C == acc,
    setscore -3.36729582998647

np-np--np = np-np* np %np-np[+np]%
    np.C == dat,
    setscore -3.36729582998647

np-np--np = np-np* np %np-np[+np]%
    np.C == nom,
    setscore -3.36729582998647

np-np--np = np-np* np %np-np[+np]%
    np.Cardinal == true,
    setscore -3.36729582998647

np-np--np = np-np* np %np-np[+np]%
    setscore -3.36729582998647

np-np--np = np-np--advx* np %np-np--advx[+np:possessive]%
    np.C == gen,
    setscore -3.36729582998647

np-np--np = np-np--comma* np %np-np--comma[+np:app]%
    C == np.C,
    Num == np.Num,
    P == np.P,
    setscore -3.36729582998647

np-np--np = np-np--comma* np %np-np--comma[+np:app]%
    C == np.C,
    P == np.P,
    setscore -3.36729582998647

np-np--np = np-np--comma* np %np-np--comma[+np:app]%
    Num == np.Num,
    P == np.P,
    setscore -3.36729582998647

np-np--np = np-np--comma* np %np-np--comma[+np:konj]%
    C == np.C,
    setscore -2.67414864942653

np-np--np = np-np--comma* np %np-np--comma[+np:konj]%
    setscore -3.36729582998647

np-np--np = np-np--comma* np %np-np--comma[+np]%
    np.C == acc,
    setscore -3.36729582998647

np-np--np = np-np--comma* np %np-np--comma[+np]%
    np.C == nom,
    setscore -3.36729582998647

np-np--np = np-np--conj* np %np-np--conj[+np:konj]%
    C == np.C,
    setscore -2.67414864942653

np-np--np = np-np--conj* np %np-np--conj[+np:konj]%
    setscore -2.67414864942653

np-np--np = np-np--conj* np %np-np--conj[+np:possessive]%
    np.C == gen,
    setscore -3.36729582998647

np-np--pp = np-np* pp %np-np[+pp:compl1]%
    setscore -1.94591014905531

np-np--pp = np-np--advx* pp %np-np--advx[+pp:compl1]%
    setscore -1.25276296849537

np-np--pp = np-np--comma* pp %np-np--comma[+pp:compl1]%
    setscore -1.94591014905531

np-np--pp = np-np--conj* pp %np-np--conj[+pp:compl1]%
    setscore -1.25276296849537

np-np--pp = pp np-np--advx* %np-np--advx[pp:compl1+]%
    setscore -1.94591014905531

np-np--pron = pron np-np* %np-np[pron+]%
    C :== pron.C,
    Num :== pron.Num,
    R :== pron.R,
    pron.O == attr,
    pron.Typ == indef,
    setscore 0

np-np--r-simpx = np-np* r-simpx %np-np[+r-simpx]%
    setscore -0.693147180559945

np-np--r-simpx = np-np--comma* r-simpx %np-np--comma[+r-simpx]%
    setscore -0.693147180559945

np-np--simpx = np-np* simpx %np-np[+simpx:app]%
    setscore -0.693147180559945

np-np--simpx = np-np--comma* simpx %np-np--comma[+simpx]%
    setscore -0.693147180559945

np-num = num* %NP[num]%
    Block := 1,
    Cardinal := true,
    setscore 0

np-num--article = article np-num* %np-num[article+]%
    setscore 0

np-pp = pp* %NP[pp:compl1]%
    Block := 1,
    setscore -0.693147180559945

np-pp = pp* %NP[pp]%
    Block := 1,
    setscore -0.693147180559945

np-pp--comma = np-pp* comma %np-pp[+comma]%
    setscore 0

np-pp--r-simpx = np-pp--comma* r-simpx %np-pp--comma[+r-simpx]%
    setscore 0

np-prn = prn* %NP[prn]%
    Block := 1,
    setscore -0.251314428280906

np-prn = prn* %NP[prn]%
    Block := 1,
    Proper := true,
    setscore -1.50407739677627

np-prn--advx = advx np-prn* %np-prn[advx+]%
    setscore -0.693147180559945

np-prn--advx = advx np-prn--article* %np-prn--article[advx+]%
    setscore -0.693147180559945

np-prn--ap = ap np-prn* %np-prn[ap+]%
    C :== ap.C,
    Num :== ap.Num,
    R :== ap.R,
    ap.O == attr,
    setscore 0

np-prn--article = article np-prn* %np-prn[article+]%
    C :== article.C,
    Num :== article.Num,
    R :== article.R,
    setscore -0.405465108108164

np-prn--article = article np-prn--ap* %np-prn--ap[article+]%
    C :== article.C,
    Num :== article.Num,
    R :== article.R,
    setscore -1.09861228866811

np-prn--conj = conj np-prn* %np-prn[conj+]%
    conj.T == comp,
    setscore 0

np-prn--pp = np-prn* pp %np-prn[+pp:compl1]%
    setscore 0

np-prn--prn = np-prn* prn %np-prn[+prn]%
    setscore 0

np-prn--pron = pron np-prn* %np-prn[pron+]%
    C :== pron.C,
    Num :== pron.Num,
    R :== pron.R,
    pron.O == attr,
    pron.Typ == indef,
    setscore 0

np-pron = pron* %NP[pron]%
    Block := 1,
    setscore -2.63905732961526

np-pron = pron* %NP[pron]%
    Block := 1,
    O == sub,
    setscore -2.63905732961526

np-pron = pron* %NP[pron]%
    Block := 1,
    O == sub,
    Typ == demons,
    setscore -1.54044504094715

np-pron = pron* %NP[pron]%
    Block := 1,
    O == sub,
    Typ == indef,
    setscore -1.25276296849537

np-pron = pron* %NP[pron]%
    Block := 1,
    O == sub,
    Typ == pers,
    setscore -1.94591014905531

np-pron = pron* %NP[pron]%
    Block := 1,
    O == sub,
    Typ == poss,
    setscore -2.63905732961526

np-pron = pron* %NP[pron]%
    Block := 1,
    O == sub,
    Typ == rel,
    setscore -2.63905732961526

np-pron = pron* %NP[pron]%
    Block := 1,
    Typ == refl,
    setscore -2.63905732961526

np-pron--advx = advx np-pron* %np-pron[advx+]%
    setscore 0

np-pron--ap = ap np-pron* %np-pron[ap+]%
    setscore -0.693147180559945

np-pron--ap = np-pron* ap %np-pron[+ap]%
    setscore -0.693147180559945

np-pron--article = article np-pron* %np-pron[article+]%
    setscore -0.693147180559945

np-pron--article = article np-pron--ap* %np-pron--ap[article+]%
    setscore -0.693147180559945

np-pron--pron = pron np-pron* %np-pron[pron+]%
    setscore 0

np-trunc = trunc* %NP[trunc:konj]%
    Block := 1,
    setscore 0

np-trunc--conj = np-trunc* conj %np-trunc[+conj]%
    conj.T == coord,
    setscore 0

np-trunc--noun = np-trunc--conj* noun %np-trunc--conj[+noun:konj]%
    setscore 0

np-unknown = unknown* %NP[unknown]%
    Block := 1,
    setscore 0

np-unknown--conj = conj np-unknown* %np-unknown[conj+]%
    conj.T == comp,
    setscore 0

np-unknown--pron = pron np-unknown* %np-unknown[pron+]%
    setscore 0

p-simpx = p-simpx-simpx--simpx*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore 0

p-simpx-simpx = simpx* %P-SIMPX[simpx:konj]%
    Block := 1,
    setscore 0

p-simpx-simpx--comma = p-simpx-simpx* comma %p-simpx-simpx[+comma]%
    setscore 0

p-simpx-simpx--conj = p-simpx-simpx--comma* conj %p-simpx-simpx--comma[+conj]%
    setscore 0

p-simpx-simpx--simpx = p-simpx-simpx--conj* simpx %p-simpx-simpx--conj[+simpx:konj]%
    setscore 0

parord = parord-conj*
    { Block := false },
    setscore 0

parord-conj = conj* %PARORD[conj]%
    Block := 1,
    setscore 0

pp = pp-adverb*
    { Block := false },
    setscore -2.70805020110221

pp = pp-advx--pp*
    { Block := false },
    setscore -3.40119738166216

pp = pp-advx--prep*
    { Block := false },
    setscore -3.40119738166216

pp = pp-ap--prep*
    { Block := false },
    setscore -2.70805020110221

pp = pp-fx--prep*
    { Block := false },
    setscore -3.40119738166216

pp = pp-fx--prepart*
    { Block := false },
    setscore -3.40119738166216

pp = pp-np--advx*
    { Block := false },
    setscore -2.01490302054226

pp = pp-np--conj*
    { Block := false },
    setscore -2.30258509299405

pp = pp-np--np*
    { Block := false },
    setscore -3.40119738166216

pp = pp-np--prep*
    { Block := false },
    setscore -2.70805020110221

pp = pp-np--prepart*
    { Block := false },
    setscore -3.40119738166216

pp = pp-pp--advx*
    { Block := false },
    setscore -2.01490302054226

pp = pp-pp--np*
    { Block := false },
    setscore -3.40119738166216

pp = pp-pp--pp*
    { Block := false },
    setscore -2.01490302054226

pp = pp-pp--prep*
    { Block := false },
    setscore -3.40119738166216

pp = pp-prep*
    { Block := false },
    setscore -3.40119738166216

pp-adverb = adverb* %PP[adverb]%
    Block := 1,
    T == pron,
    setscore -0.693147180559945

pp-adverb = adverb* %PP[adverb]%
    Block := 1,
    T == rel,
    setscore -0.693147180559945

pp-advx = advx* %PP[advx]%
    Block := 1,
    setscore 0

pp-advx--comma = pp-advx--pp* comma %pp-advx--pp[+comma]%
    setscore 0

pp-advx--conj = pp-advx--comma* conj %pp-advx--comma[+conj]%
    setscore 0

pp-advx--pp = pp-advx* pp %pp-advx[+pp:konj]%
    setscore -0.693147180559945

pp-advx--pp = pp-advx--conj* pp %pp-advx--conj[+pp:konj]%
    setscore -0.693147180559945

pp-advx--prep = prep pp-advx* %pp-advx[prep+]%
    Przyim := prep.Core,
    setscore 0

pp-ap = ap* %PP[ap]%
    Block := 1,
    O == attr,
    setscore -0.693147180559945

pp-ap = ap* %PP[ap]%
    Block := 1,
    O == pred,
    setscore -0.693147180559945

pp-ap--prep = prep pp-ap* %pp-ap[prep+]%
    Przyim := prep.Core,
    setscore 0

pp-fx = fx* %PP[fx]%
    Block := 1,
    setscore 0

pp-fx--prep = prep pp-fx* %pp-fx[prep+]%
    Przyim := prep.Core,
    setscore 0

pp-fx--prepart = prepart pp-fx* %pp-fx[prepart+]%
    Przyim := prepart.Core,
    setscore 0

pp-np = np* %PP[np]%
    Block := 1,
    setscore 0

pp-np--advx = advx pp-np--advx* %pp-np--advx[advx+]%
    setscore -1.6094379124341

pp-np--advx = advx pp-np--prep* %pp-np--prep[advx+]%
    setscore -0.510825623765991

pp-np--advx = advx pp-np--prepart* %pp-np--prepart[advx+]%
    setscore -1.6094379124341

pp-np--conj = conj pp-np--prep* %pp-np--prep[conj+]%
    setscore -0.405465108108164

pp-np--conj = conj pp-np--prepart* %pp-np--prepart[conj+]%
    setscore -1.09861228866811

pp-np--np = np pp-np--prep* %pp-np--prep[np+]%
    np.C == acc,
    setscore 0

pp-np--prep = prep pp-np* %pp-np[prep+]%
    C :== prep.Dop1,
    Przyim := prep.Core,
    setscore -0.470003629245736

pp-np--prep = prep pp-np* %pp-np[prep+]%
    Przyim := prep.Core,
    setscore -0.980829253011726

pp-np--prepart = prepart pp-np* %pp-np[prepart+]%
    C :== prepart.Dop1,
    Przyim := prepart.Core,
    setscore 0

pp-pp = pp* %PP[pp:konj]%
    Block := 1,
    setscore -0.916290731874155

pp-pp = pp* %PP[pp]%
    Block := 1,
    setscore -0.510825623765991

pp-pp--advx = advx pp-pp* %pp-pp[advx+]%
    setscore -0.916290731874155

pp-pp--advx = advx pp-pp--advx* %pp-pp--advx[advx+]%
    setscore -1.6094379124341

pp-pp--advx = pp-pp* advx %pp-pp[+advx]%
    setscore -1.6094379124341

pp-pp--advx = pp-pp--conj* advx %pp-pp--conj[+advx:konj]%
    setscore -1.6094379124341

pp-pp--comma = pp-pp* comma %pp-pp[+comma]%
    setscore 0

pp-pp--conj = pp-pp* conj %pp-pp[+conj]%
    setscore -0.693147180559945

pp-pp--conj = pp-pp--pp* conj %pp-pp--pp[+conj]%
    setscore -0.693147180559945

pp-pp--np = pp-pp* np %pp-pp[+np]%
    setscore 0

pp-pp--pp = pp-pp* pp %pp-pp[+pp:compl1]%
    setscore -1.6094379124341

pp-pp--pp = pp-pp* pp %pp-pp[+pp:konj]%
    setscore -0.916290731874155

pp-pp--pp = pp-pp--comma* pp %pp-pp--comma[+pp:konj]%
    setscore -1.6094379124341

pp-pp--pp = pp-pp--conj* pp %pp-pp--conj[+pp:konj]%
    setscore -1.6094379124341

pp-pp--prep = prep pp-pp* %pp-pp[prep+]%
    setscore 0

pp-prep = prep* %PP[prep]%
    Block := 1,
    Przyim := Core,
    setscore 0

r-simpx = r-simpx-c--fkoord*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    setscore -3.2188758248682

r-simpx = r-simpx-c--nf*
    C_OA = true,
    C_ON = false,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    setscore -3.2188758248682

r-simpx = r-simpx-c--nf*
    C_ON = false,
    MF_OA = true,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    setscore -3.2188758248682

r-simpx = r-simpx-c--nf*
    C_ON = true,
    MF_OA = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    setscore -2.52572864430826

r-simpx = r-simpx-c--nf*
    C_ON = true,
    MF_OD = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    setscore -3.2188758248682

r-simpx = r-simpx-c--nf*
    C_ON = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    setscore -2.12026353620009

r-simpx = r-simpx-c--vc*
    C_OA = true,
    C_ON = false,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    setscore -2.52572864430826

r-simpx = r-simpx-c--vc*
    C_ON = false,
    MF_OA = true,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    setscore -3.2188758248682

r-simpx = r-simpx-c--vc*
    C_ON = false,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    setscore -1.6094379124341

r-simpx = r-simpx-c--vc*
    C_ON = true,
    MF_OA = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    setscore -2.12026353620009

r-simpx = r-simpx-c--vc*
    C_ON = true,
    MF_OD = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    setscore -3.2188758248682

r-simpx = r-simpx-c--vc*
    C_ON = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    setscore -2.12026353620009

r-simpx = r-simpx-r-simpx--r-simpx*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    setscore -3.2188758248682

r-simpx-c = c* %R-SIMPX[c]%
    Block := 1,
    setscore -3.17805383034795

r-simpx-c = c* %R-SIMPX[c]%
    Block := 1,
    C_OA := OA,
    ON = false,
    OPP = false,
    setscore -2.07944154167984

r-simpx-c = c* %R-SIMPX[c]%
    Block := 1,
    C_ON := ON,
    OPP = false,
    setscore -0.613104472886409

r-simpx-c = c* %R-SIMPX[c]%
    Block := 1,
    ON = false,
    OPP = false,
    setscore -1.56861591791385

r-simpx-c = c* %R-SIMPX[c]%
    Block := 1,
    ON = false,
    OPP = true,
    setscore -2.484906649788

r-simpx-c--comma = r-simpx-c--mf* comma %r-simpx-c--mf[+comma]%
    setscore -1.94591014905531

r-simpx-c--comma = r-simpx-c--vc* comma %r-simpx-c--vc[+comma]%
    setscore -0.154150679827258

r-simpx-c--fkoord = r-simpx-c* fkoord %r-simpx-c[+fkoord]%
    setscore 0

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
    MF_OA := mf.OA,
    MF_ON := mf.ON,
    OA_C := mf.OA_C,
    ON_Num := mf.ON_Num,
    ON_P := mf.ON_P,
    mf.OPP = false,
    setscore -3.09104245335832

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
    MF_OA := mf.OA,
    MF_ON := mf.ON,
    OA_C := mf.OA_C,
    ON_Num := mf.ON_Num,
    ON_P := mf.ON_P,
    mf.OPP = true,
    setscore -3.09104245335832

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
    MF_OA := mf.OA,
    OA_C := mf.OA_C,
    OPP_C := mf.OPP_C,
    OPP_Przyim := mf.OPP_Przyim,
    mf.ON = false,
    mf.OPP = true,
    setscore -3.09104245335832

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
    MF_OA := mf.OA,
    OA_C := mf.OA_C,
    mf.ON = false,
    mf.OPP = false,
    setscore -1.99243016469021

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
    MF_OA := mf.OA,
    OA_C := mf.OA_C,
    mf.ON = false,
    mf.OPP = false,
    mf.PRED = true,
    setscore -3.09104245335832

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
    MF_OD := mf.OD,
    OD_C := mf.OD_C,
    mf.ON = false,
    mf.OPP = false,
    setscore -3.09104245335832

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
    MF_OD := mf.OD,
    OD_C := mf.OD_C,
    mf.ON = false,
    mf.OPP = false,
    mf.PRED = true,
    setscore -3.09104245335832

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
    MF_ON := mf.ON,
    ON_Num := mf.ON_Num,
    ON_P := mf.ON_P,
    OPP_C := mf.OPP_C,
    OPP_Przyim := mf.OPP_Przyim,
    mf.OPP = true,
    setscore -2.39789527279837

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
    MF_ON := mf.ON,
    ON_Num := mf.ON_Num,
    ON_P := mf.ON_P,
    mf.OPP = false,
    setscore -1.70474809223843

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
    MF_ON := mf.ON,
    ON_Num := mf.ON_Num,
    ON_P := mf.ON_P,
    mf.OPP = false,
    mf.PRED = true,
    setscore -3.09104245335832

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
    MF_ON := mf.ON,
    mf.OPP = false,
    setscore -3.09104245335832

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
    OPP_C := mf.OPP_C,
    OPP_Przyim := mf.OPP_Przyim,
    mf.ON = false,
    mf.OPP = true,
    setscore -3.09104245335832

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
    mf.ON = false,
    mf.OPP = false,
    setscore -3.09104245335832

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
    mf.ON = false,
    mf.OPP = false,
    mf.PRED = true,
    setscore -2.39789527279837

r-simpx-c--mf = r-simpx-c* mf %r-simpx-c[+mf]%
    mf.ON = false,
    mf.OPP = true,
    setscore -3.09104245335832

r-simpx-c--nf = r-simpx-c--comma* nf %r-simpx-c--comma[+nf]%
    nf.ON = false,
    nf.OPP = false,
    setscore -0.287682072451781

r-simpx-c--nf = r-simpx-c--vc* nf %r-simpx-c--vc[+nf]%
    nf.ON = false,
    nf.OPP = false,
    setscore -1.38629436111989

r-simpx-c--vc = r-simpx-c* vc %r-simpx-c[+vc]%
    vc.Num == ON_Num,
    vc.P == ON_P,
    setscore -3.13549421592915

r-simpx-c--vc = r-simpx-c--comma* vc %r-simpx-c--comma[+vc]%
    vc.Dop1 == OA_C,
    vc.Num == ON_Num,
    vc.P == ON_P,
    vc.Przyim1 == zero,
    setscore -3.13549421592915

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
    vc.Dop1 == OA_C,
    vc.Dop2 == OPP_C,
    vc.Num == ON_Num,
    vc.P == ON_P,
    vc.Przyim1 == zero,
    vc.Przyim2 == OPP_Przyim,
    setscore -2.4423470353692

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
    vc.Dop1 == OA_C,
    vc.Num == ON_Num,
    vc.P == ON_P,
    vc.Przyim1 == zero,
    setscore -1.18958406687384

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
    vc.Dop1 == OD_C,
    vc.Num == ON_Num,
    vc.P == ON_P,
    vc.Przyim1 == zero,
    setscore -2.4423470353692

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
    vc.Dop1 == OPP_C,
    vc.Num == ON_Num,
    vc.P == ON_P,
    vc.Przyim1 == OPP_Przyim,
    setscore -2.03688192726104

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
    vc.Dop1 == OPP_C,
    vc.Przyim1 == OPP_Przyim,
    setscore -3.13549421592915

r-simpx-c--vc = r-simpx-c--mf* vc %r-simpx-c--mf[+vc]%
    vc.Num == ON_Num,
    vc.P == ON_P,
    setscore -1.34373474670109

r-simpx-r-simpx = r-simpx* %R-SIMPX[r-simpx:konj]%
    Block := 1,
    setscore 0

r-simpx-r-simpx--conj = r-simpx-r-simpx* conj %r-simpx-r-simpx[+conj]%
    setscore 0

r-simpx-r-simpx--r-simpx = r-simpx-r-simpx--conj* r-simpx %r-simpx-r-simpx--conj[+r-simpx:konj]%
    setscore 0

simpx = simpx-c--fkoord*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-c--mf*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -4.93087032562739

simpx = simpx-c--nf*
    C_ON = false,
    MF_OA = true,
    MF_OD = true,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-c--nf*
    C_ON = false,
    MF_OA = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-c--nf*
    C_ON = false,
    MF_OD = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-c--nf*
    C_ON = false,
    MF_OD = true,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-c--nf*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -4.52540521751923

simpx = simpx-c--nf*
    C_ON = false,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -3.83225803695928

simpx = simpx-c--nf*
    C_ON = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-c--vc*
    C_OA = true,
    C_ON = false,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-c--vc*
    C_ON = false,
    MF_OA = true,
    MF_OD = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-c--vc*
    C_ON = false,
    MF_OA = true,
    MF_OD = true,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-c--vc*
    C_ON = false,
    MF_OA = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -4.93087032562739

simpx = simpx-c--vc*
    C_ON = false,
    MF_OA = true,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -3.67810735713203

simpx = simpx-c--vc*
    C_ON = false,
    MF_OD = true,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -4.93087032562739

simpx = simpx-c--vc*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -4.52540521751923

simpx = simpx-c--vc*
    C_ON = false,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -3.42679292885112

simpx = simpx-c--vc*
    C_ON = true,
    MF_OA = true,
    MF_OD = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-c--vc*
    C_ON = true,
    MF_OA = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-c--vc*
    C_ON = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -4.52540521751923

simpx = simpx-koord--fkoord*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -4.93087032562739

simpx = simpx-koord--vc*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-lk--comma*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = true,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-lk--fkoord*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-lk--fkoord*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = true,
    { Block := false },
    setscore -4.52540521751923

simpx = simpx-lk--koord*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = true,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-lk--mf*
    C_ON = false,
    MF_OA = true,
    MF_OD = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-lk--mf*
    C_ON = false,
    MF_OA = true,
    MF_OD = true,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-lk--mf*
    C_ON = false,
    MF_OA = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-lk--mf*
    C_ON = false,
    MF_OA = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = true,
    { Block := false },
    setscore -3.32143241319329

simpx = simpx-lk--mf*
    C_ON = false,
    MF_OA = true,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -3.32143241319329

simpx = simpx-lk--mf*
    C_ON = false,
    MF_OD = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = true,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-lk--mf*
    C_ON = false,
    MF_OD = true,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -4.93087032562739

simpx = simpx-lk--mf*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -3.5445759645075

simpx = simpx-lk--mf*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = true,
    { Block := false },
    setscore -3.5445759645075

simpx = simpx-lk--mf*
    C_ON = false,
    MF_ON = true,
    NF_ON = false,
    VF_OA = true,
    VF_ON = false,
    { Block := false },
    setscore -4.52540521751923

simpx = simpx-lk--mf*
    C_ON = false,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -2.85142878394756

simpx = simpx-lk--nf*
    C_ON = false,
    MF_OA = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -4.52540521751923

simpx = simpx-lk--nf*
    C_ON = false,
    MF_OA = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = true,
    { Block := false },
    setscore -3.42679292885112

simpx = simpx-lk--nf*
    C_ON = false,
    MF_OA = true,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -3.5445759645075

simpx = simpx-lk--nf*
    C_ON = false,
    MF_OD = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-lk--nf*
    C_ON = false,
    MF_OD = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = true,
    { Block := false },
    setscore -4.93087032562739

simpx = simpx-lk--nf*
    C_ON = false,
    MF_OD = true,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-lk--nf*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -3.13911085639934

simpx = simpx-lk--nf*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = true,
    { Block := false },
    setscore -2.48852329025819

simpx = simpx-lk--nf*
    C_ON = false,
    MF_ON = false,
    NF_ON = true,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-lk--nf*
    C_ON = false,
    MF_ON = true,
    NF_ON = false,
    VF_OA = true,
    VF_ON = false,
    { Block := false },
    setscore -4.01457959375324

simpx = simpx-lk--nf*
    C_ON = false,
    MF_ON = true,
    NF_ON = false,
    VF_OD = true,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-lk--nf*
    C_ON = false,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -2.85142878394756

simpx = simpx-lk--vc*
    C_ON = false,
    MF_OA = true,
    MF_OD = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = true,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-lk--vc*
    C_ON = false,
    MF_OA = true,
    MF_OD = true,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -4.52540521751923

simpx = simpx-lk--vc*
    C_ON = false,
    MF_OA = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-lk--vc*
    C_ON = false,
    MF_OA = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = true,
    { Block := false },
    setscore -3.67810735713203

simpx = simpx-lk--vc*
    C_ON = false,
    MF_OA = true,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -3.5445759645075

simpx = simpx-lk--vc*
    C_ON = false,
    MF_OD = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = true,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-lk--vc*
    C_ON = false,
    MF_OD = true,
    MF_ON = true,
    NF_ON = false,
    VF_OA = true,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-lk--vc*
    C_ON = false,
    MF_OD = true,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -4.52540521751923

simpx = simpx-lk--vc*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -4.23772314506745

simpx = simpx-lk--vc*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = true,
    { Block := false },
    setscore -3.5445759645075

simpx = simpx-lk--vc*
    C_ON = false,
    MF_ON = true,
    NF_ON = false,
    VF_OA = true,
    VF_ON = false,
    { Block := false },
    setscore -4.93087032562739

simpx = simpx-lk--vc*
    C_ON = false,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -2.85142878394756

simpx = simpx-lk--vf*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-lk--vf*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = true,
    { Block := false },
    setscore -4.93087032562739

simpx = simpx-mf--mf*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-mf--nf*
    C_ON = false,
    MF_OD = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-mf--nf*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-mf--vc*
    C_OA = true,
    C_ON = false,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-mf--vc*
    C_ON = false,
    MF_OA = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -4.52540521751923

simpx = simpx-mf--vc*
    C_ON = false,
    MF_OD = true,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-mf--vc*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -4.93087032562739

simpx = simpx-mf--vc*
    C_ON = false,
    MF_ON = true,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-simpx--simpx*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -4.23772314506745

simpx = simpx-vc*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-vf--fkoord*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-vf--mf*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx = simpx-vf--nf*
    C_ON = false,
    MF_ON = false,
    NF_ON = false,
    VF_ON = false,
    { Block := false },
    setscore -5.62401750618734

simpx-c = c* %SIMPX[c]%
    Block := 1,
    setscore -3.87120101090789

simpx-c = c* %SIMPX[c]%
    Block := 1,
    CONJ == 1,
    setscore -3.17805383034795

simpx-c = c* %SIMPX[c]%
    Block := 1,
    CONJ == 1,
    ON = false,
    OPP = false,
    setscore -0.826678573184468

simpx-c = c* %SIMPX[c]%
    Block := 1,
    CONJ == 1,
    OPP = false,
    setscore -1.56861591791385

simpx-c = c* %SIMPX[c]%
    Block := 1,
    C_OA := OA,
    ON = false,
    OPP = false,
    setscore -3.87120101090789

simpx-c = c* %SIMPX[c]%
    Block := 1,
    C_ON := ON,
    OPP = false,
    setscore -2.07944154167984

simpx-c = c* %SIMPX[c]%
    Block := 1,
    ON = false,
    OPP = false,
    setscore -2.484906649788

simpx-c = c* %SIMPX[c]%
    Block := 1,
    ON = false,
    OPP = false,
    PRED = true,
    setscore -3.87120101090789

simpx-c = c* %SIMPX[c]%
    Block := 1,
    ON = false,
    OPP = true,
    setscore -3.87120101090789

simpx-c = c* %SIMPX[c]%
    Block := 1,
    OPP = false,
    PRED = true,
    setscore -3.87120101090789

simpx-c--comma = simpx-c--vc* comma %simpx-c--vc[+comma]%
    setscore 0

simpx-c--fkoord = simpx-c* fkoord %simpx-c[+fkoord]%
    setscore 0

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    MF_OA := mf.OA,
    MF_OD := mf.OD,
    MF_ON := mf.ON,
    OA_C := mf.OA_C,
    OD_C := mf.OD_C,
    ON_Num := mf.ON_Num,
    ON_P := mf.ON_P,
    mf.OPP = false,
    setscore -3.78418963391826

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    MF_OA := mf.OA,
    MF_OD := mf.OD,
    MF_ON := mf.ON,
    OA_C := mf.OA_C,
    OD_C := mf.OD_C,
    mf.OPP = false,
    setscore -3.78418963391826

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    MF_OA := mf.OA,
    MF_OD := mf.OD,
    OA_C := mf.OA_C,
    OD_C := mf.OD_C,
    mf.ON = false,
    mf.OPP = false,
    setscore -3.78418963391826

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    MF_OA := mf.OA,
    MF_OD := mf.OD,
    OA_C := mf.OA_C,
    OD_C := mf.OD_C,
    mf.OPP = false,
    setscore -3.78418963391826

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    MF_OA := mf.OA,
    MF_ON := mf.ON,
    OA_C := mf.OA_C,
    ON_Num := mf.ON_Num,
    ON_P := mf.ON_P,
    OPP_C := mf.OPP_C,
    OPP_Przyim := mf.OPP_Przyim,
    mf.OPP = true,
    setscore -3.78418963391826

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
    setscore -3.78418963391826

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    MF_OA := mf.OA,
    MF_ON := mf.ON,
    OA_C := mf.OA_C,
    ON_Num := mf.ON_Num,
    ON_P := mf.ON_P,
    mf.OPP = false,
    setscore -3.09104245335832

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    MF_OA := mf.OA,
    MF_ON := mf.ON,
    OA_C := mf.OA_C,
    ON_Num := mf.ON_Num,
    ON_P := mf.ON_P,
    mf.OPP = false,
    mf.PRED = true,
    setscore -3.78418963391826

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    MF_OA := mf.OA,
    MF_ON := mf.ON,
    OA_C := mf.OA_C,
    ON_Num := mf.ON_Num,
    mf.OPP = false,
    mf.PRED = true,
    setscore -3.78418963391826

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    MF_OA := mf.OA,
    MF_ON := mf.ON,
    OA_C := mf.OA_C,
    mf.OPP = false,
    mf.PRED = true,
    setscore -3.78418963391826

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    MF_OA := mf.OA,
    OA_C := mf.OA_C,
    mf.ON = false,
    mf.OPP = false,
    setscore -3.78418963391826

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    MF_OA := mf.OA,
    OA_C := mf.OA_C,
    mf.OPP = false,
    setscore -3.09104245335832

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    MF_OA := mf.OA,
    OA_C := mf.OA_C,
    mf.OPP = false,
    mf.PRED = true,
    setscore -3.78418963391826

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    MF_OD := mf.OD,
    MF_ON := mf.ON,
    OD_C := mf.OD_C,
    ON_Num := mf.ON_Num,
    ON_P := mf.ON_P,
    OPP_C := mf.OPP_C,
    OPP_Przyim := mf.OPP_Przyim,
    mf.OPP = true,
    setscore -3.78418963391826

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    MF_OD := mf.OD,
    MF_ON := mf.ON,
    OD_C := mf.OD_C,
    ON_Num := mf.ON_Num,
    ON_P := mf.ON_P,
    mf.OPP = false,
    setscore -3.78418963391826

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    MF_OD := mf.OD,
    MF_ON := mf.ON,
    OD_C := mf.OD_C,
    ON_Num := mf.ON_Num,
    ON_P := mf.ON_P,
    mf.OPP = false,
    mf.PRED = true,
    setscore -3.78418963391826

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    MF_OD := mf.OD,
    OD_C := mf.OD_C,
    mf.OPP = false,
    setscore -3.78418963391826

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    MF_ON := mf.ON,
    ON_Num := mf.ON_Num,
    ON_P := mf.ON_P,
    OPP_C := mf.OPP_C,
    OPP_Przyim := mf.OPP_Przyim,
    mf.OPP = true,
    setscore -2.68557734525015

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    MF_ON := mf.ON,
    ON_Num := mf.ON_Num,
    ON_P := mf.ON_P,
    OPP_C := mf.OPP_C,
    OPP_Przyim := mf.OPP_Przyim,
    mf.OPP = true,
    mf.PRED = true,
    setscore -3.78418963391826

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    MF_ON := mf.ON,
    ON_Num := mf.ON_Num,
    ON_P := mf.ON_P,
    OPP_C := mf.OPP_C,
    mf.OPP = true,
    setscore -3.78418963391826

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    MF_ON := mf.ON,
    ON_Num := mf.ON_Num,
    ON_P := mf.ON_P,
    mf.OPP = false,
    setscore -2.17475172148416

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    MF_ON := mf.ON,
    ON_Num := mf.ON_Num,
    ON_P := mf.ON_P,
    mf.OPP = false,
    mf.PRED = true,
    setscore -2.17475172148416

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    MF_ON := mf.ON,
    OPP_C := mf.OPP_C,
    OPP_Przyim := mf.OPP_Przyim,
    mf.OPP = true,
    setscore -3.78418963391826

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    OPP_C := mf.OPP_C,
    OPP_Przyim := mf.OPP_Przyim,
    mf.ON = false,
    mf.OPP = true,
    setscore -3.78418963391826

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    OPP_C := mf.OPP_C,
    OPP_Przyim := mf.OPP_Przyim,
    mf.OPP = true,
    setscore -3.78418963391826

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    mf.ON = false,
    mf.OPP = false,
    setscore -3.78418963391826

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    mf.ON = false,
    mf.OPP = false,
    mf.PRED = true,
    setscore -3.78418963391826

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    mf.OPP = false,
    setscore -3.09104245335832

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    mf.OPP = false,
    mf.PRED = true,
    setscore -3.78418963391826

simpx-c--mf = simpx-c* mf %simpx-c[+mf]%
    setscore -3.09104245335832

simpx-c--nf = simpx-c--comma* nf %simpx-c--comma[+nf]%
    nf.ON = false,
    nf.OPP = false,
    setscore -0.693147180559945

simpx-c--nf = simpx-c--comma* nf %simpx-c--comma[+nf]%
    nf.OPP = false,
    setscore -1.02961941718116

simpx-c--nf = simpx-c--vc* nf %simpx-c--vc[+nf]%
    nf.ON = false,
    nf.OPP = false,
    setscore -1.94591014905531

simpx-c--vc = simpx-c* vc %simpx-c[+vc]%
    vc.Num == ON_Num,
    vc.P == ON_P,
    setscore -3.80666248977032

simpx-c--vc = simpx-c* vc %simpx-c[+vc]%
    setscore -3.11351530921037

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
    vc.Dop1 == OA_C,
    vc.Dop2 == OD_C,
    vc.Num == ON_Num,
    vc.P == ON_P,
    vc.Przyim1 == zero,
    vc.Przyim2 == zero,
    setscore -3.80666248977032

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
    vc.Dop1 == OA_C,
    vc.Dop2 == OD_C,
    vc.Przyim1 == zero,
    vc.Przyim2 == zero,
    setscore -2.70805020110221

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
    vc.Dop1 == OA_C,
    vc.Dop2 == OPP_C,
    vc.Num == ON_Num,
    vc.P == ON_P,
    vc.Przyim1 == zero,
    vc.Przyim2 == OPP_Przyim,
    setscore -3.11351530921037

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
    vc.Dop1 == OA_C,
    vc.Num == ON_Num,
    vc.P == ON_P,
    vc.Przyim1 == zero,
    setscore -2.19722457733622

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
    vc.Dop1 == OA_C,
    vc.Num == ON_Num,
    vc.Przyim1 == zero,
    setscore -3.80666248977032

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
    vc.Dop1 == OA_C,
    vc.Przyim1 == zero,
    setscore -2.42036812865043

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
    vc.Dop1 == OD_C,
    vc.Dop2 == OPP_C,
    vc.Num == ON_Num,
    vc.P == ON_P,
    vc.Przyim1 == zero,
    vc.Przyim2 == OPP_Przyim,
    setscore -3.80666248977032

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
    vc.Dop1 == OD_C,
    vc.Num == ON_Num,
    vc.P == ON_P,
    vc.Przyim1 == zero,
    setscore -3.11351530921037

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
    vc.Dop1 == OD_C,
    vc.Przyim1 == zero,
    setscore -3.80666248977032

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
    vc.Dop1 == OPP_C,
    vc.Num == ON_Num,
    vc.P == ON_P,
    setscore -3.80666248977032

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
    vc.Dop1 == OPP_C,
    vc.Num == ON_Num,
    vc.P == ON_P,
    vc.Przyim1 == OPP_Przyim,
    setscore -2.42036812865043

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
    vc.Dop1 == OPP_C,
    vc.Przyim1 == OPP_Przyim,
    setscore -2.70805020110221

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
    vc.Num == ON_Num,
    vc.P == ON_P,
    setscore -1.50407739677627

simpx-c--vc = simpx-c--mf* vc %simpx-c--mf[+vc]%
    setscore -2.42036812865043

simpx-koord = koord* %SIMPX[koord]%
    Block := 1,
    setscore 0

simpx-koord--comma = simpx-koord--vf* comma %simpx-koord--vf[+comma]%
    setscore 0

simpx-koord--fkoord = simpx-koord--comma* fkoord %simpx-koord--comma[+fkoord]%
    setscore -0.693147180559945

simpx-koord--fkoord = simpx-koord--vf* fkoord %simpx-koord--vf[+fkoord]%
    setscore -0.693147180559945

simpx-koord--mf = simpx-koord* mf %simpx-koord[+mf]%
    OPP_C := mf.OPP_C,
    OPP_Przyim := mf.OPP_Przyim,
    mf.OPP = true,
    setscore 0

simpx-koord--vc = simpx-koord--mf* vc %simpx-koord--mf[+vc]%
    vc.Dop1 == OPP_C,
    vc.Przyim1 == OPP_Przyim,
    setscore 0

simpx-koord--vf = simpx-koord* vf %simpx-koord[+vf]%
    setscore 0

simpx-lk = lk* %SIMPX[lk]%
    Block := 1,
    setscore -0.270123760238402

simpx-lk = lk* %SIMPX[lk]%
    Block := 1,
    Przyim1 == zero,
    setscore -1.48257119155531

simpx-lk = lk* %SIMPX[lk]%
    Block := 1,
    Przyim1 == zero,
    Przyim2 == zero,
    setscore -4.63957161270542

simpx-lk--comma = comma simpx-lk* %simpx-lk[comma+]%
    setscore -1.0430424375133

simpx-lk--comma = comma simpx-lk--vf* %simpx-lk--vf[comma+]%
    setscore -3.55534806148941

simpx-lk--comma = simpx-lk--mf* comma %simpx-lk--mf[+comma]%
    setscore -1.12759982554136

simpx-lk--comma = simpx-lk--vc* comma %simpx-lk--vc[+comma]%
    setscore -1.35812348415319

simpx-lk--comma = simpx-lk--vf* comma %simpx-lk--vf[+comma]%
    setscore -3.26766598903763

simpx-lk--fkoord = simpx-lk--koord* fkoord %simpx-lk--koord[+fkoord]%
    setscore -1.38629436111989

simpx-lk--fkoord = simpx-lk--vf* fkoord %simpx-lk--vf[+fkoord]%
    setscore -0.287682072451781

simpx-lk--koord = koord simpx-lk* %simpx-lk[koord+]%
    setscore -2.39789527279837

simpx-lk--koord = koord simpx-lk--vf* %simpx-lk--vf[koord+]%
    setscore -0.0953101798043249

simpx-lk--lv = lv simpx-lk--comma* %simpx-lk--comma[lv+]%
    setscore 0

simpx-lk--mf = mf simpx-lk* %simpx-lk[mf+]%
    mf.ON = false,
    mf.OPP = false,
    mf.PRED = true,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
    Dop1 == mf.OA_C,
    Dop2 == mf.OD_C,
    MF_OA := mf.OA,
    MF_OD := mf.OD,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
    Dop1 == mf.OA_C,
    MF_OA := mf.OA,
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    P == mf.ON_P,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
    Dop1 == mf.OD_C,
    MF_OD := mf.OD,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
    MF_OA := mf.OA,
    MF_OD := mf.OD,
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    OA_C := mf.OA_C,
    OD_C := mf.OD_C,
    P == mf.ON_P,
    mf.OPP = false,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
    MF_OA := mf.OA,
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    OA_C := mf.OA_C,
    P == mf.ON_P,
    mf.OPP = false,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    P == mf.ON_P,
    setscore -4.54329478227

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    P == mf.ON_P,
    mf.OPP = false,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
    mf.OPP = false,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
    mf.OPP = true,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
    mf.PRED = true,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk* mf %simpx-lk[+mf]%
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
    Dop1 == mf.OA_C,
    Dop2 == mf.OPP_C,
    MF_OA := mf.OA,
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    P == mf.ON_P,
    Przyim2 == mf.OPP_Przyim,
    mf.OPP = true,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
    Dop1 == mf.OA_C,
    MF_OA := mf.OA,
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    P == mf.ON_P,
    mf.OPP = false,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
    Dop1 == mf.OA_C,
    MF_OA := mf.OA,
    mf.ON = false,
    mf.OPP = false,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
    Dop1 == mf.OA_C,
    MF_OA := mf.OA,
    mf.OPP = false,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
    Dop1 == mf.OPP_C,
    Przyim1 == mf.OPP_Przyim,
    mf.ON = false,
    mf.OPP = true,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
    MF_OA := mf.OA,
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    OA_C := mf.OA_C,
    OPP_C := mf.OPP_C,
    OPP_Przyim := mf.OPP_Przyim,
    P == mf.ON_P,
    mf.OPP = true,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
    MF_OA := mf.OA,
    OA_C := mf.OA_C,
    mf.ON = false,
    mf.OPP = false,
    setscore -4.54329478227

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
    MF_OD := mf.OD,
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    OD_C := mf.OD_C,
    P == mf.ON_P,
    mf.OPP = false,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    P == mf.ON_P,
    mf.OPP = false,
    setscore -4.54329478227

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    P == mf.ON_P,
    mf.OPP = false,
    mf.PRED = true,
    setscore -4.13782967416184

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
    OPP_C := mf.OPP_C,
    OPP_Przyim := mf.OPP_Przyim,
    mf.ON = false,
    mf.OPP = true,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
    mf.ON = false,
    mf.OPP = false,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--koord* mf %simpx-lk--koord[+mf]%
    mf.ON = false,
    mf.OPP = false,
    mf.PRED = true,
    setscore -4.13782967416184

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    P == mf.ON_P,
    mf.OPP = false,
    setscore -4.54329478227

simpx-lk--mf = simpx-lk--lv* mf %simpx-lk--lv[+mf]%
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    P == mf.ON_P,
    mf.OPP = false,
    mf.PRED = true,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--nf* mf %simpx-lk--nf[+mf]%
    mf.ON = false,
    mf.OPP = false,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
    Dop1 == mf.OA_C,
    MF_OA := mf.OA,
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    P == mf.ON_P,
    mf.OPP = false,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    P == mf.ON_P,
    mf.OPP = false,
    setscore -3.85014760171006

simpx-lk--mf = simpx-lk--parord* mf %simpx-lk--parord[+mf]%
    mf.ON = false,
    mf.OPP = true,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    Dop1 == mf.OA_C,
    Dop2 == mf.OD_C,
    MF_OA := mf.OA,
    MF_OD := mf.OD,
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    P == mf.ON_P,
    mf.OPP = false,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    Dop1 == mf.OA_C,
    Dop2 == mf.OPP_C,
    MF_OA := mf.OA,
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    P == mf.ON_P,
    Przyim2 == mf.OPP_Przyim,
    mf.OPP = true,
    setscore -4.54329478227

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    Dop1 == mf.OA_C,
    Dop2 == mf.OPP_C,
    MF_OA := mf.OA,
    Przyim2 == mf.OPP_Przyim,
    mf.ON = false,
    mf.OPP = true,
    setscore -4.13782967416184

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    Dop1 == mf.OA_C,
    Dop2 == mf.OPP_C,
    MF_OA := mf.OA,
    Przyim2 == mf.OPP_Przyim,
    mf.ON = false,
    mf.OPP = true,
    mf.PRED = true,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    Dop1 == mf.OA_C,
    MF_OA := mf.OA,
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    P == mf.ON_P,
    Przyim2 == mf.OPP_Przyim,
    mf.OPP = true,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    Dop1 == mf.OA_C,
    MF_OA := mf.OA,
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    P == mf.ON_P,
    mf.OPP = false,
    setscore -3.44468249360189

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    Dop1 == mf.OA_C,
    MF_OA := mf.OA,
    mf.ON = false,
    mf.OPP = false,
    setscore -3.29053181377464

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    Dop1 == mf.OA_C,
    MF_OA := mf.OA,
    mf.ON = false,
    mf.OPP = false,
    mf.PRED = true,
    setscore -4.54329478227

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    Dop1 == mf.OA_C,
    MF_OA := mf.OA,
    mf.ON = false,
    mf.OPP = true,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    Dop1 == mf.OA_C,
    MF_OA := mf.OA,
    mf.OPP = false,
    setscore -4.54329478227

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    Dop1 == mf.OA_C,
    MF_OA := mf.OA,
    mf.OPP = false,
    mf.PRED = true,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    Dop1 == mf.OD_C,
    Dop2 == mf.OPP_C,
    MF_OD := mf.OD,
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    P == mf.ON_P,
    Przyim2 == mf.OPP_Przyim,
    mf.OPP = true,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    Dop1 == mf.OD_C,
    MF_OD := mf.OD,
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    P == mf.ON_P,
    mf.OPP = false,
    setscore -4.54329478227

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    Dop1 == mf.OD_C,
    MF_OD := mf.OD,
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    P == mf.ON_P,
    mf.OPP = true,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    Dop1 == mf.OD_C,
    MF_OD := mf.OD,
    mf.ON = false,
    mf.OPP = false,
    setscore -4.54329478227

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    Dop1 == mf.OD_C,
    MF_OD := mf.OD,
    mf.ON = false,
    mf.OPP = false,
    mf.PRED = true,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    Dop1 == mf.OPP_C,
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    P == mf.ON_P,
    Przyim1 == mf.OPP_Przyim,
    mf.OPP = true,
    setscore -4.13782967416184

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    Dop1 == mf.OPP_C,
    Przyim1 == mf.OPP_Przyim,
    mf.ON = false,
    mf.OPP = true,
    setscore -4.13782967416184

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    Dop1 == mf.OPP_C,
    Przyim1 == mf.OPP_Przyim,
    mf.OPP = true,
    setscore -4.54329478227

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    Dop1 == mf.OPP_C,
    Przyim1 == mf.OPP_Przyim,
    mf.OPP = true,
    mf.PRED = true,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    MF_OA := mf.OA,
    MF_OD := mf.OD,
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    OA_C := mf.OA_C,
    OD_C := mf.OD_C,
    P == mf.ON_P,
    mf.OPP = false,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    MF_OA := mf.OA,
    MF_OD := mf.OD,
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    OA_C := mf.OA_C,
    OD_C := mf.OD_C,
    P == mf.ON_P,
    mf.OPP = true,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    MF_OA := mf.OA,
    MF_OD := mf.OD,
    OA_C := mf.OA_C,
    OD_C := mf.OD_C,
    mf.ON = false,
    mf.OPP = true,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    MF_OA := mf.OA,
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    OA_C := mf.OA_C,
    OPP_C := mf.OPP_C,
    OPP_Przyim := mf.OPP_Przyim,
    P == mf.ON_P,
    mf.OPP = true,
    setscore -4.13782967416184

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    MF_OA := mf.OA,
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    OA_C := mf.OA_C,
    P == mf.ON_P,
    mf.OPP = false,
    setscore -3.29053181377464

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    MF_OA := mf.OA,
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    OA_C := mf.OA_C,
    P == mf.ON_P,
    mf.OPP = false,
    mf.PRED = true,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    MF_OA := mf.OA,
    OA_C := mf.OA_C,
    OPP_C := mf.OPP_C,
    OPP_Przyim := mf.OPP_Przyim,
    mf.ON = false,
    mf.OPP = true,
    setscore -4.54329478227

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    MF_OA := mf.OA,
    OA_C := mf.OA_C,
    OPP_C := mf.OPP_C,
    OPP_Przyim := mf.OPP_Przyim,
    mf.ON = false,
    mf.OPP = true,
    mf.PRED = true,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    MF_OA := mf.OA,
    OA_C := mf.OA_C,
    mf.ON = false,
    mf.OPP = false,
    setscore -4.13782967416184

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    MF_OA := mf.OA,
    OA_C := mf.OA_C,
    mf.ON = false,
    mf.OPP = false,
    mf.PRED = true,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    MF_OA := mf.OA,
    OA_C := mf.OA_C,
    mf.ON = false,
    mf.OPP = true,
    setscore -4.54329478227

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    MF_OA := mf.OA,
    OA_C := mf.OA_C,
    mf.OPP = false,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    MF_OD := mf.OD,
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    OD_C := mf.OD_C,
    P == mf.ON_P,
    mf.OPP = false,
    setscore -4.54329478227

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    MF_OD := mf.OD,
    OD_C := mf.OD_C,
    mf.ON = false,
    mf.OPP = false,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    OPP_C := mf.OPP_C,
    OPP_Przyim := mf.OPP_Przyim,
    P == mf.ON_P,
    mf.OPP = true,
    setscore -3.85014760171006

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    P == mf.ON_P,
    mf.OPP = false,
    setscore -2.058388132482

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    P == mf.ON_P,
    mf.OPP = false,
    mf.PRED = true,
    setscore -3.15700042115011

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    MF_ON := mf.ON,
    Num == mf.ON_Num,
    P == mf.ON_P,
    mf.OPP = true,
    setscore -4.54329478227

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    MF_ON := mf.ON,
    P == mf.ON_P,
    mf.OPP = false,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    MF_ON := mf.ON,
    mf.OPP = false,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    MF_ON := mf.ON,
    mf.OPP = false,
    mf.PRED = true,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    OPP_C := mf.OPP_C,
    OPP_Przyim := mf.OPP_Przyim,
    mf.ON = false,
    mf.OPP = true,
    setscore -4.13782967416184

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    OPP_C := mf.OPP_C,
    OPP_Przyim := mf.OPP_Przyim,
    mf.OPP = true,
    setscore -4.13782967416184

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    Przyim1 == mf.OPP_Przyim,
    mf.ON = false,
    mf.OPP = true,
    setscore -5.23644196282995

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    mf.ON = false,
    mf.OPP = false,
    setscore -3.15700042115011

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    mf.ON = false,
    mf.OPP = false,
    mf.PRED = true,
    setscore -3.29053181377464

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    mf.ON = false,
    mf.OPP = true,
    setscore -4.54329478227

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    mf.OPP = false,
    setscore -3.44468249360189

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    mf.OPP = false,
    mf.PRED = true,
    setscore -3.85014760171006

simpx-lk--mf = simpx-lk--vf* mf %simpx-lk--vf[+mf]%
    mf.OPP = true,
    setscore -5.23644196282995

simpx-lk--nf = nf simpx-lk--vf* %simpx-lk--vf[nf+]%
    nf.ON = false,
    nf.OPP = false,
    setscore -4.4188406077966

simpx-lk--nf = simpx-lk--comma* nf %simpx-lk--comma[+nf]%
    nf.ON = false,
    setscore -4.4188406077966

simpx-lk--nf = simpx-lk--comma* nf %simpx-lk--comma[+nf]%
    nf.ON = false,
    nf.OPP = false,
    setscore -0.590199211307503

simpx-lk--nf = simpx-lk--comma* nf %simpx-lk--comma[+nf]%
    nf.OPP = false,
    setscore -2.02094533499823

simpx-lk--nf = simpx-lk--comma* nf %simpx-lk--comma[+nf]%
    setscore -3.72569342723665

simpx-lk--nf = simpx-lk--mf* nf %simpx-lk--mf[+nf]%
    nf.ON = false,
    nf.OPP = false,
    setscore -2.47293045874128

simpx-lk--nf = simpx-lk--mf* nf %simpx-lk--mf[+nf]%
    nf.ON = false,
    nf.OPP = true,
    setscore -4.4188406077966

simpx-lk--nf = simpx-lk--mf* nf %simpx-lk--mf[+nf]%
    nf.OPP = false,
    setscore -4.4188406077966

simpx-lk--nf = simpx-lk--vc* nf %simpx-lk--vc[+nf]%
    NF_ON := nf.ON,
    nf.OPP = false,
    setscore -4.4188406077966

simpx-lk--nf = simpx-lk--vc* nf %simpx-lk--vc[+nf]%
    nf.ON = false,
    nf.OPP = false,
    setscore -2.33939906611676

simpx-lk--nf = simpx-lk--vc* nf %simpx-lk--vc[+nf]%
    nf.ON = false,
    nf.OPP = true,
    setscore -4.4188406077966

simpx-lk--nf = simpx-lk--vc* nf %simpx-lk--vc[+nf]%
    nf.OPP = false,
    setscore -3.72569342723665

simpx-lk--nf = simpx-lk--vf* nf %simpx-lk--vf[+nf]%
    nf.ON = false,
    nf.OPP = false,
    setscore -4.4188406077966

simpx-lk--parord = parord simpx-lk--vf* %simpx-lk--vf[parord+]%
    setscore 0

simpx-lk--vc = simpx-lk--comma* vc %simpx-lk--comma[+vc]%
    Dop3 == vc.Typ,
    setscore -4.54329478227

simpx-lk--vc = simpx-lk--comma* vc %simpx-lk--comma[+vc]%
    Dop3 == vc.Typ,
    vc.Dop1 == OA_C,
    vc.Przyim1 == zero,
    setscore -4.54329478227

simpx-lk--vc = simpx-lk--comma* vc %simpx-lk--comma[+vc]%
    Dop3 == vc.Typ,
    vc.Dop1 == OPP_C,
    vc.Przyim1 == OPP_Przyim,
    setscore -4.54329478227

simpx-lk--vc = simpx-lk--comma* vc %simpx-lk--comma[+vc]%
    setscore -4.54329478227

simpx-lk--vc = simpx-lk--koord* vc %simpx-lk--koord[+vc]%
    Dop3 == vc.VPT,
    setscore -4.54329478227

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
    Dop3 == vc.Typ,
    setscore -1.59885580310356

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
    Dop3 == vc.Typ,
    vc.Dop1 == OA_C,
    vc.Dop2 == OD_C,
    vc.Przyim1 == zero,
    vc.Przyim2 == zero,
    setscore -2.9338568698359

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
    Dop3 == vc.Typ,
    vc.Dop1 == OA_C,
    vc.Dop2 == OPP_C,
    vc.Przyim1 == zero,
    vc.Przyim2 == OPP_Przyim,
    setscore -2.46385324059017

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
    Dop3 == vc.Typ,
    vc.Dop1 == OA_C,
    vc.Przyim1 == zero,
    setscore -1.54756250871601

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
    Dop3 == vc.Typ,
    vc.Dop1 == OD_C,
    vc.Przyim1 == zero,
    setscore -3.15700042115011

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
    Dop3 == vc.Typ,
    vc.Dop1 == OPP_C,
    vc.Przyim1 == OPP_Przyim,
    setscore -2.24070968927596

simpx-lk--vc = simpx-lk--mf* vc %simpx-lk--mf[+vc]%
    Dop3 == vc.VPT,
    setscore -1.71008143821379

simpx-lk--vc = simpx-lk--vf* vc %simpx-lk--vf[+vc]%
    Dop3 == vc.Typ,
    setscore -3.44468249360189

simpx-lk--vc = simpx-lk--vf* vc %simpx-lk--vf[+vc]%
    Dop3 == vc.VPT,
    setscore -3.85014760171006

simpx-lk--vc = simpx-lk--vf* vc %simpx-lk--vf[+vc]%
    setscore -4.54329478227

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
    Dop1 == vf.OA_C,
    VF_OA := vf.OA,
    vf.ON = false,
    vf.OPP = false,
    setscore -3.65325227647079

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
    Dop1 == vf.OPP_C,
    Przyim1 == vf.OPP_Przyim,
    vf.ON = false,
    vf.OPP = true,
    setscore -5.26269018890489

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
    Dop1 == vf.OPP_C,
    Przyim1 == vf.OPP_Przyim,
    vf.OPP = true,
    setscore -5.26269018890489

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
    Dop2 == vf.OPP_C,
    Przyim2 == vf.OPP_Przyim,
    vf.ON = false,
    vf.OPP = true,
    setscore -5.26269018890489

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
    Num == vf.ON_Num,
    P == vf.ON_P,
    VF_ON := vf.ON,
    vf.OPP = false,
    setscore -1.13555580385979

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
    Num == vf.ON_Num,
    VF_ON := vf.ON,
    vf.OPP = false,
    setscore -5.26269018890489

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
    OA_C := vf.OA_C,
    VF_OA := vf.OA,
    vf.ON = false,
    vf.OPP = false,
    setscore -3.65325227647079

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
    OD_C := vf.OD_C,
    VF_OD := vf.OD,
    vf.ON = false,
    vf.OPP = false,
    setscore -5.26269018890489

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
    OPP_C := vf.OPP_C,
    OPP_Przyim := vf.OPP_Przyim,
    vf.ON = false,
    vf.OPP = true,
    setscore -5.26269018890489

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
    VF_ON := vf.ON,
    vf.OPP = false,
    setscore -4.16407790023678

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
    vf.ON = false,
    vf.OPP = false,
    setscore -1.25535700367241

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
    vf.ON = false,
    vf.OPP = false,
    vf.PRED = true,
    setscore -4.56954300834494

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
    vf.ON = false,
    vf.OPP = true,
    setscore -3.87639582778499

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
    vf.OPP = false,
    setscore -2.77778353911689

simpx-lk--vf = vf simpx-lk* %simpx-lk[vf+]%
    vf.OPP = false,
    vf.PRED = true,
    setscore -5.26269018890489

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
    Dop1 == vf.OA_C,
    VF_OA := vf.OA,
    vf.ON = false,
    vf.OPP = false,
    setscore -5.26269018890489

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
    Num == vf.ON_Num,
    P == vf.ON_P,
    VF_ON := vf.ON,
    vf.OPP = false,
    setscore -2.96010509591084

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
    vf.ON = false,
    vf.OPP = false,
    setscore -2.62363285928963

simpx-lk--vf = vf simpx-lk--comma* %simpx-lk--comma[vf+]%
    vf.OPP = false,
    setscore -2.77778353911689

simpx-lk--vf = vf simpx-lk--mf* %simpx-lk--mf[vf+]%
    Num == vf.ON_Num,
    P == vf.ON_P,
    VF_ON := vf.ON,
    vf.OPP = false,
    setscore -5.26269018890489

simpx-mf = mf* %SIMPX[mf:konj]%
    Block := 1,
    setscore -2.39789527279837

simpx-mf = mf* %SIMPX[mf]%
    Block := 1,
    MF_OA := OA,
    OPP = false,
    setscore -2.39789527279837

simpx-mf = mf* %SIMPX[mf]%
    Block := 1,
    MF_OA := OA,
    OPP = false,
    PRED = true,
    setscore -2.39789527279837

simpx-mf = mf* %SIMPX[mf]%
    Block := 1,
    MF_OA := OA,
    OPP = true,
    setscore -2.39789527279837

simpx-mf = mf* %SIMPX[mf]%
    Block := 1,
    MF_OD := OD,
    OPP = false,
    setscore -1.70474809223843

simpx-mf = mf* %SIMPX[mf]%
    Block := 1,
    MF_ON := ON,
    OPP = false,
    setscore -1.70474809223843

simpx-mf = mf* %SIMPX[mf]%
    Block := 1,
    OPP = false,
    setscore -1.70474809223843

simpx-mf = mf* %SIMPX[mf]%
    Block := 1,
    OPP = true,
    setscore -2.39789527279837

simpx-mf--c = simpx-mf* c %simpx-mf[+c]%
    C_OA := c.OA,
    OA_C := c.OA_C,
    c.ON = false,
    c.OPP = false,
    setscore 0

simpx-mf--comma = simpx-mf* comma %simpx-mf[+comma]%
    setscore -1.09861228866811

simpx-mf--comma = simpx-mf--vc* comma %simpx-mf--vc[+comma]%
    setscore -0.405465108108164

simpx-mf--mf = simpx-mf--comma* mf %simpx-mf--comma[+mf:konj]%
    setscore 0

simpx-mf--nf = simpx-mf--comma* nf %simpx-mf--comma[+nf]%
    nf.OPP = false,
    setscore 0

simpx-mf--vc = simpx-mf* vc %simpx-mf[+vc]%
    vc.Dop1 == OA_C,
    vc.Dop2 == OPP_C,
    vc.Przyim1 == zero,
    vc.Przyim2 == OPP_Przyim,
    setscore -2.30258509299405

simpx-mf--vc = simpx-mf* vc %simpx-mf[+vc]%
    vc.Dop1 == OA_C,
    vc.Przyim1 == zero,
    setscore -1.6094379124341

simpx-mf--vc = simpx-mf* vc %simpx-mf[+vc]%
    vc.Dop1 == OD_C,
    vc.Przyim1 == zero,
    setscore -1.6094379124341

simpx-mf--vc = simpx-mf* vc %simpx-mf[+vc]%
    vc.Dop1 == OPP_C,
    vc.Przyim1 == OPP_Przyim,
    setscore -2.30258509299405

simpx-mf--vc = simpx-mf* vc %simpx-mf[+vc]%
    setscore -1.20397280432594

simpx-mf--vc = simpx-mf--c* vc %simpx-mf--c[+vc]%
    vc.Dop1 == OA_C,
    vc.Num == ON_Num,
    vc.P == ON_P,
    vc.Przyim1 == zero,
    setscore -2.30258509299405

simpx-simpx = simpx* %SIMPX[simpx:konj]%
    Block := 1,
    setscore 0

simpx-simpx--comma = simpx-simpx* comma %simpx-simpx[+comma]%
    setscore 0

simpx-simpx--conj = simpx-simpx* conj %simpx-simpx[+conj]%
    setscore -0.693147180559945

simpx-simpx--conj = simpx-simpx--comma* conj %simpx-simpx--comma[+conj]%
    setscore -0.693147180559945

simpx-simpx--simpx = simpx-simpx* simpx %simpx-simpx[+simpx:konj]%
    setscore -1.38629436111989

simpx-simpx--simpx = simpx-simpx--comma* simpx %simpx-simpx--comma[+simpx:konj]%
    setscore -1.38629436111989

simpx-simpx--simpx = simpx-simpx--conj* simpx %simpx-simpx--conj[+simpx:konj]%
    setscore -0.693147180559945

simpx-vc = vc* %SIMPX[vc]%
    Block := 1,
    setscore 0

simpx-vf = vf* %SIMPX[vf]%
    Block := 1,
    setscore 0

simpx-vf--comma = simpx-vf--mf* comma %simpx-vf--mf[+comma]%
    setscore 0

simpx-vf--fkoord = simpx-vf* fkoord %simpx-vf[+fkoord]%
    setscore 0

simpx-vf--mf = simpx-vf* mf %simpx-vf[+mf]%
    setscore 0

simpx-vf--nf = simpx-vf--comma* nf %simpx-vf--comma[+nf]%
    setscore 0

vc = vc-part*
    setscore -3.93182563272433

vc = vc-vp*
    setscore -0.84078317936601

vc = vc-vp--vp*
    setscore -0.599621122549122

vc-part = part* %VC[part:vpt]%
    Block := 1,
    T == prefix,
    VPT := Core,
    setscore 0

vc-vp = vp* %VC[vp:compl3]%
    Block := 1,
    setscore -3.91202300542815

vc-vp = vp* %VC[vp:compl3]%
    Block := 1,
    Typ == inf,
    setscore -2.30258509299405

vc-vp = vp* %VC[vp:compl3]%
    Block := 1,
    Typ == part,
    setscore -2.52572864430826

vc-vp = vp* %VC[vp:compl3]%
    Block := 1,
    Typ == zuinf,
    setscore -3.91202300542815

vc-vp = vp* %VC[vp:ov]%
    Block := 1,
    Typ == inf,
    setscore -3.2188758248682

vc-vp = vp* %VC[vp:ov]%
    Block := 1,
    Typ == part,
    setscore -3.91202300542815

vc-vp = vp* %VC[vp]%
    Block := 1,
    setscore -3.91202300542815

vc-vp = vp* %VC[vp]%
    Block := 1,
    Typ == cond_past,
    setscore -2.30258509299405

vc-vp = vp* %VC[vp]%
    Block := 1,
    Typ == cond_pres,
    setscore -2.52572864430826

vc-vp = vp* %VC[vp]%
    Block := 1,
    Typ == indic_past,
    setscore -2.30258509299405

vc-vp = vp* %VC[vp]%
    Block := 1,
    Typ == indic_pres,
    setscore -1.51412773262978

vc-vp = vp* %VC[vp]%
    Block := 1,
    Typ == inf,
    setscore -1.96611285637283

vc-vp = vp* %VC[vp]%
    Block := 1,
    Typ == part,
    setscore -3.2188758248682

vc-vp = vp* %VC[vp]%
    Block := 1,
    Typ == zuinf,
    setscore -3.91202300542815

vc-vp--vp = vp vc-vp* %vc-vp[vp:compl3+]%
    Dop1 := vp.Dop1,
    Dop2 := vp.Dop2,
    Przyim1 := vp.Przyim1,
    Przyim2 := vp.Przyim2,
    setscore -3.3322045101752

vc-vp--vp = vp vc-vp* %vc-vp[vp:compl3+]%
    Dop1 := vp.Dop1,
    Dop2 := vp.Dop2,
    Przyim1 := vp.Przyim1,
    Przyim2 := vp.Przyim2,
    Przyim3 := vp.Przyim3,
    Typ := vp.Typ,
    vp.Typ == inf,
    setscore -3.3322045101752

vc-vp--vp = vp vc-vp* %vc-vp[vp:compl3+]%
    Dop1 := vp.Dop1,
    Dop2 := vp.Dop2,
    Przyim1 := vp.Przyim1,
    Przyim2 := vp.Przyim2,
    Typ := vp.Typ,
    vp.Typ == inf,
    setscore -3.3322045101752

vc-vp--vp = vp vc-vp* %vc-vp[vp:compl3+]%
    Dop1 := vp.Dop1,
    Dop2 := vp.Dop2,
    Przyim1 := vp.Przyim1,
    Przyim2 := vp.Przyim2,
    vp.Typ == inf,
    setscore -2.23359222150709

vc-vp--vp = vp vc-vp* %vc-vp[vp:compl3+]%
    Dop1 := vp.Dop1,
    Dop2 := vp.Dop2,
    Przyim1 := vp.Przyim1,
    Przyim2 := vp.Przyim2,
    vp.Typ == part,
    setscore -3.3322045101752

vc-vp--vp = vp vc-vp* %vc-vp[vp:compl3+]%
    Dop1 := vp.Dop1,
    Przyim1 := vp.Przyim1,
    To := vp.To,
    vp.Typ == inf,
    setscore -3.3322045101752

vc-vp--vp = vp vc-vp* %vc-vp[vp:compl3+]%
    Dop1 := vp.Dop1,
    Przyim1 := vp.Przyim1,
    Typ := vp.Typ,
    vp.Typ == inf,
    setscore -3.3322045101752

vc-vp--vp = vp vc-vp* %vc-vp[vp:compl3+]%
    Dop1 := vp.Dop1,
    Przyim1 := vp.Przyim1,
    Typ := vp.Typ,
    vp.Typ == part,
    setscore -3.3322045101752

vc-vp--vp = vp vc-vp* %vc-vp[vp:compl3+]%
    Dop1 := vp.Dop1,
    Przyim1 := vp.Przyim1,
    vp.Typ == inf,
    setscore -1.94591014905531

vc-vp--vp = vp vc-vp* %vc-vp[vp:compl3+]%
    Dop1 := vp.Dop1,
    Przyim1 := vp.Przyim1,
    vp.Typ == part,
    setscore -1.94591014905531

vc-vp--vp = vp vc-vp* %vc-vp[vp:compl3+]%
    To := vp.To,
    vp.Typ == inf,
    setscore -3.3322045101752

vc-vp--vp = vp vc-vp* %vc-vp[vp:compl3+]%
    Typ := vp.Typ,
    vp.Typ == inf,
    setscore -3.3322045101752

vc-vp--vp = vp vc-vp* %vc-vp[vp:compl3+]%
    vp.Typ == inf,
    setscore -2.63905732961526

vc-vp--vp = vp vc-vp* %vc-vp[vp:compl3+]%
    vp.Typ == part,
    setscore -1.7227665977411

vc-vp--vp = vp vc-vp* %vc-vp[vp:compl3+]%
    setscore -3.3322045101752

vf = vf-advx*
    setscore -1.91692261218206

vf = vf-ap*
    setscore -2.83321334405622

vf = vf-fkonj*
    setscore -3.52636052461616

vf = vf-fx*
    setscore -3.52636052461616

vf = vf-mf*
    setscore -3.52636052461616

vf = vf-np*
    setscore -1.04145387482816

vf = vf-pp*
    setscore -1.58045037556085

vf = vf-simpx*
    setscore -2.14006616349627

vf = vf-vc*
    setscore -3.52636052461616

vf-advx = advx* %VF[advx:mod]%
    Block := 1,
    setscore -1.6094379124341

vf-advx = advx* %VF[advx:oadjp]%
    Block := 1,
    setscore -1.6094379124341

vf-advx = advx* %VF[advx:oadvp]%
    Block := 1,
    setscore -1.6094379124341

vf-advx = advx* %VF[advx:pred]%
    Block := 1,
    PRED := true,
    setscore -1.6094379124341

vf-advx = advx* %VF[advx:v-mod]%
    Block := 1,
    setscore -1.6094379124341

vf-ap = ap* %VF[ap:pred]%
    Block := 1,
    PRED := true,
    setscore -0.693147180559945

vf-ap = ap* %VF[ap:v-mod]%
    Block := 1,
    setscore -0.693147180559945

vf-fkonj = fkonj* %VF[fkonj]%
    Block := 1,
    setscore 0

vf-fx = fx* %VF[fx:subject]%
    Block := 1,
    setscore 0

vf-mf = mf* %VF[mf]%
    Block := 1,
    setscore 0

vf-np = np* %VF[np:compl1]%
    Block := 1,
    C == acc,
    OA := true,
    OA_C := C,
    setscore -2.484906649788

vf-np = np* %VF[np:compl1]%
    Block := 1,
    C == dat,
    OD := true,
    OD_C := C,
    setscore -2.484906649788

vf-np = np* %VF[np:compl1]%
    Block := 1,
    OA := true,
    setscore -2.484906649788

vf-np = np* %VF[np:es]%
    Block := 1,
    ES := true,
    setscore -2.484906649788

vf-np = np* %VF[np:mod]%
    Block := 1,
    setscore -2.484906649788

vf-np = np* %VF[np:oa]%
    Block := 1,
    C == nom,
    OA := true,
    OA_C := C,
    setscore -2.484906649788

vf-np = np* %VF[np:on-mod]%
    Block := 1,
    setscore -2.484906649788

vf-np = np* %VF[np:pred]%
    Block := 1,
    PRED := true,
    setscore -2.484906649788

vf-np = np* %VF[np:subject]%
    Block := 1,
    C == nom,
    ON := true,
    ON_Num := Num,
    ON_P := P,
    setscore -2.484906649788

vf-np = np* %VF[np:subject]%
    Block := 1,
    ON := true,
    setscore -2.484906649788

vf-np = np* %VF[np:subject]%
    Block := 1,
    ON := true,
    ON_Num := Num,
    ON_P := P,
    setscore -2.484906649788

vf-np = np* %VF[np:v-mod]%
    Block := 1,
    setscore -2.484906649788

vf-pp = pp* %VF[pp:compl1]%
    Block := 1,
    OPP := true,
    setscore -1.94591014905531

vf-pp = pp* %VF[pp:compl1]%
    Block := 1,
    OPP := true,
    OPP_C := C,
    OPP_Przyim := Przyim,
    setscore -1.94591014905531

vf-pp = pp* %VF[pp:compl2]%
    Block := 1,
    OPP := true,
    setscore -1.94591014905531

vf-pp = pp* %VF[pp:compl2]%
    Block := 1,
    OPP := true,
    OPP_C := C,
    OPP_Przyim := Przyim,
    setscore -1.94591014905531

vf-pp = pp* %VF[pp:mod]%
    Block := 1,
    setscore -1.94591014905531

vf-pp = pp* %VF[pp:oa-mod]%
    Block := 1,
    setscore -1.94591014905531

vf-pp = pp* %VF[pp:v-mod]%
    Block := 1,
    setscore -1.94591014905531

vf-simpx = simpx* %VF[simpx:fopp-mod]%
    Block := 1,
    setscore -1.38629436111989

vf-simpx = simpx* %VF[simpx:mod]%
    Block := 1,
    setscore -1.38629436111989

vf-simpx = simpx* %VF[simpx:os]%
    Block := 1,
    setscore -1.38629436111989

vf-simpx = simpx* %VF[simpx:subject]%
    Block := 1,
    setscore -1.38629436111989

vf-vc = vc* %VF[vc]%
    Block := 1,
    setscore 0

vp = vp-part--verb*
    { Block := false },
    setscore -0.405465108108164

vp = vp-verb*
    { Block := false },
    setscore -1.79175946922806

vp = vp-vp--vp*
    { Block := false },
    setscore -1.79175946922806

vp-part = part* %VP[part]%
    Block := 1,
    T == zu,
    To := 1,
    setscore 0

vp-part--verb = vp-part* verb %vp-part[+verb]%
    Dop1 := verb.Dop1,
    Dop2 := verb.Dop2,
    Przyim1 := verb.Przyim1,
    Przyim2 := verb.Przyim2,
    Typ := verb.Typ,
    setscore -1.38629436111989

vp-part--verb = vp-part* verb %vp-part[+verb]%
    Dop1 := verb.Dop1,
    Przyim1 := verb.Przyim1,
    Typ := verb.Typ,
    setscore -1.38629436111989

vp-part--verb = vp-part* verb %vp-part[+verb]%
    Dop3 := verb.Dop3,
    Typ := verb.Typ,
    setscore -1.38629436111989

vp-part--verb = vp-part* verb %vp-part[+verb]%
    Typ := verb.Typ,
    setscore -1.38629436111989

vp-verb = verb* %VP[verb]%
    Block := 1,
    setscore 0

vp-vp = vp* %VP[vp:konj]%
    Block := 1,
    setscore 0

vp-vp--conj = vp-vp* conj %vp-vp[+conj]%
    setscore 0

vp-vp--vp = vp-vp--conj* vp %vp-vp--conj[+vp:konj]%
    setscore 0

