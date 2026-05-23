# 📊 Arányosító és Készlet-Optimalizáló Alkalmazás

Egy intelligens, kétlépcsős erőforrás-optimalizáló szoftver **C++ (Qt 6)** alapokon. Az alkalmazás két fő funkcionális modullal rendelkezik: egy professzionális mezőgazdasági takarmánykeverék-tervezővel (Daráló) és egy háztartási/gasztronómiai adag-optimalizálóval (Konyha).

## 🚀 Főbb Funkciók és Üzleti Logika

### 🌾 1. Takarmány-optimalizáló (Daráló)
* **Kétlépcsős matematika:** A szoftver a kért össztömeg alapján arányosan osztja szét a komponenseket a rendelkezésre álló raktárkészlet szerint.
* **Biológiai / Egészségügyi korlátozás:** Az alapanyagokhoz maximális megengedett százalékos korlát rendelhető (pl. a takarmány nem tartalmazhat 30%-nál több árpát). Ha a kezdeti eloszlás ezt túllépné, a motor lezárja az adott komponenst a határértéken, a maradék tömeget pedig intelligensen szétosztja a többi szabad anyagon.

### 🍳 2. Recept-skálázó (Konyha)
* **Daráló-elvű készletkorlát:** Nem egyszerű receptszorzást végez, hanem a hűtőben/kamrában lévő valós raktárkészletet veszi figyelembe a személyek száma alapján. Ha a kívánt adaghoz elfogyna a tej vagy a gyümölcs, a program lekorlátozza azt a meglévő maximumra, és a többi alapanyag növelésével kompenzál.
* **Intelligens folyadék-szűrés:** Felismeri a folyadékokat a megnevezésük alapján (tej, víz, olaj, joghurt), és a mennyiség függvényében automatikusan vált a mértékegységek között (0.2 liter alatt `ml`, felette `l`, míg a szilárd anyagoknál megmarad a `kg`).

### 🛡️ 3. Input Validáció (Golyóálló Szűrő)
* Mindkét modul rendelkezik egy beépített tiltólistával, amely pontos string-egyezés alapján kiszűri az oda nem illő, nem darálható vagy nem ehető tételeket (pl. *vas, zokni, karton*), és erről hibaablakban értesíti a felhasználót.
* Automatikus adat-reset a főoldalra való visszalépéskor a memóriabiztonság érdekében.

## 📂 Architektúra és Tiszta Kód (Modularizáció)

A szoftver fejlesztése során kiemelt szempont volt a **Spagetti-kód elkerülése** és az **SRP (Single Responsibility Principle)** betartása. A nehéz matematikai és üzleti logika teljesen el lett választva a felhasználói felülettől:

* `mainwindow.cpp / .h` - Kizárólag a GUI események kezeléséért, a beolvasásért és a táblázatos megjelenítésért felel.
* `KonyhaMotor.cpp / .h` - A konyhai optimalizálási algoritmust, a folyadékszűrést és a tiltólistát tartalmazó zárt háttérmotor.
* `DaraloMotor.cpp / .h` - A takarmány-arányosításért és a százalékos egészségügyi korlátok betartásáért felelős matematikai motor.

## 🛠️ Telepítés és Futtatás

1. Telepítsd a **Qt Creator** fejlesztőkörnyezetet (Qt 6.x verzió ajánlott).
2. Klónozd vagy töltsd le ezt a repository-t.
3. Nyisd meg az `AranyositoApp.pro` fájlt a Qt Creatorban.
4. Futtasd a **`Build -> Run qmake`** parancsot.
5. Kattints a **Run (Zöld nyíl)** gombra a fordításhoz és indításhoz.