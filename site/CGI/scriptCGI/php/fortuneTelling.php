#!/usr/bin/php-cgi
<?php

// parsing
// parse_str(file_get_contents('php://input'), $parsed_data);
// $user_data = $parsed_data;

// Read the question from the form
$question = $_POST['question'] ?? '';

// Generate a fortune based on the question
function generateFortune($question) {
    // You can create your own logic for generating fortunes here
    // For simplicity, we'll provide a basic example
    $fortunes = [
        "Yes, definitely.",
        "It is certain.",
        "Ask again later.",
        "Cannot predict now.",
        "Don't count on it.",
        "Very doubtful.",
    ];
    
    // Use a simple hashing to select a fortune based on the question
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
echo "<html>";
echo "<head><title>Devination Class Result</title></head>";
echo "<body>";
echo "<h1>Your Fortune</h1>";
echo "<p>You asked: <strong>$question</strong></p>";
echo "<p>The magical answer is: <strong>$fortune</strong></p>";
echo '<p>' . $user_data['name'] . ', you belong to <strong>' . $fortune[0] . '</strong>.</p>';
echo "</body>";
echo "</html>";

?>
