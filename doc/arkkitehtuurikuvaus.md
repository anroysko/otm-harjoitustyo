# Arkkitehtuurikuvaus

Sovellus koostuu grafiikkaa piirtävästä osasta `graphics`, peliä käsittelevästä osasta `game`, ja satunnaisia tarvittavia funktiota sisältävästä osasta `util`.

Peliä käsittelevästä osasta ainut ulospäin näkyvä asia on `Level` -luokka, joka kuvaa pelin nykyistä tilaa.

Grafiikkaosasta ulos näkyy `GraphicsState` -luokka, joka pitää yllä openGL-tilaa ja hoitaa piirtämisen.
