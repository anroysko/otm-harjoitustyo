# OTM-harjoitustyö
Sovellus on suppea kopio Amiga-pelistä Emerald Mine. Se on reaaliaikainen pulmapeli jossa kaivetaan kaksiulotteisessa ruutupohjaisessa luolassa timantteja, joita kerättyään tarpeeksi pitää keretä kentän uloskäynnille aikarajassa.

![Screenshot from Emerald mine](https://raw.githubusercontent.com/anroysko/otm-harjoitustyo/master/doc/game_screenshot.png)

Ohjelma on toteutettu C++:lla.

## Dokumentaatio
[Tuntikirjanpito.md](https://github.com/anroysko/otm-harjoitustyo/blob/master/doc/tuntikirjanpito.md)

[vaatimusmaarittely.md](https://github.com/anroysko/otm-harjoitustyo/blob/master/doc/vaatimusmaarittely.md)

[The link text](http://example.net/)

## Releaset
[Viikko 5](https://github.com/anroysko/otm-harjoitustyo/tree/0.1.0)


## Komentorivitoiminnot

Makefile kompiloi ohjelman käyttämällä compileria `g++-7`. Vastaavaa voi käyttää vaihtamalla Makefilessä `$(CC)`:n joksikin muuksi. Koodi käyttää kuitenkin c++17-ominaisuuksia, joten hyvin uusi compiler vaaditaan.

Ohjelman kompilointiin tarvitaan (ainakin) kirjastoja:

    libglfw3-dev libglew2.0 libgl1-mesa-dev

Testikattavuusraportin generointiin tarvitsee ohjelman `lcov`

Tyylin tarkistamiseen tarvitsee ohjelman `clang-format-3.9`. Eri versioita voi myös käyttää, mutta silloin Makefilessä pitää vaihtaa `$(clang_format_program)` haluttuun.

Ne voi asentaa aptista

### Testaus
Testit suoritetaan komennolla

    make test

Testikattavuusraportti luodaan komennolla

    make coverage

Kattavuusraporttia voi tarkastella avaamalla selaimella tiedosto `src/bin/cov/html/index.html`
tyylin tarkastuksen voi tehdä komennolla

    make checkstyle


### Ohjelman ajaminen
Ohjelman voi ajaa komennolla

    make run


### Binäärin generoiminen
Ajamalla komennon

    make

source-kansiossa, generoi Make executablen pathilla `src/target/bin/main`. Sen voi ajaa src-kansiossa komennolla `./target/bin/main` tai vaan käyttää `make run`-komentoa. Huomaa että koodi olettaa että se ajetaan src-kansiossa.

### Koodihakemiston puhdistaminen
src-folderin voi puhdistaa kompiloitaessa luotavista tiedostoista komennolla

    make clean

