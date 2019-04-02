import itertools

def x(a, c, e, g, i, k, y):
    if (a-e-g+k) != 0:
        return (y*(i-k-c+e)+k+e)/(a-e-g+k)


def z(a, c, e, y, x):
    return a*x + c*y + e*(1-x-y)


def vals2(lst):
    once = False
    for sub in lst:
        a = sub[0]
        c = sub[1]
        e = sub[2]
        g = sub[3]
        i = sub[4]
        k = sub[5]
        m = sub[6]
        o = sub[7]
        q = sub[8]
        for y in range(1,10):
            y /= 10
            x1 = x(a, c, e, g, i, k, y)
            if x1 == None:
                continue
            z1 = z(a, c, e, y, x1)
            x2 = x(a, c, e, m, o, q, y)
            if x2 == None:
                continue
            z2 = z(a, c, e, y, x2)
            if x1 == x2 and z1 == z2 and x1 + y <= 1 and x2 + y <= 1:
                if not once:
                    once = True
                else:
                    return a, c, e, g, i, k, m, o, q, y


lst = list(itertools.permutations([1,2,3,4,5,6,7,8,9]))
print(vals2(lst))
lst = list(itertools.permutations([-8,-7,-6,-5,-4,-3,-2,-1,0]))
print(vals2(lst))
# print(list(itertools.permutations([6,7,8,9,10,11,12,13,14])))
