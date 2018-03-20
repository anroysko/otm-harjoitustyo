package com.mycompany.unicafe;

import static org.junit.Assert.*;
import org.junit.Before;
import org.junit.Test;

public class KassapaateTest {
    Kassapaate paate;
    Maksukortti kortti;

    @Before
    public void setUp() {
        paate = new Kassapaate();
	kortti = new Maksukortti(0);
    }

    @Test
    public void createdPaateExists() {
	assertTrue(paate != null);
    }
    
    @Test
    public void intialValuesCorrect() {
        assertTrue(paate.kassassaRahaa() == 100000);      
        assertTrue(paate.maukkaitaLounaitaMyyty() == 0);      
        assertTrue(paate.edullisiaLounaitaMyyty() == 0);      
    }

    @Test
    public void cashPaymentWorks() {
	assertTrue(paate.syoEdullisesti(300) == 300-240);
	assertTrue(paate.syoEdullisesti(240) == 0);
	assertTrue(paate.syoEdullisesti(239) == 239);

	assertTrue(paate.syoMaukkaasti(500) == 500-400);
	assertTrue(paate.syoMaukkaasti(400) == 0);
	assertTrue(paate.syoMaukkaasti(400) == 0);
	assertTrue(paate.syoMaukkaasti(399) == 399);

	assertTrue(paate.kassassaRahaa() == 100000 + 2 * 240 + 3 * 400);
	assertTrue(paate.edullisiaLounaitaMyyty() == 2);
	assertTrue(paate.maukkaitaLounaitaMyyty() == 3);
    }

    @Test
    public void cardPaymentMaukkaastiWorks() {
	kortti.lataaRahaa(4 * 400 - 1);
	assertTrue(paate.syoMaukkaasti(kortti));
	assertTrue(paate.syoMaukkaasti(kortti));
	assertTrue(paate.syoMaukkaasti(kortti));
	assertTrue(!paate.syoMaukkaasti(kortti));
	assertTrue(kortti.saldo() == 400 - 1);

	assertTrue(paate.kassassaRahaa() == 100000);
	assertTrue(paate.edullisiaLounaitaMyyty() == 0);
	assertTrue(paate.maukkaitaLounaitaMyyty() == 3);
    }
    @Test
    public void cardPaymentEdullisestiWorks() {
	kortti.lataaRahaa(3 * 240 - 1);
	assertTrue(paate.syoEdullisesti(kortti));
	assertTrue(paate.syoEdullisesti(kortti));
	assertTrue(!paate.syoEdullisesti(kortti));
	assertTrue(kortti.saldo() == 240 - 1);

	assertTrue(paate.kassassaRahaa() == 100000);
	assertTrue(paate.edullisiaLounaitaMyyty() == 2);
	assertTrue(paate.maukkaitaLounaitaMyyty() == 0);
    }
    @Test
    public void chargeCardWorks() {
	paate.lataaRahaaKortille(kortti, 20);
	assertTrue(paate.kassassaRahaa() == 100000 + 20);
	assertTrue(kortti.saldo() == 20);
	
	paate.lataaRahaaKortille(kortti, 0);
	assertTrue(paate.kassassaRahaa() == 100000 + 20);
	assertTrue(kortti.saldo() == 20);
	
	paate.lataaRahaaKortille(kortti, -100);
	assertTrue(paate.kassassaRahaa() == 100000 + 20);
	assertTrue(kortti.saldo() == 20);
    }
}
