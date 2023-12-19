#!/usr/bin/php-cgi
<?php

// Read the selected ingredients from the form
$selectedIngredients = array_map('strval', $_POST['ingredients'] ?? []);

// Map of ingredients to their corresponding Harry Potter names
$ingredientNames = [
    "lacewing_flies" => "Lacewing Flies",
    "leeches" => "Leeches",
    "powdered_bicorn_horn" => "Powdered Bicorn Horn",
    "knotgrass" => "Knotgrass",
    "fluxweed" => "Fluxweed",
    "shredded_boomslang_skin" => "Shredded Boomslang Skin",
];

// Map of ingredient combinations to potion names
$potionRecipes = [
    ["lacewing_flies", "leeches"] => "Polyjuice Potion",
    ["knotgrass", "fluxweed"] => "Felix Felicis",
    // Add more potion recipes as needed
    // Example:
    ["some_ingredient", "another_ingredient"] => "Another Potion",
];

// List of predefined potion names
$predefinedPotionNames = [
    "Polyjuice Potion",
    "Felix Felicis",
    "Amortentia",
    "Veritaserum",
    "Draught of Living Death",
    "Wolfsbane Potion",
    "Pepperup Potion",
    "Skele-Gro",
    "Love Potion",
    "Fire Protection Potion",
];

// Ensure that $selectedIngredients are strings
$selectedIngredients = array_map('strval', $selectedIngredients);

// Generate a potion name based on the selected ingredients
function generatePotionName($ingredients, $ingredientNames, $potionRecipes, $predefinedPotionNames) {
    // Check if the selected combination matches any known potion recipe
    foreach ($potionRecipes as $recipeIngredients => $potionName) {
        // Ensure that keys are strings for comparison
        $recipeIngredients = array_map('strval', $recipeIngredients);
        // Sort the arrays to ensure order doesn't matter
        sort($recipeIngredients);
        sort($ingredients);
        if ($recipeIngredients === $ingredients) {
            return $potionName;
        }
    }

    // Check if the selected combination matches any predefined potion name
    $matchingPotionNames = array_intersect($ingredients, array_keys($predefinedPotionNames));
    if (!empty($matchingPotionNames)) {
        return implode(" ", $matchingPotionNames);
    }

    // If no match is found, create a generic potion name
    $potionName = "Magical Potion with ";
    foreach ($ingredients as $ingredient) {
        $potionName .= $ingredientNames[$ingredient] . ", ";
    }
    $potionName = rtrim($potionName, ", "); // Remove the trailing comma and space
    return $potionName;
}

// Generate the potion name
$potionName = generatePotionName($selectedIngredients, $ingredientNames, $potionRecipes, $predefinedPotionNames);

// Print the CGI header
header("Content-type: text/html");

// Output HTML content
echo "<html>";
echo "<head><title>Harry Potter Potion Result</title></head>";
echo "<body>";
echo "<h1>Harry Potter Potion Created!</h1>";
echo "<p>Your potion, named <strong>$potionName</strong>, has been successfully brewed.</p>";
echo "<p>Enjoy your magical concoction!</p>";
echo "</body>";
echo "</html>";

?>
