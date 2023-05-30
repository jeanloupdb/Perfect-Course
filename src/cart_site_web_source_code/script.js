// Fonction pour afficher le formulaire de quantité à côté du bouton "Ajouter au panier"
function showQuantityForm(event) {
    var productReference = event.target.getAttribute("data-reference");
    var addButton = event.target;
    var listItem = addButton.parentNode;
    var quantityForm = document.getElementById("quantityForm");
    var quantityInput = document.getElementById("quantityInput");

    // cacher le bouton "Ajouter au panier"
    addButton.style.visibility = "hidden";

    // Modifier l'action du formulaire pour inclure la référence du produit
    quantityForm.action = "addToCart.php?productReference=" + productReference;

    // Positionner le formulaire de quantité à côté du bouton
    listItem.insertBefore(quantityForm, addButton.nextSibling);

    // Afficher le formulaire et mettre le focus sur le champ de quantité
    quantityForm.style.display = "inline-block";
    quantityInput.focus();
}





// Fonction pour ajouter un produit au panier avec la quantité spécifiée
function addToCart(event) {
    event.preventDefault(); // Empêcher la soumission du formulaire
    var productReference = event.target.getAttribute("data-reference");
    var quantity = document.getElementById("quantityInput").value;

    var xhr = new XMLHttpRequest();
    xhr.open("POST", "addToCart.php", true);
    xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    xhr.onreadystatechange = function() {
        if (xhr.readyState === 4 && xhr.status === 200) {
            console.log("Le produit a été ajouté au panier avec succès.");
            highlightSelectedProducts(); // Mettre à jour le style des produits sélectionnés
            filterProducts(); // Mettre à jour l'affichage des produits filtrés si nécessaire
        }
    };
    xhr.send("productReference=" + productReference + "&quantity=" + quantity);
    return false; // Empêcher la rechargement de la page
}


// Fonction pour mettre en évidence les produits sélectionnés
function highlightSelectedProducts() {
    var cartFile = "cart.txt";
    var productList = document.getElementById("productList");
    var cartItems = document.getElementById("cartItems");

    // Effacer les anciens produits du panier
    cartItems.innerHTML = "";

    // Récupérer les références et quantités du fichier cart.txt
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function() {
        if (xhr.readyState === 4 && xhr.status === 200) {
            var cartLines = xhr.responseText.split("\n");
            cartLines.forEach(function(cartLine) {
                var [reference, quantity] = cartLine.split(" ");
                var productItem = productList.querySelector(
                    '[data-reference="' + reference + '"]'
                );
                //remplacer "ajouter au panier" par quantity
                productItem.textContent = quantity;
                if (productItem) {
                    productItem.parentElement.classList.add("selection");

                    // Ajouter le produit au panier
                    var cartItem = document.createElement("li");
                    cartItem.textContent = productItem.parentElement.textContent;
                    //supprimer les chiffres entre 0 et 9 dans le nom du produit
                    cartItem.textContent = cartItem.textContent.replace(/[0-9]/g, '');

                    cartItem.textContent += " (" + quantity + ")";
                    cartItems.appendChild(cartItem);
                }
            });
        }
    };
    xhr.open("GET", cartFile, true);
    xhr.send();
}





// Fonction pour récupérer le contenu du fichier catalogue.txt
function getCatalogue() {
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function() {
        if (xhr.readyState === 4 && xhr.status === 200) {
            var catalogue = xhr.responseText;
            displayCatalogue(catalogue);
        }
    };
    xhr.open("GET", "catalogue.txt", true);
    xhr.send();
    highlightSelectedProducts();
}

// Fonction pour afficher le contenu du catalogue dans la page
function displayCatalogue(catalogue, searchQuery) {
    var productList = document.getElementById("productList");
    var lines = catalogue.split("\n");

    for (var i = 0; i < lines.length; i++) {
        var line = lines[i].trim();
        if (line !== "") {
            var values = line.split(" ");
            var productReference = values[0];
            var productName = values[1];

            // Vérifier si le produit correspond à la recherche
            if (!searchQuery || productName.toLowerCase().includes(searchQuery.toLowerCase())) {
                var listItem = document.createElement("li");
                listItem.textContent = productName;

                var addButton = document.createElement("button");
                addButton.textContent = "Ajouter au panier";
                addButton.setAttribute("data-reference", productReference);
                addButton.addEventListener("click", showQuantityForm);

                listItem.appendChild(addButton);
                productList.appendChild(listItem);
            }
        }
    }
}


// Appeler la fonction pour récupérer le contenu du catalogue au chargement de la page
window.onload = getCatalogue;

// Fonction pour filtrer les produits en fonction de la recherche
function filterProducts() {
    var searchInput = document.getElementById("searchInput");
    var productList = document.getElementById("productList");
    var filter = searchInput.value.toLowerCase();

    var listItems = productList.children;

    for (var i = 0; i < listItems.length; i++) {
        var listItem = listItems[i];
        var productName = listItem.textContent.toLowerCase();

        if (productName.indexOf(filter) > -1) {
            listItem.style.visibility = "visible";
            listItem.classList.remove("filtered");
        } else {
            listItem.style.visibility = "hidden";
            listItem.classList.add("filtered");
        }
    }
}




// Ajouter un écouteur d'événement pour détecter les changements dans le champ de recherche
var searchInput = document.getElementById("searchInput");
searchInput.addEventListener("input", filterProducts);


// Fonction pour vider le panier
function emptyCart() {
    // Supprimer tous les éléments de la liste du panier
    var cartItems = document.getElementById("cartItems");
    cartItems.innerHTML = "";

    // Appeler le script PHP pour vider le fichier cart.txt
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "emptyCart.php", true);
    xhr.onreadystatechange = function() {
        if (xhr.readyState === 4 && xhr.status === 200) {
            console.log("Le panier a été vidé avec succès.");
        }
    };
    xhr.send();
}

// Ajouter un écouteur d'événement au bouton "Vider le panier"
var emptyCartButton = document.getElementById("emptyCartButton");
emptyCartButton.addEventListener("click", emptyCart);