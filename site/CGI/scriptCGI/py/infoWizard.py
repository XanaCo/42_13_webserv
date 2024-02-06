import random
import cgi
import os
from urllib.parse import parse_qs

# parsing
wizard_id = os.environ.get('WIZARD_ID').replace("wizard_id=", "")

def read_file(file_path):
	try:
		with open(file_path, 'r') as file:
			return file.read()
	except FileNotFoundError:
		return None
	except Exception as e:
		print(f"Error reading file {file_path}: {e}")
		return None

def main():
	house_file_path = 'site/CGI/cookies/' + wizard_id + '_house'
	position_file_path = 'site/CGI/cookies/' + wizard_id + '_position'
	destiny_file_path = 'site/CGI/cookies/' + wizard_id + '_destiny'
	potion_file_path = 'site/CGI/cookies/' + wizard_id + '_potion'

	house = read_file(house_file_path)
	position = read_file(position_file_path)
	destiny = read_file(destiny_file_path)
	potion = read_file(potion_file_path)

	# HTTP Header
	# print("protocol: HTTP/1.1 200")
	# print("content-type: text/html")
	# print("")

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

	if house is not None:
		print('<h3>House :</h3>')
		print('<p>Your house is <strong>' + house + '</strong>')

	if position is not None:
		print('<h3>Quidditch position :</h3>')
		print('<p>You are a <strong>' + position + '</strong>')

	if destiny is not None:
		print('<h3>The answer to your question was :</h3>')
		print('<p><strong>' + destiny + '</strong>')

	if potion is not None:
		print('<h3>Your favorite potion is :</h3>')
		print('<p><strong>' + potion + '</strong>')

	print("</body>")
	print("</html>")

# security
if __name__ == "__main__":
	main()
