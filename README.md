### Specifikáció a Játékhoz

#### 1. Bevezetés

Ez a specifikáció egy farmos játék működését írja le, amelyet a **Stardew Valley** és a **Farmerama** ihletett. A játék célja, hogy a játékos egy farmot építsen és gondozzon, különféle növényeket ültetve, gondozva, majd betakarítva azokat. A játék egy rács alapú pályát használ, amelyen a játékos karaktere szabadon mozoghat, és különféle mezőgazdasági tevékenységeket végezhet, például ültetést, öntözést és betakarítást. A játékosnak lehetősége van felfedezni a pályát, bővíteni a farmot.

#### 2. Játékmenet

- A játékos a karaktert billentyűkkel mozgatja a pályán, így különböző csempéken végezhet feladatokat.
- A karakter körüli csempéken dolgozhat különböző eszközökkel, például ültethet, öntözhet, és betakaríthat.
- A pálya középpontjában mindig a karakter helyezkedik el, így a térkép automatikusan követi a mozgását.
- Két gombbal lehet zoomolni: egy gombbal nagyítani, egy másikkal kicsinyíteni, így a játékos különböző részletességgel tekintheti meg a pályát.
- A játékosnak különféle növények és termények állnak rendelkezésére, amelyeket fejleszthet és bővíthet a játék előrehaladtával.

#### 3. Játék leírása

- A játék egy farm környezetében játszódik, ahol a játékos a pályán mozgatva a karaktert különféle mezőgazdasági tevékenységeket végezhet.
- A nyílbillentyűkkel navigálva a pálya mindig követi a karaktert, így az mindig középen marad.
- A játékos különböző mezőgazdasági eszközöket választhat ki, például kapát, öntözőkannát vagy kaszát, amelyekkel interakcióba léphet a csempéken elhelyezkedő termőföldekkel és növényekkel.
- A játék folyamán a játékos új növényekhez és eszközökhöz férhet hozzá, amelyeket beültethet és karbantarthat.

#### 4. Felhasználói Interakciók

- **Karakter Mozgatása**: A játékos fel, le, balra és jobbra mozgathatja a karaktert a pályán billentyűk segítségével.
- **Zoom Szint Változtatása**: A felhasználói felületen két gomb található, amelyekkel a játékos növelheti vagy csökkentheti a zoom szintet.
- **Eszközök Használata**: A játékos különböző mezőgazdasági eszközöket választhat ki, majd a pálya adott csempéjére kattintva használhatja őket (pl. földművelés, ültetés, öntözés).
- **Növénytermesztés és Betakarítás**: A játékosnak lehetősége van különböző növényeket ültetni, amelyeket aztán gondozhat és betakaríthat, amint elérik az érettségi állapotukat.
- **Piac Használata**: A játékos eladhatja a betakarított növényeket a piacon, és bevételre tehet szert..

#### 5. Megjelenítés

- **Pálya**: A pálya egy rács alapú rendszer, ahol minden csempe egy adott mezőgazdasági funkciót vagy növényt képvisel. A csempék különböző színekkel és textúrákkal rendelkeznek, hogy megkülönböztethetőek legyenek a különféle földtípusok, például üres föld, ültetett növények, és betakarításra kész termények.
- **Karakter**: A karakter középen helyezkedik el, és mindig látható, ahogy dolgozik és mozog a pályán.
- **Eszközhasználat Jelzése**: A kijelölt eszköz ikonja megjelenik a képernyőn, jelezve, hogy a játékos éppen melyik eszközt használja.

#### 6. Növények és Növekedési Fázisok

- **Növények Ültetése**: A játékos különféle növényeket ültethet a megfelelő csempékre, például zöldségeket, gyümölcsöket vagy virágokat.
- **Növekedési Fázisok**: A növények több növekedési fázison mennek keresztül, és a játékos ezeket a fázisokat vizuálisan követheti a pályán. Minden fázis egy adott csempe textúrával jelenik meg, amely azt mutatja, hogy a növény éppen fejlődésben van, vagy betakarítható.
- **Betakarítás**: A növények betakaríthatók, amint elérik az érett állapotot. A betakarított növények eladhatók, amellyel további bevételhez jut a játékos.

#### 7. Bemenetek és Kimenetek

- **Bemenetek**:
  - Billentyűk a karakter mozgatásához.
  - Egérkattintás a zoom és eszköz gombok használatához.
  - Billentyűk az eszközök kiválasztásához és a növények ültetéséhez.
- **Kimenetek**:
  - A pálya és a karakter megjelenítése.
  - Az eszközhasználat és növekedési fázisok megjelenítése a pályán (pl. a csempe kinézetének változása).
  - A pálya illetve a karakter állapotának mentése fájlba

#### 8. Hibakezelés

- Hibás vagy érvénytelen bemenet esetén a program figyelmeztető üzenetet jelenít meg.
- Ha a karakter a pálya széléhez ér, a mozgás megáll abban az irányban, így nem léphet ki a játékos a kijelölt pálya területéről.

#### 9. Fájlformátumok

- A pálya állapota szöveges fájlban van tárolva, ahol minden sor egy sor csempét jelent.
- A fájlban az egyes csempéket számokkal jelöljük, és a számok szóközzel vannak elválasztva. Minden szám egy adott textúrát vagy csempetípust reprezentál, például 0 az üres földet, 1 az elültetett növényeket, 2 a fejlődő növényeket és 3 a betakarításra kész növényeket jelenti.

#### 10. Példa Fájlformátum

```
0 1 2 3
1 0 3 2
2 3 1 0
3 2 0 1
```

#### 11. Felhasználói Felület

- A felhasználói felület tartalmazza a pályát, a karaktert, a zoom gombokat, és az eszközválasztó ikont.
- A zoom gombok a képernyő bal felső sarkában találhatók: a zöld gomb nagyít, a piros gomb kicsinyít.
- Az eszközválasztó ikonok a képernyő tetején láthatók, így a játékos gyorsan kiválaszthatja a használni kívánt eszközt.
- A növények fejlődési fázisait az ikonok és a csempe színezése is megjeleníti.

#### 12. Pénzrendszer és Alapvető Piac

- **Pénzrendszer**: A játékosnak kezdetben egy alap pénzösszege van, amelyet a termények eladásával növelhet.
- **Piac**: A pályán elhelyezkedik egy piac, ahol a játékos a betakarított növényeket eladhatja, és így pénzt szerezhet. A bevételből új eszközöket, különleges magokat vagy növekedési gyorsítókat vásárolhat, amelyekkel hatékonyabb lehet a gazdálkodásban.
