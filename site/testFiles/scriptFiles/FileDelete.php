<?php

if ($_SERVER["REQUEST_METHOD"] == "DELETE" && isset($_GET["file"])) {
    $downloadDirectory = '../../fileFolder/';
    $filePath = $downloadDirectory . $_GET["file"];

	if (file_exists($filePath)) {
        if (unlink($filePath)) {
            echo "File deleted successfully.";
        } else {
            echo "Error deleting the file.";
        }
    } else {
        echo "File not found.";
    }
}

?>
