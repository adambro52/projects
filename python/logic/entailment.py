# need to check if you are adding the same sentence that is not the same string

class KnowledgeBase:
    def __init__(self, base = None):
        self.base = base

class sentence:
    def __init__(self, literals, mom = None, dad = None):
        self.literals = literals
        self.mom = mom
        self.dad = dad

def resolve( a, b ):
    # parse strings
    c = a.literals.split()
    d = b.literals.split()
    complit = 0
    for i in range( len( c )):
        for j in range( len( d )):
            try:
            # see if there are complimentary literals
                if c[i] == d[j] and c[i-1] == "not":
                    complit += 1
                    idx = i
                    jdx = j
                    negatedI = True
                if c[i] == d[j] and d[j-1] == "not":
                    complit += 1
                    idx = i
                    jdx = j
                    negatedI = False
            except IndexError:
                continue
    if complit == 1:
        if negatedI:
            del c[idx] # removes literal
            del c[idx-1] # removes "not"
            del d[jdx] # removes other literal
        else:
            del c[idx]
            del d[jdx]
            del d[jdx-1]

        combined = c + d
        newSentence = []
        # puts 'not' and literal in same string so you don't have to add redundancies
        i = 0
        while i < len(combined) - 1:
            if combined[i] == "not":
                combined[i] = combined[i] + " " + combined[i + 1]
                del combined[i + 1]
            i += 1

        for s in combined:
            if s not in newSentence:
                newSentence += [s]

        retstring = ""
        for s in newSentence:
            if s == newSentence[-1]:
                retstring += s
            else:
                retstring += s + " "

        return sentence(retstring, a, b)

    return None

base = [sentence("not A B C not D"), sentence("B not C D"), sentence("A B not D"),
        sentence("A not B C"), sentence("not A C"), sentence("not B D"),
        sentence("not A"), sentence("not C")]
kb = KnowledgeBase(base)

# assumes only one complimentary literal per pair is valid to be resolved
# and that sentences can be reused for resolving
def entails( kb ): # have to pass in KB and ~a as the kb.base
    size = 0
    while size != len( kb.base ):
        size = len( kb.base )
        temp = []
        for i in range( len( kb.base )):
            for j in range(i + 1, len(kb.base), 1):
                resolvant = resolve(kb.base[i], kb.base[j])
                if resolvant and resolvant.literals == "":
                    printPath(resolvant, 1)
                    return True
                if resolvant and not contains(kb.base, resolvant):
                    kb.base.append(resolvant)

    print("No further resolutions breh you stuck. Your dumb KB is:")
    for s in kb.base:
        print(s.literals)
    return False

def contains(base, sen):
    # split base into list of lists containing literals i.e. ["A", "not B", "D"]
    lits = sen.literals.split()
    i = 0
    while i < len(lits) - 1:
        if lits[i] == "not":
            lits[i] = lits[i] + " " + lits[i + 1]
            del lits[i + 1]
        i += 1
    # and check if the set(sen) is within the set of each sublist
    for sentence in base:
        ayyeelits = sentence.literals.split()
        i = 0
        while i < len(ayyeelits) - 1:
            if ayyeelits[i] == "not":
                ayyeelits[i] = ayyeelits[i] + " " + ayyeelits[i + 1]
                del ayyeelits[i + 1]
            i += 1
        if set(ayyeelits) == set(lits):
            return True
    return False

def printPath(sen, depth):
    if sen.mom and sen.dad:
        print("combine", sen.mom.literals, "with", sen.dad.literals, depth)
        printPath(sen.mom, depth+1)
        printPath(sen.dad, depth+1)

print(entails(kb))
# print(contains(['not B not C not A'], 'not A not B not C'))
