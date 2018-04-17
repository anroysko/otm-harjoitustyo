# OTM-harjoitustyö
Sovellus on suppea kopio Amiga-pelistä Emerald Mine. Se on reaaliaikainen pulmapeli jossa kaivetaan kaksiulotteisessa ruutupohjaisessa luolassa timantteja, joita kerättyään tarpeeksi pitää keretä kentän uloskäynnille aikarajassa.

![Screenshot from Emerald mine](https://github.com/anroysko/otm-harjoitustyo/blob/master/doc/game_screenshot.png)


Ohjelma on toteutettu C++:lla.
## Dokumentaatio
[Tuntikirjanpito.md](https://github.com/anroysko/otm-harjoitustyo/blob/master/doc/tuntikirjanpito.md)

[vaatimusmaarittely.md](https://github.com/anroysko/otm-harjoitustyo/blob/master/doc/vaatimusmaarittely.md)

## Komentorivitoiminnot

Ohjelman kompilointiin tarvitaan (ainakin) kirjastoja:
```
libglfw3 libglew2.0 libgl1-mesa-dev
```
Testikattavuusraportin generointiin tarvitsee ohjelman `lcov`

Tyylin tarkistamiseen tarvitsee ohjelman `clang-format-3.9`. Eri versioita voi myös käyttää, mutta silloin Makefilessä pitää vaihtaa `$(clang_format_program)` haluttuun.

Ne voi asentaa aptista

### Testaus
Testit suoritetaan komennolla
```
make test
```
Testikattavuusraportti luodaan komennolla
```
make coverage
```
Kattavuusraporttia voi tarkastella avaamalla selaimella tiedosto `src/bin/cov/html/index.html`
tyylin tarkastuksen voi tehdä komennolla
```
make checkstyle
```
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
