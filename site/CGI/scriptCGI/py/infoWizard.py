
import random
import cgi
import os
from urllib.parse import parse_qs

# parsing
wizard_id = os.environ.get('WIZARD_ID').replace("wizard_id=", "")

def main():

	with open(wizard_id + '_house', 'r') as file:
		house = file.read()
	with open(wizard_id + '_position', 'r') as file:
		position = file.read()
	# with open(wizard_id + '_destiny', 'r') as file:
	# 	destiny = file.read()
	# with open(wizard_id + '_potion', 'r') as file:
	# 	potion = file.read()

	# HTTP Header
	print("protocol: HTTP/1.1 200")
	print("content-type: text/html")
	print("")

	# HTML Response
	print("<html>")
	print("<head>")
	print('<meta charset="UTF-8">')
	print('<meta name="viewport" content="width=device-width, initial-scale=1.0">')
	print('<title>Welcome to Hogwarts</title>')
	print('<link rel="stylesheet" href="../../../css/style.css" />')
	print('<link rel="icon" type="image/png" href="../../../img/LOGO_cursor.png" />')
	print("</head>")
	print("<body>")
	print('<h1>Your school curriculum at Hogwarts</h1>')
	print('<h3>Wizard ID Session :</h3>')
	print('<p>WID <strong>' + wizard_id + '</strong></p>')
	print('<h3>House :</h3>')
	print('<p>Your house is <strong>' + house + '</strong> !</p>')
	print('<h3>Quidditch position</h3>')
	print('<p>You are a <strong>' + position + '</strong> !</p>')
	print('<h3>Destiny</h3>')
	# print('<p>Your answer was <strong>' + destiny + '</strong></p>')
	print('<h3>Favorite potion</h3>')
	# print('<p>Your potion is <strong>' + potion + '</strong></p>')
	print("</body>")
	print("</html>")

# security
if __name__ == "__main__":
	main()
