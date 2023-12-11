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
def main():
	name, preferred_creature = get_form()

	if preferred_creature in creatures:
		selected_house = creatures[preferred_creature]
	else
		selected_house = random.choice(list(creatures.values()))

# html response
print("<html><body>")
print(f"<h1>Sorting Hat Result</h1>")
print(f"<p>{name}, you belong to <strong>{selected_house}</strong>.</p>")
print("</body></html>")

# security
if __name__ == "__main__":
	main()
