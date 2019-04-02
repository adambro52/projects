#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Tue Feb  6 13:02:35 2018

@author: breese
"""
from __future__ import division
import re
import codecs

################# FUNCTION DEFINITIONS -- DO NOT CHANGE #######################

def precision(system,gold):
    s = set(system)
    g = set(gold)
    tp = s.intersection(g)
    return len(tp)/len(s)

def recall(system,gold):
    s = set(system)
    g = set(gold)
    tp = s.intersection(g)
    return len(tp)/len(g)

def f1(p,r):
    return 2 * ((p * r)/(p + r))


def analyze(system,gold):
    p = precision(system,gold)
    r = recall(system,gold)
    f = f1(p,r)
    print("Precision: %f\tRecall: %f\tF1: %f" % (p,r,f))

################################################################################


# 0. Read the dates from the gold standard file into the list gold_dates.  These are 
# all of the dates in the dev.txt that your regular expression should match.  
gold_dates = [date.strip() for date in open("dev_gs.txt", "rU").readlines() if date.strip()]


# 1. Read the contents of dev.txt into the string doc.  This is the file that your
# regular expression will scrape for date expressions.
doc = codecs.open("dev.txt", encoding='cp1251').read()


# 2. Define your regular expression in the string pattern.  The VERBOSE flag allows you to 
# leverage multi-line strings, whitespace, and comments within a regex definition.
pattern = r'''\b(?:[12][0-9]?|3[01])(?:st,?|nd,?|rd,?|th,?)?(?:\.|\/|\-|\ )[0-9][0-9]?,?(?:\.|\/|\-|\ )[0-9][0-9][0-9][0-9]\b|
    \b[0-9][0-9]?,?(?:\.|\/|\-|\ )(?:[0-9][\ ]?|1[0-2][\ ]?|Jan[\ ]?|Feb[\ ]?|Mar[\ ]?|Apr[\ ]?|May[\ ]?|Jun[\ ]?|Jul[\ ]?|Aug[\ ]?|Sep[\ ]?|Oct[\ ]?|Nov[\ ]?|Dec[\ ]?|January|February|March|April|May|June|July|August|September|October|November|December),?(?:\.|\/|\-|\ )[0-9][0-9][0-9][0-9]\b|
    \b(?:[0-9][\ ]?|1[0-2][\ ]?|Jan[\ ]?|Feb[\ ]?|Mar[\ ]?|Apr[\ ]?|May[\ ]?|Jun[\ ]?|Jul[\ ]?|Aug[\ ]?|Sep[\ ]?|Oct[\ ]?|Nov[\ ]?|Dec[\ ]?|January|February|March|April|May|June|July|August|September|October|November|December),?(?:\.|\/|\-|\ )[0-9][0-9][0-9][0-9]\b|
    \b(?:[0-9][\ ]?|1[0-2][\ ]?|Jan[\ ]?|Feb[\ ]?|Mar[\ ]?|Apr[\ ]?|May[\ ]?|Jun[\ ]?|Jul[\ ]?|Aug[\ ]?|Sep[\ ]?|Oct[\ ]?|Nov[\ ]?|Dec[\ ]?|January|February|March|April|May|June|July|August|September|October|November|December),?[\ ][0-9][0-9]\b|
    \b(?:[012][0-9]?|3[01])(?:st,?|nd,?|rd,?|th,?)?(?:\.|\/|\-|\ )(?:[0-9][\ ]?|1[0-2][\ ]?|Jan[\ ]?|Feb[\ ]?|Mar[\ ]?|Apr[\ ]?|May[\ ]?|Jun[\ ]?|Jul[\ ]?|Aug[\ ]?|Sep[\ ]?|Oct[\ ]?|Nov[\ ]?|Dec[\ ]?|January|February|March|April|May|June|July|August|September|October|November|December),?(?:\.|\/|\-|\ )[0-9][0-9]\b|
    \b(?:[0-9][\ ]?|1[0-2][\ ]?|Jan[\ ]?|Feb[\ ]?|Mar[\ ]?|Apr[\ ]?|May[\ ]?|Jun[\ ]?|Jul[\ ]?|Aug[\ ]?|Sep[\ ]?|Oct[\ ]?|Nov[\ ]?|Dec[\ ]?|January|February|March|April|May|June|July|August|September|October|November|December)(?:\-|\.|\/|\ )[0-9][0-9]?(?:st,?|nd,?|rd,?|th,?)?(?:\.|\/|\-|\ )[0-9][0-9][0-9][0-9]\b|
    \b[0-9][0-9][0-9][0-9](?:\-|\.|\/|\ )(?:[012][0-9]?|3[01])(?:st,?|nd,?|rd,?|th,?)?(?:\.|\/|\-|\ )(?:[0-9][\ ]?|1[0-2][\ ]?|Jan[\ ]?|Feb[\ ]?|Mar[\ ]?|Apr[\ ]?|May[\ ]?|Jun[\ ]?|Jul[\ ]?|Aug[\ ]?|Sep[\ ]?|Oct[\ ]?|Nov[\ ]?|Dec[\ ]?|January|February|March|April|May|June|July|August|September|October|November|December)\b|
    \b[0-9][0-9](?:\.|\/|\-|\ )(?:[0-9][\ ]?|1[0-2][\ ]?|Jan[\ ]?|Feb[\ ]?|Mar[\ ]?|Apr[\ ]?|May[\ ]?|Jun[\ ]?|Jul[\ ]?|Aug[\ ]?|Sep[\ ]?|Oct[\ ]?|Nov[\ ]?|Dec[\ ]?|January|February|March|April|May|June|July|August|September|October|November|December),?(?:\.|\/|\-|\ )(?:[012][0-9]?|3[01])(?:st,?|nd,?|rd,?|th,?)?\b|
    \b[0-9][0-9][0-9][0-9](?:\-|\.|\/|\ )(?:[012][0-9]?|3[01])(?:st,?|nd,?|rd,?|th,?)?(?:\.|\/|\-|\ )(?:[0-9][\ ]?|1[0-2][\ ]?|Jan[\ ]?|Feb[\ ]?|Mar[\ ]?|Apr[\ ]?|May[\ ]?|Jun[\ ]?|Jul[\ ]?|Aug[\ ]?|Sep[\ ]?|Oct[\ ]?|Nov[\ ]?|Dec[\ ]?|January|February|March|April|May|June|July|August|September|October|November|December)\b|
    \b[0-9][0-9][0-9][0-9](?:st,?|nd,?|rd,?|th,?)?(?:\.|\/|\-|\ )(?:[0-9][\ ]?|1[0-2][\ ]?|Jan[\ ]?|Feb[\ ]?|Mar[\ ]?|Apr[\ ]?|May[\ ]?|Jun[\ ]?|Jul[\ ]?|Aug[\ ]?|Sep[\ ]?|Oct[\ ]?|Nov[\ ]?|Dec[\ ]?|January|February|March|April|May|June|July|August|September|October|November|December)(?:\-|\.|\/|\ )(?:[012][0-9]?|3[01])\b
'''
    # \b[0-9][0-9][0-9][0-9](?:\-|\.|\/|\ )(?:[0-9][\ ]?|1[0-2][\ ]?|Jan[\ ]?|Feb[\ ]?|Mar[\ ]?|Apr[\ ]?|May[\ ]?|Jun[\ ]?|Jul[\ ]?|Aug[\ ]?|Sep[\ ]?|Oct[\ ]?|Nov[\ ]?|Dec[\ ]?|January|February|March|April|May|June|July|August|September|October|November|December)(?:\-|\.|\/|\ )(?:[012][0-9]?|3[01])(?:st,?|nd,?|rd,?|th,?)?\b
    # (\b(?:(?:[12][0-9]?|3[01])(?:st|nd|rd|th)*(?:\-|\.|\/|\ ))*(?:[0-9]|1[0-2]|Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec|January|February|March|April|May|June|July|August|September|October|November|December)(?:\-|\.|\/|,?\ )[0-9][0-9][0-9][0-9]\b)|
    # (?:(?:[0-9]|1[0-2]|Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec|January|February|March|April|May|June|July|August|September|October|November|December)(?:\-|\.|\/|\ )(?:[12][0-9]?|3[01])(?:st,?|nd,?|rd,?|th,?)*(?:(?:\-|\.|\/|\ )[0-9][0-9][0-9][0-9])*)|
    # (?:[0 - 9][0 - 9][0 - 9][0 - 9](?:\-|\.|\/|\ )(?:[12][0-9]?|3[01])(?:st|nd|rd|th)*(?:\-|\.|\/|\ )(?:[0-9]|1[0-2]|Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec|January|February|March|April|May|June|July|August|September|October|November|December))
# '''

regex = re.compile(pattern, re.VERBOSE)


# 3. Use the findall method to return a list of every occurrence of a date 
# in the development file.
dates = re.findall(regex, doc)
print('\nlength of dates: ' + str(len(dates)))
for date in dates:
    print (date)


# 4. Evaluate your regex using the evaluate function.
print(analyze(dates, gold_dates))
