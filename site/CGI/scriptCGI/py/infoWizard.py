
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
	# HTTP Header
	print("protocol: HTTP/1.1 200")
	print("content-type: text/html")
	print("")
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
	print('<h1>your school curriculum at Howgard</h1>')
	print('<h1>House :</h1>')
	print('<p>your house is ' + house + ' !</p>')
	print('<h1>Quidditch position</h1>')
	print('<h1>Destiny</h1>')
	print('<h1>Favorite potion</h1>')
	print("</body>")
	print("</html>")

# with open(wizard_id + '_house', 'r') as file:
#	 house = file.read()
# with open(wizard_id + '_house', 'r') as file:
#	 house = file.read()

# security
if __name__ == "__main__":
	main()
