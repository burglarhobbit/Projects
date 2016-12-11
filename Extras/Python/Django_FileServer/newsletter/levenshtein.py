# File Owner: Bhavya Patwa
# This package contains 2 modules of Levenshtein String Search Algorithm : levSearch and oldLevSearch
# The former uses only 2 rows of an imaginary matrix of size [LengthOfString1][LengthOfString2]
# While the latter uses the whole matrix of the size as mentioned above
# The levSearch complexity is O(2*(m+n)) while oldLevSearch complexity is O(n*m)

def levSearch(lhs, rhs, preferredCost):
	a1 = len(lhs) + 1
	b1 = len(rhs) + 1
	if a1-b1 < -preferredCost or a1-b1 > preferredCost:
		return -1

	horizontal = [x for x in range(b1)]
	newH = [x for x in range(b1)]

	for i in range(1, a1):
		newH[0] = i
		for j in range(1, b1):
			if lhs[i-1] == rhs[j-1]:
				cost = 0
			else:
				cost = 1

			newH[j] = min(newH[j-1] + 1, horizontal[j] + 1, horizontal[j-1] + cost)
			if i == j and newH[j] >= preferredCost:
				return newH[j]
		horizontal = newH[:]
	return newH[b1-1]

def oldLevSearch(lhs, rhs):
	a1 = len(lhs)+1
	b1 = len(rhs)+1
	distance = [[0 for x in range(b1)] for x in range(a1)]
	print distance
	for index, i in enumerate(distance):
		distance[index][0] = index
	print distance
	distance[0] = [index for index, i in enumerate(distance[0])]
	print distance
	#check = True
	
	for i in range(1,a1):
		for j in range(1,b1):
			if lhs[i-1] == rhs[j-1]:
				cost = 0
			else:
				cost = 1
			distance[i][j] = min(distance[i-1][j] + 1, distance[i][j-1] +1, distance[i-1][j-1] + cost)
			if i==j and newH[j]>=3:
				return distance[j]

	#print distance
	return distance[a1-1][b1-1]
