#!/usr/bin/env python3

import random
import cgi
import os
from urllib.parse import parse_qs   # pour parse les arguments

# parsing
parsed_data = parse_qs(os.environ.get('CONTENT_BODY'))
user_data = {key: value[0] for key, value in parsed_data.items()}

# data struct
qualities = {
    "Speed": "Seeker",
    "Resilience": "Chaser",
    "Control": "Keeper",
	"Force": "Beater",
}

# # form input
# def get_form():
# 	form = cgi.FieldStorage()
# 	name = form.getvalue('name', '')
# 	quality = form.getvalue('quality', '')
# 	return name, quality

# # main function
def main():

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
    print('<p>' + user_data.get('name') + ', you belong to <strong>' + qualities.get(user_data.get('quality')) + '</strong>.</p>')
    print("</body></html>")

# security
if __name__ == "__main__":
	main()
