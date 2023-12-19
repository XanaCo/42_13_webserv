#!/usr/bin/env python3

import random
import cgi

# data struct
qualities = {
    "Speed": "Seeker",
    "Resilience": "Chaser",
    "Control": "Keeper",
	"Force": "Beater",
}

# form input
def get_form():
	form = cgi.FieldStorage()
	name = form.getvalue('name', '')
	quality = form.getvalue('quality', '')
	return name, quality

# main function
def main():
	name, preferred_quality = get_form()

	if preferred_quality in qualities:
		position = qualities[preferred_quality]
	else
		position = random.choice(list(qualities.values()))

# html response
print("<html><body>")
print(f"<h1>Position Assignment</h1>")
print(f"<p>{name}, you are a <strong>{position}</strong>.</p>")
print("</body></html>")

# security
if __name__ == "__main__":
	main()