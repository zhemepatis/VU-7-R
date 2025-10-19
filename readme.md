# Apsaugos nuo dujų pratekėjimo sistemos prototipas

## Užduotis
Sukurti sistemą aptinkančią pavojingą  dujų lygmenį ir paleidžiančią įspėjamąjį signalą bei įjungiantį specialaus atsako sistemą. Šiuo atveju specialus atsako sistema yra ventiliacija. 

## Komponentai
Šiame darbe buvo panaudoti:
- Arduino UNO R3
- Dujų aptikimo sensorius
- Pjezo-elektrinis garsiakalbis
- Nekintamos srovės motoras
- Mygtukas
- Rezistoriai
- Laidai

## Prototipo schema
Nuorodos į sukurto prototipo schemas:
- [Prototipo jungimas TinkerCAD aplinkoje](img/arduino_gas_detector_circuit.png) 
- [Prototipo schema](img/arduino_gas_detector_schema.png)

## Tolimesni patobulinimai

### Sensorių pritaikymas realioms sąlygoms
Šiuo metu yra naudojami TinkerCAD suteikiami sensoriai nepritaikyti konkrečiai dujų rūšiai. Pritaikant šį prototipą realiame gyvenime, reikėtų, vietoj turimo abstraktaus dujų sensoriaus, **pridėti norimam panaudojimo atvejui tinkamą dujų sensorių**. Be to, norint užtikrinti, kad sistema būtų tinkama praktiniam naudojimui, reikėtų **pagerinti matavimo tikslumą ir patikimumą**.

### Pažangios ventiliavimo ir įspėjimo signalo sistemos
Tikėtina, kad realiam sistemos panaudojimui neužteks vieno motoro ir pjezo-elektrinio garsiakalbio. Siekiant užtikrinti patalpų saugumą nuo dujų pratekėjimo, reikėtų **įdiegti pažengesnes ventiliavimo**, galinčias išleisti dujas iš didesnių patalpų, bei **patobulinti pavojaus signalo sistemą**, užtikrinančią, kad signalas būtų girdimas esant didesniam atstumui.

### Išmaniosios funkcijos
Nesant galimybės pasiekti sistemos fiziškai, yra paranku, kai sistema gali būti nuotoliu. Taigi norint toliau vystyti šį projektą, galima būtų **sukurti mobilią programėlę, leidžiančią išjungti alarmą bei aktyvuoti ar išjungti ventiliaciją**. Taip pat, kadangi nesant fiziškai patalpose nėra galimybės sužinoti dujų pratekėjimo statuso, galima būtų **įdiegti pranešimų siuntimą į išmanųjį telefoną avarinės situacijos metu**.
