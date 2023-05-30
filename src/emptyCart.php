<?php
// Vider le contenu du fichier cart.txt
file_put_contents('cart.txt', '');

// Répondre avec un message de succès
echo "Le panier a été vidé avec succès.";

// Rediriger vers la page d'accueil
header("Location: index.html");
?>
