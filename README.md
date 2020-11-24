# infrapuna-auto

Infrapunakauko-ohjaimella ohjattavan pikkuauton ohjelma. Arduino nano alustana.

Tarvittavat ominaisuudet:

    -Kaukosäätimen nappia painamalla muuttaa nopeutta. Voi olla vaikka MOVE-muuttuja, jonka arvoa muutetaan yksi per painallus. MOVE muuttujan arvot rajoittettu välille -3 -> +3.(miinus taaksepäin ja plussa eteenpäin) ***Pitää tarkistaa onko arduinon analog outputissa mahdollisuus laittaa eri voltages outputtiin
    
    -Kaukosäätimen sivunäppäimet muuttavat TURN-muuttujan arvoa yksi arvo per painallus. Tässä voisi vaihteluväli olla -2 -> +2. 
(miinus vaikka vasen ja plussa oikea) ***-||-


Optional features:

    -Kaukosäätimen nappien määrittelylle pieni ohjelma, joka tunnistaisi erilaisten kaukosäätimien outputit ja voisi reprogram as a new remote ohjain. 
    (Saattaa olla liian monimutkainen ja tavallaan turha ominaisuus)
    
    -Valojen vilkkuminen, sen voisi toteuttaa timer-osion avulla. Käynnistetään ja suljetaan samalla kaukosäätimen painikkeella. 
    (Vilkkuu sillee vähän niinku poliisiautojen vilkut u know)
    
    -Lähisensorin asennus auton eteen, joka timerin-avulla tarkastaa tietyn väliajoin onko auton edessä esteitä. Muuttaa liikkumismuuttujan nollaan, jos havaitsee esteen lähellä.
