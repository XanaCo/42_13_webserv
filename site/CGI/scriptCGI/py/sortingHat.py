#!/usr/bin/env python3

# Le body : name=GuapoPablo&creature=Octopus

import random
import cgi
import os
from urllib.parse import parse_qs   # pour parse les arguments

# parsing
parsed_data = parse_qs(os.environ.get('CONTENT_BODY'))
user_data = {key: value[0] for key, value in parsed_data.items()}

# data struct
creatures = {
    "Crab": "Gryffindor",
    "Octopus": "Slytherin",
    "Guepard": "Ravenclaw",
	"Capybara": "Hufflepuff",
}

# form input
# def get_form():
# 	form = cgi.FieldStorage()
# 	name = form.getvalue('name', '')
# 	creature = form.getvalue('creature', '')
# 	return name, creature

# main function
#def main():
#	name, preferred_creature = get_form()
#
#	if preferred_creature in creatures:
#		selected_house = creatures[preferred_creature]
#	else :
#		selected_house = random.choice(list(creatures.values()))

def main():

# HTTP Header
    print("protocol: HTTP/1.1 200")
    print("content-type: text/html")
    print()

# html response
    print("<html><body>")
    print('<h1>Sorting Hat Result</h1>')
    print('<p>' + user_data.get('name') + ', you belong to <strong>' + creatures.get(user_data.get('creature')) + '</strong>.</p>')
    print("</body></html>")

# security
if __name__ == "__main__":
	main()
