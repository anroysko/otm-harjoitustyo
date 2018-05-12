# Arkkitehtuurikuvaus

Sovellus koostuu grafiikkaa piirtävästä osasta `graphics`, peliä käsittelevästä osasta `game`, ja satunnaisia tarvittavia funktiota sisältävästä osasta `util`.

Peliä käsittelevästä osasta ainut ulospäin näkyvä asia on `Level` -luokka, joka kuvaa pelin nykyistä tilaa.

Grafiikkaosasta ulos näkyy `GraphicsState` -luokka, joka pitää yllä openGL-tilaa ja hoitaa piirtämisen.

Peliloopissa:
1. Kysytään siirto GraphcisStatelta.
2. Updatetaan levelin state. Samalla saadaan tieto siitä mitä pitää piirtää.
3. Piirretään tätä kunnes on aika tehdä seuraava siirto.
