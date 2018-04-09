# OTM-harjoitustyö
Sovellus on suppeampi versio Amiga-pelistä Emerald Mine. Se on reaaliaikainen pulmapeli jossa kaivetaan kaksiulotteisessa ruutupohjaisessa luolassa timantteja, joita kerättyään tarpeeksi pitää keretä kentän uloskäynnille aikarajassa.

![Screenshot from Emerald mine](https://github.com/anroysko/otm-harjoitustyo/blob/master/doc/game_screenshot.png)


Ohjelma on toteutettu C++:lla.
## Dokumentaatio
[Tuntikirjanpito.md](https://github.com/anroysko/otm-harjoitustyo/blob/master/doc/tuntikirjanpito.md)

[vaatimusmaarittely.md](https://github.com/anroysko/otm-harjoitustyo/blob/master/doc/vaatimusmaarittely.md)

## Komentorivitoiminnot
### Testaus
Testit suoritetaan komennolla
```
make test
```
Testikattavuusraportti luodaan komennolla
```
(TODO: automaattinen testikattavuusraportin generoiminen)
```
Kattavuusraporttia voi tarkastella avaamalla selaimella tiedosto `(TODO: insert file)`
### Koodin ajaminen
Koodin voi ajaa komennolla
```
make run
```
### Kompilointitiedostojen poistaminen
src-folderin voi puhdistaa (poistaa compiloidessa luodut .o, .d, ja binääritiedostot) komennolla
```
make clean
```
