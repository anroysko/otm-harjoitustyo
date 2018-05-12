# Käyttöohje

Ohjelman voi ajaa komennolla `make run` src-kansiossa.

Kun ohjelman ajaa, se avaa ensin kentänvalintavalikon. Oletuksena se avataan kansioon `assets/`.

Jos käyttäjä ei valitse mitään, ohjelma sulkeutuu. Kun käyttäjä valitsee yhden kentistä, peli alkaa. Jos käyttäjä avaa replay-tiedoston, se ajetaan.

Ikkunan voi sulkea painamalla `esc` tai ruksaamalla sen. Ikkuna sulkeutuu myös kun kenttä on läpäisty tai hävitty.

Kun kaikki hahmot ovat päässeet maaliin, tai jokin hahmo on menetetty, ikkuna suljetaan, ja käyttäjälle ilmoitetaan kuinka paljon aikaa kului ja kuinka monta smaragdia saatiin kerättyä.

Jos käyttäjä pääsi kentän läpi, häneltä kysytään mihin replay halutaan tallentaa.

## Pelin toiminta
### Kontrollit
Hahmoa (tai kentästä riippuen hahmoja) ohjataan nuolinäppäimillä. Jos siirron aikana pitää vasenta shift-näppäintä pohjassa, sijoittaa hahmo dynamiitin jos mahdollista. Jos siirron aikana pitää oikeaa control-näppäintä pohjassa, hahmo kerää suunnassa olevassa ruudussa olevan asian, siirtymättä itse siihen.
### Update-vaihe
Jokaisella tapahtumalla, kuten kiven tippumisella ja pelaajan liikkumisella on prioriteetti. Näin saavutetaan usean objektin samanaikainen liikkuminen ilman että sallitaan pelaajan kulkea seinien läpi tai muuta vastaavaa.
Eri tapahtumien prioriteetit (Pienin tapahtuu ensimmäisenä)
1. Ylöspäin liikkuminen
2. Vasemmalle ja oikealle liikkuminen ja työntäminen.
3. Alaspäin liikkuminen ja tippuminen.
4. Sivulle liukuminen.
5. Sivulta kerääminen.
6. Räjähtäminen
Saman prioriteetin tapahtumista tapahtuu ensin sen objektin tapahtuma, joka sijaitsee ensisijaisesti niin alhaalla, ja toissijaisesti niin oikealla kuin mahdollista.
### Palikkatyypit
- Hiekka. Se on staattinen kerättävä esine, jonka kerääminen ei tee mitään muuta kuin vapauttaa ruudun.
- Seinä. Seinä on staattinen, mahdollisesti pyöristetty ja/tai huonokuntoinen. Huonokuntoiset seinät voi räjäyttää, ja pyöreiden seinien päällä olevat epästaattiset objektit voivat liukua sivuun.
- Kivi. Painovoima vaikuttaa kiviin, ja jos kiven alla on tyhjää, se putoaa alaspäin. Jos hahmo jää kiven alle, se tuhoutuu. Kiveä pystyy myös työntämään, kunhan se ei ole putoamassa ja sitä ollaan työntämässä tyhjään ruutuun. Koska kivi on erityisen kova, se pystyy myös murskaamaan smaragdeja.
- Smaragdi. Smaragdit toimivat kuten kivet (eli niiden alle ei kannata jäädä!), mutta sen sijaan että niitä voisi työntää, niitä pystyy keräämään. Smaragdin keräämisestä saa pisteen, joita vaaditaan jokin määrä kentän läpäisyä varten.
- Pommi. Pommi on kuten kivi, mutta jos se aloitettuaan putoamisen lopettaa sen, tai jos jokin tippuu sen päälle, se räjähtää. Räjähdys korvaa 3x3-ruudukossa pommin ympärillä kaikki palikat jotka eivät kestä räjähdyksiä tyhjällä. Räjähdys voi myös räjäyttää muita pommeja tai dynamiitteja.
- Pussi. Pussi on fysiikoiltaan samanlainen kuin smaragdi, mutta ei ole kerättävä. Sen sisällä on kuitenkin smaragdi, joksi se muuttuu kun pussin päälle tiputtaa kiven.
- Dynamiitti. Dynamiitti on staattinen kerättävä objekti, jonka voi kerättyään sijoittaa. Sijoitetussa dynamiitissa on kahdeksan askeleen ajastin, jonka jälkeen se räjähtää pommin tavoin 3x3 ruudukossa itsensä ympärillä.
- Maali. Maali avautuu kun tarpeeksi timantteja on kerätty, ja sulkeutuu kun hahmo menee sinne sisään. Jos kentässä on useampi hahmo, niiden täytyy poistua eri maaleista.
- Pelaaja. Jos Hahmoja on useampi, ne kaikki tekevät juuri mitä inputtia on annettu. Dynamiitit ovat hahmokohtaisia.
