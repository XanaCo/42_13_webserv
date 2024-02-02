<?php

if ($_SERVER["REQUEST_METHOD"] == "GET" && isset($_GET["file"])) {
    $downloadDirectory = '../../fileFolder/';
    $filePath = $downloadDirectory . $_GET["file"];

    if (file_exists($filePath)) {
        header("Content-Disposition: attachment; filename=" . basename($filePath));
        readfile($filePath);
        exit;
    } else {
        echo "File not found.";
    }
}

?>
