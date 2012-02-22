# test binaryzacji z atrybutami

x0 = a b c
    b.X == c.Y

x1 = d e f
    d.X = e.Y

x2 = g2* h2 i2
    h2.X == i2.Y

x3 = g3 h3* i3
    g3.X == i3.Y

x4 = g4 h4 i4*
    g4.X == h4.Y	

x5 = g5 h5 i6*
    g5.X = a

x6 = j6 k6 l6 m6
    j6.X == k6.X,
    k6.X == l6.X,
    l6.X == m6.X

x7 = j7 k7 l7 m7 n7
    j7.X = a,
    k7.X = a,
    l7.X = a,
    m7.X = a,
    n7.X = a


x8 = o p* s

y8 = o p* r
    r.X = a,
    r.Y = b

z8 = p* r s
    s.X = a

