# OTM-harjoitustyö
Sovellus on suppea kopio Amiga-pelistä Emerald Mine. Se on reaaliaikainen pulmapeli jossa kaivetaan kaksiulotteisessa ruutupohjaisessa luolassa timantteja, joita kerättyään tarpeeksi pitää keretä kentän uloskäynnille aikarajassa.

![Screenshot from Emerald mine](https://raw.githubusercontent.com/anroysko/otm-harjoitustyo/master/doc/game_screenshot.png)

Ohjelma on toteutettu C++:lla.

## Dokumentaatio
[Tuntikirjanpito.md](https://github.com/anroysko/otm-harjoitustyo/blob/master/doc/tuntikirjanpito.md)

[vaatimusmaarittely.md](https://github.com/anroysko/otm-harjoitustyo/blob/master/doc/vaatimusmaarittely.md)

[ohje.md](https://github.com/anroysko/otm-harjoitustyo/blob/master/doc/ohje.md)

[arkkitehtuurikuvaus.md](https://github.com/anroysko/otm-harjoitustyo/blob/master/doc/arkkitehtuurikuvaus.md)

## Releaset
[Viikko 5](https://github.com/anroysko/otm-harjoitustyo/tree/0.1.0)

[Viikko 6](https://github.com/anroysko/otm-harjoitustyo/tree/0.2.0)

[Lopullinen](https://github.com/anroysko/otm-harjoitustyo/tree/1.1.0)

## Komentorivitoiminnot

Makefile kompiloi ohjelman käyttämällä compileria `g++-7`. Vastaavaa voi käyttää vaihtamalla `$(CC)` -muuttujan makefilessä. Koodi käyttää kuitenkin c++17-ominaisuuksia, joten hyvin uusi compiler vaaditaan.

Ohjelman kompilointiin tarvitaan (ainakin) kirjastoja:

    libglfw3-dev libglew-dev libgl1-mesa-dev

Testikattavuusraportin generointiin tarvitsee ohjelman `lcov`

Dokumentoinnin generointiin tarvitsee ohjelman `doxygen`

Tyylin tarkistamiseen tarvitsee ohjelman `clang-format-3.9`. Eri versioita voi myös käyttää, mutta silloin Makefilessä pitää vaihtaa `$(clang_format_program)` haluttuun.

Ne voi asentaa aptista

### Testaus
Testit suoritetaan komennolla

    make test

Testikattavuusraportti luodaan komennolla

    make coverage

Kattavuusraporttia voi tarkastella avaamalla selaimella tiedosto `src/target/cov/html/index.html`
tyylin tarkastuksen voi tehdä komennolla

    make checkstyle

Dokumentoinnin voi generoida komennolla

    make doc

Sitä voi tarkastella avaamalla selaimella tiedosto `src/target/doc/html/index.html`


### Ohjelman ajaminen
Ohjelman voi ajaa komennolla

    make run


### Binäärin generoiminen
Ajamalla komennon

    make

source-kansiossa, generoi make executablen pathilla `src/target/bin/main`. Sen voi ajaa src-kansiossa komennolla `./target/bin/main` tai vaan käyttää `make run`-komentoa. Huomaa että koodi olettaa että se ajetaan src-kansiossa.

### Koodihakemiston puhdistaminen
src-folderin voi puhdistaa kompiloitaessa luotavista tiedostoista komennolla

    make clean

