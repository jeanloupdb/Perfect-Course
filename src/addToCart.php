<?php
$productReference = isset($_GET['productReference']) ? $_GET['productReference'] : '';

$quantity = $_POST['quantity'];
$cartEntry = $productReference . " " . $quantity;

// Ajouter l'entrée au fichier cart.txt
$file = fopen("cart.txt", "a");
fwrite($file, $cartEntry . PHP_EOL);
fclose($file);
// Rediriger vers la page d'accueil
 header("Location: index.html");
?>

