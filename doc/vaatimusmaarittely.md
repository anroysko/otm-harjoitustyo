# Vaatimusmäärittely

## Sovelluksen tarkoitus
Sovellus on suppeampi versio Amiga-pelistä Emerald Mine. Siinä kaivetaan kaksiulotteisessa ruutupohjaisessa luolassa timantteja, joita kerättyään tarpeeksi pitää keretä kentän uloskäynnille aikarajassa. Peli on reaaliaikainen.

![Image from Emerald mine](https://www.emuparadise.me/GameBase%20Amiga/Screenshots/E/Emerald_Mine_2.png)

## Suunnitellut toiminnallisuudet

### Pelin mekaniikat

* Kuusi eri palikkatyyppiä:
  - seinä
  - hiekka
  - timantti
  - kivi
  - uloskäynti
  - tyhjä ruutu

* Jokaisessa kentässä on aikaraja. Kun ylittää aikarajan, häviää.

* Jos kivi tippuu pelaajan tai timantin päälle, pelaaja tai timantti tuhoutuu.

* Jos timantti tai kivi on pyöreän objektin päällä, se vierii satunnaiseen viereiseen tyhjään ruutuun, jonka alla on tyhjä ruutu. Timantit, kivet ja uloskäynnit ovat pyöreitä objekteja.

* Jos kiven tai timantin alla ei ole mitään, se tippuu.

* Kun pelaaja kävelee hiekan tai timantin yli, se poistuu. Jos se oli timantti, se keräytyy.

* Kun pelaaja kerää kentästä riippuvan määrän timantteja, uloskäynti aukeaa.

* Pelaaja voi vierittää kiven viereiseen tyhjään ruutuun kävellemällä sitä päin sivulta

### Main menu

Menussa voi valita pelattavan kentän listasta, tai poistua ohjelmasta.

Ohjelma lataa kentät automaattisesti. Ne tallennetaan tekstitiedostoina.

## Kehitysideat

* kenttäeditori

* musiikki

* replayt ja high scoret

* Lisää erilaisia blockeja
