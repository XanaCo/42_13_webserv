#!/usr/bin/env python3

import random
import cgi
import os
from urllib.parse import parse_qs

# parsing
parsed_data = parse_qs(os.environ.get('CONTENT_BODY'))
user_data = {key: value[0] for key, value in parsed_data.items()}
wizard_id = os.environ.get('WIZARD_ID').replace("wizard_id=", "")

# data struct
qualities = {
    "Speed": "Seeker",
    "Resilience": "Chaser",
    "Control": "Keeper",
	"Force": "Beater",
}

# main function
def main():

	with open(wizard_id + '_position', 'w') as file:
		file.write(qualities.get(user_data.get('quality')))

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
print('<h1>Quidditch selection result</h1>')
print('<p>' + user_data.get('name') + ', you are a <strong>' + qualities.get(user_data.get('quality')) + '</strong>.</p>')
print("</body>")
print("</html>")

# security
if __name__ == "__main__":
	main()
