# #!/usr/bin/env python3

import random
import cgi
import sys
import os
from urllib.parse import parse_qs

# parsing
parsed_data = parse_qs(os.environ.get('CONTENT_BODY'))
user_data = {key: value[0] for key, value in parsed_data.items()}
wizard_id = os.environ.get('WIZARD_ID').replace("wizard_id=", "")

# data struct
creatures = {
	"Crab": "Hufflepuff",
	"Octopus": "Slytherin",
	"Guepard": "Ravenclaw",
	"Capybara": "Gryffindor",
}

def main():

# register data
	sys.stderr.write("voici mon wizard id :" + wizard_id + "\n")
	with open(wizard_id + '_house', 'w') as file:
		file.write(creatures.get(user_data.get('creature')))

# HTTP Header
	print("protocol: HTTP/1.1 200")
	print("content-type: text/html")
	print()

# html response
	print("<html>")
	print("<head>")
	print('<meta charset="UTF-8">')
	print('<meta name="viewport" content="width=device-width, initial-scale=1.0">')
	print('<title>Welcome to Hogwarts</title>')
	print('<link rel="stylesheet" href="../../../css/style.css" />')
	print('<link rel="icon" type="image/png" href="../../../img/LOGO_cursor.png" />')
	print("</head>")
	print("<body>")
	print('<h1>Sorting Hat Result</h1>')
	print('<p>' + user_data.get('name') + ', you belong to <strong>' + creatures.get(user_data.get('creature')) + '</strong>.</p>')
	print("</body>")
	print("</html>")

# security
if __name__ == "__main__":
	main()
