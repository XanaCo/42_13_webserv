#!/usr/bin/php-cgi
<?php

// Read the ingredients in the ENV data from the request body
$postData = getenv("CONTENT_BODY");

// Parse the raw data into an associative array
parse_str($postData, $parsedData);

// Take the question
$question = $parsedData['question'] ?? '';

// Generate random response
function generateFortune($question) {

	$fortunes = [
		"Yes, definitely.",
		"It is certain.",
		"Ask again later.",
		"Cannot predict now.",
		"Don't count on it.",
		"Very doubtful.",
	];

	$fortuneIndex = crc32($question) % count($fortunes);

	return $fortunes[$fortuneIndex];
}

// Generate the fortune
$fortune = generateFortune($question);

// Print the CGI header
header("content-type: text/html");
header("protocol: HTTP/1.1 200");
header("");

// Output HTML content
echo "<html>\n";
echo "<head>\n";
echo "<title>Devination Class Result</title>\n";
echo '<link rel="stylesheet" href="../../../css/style.css" />';
echo "\n";
echo '<link rel="icon" type="image/png" href="../../../img/LOGO_cursor.png"/>';
echo "\n";
echo "</head>\n";
echo "<body>\n";
echo "<h1>Your Fortune</h1>\n";
echo "<p>You asked: <strong>$question</strong></p>\n";
echo "<p>The magical answer is: <strong>$fortune</strong></p>\n";
echo "</body>\n";
echo "</html>\n";

?>
