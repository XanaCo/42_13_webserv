#!/usr/bin/env python3

import random
import cgi

# data struct
creatures = {
    "Crab": "Gryffindor",
    "Octopus": "Slytherin",
    "Guepard": "Ravenclaw",
	"Capybara": "Hufflepuff",
}

# form input
def get_form():
	form = cgi.FieldStorage()
	name = form.getvalue('name', '')
	creature = form.getvalue('creature', '')
	return name, creature

# main function
#def main():
#	name, preferred_creature = get_form()
#
#	if preferred_creature in creatures:
#		selected_house = creatures[preferred_creature]
#	else :
#		selected_house = random.choice(list(creatures.values()))

def main():
    name = "Alban"
    selected_house = "Serpentard"

# HTTP Header
    print("content-type: text/html")
    print("status-code: 200")
    print("protocol: HTTP/1.1")
    print()

# html response
    print("<html><body>")
    print('<h1>Sorting Hat Result</h1>')
    print('<p>{Alban, you belong to <strong>Serpentard</strong>.</p>')
    print("</body></html>")

# security
if __name__ == "__main__":
	main()
