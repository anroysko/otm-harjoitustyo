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
make coverage
```
Kattavuusraportin generoinnissa käytetään lcov:ia. Sen voi asentaa komennolla
```
sudo apt install lcov
```
Kattavuusraporttia voi tarkastella avaamalla selaimella tiedosto `src/bin/cov/html/index.html`
### Ohjelman ajaminen
Ohjelman voi ajaa komennolla
```
make run
```
### Koodihakemiston puhdistaminen
src-folderin voi puhdistaa kompiloitaessa luotavista tiedostoista komennolla
```
make clean
```
