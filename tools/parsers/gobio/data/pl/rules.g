# Toy grammar for Polish

s = np b vp*
    np.Number == vp.Number

ap = adj*

ap = adv b ap*

np = subst*

np = ap b np*
    ap.Case == np.Case,
    ap.Gender == np.Gender,
    ap.Number == ap.Number

vp = verb*

vp = adv b vp*

vp = vp* b np
    np.Case == acc

