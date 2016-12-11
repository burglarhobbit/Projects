def capitalize_all(t):
	res = ""
	for s in t:
		res += s.capitalize()
	return res

def capitalize_nested(list_of_strings):

	final_list = []
	for item in list_of_strings:
		final_list.append(capitalize_all(item))
	return final_list

print ("Enter list of strings(separated by spaces): ")
w = map(str, raw_input().strip().split(' ') )
print "\nCapitalized String: \n", capitalize_nested(w)
