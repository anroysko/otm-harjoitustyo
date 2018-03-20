# Vaatimusmäärittely

## Sovelluksen tarkoitus
Sovellus on suppeampi versio amiga-pelistä emerald mine. Siinä kaivetaan 2d tile-based luolassa timantteja, joita kerättyään tarpeeksi pitää keretä kentän uloskäynnille aikarajassa.

![Image from Emerald mine](https://www.emuparadise.me/GameBase%20Amiga/Screenshots/E/Emerald_Mine_2.png)

## Suunnitellut toiminnallisuudet

### pelin mekaniikat

Kuusi eri block-tyyppiä:
- Seinä
- Hiekka
- Timantti
- Kivi
- Uloskäynti
- Tyhjä ruutu

* Jokaisessa kentässä on aikaraja. Kun ylittää aikarajan, häviää.

* Jos kivi tippuu pelaajan tai timantin päälle, pelaaja tai timantti tuhoutuu.

* Jos timantti tai kivi on pyöreän objektin päällä, se kaatuu satunnaiselle puolelle. Timantit, kivet ja uloskäynnit ovat pyöreitä objekteja.

* Jos kiven tai timantin alla ei ole mitään, se tippuu.

* Kun pelaaja kävelee hiekan tai timantin yli, se katoaa, ja jos se oli timantti, se keräytyy.

* Kun pelaaja kerää kentästä riippuvan määrän timantteja, uloskäynti aukeaa.

### Main menu

Menussa voi valita Kentän listasta, tai poistua ohjelmasta.

Ohjelma lataa kentät automaattisesti. Ne tallennetaan tekstitiedostoina.

## Kehitysideat

* Kenttäeditori

* Musiikki

* Replayt ja high scoret

* Lisää erilaisia blockeja
