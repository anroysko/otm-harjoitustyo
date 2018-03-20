
package com.mycompany.unicafe;

import static org.junit.Assert.*;
import org.junit.Before;
import org.junit.Test;

public class MaksukorttiTest {

    Maksukortti kortti;

    @Before
    public void setUp() {
        kortti = new Maksukortti(10);
    }

    @Test
    public void luotuKorttiOlemassa() {
        assertTrue(kortti!=null);      
    }
    
    @Test
    public void initialBalanceIsCorrect() {
        assertTrue(kortti.saldo() == 10);      
    }

    @Test
    public void addingBalanceWorks() {
        kortti.lataaRahaa(17);
	assertTrue(kortti.saldo() == 27);
    }

    @Test
    public void takingBalanceWorks() {
	assertTrue(kortti.otaRahaa(5));
	assertTrue(kortti.saldo() == 5);
	assertTrue(!kortti.otaRahaa(7));
	assertTrue(kortti.saldo() == 5);
	assertTrue(kortti.otaRahaa(5));
	assertTrue(kortti.saldo() == 0);

	kortti.lataaRahaa(200);
	assertTrue(kortti.saldo() == 200);
	assertTrue(kortti.otaRahaa(200));
	assertTrue(kortti.saldo() == 0);
	
	kortti.lataaRahaa(100);
	assertTrue(kortti.saldo() == 100);
	assertTrue(!kortti.otaRahaa(200));
	assertTrue(kortti.saldo() == 100);
    }
    
    @Test
    public void toStringWorks() {
	assertTrue(kortti.toString().equals("saldo: 0.10"));
	kortti.lataaRahaa(1100);
	assertTrue(kortti.toString().equals("saldo: 11.10"));
    }
}
