#!/usr/bin/php-cgi
<?php

// Read the ingredients in the ENV data from the request body
$postData = getenv("CONTENT_BODY");

// Parse the raw data into an associative array
parse_str($postData, $parsedData);

// Take the selected ingredients
$selectedIngredients = $parsedData['ingredients'] ?? [];

// Map of ingredients
$ingredientNames = [
	"lacewing_flies" => "Lacewing Flies",
	"leeches" => "Leeches",
	"powdered_bicorn_horn" => "Powdered Bicorn Horn",
	"knotgrass" => "Knotgrass",
	"fluxweed" => "Fluxweed",
	"shredded_boomslang_skin" => "Shredded Boomslang Skin",

	"ashwinder_egg" => "Ashwinder egg",
	"sloth_brain" => "Sloth brain",
	"occamy_eggshell" => "Occamy eggshell",
	"extract_of_coccinella" => "Extract of coccinella",
	"tincture_of_thyme" => "Tincture of thyme",

	"pearl_dust" => "Pearl dust",
	"unicorn_hair" => "Unicorn hair",
	"valentine's_day_card" => "Valentine's Day card",

	"ginger" => "Ginger",
	"peppermint" => "Peppermint",
	"flobberworm_mucus" => "Flobberworm Mucus",

	"wiggentree_bark" => "Wiggentree Bark",
	"essence_of_murtlap" => "Essence of Murtlap",

	"rose_petals" => "Rose Petals",
	"dittany" => "Dittany",
	"lavender" => "Lavender",

	"salamander_blood" => "Salamander Blood",
	"griffin_claw" => "Griffin Claw",
	"ground_scarab_beetles" => "Ground Scarab Beetles"

];

// Map of ingredient combinations to potion names
$potionRecipes = [
	"lacewing_flies,leeches,powdered_bicorn_horn,knotgrass,fluxweed,shredded_boomslang_skin" => "Polyjuice Potion",
	"ashwinder_egg,sloth_brain,occamy_eggshell,extract_of_coccinella,tincture_of_thyme" => "Felix Felicis",
	"pearl_dust,unicorn_hair,valentine's_day_card" => "Amortentia",
	"shredded_boomslang_skin,powdered_bicorn_horn,knotgrass,leeches" => "Veritaserum",
	"ginger,peppermint,flobberworm_mucus" => "Pepperup Potion",
	"dittany,wiggentree_bark,essence_of_murtlap" => "Skele-Gro",
	"rose_petals,dittany,lavender" => "Love Potion",
	"salamander_blood,griffin_claw,ground_scarab_beetles" => "Fire Protection Potion",

];

// List of predefined potion names
$predefinedPotionNames = [
	"Polyjuice Potion",
	"Felix Felicis",
	"Amortentia",
	"Veritaserum",
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
		$recipeIngredientsArray = explode(',', $recipeIngredients);
		if (count(array_diff($ingredients, $recipeIngredientsArray)) === 0 && count(array_diff($recipeIngredientsArray, $ingredients)) === 0) {
			return $potionName;
		}
	}

	// Check if the selected combination matches any predefined potion name
	$matchingPotionNames = array_intersect($ingredients, array_keys($predefinedPotionNames));
	if (!empty($matchingPotionNames)) {
		return implode(" ", $matchingPotionNames);
	}

	// If no match is found, generic potion name
	 $potionName = "a really weird concoction";
		return $potionName;
}

// Generate the potion name
$potionName = generatePotionName($selectedIngredients, $ingredientNames, $potionRecipes, $predefinedPotionNames);

// Print the CGI header
header("protocol: HTTP/1.1 200");
header("content-type: text/html");
header("");

// Output HTML content
echo '<html>';
echo '<head>';
echo '<meta charset=UTF-8>';
echo '<meta name="viewport" content="width=device-width, initial-scale=1.0">';
echo '<title>Magical Potion Result</title>';
echo '<link rel="stylesheet" href="../../../css/style.css" />';
echo '<link rel="icon" type="image/png" href="../../../img/LOGO_cursor.png" />';
echo '</head>';
echo '<body>';
echo '<h1>Magical Potion Created!</h1>';
echo "<p>Your potion, <strong>$potionName</strong>, has been successfully brewed.</p>";
echo '<p>Enjoy your magical concoction!</p>';
echo '</body>';
echo '</html>';
?>
