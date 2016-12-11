import cPickle as pickle
import os, time
from levenshtein import levSearch

def levenshtein(index, query, preferredCost):
	tfile = []
	tdir = []
	for path, dir, file in index:
		if dir!=[]:
			for i in dir:
				cost = levSearch(query, i, preferredCost)
				if cost<3 and cost>=0:
					tdir.append((i,path))
		if file!=[]:
			for i in file:
				cost = levSearch(query, i, preferredCost)
				if cost<3 and cost>=0:
					tfile.append((i,path))
	print tfile
	print tdir

searchTerm = raw_input(">>Enter search term: ")
preferredCost = 3
drivePath = raw_input(">>Enter Drive letter or path: ")
x = time.time()

if len(drivePath) == 1:
	drivePath += ":/"

list = []
for (path,dir,files) in os.walk(drivePath):
	if(dir!=[] or files!=[]):
		list.append((path, dir, files))
	"""for i in files:
		try:
			#print i,path
			i.encode('utf8')
		except Exception as e:
			os.system('pause')"""

y = time.time()
print y-x

#levenshtein(list, searchTerm, preferredCost)
z = time.time()
print z-y

output = open("list", "wb")
pickle.dump(list, output, pickle.HIGHEST_PROTOCOL) #-1 = pickle.HIGHEST_PROTOCOL
output.close()
input = open("list", "rb")
obj = pickle.load(input)
print len(obj)
os.system('pause')