<?php

if ($_SERVER["REQUEST_METHOD"] == "POST" && isset($_FILES["file"])) {
    $uploadDirectory = '../../fileFolder/';
    $targetFile = $uploadDirectory . basename($_FILES["file"]["name"]);

    if (move_uploaded_file($_FILES["file"]["tmp_name"], $targetFile)) {
        echo "File deleted successfully.";
    } else {
        echo "Error deleting file.";
    }
}

?>
