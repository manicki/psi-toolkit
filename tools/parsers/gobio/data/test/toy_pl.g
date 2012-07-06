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

#    np.Case == acc

# Bezpośrednie przypisywanie atrybutów na razie nie działa,
# ponieważ są zamieniane przez tgbg_combinator::add_rules na liczby

